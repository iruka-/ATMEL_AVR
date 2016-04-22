rem 
rem カレントDIRにある main-0000.hexをCQ-STARMに書き込んで自動的に終了する.
rem
openocd.exe -f blaster.cfg -f stm32.cfg -f batch.cfg
rem pause