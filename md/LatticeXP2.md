[2010-03](2010-03.md) 

- - - -
## LatticeXP2 FPGA基板でＬチカ成功！



![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/lattic1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/lattic1.jpg) 

- 左側:PIC18F2550基板改　右側:DDT 2009 Spring号付録 LatticeXP2基板

<!-- dummy comment line for breaking list -->



![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/lattic2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/lattic2.jpg) 

- ＦＰＧＡ書き込み基板を接続したところ。

<!-- dummy comment line for breaking list -->

- - - -

- デバイスプロパティ

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/xp2w0.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/xp2w0.png) 

- XP2Writeにて書き込み中

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/xp2w2.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/xp2w2.png) 

- 書き込み時間

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/xp2w1.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/xp2w1.png) 


## 回路図


■ 配線            PIC18F2550

	                  ___    ___
	Vdd--10kΩ---MCLR [1  |__| 28] RB7/PGD 
	             RA0 [2       27] RB6/PGC 
	             RA1 [3       26] RB5/PGM --4.7kΩ-------GND (LVP書き込みchipはPGMプルダウン必須)
	             RA2 [4       25] RB4
	             RA3 [5       24] RB3 ------->|TMS |
	             RA4 [6       23] RB2         |    |
	             RA5 [7       22] RB1 ------->|TCK |
	  GND--------Vss [8       21] RB0 ------->|TDO |
	+---------->OSC1 [9       20] Vdd ------->|Vcc |(3.3v)
	+---|□|--->OSC2 [10      19] Vss ------->|GND |
	   20MHz     RC0 [11      18] RC7 ------->|TDI |
	   Xtal      RC1 [12      17] RC6
	             RC2 [13      16] RC5/D+  --------------------------> USB D+
	          +-Vusb [14      15] RC4/D-  --------------------------> USB D-
	          |       ~~~~~~~~~~
	         0.3uF
	          |
	         GND
- 注意 PICのvddなどの電源はUSB給電の5Vを３端子レギュレータで3.3Vに安定化して与えます。

<!-- dummy comment line for breaking list -->

LatticeXP2基板のJTAG端子ピン割り当て
	1:vcc(3.3v)
	2:TDO
	3:TDI
	4:
	5:
	6:TMS
	7:GND
	8:TCK

- - - -
参考文献:
- [http://sa89a.net/mp.cgi/ele/ub.htm](http://sa89a.net/mp.cgi/ele/ub.htm) 

<!-- dummy comment line for breaking list -->

- ファームはSPI版を使用致しました。
- 規格外ですが、18F2550を3.3V駆動にしてあります。18LF2550が入手出来るならそちらを使ったほうが良いです。
- 5V<->3.3Vレベル変換の抵抗は全部省略してみました。
- XP2Writeにて、Ｌチカのjedecファイルをそのまま書き込んでＬＥＤの点滅を確認しました。
- 書き込み時間は7秒程度でした。

