Several years ago, Euphoric used a different format than nowadays for its
disks images. The old format was somewhat similar to the raw format used for
floppy disk images on a PC, except that the variable number of sectors and
tracks used by DOS, Sedoric, FT-DOS, etc obliged me to add a header to the
disk image, providing the number of sectors, tracks and sides...

This was before discovering that RANDOS uses a combination of 256-bytes
sectors and 512-bytes sectors, and that other DOS have protections that cannot
be emulated without a more precise format, so I introduced the MFM disk
images.

This tool allows to convert disk images from the old format to the new one.
