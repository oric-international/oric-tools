CC = gcc

all: sheet plotter mcp2bmp 


sheet:
	$(CC) -I. -c -o sheet.o sheet.c

plotter:
	$(CC) -I. -c -o plotter.o plotter.c

mcp2bmp:
	$(CC) -I. -o mcp2bmp mcp2bmp.c plotter.o sheet.o

clean:
	rm *.o
