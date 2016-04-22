@echo off
REM make HIDaspx (MONITOR only)
make clean
make MAIN=main-small
make MAIN=main-small install
REM make for libusb
make -f Makefile.unix clean
make -f Makefile.unix
make -f Makefile.unix install
REM make default (HIDaspx)
make clean
make
make install
