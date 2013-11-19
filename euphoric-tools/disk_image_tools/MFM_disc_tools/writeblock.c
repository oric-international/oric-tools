#include <stdio.h>
#include "mfmlib.h"

int main(int argc,char *argv[])
{
	FILE *fd;
	char sectbuf[256];
	int sector,read,err;

	if (argc!=4) {
		printf("Usage: writeblock disk_image blockfile sector_no\n");
		printf("\nThis tool will write <blockfile> onto side 2 of a double-sided disk image,\n");
		printf("starting at sector sector_no (zero-based).\n\n");
		printf("Caution:\n");
		printf("\t-the disk should have been previously formatted (17 sectors per track)\n");
		printf("\t-the logical sector number should not exceed the capacity of one side\n");
		exit(1);
	}
	if (open_disk(argv[1])) {
		printf("Cannot open disk image %s, or invalid disk image.\n",argv[1]);
		exit(2);
	}
	fd=fopen(argv[2],"rb");
	if (fd==NULL) {
		printf("Cannot open blockfile %s\n",argv[2]);
		exit(2);
	}
	sector=atoi(argv[3]);
	read=fread(sectbuf,1,256,fd);
	while (read>0) {
		err=write_sector(sectbuf,sector/17,1,sector%17+1);
		if (err) {
			printf("Cannot write sector, error %d\n",err);
			exit(4);
		}
		sector++;
		read=fread(sectbuf,1,256,fd);
	}
	close_disk();
}

