@echo off

:FCPU12
make clean
make RXBUF=16
make RXBUF=16 install

REM Program:2008 bytes (98.0% Full)‚Å2kBˆÈ“à‚ÉŽû‚Ü‚é
:FCPU16DCD
make DCD=1 RXBUF=8 F_CPU=16000000 clean
make DCD=1 RXBUF=8 F_CPU=16000000
make DCD=1 RXBUF=8 F_CPU=16000000 install

avr-size ../bin/firmware/*.hex
:END
