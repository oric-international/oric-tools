/**********************************************************************************/
/*                                                                                */
/* Build MFM disk from tracks saved on tape                                       */
/*                                                                                */
/*                                                                                */
/**********************************************************************************/

#include <stdio.h>
#include "mfmlib.h"

static FILE *tape;
static FILE *disk;
static struct {
	char signature[8];
	int sides,tracks,geometry;
	char reserved[256-20];
} header;
static unsigned char track[6400];

int main(int argc, char *argv[])
{
	char signature[]="MFM_DISK";
	int i,t;

	if (argc!=3) {
		printf("Usage: buildmfm <input_tape_file> <output_dsk_file>\n");
		exit(1);
	}
	tape = fopen(argv[1],"rb");
	if (tape==NULL) {
		printf("Unable to open %s for reading\n",argv[1]);
		exit(1);
	}
	disk = fopen(argv[2],"wb");
	if (disk==NULL) {
		printf("Unable to open %s for writing\n",argv[2]);
		exit(1);
	}
	for (i=0;i<8;i++) header.signature[i]=signature[i];
	header.sides=1;
	header.tracks=42;
	header.geometry=1;
	fwrite((char *)&header,1,256,disk);

	for (t=0; t<42; t++) {
		read_synchro();
		read_tape_header();
		read_tape_name();
		fread(track,0x1A01,1,tape);
		fwrite(track,6400,1,disk);
	}
}

int read_synchro()
{
	char sync[4];
	fread(sync,1,4,tape);
	if (sync[0]!=0x16 || sync[1]!=0x16 || sync[2]!=0x16 || sync[3]!=0x24)
		printf("sync error");
}

int read_tape_header()
{
	char header[9];
	fread(header,1,9,tape);
}

int read_tape_name()
{
	char c;
	while((c=fgetc(tape))!=0) printf("%c",c);
	printf("\n");
}
