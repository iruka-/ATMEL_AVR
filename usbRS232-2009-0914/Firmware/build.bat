@echo off
goto FCPU16DCD

:FCPU12
make clean
make RXBUF=16
goto END

REM Program:2058 bytes (100.5% Full)でサイズオーバー
:FCPU12DCD
make clean
make DCD=1 RXBUF=16
goto END

:FCPU16
make RXBUF=8 F_CPU=16000000 clean
make RXBUF=8 F_CPU=16000000
goto END

REM Program:2008 bytes (98.0% Full)で2kB以内に収まる
:FCPU16DCD
make DCD=1 RXBUF=8 F_CPU=16000000 clean
make DCD=1 RXBUF=8 F_CPU=16000000

:END
