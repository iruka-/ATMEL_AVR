***AVR_Monit って、いったい・・・。 [#m27c16cf]
**バルク転送で遊ぶ。**
- ＡＴｔｉｎｙ２３１３を**ＰＣから直接コントロールするにはどうしたらよいか**という命題。

<!-- dummy comment line for breaking list -->

つまり、こういうことだ。
- いちいちプログラムを書いてＲＯＭに焼くのは面倒だ。

<!-- dummy comment line for breaking list -->

- 原始時代の大昔、ワンボードマイコンでやっていたような、メモリーを読み書きする簡易ＭＯＮＩＴＯＲのイメージに近い。

<!-- dummy comment line for breaking list -->



- - - -
## SIZE(28){それは何ですか？}
	    +------+
	 ／  PC  ／| 
	+-------+  |
	|−＝− |  |
	+-------+  |
	|       |  |  ________________
	+-------+  |／  USB CABLE   ／
	|    □ |  |              ／
	|    ・ |  |          |AVRChip|
	|       | ／           ~~~~~~~
	+-------+
へたれな絵

- ＵＳＢに繋がっているＡＴｔｉｎｙ２３１３を制御するファームです。
- ＡＶＲ側の任意のメモリーやＩ/Ｏポートを読み書きできます。

<!-- dummy comment line for breaking list -->

    - ＵＳＢ　ＬｏｗＳｐｅｅｄバルク転送のスケルトンにもなっています。
    - 転送速度ベンチマークテスト用にも使えます。

<!-- dummy comment line for breaking list -->

#### ＬｏｗＳｐｅｅｄなのにバルク転送を使っています。
- ８バイトのコマンドパケットを送って、必要なら８バイトのリターンパケットを受け取ります。
- コマンドパケットの内容

		   0      1      2      3      4      5      6      7
		+------+------+-------------+------+------+-------------+
		| req  | cmd  |  address    | data0|data1 |   reserved  |
		+------+------+-------------+------+------+-------------+
- address は読み書きしたいＡＶＲの番地
- data0 は書き込みたいデータ。
- data1 はマスク書き込みする場合のマスクパターン。
    - data1 が0なら *address = data0;
    - data1 が0以外なら *address = (*address & data1) | data0;
- 読み出しの場合は data0に長さを入れる（最大８バイトまで)
- 読み出しの場合のアドレス空間は cmd の上位［6:5］bitの値で決める。(0=RAM 20=EEPROM 40=PGMEM)
- reqはコントロール転送時の予約(bmRequestType)

<!-- dummy comment line for breaking list -->
~
- リターンパケットの内容

		   0      1      2      3      4      5      6      7
		+------+------+------+------+------+------+------+------+
		|  読み出したＲＡＭの内容（最大８バイトまで）           |
		+------+------+------+------+------+------+------+------+
- と、こんな感じで作ってみました。

<!-- dummy comment line for breaking list -->
- - - -
## ＤＯＷＮＬＯＡＤ

- [upload/AVRmonit.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/AVRmonit.zip) ~

<!-- dummy comment line for breaking list -->
をダウンロードしてください。（ソース、ＨＥＸ、ＥＸＥ一式）

マニュアル
- [ＡＶＲ＿Ｍｏｎｉｔマニュアル](ＡＶＲ＿Ｍｏｎｉｔマニュアル.md) 

<!-- dummy comment line for breaking list -->

- - - -
## ハードウェア
配線は~~ＡＶＲＵＳＢ~~ [ＶＵＳＢ](http://www.obdev.at/products/vusb/index.html) 互換。
- Ｄ＋，Ｄ−はPD2,PD3に繋ぎます。~

<!-- dummy comment line for breaking list -->
[http://www.obdev.at/Images/vusb/circuit-zoomed.gif](http://www.obdev.at/Images/vusb/circuit-zoomed.gif) 

補足
- R1,R2:75Ω程度でもＯＫ。
- R3: 1K5は1.5kΩのこと。5V駆動の場合は2.2kΩ。
- C1: 4u7は4.7μFの電解コンデンサ。
- C2,C3は省略しても一応動きます。ただし発振周波数12MHzからわずかに誤差が発生します。
- D1,D2は一般の整流用シリコンダイオードです。これを入れるとAVRの動作電圧が5V-(0.6V*2)=3.7〜3.8V程度になります。
- バリエーションとしては、普通の三端子レギュレータで3.3V〜3.5Vを作って供給する手もあります。

<!-- dummy comment line for breaking list -->

~
AVR側を5V動作にしたい場合は
- D1,D2を省略して、かわりにUSBの（D+とGND間）と(D-とGND間)に青色LEDをそれぞれ入れることでUSB信号の電圧を3.3V程度にクランプします。
    - 5V動作かつ青色LEDを省略してもたいていは動作しますが、ホストＰＣとの相性によりUSB認識しない場合もあります。

<!-- dummy comment line for breaking list -->


- win32側のビルドはmingw GCCを使います。(cygwinでも出来るはず)
- Vendor ID , Product IDはPowerSwitchと同じになってます。PowerSwitchと同時に使うことは出来ません。（そのかわりlibusbドライバーの組み込みは１回で済む）

<!-- dummy comment line for breaking list -->

    - ＴｏＤｏ：
    - ~~VendorID,ProductIDが重複していてもアプリ名称で区別してデバイスを認識させること。~~
    - ~~スクリプトファイルをあらかじめ用意しておいて食わせるモードが欲しい。(-ifile)~~ -済み
    - ~~ファーム側のＤＤＲ設定のデフォルトをＩＮかつＰＵＬＬＵＰにしたい。~~
    - 全ＰＯＲＴのログを取れるデータロガーモードを付けたい。
    - ＰＯＲＴをビット単位でＯＮ/ＯＦＦするコマンド。
    - ＰＯＲＴに時系列データを食わせてＯＮ/ＯＦＦできるやつ。
    - 超低速だがロジアナにならんかな。

<!-- dummy comment line for breaking list -->


これをベースに、ＰＣとＡＶＲのコミュニケーションを行うつもり。
- コントロール転送は使わない。
- バルク転送メッセージは必ず８バイト単位。
- - - -
~~と、書いたが、Ｖｉｓｔａ対応の件もあり、コントロール転送版（但し遅い）と両立させようと思う。~~
- 済み。
- コントロール転送ではベンダーリクエストを送ることになるので、８バイトの先頭１バイトはbmRequestTypeを入れなければならない。
- そうすると、使えるのは残り５バイト（最後の２バイトはwLength）
- しかたなく、コマンド構造体を変更。
- 面倒なのでコントロール転送では常にリターンパケット８バイトを受信しようと思う。
- ほぼPowerSwitchと同じつくりになる。

<!-- dummy comment line for breaking list -->

- - - -
### 操作例
	F:\avr\AVRmonit\win32> monit -c
	found 3 busses
	AVR> help
	* AVR_Monit Ver 0.1
	Command List
	 d  <ADDRESS1> <ADDRESS2>    Dump Memory(RAM)
	 dr <ADDRESS1> <ADDRESS2>    Dump Memory(EEPROM)
	 dp <ADDRESS1> <ADDRESS2>    Dump Memory(PGMEM)
	 e  <ADDRESS1> <DATA>        Edit Memory
	 f  <ADDRESS1> <ADDRESS2> <DATA> Fill Memory
	 p ?                         Print PortName-List
	 p .                         Print All Port (column format)
	 p *                         Print All Port (dump format)
	 p <PortName>                Print PortAddress and data
	 p <PortName> <DATA>         Write Data to PortName
	 sleep <n>                   sleep <n> mSec
	 bench <CNT> <PACKET_SIZE>   Bulk(or ctrl) Write Speed Test
	 q                           Quit to DOS
	AVR> p .
	 DIDR = 0000_0000  PORTD = 0111_0011 WDTCSR = 0000_0000 TCCR1A = 0000_0000
	UBRRH = 0000_0000 GPIOR0 = 0000_0000 TCCR1C = 0000_0000 TCCR0A = 0000_0000
	UCSRC = 0000_1110 GPIOR1 = 0000_0000  GTCCR = 0000_0000 OSCCAL = 0110_0010
	 ACSR = 0001_0000 GPIOR2 = 0000_0000  ICR1L = 0000_0000  TCNT0 = 0000_0000
	UBRRL = 1001_1011   PINB = 1111_1111  ICR1H = 0000_0000 TCCR0B = 0000_0000
	UCSRB = 0001_1000   DDRB = 0000_0000  CLKPR = 0000_0000  MCUSR = 0000_0001
	UCSRA = 0110_0010  PORTB = 1111_1111 OCR1BL = 0000_0000  MCUCR = 0000_0011
	  UDR = 0000_0000   PINA = 0000_0000 OCR1BH = 0000_0000  OCR0A = 0000_0000
	  RXB = 0000_0000   DDRA = 0000_0000  OCR1L = 0000_0000 SPMCSR = 0000_0000
	  TXB = 0000_0000  PORTA = 0000_0000  OCR1H = 0000_0000   TIFR = 0000_0000
	USICR = 0000_0000   EECR = 0000_0000 OCR1AL = 0000_0000  TIMSK = 0000_0000
	USISR = 0000_0000   EEDR = 0000_0000 OCR1AH = 0000_0000   EIFR = 0000_0000
	USIDR = 0000_0000   EEAR = 0101_1101 TCNT1L = 0000_0000  GIMSK = 0100_0000
	 PIND = 0111_1011  EEARL = 0101_1101 TCNT1H = 0000_0000  OCR0B = 0000_0000
	 DDRD = 0111_0011  PCMSK = 0000_0000 TCCR1B = 0000_0000
	AVR> p
	   PIND(30) 73 0111_0011
	  PORTD(32) 73 0111_0011
	   PINB(36) ff 1111_1111
	  PORTB(38) ff 1111_1111
	   PINA(39) 00 0000_0000
	  PORTA(3b) 00 0000_0000
	AVR> p portb aa
	  portb(38) aa 1010_1010
	AVR> p portb 55
	  portb(38) 55 0101_0101
	AVR> p
	   PIND(30) 7b 0111_1011
	  PORTD(32) 73 0111_0011
	   PINB(36) ff 1111_1111
	  PORTB(38) 55 0101_0101
	   PINA(39) 00 0000_0000
	  PORTA(3b) 00 0000_0000
	AVR> q
	Bye.

	F:\avr\AVRmonit\win32>
- - - -
### 起動オプション
	-c       コントロール転送で接続する。
	-i<file> モニターコマンドを記述したスクリプトファイルを読み込んで実行する。


- - - -
## 作った後で気づいた、驚愕の事実

- いしかわさんが、[２００５年１０月の日記](http://www.asahi-net.or.jp/~qx5k-iskw/darkside/0510dark.html) 

<!-- dummy comment line for breaking list -->
にて、汎用 USB-IO 互換ファームウェアをすでにＡＴ９０Ｓ２３１３で実現されておりました。

    - SIZE(9){いやー恥ずかしい。}
    - SIZE(9){またやっちまったぜ、車輪の再発明・・・。}

<!-- dummy comment line for breaking list -->

- - - -
[English Translation](http://www.excite.co.jp/world/english/web/?wb_lp=JAEN&amp;wb_url=http://hp.vector.co.jp/authors/VA000177/html/AVR_Monit.html) 

![screenshot](https://github.com/iruka-/ATMEL_AVR/blob/master/counter/dream.cgi?id=HIDasp&name=a.gif)

