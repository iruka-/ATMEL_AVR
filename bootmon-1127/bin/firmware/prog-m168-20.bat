@echo off
@echo ATmega168 setting FUSE (Xtal 20MHz mode).
hidspx -d10 -e
hidspx -d10 -FL0xFF -FH0xDD -FX0xF8
@echo Write bootloader and hidmon88 Firmware
hidspx -d1 main168-20.hex
echo "bootmonHID main168-test-20.hex"
