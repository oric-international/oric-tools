#include <dpmi.h>
#include <go32.h>
#include <stdio.h>

/* Caution: this programs replaces <CR><LF> sequences by <LF> chars !!
 * And the bug workaround below doesn't seem to work...
 */

char *version="Writedsk 2.2";
unsigned char track[2][82][6400];
unsigned char sector[1024];
unsigned char sector_to_write[1500];
int chrntab[40];
int sectsize[40];
int ptr;
int drive;
int adr;
int biosparams=0x522;
_go32_dpmi_registers r;

unsigned int search_id(int cyl,int head)
{
	for(;ptr<6400;ptr++)
		if ( *(unsigned *)(&track[head][cyl][ptr])==0
		&& *(unsigned *)(&track[head][cyl][ptr+4])==0xFEA1A1A1) {
			ptr+=8;
			return *(unsigned *)(&track[head][cyl][ptr]);
		}
	return 0;
}

void find_data(int cyl,int head)
{
	for(;ptr<6400;ptr++)
		if ( *(unsigned *)(&track[head][cyl][ptr])==0xFBA1A1A1) {
			ptr+=4;
			return;
		}
}

/* Bug workaround: 
  <CR><LF> is replaced by <LF> so we prepend a <CR> to all <CR><LF> sequences
 */
void transform_sector(char *buf, char *sector, int size)
{
	int i,j;
	for (i=0, j=0; i<size; i++,j++) {
		if (buf[i]==0x0D && i+1<size && buf[i+1]==0x0A) {
			printf("CR added \n");
			sector[j++]=0x0D;
		}
		sector[j]=buf[i];
	}
}


int read_sector(char *buf,int chrn,int track,int head)
{
	int try, size= ((char *)&chrn)[3];
	dosmemput(&size,1,biosparams+3);
	for(try=0;try<3;try++) {
		r.x.ax=0x0201; r.h.ch=track;
		r.h.cl=((char *)&chrn)[2];
		r.h.dh=head; r.h.dl=drive;
		r.x.es=adr>>4; r.x.bx=adr&0xF;
		_go32_dpmi_simulate_int(0x13,&r);
		if (r.h.ah==0) {
			dosmemget(adr,128<<size,buf);
			return 0;
		}
		r.x.ax=0;
		_go32_dpmi_simulate_int(0x13,&r);
	}
	return -1;
}

int write_sector(char *buf,int chrn,int track,int head)
{
	int try, size= ((char *)&chrn)[3];
	dosmemput(&size,1,biosparams+3);
	transform_sector(buf,sector_to_write,128<<size);
	dosmemput(sector_to_write,128<<size,adr);
	for(try=0;try<3;try++) {
		r.x.ax=0x0301; r.h.ch=track;
		r.h.cl=((char *)&chrn)[2];
		r.h.dh=head; r.h.dl=drive;
		r.x.es=adr>>4; r.x.bx=adr&0xF;
		_go32_dpmi_simulate_int(0x13,&r);
		if (r.h.ah==0) return 0;
		r.x.ax=0;
		_go32_dpmi_simulate_int(0x13,&r);
	}
	return -1;
}

int format_track(char *chrnlist,int cyl,int head,int sectors)
{
	int try;
	char gap=40;
	dosmemput(&gap,1,biosparams+7);
	dosmemput(&sectors,1,biosparams+4);
	dosmemput(chrnlist,sectors*4,adr);
	for (try=0;try<3;try++) {
		r.x.bx=adr&0xF; r.x.es=adr>>4;
		r.h.ch=cyl; r.h.cl=1; r.h.dh=head; r.h.dl=drive;
		r.h.ah=5; r.h.al=sectors;
		_go32_dpmi_simulate_int(0x13,&r);
		if (r.h.ah==0) return 0;
		r.x.ax=0;
		_go32_dpmi_simulate_int(0x13,&r);
	}
	return -1;
}

write_track(int cyl,int head)
{
	int sectors=0, errors, i, try;
	putchar(' '); fflush(stdout);
	ptr=0;
	while((chrntab[sectors]=search_id(cyl,head))!=0) {
		find_data(cyl,head);
		sectsize[sectors]= 128 << ((char *)(chrntab+sectors))[3];
		ptr+=sectsize[sectors];
		sectors++;
	}
	for(try=0;try<3;try++) {
		putchar('\b'); 	putchar('W'); fflush(stdout);
		ptr=0; errors=0;
		for(i=0;i<sectors;i++) {
			search_id(cyl,head); find_data(cyl,head);
			if (write_sector(&track[head][cyl][ptr],chrntab[i],cyl,head)!=0) {
				errors++;
				break;
			}
		}
		if (errors==0) {
			putchar('\b'); putchar('R'); fflush(stdout);
			ptr=0;
			for (i=0;i<sectors;i++) {
				search_id(cyl,head); find_data(cyl,head);
				if (read_sector(sector,chrntab[i],cyl,head)!=0)
					errors++;
				if (memcmp(sector,&track[head][cyl][ptr],sectsize[i])!=0)
					errors++;
			}
			if (errors==0) {
				putchar('\b'); putchar('.'); fflush(stdout);
				return 0;
			}
		} else {
			putchar('\b'); putchar('F'); fflush(stdout);
			format_track((char *)chrntab,cyl,head,sectors);
		}
	}
	i=2;
	dosmemput(&i,1,biosparams+3);
	printf("\nWrite error\n");
	exit(1);
}

unsigned char params[11];
int adr_params;

read_drive_params()
{
	adr=_go32_info_block.linear_address_of_transfer_buffer & 0xFFFFF;
	r.h.ah=8; r.h.dl=drive;
	_go32_dpmi_simulate_int(0x13,&r);
	adr_params=(r.x.es<<4)+r.x.di;
        /* bizarre: l'adresse retournee n'est pas 00522, elle pointe sur
                une table de parametres mais qui n'a pas l'air de servir */
	dosmemget(adr_params,11,params);
	r.h.ah=0x18; r.h.ch=79; r.h.cl=9; r.h.dl=drive;
	_go32_dpmi_simulate_int(0x13,&r);
}

main(int argc,char *argv[])
{
	FILE *fd;
	int t,h,tracks,sides,geometry,n=2;
	char diskid[8], *signature="MFM_DISK";
	char answer;

	printf("%s\n",version);
	if (argc!=3) {
		printf("Usage: writedsk <disk-image> <floppy-drive:>\n");
		exit(1);
	}
	fd=fopen(argv[1],"rb");
	if (fd==NULL) {
		printf("Unable to open %s for reading\n",argv[1]);
		exit(1);
	}
	fread(diskid,8,1,fd);
	if (strncmp(diskid,signature,8)!=0) {
		printf("%s is not a disk image\n",argv[1]);
		exit(1);
	}
	if (strcmp(argv[2],"a:")==0 || strcmp(argv[2],"A:")==0)
		drive=0;
	else if (strcmp(argv[2],"b:")==0 || strcmp(argv[2],"B:")==0)
		drive=1;
	else {
		printf("Invalid floppy drive specification (%s)\n",argv[2]);
		exit(1);
	}
	fread(&sides,1,4,fd); fread(&tracks,1,4,fd); fread(&geometry,1,4,fd);
	fread(&track[0][0],256-20,1,fd);
	for (h=0;h<sides;h++)
		for (t=0;t<tracks;t++)
			fread(track[h][t],6400,1,fd);

	printf("Warning: this operation will erase all data on floppy !\n");
	printf("Do you wish to continue (y/n) ? ");
	scanf("%c",&answer);
	if (answer!='y' && answer!='Y') exit(0);
	read_drive_params();
	for (h=0;h<sides;h++) {
		for (t=0;t<tracks;t++)
			write_track(t,h);
		if (t!=80) printf("\n");
	}
	dosmemput(&n,1,biosparams+3);
}
