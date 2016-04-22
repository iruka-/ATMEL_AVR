[PIC32MX]]　[[ブートローダーを作る](HIDBootX.md) 

## [Pinguino](http://wiki.pinguino.cc/index.php/Main_Page/ja) について

スクリーンショット

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pinguino.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pinguino.png) 

- **[[Pinguino:http://wiki.pinguino.cc/index.php/Main_Page/ja]]**は、Arduinoに良く似た、無料の、お手軽プロトタイピング開発ツールです。

<!-- dummy comment line for breaking list -->

- 対応するマイコンは、MicroChip社の8bit MCU(PIC18F2550など)と32bit MCU(PIC32MX)の２種類です。
- いずれもUSB機能をチップに内蔵していて、非常に廉価です。((秋月などで１個220円程度で買えます。))
- また、DIPパッケージ((DIP以外のパッケージも存在します))なのでブレッドボードで試したり、試作がやりやすいというメリットがあります。

<!-- dummy comment line for breaking list -->

~
このＨＰでは、32bit版(PIC32MX)のほうを扱います。


~
- - - -
## Pinguinoを始めよう。

まずは、マイコンの入手からです。

Pinguino用の基板はOLIMEXが各種発売しています。
- https://www.olimex.com/Products/Duino/PIC32/

<!-- dummy comment line for breaking list -->

チップ単体は秋月などで扱ってます。
- [http://akizukidenshi.com/catalog/c/cpic32/](http://akizukidenshi.com/catalog/c/cpic32/) 

<!-- dummy comment line for breaking list -->

- USB内蔵品種を選びますが、Flash容量16kBは少なすぎてちょっと無理なので、今のところはMX220F032Bという品種一択になります。今後は大容量品やDIP以外を扱ってくれることを期待しましょう。

<!-- dummy comment line for breaking list -->

- - - -
## マイコンにファームウェア（ブートローダー）を書き込むライターを入手します。

- MicroChip純正のPicKit3かPicKit2をお勧めします。
- もし、すでにPIC18F2550をお持ちであれば、PicKit2の互換機が簡単に作れます。
- 秋月で110円で売っている別のマイコン(LPC1114FN28)をライターにすることも可能です。
    - read more : [PIC32MX](PIC32MX.md) 

<!-- dummy comment line for breaking list -->

~
~
~

## 自作PIC32MX基板を配線する。

■ 配線         PIC32MX220F032B 
	               3.3V
	                |
	                *------10Ω--------------+
	               10k                       |
	                |       ___    ___       | 0.1u
	  ラ   -->  ----*-MCLR [1  |__| 28] AVDD-*-||---GND
	  イ   -->  --PGD3/RA0 [2       27] AVSS--------GND  LED
	  ター -->  --PGC3/RA1 [3       26] RB15--1kΩ-------|＞|--GND
	                   RB0 [4       25] RB14
	                   RB1 [5       24] RB13
	              SDA2/RB2 [6       23] Vusb3v3--------3.3V
	              SCL2/RB3 [7       22] usb D-
	   Xtal     GND----Vss [8       21] usb D+   +10uF
	+-----------------OSC1 [9       20] Vcap------||---GND
	*--|□|--*--------OSC2 [10      19] Vss------------GND
	|  8MHz  |    U1TX/RB4 [11      18] RB9                        *--10k-----3.3V
	22pF    22pF  U1RX/RA4 [12      17] RB8                        |
	|        |   3.3V--Vdd [13      16] RB7 -----------------------*--BOOT_SW--GND
	|        |         RB5 [14      15] Vbus-----------USB Vbus(5V)
	GND    GND              ~~~~~~~~~~


（準備中）
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/sch-pinguino.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/sch-pinguino.png) 

- BOOT_SWはRB7入力のテストとしても使用できます
- 使い勝手を考えると、UBW互換と逆の論理にしたほうが便利です。(ONでスケッチ実行、OFFでBOOTLOADER)
- そうすると、再度**スケッチを書き込むときはRESETを押すだけで済みます。**
- そのままプロトタイピング完成品として使用する場合は、BOOT_SWを押しボタンではなく、スライドSWか、基板ジャンパーにして、ショートポジションにしましょう。
- 最新の[HIDBoot_Mips32gcc.X.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/HIDBoot_Mips32gcc.X.zip) では、そのように論理変更しました。

<!-- dummy comment line for breaking list -->



## 開発環境を入手しよう。
- [http://wiki.pinguino.cc/index.php/Main_Page/ja](http://wiki.pinguino.cc/index.php/Main_Page/ja) 
- 安定版はPinguino3.Xです。
- 開発版はPinguino4.Xです。
- Pinguino4.Xは ここ（ [http://code.google.com/p/pinguino32/downloads/list](http://code.google.com/p/pinguino32/downloads/list)  ）にある
    - これ（ Pinguino-X4-EasyPack-Win32-Rev903-v01-2013-10.7z ）もしくはこれより新しいものを入手して 7zファイルを展開します。
    - 展開したディレクトリ名は分かりにくいので、
    - すこし分かりやすいように C:\Pinguino4.X\とか D:\Pinguino4.X\のようなディレクトリ名にリネームしておきます。

<!-- dummy comment line for breaking list -->

~
## ブートローダーを書き込む。
- PicKit3,PicKit2、あるいは[PicKit2互換機](PIC32MX.md) を使ってPIC32MX220にブートローダーを書き込みます。
    - **ダウンロード:**[HIDBoot_Mips32gcc.X.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/HIDBoot_Mips32gcc.X.zip) 

<!-- dummy comment line for breaking list -->

- USBに繋ぐとPB15に繋がれたLEDが高速で点滅を始めます。
- LEDが点滅しないときはBOOT_SW OPEN(RB7=High)を確認して、再度RESETを行います。
- USB HIDデバイスなので、Windows(XP|7|8)側はUSBドライバーの導入は不要です。

<!-- dummy comment line for breaking list -->


~
## Ｌチカ（ＬＥＤ点滅）をやってみる。
Pinguino IDEを起動します。
- まず、Pinguinoボードの種類を「PIC32 Pinguino 220」に切り替えます。
- 右のToolsの中から、Directories＞Examples->01.Basic->Blinkを選びます。
- 下のFiles＞メニューにあるBlink.pdeをダブルクリックします。
- ボードの種類選択の右にある「Compile」ボタンを押します。
- エラーがなければその右の「Upload」ボタンを押します。
- Lチカが始まれば成功です。
- 始まらないときは、RESETボタンを押してみてください。

<!-- dummy comment line for breaking list -->




~
## print文でprintしてみる。（USB経由）
同様に、
- まず、Pinguinoボードの種類を「PIC32 Pinguino 220」に切り替えます。
- 右のToolsの中から、Directories＞Examples->04.Communications->CDCを選びます。
- 下のFiles＞メニューにあるPrintf.pdeをダブルクリックします。
- ボードの種類選択の右にある「Compile」ボタンを押します。
- エラーがなければその右の「Upload」ボタンを押します。
- アップロードが終わるとUSB-CDCデバイスとして新たなデバイスをWindowsが認識します。
- ドライバーinfはPinguinoには含まれていませんので、MicroChipのUSBフレームワークのUSB-CDCのものを使用します。
- teraterm等で、USB-CDCデバイス（Windowsからはシリアルポート COMxx: (xxは数字) として出現）に接続します。
    - USB-CDCの場合はボーレートはどれでも関係なく通信できます。

<!-- dummy comment line for breaking list -->

~
- これ以外のスケッチでCDC.printfを使用している場合にスケッチをコンパイルしたサイズが16kBを超えたら実行できませんので、適宜print文を削ってサイズを減らしてから実行してみます。

<!-- dummy comment line for breaking list -->

- （8kBバージョンのブートローダーを使用する場合は、スケッチサイズを20kBまで拡大できますが（割り込みベクターが4kB消費）そのためにはリンカースクリプトの変更が必要です。）

<!-- dummy comment line for breaking list -->

~
## シリアルポートを使ってみる。
同様に、
- Directories＞Examples->04.Communications->Serialを選びます。
- 下のFiles＞メニューにあるserialprintf.pdeをダブルクリックします。
- ボードの種類選択の右にある「Compile」ボタンを押します。
- エラーがなければその右の「Upload」ボタンを押します。

<!-- dummy comment line for breaking list -->

配線
- TxDはRB4（出力）、RxDはRA4（入力）です。

<!-- dummy comment line for breaking list -->

- シリアルのボーレートはスケッチ内で定義されています（9600）

<!-- dummy comment line for breaking list -->

~
## Pinguino UART1の RxD/TxDの割り当てソースはどこなのか？
- C:/PinguinoX.4/p32/include/pinguino/core/io.c

		// PIC32 Peripheral Remappage
		void IOsetRemap()
		{
		#if defined(PIC32_PINGUINO_220)
		SystemUnlock();
		CFGCONbits.IOLOCK=0;			// unlock configuration
		CFGCONbits.PMDLOCK=0;
		#ifdef __SERIAL__
			U2RXRbits.U2RXR=6;			// Define U2RX as RC8 ( D0 )
			RPC9Rbits.RPC9R=2;			// Define U2TX as RC9 ( D1 )
			U1RXRbits.U1RXR=2;			// Define U1RX as RA4 ( UEXT SERIAL )
			RPB4Rbits.RPB4R=1;			// Define U1TX as RB4 ( UEXT SERIAL )
		#endif
		・・・
- ちなみにUART2のほうはRC8/RC9に割り当てられるっぽいけれど、これは44PINのChipでないと出てこない。
- なので、UART2をRC8/RC9以外にしたい場合は、システム側のIOsetRemap()を書き直すか、これを呼ばずに、コピペして作ったOwnCodeを代わりに呼ぶしかなさそうだ。

<!-- dummy comment line for breaking list -->

- UART以外のI/O Remapに関しても、このio.cを読めばどうなっているか判ると思う。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## PinguinoX.4 のスケッチを元に、コマンドライン開発環境を整える方法。

- スケッチは確かにお手軽で良いのですが、普通に秀丸＋DOS窓でmakeしたい人。
- スケッチの記述よりもはるかにディープなコードを書きたいけれど、１から始めるのは嫌な人（お手軽フレームワークをそのまま使いたい人。）

<!-- dummy comment line for breaking list -->

向けの**環境整備指南**です。

~
これは意外と簡単です。

	D:\PinguinoX.4\ ディレクトリにインストールされていると仮定して説明します。

- １）PinguinoX.4を立ち上げ、デバイス=PINGUINO MX220を選択して、好きなスケッチを読み込む。
- ２）ビルドボタンを押す。
- ３）D:\PinguinoX.4\source\ ディレクトリに必要なファイルが全部用意されますので、これを D:\PinguinoX.4\src\ のようなディレクトリを作って丸コピーします。

<!-- dummy comment line for breaking list -->

- ４）Makefileを書き換えます。

<!-- dummy comment line for breaking list -->

	D:\PinguinoX.4\src\> copy Makefile32.win32 Makefile
	D:\PinguinoX.4\src\> notepad Makefile

notepadでなくて秀丸とかでOKです。
- D:\PinguinoX.4\src\Makefile

		------
		SHELL			=	cmd.exe
		#この後ろに、HOME,BOARD,PROC(essor) の環境変数を付け足します。
		HOME			=        D:\PinguinoX.4
		BOARD			=	PIC32_PINGUINO_220
		PROC			=	32MX220F032D
		#
		#SRCDIR		=	$(HOME)\source
		SRCDIR		=	$(HOME)\src
		#SRCDIR を変えます。
- 出来たら、次は、
- D:\PinguinoX.4\src\setenv.bat

		PATH D:\PinguinoX.4\win32\p32\bin;%PATH%
- のような１行バッチを書いてこれを実行し、実行PATHを設定します。
- 面倒なら、あらかじめパスの通っているディレクトリに置いて実行しても良いです。

<!-- dummy comment line for breaking list -->

- あとは Makeするだけです。

<!-- dummy comment line for breaking list -->

~
追記
	clean:
	if exist $(SRCDIR)\main32.o $(RM) $(SRCDIR)\main32.o
	if exist $(SRCDIR)\main32.elf $(RM) $(SRCDIR)\main32.elf
	if exist $(SRCDIR)\main32.hex $(RM) $(SRCDIR)\main32.hex
	#	if exist $(OBJDIR)\processor.o $(RM) $(OBJDIR)\processor.o
- processor.oは、GUIによってデバイスごとにコピーされるので、いったん消してしまうと復活が面倒です。
    - （具体的にはOBJDIRの32MX220F032B.o をコピーするだけですが）
    - なので、消さないようにコメントアウトします。

<!-- dummy comment line for breaking list -->





~
~
~
- - - -
## PinguinoX.4(rev903)用のスケッチエリア拡大方法
- 以下のアーカイブを適用することによって、スケッチエリアを9kBほど拡大することが出来ます。
- スケッチエリアが足らなくて困る場合は、ぜひ、お試しください。

<!-- dummy comment line for breaking list -->

- **ダウンロード:**[PinguinoX4patch.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/PinguinoX4patch.zip) 

<!-- dummy comment line for breaking list -->

~
- 上記パッチには、スケッチエリア拡大のほかに、PWM (または Sound) の不具合修正も含んでいます。
    - 音抜け問題と、noTone()やTone(pin,0,duration) でのゼロ割問題を対処。
    - PIC32MX220では、PWM出力用のPINに8番は使用できません。12番などをお使いください。(see playMusic.pde)
- OnTimer1〜5を実装してあります。
    - TMR2はmillisモジュールと競合しますので、排他的に使ってください。
    - TMR3はPWMでも使っていますので、排他的に使ってください。
    - 全タイマーは16bitモードで使用します。プリスケーラーは1/256まで、かつTIMER16bitなので、最大インターバルは0.41秒程度までです。（ソフトウェアによる間引きcallback呼び出しは未実装）

<!-- dummy comment line for breaking list -->
~
    - 以下のようなスケッチで実行します。
    
    		#define TMR1INT
    		#define TMR3INT
    		#define TMR4INT
    		#define TMR5INT
    		#include &lt;interrupt.c&gt;
    		void myBlink() {
    		   static int c=0;
    		   c++;
    		   if(c&gt;=(100)) {
    		       c=0;
    		       toggle(USERLED);
    		   }
    		}
    		void setup() {
    		  IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    		  IntEnableInterrupts();
    		  pinMode(USERLED, OUTPUT);
    		  OnTimer4(myBlink, INT_MILLISEC, 100);	// Use Timer0 to toggle pin 0 every 100 ms
    		}
    		void loop() {}
    - ブートローダーの更新が必要です。[HIDBootX]]のページにある最新版（[[HIDBoot_Mips32gcc.X.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/HIDBoot_Mips32gcc.X.zip) ）を入手して更新してください。
    
    		■ メモリーマップ
    		#
- オリジナル版Pinguino

		9D00_0000 +---------------+
		          | HIDBoot.X     |
		          |               |
		          | UBW32用の     |
		          | ブートローダー|
		          | をそのまま使用|
		          |               |
		9D00_3000 +---------------+
		          | スケッチ用の  |
		          | 例外ベクター  |
		9D00_4000 +---------------+
		          |               |
		          |               |
		          |               |
		          |               |
		          |スケッチエリア |
		          |    (16kB)     |
		          |               |
		          |               |
		          |               |
		9D00_8000 +---------------+
- パッチ適用後のPinguino

		9D00_0000 +---------------+
		          | HIDBoot.X     |
		          | 改良版        |
		          | ブートローダー|
		9D00_1000 +---------------+
		          | スケッチ用の  |
		          | 例外ベクター  |
		9D00_1A90 +---------------+
		          |               |
		          |               |
		          |               |
		          |               |
		          |               |
		          |               |
		          |スケッチエリア |
		          |    (25kB)     |
		          |               |
		          |               |
		          |               |
		          |               |
		          |               |
		9D00_8000 +---------------+
- ~
- 蛇足：
- 尚、水晶の代わりに内蔵RC発振を使用するファーム（[HIDBoot.X.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/HIDBoot.X.zip) ）

<!-- dummy comment line for breaking list -->
をご使用の場合は、どうしてもファームサイズが8kBになりますので、パッチのzipファイルに含まれるprocdefs2000.ld
のほうをprocdefs.ldと差し替えて使ってみてください。


~
~
~
~
- - - -
**INDEX:**


- - - -
## 参考リンク

本家Wiki（日本語ページ）
- [http://wiki.pinguino.cc/index.php/Main_Page/ja](http://wiki.pinguino.cc/index.php/Main_Page/ja) 

<!-- dummy comment line for breaking list -->

マイコン風雲録  Pinguino X4 IDE 簡単インストール
- [http://mpu.seesaa.net/article/399685638.html](http://mpu.seesaa.net/article/399685638.html) 

