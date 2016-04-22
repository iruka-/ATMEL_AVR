*PICspx-classic [#p8dd2ab8]


[2009-08](https://github.com/iruka-/ATMEL_AVR/blob/master/web/log/2009-08.html) 　[2009-09](https://github.com/iruka-/ATMEL_AVR/blob/master/web/log/2009-09.html) 

	 -------------------------------------------------------------------
	        HIDaspx を使用して PIC18FシリーズのFlashを読み書き.
	                           PICspx-classic
	 -------------------------------------------------------------------

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/wsn216.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/wsn216.jpg)    ((画像は [wsnak](http://www.wsnak.com/)  様が販売している [WSN216基板](http://www.wsnak.com/kit/216/index.htm)  　))
＝＝＞
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50s.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50s.jpg) 


■ 概要

	  これはUSB接続のAVRライター HIDaspx のハードウェアをそのまま利用して、
	  PIC18Fへの書き込みを行おうという企画です。

■ 何に使うのですか？

	  PIC 18F2550/18F4550/18F14K50 など USB I/F 内蔵の廉価なPICシリーズの
	  マイコンにブートローダーを書き込むのに使用できたらいいなということで
	  制作を始めました。

■ 用意するハードウェアは？

	  （１）HIDaspx  AVRライター.
	
	  AVRマイコンやHIDaspxに関しては 千秋ゼミさんのサイトに膨大な情報が蓄積されています。
	  http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2FHIDaspx00
	  

	  （２）PIC 18F2550/18F4550/18F14K50 （どれか）

	   LVP 書き込みがOFFになっていると読み出せません。
	   (未使用のチップではONになっていると思います)

	   書き込み済みのチップでLVP=OFFの場合は、HVP書き込みが出来るライターで一度消去してから
	   LVP=ONで書き込んでください.

	   またLVP=ON状態のチップを実際に使用するときはPGM端子を常時PullDownしなければなりません。
	   (PGM端子は他の用途に使えません)

■ 接続:

	AVR用ISP 6PIN                  PIC18F2550/14K50

	1 MISO    ------------------   PGD
	2 Vcc     ------------------   Vcc
	3 SCK     ------------------   PGC
	4 MOSI    ------------------   PGM
	5 RESET   ------------------   MCLR
	6 GND     ------------------   GND


■ PIC 書き込みツール

	  src/ ディレクトリにあります。
	  
	  PICspx.exe
- - - -
	* PICspx Ver 0.1 (Aug 18 2009)
	Usage is
	  PICspx.exe [Options] hexfile.hex
	Options
	 -p[:XXXX]   ...  Select serial number (default=0000).
	 -r          ...  Read Info.
	 -v          ...  Verify.
	 -e          ...  Erase.
	 -rp         ...  Read program area.
	 -rf         ...  Read fuse(config).
	 -wf         ...  Write fuse(config).
	 -d          ...  Dump ROM.
- - - -

■ PIC 書き込みツールの使い方

	 WindowsXP から DOS 窓を開いて
	 C:> picspx  -r
	     ~~~~~~~~~~
	 のように入力すると、接続しているPIC18F の品種を表示します。
	 
	 
	 C:> picspx  bootloader-0000.hex
	     ~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 のように入力すると、「bootloader-0000.hex」をPICに書き込むことが出来ます。


	 C:> picspx  -rf
	     ~~~~~~~~~~~
	 のように入力すると、Fuse(config)の内容を読み出して コンソール表示します。
	 

	 C:> picspx  -d read.hex
	     ~~~~~~~~~~~~~~~~~~~
	 のように入力すると、FlashROMの内容を読み出して コンソールにダンプ表示します。

	 C:> picspx  -rp read.hex
	     ~~~~~~~~~~~~~~~~~~~~
	 のように入力すると、FlashROMの内容を読み出して カレントディレクトリに
	 「read.hex」のファイル名で出力します。（非常に時間が掛かります）

	 C:> picspx  -wf bootloader-0000.hex
	     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 のように入力すると、bootloader-0000.hexに含まれるFuse(config)の内容のみを
	 デバイスに書き込みます。

	 逆に、Fuseを書き込みたくない場合は、お手数ですがHexファイルをテキストエディタ
	 で編集して、0x30000X 番地のレコードを抹消してください。
	 （ただし、Fuseそのものは書き込みの前にBulk Eraseにより初期化されてしまいます） 



■ テストツール( このツールをデバッグする場合のみに使用してください )

	  test/ ディレクトリにあります。
	    HIDmon を改造してpgmコマンドを追加したものです。
	    a.bat を起動するとPICのflash ROMの内容(先頭64byte)とfuse(config),ID
	  を読み出して１６進表示します。


■ 現在のステータス

	  消去済みのPIC 18F14K50に対して、
	  HIDmon-14k のファームウェア bootloader-0000.hex を以下の手順
	  C:> picspx  bootloader-0000.hex
	      ~~~~~~~~~~~~~~~~~~~~~~~~~~~
	  で書き込んで、HIDmon/HIDboot として動作することを確認しています。



	  ポートの上げ下げに USBの1フレーム(1mSもしくは4mS(UHCI))の時間が掛かりますので
	  読み出しや書き込みは非常に遅いです。
	  
	  ブートローダーを書き込むのが目的なので、速度については目をつぶることにします。
	  
	  パソコン側のUSBが2.0(480Mbps)をサポートしている場合は、パソコンとHIDaspxライター
	  の間に、USB 2.0 Hub(480Mbps)を挟むことで書き込みを高速化することが出来ます。

	  あるいは、Intel/VIAのUSB Hostコントローラー(UHCI)を使う代わりに、SiS/AMD/NEC製
	  のUSB Hostコントローラー(OHCI) （マザーボード、もしくはPCIカード）を使用すると
	  でも書き込みを高速化することが出来ます。

	  高速化される理由は、USB 2.0(480Mbps)規格で拡張されたマイクロフレーム(1mSの1/8の時間
	  でのUSB応答サイクル)を使用できるためです。
	  （通常はUSBのフレーム時間は1mSであり、UHCIホストではコントロール転送に４フレーム
	  (4mS)消費するので、毎秒250回しかポートコントロール出来ないのが遅くなる理由です）

■ Download

暫定版：
- [picspx-2009-0904-classic.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/picspx-2009-0904.zip) 　(senshuさんがEraseコマンドを修正したバージョンです)
- ご注意：現在、PICspxをテスト頂いている方から、書き込み後FUSEがゼロクリアされる現象が報告されております。
- なので、HVP書き込みできるライターをお持ちでない方は、~~書き込みの御利用をお控えいただきますようお願いいたします。~~
    - 注意して書き込みをお願いいたします。成功リポートをお待ちしております。
    - ツールソースのダウンロード、展開、および18Fシリーズの読み出しテストまでは今のところ問題無いようです。
    - PIC18F14K50以外ではトラブルは今のところありません。

<!-- dummy comment line for breaking list -->


- FUSEがゼロクリアされる原因が修正され次第、正式版を公開致します。

<!-- dummy comment line for breaking list -->



- - - -
~
~
~
~
## 高速化の方法
- HIDaspxファームウェアを使う代わりに、HIDmonファームウェアにPIC書き込み専用のいくつかの

<!-- dummy comment line for breaking list -->
コマンドを追加することで実用的な速度で読み書きできるようになると思います。
- その場合はHIDmonファームのデバイスIDを新規に用意してHIDaspxと区別するようにすることで

<!-- dummy comment line for breaking list -->
パソコン側のツールは自動判別して高速動作に切り替えることが可能です。
- ハードウェアやPICとの接続は共通になります。
- 但し、AVRへの書き込みに使用できなくなります。(HIDmonとしては使用できます)

<!-- dummy comment line for breaking list -->

~~今のところ、そういったファームウェアを作成していませんので期待しないでください。~~
- PIC18F用に作成したものがあります。AVR/PIC両用ライターにもなります。**お勧めです。**

<!-- dummy comment line for breaking list -->

read more : [pic18spx](pic18spx.md) 


~
~
~
- - - -
## PICspx : 18Fシリーズの他のマイコンは書き込めますか？
- たぶん読み出しはOKだと思います。
- 消去コマンドがマイコンごとに異なっているので、それを吸収しないことには消したり書いたり出来ないと思います、が、
- コードの追加部分は全部Windows側なので、（Ｃ言語が触れる方であれば）変更自体は簡単に出来ると思います。

<!-- dummy comment line for breaking list -->

参考：アルゴ算法堂さん　ＰＩＣ１８Ｆシリーズの解説
- [http://homepage2.nifty.com/sampodo/craft/craft.html](http://homepage2.nifty.com/sampodo/craft/craft.html) 

<!-- dummy comment line for breaking list -->

~

秋月通販　ＰＩＣ１８Ｆ
- [http://akizukidenshi.com/catalog/c/cpic18f/](http://akizukidenshi.com/catalog/c/cpic18f/) 
- PIC18Fシリーズで試したいチップは、結局2550/4550/14K50だけでした。
- （これらのチップはUSBが付いていますが、USBを使わなければ他の18Fシリーズのようにも使用できます。）
- しかし、USBを使わないのであればATmegaシリーズに対しての優位性がありませんので、PICを使う必要性も薄れます。

