Euphoric uses a specific format for its disk images, called MFM format.
It contains all the data bits that are encoded on a Double Density disk in MFM
format (but not the clock transitions).

On the PC, floppy disk images also exists, but they are usually in RAW format,
which means that only the contents of the sectors is stored.

So the raw2mfm tool will allow you to convert a PC floppy disk image to a disk
image that you can use with Euphoric (of course you will need special Oric
tools to read or write it in the emulator)...

In the other hand, the MFM format is not always very practical when you want
to analyze the contents of a disk : sectors are separated by gaps, so for
example if you use an hex editor, the sectors are not contiguous.
mfm2raw will thus extract the contents of the sectors and write it
contiguously in a raw file.
