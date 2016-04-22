@echo off
echo Build bootmon for ATmega88(RS-OSC mode).
make F_CPU=12800000 clean
make F_CPU=12800000
