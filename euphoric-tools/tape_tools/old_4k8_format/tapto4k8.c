#include <stdio.h>
FILE *inp, *out;
int c;

main(int argc, char *argv[])
{
	if (argc!=3) {
		printf("Usage: %s <source (virtual-tape)> <destination (4800Hz-sampled-tape)>\n",argv[0]);
		exit(1);
	}
	inp=fopen(argv[1],"rb");
	if (inp==NULL) {
		printf("Unable to open %s\n",argv[1]);
		exit(1);
	}
	out=fopen(argv[2],"wb");
	if (out==NULL) {
		printf("Unable to open %s for writing\n",argv[2]);
		exit(1);
	}
	while ((c=fgetc(inp))!=EOF) {
		find_synchro();
		output_big_synchro();
		output_file();
	}
	flush_output();
}

find_synchro()
{
	int synchro_state=0;
	int i;
	for (i=0;i<5;i++) output_half_period(1);
	while (1) {
		if (c==0x16) {
			if (synchro_state<3) synchro_state++;
		} else if (c==0x24 && synchro_state==3) return;
		else synchro_state=0;
		output_byte(c);
		c=fgetc(inp);
	}
}

output_big_synchro()
{
	int i;
	for (i=0;i<256;i++) output_byte(0x16);
	output_byte(0x24);
}

output_file()
{
	unsigned char header[9];
	unsigned start,end;
	unsigned i;

	for (i=0;i<9;i++) {
		header[i]=fgetc(inp);
		output_byte(header[i]);
	}
	start=header[6]*256+header[7];
	end=header[4]*256+header[5];
	while (c=fgetc(inp)) output_byte(c);
	output_byte(0);
	for (i=0;i<6;i++) output_half_period(1);
	for (i=start;i<=end;i++) output_byte(fgetc(inp));
	for (i=0;i<2;i++) output_half_period(1);
}

output_byte(int c)
{
	int i, bit, parity=0;
	output_half_period(1);
	output_bit(0);
	for (i=1;i<256;i<<=1) {
		bit= c&i?1:0;
		output_bit(bit);
		parity^=bit;
	}
	output_bit(parity^1);
	output_bit(1);
	output_bit(1);
	output_bit(1);
}

output_bit(int b)
{
	output_half_period(1);
	output_half_period(b?1:2);
}

int current_level=0;
int shifter;
int shift_count=0;

output_half_period(int length)
{
	int i;
	for (i=0;i<length;i++) {
		shifter=(shifter<<1)+current_level;	
		shift_count++;
		if (shift_count==8) {
			shift_count=0;
			fputc(shifter,out);
		}
	}
	current_level^=1;
}

flush_output()
{
	int i;
	for (i=0;i<8-shift_count;i++)
		shifter=(shifter<<1)+1;	
	fputc(shifter,out);
}
