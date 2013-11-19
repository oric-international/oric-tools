This program translates Oric-MCP40 plotter output (also known as Tandy CGP115
or Atari-1020 or Commodore-1520) into a "virtual-sheet-of-paper" ((TM) ;-) in BMP format.

Here is a sample usage, that will create file Sheet1.bmp, which you can view with your favorite viewer:

mcp2bmp demo.txt

You can rename the program to a more familiar name if you are a Commodore/Atari/Tandy/other fan...

Windows and Linux users may also associate a file extension for use with the program: for example, if you make your favorite emulator generate .mcp printer outputs, and associate the .mcp file extension to this program, then double-clicking a printer output of your emulator will automatically generate a virtual sheet of paper in the same directory. Existing sheets are not overwritten: the program generates Sheet1.bmp, Sheet2.bmp, Sheet3.bmp, etc.

Have fun,

Euphoric.

NB: mcp2bmp doesn't emulate the 6805 code found in these plotters (I don't know how to read this code from the mcu).
So, if you find behavior differences, please report them...
