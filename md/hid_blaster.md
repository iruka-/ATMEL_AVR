[2010-10]]　[[pic18blaster]]　[[arm_blaster]]　[[STM8S（改）OpenOCDライター](stm8s_blaster.md) 

- - - -
## ATtiny2313(HIDaspxハードウェア)を使ってARM用JTAGアダプター(FlashROMへの書き込み、OpenOCD用デバッガ)を製作する。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/wsn216.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/wsn216.jpg)    ((画像は [wsnak](http://www.wsnak.com/)  様が販売している [WSN216基板](http://www.wsnak.com/kit/216/index.htm)  　))
===JTAG CABLE===>
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32.jpg) 


■ 概要

	  これは、OpenOCD の DLL ハック（実験）です。
	  即ち、外部に DLL を置いて、JTAGアダプターのドライバーの分離実装を試みています。

■ 現在のステータス

	  低速ですが、とりあえず、ATtiny2313を使用したJTAGアダプターが動いています。
	  現在、8kBファームの書き込み時間は3.4秒程度にまで高速化しています。
	  (USB2.0HUBを通した状態での計測です)

■ 特筆すべき点

- [HIDaspx](ＨＩＤａｓｐｘ.md) のファーム差し替えのみで実現しています。((但しAVRチップには3.3Vを供給する必要があります))
- HIDaspx AVRライターとしては、今までどおりに使えます。((ファームウェア容量節約のため、若干の速度低下があります。))
- [PICライター](PICspx.md) として、PIC18Fへブートローダーを書き込む用途に対しても今までどおりに使えます。

<!-- dummy comment line for breaking list -->


■ 試し方

	  試行にはWindowsXPを用います。
	  HIDaspx ハードウェアを用意して、ファームウェアのみ、下記のものに差し替えておきます。
	    hid_blaster/firmware/main-12.hex

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/sch-2313.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/sch-2313.png) 

	註：上記回路図は5V駆動になっていますが、実際は3.3Vレギュレータを入れて3.3V駆動する必要があります。
- ATtiny2313を5V駆動にする場合は、sa89a.net様の[USBブラスターもどき](http://sa89a.net/mp.cgi/ele/ub.htm) のように抵抗分圧する手法もあります。

<!-- dummy comment line for breaking list -->

	結線は、
	  ATtiny2313        ARM基板 JTAG端子
	         PB7  ------------- TCK
	         PB6  ------------- TDI
	         PB5  ------------- TDO
	         PB4  ------------- TMS
	 となります。これ以外のnTRSTピンなどはまだサポートしていません。(USB-Blasterと同様)
	  (上記４本の線は直結でも動作しましたが、100Ω程度の直列抵抗を入れてショート事故を防ぐようにする
	 　ほうが安全です)

	  ARM側は3.3Vなので、AVR側の電圧に注意してください。(ATtiny2313側も3.3Vにする必要があります)

	  hid_blaster/ ディレクトリの ocd.bat あるいは、 ocd2.bat を起動して、openocd.exeの吐き出すメッセージ
	  を確認することが出来ます。

- ocd.batは書き込みターゲットSTM32マイコン用、ocd2.batは書き込みターゲットLPC2388マイコン用の起動ファイルです。

<!-- dummy comment line for breaking list -->

	  正常に接続出来ているようでしたら、telnetで localhost:4444 番に接続して、OpenOCDコマンドを実行して
	  みてください。


■ ディレクトリ構成

	hid_blaster-+- ソース.
	            |
	            +--helper\   ヘッダーファイル.
	            +--jtag\     ヘッダーファイル.
	            |
	            +--firmware\ ATtiny2313側のファームウェア (main-12.hexを焼きます)
	            |
	            +--openocd_patch\  openocd本体側作成用の改造点


■ プログラムの再ビルド方法

	  WindowsXP上のMinGW-gccコンパイラを用いてmakeしてください。
	  makeすると、hidblast.dll が作成されます。

- openocd.exe本体を再ビルドする方法は、[ここ](openocd-build.md) を参照してください。

<!-- dummy comment line for breaking list -->

	  今回の改造部分ソースはopenocd_patch/ ディレクトリに置いています。

	  Linux上でのビルドオプションは、こんな感じです。
	  $ ./configure \
	      --build=i686-pc-linux-gnu \
	      --host=i586-mingw32msvc \
	      --enable-maintainer-mode \
	      --enable-dummy

	  出来上がった openocd.exe 本体は、ドライバーとして、同一ディレクトリに存在する hidblast.dll を
	  起動時に呼び出します。(存在しなければ、dummyドライバーのみが組み込まれます)


■ 現状の問題点

	  まだ遅いです。
	  HIDmon / HIDaspx の機能を残してはいますが、コードサイズ節約による速度低下があります。

■ ライセンス

	  OpenOCDの配布ライセンスに準じます。


■ 展望

- hidblast.dll ファイルを(自力で)差し替えるだけで、これ以外のJTAG自作書き込み器をサポートすることが可能になります。
    - OpenOCD本体ソースに変更を加えることなく、自由にドライバー作成できるようになります。
    - OpenOCD本体ソースには殆ど手をつけていませんので、今後のOpenOCDの変更点に追従する手間はほとんど要りません。
    - （ただしjtag_command_queueの構造が変わった場合はもろに影響を受けますが・・・）

<!-- dummy comment line for breaking list -->

- hidblast.dll のエントリーポイントは、

		     DLL_int get_if_spec(struct jtag_command **q);
- だけです。
- 引数のstruct jtag_command **qのqには、openocd本体のjtag_command_queueというグローバル変数のアドレスを渡します。

<!-- dummy comment line for breaking list -->

- 戻り値は、(intになっていますが) ドライバー記述構造体のアドレスになります。

<!-- dummy comment line for breaking list -->

## ダウンロード(仮)


|[hid_blaster.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/AVR/hid_blaster.zip) |WindowsXP,MinGW用ソース、実行ファイル、AVR側ファームウェア、openocd本体改造パッチ|

- 数日おきに更新していますので、古いと思ったら最新を取得してみてください。

<!-- dummy comment line for breaking list -->


~
~
~
## バッチ実行によるFLASH書き込みログ
	C:\hid_blaster>openocd.exe -f blaster.cfg -f stm32.cfg -f batch.cfg
	Open On-Chip Debugger 0.5.0-dev (2010-10-10-20:52)
	Licensed under GNU GPL v2
	For bug reports, read
	        http://openocd.berlios.de/doc/doxygen/bugs.html
	if load
	Info : only one transport option; autoselect 'jtag'
	10 kHz
	1000 kHz
	adapter_nsrst_delay: 100
	jtag_ntrst_delay: 100
	=*= dummy_init(void)
	TARGET DEV_ID=5a
	Info : clock speed 1000 kHz
	Debug: 1 0 bitbang.c:439 bang_reset(): reset to: RESET
	Info : JTAG tap: stm32.cpu tap/device found: 0x3ba00477 (mfg: 0x23b, part: 0xba00, ver: 0x3)
	Info : JTAG tap: stm32.bs tap/device found: 0x16410041 (mfg: 0x020, part: 0x6410, ver: 0x1)
	Info : stm32.cpu: hardware has 6 breakpoints, 4 watchpoints
	   TapName             Enabled  IdCode     Expected   IrLen IrCap IrMask
	-- ------------------- -------- ---------- ---------- ----- ----- ------
	 0 stm32.cpu              Y     0x3ba00477 0x3ba00477     4 0x01  0x0f
	 1 stm32.bs               Y     0x16410041 0x06412041     5 0x01  0x03
	                                           0x06410041
	                                           0x06410041
	                                           0x06410041
	                                           0x06410041
	Info : JTAG tap: stm32.cpu tap/device found: 0x3ba00477 (mfg: 0x23b, part: 0xba00, ver: 0x3)
	Info : JTAG tap: stm32.bs tap/device found: 0x16410041 (mfg: 0x020, part: 0x6410, ver: 0x1)
	Warn : Only resetting the Cortex-M3 core, use a reset-init event handler to reset any peripherals
	target state: halted
	target halted due to debug-request, current mode: Thread
	xPSR: 0x01000000 pc: 0x08000120 msp: 0x20005000
	auto erase enabled
	Info : device id = 0x20016410
	Info : flash size = 128kbytes
	wrote 8192 bytes from file main-0000.hex in 3.421875s (2.338 KiB/s)
	verified 8016 bytes in 1.140625s (6.863 KiB/s)
	shutdown command invoked
	=*= dummy_quit(void)

~
~
## JTAGコマンドの追加とプロトコルについて

- hidcmd.h

		#define HIDASP_JTAG_WRITE	  0x34	//JTAG 書き込み.
		#define HIDASP_JTAG_READ	  0x35	//JTAG 読み書き.
- が追加されてます。

<!-- dummy comment line for breaking list -->

- HidReportの下りストリーム(PC->AVR) サイズは最大38バイトまでです.

		+------+------+-------------------+------+-------------------+------+-------------+-----+
		| 0x34 | jcmd |  data列           | jcmd |  data列           | jcmd |  data列     | 0x00|
		+------+------+-------------------+------+-------------------+------+-------------+-----+
    - jcmdの1バイトは以下のように定義（その１）
    
    		bit 7   6   5   4   3   2   1   0
    		  +---+---+---+---+---+---+---+---+
    		  | 0 | b6| JTAG転送bit数(TDIの数)|   ＋ JTAG転送bit数分の TDIビット（LSBファースト）
    		  +---+---+---+---+---+---+---+---+
        - b6はTDI送出の最終bitでTMSを1にするなら1 しないなら 0 ： TMSは最終bit以外は常時0

<!-- dummy comment line for breaking list -->

~
    - jcmdの1バイトは以下のように定義（その２）
    
    		bit 7   6   5   4   3   2   1   0
    		  +---+---+---+---+---+---+---+---+
    		  | 1 |BITBANG転送回数(後続byte数)|   ＋ BITBANG転送回数分(byte数)のデータ
    		  +---+---+---+---+---+---+---+---+
    - BITBANGデータの１回分は、TCK=LOWのサンプルとTCK=HIGHのサンプルをパックしたデータ。
    
    		bit 7   6   5   4   3   2   1   0
    		  +---+---+---+---+---+---+---+---+
    		  |TCK|TDI|TDO|TMS|TCK|TDI|TDO|TMS|  （上位４ビットが最初にセットされ、次に下位４ビットがセットされます）
    		  +---+---+---+---+---+---+---+---+
    - TCKを変化させたくないときは、両方のTCKビットを同じ値にします。
    - TDOは入力PINなので出力されるわけではないけれど、ATtiny2313の場合はTDOプルアップ指定(1)になります。

<!-- dummy comment line for breaking list -->

~
~

- HidReportの登りストリーム(AVR->PC) サイズは最大38バイトまで. HIDASP_JTAG_READ発行時のみ折り返し返送されます.

		+----------------------------------------+
		| JTAG受信データ(TDOの読み取りビット列)  |  （最大38バイトまで）
		+----------------------------------------+
    - ビット列はLSBファースト. 送信されたTDIビット列とそのまま対応しています.

<!-- dummy comment line for breaking list -->

- - - -
その他補足
	#define HIDASP_JTAG_READ	  0x35	//JTAG 読み書き.
- を実行するときは、HidReportの下りストリーム(PC->AVR)を単純な形式（１コマンドのみ）にします。

		+------+------+-------------------+------+
		| 0x35 | jcmd |  data列           | 0x00 |
		+------+------+-------------------+------+
- jcmdがBitBangモードのときは、返答データはありません。JTAG(TDI列)のときのみ(TDO列)を返します。
- JTAGストリームが長い場合(56bit以上のTDIを送ってTDOを受け取る)は、56bit単位に分割転送します。
- その場合、最後のストリームの最終bitのみ、TMSを1にする処理が入ります。(b6=1のパケットを用意します)

<!-- dummy comment line for breaking list -->

~

- ATtiny2313はファームウェアサイズが極端に少なく(2048byte)複雑な処理を入れられないので、TDOの読み取り動作

<!-- dummy comment line for breaking list -->
は常に実行されます。(返答不要な場合も)
- また、TDI送出->TDO読み取りの処理も、8bitループと8bit未満端数ループに分離すれば高速化出来ますが、ファームウェアサイズの制限のためそれを行っていません。

<!-- dummy comment line for breaking list -->

~
- ATtiny2313のファームを再ビルドするときは、gcc3.4ベースのavr-gccを使用してください。
- gcc-4.xベースのavr-gccだとサイズオーバーします。

<!-- dummy comment line for breaking list -->

WinAVR-20060421の入手先は以下を参照してください。
- [ＤｏｗｎＬｏａｄ](ＤｏｗｎＬｏａｄ.md) 

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## OpenOCDの簡単な使い方

- 起動したらまず、localhost:4444番ポートにtelnetで(TeraTermなどを使って)繋いでください。

<!-- dummy comment line for breaking list -->

- TeraTermから、以下のようなコマンドをタイプすると、結果が表示されます。

<!-- dummy comment line for breaking list -->
|OpenOCDコマンド|意味|
|scan_chain|接続されているＴＡＰのリストを見る。|
|reset halt|ターゲットCPUをHALTさせる|
|reg|ターゲットCPUのレジスタを見る|
|mdw アドレス カウント|display memory words <addr> [count]メモリーダンプ|
|step|ＣＰＵをステップ実行させる|
|flash write_image erase main.hex|main.hexをFLASH ROMに書き込む(そのまえにCPUをHALTにしておきます)|


~
~
~
- - - -
## 参考文献(LINK)

OpenOCDが動くまで (kimura Lab)
- [http://www.kimura-lab.net/wiki/index.php/OpenOCD%E3%81%8C%E5%8B%95%E3%81%8F%E3%81%BE%E3%81%A7](http://www.kimura-lab.net/wiki/index.php/OpenOCD%E3%81%8C%E5%8B%95%E3%81%8F%E3%81%BE%E3%81%A7) 

<!-- dummy comment line for breaking list -->

OpenOCD (ベストテクノロジーさん)
- [http://www.besttechnology.co.jp/modules/knowledge/?OpenOCD](http://www.besttechnology.co.jp/modules/knowledge/?OpenOCD) 

<!-- dummy comment line for breaking list -->

OpenOCD本家
- [http://openocd.berlios.de/web/](http://openocd.berlios.de/web/) 

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## リンク
千秋ゼミ：arm_blaster AVR/news58
- [http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2Fnews58](http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2Fnews58) 

<!-- dummy comment line for breaking list -->

~
~
~


~
~
~

