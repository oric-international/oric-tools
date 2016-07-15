/* tap2dsk : converts an Oric tape image to a sedoric/stratsed dsk image */
/* (c) F.Frances 2004 */

/* Limitations :
 *   - only 1 bitmap sector, thus limits the capacity to ~2048 sectors
 *   - only 1 side with 80 tracks max.
 */

#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#include <stdlib.h> // exit
#endif
#define DIRTRACK 20
#define NBTRACKS 21
#define NBSECT 17

typedef unsigned char byte;
typedef byte sector[256];

struct {
	char signature[8];
	int sides;
	int tracks;
	int sect;
	char pad[256-20];
} imageheader = { "ORICDISK", 1, NBTRACKS, NBSECT };

sector disk[80][NBSECT];
sector directory,descriptor;
sector system_sect={	// some system parameters
	0xD0,0xD0,0xD0,0xD0,	// drive table
	0,			// keyboard type
	100,0,10,0,	// RENUM parameters
	'T','A','P','2','D','S','K',' ','S','A','M','P','L','E',' ','D','I','S','C',' ',' ',  // disk name
	' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '			// inist
};
sector bitmap={
	0xFF,		// signature
	0,
	0,0,		// number of free sectors
	0,0,		// number of files
	NBTRACKS,	// number of tracks/side
	NBSECT,		// number of sectors/track
	1,			// number of directory sectors
	NBTRACKS,	// number of tracks + 128 if double sided
	0			// master/slave/gameinit indicator
};

byte sedoric[]=
{
#include "sedoric3.h"
};

int track,sect=1;
int dir_offset=0x10;
int used_sectors;



void allocate_sector(int track, int sect, byte *contents)
{
	int linear_sector=track*NBSECT+sect-1;
	bitmap[0x10+linear_sector/8] &= ~(1<<linear_sector%8);
	memcpy(disk[track][sect-1],contents,sizeof(sector));
	used_sectors++;
}

void find_free_sector(byte *contents)
{
	int linear_sector=0;
	while ((bitmap[0x10+linear_sector/8] & (1<<linear_sector%8)) == 0)
		linear_sector++;

	track=linear_sector/NBSECT;
	sect=linear_sector%NBSECT+1;
	if (track==80) {
		fprintf(stderr,"Not enough space on sample disc");
		exit(1);
	}
	allocate_sector(track,sect,contents);
}

void update_sector(int track, int sect, byte *contents)
{
	memcpy(disk[track][sect-1],contents,sizeof(sector));
}

void convert_basename(char *dest, char *name)
{
	static int filenumber=0;
	int dest_offset=0, src_offset=0;
	printf("Storing ");
	while (dest_offset<9 && src_offset<17 && name[src_offset]) {
		if (name[src_offset]>='a' && name[src_offset]<='z')
			name[src_offset]-=0x20;
		if (name[src_offset]>='0' && name[src_offset]<='9' 
			|| name[src_offset]>='A' && name[src_offset]<='Z') {
				putchar(name[src_offset]);
				dest[dest_offset++]=name[src_offset];
		}
		src_offset++;
	}
	if (dest_offset) {
		while (dest_offset<9)
			dest[dest_offset++]=' ';
	} else {
		printf("NONAME%03d",filenumber);
		sprintf(dest,"NONAME%03d",filenumber++);
	}
}

void store_file(byte *buf, char *name, byte *header)
{
	int start=(header[6]<<8)+header[7];
	int end=(header[4]<<8)+header[5];
	int exec=header[3];
	int block=header[2];
	int sectors=(end-start)/sizeof(sector)+1;
	int desc_sect,desc_track;
	int desc_off=0x0C;

	memset(descriptor,0,sizeof(sector));
	find_free_sector(descriptor);
	desc_track=track; desc_sect=sect;
	descriptor[2]=0xFF;
	descriptor[3]=(block?0x40:0x80)+(exec?1:0);
	descriptor[4]=header[7];
	descriptor[5]=header[6];
	descriptor[6]=header[5];
	descriptor[7]=header[4];
	if (exec) {
		descriptor[8]=descriptor[4];
		descriptor[9]=descriptor[5];
	} else {
		descriptor[8]=descriptor[9]=0;
	}
	descriptor[10]=sectors&0xFF;
	descriptor[11]=sectors>>8;

	convert_basename(directory+dir_offset,name);
	if (exec) {
		sprintf(directory+dir_offset+9,"COM");
		printf(".COM");
	} else {
		sprintf(directory+dir_offset+9,"%s",block?"BIN":"BAS");
		printf(".%s",block?"BIN":"BAS");
	}
	printf("\n");
	directory[dir_offset+12]=desc_track;
	directory[dir_offset+13]=desc_sect;
	directory[dir_offset+14]=sectors+1+(sectors>=(256-12)/2?1:0); // Sedoric bug work-around : allocate a second descriptor when exactly 122 sectors of data
	directory[dir_offset+15]=0x40; // UNPROT

	while (sectors--) {
		find_free_sector(buf);
		buf+=sizeof(sector);
		descriptor[desc_off++]=track;
		descriptor[desc_off++]=sect;
		if (desc_off==0x100) {			// Sedoric bug work-around : allocate a second descriptor when the first is full, even if not needed
			find_free_sector(descriptor);
			descriptor[0]=track;
			descriptor[1]=sect;
			update_sector(desc_track,desc_sect,descriptor);
			memset(descriptor,0,sizeof(sector));
			desc_track=track;
			desc_sect=sect;
			desc_off=2;
		}
	}
	update_sector(desc_track,desc_sect,descriptor);
}

char generateEscapeCode(int color) {
	return (char) 64 + color;
}

int main(int argc, char *argv[])
{
	byte header[9];
	char name[17];
	byte file_buffer[48*1024];
	FILE *tape, *dsk;
	int dir_track=DIRTRACK, dir_sect=4;
	int tracks=DIRTRACK+1; // minimum track number
	int total_sectors,free_sectors;
	int tape_num,i, options=0;
	int tape_name_index = -1;
	int paper_color = -1;
	int ink_color = -1;

	printf("Tap2dsk V2.1\n");

	if (argc<2) {
		fprintf(stderr,"Usage: tap2dsk [-c<paper_color>:<ink_color>] [-n<disk_label>] [-i<init_string>] <tape_image1> ...<tape_imageN> <old_disk_image>\n");
		exit(1);
	}

	for (i=1; i<argc; i++) {
		if (argv[i][0]=='-') {
			switch (argv[i][1]) {
			case 'c': case 'C':
				if (sscanf(argv[i], "-c%d:%d", &paper_color, &ink_color) != 2) {
					fprintf(stderr, "Malformed color option. It should be -c<paper_color>:<ink_color>\n");
					exit(3);
				}
				if (paper_color <16 || paper_color >23 || ink_color < 0 || ink_color > 7) {
					fprintf(stderr, "Wrong colors. Paper color should be between 16 and 23 and ink color between 0 and 7\n");
					exit(3);
				}
				// if the name was before
				if (tape_name_index > 0) {
					memset(system_sect + 9, ' ', 21);
					*(system_sect + 9) = (char)27;
					*(system_sect + 10) = generateEscapeCode(paper_color);
					*(system_sect + 11) = (char)27;
					*(system_sect + 12) = generateEscapeCode(ink_color);
					if (strlen(argv[tape_name_index]) - 2 > 17) {
						fprintf(stderr, "Disk label too long (if you use colors you loose four characters)\n");
						exit(3);
					}
					memcpy(system_sect + 13, argv[tape_name_index] + 2, strlen(argv[tape_name_index]) - 2);
				}
				break;
			case 'i': case 'I':
				if (strlen(argv[i]) - 2 > 60) {
					fprintf(stderr,"Init string too long\n");
					exit(3);
				}
				memcpy(system_sect+0x1E,argv[i]+2,strlen(argv[i])-2);
				// remember where it was
				tape_name_index = i;
				break;
			case 'n': case 'N':
				memset(system_sect+9,' ',21);
				// if the colors were before
				if (paper_color > 0 && ink_color >= 0) {
					*(system_sect + 9) = (char)27;
					*(system_sect + 10) = generateEscapeCode(paper_color);
					*(system_sect + 11) = (char)27;
					*(system_sect + 12) = generateEscapeCode(ink_color);
					if (strlen(argv[i]) - 2 > 17) {
							fprintf(stderr, "Disk label too long (if you use colors you loose four characters)\n");
							exit(3);
					}
					memcpy(system_sect + 13, argv[i] + 2, strlen(argv[i]) - 2);
				}
				else {
					// if there are no colors do like before
					// if the colors are after we will overwrite the label
				if (strlen(argv[i]) - 2 > 21) {
					fprintf(stderr,"Disk label too long\n");
					exit(3);
				}
				memcpy(system_sect+9,argv[i]+2,strlen(argv[i])-2);
				}
				tape_name_index = i;
				break;
			default: fprintf(stderr,"Bad option : %c\n",argv[i][1]);
				exit(2);
			}
		}
	}

	dsk=fopen(argv[argc-1],"wb");
	if (dsk==NULL) {
		fprintf(stderr,"Cannot open %s for writing\n",argv[2]);
		exit(1);
	}

	memset(bitmap+0x10,0xFF,sizeof(sector)-0x10);
	for (i=0; i<99; i++) {	// 99 secteurs pour sedoric
		int track= i/NBSECT;
		int sector = i%NBSECT + 1;
		allocate_sector(track,sector,sedoric+i*256);
	}
	allocate_sector(DIRTRACK,1,system_sect);
	allocate_sector(DIRTRACK,2,bitmap);
	allocate_sector(DIRTRACK,4,directory);

	for (tape_num=1; tape_num<argc-1; tape_num++) {
		if (argv[tape_num][0]=='-') continue;
		tape=fopen(argv[tape_num],"rb");
		if (tape==NULL) {
			fprintf(stderr,"Cannot read tape image %s\n",argv[tape_num]);
			exit(1);
		}
		printf("Reading %s\n",argv[tape_num]);


		while (fgetc(tape)!=EOF) {
			int start,end,i;
			while (fgetc(tape)!=0x24)
				;
			for (i=0;i<9;i++) header[i]=fgetc(tape);
			for (i=0;i<17;i++) {
				name[i]=fgetc(tape);
				if (name[i]==0) break;
			}

			// if the name is null then copy the file name instead
			if (name[0] == 0) {
				// only take the file name from the path
				char *fileName = argv[tape_num];
				// try to find \\  
				char *lastsep = strrchr(fileName, '\\');
				if (lastsep != NULL) {
					// if there is something after the separator
					if (lastsep+1 != 0)
						fileName = lastsep + 1;
				} else {
					// try to find / 
					lastsep = strrchr(fileName, '/');
					if (lastsep != NULL) {
						// if there is something after the separator
						if (lastsep + 1 != 0)
							fileName = lastsep + 1;
					}
				}
				// remove the extension if there is one
				char *lastdot = strrchr(fileName, '.');
				if (lastdot != NULL)
					*lastdot = 0;
				for (i = 0; i<17; i++) {
					name[i] = fileName[i];
					if (name[i] == 0) break;
				}
			}

			start=(header[6]<<8)+header[7];
			end=(header[4]<<8)+header[5];
			for (i=0; i<end+1-start; i++)
				file_buffer[i]=fgetc(tape);
			printf("Found %s\n",name);
			store_file(file_buffer,name,header);
			bitmap[4]++; // number of files
			dir_offset+=16;
			if (dir_offset==0x100) {
				find_free_sector(directory);
				directory[0]=track;
				directory[1]=sect;
				update_sector(dir_track,dir_sect,directory);
				memset(directory,0,sizeof(sector));
				dir_track=track;
				dir_sect=sect;
				dir_offset=0x10;
				update_sector(dir_track,dir_sect,directory);
			}
		}
		fclose(tape);
		tape=NULL;
	}
	directory[2]=dir_offset;
	update_sector(dir_track,dir_sect,directory);

	if (track>=tracks) tracks=track+1;
	total_sectors=tracks*NBSECT;
	free_sectors=total_sectors-used_sectors;
	bitmap[2]=free_sectors & 0xFF;
	bitmap[3]=free_sectors >> 8;
	bitmap[6]=tracks;
	bitmap[9]=tracks;
	update_sector(DIRTRACK,2,bitmap);

	imageheader.tracks=tracks;
	fwrite(&imageheader,sizeof(imageheader),1,dsk);
	for (track=0;track<tracks;track++)
		for (sect=1;sect<=NBSECT;sect++)
			fwrite(disk[track][sect-1],sizeof(sector),1,dsk);
}


