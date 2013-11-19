#include <stdlib.h>
#include <stdio.h>
#include "boolean.h"
#include "plotter.h"
#include "sheet.h"

static FILE *in;

static bool another_arg()
{
	char c=fgetc(in);
	while (c==' ')
		c=fgetc(in);
	if (c!=',')
		ungetc(c,in);
	return c==',';
}

static int get_number()
{
	int number=0,sign=1;
	char c=fgetc(in);
	while (c==' ')
		c=fgetc(in);
	if (c=='+' || c=='-') {
		sign=(c=='-')?-1:1;
		c=fgetc(in);
	}
	while (c>='0' && c<='9') {
		number=number*10+c-'0';
		c=fgetc(in);
	}
	if (c=='.') {
		c=fgetc(in);
		while (c>='0'&& c<='9')
			c=fgetc(in);
	}
	ungetc(c,in);
	return number*sign;
}

/**************************************************************************/
/* Les commandes texte                                                    */
/**************************************************************************/

void back_space() {	move_pen(LEFT,6*(get_char_size()+1)); }

void carriage_return()
{
	position current=get_pen_pos();
	current.x=0;
	set_pen_pos(current);
}

void line_feed() { move_pen(DOWN,12*(get_char_size()+1)); }
void reverse_line_feed() { move_pen(UP,12*(get_char_size()+1)); }

void draw_char(char c)
{
	static char *charset[96]={
#include "vector_charset.h"
	};
	char *pattern;
	position current=get_pen_pos();
	int step=get_char_size()+1;
	c = c & 0x7F;
	if (c<32) return;
	pattern=charset[c-32];

	if (current.x+6*step>XMAX) {
		carriage_return();
		line_feed();
		current=get_pen_pos();
	}

	move_pen(UP,step);
	while (*pattern) {
		switch (*pattern++) {
		case 'D': pen_down(); break;
		case 'd': pen_down_no_mark(); break;
		case 'U': pen_up(); break;
		case '0': move_pen(RIGHT,step); break;
		case '1': move_pen(UPRIGHT,step); break;
		case '2': move_pen(UP,step); break;
		case '3': move_pen(UPLEFT,step); break;
		case '4': move_pen(LEFT,step); break;
		case '5': move_pen(DOWNLEFT,step); break;
		case '6': move_pen(DOWN,step); break;
		case '7': move_pen(DOWNRIGHT,step); break;
		}
	}
	pen_up();
	set_pen_pos(current);
	move_pen(RIGHT,6*step);
}

/**************************************************************************/
/* les commandes graphiques                                               */
/**************************************************************************/


void draw_absolute_line()
{
	int x,y;
	position origin=get_origin();
	do {
		x=get_number();
		if (!another_arg()) break;
		y=get_number();
		if (x<-999 || x>999 || y<-999 || y>999) break;
		draw_to(origin.x+x,origin.y-y);
	} while (another_arg());
}

void draw_relative_line()
{
	int x,y;
	do {
		position pos=get_pen_pos();
		x=get_number();
		if (!another_arg()) break;
		y=get_number();
		if (x<-999 || x>999 || y<-999 || y>999) break;
		draw_to(pos.x+x,pos.y-y);
	} while (another_arg());
}

void move_absolute()
{
	int x,y;
	x=get_number();
	if (!another_arg()) return;
	y=get_number();
	if (x>=-999 && x<=999 && y>=-999 && y<=999) {
		position origin=get_origin(), pos=get_pen_pos();
		int dx=(origin.x+x)-pos.x;
		int dy=(origin.y-y)-pos.y;
		while (dx<0) { dx++; pen_step(LEFT); }
		while (dx>0) { dx--; pen_step(RIGHT); }
		while (dy<0) { dy++; pen_step(UP); }
		while (dy>0) { dy--; pen_step(DOWN); }
	}
}

void move_relative()
{
	int x,y;
	x=get_number();
	if (!another_arg()) return;
	y=get_number();
	if (x>=-999 && x<=999 && y>=-999 && y<=999) {
		while (x<0) { x++; pen_step(LEFT); }
		while (x>0) { x--; pen_step(RIGHT); }
		while (y<0) { y++; pen_step(DOWN); }
		while (y>0) { y--; pen_step(UP); }
	}
}

static void vertical_tick()
{
	pen_step(DOWN); pen_step(DOWN);
	pen_down();
	pen_step(UP); pen_step(UP); pen_step(UP); pen_step(UP);
	pen_up();
	pen_step(DOWN); pen_step(DOWN);
}

static void horizontal_tick()
{
	pen_step(LEFT);pen_step(LEFT);
	pen_down();
	pen_step(RIGHT); pen_step(RIGHT); pen_step(RIGHT); pen_step(RIGHT);
	pen_up();
	pen_step(LEFT);pen_step(LEFT);
}

void draw_axis() {
	int dir,step,nbstep,i;
	dir=get_number();
	if (!another_arg()) return;
	step=get_number();
	if (!another_arg()) return;
	nbstep=get_number();
	if ( (dir!=0 && dir!=1) || step<-999 || step>999 || nbstep<1 || nbstep>255) return;
	if (dir==0) horizontal_tick();
	else vertical_tick();
	while (nbstep--) {
		if (dir==0) {	/* vertical */
			pen_down();
			for (i=0;i<abs(step);i++) {
				if (step<0) pen_step(DOWN);
				else pen_step(UP);
			}
			pen_up();
			horizontal_tick();
		} else {		/* horizontal */
			pen_down();
			for (i=0;i<abs(step);i++) {
				if (step<0) pen_step(LEFT);
				else pen_step(RIGHT);
			}
			pen_up();
			vertical_tick();
		}
	}
}

void print_text_chars()
{
	char c=fgetc(in);
	while (c!=13) {
		if (c>=32) draw_char(c);
		c=fgetc(in);
	}
}


/**************************************************************************/
void reset()
{
	carriage_return();
	set_origin();
	set_text_mode();
	set_dir(0);
}

int main(int argc, char *argv[])
{
	init_plotter();

	if (argc!=2 || !(in=fopen(argv[1],"rb"))) {
		fprintf(stderr,"4-color plotter converter V1.1\n");
		fprintf(stderr,"F.Frances 2003\n\n");
		fprintf(stderr,"Usage: %s <printer_file>\n",argv[0]);
		exit(1);
	}
	while (!feof(in)) {
		int c=fgetc(in);
		if (c==EOF) break;
		if (is_graphic_mode()) {
			switch(c) {
			case 17: set_text_mode(); break;
			case 'A': reset(); break;
			case 'C': change_color(get_number()); break;
			case 'D': draw_absolute_line(); break;
			case 'H': home(); break;
			case 'I': set_origin(); break;
			case 'J': draw_relative_line(); break;
			case 'M': move_absolute(); break;
			case 'L': set_line_type(get_number()); break;
			case 'P': print_text_chars(); break;
			case 'S': set_char_size(get_number()); break;
			case 'Q': set_dir(get_number()); break;
			case 'R': move_relative(); break;
			case 'X': draw_axis(); break;
			}
		} else {
			c &= 0x7F;
			if (c>=32)
				draw_char(c);
			else
				switch(c) {
				case 8: back_space(); break;
				case 10: line_feed(); break;
				case 11: reverse_line_feed(); break;
				case 13: carriage_return(); break;
				case 17: break; /* text mode */
				case 18: set_graphic_mode(); break;
				case 29: rotate_color(); break;
				}
		}
	}
	fclose(in);

	write_bmp();
	return 0;
}
