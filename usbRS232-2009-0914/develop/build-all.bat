@echo off
REM Written by senshu (2008/11/04)
pushd ..

cd firmware
echo ======= firmware build =======
make clean
make
make install

REM Program:2008 bytes (98.0% Full)‚Å2kBˆÈ“à‚ÉŽû‚Ü‚é
:FCPU16DCD
make DCD=1 F_CPU=16000000 clean
make DCD=1 F_CPU=16000000
make DCD=1 F_CPU=16000000 install

make clean
make DCD=1 F_CPU=16000000 clean

cd ..
echo ======= All update. =======
popd
avr-size ../bin/firmware/*.hex
pause
