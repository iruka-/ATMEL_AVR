@echo off
REM Written by senshu (2008/11/12)
pushd ..
echo ======= Build bootloadHID.exe =======
cd commandline
make clean
make
make install
make clean
cd ..
echo ======= Build hidmon88.exe =======
cd src
make clean
make
make install
make clean
cd ..
echo ======= Build firmware =======
del bin\firmware\main*.hex
cd firmware

REM make  (ATmega88-20MHz)
make clean
make
make install
make clean

REM make  (ATmega88-12)
make F_CPU=12000000 clean
make F_CPU=12000000
make F_CPU=12000000 install
make F_CPU=12000000 clean

REM make  (ATmega88-RC)
make F_CPU=12800000 clean
make F_CPU=12800000
make F_CPU=12800000 install
make F_CPU=12800000 clean

REM make (ATmega168-20MHz)
make MCU=m168 clean
make MCU=m168
make MCU=m168 install
make MCU=m168 clean

REM make (ATmega168-12MHz)
make MCU=m168 F_CPU=12000000 clean
make MCU=m168 F_CPU=12000000
make MCU=m168 F_CPU=12000000 install
make MCU=m168 F_CPU=12000000 clean

REM make (ATmega168-RC)
make MCU=m168 F_CPU=12800000 clean
make MCU=m168 F_CPU=12800000
make MCU=m168 F_CPU=12800000 install
make MCU=m168 F_CPU=12800000 clean

cd ..
echo ======= Build test firmware =======
cd test
REM make  (ATmega88-RC)
make F_CPU=12800000 clean
make F_CPU=12800000
make F_CPU=12800000 install
make F_CPU=12800000 clean

REM make (ATmega88)
make F_CPU=20000000 clean
make F_CPU=20000000
make F_CPU=20000000 install
make F_CPU=20000000 clean

REM make (ATmega168-RC)
make MCU=m168 F_CPU=12800000 clean
make MCU=m168 F_CPU=12800000
make MCU=m168 F_CPU=12800000 install
make MCU=m168 F_CPU=12800000 clean

REM make (ATmega168)
make MCU=m168 F_CPU=20000000 clean
make MCU=m168 F_CPU=20000000
make MCU=m168 F_CPU=20000000 install
make MCU=m168 F_CPU=20000000 clean

cd ..

echo ======= All update. =======
popd
pause
