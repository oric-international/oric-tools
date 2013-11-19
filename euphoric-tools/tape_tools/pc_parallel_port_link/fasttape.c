#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <i86.h>

#define BLOCKSIZE 1024
#define NONE 0
#define ODD 1
#define EVEN 2

int port=1;
int adrport;
int stopbits=1;
int parity=NONE;
int speed=4800;
char paritytab[256];

void (__interrupt __far *old_handler)();
volatile int tick;

unsigned char block[BLOCKSIZE];

usage()
{
        printf("Usage: fasttape [options] <file.tap>\n");
        printf("(use sendtape to transfer a loader to your oric first) \n");
        printf("Options:\n");
        printf("\t-lpt{1|2|3} use the specified parallel port (default lpt1:)\n");
        printf("\t-speed:nnn use the specified baud rate (default 4800)\n");
        printf("\t-parity:{even|odd|none} use the specified parity (default none)\n");
        printf("\t-stopbits:n use the specified number of stop bits (default 1)\n");
        exit(1);
}

init_timer(int val)
{
        outp(0x40,val&0xFF);
        outp(0x40,val>>8);
}

void __interrupt timer_handler()
{
        tick++;
        outp(0x20,0x20);
}


init_paritytab(int parity)
{
        int i,bit,sum;
        if (parity==NONE) return;
        for (i=0;i<256;i++) {
                sum=0;
                for (bit=0x80;bit;bit>>=1) sum+= (i&bit)!=0;
                paritytab[i]= (sum^parity) & 1;
        }
}

sendbit(int bit)
{
        while(!tick);
        tick=0;
        outp(adrport,bit?0xff:0);
}


sendblock()
{
        int i,j;
        old_handler=_dos_getvect(8);
        _dos_setvect(8,timer_handler);
        init_timer(1193182L/speed);
        for (i=0;i<BLOCKSIZE;i++) {
                sendbit(0);             /* start bit */
                sendbit(block[i]&0x01);
                sendbit(block[i]&0x02);
                sendbit(block[i]&0x04);
                sendbit(block[i]&0x08);
                sendbit(block[i]&0x10);
                sendbit(block[i]&0x20);
                sendbit(block[i]&0x40);
                sendbit(block[i]&0x80);
                if (parity) sendbit(paritytab[i]);
                for (j=0;j<stopbits;j++) sendbit(1); /* stop bits */
        }
        init_timer(0);          /* restore 18.2 Hz */
        _dos_setvect(8,old_handler);
}


main(int argc, char *argv[])
{
        int i;
        FILE *fd;

        printf("FastTape 1.1\n");
        if (argc==1) usage();
        for (i=1;i<argc && (argv[i][0]=='-' || argv[1][0]=='/'); i++) {
                if (strncmp(argv[i]+1,"lpt",3)==0) {
                        port=atoi(argv[i]+4);
                        if (port<1 || port>3) usage();
                } else if (strncmp(argv[i]+1,"speed:",6)==0) {
                        speed=atoi(argv[i]+7);
                } else if (strncmp(argv[i]+1,"parity:",7)==0) {
                        if (strcmp(argv[i]+8,"even")==0) parity=EVEN;
                        else if (strcmp(argv[i]+8,"odd")==0) parity=ODD;
                        else if (strcmp(argv[i]+8,"none")==0) parity=NONE;
                        else usage();
                        init_paritytab(parity);
                } else if (strncmp(argv[i]+1,"stopbits:",9)==0) {
                        stopbits=atoi(argv[i]+10);
                } else usage();
        }
        if (i==argc) usage();

        adrport= ((short far *)0x408)[port-1];
        outp(adrport,1);        /* initialize mark level */

        fd=fopen(argv[argc-1],"rb");
        if (fd==NULL) usage();

        printf("Sending data...\n");
        while (!feof(fd)) {
                fread(block,BLOCKSIZE,1,fd); 
                sendblock();
        }
        fclose(fd);
        printf("...done.\n");
}
