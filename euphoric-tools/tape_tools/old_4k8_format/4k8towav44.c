#include <stdio.h>
FILE *fd, *fd2;
struct {
	char sig[4];
	int riff_size;
	char datasig[4];
	char fmtsig[4];
	int fmtsize;
	short tag;
	short channels;
	int freq;
	int bytes_per_sec;
	short byte_per_sample;
	short bits_per_sample;
	char samplesig[4];
	int length;
} sample_riff= {"RIFF",0,"WAVE","fmt ",16,1,2,44100,4*44100,4,16,"data",0};

main(int argc,char *argv[])
{
	int i,c,size;

	if (argc!=3) {
		printf("Usage: oric2wav <4K8_tape_image> <wav_file>\n");
		exit(1);
	}
	fd=fopen(argv[1],"rb");
	if (fd==NULL) {
		printf("Unable to open %s\n",argv[1]);
		exit(1);
	}
	fd2=fopen(argv[2],"wb");
	if (fd2==NULL) {
		printf("Unable to open %s\n",argv[2]);
		exit(1);
	}
	fseek(fd,0,SEEK_END); size=ftell(fd)+50; fseek(fd,0,SEEK_SET);
	sample_riff.length=size*8*9*4;
	sample_riff.riff_size=sample_riff.length+36;
	fwrite(&sample_riff,1,sizeof(sample_riff),fd2);
	for(i=0;i<size;i++) {
		c=fgetc(fd);
                fputc4(c&0x80,fd2);
                fputc4(c&0x40,fd2);
                fputc4(c&0x20,fd2);
                fputc4(c&0x10,fd2);
                fputc4(c&0x08,fd2);
                fputc4(c&0x04,fd2);
                fputc4(c&0x02,fd2);
                fputc4(c&0x01,fd2);
	}
}

fputc4(int c, FILE *fd2) {
 int i;
 for(i=0;i<9*2;i++)  {
	fputc(c?0xFF:0x00,fd2);
	fputc(c?0x7F:0x80,fd2);
 }
}
