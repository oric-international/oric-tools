#include "boolean.h"
#include "sheet.h"
#include "plotter.h"

static struct {
	position pos;
	bool pen_down;
	int pen;		/* n° de stylo courant */
	int direction;	/* direction d'écriture */
	int char_size;	/* taille caractère: 0 à 63 */
	bool graphics;
	position origin;
	int line_type;	/* espacement des pointillés: 0 à 15 */
} plotter;

void inc_x()
{
	if (plotter.pos.x < XMAX)
		plotter.pos.x++;
}

void dec_x()
{
	if (plotter.pos.x > 0)
		plotter.pos.x--;
}

void inc_y()
{
	if (roll_sheet(plotter.pos.y+1))
		plotter.pos.y++;
}

void dec_y()
{
	if (roll_sheet(plotter.pos.y-1))
		plotter.pos.y--;
}

void change_color(int pen)
{
	if (pen>=0 && pen<=3)
		plotter.pen=pen;
}

void set_dir(int dir)
{
	if (dir>=0 && dir<=3)
		plotter.direction=dir;
}

void home() { plotter.pos=plotter.origin; }
void set_origin() { plotter.origin=plotter.pos; }
void set_graphic_mode() { plotter.graphics=true; set_origin(); }
void set_text_mode() { plotter.graphics=false; }

void set_line_type(int line_type)
{
	if (line_type>=0 && line_type<=15)
		plotter.line_type=line_type;
}

void set_char_size(int char_size)
{
	if (char_size>=0 && char_size<=63)
		plotter.char_size=char_size;
}

void plot(position pos, int colour)
{
	if (roll_sheet(pos.y))
		mark_point(pos.x,colour+1);
}

void pen_down()
{
	plotter.pen_down=true;
	plot(plotter.pos,plotter.pen);
}

void pen_down_no_mark()
{
	plotter.pen_down=true;
}

void pen_up() {	plotter.pen_down=false; }
void rotate_color() { plotter.pen=(plotter.pen+1)%NBCOLORS; }
int get_dir() {	return plotter.direction; }
int get_char_size() { return plotter.char_size; }
position get_pen_pos() { return plotter.pos; }
position get_origin() { return plotter.origin; }
void set_pen_pos(position newpos) { plotter.pos=newpos; }
bool is_graphic_mode() { return plotter.graphics; }

void pen_step(int absolute_dir)
{
	switch (absolute_dir) {
	case UPRIGHT:	
		dec_y(); 
	case RIGHT:	
		if (plotter.pen_down)
			plot(plotter.pos,plotter.pen);
		inc_x();
		break;
	case UP:		
		dec_y(); 
		if (plotter.pen_down)
			plot(plotter.pos,plotter.pen);
		break;
	case UPLEFT:	
		dec_y();
	case LEFT:		
		dec_x(); 
		if (plotter.pen_down)
			plot(plotter.pos,plotter.pen);
		break;
	case DOWNLEFT:	
		dec_x();
	case DOWN:		
		if (plotter.pen_down)
			plot(plotter.pos,plotter.pen);
		inc_y(); 
		break;
	case DOWNRIGHT: 
		if (plotter.pen_down)
			plot(plotter.pos,plotter.pen);
		inc_y(); 
		inc_x();
		break;
	}
}

void move_pen(int relative_dir,int length)
{
	int i;
	int absolute_dir = (8-2*get_dir()+relative_dir)%8;
	for (i=0;i<length;i++) {
		switch (absolute_dir) {
		case UPRIGHT: dec_y(); 
		case RIGHT:	inc_x(); break;
		case UP:	dec_y(); break;
		case UPLEFT:dec_y();
		case LEFT:	dec_x(); break;
		case DOWNLEFT:dec_x();
		case DOWN:	inc_y(); break;
		case DOWNRIGHT:inc_y(); inc_x(); break;
		}
		if (plotter.pen_down)
			plot(plotter.pos,plotter.pen);
	}
}

void dot_count()
{
	static int cnt=0;
	static bool draw=false;
	if (plotter.line_type==0) draw=true;
	else {
		cnt++;
		if (cnt>=plotter.line_type) {
			draw = !draw;
			cnt=0;
		}
	}
	if (draw) pen_down_no_mark();
	else pen_up();
}

void draw_to(int xdest, int ydest)
{
	int i;
	int dx=xdest-plotter.pos.x;
	int dy=ydest-plotter.pos.y;
	if (dx==0 && dy==0) return;
	if ( abs(dx) < abs(dy) ) {
		double frac=0.0;
		double step=((double)abs(dx))/((double)abs(dy));
		for (i=0;i<abs(dy);i++) {
			dot_count();
			frac+=step;
			if (frac>0.5) {
				frac-=1.0;
				if (dx>0) {
					if (dy>0) pen_step(DOWNRIGHT);
					else pen_step(UPRIGHT);
				} else {
					if (dy>0) pen_step(DOWNLEFT);
					else pen_step(UPLEFT);
				}
			} else {
				if (dy>0) pen_step(DOWN);
				else pen_step(UP);
			}
		}
	} else {
		double frac=0.0;
		double step=((double)abs(dy))/((double)abs(dx));
		for (i=0;i<abs(dx);i++) {
			dot_count();
			frac+=step;
			if (frac>0.5) {
				frac-=1.0;
				if (dy>0) {
					if (dx>0) pen_step(DOWNRIGHT);
					else pen_step(DOWNLEFT);
				} else {
					if (dx>0) pen_step(UPRIGHT);
					else pen_step(UPLEFT);
				}
			} else {
				if (dx>0) pen_step(RIGHT);
				else pen_step(LEFT);
			}
		}
	}
	pen_up();
}

void init_plotter()
{
	int i;
	plotter.char_size=1;	/* suivant la position du switch arrière: 0 pour 80 colonnes, 1 pour 40 colonnes */
	plotter.origin.y = plotter.pos.y = 999;
	for (i=0;i<24;i++)
		pen_step(DOWN);
}


