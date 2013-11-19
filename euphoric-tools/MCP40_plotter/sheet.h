#define XMAX 480
#define YMAX 65000
#define BORDER 16
#define SHEET_WIDTH (XMAX+2*BORDER)

extern bool roll_sheet(int y);
extern void mark_point(int x,int colour);
extern void write_bmp();
