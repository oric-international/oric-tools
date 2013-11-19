#include <malloc.h>
#include <stdio.h>
#include "boolean.h"
#include "sheet.h"

static struct {
	char *line[YMAX+1];
	int current_y;
} sheet;

bool roll_sheet(int y)
{
	int i;
	if (y<0 || y>YMAX) return 0;
	sheet.current_y=y;
	if (sheet.line[y]==NULL) {
		sheet.line[y]=(char *)malloc(SHEET_WIDTH);
		if (sheet.line[y]==NULL) {
			fprintf(stderr,"Not enough memory\n");
			exit(1);
		}
		for (i=0;i<=SHEET_WIDTH;i++)
			sheet.line[y][i]=0;
	}
	return true;
}

void mark_point(int x, int colour)
{
	sheet.line[sheet.current_y][x+BORDER]=colour;
}

void write_bmp()
{
	int firstline,lastline,nblines,i;
	char signature[]="BM";
	unsigned int header[]={ 0x70C36,0,1024+54,40,480,960,0x080001,0,0,0,0,0,0 };
	long palette[256]={0xFFFFFF,0,0xFF0000,0x00FF00,0x0000FF};

	FILE *bmpfile;
	char name[128];
	for (i=1;i<100;i++) {
		sprintf(name,"Sheet%d.bmp",i);
		bmpfile=fopen(name,"r");
		if (bmpfile) fclose(bmpfile);
		else break;

	}
	if (i==100) {
		fprintf(stderr,"Too many MCP sheets\n");
		exit(2);
	}
	bmpfile=fopen(name,"wb");
	if (bmpfile==NULL) {
		fprintf(stderr,"Cannot create %s\n",name);
		exit(3);
	}

	for (firstline=1000; firstline>0 && sheet.line[firstline-1]; firstline--)
		;
	for (lastline=1000; lastline<YMAX && sheet.line[lastline+1]; lastline++)
		;
	nblines=lastline-firstline+1;
	header[4]=SHEET_WIDTH;
	header[5]=nblines;
	header[0]=SHEET_WIDTH*nblines+sizeof(palette)+54;

	fwrite(signature,1,2,bmpfile);
	fwrite(header,4,13,bmpfile);
	fwrite(palette,4,256,bmpfile);
	for(i=lastline;i>=firstline;i--) fwrite(sheet.line[i],1,SHEET_WIDTH,bmpfile);
	fclose(bmpfile);
}
