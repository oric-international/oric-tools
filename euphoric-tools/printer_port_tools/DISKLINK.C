/* GNU compiler */
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <pc.h>

usage()
{
        printf("Usage: disklink [-lpt{1|2|3}] [<filename.dsk>]\n");
        exit(1);
}

unsigned char buf[6400]; /* room for a track */
unsigned int size;
unsigned int sides, tracks;
int geometry=1;
FILE *out;
char *filename="image.dsk";
short printer_ports[3];
int port, printer_out, printer_status;


main(int argc, char *argv[])
{
        int n,side,track;

        printf("DiskLink 2.0\n");
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

        printf("Type RUN on the Oric\n");
        receive_track();
        sides=buf[0]; tracks=buf[1];
        if (sides>2 || tracks>82) {
                printf("Bad parameters received (%d side(s), %d tracks).\n"
                        ,sides,tracks);
                exit(1);
        }
        fprintf(out,"MFM_DISK");
        fwrite(&sides,4,1,out);
        fwrite(&tracks,4,1,out);
        fwrite(&geometry,4,1,out);
        memset(buf,0,6400);
        fwrite(buf,256-20,1,out);

        for (side=0;side<sides;side++)
                for (track=0;track<tracks;track++) {
                        cprintf("Side %d Track %2d\r",side,track);
                        receive_track();
                        fwrite(buf,6400,1,out);
                }
        printf("%s has been written.\n",filename);
}

receive_track()
{
        int n;
        outportb(printer_out,0xFF);
        for (n=0;n<6400;n++) buf[n]=receive_byte();
}

receive_byte()
{
        unsigned int high,low;
        static int oric_started=0;

        do {
                while ( !(inb(printer_status)&0x80) ) ;
                low= ( inb(printer_status) & 0x78 ) >> 3;
                outportb(printer_out,0);
                outportb(printer_out,0xFF);
                if (!oric_started && low) oric_started=1;
        } while (!oric_started);

        while ( inb(printer_status)&0x80 ) ;
        high= ( inb(printer_status) & 0x78 ) << 1;
        outportb(printer_out,0);
        outportb(printer_out,0xFF);

        return high|low;
}

receive_nibble()
{
        int nibble;
        return nibble;
}

