@echo off
REM Written by senshu (2008/11/04)
pushd ..
echo ======= hidspx.exe build =======
cd src
make clean
make
make install
make clean
REM
make -f Makefile.gcc clean
make -f Makefile.gcc
make -f Makefile.gcc install
make -f Makefile.gcc clean
cd ..
echo ======= firmware build =======
cd firmware
REM make HIDaspx (MONITOR only)
make MAIN=main-small clean
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
make clean
make MAIN=main-small clean
make -f Makefile.unix clean
cd ..
echo ======= All update. =======
popd
sleep 5
