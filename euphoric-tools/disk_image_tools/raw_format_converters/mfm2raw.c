#include <stdio.h>

FILE *out;
char *version="MFM to RAW";
unsigned char track[6400];
int chrntab[40];
int ptr;
int drive;
int adr;

search_id(int sectnum)
{
	for(;ptr<6400;ptr++)
		if ( *(unsigned *)(track+ptr)==0
		&& *(unsigned *)(track+ptr+4)==0xFEA1A1A1) {
			ptr+=8;
			if (sectnum==0 || track[ptr+2]==sectnum)
				return *(unsigned *)(track+ptr);
		}
	return 0;
}

find_data()
{
	for(;ptr<6400;ptr++)
		if ( *(unsigned *)(track+ptr)==0xFBA1A1A1) {
			ptr+=4;
			return;
		}
}

int write_sector(char *buf,int chrn,int track,int head)
{
	int try, size= ((char *)&chrn)[3]&3;
	fwrite(buf,128<<size,1,out);
	return 0;
}

write_track(int cyl,int head)
{
	int sectors=0, errors, i, try;
	ptr=0;
	while((chrntab[sectors]=search_id(0))!=0) {
		find_data();
		ptr+= 128 << (((char *)(chrntab+sectors))[3]&3);
		sectors++;
	}
	for(i=0;i<sectors;i++) {
		ptr=0; errors=0;
		search_id(i+1); find_data();
		if (write_sector(track+ptr,chrntab[i],cyl,head)!=0) {
			errors++;
			break;
		}
	}
	return errors;
}

main(int argc,char *argv[])
{
	FILE *fd;
	int t,h,tracks,sides,geometry,n=2;
	char diskid[8], *signature="MFM_DISK";
	char answer;

	printf("%s\n",version);
	if (argc!=3) {
		printf("Usage: writedsk <disk-image> <raw-disk-image>\n");
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
	out=fopen(argv[2],"wb");
	if (out==NULL) {
		printf("Unable to open %s for writing\n",argv[2]);
		exit(1);
	}
	fread(&sides,1,4,fd); fread(&tracks,1,4,fd); fread(&geometry,1,4,fd);
	for (t=0;t<tracks;t++) {
		for (h=0;h<sides;h++) {
			if (geometry==1)
				fseek(fd,256+(h*tracks+t)*6400,SEEK_SET);
			else
				fseek(fd,256+(t*sides+h)*6400,SEEK_SET);
			fread(track,6400,1,fd);
			write_track(t,h);
		}
	}
}
