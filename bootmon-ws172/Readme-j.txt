                                                                      2008年11月27日

sorry,Japanese Only.

   このソースはまだ 実験版です。

■ bootloadHID 改

  　これは、AVR-USB のリファレンスプロジェクトである、bootloadHID にメモリー
  読み書きのコマンドを追加する実験です。11/10 から、ATmega88/mega168 の両方
  で動作するようになりました。


■ 出来ること

  bootloader 機能は、bootloadHID と互換性があり、同時に hidmon88 としても使
  えます。

  使い方としては、 
    適当なアプリケーションを作成しbootloadHID で転送します。
    hidmon88 で実行を指示します。
    そして、RAM 記録した内容は、hidmon88 で読み取ることが可能です。

■ アーカイブの構成

.\
├─bin
│  └─firmware
├─commandline
├─develop
├─firmware
│  └─usbdrv
├─src
└─test
    └─usbdrv


・ATmega168用
   text    data     bss     dec     hex filename
      0    2038       0    2038     7f6 main168-12.hex
      0    1970       0    1970     7b2 main168-20.hex
      0    2024       0    2024     7e8 main168-RC.hex
      0    2122       0    2122     84a main168-test-20.hex
      0    2540       0    2540     9ec main168-test-RC.hex

・ATmega88用
   text    data     bss     dec     hex filename
      0    2016       0    2016     7e0 main88-12.hex
      0    1948       0    1948     79c main88-20.hex
      0    2012       0    2012     7dc main88-RC.hex
      0    2472       0    2472     9a8 main88-test-RC.hex

※ -12, -20 はクリスタル発振子の周波数 (MHz)、-RC は RC 発振 (12.8MHz) を意
   味します。test が付いたものは、応用領域で動作するファーム（hidmon88 機能）
   です。ハードウェアにあわせ、適切に書き込んでください。

■ FUSE（クリスタル発振器の場合）


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
#          ||+-- BOOTRST [0]BootLoaderを実行
#          ++-- BOOTSZ[1:0] ブート領域サイズ（2kB）
#
###############################################################################

■ 回路図など

  Readme88.txtをご覧ください。


■ 現状の問題点

  bootloader で書き込んだ直後、hidmon で 0～ 0x1800 番地を見ても 0xff しか
  表示されない不具合がありました。別の AVR ライターで読む、あるいは USB を
  挿しなおすと、正常に書き込まれていることは確認できます。
  --> 08/11/02 kuga様のご教授により修正できました。ありがとうございました。

  test/ 以下は hidmon88 （ATmega88/168 用のモニタプログラム）を納めています。
  これは応用領域で動作するコードの例です。この応用領域では、bootloader 機能
  は使えません。

  コンパイラは WinAVR-2006-0422.7z を使用しています。それ以外の WinAVR では
  hex の容量が 2k をオーバーする可能性があります。（少なくとも gcc-3 ベース
  の WinAVR を使う必要があります）

  僅かにオーバーする場合は Vendor,Product 文字列を縮めて実験してください。


■ 拡張予定

  読み書きするメモリーの種類を増やす。--> 済み.
  POLL/GRAPHコマンドのサポート。      --> 済み.
  GO/REBOOT コマンドのサポート。      --> 仮実装のみ.

■ senshuの行った作業メモ

hidasp.c はかなり変更しました。適切な差分は取れないと思います。（理由：ISP
機能に関する部分を後半に移動したため）

1. genserial.awk をbootmon88にも使えるように修正しました。

2. hidmon88コマンドを修正し、-p:XXXX により、シリアル番号指定を可能にしました

3. hidasp.c を最新にしました

4. クリスタル（20MHz, 12MHz） の両方で動作することを確認しました(m88版)

  avr-size main.bin (20MHzの場合)
   text    data     bss     dec     hex filename
   1960       8      55    2023     7e7 main.bin
   
   
  avr-size main.bin(12MHzの場合)
   text    data     bss     dec     hex filename
   2028       8      55    2091     82b main.bin

  AVR> bench(USB-HUB有り)
  hid write start
  hid write end, 138000 bytes/5000 ms,  27600 bytes/s

  hid write start(USB-HUBなし)
  hid write end, 138000 bytes/24000 ms,  5750 bytes/s

5. develop, bin のディレクトリを追加し、hidmon,hidspx と類似の構成に変更。

6. firmware内のMakefileを修正（2008-11-10）

   make MCU=m168 のように make すると、ATmega168 用の main168.hex を生成し、
   省略時には、ATmega88 用の main88.hex を生成します。
   
   F_CPU=12800000 を指定すると、RC発振器用のコードを生成します。
   F_CPU=12000000 を指定すると、クリスタル(12MHz)用のコードを生成します。
   F_CPU=20000000 を指定すると、クリスタル(20MHz)用のコードを生成します。

   また、make help で使える機能をリストします。書き込み用の AVR ライタに 
   hidspx を使っている場合には、以下に示す全てのコマンドが機能します。

    make help
    ^^^^^^^^^
    make [MCU=m168] ... Build HEX file
    make [MCU=m168] readfuse ... Read FUSE info.
    make [MCU=m168] readflash ... Read flash
    make [MCU=m168] flash ... Write flash
    make [MCU=m168] fuse ... Write fuse
    make [MCU=m168] lock ... Write LOCKBIT
    make [MCU=m168] clean ... Clean project
    make [MCU=m168] install ... install HEX file

7. firmwareのシリアル情報
   HIDaspxとの重複を避ける為、'0001'にしています。
   hidmon88 -p1 で接続してください。(hidmon88の省略時の値を'0001'にしました)

8. hidasp.cを修正し、hidmon88利用後のbootloadHIDに生じる不具合を解決しました。

9. irukaさんの協力により、ATmega168でも2kB以内に収まるようになりました。
   若干ですが、空きもあります。

    avr-objcopy -j .text -j .data -O ihex main.bin main168.hex
    avr-size main.bin
      text    data     bss     dec     hex filename
      2032       2      55    2089     829 main.bin

10. vendor, productの文字列の照合を無効にし、VID, PIDとシリアル情報の照合を行います。

  VID=0x16c0 PID=0x05df vendor="YCIT" product="HIDaspx" serial="0000"
  VID=0x16c0 PID=0x05df vendor="AVRetc" product="bmon" serial="0001"

11. test/ディレクトリに応用領域用のfirmを用意しました。 (irukaさんの改良)

    hidmon88の省略時のシリアル情報を「0000」⇒「0001」に変更
    >hidmon88起動時に「TARGET DEV_ID=89」を表示

12. 内蔵RC発振周波数の自動調整機能が有効になりました。

    kuga さんのアドバイスにより、mega88/168 の両方で、無調整で動作します。
    FUSE 設定を以下に示します。PB0 への周波数設定は動作確認用ですから、動作
    が確認できたら、無効にすることもできます。

    RC発振用のFUSE設定(ATmega88/168兼用)

	Low: 10100010 (0xA2)
	     ||||++++-- CKSEL[3:0] システムクロック選択
	     ||++-- SUT[1:0] 起動時間
	     |+-- CKOUT (0:PB0にシステムクロックを出力)
	     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)
	
	High:11-11101 (0xDD)
	     |||||+++-- BODLEVEL[2:0] (111:無, 110:1.8V, 101:2.7V, 100:4.3V)
	     ||||+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
	     |||+-- WDTON (1:WDT通常動作, 0:WDT常時ON)
	     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
	     |+-- DWEN (On-Chipデバッグ 1:無効, 0:有効)
	     +-- RSTDISBL (RESETピン 1:有効, 0:無効(PC6))
	
	Ext: -----000 (0xF8)
	          ||+-- BOOTRST ※データシート参照
	          ++-- BOOTSZ[1:0] ※データシート参照


13. 応用領域（内蔵RC発振モード）で動作するhidmon88用のファームを追加しました。

    iruka さんの作成したアーカイブに添付されていましたが、私の理解不足で
    不十分なものとなっていました。test ディレクトリには、応用領域 (ATmega88
    では 0000～ 17ff まで）で動作するプログラムです。

    そこで、hidmon88 の機能を有効にした HEX ファイルを生成可能にしました。
    ブートローダの応用領域の実行機能を使えば、bootloadHID でこのプログラム
    を書き込み実行できますが、それが面倒な場合には、AVR ライタで書き込み、
    FUSE ext を 0xF9（ルートローダ機能を無効化） に設定してください。

    main88-test-RC.hex   0000 - 099B
    main168-test-RC.hex  0000 - 09DF

    なお、応用領域で動作するコードには、2kB という制限はありません。
    ATmega88 で 8kB, ATmega168 で 16kB までのサイズのコードを作成可能です。

14. hidmon88のbenchコマンドで繰り返し数が巨大な数値（例100万回）を指定時、
    表示が不正になるのを修正

 ※ 内蔵RC発振器での動作について（お願い）

    RC 発振モードでは、3.3V レギュレータなどで安定化した電源で動作させる必
    要があります。また、bootloadHID を実装していますが、FLASH メモリの更新
    でエラーが発生する場合があり、実用的にするには改良が必要です。

15. RC発振モードでの動作を改良（kugaさんからの提案）

・firmware. testの両方のコードに以下の修正を実施

  -usbconfig から osctune マクロを削除し、 avrusb-20081022 にある osctune.h
   を include に変更。(osctune.h は OSCCAL への 0x20 の加算を削除)

  - 1 サイクル前のタイマーの値を覚えておく必要があるので、main.c 内に 
   lastTimer0Value 変数を追加。

16. testモードの動作を確認しました（1125以前の版には不具合あり）

test モードで生成されるファームウエアは、hidmon88 機能以外のブートローダ機
能が残っていた為、動作に不具合がありました。今回は、応用領域に書き込むテス
ト済みファームを用意しました。

hidmon88で表示される「TARGET DEV_ID=XX」の意味は、以下のとおりです。

#define	DEV_ID_FUSION		0x55
#define	DEV_ID_STD		0x5a
#define	DEV_ID_MEGA88		0x88
#define	DEV_ID_MEGA88_USERMODE	0x89

今回公開するHEXファイルのサイズ。

■ bootloader + hidmon88
   text    data     bss     dec     hex filename
      0    2038       0    2038     7f6 main168-12.hex
      0    1970       0    1970     7b2 main168-20.hex

      0    2016       0    2016     7e0 main88-12.hex
      0    1948       0    1948     79c main88-20.hex

■ RC発振モード用（bootloaderのみ）
   text    data     bss     dec     hex filename
      0    2010       0    2010     7da main88-RC.hex
      0    2022       0    2022     7e6 main168-RC.hex

■ hidmon88のみ（テスト用：応用領域で動作）
   text    data     bss     dec     hex filename
      0    1968       0    1968     7b0 main88-test-20.hex
      0    2036       0    2036     7f4 main168-test-20.hex

      0    2384       0    2384     950 main88-test-RC.hex
      0    2452       0    2452     994 main168-test-RC.hex

RC モードでも動作確認を行っています。以下の BAT ファイルを利用すれば、比較
的簡単に構成できます。なお、動作途中での発振数の変更を中止しました。

17. registor割当てにミスがあり、修正しました。
また、テスト用のBATファイルを追加しました。

==> prog-m168-20.bat <==
@echo off
@echo ATmega168 setting FUSE (Xtal 20MHz mode).
hidspx -d10 -e
hidspx -d10 -FL0xFF -FH0xDD -FX0xF8
@echo Write bootloader and hidmon88 Firmware
hidspx -d1 main168-20.hex
echo "bootmonHID main168-test-20.hex"

==> prog-m168-rc.bat <==
@echo off
@echo ATmega168 setting FUSE (RC mode).
hidspx -d10 -e
hidspx -d10 -FL0xA2 -FH0xDD -FX0xF8
@echo Write Bootloader)not include hidmon88)
hidspx -d1 main168-RC.hex
echo "bootmonHID main168-test-RC.hex"

==> prog-m88-20.bat <==
@echo off
@echo ATmega88 setting FUSE (Xtal 20MHz mode).
hidspx -d10 -e
hidspx -d10 -FL0xFF -FH0xDD -FX0xF8
@echo Write Bootloader and hidmon88
hidspx -d1 main88-20.hex
@echo Write bootloader and hidmon88 Firmware
echo "bootmonHID main88-test-20.hex"

==> prog-m88-rc.bat <==
@echo off
@echo ATmega88 setting FUSE (RC mode).
hidspx -d10 -e
hidspx -d10 -FL0xA2 -FH0xDD -FX0xF8
@echo Write bootloader(not include hidmon88)
hidspx -d1 main88-RC.hex
echo "bootmonHID main88-test-RC.hex"

■ RC発振モードの評価法

1. ATmega88(168)を安定な電源で動作させる
2. prog-m88-rc.batを使って、ファームウェアを書き込む。
   （Xtalが接続されていても、RC発振への切り替えは可能で、取り外し不要）
3. bootmonHID main88-test-RC.hex で応用領域にテスト用のモニタを書き込む
4. PD5をGNDに接続し、USBコネクタを抜差し（あるいはAVRマイコンをRESET）する
5. hidmon88 でモニタが動作することを確認する

  >hidmon88
   ^^^^^^^^
  TARGET DEV_ID=89  ... 89は、応用領域で動作を表します
  AVR>

※ PD5 の切り替えが、やや面倒ですが、Xtal で動作させる場合には、モニタ機能
   をブート領域に含めることが出来るので、デバッグ時に PD5 の切り替え操作は、
   hidmon88 で指示可能です。（go boot と入力する）

この場合には、テスト用のプログラムとして hidmon88 用のファームウェアを書き
込みましたが、独自に作成したプログラムをブート領域を除いたメモリ（mega88:6kB,
mega168:14kB）に書き込み実行できます。つまり、一度この環境を用意できると、AVR
ライタが無くとも、プログラム開発が可能です。デバッグが完了したプログラムは、
PD5 を GND にすれば、特別な操作を行わなくとも RESET 直後に実行できます。

=================================
追記 2009-2-6:
  このバージョンは、bootmon-1127.zip をWSN#172基板に対応させたものです。
  X'talの周波数は 各Makefile内で指定できますが、省略値として12MHz版のHex
  を生成しています。

============== EOF ==============
