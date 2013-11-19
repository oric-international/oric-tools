/**********************************************************************************/
/*                                                                                */
/* MFM disk library routines v2.0                                                 */
/* beware : check any tool build with v1.0                                        */
/*                                                                                */
/* F.Frances 2/12/01                                                              */
/*                                                                                */
/**********************************************************************************/

#include <stdio.h>
#include "mfmlib.h"

static FILE *disk;
static struct {
	char signature[8];
	int sides,tracks,geometry;
	char r2eserved[256-20];
} header;
static unsigned char track[6400];
static int side, trackno=-1;
static unsigned int ptr, pos, dirty;

int sides() { return header.sides; }
int tracks() { return header.tracks; }

unsigned char read_byte(int offset)
{
	return track[offset];
}

void write_byte(int offset, unsigned char val)
{
	track[offset] = val;
	dirty = 1;
}

int find_data()
{
	for(;ptr<6250;ptr++) {
		if (track[ptr+1]==0xA1 && track[ptr+2]==0xA1 && track[ptr+3]==0xFB) {
			if (track[ptr]!=0xA1) { // fix desynchronized first byte
				track[ptr]=0xA1;
				dirty=1;
			}
			ptr+=4;
			return ptr;
		}
	}
	return -1;
}



int search_id(int cyl, int head, int sector)
{
	find_first_id();
	while (ptr!=-1 && track[ptr]!=cyl && track[ptr+1]!=head && track[ptr+2]!=sector) {
		int size=128<<(track[ptr+3]&3);
		find_data();
		ptr+=size+2;
		find_next_id();
	}
	return ptr;
}

int find_first_id()
{
	ptr=0;
	return find_next_id();
}

int find_next_id()
{
	for (;ptr<6250;ptr++)
		if (track[ptr+1]==0xA1 && track[ptr+2]==0xA1 && track[ptr+3]==0xFE) {
			if (track[ptr]!=0xA1) { // fix desynchronized first byte
				track[ptr]=0xA1;
				dirty=1;
			}
			ptr+=4;
			return ptr;
		}
	return -1;
}


static void flush_track()
{
	fseek(disk,pos,SEEK_SET);
	fwrite(track,6400,1,disk);
	dirty=0;
}

int read_track(int cyl, int head)
{
	if (cyl==trackno && head==side) return 0;
	if (dirty) flush_track();
	trackno=cyl; side=head;
	if (header.geometry==2)
		pos=(trackno*header.sides+side)*6400+256;
	else
		pos=(side*header.tracks+trackno)*6400+256;
	fseek(disk,pos,SEEK_SET);
	if (fread(track,6400,1,disk)!=1) return 1;
	return 0;
}

static unsigned int crctab[256] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78, 
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067, 
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
	
unsigned short compute_crc(int offset,int count)
{
	int i;
	unsigned short crc=0xFFFF,byte;
	for (i=0;i<count;i++) {
		byte= track[offset++];
		crc=(crc<<8)^crctab[(crc>>8)^byte];
	}
	return crc;
}

unsigned short read_crc(int offset)
{
	return ((track[offset]&0xFF)<<8) + (track[offset+1]&0xFF);
}

void write_crc(int offset, unsigned short crc)
{
	track[offset]=crc>>8;
	track[offset+1]=crc&0xFF;
}

/*****************************************************************************/

int open_disk(char *name)
{
	if (disk) return DISK_ALREADY_OPENED;
	disk=fopen(name,"r+b");
	if (disk==NULL) return CANNOT_OPEN_DISK;
	if (fread(&header,sizeof(header),1,disk)!=1 || strncmp(header.signature,"MFM_DISK",8)!=0)
		return INVALID_DISK;
	return OK;
}

int close_disk()
{
	if (disk==NULL) return NO_OPENED_DISK;
	flush_track();
	fclose(disk);
}

int read_sector(char *buf, int cyl, int head, int sector)
{
	int id_offset, data_offset;
	int size;

	if (disk==NULL) return NO_OPENED_DISK;
	if (read_track(cyl,head)) return TRACK_NOT_FOUND;
	id_offset=search_id(cyl,head,sector);
	if (id_offset==-1) return SECTOR_ID_NOT_FOUND;
	size=128<<(track[id_offset+3]&3);
	data_offset=find_data();
	if (data_offset==-1) return SECTOR_DATA_NOT_FOUND;
	memcpy(buf,track+data_offset,size);
	return OK;
}

int write_sector(char *buf, int cyl, int head, int sector)
{
	int id_offset,data_offset;
	int size;
	unsigned short crc;

	if (disk==NULL) return NO_OPENED_DISK;
	if (read_track(cyl,head)) return TRACK_NOT_FOUND;
	id_offset=search_id(cyl,head,sector);
	if (id_offset==-1) return SECTOR_ID_NOT_FOUND;
	size=128<<(track[id_offset+3]&3);
	data_offset=find_data();
	if (data_offset==-1) return SECTOR_DATA_NOT_FOUND;
	memcpy(track+data_offset,buf,size);
	crc=compute_crc(data_offset-4,size+4);
	write_crc(data_offset+size,crc);
	dirty=1;
	return OK;
}

