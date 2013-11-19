#include <stdio.h>

char *keywords[]= { 
"END","EDIT","STORE","RECALL","TRON","TROFF","POP","PLOT",
"PULL","LORES","DOKE","REPEAT","UNTIL","FOR","LLIST","LPRINT","NEXT","DATA",
"INPUT","DIM","CLS","READ","LET","GOTO","RUN","IF","RESTORE","GOSUB","RETURN",
"REM","HIMEM","GRAB","RELEASE","TEXT","HIRES","SHOOT","EXPLODE","ZAP","PING",
"SOUND","MUSIC","PLAY","CURSET","CURMOV","DRAW","CIRCLE","PATTERN","FILL",
"CHAR","PAPER","INK","STOP","ON","WAIT","CLOAD","CSAVE","DEF","POKE","PRINT",
"CONT","LIST","CLEAR","GET","CALL","!","NEW","TAB(","TO","FN","SPC(","@",
"AUTO","ELSE","THEN","NOT","STEP","+","-","*","/","^","AND","OR",">","=","<",
"SGN","INT","ABS","USR","FRE","POS","HEX$","&","SQR","RND","LN","EXP","COS",
"SIN","TAN","ATN","PEEK","DEEK","LOG","LEN","STR$","VAL","ASC","CHR$","PI",
"TRUE","FALSE","KEY$","SCRN","POINT","LEFT$","RIGHT$","MID$"
};

unsigned char buf[48192];

main(int argc, char **argv)
{
        unsigned int i=0, namelength=0, car;
	unsigned char *header,*name,*prog;
        FILE *in;
        if (argc!=2) { 
                fprintf(stderr,"Usage : bas2txt <Oric-BASIC-file>\n");
                exit(1);
        }
        in=fopen(argv[1],"rb");
        if (in==NULL) { fprintf(stderr,"Can't open file\n"); exit(1); }

        fread(buf,1,48192,in); fclose(in);
	while (buf[i]==0x16) i++;
	if (buf[i++]!=0x24) {
                fprintf(stderr,"Not an Oric file\n");
                exit(1);
        }
	header = buf+i;
        if (header[2]) { fprintf(stderr,"Not a BASIC file\n"); exit(1); }
	name = header+9;
	while(name[namelength])
		namelength++;
	prog = name + namelength+1;
	i=0;
        while(prog[i] || prog[i+1]) {
                i+=2;
                printf(" %u ",prog[i]+(prog[i+1]<<8));
                i+=2;
                while(car=prog[i++]) {
                        if (car<128) putchar(car);
                        else printf("%s",keywords[car-128]);
                }
                putchar('\n');
        }
}
