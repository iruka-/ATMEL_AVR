#
#	Mingw ‚Å E-Nash ‚³‚ñ‚Ìpkclock ‚ğì‚éB
#
.c.o:
	gcc -c -mwindows -O2 $< -o $*.o
#
sources = \
	pkclock.c	\
      win32.core.c	\
      win32.debug.c	\
      win32.main.c	\
      win32.system.c	\
      stdafx.c


objs	= $(sources:.c=.o)


pkclock.exe : $(objs) pkclock.res
	gcc -mwindows $(objs) pkclock.res -lwinmm -o pkclock.exe


pkclock.res : win32.rc
	windres $< -O coff -o $@
