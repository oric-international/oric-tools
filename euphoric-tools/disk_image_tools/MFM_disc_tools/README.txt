Two simple tools :

buildmfm has been used for 3" disks transfers: disks are read on the oric, one
track at a time, and the tracks are saved on a tape. The tape image is then
converted back by buildmfm to a MFM disk image.

writeblock has been used by new-generation Oric developpers: a program taken
from a tape image is written on side 2 of a disk image, in contiguous sectors.
This allows programs that have their own disk read routines to reclaim the
space in overlay ram.
