*Ｔｉｐｓ [#o2d0d7f6]

- - - -
### ＡＶＲ-ＧＣＣ（ＷｉｎＡＶＲ）のバージョンはどれが良いの？
- とりあえず、ＧＣＣ３ベースの2006-0421というのを使ってます。

<!-- dummy comment line for breaking list -->


- - - -
### ＵＳＢのＤ＋、Ｄ−の配線を下図[ＡＶＲＵＳＢ](http://www.obdev.at/products/avrusb/index.html) と同じにしたい
[http://www.obdev.at/Images/vusb/circuit-zoomed.gif](http://www.obdev.at/Images/vusb/circuit-zoomed.gif) 

- （１）usbconfig.hを編集します。

		#define USB_CFG_IOPORTNAME      D
		/* This is the port where the USB bus is connected. When you configure it to
		* "PORTB", the registers PORTB, PINB (=PORTB-2) and DDRB (=PORTB-1) will be
		* used.
		*/
		#define USB_CFG_DMINUS_BIT      3
		/* This is the bit number in USB_CFG_IOPORT where the USB D- line is connected.
		* This MUST be bit 0 or 7. All other values will result in a compile error!
		*/
		#define USB_CFG_DPLUS_BIT       2
		/* This is the bit number in USB_CFG_IOPORT where the USB D+ line is connected.
		* This may be any bit in the port. Please note that D+ must also be connected
		* to interrupt pin INT0!
		*/
- オリジナルはB,0,1ですが、これをD,3,2にします。
- （２）ポート方向の初期化を変更します。

		int main(void)
		{
			DDRD = ~(1 &lt;&lt; 2);
			DDRB = ~(USBMASK|(1&lt;&lt;5));    /* all outputs except USB data */
- のDDRDの設定を、以下のように書き換えます。

		DDRD = ~(3 &lt;&lt; 2);   // D2,D3を入力に。他は出力。
			DDRB = ~(        (1&lt;&lt;5));    //PB5は入力（SPIの'MOSI'）
- 実は（２）の変更に気づかなかったため、ＨＩＤａｓｐが動かない**動かない**とずっと悩んでいたのは秘密。

<!-- dummy comment line for breaking list -->

- こうすることで、Ｉ/Ｏピン１本と配線１本が節約できますが、
- ＨＩＤａｓｐはじめいくつかのアプリはたいていB,0,1がデフォルトになっています。
- 何故そうなのかはいまだ不明です。（別の品種ではPORT Dが無いとか？？？）

<!-- dummy comment line for breaking list -->

- - - -
### コードを縮めたい。（ＡＴｔｉｎｙ２３１３限定）
- Ｃランタイムのスタートアップを自分で書きます。
- といっても、ＷｉｎＡＶＲのＬＩＢＣソースからcrt1.Sを取り出して改造するだけです。~

<!-- dummy comment line for breaking list -->
[https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/crt_S.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/crt_S.zip) 
- 自分のソース(main.c)のMakefileにcrt.oを追加する。

		OBJECTS =       usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o main.o
		  　　　　↓追加する
		OBJECTS = crt.o usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o main.o
- リンク時にデフォルトのcrt.oを使わないようにする。

		# file targets:
		main.bin: $(OBJECTS)
		$(COMPILE) -o main.bin $(OBJECTS)
		                                          ↓追加する。
		$(COMPILE) -o main.bin $(OBJECTS) -nostdlib
- 注意:
- .data .bss セクションのサイズが非ゼロであることを前提にしています。
- どちらかがゼロのときは、.data , .bssそれぞれに対して、コピーループ、クリアループそのものを削除します。

<!-- dummy comment line for breaking list -->

- - - -
### さらに縮めたい。（機種限定なし）
- [ＡＶＲＵＳＢ](http://www.obdev.at/products/avrusb/index.html) のPowerSwitchアプリケーションに含まれているusbdrvは、１２ＭＨｚ版のみでなく、１６ＭＨｚ、１６.５ＭＨｚ版が存在します。

		usbdrv/usbdrvasm12.S
		usbdrv/usbdrvasm16.S
		usbdrv/usbdrvasm165.S
- これを切り替えるには、main.cディレクトリに置かれている usbconfig.h を編集します。

		/* #define USB_CFG_CLOCK_KHZ       (F_CPU/1000) */
- これを有効にして、F_CPUを16000000 などとします。
- どこかでＣＰＵクロック定数＝F_CPUを定義していないときは 直接

		#define USB_CFG_CLOCK_KHZ  16000
- としても有効ですが、usbdrv/oddebug.h にF_CPUのデフォルト定義があるので、出来れば
- oddebug.hをインクルードする前で

		#define F_CPU 16000000
- もしくはMakefile中に-Dオプションで

		-DF_CPU=16000000UL
- としたほうが良いでしょう。
- PowerSwitch のファームサイズは、クロック別で以下のようになりました。

		ＡＴｔｉｎｙ２３１３  ＲＯＭ　　ＲＡＭ
		１２ＭＨｚ            1706      56
		１６ＭＨｚ            1582      56
		１６.５ＭＨｚ         1710      56
- １６ＭＨｚの時は１２ＭＨｚ時に比べて２２４バイトも縮んでいます。~
- これは**美味しい**かも。
- ソースコードのサイズも usbdrv/usbdrvasm16.S が小さいですね。
- クロック数に余裕があるので、８ビットの処理全部をアンロール展開せずにループ処理で対応出来ているみたいです。

<!-- dummy comment line for breaking list -->

- - - -

### ＡＶＲＵＳＢドライバーの１６.５ＭＨｚモードとは？
- クリスタルに限らず内蔵ＲＣやセラミック発振の精度を許容する。
- １２ＭＨｚ、１６ＭＨｚはどちらもクリスタル発振の精度が必要です。
- １６.５ＭＨｚ±1.0％の精度は必要です。
- 詳細は-[ＡＶＲＵＳＢ](http://www.obdev.at/products/avrusb/index.html) のEasyLoggerアプリケーションにあります。
- ８ピンのＡＴｔｉｎｙ４５などで内蔵ＲＣ発振を選択するとピン数が節約出来るので有効。
- main.cの先頭で、校正済内蔵ＲＣの８ＭＨｚをわずかだけオーバークロック調整して８.２５ＭＨｚにしています。
- 内蔵ＲＣの後の１/２プリスケーラを通さずに×８ＰＬＬ（その後１/４されＣＰＵに入る）に入れることでＣＰＵクロックを１６.５ＭＨｚにしています。（Ｆｕｓｅで設定する ＨＦ　ＰＬＬ）

<!-- dummy comment line for breaking list -->

- 残念ながらＡＴｔｉｎｙ２３１３では１６.５ＭＨｚモードを使うことは出来ません。
    - ｔｉｎｙ４５とはクロックＳＥＬの回路が異なっており、ｘ８ＰＬＬは存在しないためです。
- 参考：ＡＴｔｉｎｙ２３１３で選択できるクロック

<!-- dummy comment line for breaking list -->
|CKSEL3-0|意味|
|1111-1000|外部クリスタル／セラミック発振子|
|0110|128kHz内部(ＷＤＴ)発振器|
|0100|８ＭＨｚ校正付内蔵ＲＣ発振器|
|0010|４ＭＨｚ校正付内蔵ＲＣ発振器|
|0000|外部クロック信号|
|0xx1|予約|

ちなみに８ＭＨｚ校正付の校正値はOSCCALの値を１〜１２７まで書き換えることで
約４ＭＨｚから１２ＭＨｚ程度まで可変できるようです。（出荷時は８ＭＨｚに校正された値になっています）
- - - -

### ところで、どうして１６.５ＭＨｚなんていう半端なクロックなの？
- それは、１.５ＭＨｚの１１倍だから。
- つまり、ＬｏｗＳｐｅｅｄ　ＵＳＢのベースクロックが１.５ＭＨｚなので、１ビットを送受する時間がＣＰＵの命令数で数えてちょうど１１ステップになるようにコーディングしてあるわけ。
- １２ＭＨｚの場合はそれが８ステップになるので、超絶技巧プログラミングになるのだけれど、１６.５ＭＨｚの場合は３ステップの余裕があるから、受信時の同期取りコードを入れることが出来るということらしい。
- だけど送信時は完全にこっちのペースで送信することしか出来ないので、１％以内の精度は必要らしい。

<!-- dummy comment line for breaking list -->

### では、１.５ＭＨｚの整数倍になってない１６ＭＨｚで動くのは何故？
- そう、１６ＭＨｚだと、１０＋（２/３）クロックなんだ。
- usbdrv/usbdrvasm16.Sのソースを読んでいるが、読解出来ないっす。
- コメントには凄いことが書いてある。
    - 何をやろうとしているか本当に理解してないならコードに触ってはだめだ。
    - とにかく、これ書いた奴ら、凄すぎ。
    - 全部アセンブラなigorさんも凄いけど、この１６を書いた人はもっと尊敬する。というかチャレンジャーだ。
    - →[usbdrvasm16.S解読](https://github.com/iruka-/ATMEL_AVR/blob/master/web/log/2007-10.html#if7e6d57) 
- - - -
### その他のＴｏｐｉｃ
- [ＵＳＢ転送モードについてのまとめ](なんちゃってシリアル#uf936d7d.md) 

<!-- dummy comment line for breaking list -->

- [ＵＳＢ転送速度ベンチマーク](今後の目標#l07b1d63.md) 

