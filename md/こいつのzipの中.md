[2009-04](2009-04.md) 

The directory tree and content of the sample software
===================

common
	   -- inc
	-- lpc23xx.h		Definition file of all the registers of LPC23xx
				The LPC24xx register definition is also included. 
	-- irq.h		Interrupt related definitions and API prototyping
	-- type.h		Type definition
	-- target.h		Target specific definition for Keil's MCB2300 board
	-- timer.h		Definition and header file for timer module
	   -- src
	-- startup.s		Code start up entry
	-- swi_handler.s	SWI handler
				Note: not used, but a good example for future references. 
	-- irq.c		Generic interrupt handler related APIs, handle both IRQ and FIQ. 
	-- target.c		Target specific modules
	-- timer.c		Timer related APIs

GPIO
	   -- fio.h			GPIO and fast I/O header
	   -- fio.c			GPIO and fast I/O APIs
	   -- fiotest.c		GPIO and Fast I/O test module
	   -- fio.uv2			uVision project file


EXTINT
	   -- extint.h			External interrupt header
	   -- extint.c			External interrupt APIs
	   -- einttest.c		External interrupt test module
	   -- extint.uv2		uVision project file

Timer
	   -- tmrtest.c		Timer test module, note: API modules
				are in the COMMON directory and 
				shared and used by some other peripheral testing.
	   -- timer.uv2		uVision project file

PWM
	   -- pwmc.h			PWM header
	   -- pwm.c			PWM APIs
	   -- pwmtest.c		PWM test module
	   -- pwm.uv2			uVision project file

SPI
	   -- spi.h			SPI header
	   -- spi.c			SPI APIs
	   -- spitest.c		SPI test module
	   -- spi.uv2			uVision project file

SSP
	   -- ssp.h			SSP(SPI1) header
	   -- ssp.c			SSP(SPI1) APIs
	   -- dma.h			SSP DMA header
	   -- dma.c			SSP DMA APIs
	   -- ssptest.c		SSP(SPI1) test module
	   -- ssp.uv2			uVision project file

UART
	   -- uart.h			UART header
	   -- uart.c			UART APIs
	   -- uarttest.c		UART test module
	   -- uart.uv2			uVision project file

RTC
	   -- rtc.h			Real-time clock(RTC) header
	   -- rtc.c			Real-time clock(RTC) APIs
	   -- rtctest.c		Real-time clock(RTC) test module
	   -- rtc.uv2			uVision project file

MCI
	   -- mci.h			MCI header
	   -- mci.c			MCI APIs for both SD and MMC card interface
	   -- dma.h			MCI DMA header
	   -- dma.c			MCI DMA APIs.
	   -- mcitest.c		MCI test module
	   -- mci.uv2			uVision project file

ADC
	   -- adc.h			ADC header
	   -- adc.c			ADC APIs
	   -- adctest.c		ADC controller test module
	   -- adc.uv2			uVision project file

DAC
	   -- dac.h			DAC header
	   -- dac.c			DAC APIs
	   -- dactest.c		DAC test module
	   -- dac.uv2			uVision project file

I2C
	   -- i2c.h			I2C header
	   -- i2c.c			I2C APIs
	   -- i2cmst.c			I2C test module
	   -- i2cmst.uv2		uVision project file

I2S
	   -- i2s.h			I2S header
	   -- i2s.c			I2S APIs
	   -- dma.c			I2S DMA APIs
	   -- dma.h			I2S DMA header
	   -- i2stest.c		I2S test module
	   -- i2s.uv2			uVision project file

CAN
	   -- can.h			CAN header
	   -- can.c			CAN APIs
	   -- cantest.c		CAN test module
	   -- can.uv2			uVision project file

DMA
	   -- dma.h			DMA header
	   -- dma.c			GPDMA APIs, memory to memory only.
				Note: memory to peripheral or peripheral
				to memory are in SSP, MCI, and I2S directories.
	   -- dmatest.c		DMA test module
	   -- dma.uv2			uVision project file

WDT
	   -- wdt.h			Watchdog timer header
	   -- wdt.c			Watchdog timer APIs
	   -- wdttest.c		Watchdog timer test module
	   -- wdt.uv2			uVision project file

PWRMAN
	   -- pwrman.h			Power management header
	   -- pwrman.c			Power management APIs 
	   -- pmtest.c			Power management test module,
				test powerdown and external interrupt wakeup.
	   -- pwrman.uv2		uVision project file

PortLCD
	   -- portlcd.h		4-bit port to LCD driver header for Keil MCB2300
	   -- portlcd.c		4-bit port to LCD driver APIs 
	   -- lcdtest.c		4-bit port to LCD test module
	   -- portlcd.uv2		uVision project file

EMAC
	   -- crc32.h			CRC calculation header
	   -- crc32.c			CRC calculation APIs
	   -- emac.h			EMAC header
	   -- emac.c			EMAC APIs, options include transmit only(TX_ONLY);
				receive packet, switch source and destination
				and transmit(BOUNCE_RX), WOL, etc. 
	   -- emactest.c		EMAC test module
	   -- emac.uv2			uVision project file
	   -- ethpkt.txt		Ethernet packet example for transmit

USBDEV
	   -- hid.h			USB HID related header
	   -- hiduser.h		USB HID user specific header
	   -- hiduser.c		USB HID user APIs
	   -- usb.h			USB header
	   -- usbcfg.h			USB configuration header
	   -- usbcore.h		USB Core header
	   -- usbcore.c		USB Core APIs
	   -- usbdesc.h		USB descriptor header
	   -- usbdesc.c		USB descriptor APIs
	   -- usbhw.h			USB hardware header
	   -- usbhw.c			USB hardware APIs
	   -- usbreg.h			USB misc. register header
	   -- usbuser.h		USB user header
	   -- useuser.c		USB user APIs
	   -- usbmain.c		USB HID test module, power management, USB 
				suspend to powerdown, and resume to USB
				wakeup. 
	   -- usbdev.uv2		uVision project file

USBAudio
	   -- audio.h			USB Audio related header
	   -- adcuser.h		USB Audio Device Class user specific header
	   -- adcuser.c		USB Audio Device Class user APIs
	   -- usb.h			USB header
	   -- usbaudio.h		USB Audio misc. definition header
	   -- usbcfg.h			USB configuration header
	   -- usbcore.h		USB Core header
	   -- usbcore.c		USB Core APIs
	   -- usbdesc.h		USB descriptor header
	   -- usbdesc.c		USB descriptor APIs
	   -- usbhw.h			USB hardware header
	   -- usbhw.c			USB hardware APIs
	   -- usbreg.h			USB misc. register header
	   -- usbuser.h		USB user header
	   -- useuser.c		USB user APIs
	   -- usbmain.c		USB Audio test module
	   -- usbaudio.uv2		uVision project file

EXTMEM ( LPC24xx only, tested on Embedded Artists LPC24xx OEM board)
	   -- NOR_Flash
	-- ex_norflash.h	NOR flash configuration header for Embedded Artists LPC2400 
				OEM board
	-- ex_norflash.c	NOR flash configuration and access APIs 
	-- norflash_test.c	NOR flash test module
	-- ex_norflash.uv2	uVision project file
	   -- NAND_Flash
	-- ex_nandflash.h	NAND flash configuration header for Embedded Artists LPC2400 
				OEM board
	-- ex_nandflash.c	NAND flash configuration and access APIs 
	-- nandflash_test.c	NAND flash test module
	-- ex_nandflash.uv2	uVision project file	
	   -- SDRAM
	-- ex_sdram.h		SDRAM configuration header for Embedded Artists LPC2400 
				OEM board
	-- ex_sdram.c		SDRAM configuration and access APIs
	-- uart.h		UART debugging header file for stand-alone SDRAM test
	-- uart.c		UART debugging file for stand-alone SDRAM test 
	-- sdram_test.c		SDRAM test module
	-- ex_sdram.uv2		uVision project file

EA_LCD (Embedded Artists QVGA LCD controller OEM board )
	   -- lcd_hw.h			16-bit port to LCD controller driver header for Embedded Artists
				LPC2400 OEM base board
	   -- lcd_hw.c			16-bit port to LCD controller driver APIs
	   -- lcd_grph.c		Graphic related APIs such as drawing an object such as a line,
				a circle, etc.
	   -- lcd_grph.h		Header file to support lcd_graph.c
	   -- font5x7.c		a 5x7 font for letters and numbers, etc.
	   -- font5x7.h		font related header
	   -- font_macro.h		font macro
	   -- ealcd_test.c		16-bit port to LCD controller test module
	   -- ea_lcd.uv2		uVision project file

USBHost
	   -- usbhost.h		USB host header
	   -- usbhost.c		USB host APIs 
	   -- hcdtest.c		USB host test module,
				USB host initialization only, no queue setup and enumeration yet.
	   -- usbhost.uv2		uVision project file

USBUVC
	   -- lpc2300.s		Startup file for USBUVC project.
				(This stand-alone project doesn't use any of the library files 
				under the common directory.)
	   -- uvc.h			USB Video related header
	   -- uvcuser.h		USB Video Device Class user specific header
	   -- uvcuser.c		USB Video Device Class user APIs
	   -- usb.h			USB header
	   -- usbcfg.h			USB configuration header
	   -- usbcore.h		USB Core header
	   -- usbcore.c		USB Core APIs
	   -- usbdesc.h		USB descriptor header
	   -- usbdesc.c		USB descriptor APIs
	   -- usbhw.h			USB hardware header
	   -- usbhw.c			USB hardware APIs
	   -- usbreg.h			USB misc. register header
	   -- usbuser.h		USB user header
	   -- usbuser.c		USB user APIs
	   -- type.h			Local type definition header
	   -- lcd.h			display support module header
	   -- lcd.c			Display support module
	   -- uvcdemo.c		USB UVC test module
	   -- uvc.uv2			uVision project file
	   -- debug.ini		Debug configuration setting

USBCDC
	   -- lpc2300.s		Startup file for USBUVC project.
				(This stand-alone project doesn't use any of the library files 
				under the common directory.)
	   -- swi.s			SWI handler to handle USB_WriteEP().    
	   -- cdc.h			USB CDC related header
	   -- cdcuser.h		USB CDC Device Class user specific header
	   -- cdcuser.c		USB CDC Device Class user APIs
	   -- usb.h			USB header
	   -- usbcfg.h			USB configuration header
	   -- usbcore.h		USB Core header
	   -- usbcore.c		USB Core APIs
	   -- usbdesc.h		USB descriptor header
	   -- usbdesc.c		USB descriptor APIs
	   -- usbhw.h			USB hardware header
	   -- usbhw.c			USB hardware APIs
	   -- usbreg.h			USB misc. register header
	   -- usbuser.h		USB user header
	   -- usbuser.c		USB user APIs
	   -- type.h			Local type definition header
	   -- lcd.h			display support module header
	   -- lcd.c			Display support module
	   -- vcomdemo.h		header for VCOM demo
	   -- vcomdemo.c		USB CDC(Virtual COM) test module, main entry
	   -- virtualCom.uv2		uVision project file
	   -- serial.h			Header for UART port
	   -- serial.c			UART module APIs
	   -- lcd.h			Header for LCD display
	   -- lcd.c			APIs for LCD display
	   -- mcb2300-vcom.inf		The host side driver installation file

