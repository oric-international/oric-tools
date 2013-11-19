#include <stdio.h>

char *keywords[3][119]= {
	{ // table mots-clés rom 1.0
		"END","EDIT","NORMAL","INVERSE","TRON","TROFF","POP","PLOT",
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
	},
	{ // table mots-clés rom 1.1
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
	},
	{ // table mots-clés Basic Evolution
		"END","SUBR","WHILE","RECALL","TRON","TROFF","POP","PLOT",
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
	}
};

unsigned char buf[48*1024];
unsigned char head[14]={ 0x16,0x16,0x16,0x24,0,0,0,0,0,0,5,1,0,0 };

main(int argc, char *argv[])
{
	unsigned int i, car, number=0, end, lastptr, adr;
	int j,ptr,keyw,string,rem,data;
	unsigned char ligne[256];
	int version=1,options=0;
	FILE *in,*out;
	if (argc>1 && argv[1][0]=='-') options++;
	if (argc!=options+3) { 
		printf("Usage : txt2bas [-v0|-v1|-v2] txtfile <Oric-BASIC-file>\n");
		printf("Options :\n");
		printf("\t-v0 : Oric-1 keywords\n");
		printf("\t-v1 : Atmos keywords\n");
		printf("\t-v2 : Basic Evolution keywords, and auto-numbering\n");
		exit(1);
	}
	if (options) {
		if (argv[1][1]!='v' || (version=argv[1][2]-'0')<0 || version>2 || argv[1][3]) {
			printf("Bad option: %s\n",argv[1]);
			exit(1);
		}
	}

	in=fopen(argv[options+1],"r");
	if (in==NULL) { printf("Can't open input file %s\n",argv[options+1]); exit(1); }
	out=fopen(argv[options+2],"wb");
	if (out==NULL) { printf("Can't open file %s for writing\n",argv[options+2]); exit(1); }

	i=0;
	while(fgets(ligne,255,in)!=NULL) {
		ptr=0; rem=0; string=0; data=0;
		if (version<2) {
			number=0;
			while (ligne[ptr]==' ') ptr++;
			while (isdigit(ligne[ptr]))
				number=number*10+(ligne[ptr++]-'0');
		} else
			number++;

		buf[i++]=0; buf[i++]=0; // link to next line
		buf[i++]=number&0xFF; buf[i++]=number>>8; //line number

		while (ligne[ptr]==' ')
			if (version<2) ptr++;
			else buf[i++]=ligne[ptr++];

		while (ligne[ptr]!='\0' && ligne[ptr]!='\n') {
			if (rem) {
				buf[i++]=ligne[ptr++];
		    } else if (string) {
				if (ligne[ptr]=='"') string=0;
				buf[i++]=ligne[ptr++];
		    } else if (data) {
				if (ligne[ptr]==':') data=0;
				if (ligne[ptr]=='"') string=1;
				buf[i++]=ligne[ptr++];
		    } else if (ligne[ptr]==' ') {
/*
				if (version<2)
					buf[i++]=ligne[ptr++];
				else
*/
					ptr++;
			} else {
				keyw=search_keyword(version,ligne+ptr);
				if (keyw==29 || ligne[ptr]=='\'') rem=1;
				if (keyw==17) data=1;
				if (ligne[ptr]=='"') string=1;
				if (keyw>=0) {
					buf[i++]=keyw+128; ptr+=strlen(keywords[version][keyw]);
				} else {
					if (ligne[ptr]<0x20 || ligne[ptr]>0x7E) {
						int line=number;
						if (version>=2) line=number/10;
						printf("Bad character in line %d, column %d : %c\n",line,ptr,ligne[ptr]);
						exit(1);
					} else {
						if (version<2 || !isalpha(ligne[ptr]))
							buf[i++]=ligne[ptr++];
						else
							while (isalnum(ligne[ptr])) buf[i++]=ligne[ptr++];
					}
				}
		    }
		}
		buf[i++]=0;
	}
	buf[i++]=0; buf[i++]=0; buf[i]=0x55;
	end=0x501+i; head[8]=end>>8; head[9]=end&0xFF;
	for(j=4,lastptr=0;j<i;j++)
		if (buf[j]==0) {
			adr=0x501+j+1; 
			buf[lastptr]=adr&0xFF; buf[lastptr+1]=adr>>8;
			lastptr=j+1;
			j+=4;
		}
	fwrite(head,1,14,out);
	fwrite(buf,1,i+1,out); fclose(out);
}

search_keyword(int ver,char *str)
{
	int i;
	for (i=0;i<119;i++) {
		int len=strlen(keywords[ver][i]);
		if (strncmp(keywords[ver][i],str,len)==0) {
			if (ver<2 || !isalnum(str[len-1]) || !isalnum(str[len])) return i;
		}
	}
	return -1;
}

/*
int isalphanum(char c)
{
	return (c>='0' && c<='9') || (c>='A' && c<='Z') || (c>='a' && c<='z');
}
*/
