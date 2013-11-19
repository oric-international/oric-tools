#include <stdio.h>

#ifndef __GO32__
#include <asm/io.h>
#include <signal.h>
void int_handler()
{
	flush_shifter();
	save_file();
	exit(0);
}
#endif

#define PRINTER_OUT 0x378
#define PRINTER_IN 0x379

#define OUT(val,port) outb(val,port)
#define IN(port) inb(port)

unsigned char buf[45*60*2400/8];
unsigned int size;
FILE *out;


main()
{
#ifndef __GO32__
	iopl(3);
	signal(SIGINT,int_handler);
#endif
	normal_ack();
	out=fopen("image.tap","wb");
	if (out==NULL) {
		printf("Can't open image.tap for writing\n");
		exit(1);
	}
	printf("OricLink v2.0\n");
	printf("Start the tape player and type RUN on the Oric\n");
	printf("Press a key on the PC to stop the transfert\n");
	receive_tape(); 
	flush_shifter();
	save_file();
}

normal_ack()
{
	asm("movl $0x0378,%edx");
	asm("movb $0xFF,%al");
	asm("outb %al,%dx");
}

send_ack()
{
	asm("movl $0x0378,%edx");
	asm("movb $0,%al");
	asm("outb %al,%dx");
}

unsigned char shifter;
int shiftcount;

receive_tape()
{
	int bit;
	for(;;) {
		while ( !(inb(PRINTER_IN)&0x80) )
#ifdef __GO32__
			if (kbhit()) return
#endif
			;
		bit= ( inb(PRINTER_IN) >> 3 ) & 1;
		send_ack();
		shifter=(shifter<<1)+bit;
		shiftcount++;
		if (shiftcount==8) {
			buf[size++]=shifter;
			shiftcount=0;
		}
		normal_ack();
		while ( inb(PRINTER_IN)&0x80 );
	}
}

flush_shifter()
{
	while (shiftcount) {
		shifter=(shifter<<1)+1;
		shiftcount++;
		if (shiftcount==8) {
			buf[size++]=shifter;
			shiftcount=0;
		}
	}
}

save_file()
{
	int i;
	for (i=0;i<size;i++) {
		output_bit(buf[i]&0x80);
		output_bit(buf[i]&0x40);
		output_bit(buf[i]&0x20);
		output_bit(buf[i]&0x10);
		output_bit(buf[i]&0x08);
		output_bit(buf[i]&0x04);
		output_bit(buf[i]&0x02);
		output_bit(buf[i]&0x01);
	}
	flush_file();
}

output_bit(int bit)
{
	output_level(1);
	output_level(0);
	if (bit==0) output_level(0);
}

output_level(int l)
{
	shifter=(shifter<<1)+l;
	shiftcount++;
	if (shiftcount==8) {
		putc(shifter,out);
		shiftcount=0;
	}
}

flush_file()
{
	while (shiftcount) {
		shifter=(shifter<<1)+1;
		shiftcount++;
		if (shiftcount==8) {
			putc(shifter,out);
			shiftcount=0;
		}
	}
}
