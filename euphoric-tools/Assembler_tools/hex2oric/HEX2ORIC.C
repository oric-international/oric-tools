#include <stdio.h>

char buffer[65536];
char header[]={ 0x16,0x16,0x16,0x24, 0,0,0x80,0xC7,0,0,0,0,0, 0 };
char ligne[80];

hex2dec(char x) {
	return x>'9' ? x-'a'+10 : x-'0';
}

main(int argc, char *argv[]) {
	unsigned min=0xFFFF,max=0,adr,size;
	int ptr,val;
	char *infile, *oricfile;
	FILE *in, *fd;

	if (argc!=2 && argc!=4) {
		printf("Usage: out2oric [-o <oricfile>] <outfile>\n");
		exit(1);
	}
	if (strcmp(argv[1],"-o")==0) {
		oricfile=argv[2];
		infile=argv[3];
	} else {
		oricfile="ORIC.OUT";
		infile=argv[1];
	}
	in=fopen(infile,"rb");
	if (in==NULL) {
		printf("Can't open %s file\n",infile);
		exit(1);
	}
	fd=fopen(oricfile,"wb");
	if (fd==NULL) {
		printf("Can't open %s file for writing\n",oricfile);
		exit(1);
	}

	for(;;) {
		if (fscanf(in,"%s\n",ligne)==-1) break;
		if (ligne[0]!=':') continue;
		size=(hex2dec(ligne[1])<<4)+hex2dec(ligne[2]);
		adr=(hex2dec(ligne[5])<<4)+hex2dec(ligne[6]);
		adr+=(hex2dec(ligne[3])<<12)+(hex2dec(ligne[4])<<8);
		if (size==0) break;
		if (adr<min) min=adr;
		ptr=9;
		while(size--) {
			if (adr>max) max=adr;
			val=(hex2dec(ligne[ptr])<<4)+hex2dec(ligne[ptr+1]);
			buffer[adr++]=val;
			ptr+=2;
		}		
	}
	fclose(in);
	header[10]=min/256; header[11]=min%256;
	header[8]=max/256; header[9]=max%256;
	fwrite(header,14,1,fd);
	fwrite(buffer+min,max-min+1,1,fd);
	fclose(fd);
}
