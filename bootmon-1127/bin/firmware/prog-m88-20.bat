@echo off
@echo ATmega88 setting FUSE (Xtal 20MHz mode).
hidspx -d10 -e
hidspx -d10 -FL0xFF -FH0xDD -FX0xF8
@echo Write Bootloader and hidmon88
hidspx -d1 main88-20.hex
@echo Write bootloader and hidmon88 Firmware
echo "bootmonHID main88-test-20.hex"
