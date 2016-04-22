■ HIDmon88 説明書

  これは ATtiny2313用のUSB接続モニタ HIDmon を ATmega88/168 に移植したものです。

■ 使い方

  ATmega88 には、 firmware/main88.hex を書き込みます。
  ATmega168 には、firmware/main168.hex を書き込みます。
  bin/hidmon88.exe をDOS窓から起動します。

  AVR> というプロンプトが出ますので、そこで ? を入れてください。
  AVR> ?
* HID_Monit Ver 0.1
Command List
 d  <ADDRESS1> <ADDRESS2>    Dump Memory(RAM)
 e  <ADDRESS1> <DATA>        Edit Memory
 f  <ADDRESS1> <ADDRESS2> <DATA> Fill Memory
 p ?                         Print PortName-List
 p .                         Print All Port (column format)
 p *                         Print All Port (dump format)
 p <PortName>                Print PortAddress and data
 p <PortName> <DATA>         Write Data to PortName
 p <PortName>.<bit> <DATA>   Write Data to PortName.bit
 sleep <n>                   sleep <n> mSec
 bench <CNT>                 HID Write Speed Test
 poll <portName>             Continuous polling port
 graph <portName>            Graphic display
 q                           Quit to DOS
AVR> p.
  PINB = 0001_1010 TCCR0B = 0000_0000 PCMSK1 = 0110_1000 OCR1BH = 0000_0000
  DDRB = 0000_0000  TCNT0 = 0000_0000 PCMSK2 = 0100_1000 TCCR2A = 1010_0000
 PORTB = 0001_1000  OCR0A = 0000_0000 TIMSK0 = 1011_1100 TCCR2B = 1111_0111
  PINC = 0000_0000  OCR0B = 0000_0000 TIMSK1 = 0111_0111  TCNT2 = 1100_0000
  DDRC = 0000_0000 GPIOR1 = 0000_0000 TIMSK2 = 1111_1111  OCR2A = 0111_0111
 PORTC = 0000_0000 GPIOR2 = 0000_0000   ADCL = 1010_1001  OCR2B = 0101_0101
  PIND = 1111_1000   SPCR = 0000_0000   ADCH = 0001_1011   ASSR = 0000_0000
  DDRD = 0000_0000   SPSR = 0000_0000 ADCSRA = 1011_1111   TWBR = 1111_1000
 PORTD = 1111_0000   SPDR = 1111_1111 ADCSRB = 0111_0111   TWSR = 1111_1110
 TIFR0 = 0000_0000   ACSR = 0001_0000  ADMUX = 1010_1000   TWAR = 0010_0010
 TIFR1 = 0010_0000  MONDR = 0001_0000  DIDR0 = 0010_0010   TWDR = 0000_0000
 TIFR2 = 0000_0000   SMCR = 0000_0000  DIDR1 = 0000_0000   TWCR = 0010_1000
 PCIFR = 0000_0000  MCUSR = 0000_0001 TCCR1A = 1001_1001  TWAMR = 0001_0011
  EIFR = 0000_0000  MCUCR = 0000_0000 TCCR1B = 0111_0010 UCSR0A = 0101_0101
 EIMSK = 0000_0001 SPMCSR = 0000_0000 TCCR1C = 0100_0000 UCSR0B = 0000_0000
GPIOR0 = 0000_0000 WDTCSR = 0011_0000 TCNT1L = 1101_1000 UCSR0C = 0000_0000
  EECR = 0000_0000  CLKPR = 0011_0000 TCNT1H = 1111_1110 UBRR0L = 1001_0100
  EEDR = 0000_0000    PRR = 0101_1111  ICR1L = 0010_0010 UBRR0H = 0101_1100
 EEARL = 1001_0001 OSCCAL = 0011_0001  ICR1H = 0000_0000   UDR0 = 1011_1110
 EEARH = 0000_0001  PCICR = 0011_0001 OCR1AL = 1110_0001
 GTCCR = 0000_0000  EICRA = 0000_0000 OCR1AH = 1100_0000
TCCR0A = 0000_0000 PCMSK0 = 0111_0111 OCR1BL = 0100_0000
AVR> q
Bye.

■ 起動オプション

 C:>  hidmon88  -i script.txt
 にて、一連のコマンドを連続実行します。

■ サンプルスクリプト

 準備中です。

■ 配線図
                          ATmega88/168
                         ___    ___
    (PCINT14/_RESET)PC6 [1  |__| 28] PC5(ADC5/SCL/PCINT13)
       (PCINT16/RxD)PD0 [2       27] PC4(ADC4/SDA/PCINT12)
       (PCINT17/TxD)PD1 [3       26] PC3(ADC3/PCINT11)
      (PCINT18/INT0)PD2 [4       25] PC2(ADC2/PCINT10)
 (PCINT19/OC2B/INT1)PD3 [5       24] PC1(ADC1/PCINT9)
    (PCINT20/XCK/T0)PD4 [6       23] PC0(ADC0/PCINT8)
                    Vcc [7       22] GND
                    GND [8       21] AREF
(PCINT6/XTAL1/TOSC1)PB6 [9       20] AVcc
(PCINT7/XTAL2/TOSC2)PB7 [10      19] PB5(SCK/PCINT5)
   (PCINT21/OC0B/T1)PD5 [11      18] PB4(MISO/PCINT3)
 (PCINT22/OC0A/AIN0)PD6 [12      17] PB3(MOSI/OC2A/PCINT3)
      (PCINT23/AIN1)PD7 [13      16] PB2(_SS/OC1B/PCINT2)
  (PCINT0/CLKO/ICP1)PB0 [14      15] PB1(OC1A/PCINT1)
                         ~~~~~~~~~~

   ---------------------------------------
   USB:     PD3   ===> USB D-
            PD2   ===> USB D+
   XTAL:    XTAL1,2 => Crystal 20MHz
   ---------------------------------------

■ FUSE


############################## ATMega88/168 ##############################
# Fuse Low byte: (0xFF)
#Low: 11111111
#     ||||++++-- CKSEL[3:0] システムクロック選択
#     ||++-- SUT[1:0] 起動時間
#     |+-- CKOUT (0:PB0にシステムクロックを出力)
#     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)
#
# Fuse High byte: (0xDD)
#High:11-11101 
#     |||||+++-- BODLEVEL[2:0] (111:無, 110:1.8V, 101:2.7V, 100:4.3V)
#     ||||+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
#     |||+-- WDTON (1:WDT通常動作, 0:WDT常時ON)
#     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
#     |+-- DWEN (On-Chipデバッグ 1:無効, 0:有効)
#     +-- RSTDISBL (RESETピン 1:有効, 0:無効(PC6))
#
# Fuse Extend byte: (0xF8)
#Ext: -----000
#          ||+-- BOOTRST ※データシート参照
#          ++-- BOOTSZ[1:0] ※データシート参照
#
###############################################################################

■ CLOCK

  必要ならば、firmware/Makefile 内の記述を変更してください。

	#F_CPU = 12000000
	F_CPU = 20000000


###### EOF ######
