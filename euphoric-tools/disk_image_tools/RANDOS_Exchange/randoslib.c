#include "mfmlib.h"
#include "randoslib.h"
#include <stdio.h>

static struct {
	unsigned char system[40];
/*
	char type;
	sectnumber currentdirsect;
	sectnumber execdirsect;
	sectnumber rootdirsect;
	unsigned int serialno;
	char doublesided;
	char execdirname[6];
	char tracks;
	char reserved[21];
*/
	unsigned char fat[512*13-40];	/* biggest Randos filesystem has a 13-sectors FAT */
} sys_info;
static int fatsectors, tracks, doublesided;
static sectnumber rootdirsect;
static int fatdirty;
static struct dir_entry dir[1024];		/* should be sufficient for holding the biggest directories */
static int dir_entries, dirdirty;
static int current_entry;



/******************************************************************************************/
/*  reading and writing linear sectors                                                    */

static int read_lin_sect(char *sectbuf,sectnumber sector)
{
	int track=sector/9, head=0;
	if (track>=tracks && doublesided) {
		track-=tracks;
		head=1;
	}
	if (track>=tracks) return INVALID_SECTOR_NUMBER;
	return read_sector(sectbuf,track,head,sector%9+1);
}

static int write_lin_sect(char *sectbuf, sectnumber sector)
{
	int track=sector/9, head=0;
	if (track>=tracks && doublesided) {
		track-=tracks;
		head=1;
	}
	if (track>=tracks) return INVALID_SECTOR_NUMBER;
	return write_sector(sectbuf,track,head,sector%9+1);
}

/*****************************************************************************************/
/* handling the FAT                                                                      */


static sectnumber getlink(sectnumber sector)
{
	if (sector%2==0)
		return sys_info.fat[sector*3/2] + ((sys_info.fat[sector*3/2+1] & 0xF0) <<4);
	else
		return ((sys_info.fat[sector*3/2] & 0x0F) << 8) + sys_info.fat[sector*3/2+1];
}

static void setlink(sectnumber sector, sectnumber link)
{
	if (sector%2==0) {
		sys_info.fat[sector*3/2]= link & 0xFF;
		sys_info.fat[sector*3/2+1]= sys_info.fat[sector*3/2+1] & 0x0F | ((link>>4) & 0xF0);
	} else {
		sys_info.fat[sector*3/2]= sys_info.fat[sector*3/2] & 0xF0 | (link>>8);
		sys_info.fat[sector*3/2+1]= link & 0xFF;
	}
	fatdirty=1;
}

static int find_free_sector()
{
	int sect;
	for (sect=0; getlink(sect)!=0xFFA; sect++)
		if (getlink(sect)==0xFFF) {
			setlink(sect,0xFFE);
			return sect;
		}
	return -1;
}

static void free_sect_chain(sectnumber sect)
{
	while (sect!=0xFFE) {
		int prev=sect;
		sect=getlink(sect);
		setlink(prev,0xFFF);
	}
}

static int read_sys_info()
{
	int sect;
	int err=read_lin_sect((char *)&sys_info,0xA2);	/* system sector is always at 0xA2 */
	if (err) return CANNOT_READ_SYSTEM_SECTOR;
	rootdirsect=sys_info.system[5]+256*sys_info.system[6];
	doublesided=(sys_info.system[11]==0xFE);
	tracks=sys_info.system[18];
	fatsectors=1;
	for (sect=0xA3; getlink(sect)==0xFFD; sect++) {
		err=read_lin_sect(((char *)&sys_info)+(sect-0xA2)*512,sect);
		if (err) return CANNOT_READ_FAT_SECTOR;
		fatsectors++;
	}
	return OK;
}

static int flush_sys_info()
{
	int i;
	for (i=0;i<fatsectors;i++)
		if (write_lin_sect(((char *)&sys_info)+i*512,i+0xA2))
			return CANNOT_WRITE_FAT_SECTOR;
	return OK;
}


/*************************************************************************************/
/* handling directories                                                              */

static int getsize(unsigned char size[3])
{
	return size[0]+256*(size[1]+256*size[2]);
}

static void setsize(unsigned char size[3],int n)
{
	size[0] = n & 0xFF;
	size[1] = (n >> 8) & 0xFF;
	size[2] = n >> 16;
}

static int read_dir(sectnumber dirsect)
{
	int dirsectors,err=read_lin_sect((char *)dir,dirsect);
	int n,entry;
	if (err) return CANNOT_READ_DIR_SECTOR;
	dirsectors=(dir[0].size[1]+ (dir[0].size[2]<<8))/2;
	dir_entries=dirsectors*32;
	for (n=32; n<dir_entries; n+=32) {
		dirsect=getlink(dirsect);
		if (dirsect==0xFFE) return TRUNCATED_FILE;
		err=read_lin_sect((char*)&dir[n],dirsect);
	}
	return 0;
}

static int flush_dir()
{
	int dirsect,err;
	int n,entry;

	dirsect=dir[0].first_sect;
	while (dirsect!=0xFFE) {
		err=write_lin_sect((char *)dir,dirsect);
		if (err) return CANNOT_WRITE_DIR_SECTOR;
		dirsect=getlink(dirsect);
	}
	return 0;
}

static int find_free_entry()
{
	int n;
	for (n=0;n<dir_entries;n++)
		if (dir[n].name[0]==-1)
			return n;
	return 0;
}

static int create_new_entry()
{
	return find_free_entry();	/* il faudrait agrandir le répertoire !! */
}

static int find_next_entry(char pattern[9])
{
	current_entry++;
	while (current_entry<dir_entries) {
		if (dir[current_entry].name[0]!= -1) {
			int i, match=1;
			for (i=0;i<6;i++)
				if (pattern[i]!='?' && dir[current_entry].name[i]!=pattern[i])
					match=0;
			for (i=0;i<3;i++)
				if (pattern[i+6]!='?' && dir[current_entry].ext[i]!=pattern[i+6])
					match=0;
			if (match)
				return current_entry;
		}
		current_entry++;
	}
	return 0;
}

static int find_first_entry(char pattern[9])
{
	current_entry=0;
	return find_next_entry(pattern);
}

static void print_entry(int n)
{
	int i;
	for (i=0;i<6;i++) putchar(dir[n].name[i]);
	putchar('.');
	for (i=0;i<3;i++) putchar(dir[n].ext[i]);
	printf("\t flags %02X size %d\n",dir[n].flags,getsize(dir[n].size));
}

static void reducefilename(char *filename, char *name)
{
	int i;
	for (i=0;i<9;i++)
		name[i]=' ';
	name[9]=0;
	for (i=0; filename[i]!='.' && filename[i]; i++)
		if (i<6) name[i]=filename[i];
	if (filename[i]=='.') {
		int j=6;
		for (i=i+1; filename[i]; i++)
			if (j<9) name[j++]=filename[i];
	}
}
	
void print_dir()
{
	int n=find_first_entry("?????????");
	while (n) {
		print_entry(n);
		n=find_next_entry("?????????");
	}
}

/*****************************************************************************************/
/* High-level operations                                                                 */


int open_randos_disk(char *name)
{
	int err=open_disk(name);
	if (err) return err;
	tracks=40; doublesided=0;
	err=read_sys_info();
	if (err) return err;
	return read_dir(rootdirsect);
}

int close_randos_disk()
{
	if (dirdirty) flush_dir();
	if (fatdirty) flush_sys_info();
	close_disk();
}


int load(char *name)
{
	FILE *hostfile=fopen(name,"wb");
	int n;
	char buf[512];
	char pattern[10];
	int size,sect,err;

	reducefilename(name,pattern);
	n=find_first_entry(pattern);
	if (hostfile==NULL) return CANNOT_CREATE_HOST_FILE;
	if (n==0) return FILE_NOT_FOUND;
	size=getsize(dir[n].size);
	sect=dir[n].first_sect;
	while (size>0) {
		if (sect>=0xFF8) return TRUNCATED_FILE;
		err=read_lin_sect(buf,sect);
		if (err) return err;
		if (fwrite(buf,size<512?size:512,1,hostfile)!=1)
			return CANNOT_WRITE_HOST_FILE;
		size-=512;
		sect=getlink(sect);
	}
	fclose(hostfile);
	return OK;
}

int delete(char *name)
{
	char pattern[10];
	int n;
	reducefilename(name,pattern);
	n=find_first_entry(pattern);
	if (n==0) return FILE_NOT_FOUND;
	dir[n].name[0]=0xFF;
	free_sect_chain(dir[n].first_sect);
	return OK;
}

int save(char *name)
{
	FILE *hostfile; 
	int n,read,size,sect,prev,err,i;
	char pattern[10];
	char buf[512];

	delete(name);
	n=create_new_entry();
	if (n==0)
		return NO_SPACE_LEFT;
	dir[n].first_sect=prev=find_free_sector();
	if (prev==-1)		
		return NO_SPACE_LEFT;
	hostfile=fopen(name,"rb");
	if (hostfile==NULL) return CANNOT_OPEN_HOST_FILE;
	read=fread(buf,1,512,hostfile);
	size=read;
	err=write_lin_sect(buf,prev);
	if (err) {
		fclose(hostfile);
		return err;
	}
	while (read=fread(buf,1,512,hostfile)) {
		size+=read;
		sect=find_free_sector();
		if (sect==-1) {
			fclose(hostfile);
			free_sect_chain(dir[n].first_sect);
			return NO_SPACE_LEFT;
		}
		err=write_lin_sect(buf,sect);
		setlink(prev,sect);
		prev=sect;
	}
	fclose(hostfile);
	reducefilename(name,pattern);
	for (i=0;i<9;i++) dir[n].name[i]=pattern[i];
	dir[n].flags=0x28; /* signification des bits ?? */
	setsize(dir[n].size,size);
	return OK;
}

