# oric-tools
Mod of disk_image_tools\sedoric_tool\tap2dsk.c to support tracks &lt;= 21 in *.dsk files (-> file size &lt;= 128kB)

1. Microsoft Visual Studio 2015 Update 3 solution and project (x64) to be able to compile tap2dsk with MS compiler.
2. Support paper color and ink color (sync with OSDK)
3. Add define DIRTRACK 20 and replace values 20 by define to anable the next step.
4. Change DIRTRACK 20 and NBTRACKS 21 to 15 and 16 to get *.dsk files smaller than 128kB.
