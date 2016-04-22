rem
rem OpenOCD: HIDaspxによるJTAGアダプターで STM32マイコン(CQ-STARMなど)に接続する.
rem
openocd.exe -f blaster.cfg -f stm32.cfg 

rem openocd.exe -f blaster.cfg -f stm32.cfg -f batch.cfg
rem pause