/* wavclean tool by F.Frances */
/* second version adapted by S.Guyart 06/2007*/
/* third version with more accurate sampling for long periods by F.Frances 11/2007 */
#include <stdio.h>


struct riff {
	char sig[4];
	int riff_size;
	char datasig[4];
	char fmtsig[4];
	int fmtsize;
	short tag;
	short channels;
	int freq;
	int bytes_per_sec;
	short byte_per_sample;
	short bits_per_sample;
	char samplesig[4];
	int length;
};
struct riff sample_riff= {"RIFF",0,"WAVE","fmt ",16,1,1,4800,4800,1,8,"data",0};
struct riff header;

FILE *in, *out;
int size;
int THRESHOLD=23; /* a short period should be 18 samples at 44.1 kHz */
                  /* a long one at least 27 */
enum { CLASSIC, LOW_LEVEL } method; /* more methods might be added in the future */

init(int argc, char *argv[])
{
	if (argc<3) return 1;
	if (argv[1][0]=='-') {
		if (feed_parameters(argv[1])) return 1;
	}
	if (argv[2][0]=='-') {
		if (feed_parameters(argv[2])) return 1;
	}

	in=fopen(argv[argc-2],"rb");
	if (in==NULL) {
		printf("Cannot open %s file\n\n",argv[argc-2]);
		return 1;
	}
	out=fopen(argv[argc-1],"wb");
	if (out==NULL) {
		printf("Cannot create %s file\n\n",argv[argc-1]);
		return 1;
	}
	return 0;
}

int feed_parameters(char *param)
{
	switch (param[1]) {
		case 'c':
		case 'C':
			method = CLASSIC;
			break;
		case 'l':
		case 'L':
			method = LOW_LEVEL;
			break;
		case '1':
		case '2':
			THRESHOLD = atoi(param+1);
			if (THRESHOLD<18 || THRESHOLD>27) {
				printf("Bad value: %s\n",param+1);
				return 1;
			}
			break;
		default:
			printf("Bad parameter: %s\n", param);
			return 1;
	}
	return 0;
}


main(int argc,char *argv[])
{	
	int i;
	if (init(argc,argv)) {
		printf("\nWavclean 1.3  (c) 11/2007\n");
		printf("Usage: %s [ -c | -l ] [ -xx ] <.TAP file> <.WAV file>\n",argv[0]);
		printf("Options: try first without using options. If there are loading errors\n");
		printf("         or the file can't be loaded at all, try the following.\n");
		printf("  -c  classic decoding method (default)\n");
		printf("  -l  low-level decoding if classic fails (try classic 1st!)\n\n");
		printf("  -xx threshold value, xx between 18 and 27\n");
		printf("      only modify threshold if you think your tape player played\n");
		printf("      the tape too fast (then reduce threshold value) or too slow\n");
		printf("      (then increase threshold value)\n\n");
		exit(1);
	}
	fread(&header,sizeof(struct riff),1,in);
	if (header.channels!=1 || header.freq!=44100 || header.bits_per_sample!=8) {
		printf("Invalid WAV format: should be 44100 Hz, 8-bit, mono\n");
		exit(1);
	}

	fwrite(&sample_riff,sizeof(struct riff),1,out);
	switch (method) {
		case LOW_LEVEL:
				convert_low(); break;
		case CLASSIC:
		default:
				convert_classic(); break;
	}

	rewind(out);
	sample_riff.length=size;
	sample_riff.riff_size=sample_riff.length+36;
	fwrite(&sample_riff,1,sizeof(struct riff),out);
	printf("Wavclean 1.1  (c) 06/2007 --- File converted.\n");
}


convert_classic()
{
	int max=0,min=255,up=0,thres=128,last=0;
	int val,length1,length2;
	while(1) {
		length1=length2=0;
		while(1) {
			val=getc(in);
			if (val==EOF) return;
			if (val>max) max=val;
			if (val<last && up) { up=0; thres=(max+min)/2; min=255;}
			last=val;
			if (val>thres) length1++;
			else break;
		}
		while(1) {
			val=getc(in);
			if (val==EOF) return;
			if (val<min) min=val;
			if (val>last && !up) { up=1; thres=(max+min)/2; max=0; }
			last=val;
			if (val<thres) length2++;
			else break;
		}
		if (length1+length2<THRESHOLD) { /* short period : 1+1 */
			output_level(1);
			output_level(0);
		} else if (length1+length2<THRESHOLD+9) { /* intermediate period : 1+2 or 2+1 */
			output_level(1);
			output_level(length1>length2 ? 1 : 0);
			output_level(0);
		} else if (length1+length2<THRESHOLD+18) { /* long period : normally 2+2 */
			output_level(1);
			output_level(length1>THRESHOLD-9 ? 1 : 0);
			output_level(length1>THRESHOLD ? 1 : 0);
			output_level(0);
		} else {	/* very long period */
			int full_length = 3 + (length1+length2-THRESHOLD)/9;
			int length_level1 = (length1+4)/9;
			int i;
			if (length_level1==full_length) length_level1--;
			for (i=0; i<length_level1; i++) output_level(1);
			for (; i<full_length; i++) output_level(0);
		}
    }
}


convert_low()
{
   int max=0,min=255,up=0,thres=128,last=0;
   int val,valprev,length1,length2;

   valprev=getc(in);
   if (valprev==EOF) return;

   //recherche du 1er minimum
   while(1) {
            val=getc(in);
            if (val==EOF) return;
            if ((val>valprev) && (val<127))
               { min=valprev;
                 break;
               }
            else
               { valprev=val;
               }
            }

   while(1) {
        length1=length2=0;
        while(1) {
                 val=getc(in);
                 if (val==EOF) return;
                 if (val>=valprev)
                    { max=val;
                      length1++;
                      valprev=val;
                    }
                 else
                    { max=valprev;
                      length1++;
                      valprev=val;
                      break;
                    }
                 }

        while(1) {
                 val=getc(in);
                 if (val==EOF) return;
                 if (val<=valprev)
                    { length1++;
                      valprev=val;
                    }
                 else
                    { min=valprev;
                      length1++;
                      valprev=val;
                      break;
                    }
                 }
	output_level(1);
	output_level(0);
	if (length1+length2>=THRESHOLD) output_level(0);
	if (length1+length2>10*THRESHOLD) output_silence((length1+length2)/9);
    }
}

output_level(int level)
{
	putc(level==1?0xC0:0x40,out);
	size++;
}

output_silence(int length)
{
	int i;
	for (i=0;i<length;i++) putc(0x80,out);
	size+=length;
}
