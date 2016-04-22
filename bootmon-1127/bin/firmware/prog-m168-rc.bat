@echo off
@echo ATmega168 setting FUSE (RC mode).
hidspx -d10 -e
hidspx -d10 -FL0xA2 -FH0xDD -FX0xF8
@echo Write Bootloader)not include hidmon88)
hidspx -d1 main168-RC.hex
echo "bootmonHID main168-test-RC.hex"
