/* Watcom C */
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <i86.h>
#define BLOCKSIZE 1024

int port=1;
int adrport;
void (__interrupt __far *old_handler)();
volatile int tick;

unsigned char *blocks[500]; /* about 15 minutes max tape length */

usage()
{
        printf("Usage: sendtape [-lpt{1|2|3}] <file.tap>\n");
        printf("(connect the specified printer port (lpt1: default) to the Oric's\n");
        printf(" tape port and type CLOAD\"\" on your oric first)\n");
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


sendbit(int bit)
{
        while(!tick);
        tick=0;
        outp(adrport,bit!=0);
}


sendblock(unsigned char *block)
{
        int i;
        for (i=0;i<BLOCKSIZE;i++) {
                sendbit(block[i]&0x80);
                sendbit(block[i]&0x40);
                sendbit(block[i]&0x20);
                sendbit(block[i]&0x10);
                sendbit(block[i]&0x08);
                sendbit(block[i]&0x04);
                sendbit(block[i]&0x02);
                sendbit(block[i]&0x01);
        }
}


main(int argc, char *argv[])
{
        int i;
        FILE *fd;

        printf("SendTape 1.0\n");
        if (argc==1) usage();
        if (argv[1][0]=='-' || argv[1][0]=='/') {
                if (argc==2) usage();
                if (strncmp(argv[1]+1,"lpt",3)==0) {
                        port=atoi(argv[1]+4);
                        if (port<1 || port>3) usage();
                } else usage();
        }
        adrport= ((short far *)0x408)[port-1];

        fd=fopen(argv[argc-1],"rb");
        if (fd==NULL) usage();
        i=0;
        while (!feof(fd)) {
                blocks[i]=(unsigned char *)malloc(BLOCKSIZE);
                if (blocks[i]==NULL) {
                        printf("Not enough memory.\n");
                        exit(2);
                }
                fread(blocks[i],BLOCKSIZE,1,fd); 
                i++;
        }
        fclose(fd);

        printf("Sending data...\n");
        old_handler=_dos_getvect(8);
        _dos_setvect(8,timer_handler);
        init_timer(248);        /* 4800 Hz */
        for (i=0;blocks[i];i++) {
                sendblock(blocks[i]);
        }
        init_timer(0);          /* restore 18.2 Hz */
        _dos_setvect(8,old_handler);
        printf("...done.\n");
}
