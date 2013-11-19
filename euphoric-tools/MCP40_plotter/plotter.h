#define RIGHT 0
#define UPRIGHT 1
#define UP 2
#define UPLEFT 3
#define LEFT 4
#define DOWNLEFT 5
#define DOWN 6
#define DOWNRIGHT 7

#define NBCOLORS 4

typedef struct {
	int x,y;
} position;

extern void init_plotter();
extern void plot(position pos, int colour);
extern void line_plot(position pos,int colour);
extern void pen_down(),pen_down_no_mark(),pen_up(),rotate_color();
extern void set_pen_pos(position newpos);
extern void pen_step(int absolute_dir);
extern void move_pen(int relative_dir,int length);
extern void draw_to(int xdest, int ydest);
extern void change_color(int pen);
extern void set_dir(int dir);
extern void home();
extern void set_origin();
extern void set_line_type(int line_type);
extern void set_char_size(int char_size);
extern void set_graphic_mode(), set_text_mode();

extern int get_dir(),get_char_size();
extern position get_pen_pos(), get_origin();
extern bool is_graphic_mode();
