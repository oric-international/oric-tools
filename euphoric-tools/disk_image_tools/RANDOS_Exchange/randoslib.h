






#define INVALID_SECTOR_NUMBER 16
#define CANNOT_READ_SYSTEM_SECTOR 17
#define CANNOT_READ_FAT_SECTOR 18
#define CANNOT_READ_DIR_SECTOR 19
#define CANNOT_WRITE_FAT_SECTOR 20
#define CANNOT_WRITE_DIR_SECTOR 21
#define FILE_NOT_FOUND 22
#define CANNOT_CREATE_HOST_FILE 23
#define CANNOT_OPEN_HOST_FILE 24
#define CANNOT_WRITE_HOST_FILE 25
#define NO_SPACE_LEFT 26
#define TRUNCATED_FILE 27

typedef unsigned short sectnumber;

struct dir_entry {
	char name[6];
	char ext[3];
	unsigned char flags;
	sectnumber first_sect;
	char unknown;
	unsigned char size[3];
};
