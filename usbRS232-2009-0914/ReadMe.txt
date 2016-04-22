                                                            2008年11月09日（初版）
                                                            2009年 1月14日
                                                            2009年 3月 9日
                                                            2009年 9月14日


■ usbRS232 の紹介

  これは、ATtiny2313を使った、USBシリアル変換器です。
  CDC(コミュニケーション・デバイス・クラス) で実装していますので Windows 標
  準の CDC 仮想 COM ポートドライバーにより、"COMx:" (x は 1～ 9 まで) ポー
  トに見えるはずです。

■ アーカイブの構成

.\
├─bin
│  └─firmware
├─develop
└─Firmware
    └─usbdrv

■ 配線図

 * 	回路図は、HIDasp(x)、HIDsphのものと共通です。

                  ATtiny2313
                  ___    ___
RESET            [1  |__| 20] Vcc
PD0(RxD)         [2       19] PB7(SCK)
PD1(TxD)         [3       18] PB6(MISO)
XTAL2            [4       17] PB5(MOSI)
XTAL1            [5       16] PB4
PD2              [6       15] PB3(BUSY LED)
PD3(D+)          [7       14] PB2
PD4(D-)          [8       13] PB1
PD5(USB pullup)  [9       12] PB0
GND              [10      11] PD6
                  ~~~~~~~~~~~

・括弧()の書かれているpinと、GNC,Vccに配線をしてください。

・GND,Vcc間にはパスコン(10uF以上)を入れてください。

・XTAL1,XTAL2間に 12MHz の水晶振動子を取り付けます。

・PB5,PB6,PB7 は、ISP(イン・システム・プログラミング) 端子で、外部にある AVR
  ライターとの接続が出来るようにすると、いつでもファームウェアの更新が出来
  るので便利です。

・USB D+,D-は、68Ω程度の抵抗を経由してUSBコネクタに配線します。

・USB D- のUSBコネクタ側端子は1.5kΩで5Vもしくは3.3Vにpull upします。

・Vcc を 5V 駆動させる場合は USB D+,D- のコネクタ端子側にツェナー Di を入れ
  て 3.3V 程度にクランプします。

・RxD,TxDはいわゆるTTLレベルです。(通常はHiで、スタートビットがLoになります。)

   ---------------------------------------
   SPI:     PB7-5
   USB:     PD4   ===> USB D-
            PD3   ===> USB D+
            PD5   ===> USB D- pullup (Option)
            PB3   ===> BUSY LED (Option)
   XTAL:    XTAL1,2 => Crystal 12MHz
   ---------------------------------------

■ firmwareとFUSE設定

HIDaspx の回路で動作させる場合には、2 つの HEX ファイル（bin/firmware/main.
hex と bin/firmware/main.eep） を Tiny2313 に書き込んでください．
ヒューズ設定時は、必ず 12MHz のクリスタルを接続し、設定してください。

 >hidspx main.hex main.eep
  ^^^^^^^^^^^^^^^^^^^^^^^^
 Detected device is ATtiny2313.
 Erase Flash memory.
 Write   Flash: 2002/2002 B
 Verify  Flash: 2002/2002 B
 Passed.
 Write  EEPROM:    7/   7 B
 Verify EEPROM:    7/   7 B
 Passed.

 >hidspx -fL0xFF -fH0xDB -fX0xFF
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Low: 11111111 (0xFF)
     ||||++++-- CKSEL[3:0] システムクロック選択
     ||++-- SUT[1:0] 起動時間
     |+-- CKOUT (0:PD2にシステムクロックを出力)
     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)

High:11-11011 (0xDB)
     |||||||+-- RSTDISBL (RESETピン 1:有効, 0:無効(PA2))
     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
     |||+-- WDTON (WDT 0:常時ON, 1:通常)
     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
     |+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
     +-- DWEN (On-Chipデバッグ 1:無効, 0:有効)

Ext: -------1 (0xFF)
            +-- SPMEN (SPM命令 1:無効, 0:有効)



■ テスト

・w32term.exeはWindowsのコマンドラインで動作する簡易ターミナルソフトです。
・teraterm等でも試験は出来ます。

    ATtiny2313 の pin 2(RxD) と pin 3(TxD) を接続すれば、エコーバックテスト
    が出来ます。

・Windows Vista では、USB の LowSpeed Bulk 転送が許されていないため、そのま
  までは動作しません。

■ ビルド時の注意

・ビルドには WinAVR-20060421 を使用します。最新の avr-gcc(gcc4 ベースのもの
  全て) ではコードサイズが大きくなり 2kB に入りません。

・AVR-Wikiにある、WinAVR-20060421からmovw命令を使えるようにした版
  WinAVR-20060421.7z を使用すると、さらにメモリーサイズを節約できます。

・hardware.h  の HW_LED_PIN の値を変えると、BUSY LEDをPB0～3のいずれかに変更できます.

・usbconfig.h の
    #define HWPIN_USB_IS_D_4_3	0
    #define HWPIN_USB_IS_D_3_2	1
  の設定を変えると、USB D-,D+の接続を PORTD 3,2 に変更することができます。

 PD2 を USB 通信に利用する時には、FUSE 設定に注意し、クロック出力機能を無効
 にする必要があります。

■ ビルド時の注意

同梱の build-all.bat をご覧ください。12/16MHz 用の HEX ファイルを一回のコマ
ンドの実行で生成できます。RXBUFやDCDはコンパイル時に指定できるようにしました。
ただし、テストを行っておりませんので、何方かにテストをお願いできれば幸いです。

==== build-all.bat の内容 ====
@echo off

:FCPU12
make clean
make
make install

REM Program:2008 bytes (98.0% Full)で2kB以内に収まる
:FCPU16DCD
make DCD=1 F_CPU=16000000 clean
make DCD=1 F_CPU=16000000
make DCD=1 F_CPU=16000000 install

avr-size ../bin/firmware/*.hex
:END
================================

■ 生成されるファイル名について

main-12.* 12MHz動作用
main-16.* 16MHz動作用

   text    data     bss     dec     hex filename
      0       7       0       7       7 main-12.eep
      0    2002       0    2002     7d2 main-12.hex
      0       7       0       7       7 main-16.eep
      0    2008       0    2008     7d8 main-16.hex
      0    2016       0    2016     7e0 save.hex

AVRマイコンに書き込む場合には、main-12.hex, main-12.eepを同時に
書き込んでください。

■ 実用速度と注意点

  0. 今回の改定で、16MHz用のモジュールを生成できます。
     16MHz 用にコンパイルを行うと、Program:2008 bytes (98.0% Full) で 2kB
     以内に収まることが確認できました。これにより、若干の機能の追加や修正が
     可能になると考えます。

  1. UCHI ホスト (intel 系) では 38,400bps で文字落ちが確認されており、19,200bps
     が限界です。UCHI ホストに対しては、PC-USB デバイス間に HighSpeed-HUB
     を接続し、PC-Hub 間を 480Mbps で接続することで速度改善の可能性がありま
     すが、現在のところ動作が確認されているのは VIA の USB チップでのみです。

  2. OHCI ホスト (AMD,SiS,NEC 系) では 60,000bps で文字落ちが確認されています
     ので、57,600bps が限界です。

  3. UCHI ホストに対しては、PC-USB デバイス間に HighSpeed-HUB を接続し、
     PC-Hub 間を 480Mbps で接続することで速度改善の可能性があります。（未確認）


■ 免責

    まだ、いろいろな評価を行っていませんので不具合等がある可能性が高いです。
	送信バッファフルの処理がありません。
	フローコントロールがありません。

    ボーレートは 600bps から 12800bps の間の任意の偶数値を取れます。
            (baudrateの下位1ビットは無視されます。)
    MIDIのような31250bps に設定することも可能です。

    ボーレートレジスタの設定式は 16bit 精度でしか計算していませんので、設定
    値によっては誤差が生じる場合があります。

    計算式  UBRR = (USARTCLOCK / (baudrate/2)) - 1;
                    USARTCLOCK = 12000000 / 8 / 2 ;
	-----------------------------------------------


■ 謝辞

  LowSpeed AVR-USBでCDCクラスの実現は、Recursion Co., Ltd.
     http://www.recursion.jp/avrcdc/indexj.html
  の田村修氏の手により最初に行われました。
  このもとになったアイディアは2005年末の石川恭輔氏の実験に遡ります。

  また、AVR-USB(http://www.obdev.at/products/avrusb/index.html)
  のリファレンスプロジェクトのひとつである、
     AVR-Doper - an STK500 Compatible AVR Programmer
  のソースを今回のフレームワークに使わせていたきました。

 上記の先達の方々に深く敬意と感謝を述べたいと思います。


■ 開発者メモ

このアーカイブは、kugaさんの2008-1028版を元に作成しています。

　usbRS232を修正しました。

  (1) バッファポインタの扱いバグ修正
  (2) 送信を割り込みからポーリングに変更
   　　(割り込みに変更しても効果がないのでコード縮小を優先)
  (3) 通信パラメータをEEPROMに保存
　　　（RAMに保存せずに、その分送信バッファーに割当てた)
  (4) 送信バッファー 8->16に拡大
  (5) 12MHz, 16MHzの変更を可能にしました。
  (6) V-USBの改定を反映しました（16MHzでの動作が正常化）


