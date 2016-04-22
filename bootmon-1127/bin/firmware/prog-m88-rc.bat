@echo off
@echo ATmega88 setting FUSE (RC mode).
hidspx -d10 -e
hidspx -d10 -FL0xA2 -FH0xDD -FX0xF8
@echo Write bootloader(not include hidmon88)
hidspx -d1 main88-RC.hex
echo "bootmonHID main88-test-RC.hex"
