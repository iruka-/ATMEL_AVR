[2010-10]]　AVRを使った[[OpenOCDアダプターはこちら](hid_blaster.md) 

- - - -

## PIC18F2550/14K50を使ってARM用JTAGアダプター(FlashROMへの書き込み、OpenOCD用デバッガ)を製作する。

【Windowsパソコン(OpenOCDホスト)】~
　　　｜~
　　　｜(USBケーブル) ~
　　　｜~
　　　Ｖ~
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ae18f2550.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ae18f2550.jpg) 
===JTAG CABLE===>
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32.jpg) 

写真は[秋月18F2550モジュール](http://www.akizukidenshi.com/catalog/g/gI-02161/) 　　　と　　　付録基板 CQ-STARM(STM32 CPU)

18F14K50でも製作可能

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg) 

■ 概要

	  これは、OpenOCD の DLL ハック（実験）です。
	  即ち、外部に DLL を置いて、JTAGアダプターのドライバーの分離実装を試みています。

■ 現在のステータス

	  とりあえず、PIC18F2550/14K50を使用したJTAGアダプターが動いています。


■ 試し方

	  WindowsXPを用います。

	  PIC18spxファームを焼いたPIC18F14K50(もしくはPIC18F2550)基板と、
	  JTAGが使用できる適当なARM基板を以下のような対応でJTAG接続しておきます。

- 結線は、

		  (PICライター機能の)         ARM基板 JTAG端子
		          MCLR  ------------- TMS
		          PGM   ------------- TDI
		          PGD   ------------- TDO
		          PGC   ------------- TCK
- となります。これ以外のnTRSTピンなどはまだサポートしていません。(USB-Blasterと同様)

<!-- dummy comment line for breaking list -->

	ARM側は3.3Vなので、PIC側の電圧に注意してください。(5Vは危険です)
- PIC側にPIC18F2550を用いるときは、電圧変換の必要が生じます。
- 参考例としては、USB-Blaster（もどき）の実装における、抵抗分圧を利用する方法と、
- あまりお勧めしませんが、PIC18F2550を3.3V駆動する方法などがあります。(18LF2550を使えば規格内です)

<!-- dummy comment line for breaking list -->

	  hidblast/ ディレクトリの ocd.bat あるいは、 ocd2.bat を起動して、openocd.exeの吐き出すメッセージ
	  を確認することが出来ます。

	  正常に接続出来ているようでしたら、telnetで localhost:4444 番に接続して、OpenOCDコマンドを実行して
	  みてください。


■ ディレクトリ構成

	hidblast-+ ソース.
	         |
	         |.\helper\   ヘッダーファイル.
	         |.\jtag\     ヘッダーファイル.
	         |.\openocd_patch\  openocd本体側作成用の改造点


■ プログラムの再ビルド方法

	  WindowsXP上のMinGW-gccコンパイラを用いてmakeしてください。
	  makeすると、hidblast.dll が作成されます。

	  openocd.exe本体を再ビルドする方法は、以下のURLを参照してください。

- [http://hp.vector.co.jp/authors/VA000177/html/2010-09.html](http://hp.vector.co.jp/authors/VA000177/html/2010-09.html) 

		#
		  今回の改造部分ソースはopenocd_patch/ ディレクトリに置いています。
		  Linux上でのビルドオプションは、こんな感じです。
		  $ ./configure \
		      --build=i686-pc-linux-gnu \
		      --host=i586-mingw32msvc \
		      --enable-dummy
		  出来上がった openocd.exe 本体は、ドライバーとして、同一ディレクトリに存在する hidblast.dll を
		  起動時に呼び出します。(存在しなければ、dummyドライバーのみが組み込まれます)
- ■ 現状の問題点

		  まだ遅いです。
		  不安定です。
- ■ ライセンス

		  OpenOCDの配布ライセンスに準じます。
- ■ 展望

		  hidblast.dll ファイルを(自力で)差し替えるだけで、自作デバイスがサポート可能になります。
		  （たとえばATtiny2313を使用したJTAGアダプターなどをサポート出来る可能性があります）
		  hidblast.dll のエントリーポイントは、
		     DLL_int get_if_spec(struct jtag_command **q);
		  だけです。引数のstruct jtag_command **qのqには、openocd本体のjtag_command_queueという
		  グローバル変数のアドレスを渡します。
		  戻り値は、(intになっていますが) ドライバー記述構造体のアドレスになります。
- *ダウンロード(仮) [#abc45486]
- |[pic18blaster.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic18blaster.zip) |WindowsXP,MinGW用ソース、実行ファイル、openocd本体改造パッチ|
- 数日おきに更新していますので、古いと思ったら最新を取得してみてください。

<!-- dummy comment line for breaking list -->

~
~
~
~
~
~~
~
~
## おまけファイル

- 以下のファイルは、sa89a.net様の[USBブラスターもどき](http://sa89a.net/mp.cgi/ele/ub.htm) と同一ハードウェアにフィットさせたものです。

<!-- dummy comment line for breaking list -->

|[hid_blaster.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/hid_blaster.zip) |WindowsXP,MinGW用ソース、実行ファイル、openocd本体改造パッチ|

- PIC側ファームウェアには、pic18spxのものではなく、hid_blaster/firmware/picmon-18F2550.hex を使用してください。
- 上記pic18blaster.zipファイルとの違いは、ファームウェア側のJTAG:TMSの結線がPORTC.6からPORTB.3に変更されているのみです。

<!-- dummy comment line for breaking list -->


- このアーカイブでは高速化は行っていません（8kB書き込みに約30秒かかります）
- 高速化するには、上記 [pic18blaster.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic18blaster.zip)  のファームウェアを修正して再ビルドしてください。

		#define	ALTERNATE_PGxPIN_ASSIGN	1	// PGC,PGD,PGMの結線を入れ替えます.
- Makefile の18F14K50を18F2550に変更してビルドします。

<!-- dummy comment line for breaking list -->


~
~
~

