#
#	Windows用です.
#
#	コンパイラは minGw gccを使用します。
#	試してはいませんがcygwinでも大丈夫なはずです。
#
EXE_SUFFIX = .exe

CC		= gcc
USBFLAGS = -DHIDMON88=1
CFLAGS	= $(USBFLAGS) -O2 -Wall
LIBS	= $(USBLIBS)
WIN32LIB= -lkernel32 -luser32 -lgdi32

TARG = hidmon88
OBJS = gr.o hidasp.o monit.o util.o

PROGRAM = $(TARG)$(EXE_SUFFIX)

all: $(PROGRAM)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJS)
	$(CC) -s -o $(PROGRAM) $(OBJS) $(WIN32LIB) -lsetupapi $(LIBS)

strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f *.o $(PROGRAM)

install:
	cp $(PROGRAM) ../bin
#
###
gr.o: gr.h
hidasp.o: usbhid.h hidasp.h avrspx.h ../firmware/hidcmd.h
monit.o: monit.h util.h hidasp.h gr.h portlist.h portlist_mega88.h
util.o: hidasp.h monit.h util.h ../firmware/hidcmd.h
