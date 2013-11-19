/* GNU compiler */
#include <stdio.h>
#include <conio.h>
#include <dos.h>

struct {
	char sig[4];
	int riff_size;
	char typesig[4];
	char fmtsig[4];
	int fmtsize;
	short tag;
	short channels;
	int freq;
	int bytes_per_sec;
	short byte_per_sample;
	short bits_per_sample;
	char samplesig[4];
	int datalength;
} sample_riff= { "RIFF",0,"WAVE","fmt ",16,1,1,0,0,1,8,"data",0 };


usage()
{
        printf("Usage: tapelink [-lpt{1|2|3}] [<filename.wav>]\n");
        exit(1);
}

unsigned char buf[45*60*2400/8]; /* room for a 45' tape filled with '1' bits */
unsigned int size;
FILE *out;
char *filename="image.wav";
short printer_ports[3];
int port, printer_out, printer_status;
int speed=4800;


main(int argc, char *argv[])
{
        int n;

        printf("TapeLink 2.2\n");
        for (n=1;n<argc;n++) {
                if (strncmp(argv[n],"-lpt",4)==0) {
                        port=argv[1][4]-'1';
                        if (port<0 || port>2 || argv[1][5]) usage();
                } else filename=argv[n];
        }
        out=fopen(filename,"wb");
	if (out==NULL) {
                printf("Can't open %s for writing\n",filename);
		exit(1);
	}
        dosmemget(0x408,3*2,printer_ports);
        printer_out=printer_ports[port];
        printer_status=printer_ports[port]+1;

	printf("Start the tape player and type RUN on the Oric\n");
        printf("Press a key on the PC to stop the transfer\n");
	receive_tape(); 
	flush_shifter();
	save_file();
        printf("\r%s has been written.\n",filename);
}

unsigned char shifter;
int shiftcount;
char graphic[4]={'-','\\','|','/'};

receive_tape()
{
        int bit,flag;
        char counter=0;

        outb(0xFF,printer_out);
	for(;;) {
                while ( !(inb(printer_status)&0x80) )
                        if (counter==0) {
                                counter++;
                                flag= (flag+1)&3;
                                putch('\r');
                                putch(graphic[flag]);
                        }
                bit= ( inb(printer_status) >> 3 ) & 1;
                outb(0,printer_out);
		shifter=(shifter<<1)+bit;
		shiftcount++;
		if (shiftcount==8) {
			buf[size++]=shifter;
			shiftcount=0;
		}
                outb(0xFF,printer_out);
                counter++;
                while ( inb(printer_status)&0x80 )
                        if (kbhit()) return;
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
	sample_riff.freq=sample_riff.bytes_per_sec=speed;
	fwrite(&sample_riff,1,sizeof(sample_riff),out);

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
	for (i=0;i<4;i++) output_bit(1);
}

output_bit(int bit)
{
	fputc(0xC0,out);
	fputc(0x40,out);
	if (bit==0) fputc(0x40,out);
}
