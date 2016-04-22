[2013-09](2013-09.md) 



## PIC32MXでVGA出力に挑戦。

- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/vgatest2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/vgatest2.jpg) 

<!-- dummy comment line for breaking list -->

## 目次


## 概要

- PIC32MX220F032B (秋月価格で220円) １個だけでVGA信号でASCII文字出力を行なっています。

<!-- dummy comment line for breaking list -->

必要な部品（おおよそ）
,3.3Vレギュレータ（＋５Ｖ電源。USB給電も可）USB無しで外部3.3V電源の場合はレギュレータは不要,電気は要るよね。普通
,積セラ 0.1uF x 3 、 10uF x 1 程度,パスコンは忘れずに
,抵抗 10kΩ x 2 、100Ω x 4 程度,無駄な抵抗はよそう
,DSUB 15pin VGAコネクタ,PC互換機の古いVGAカードから取り出しでも可。
,8MHz水晶,4/8/12/16/20MHzのどれでも可。
,そしてPIC32MX220F032B,エラッタ上等！

- かなりお安く作れると思います。（１０００円以下？）

<!-- dummy comment line for breaking list -->

~

- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32vga.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32vga.jpg) 
- ファームウェア書き込みにHID Bootloaderを使用する場合のみ、USB周りの部品が必要です。
- 回転式のＳＷは、色決め用(GREEN,RED,BLUEのどれに接続されるか8通りをロータリー式DIPで切り替え)です。

<!-- dummy comment line for breaking list -->

- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/vgatest3.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/vgatest3.jpg) 

<!-- dummy comment line for breaking list -->

## とりあえずダウンロード

- ↓ VGA/SVGAの両方に対応させてみました。

<!-- dummy comment line for breaking list -->

ファームウェアソース：[mxvga.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/mxvga.zip) 

- - - -
## おまけダウンロード

- ↓ USB接続([USB簡易モニター](PIC32mon.md) )を生かしたまま、VGA出力を行ないます。

<!-- dummy comment line for breaking list -->

ファームウェアソース：[mz32k.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/mz32k.zip) 

- WindowsPC　Host側の接続ソフトは[USB簡易モニター](PIC32mon.md) 付属のものをお使いください。
- - - -
## さらに、USBシリアルで簡易ターミナルっぽいもの。

- WindowsPC側とUSB接続して、Teratermなどから打ち込んだ文字をVGA画面にも表示します。

<!-- dummy comment line for breaking list -->

ファームウェアソース：[cdcbasic.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/cdcbasic.zip) 

- ~~basint(BASICインタプリタ)まで入れようとしていた痕跡が残っています。~~
- ~~basintを入れると、メモリーが足らなくなるので、HID Bootloaderから書き込めなくなります。~~
- ~~また、SRAMも足らなくなるので、basintまで入れると今のところ、動作しません。~~
- **無理やりbasintまで詰め込みました。**

<!-- dummy comment line for breaking list -->

~
- cdcbasicの使い方はこのページの下のほうに追記しました。

<!-- dummy comment line for breaking list -->
~
~
~

- - - -
## 256 x 200ドットのグラフィック表示バージョンを追加しました。

- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/vgagfx.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/vgagfx.jpg) 
- 表示は、味の無い32bit整数値のインクリメントパターン
- グラフィックVRAM(bitmap)の構造は、32bit SPIデータの列(整数単位で、MSBが左、LSBが右)になっています。

<!-- dummy comment line for breaking list -->

- - - -

ファームウェアソース：[vgagfx.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/vgagfx.zip) 

- ＵＳＢ簡易モニターを使用して、直接VRAMに数値を書き込むとリアルタイムで表示が変わります。（今のところそれしか出来ません）
- まだ、グラフィック描画関数やコマンドは未実装です。
- SVGA 800x600 @60Hz 上に256x200グラフィック画面を（横２倍、縦３倍）サイズ(512 x 600)で表示しています。

<!-- dummy comment line for breaking list -->

~
- ファームウェアサイズは16kB程度ですが、SRAMはUSBと共存している関係でほぼ空きがありません。
- USBを外すことで、Invader程度の簡単なTVゲームは実装出来ると思います。
    - グラフィックVRAMは6400byte使用しますので、VRAMを除いたSRAM使用可能容量は1792byteです。
    - テキストVRAM表示と違って、HSYNC割り込み内での、CG-ROMアクセス処理(pixel変換)が不要になりますので、

<!-- dummy comment line for breaking list -->
フォアグラウンド側の処理は速くなります。

~

~
~
- - - -
## PIC32MX250用に、384 x 300ドットのグラフィック表示バージョンを追加しました。

- PIC32MX250では、SRAMの容量が32kBに増量されていますので、解像度UPしてみました。
- VRAMの容量は約15kBです。
- （400 x 300 ドットでない理由は 32bit SPI出力なので、横解像度が32の倍数になるからです）
- （384 x 600 dot への変更は可能ですが、1dotが横長長方形になります）

<!-- dummy comment line for breaking list -->

ファームウェアソース：[vgagfx250.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/vgagfx250.zip) 


~
~
~
~
- - - -
## 解説

- 水晶の周波数の都合により、いわゆるVGA規格(640x480 @60Hz ドットクロック25.175MHz)のかわりにSVGA規格(800x600 @60Hz ドットクロック40MHz) を使用しました。

<!-- dummy comment line for breaking list -->

- DIP型のPIC32MXは駆動周波数の上限が40MHz(50MHz)になっていて、SPIのマスタークロックはその1/2に制限されます。
- なので、実際に動作させているドットクロックは20MHzです。
- すなわち、実質解像度は 400 x 600 dotということになります。

<!-- dummy comment line for breaking list -->

## 動作
- HSYNC信号の生成にTIMER1割り込みを使用しています。（周期は26.4uS）
- HSYNCの割り込みでHSYNC信号を作り、その後にDMAをKICKします。
- DMAは48byte分のSPI WriteをCPUを介在しないで実行します。
- SPI out 信号を GREEN,RED,BLUEの端子に抵抗分圧して繋いでください。
- 分圧比率を適当に変えることで、輝度や色合いなどを調整できます。

<!-- dummy comment line for breaking list -->

~
- DMAをKICKした後、次の１ラインのためのLINE OUT BUFFERを更新します。
- １ライン（横384dot x 縦1 dot）分のLINE OUT BUFFERは２組あります。１つはDMAがSPIに送信中のバッファ、
- もうひとつは、CPUが次のライン出力のために計算中のものです。
- １ライン時間が過ぎたらBUFFERは役割を入れ替えます。（ダブルバッファ）

<!-- dummy comment line for breaking list -->

~
- CPUの更新内容としては、テキストVRAMに書かれた文字を読み取り、その文字の、該当Ｙラインにあたるビットパターン（８ドット分）を取得します。
- これを４８文字分繰り返して、１ラインのドットの並びを生成します。

<!-- dummy comment line for breaking list -->

- 次のDMA KICKではそのデータをそのままSPI outに流します。

<!-- dummy comment line for breaking list -->

~
- 8x16dotのフォントは、手元で適当に動いていたLinux(ubuntu13.04)の/usr/share/consolefonts/*.psf.gz

<!-- dummy comment line for breaking list -->
から好きな奴を選んで、hexダンプしたものを組み込んでいます。
- 先頭の４バイトがヘッダー情報なので捨てます。
- 文字は0x00〜0xffまでの256文字が定義されていました。

<!-- dummy comment line for breaking list -->


~

- SPI Clock(20MHz)とTIMER1 割り込み周期のClockがかみ合っていない（倍数関係に無い）場合は１ラインごとに

<!-- dummy comment line for breaking list -->
規則的なジッターが発生します。（kats_me様のご指摘により修正できました。ありがとうございました）

~
~
## 高望みしないでね

- 廉価PIC32MXなので、Flash32kB、SRAM8kBしかありません。

<!-- dummy comment line for breaking list -->

- テキストVRAMのサイズは横48 x 縦36 (1728byte) ASCII (0〜255までの文字コード)です。

<!-- dummy comment line for breaking list -->

- SVGAモード(800x600 @60Hz)を使ってはいますが、実質解像度は400x600です。

<!-- dummy comment line for breaking list -->

- 色はGREEN,RED,BLUEの端子にSPI出力をどう繋ぐかでかわりますが、画面全体で１色です。文字ごとに色変えは出来ません。

<!-- dummy comment line for breaking list -->

- DMA KICKの処理がことのほか重いので、フォアグラウンド側の処理はそんなに速く出来ません。

<!-- dummy comment line for breaking list -->

- HSYNC割り込みが、他の割り込み要因により邪魔されると画面が乱れるので、他の割り込みを使わないとか、HSYNCと干渉しないように特別なテクニックが必要になるかもしれません。

<!-- dummy comment line for breaking list -->






~
~
~
## 改造指南
- CPUを50.35MHzで動作させることが出来るなら、ドットクロック25.175MHzは可能です。
- そうすれば、400x600ではなくて、640x480の表示は可能です。
- 必要な水晶は、4.1958MHzか、その整数倍（５倍まで）です。

<!-- dummy comment line for breaking list -->

また、
- CPUにDIP品ではなく上位品種のものを使用すれば80MHz動作が可能なので、ドットクロック40MHz

<!-- dummy comment line for breaking list -->
(800x600)も可能になります。その場合SRAM容量も増えますので高解像度(800x600ですけど)対応できると思います。
- SRAMが60kB程取れる場合は、グラフィック（単色ですが）表示が可能です。(VGA解像度なら38kB)
- その場合はHSYNCのDMA KICKでは特に計算量が要らなくなるのでフォアグラウンド処理にCPUを割くことが出来ます。
    - （正確に言うと32bit SPIでは MSB firstなのでbyteアクセスすると変則なbitmapにはなります）
    - 文字書きがグラフィックへの書き込みになるので少し処理が増えますが、毎HSYNC毎にフォント取得するよりはずっとましです。
    - SRAMが足りない場合は表示ウィンドウを少し狭くするという方法も可能です。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## [cdcbasic.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/cdcbasic.zip) ファームウェアの使い方。
HEXファイルをとりあえず試したい場合：
- HID bootloaderもしくはPICkit2のようなものから svgarom.hexを書き込みます。

<!-- dummy comment line for breaking list -->

~

ビルドして書き込みたい場合：
- 添付ReadMe.txtを参考に、MPLABとPinguinoをインストールします。
- makeします。
- HID bootloader(HIDBoot.X)からの書き込みは w.bat です。
- PICKit2(3)からの書き込みは wp.bat です。

<!-- dummy comment line for breaking list -->

- 添付ReadMe.txtにあわせて適当なVGAモニターを繋ぎます。

<!-- dummy comment line for breaking list -->

- USB-CDCデバイスとして認識されていますので、Teratermで繋ぐか、あるいは添付のw32term.exeをコマンドライン

<!-- dummy comment line for breaking list -->
から操作します。
	D:> w32term -p
	・・・COMデバイスの列挙・・・
	D:> w32term -c4   (4は、該当COMポートの番号)
	適当に文字を打つ

- 制御文字の実装

<!-- dummy comment line for breaking list -->
,^H,バックスペース
,^I,タブ
,^L,画面消去
,RETURN,改行


~
~
~
- - - -
## 番外編：Tiny Basic News Letter


元ネタ： 1990-1-1
- [http://www.programmersheaven.com/download/16060/Download.aspx](http://www.programmersheaven.com/download/16060/Download.aspx) 

<!-- dummy comment line for breaking list -->

RasPiにTinyBasicが移植されたって！こりゃ追っかけナイト。
- [http://www.staff.city.ac.uk/afl/tinybasic/downloads.html](http://www.staff.city.ac.uk/afl/tinybasic/downloads.html) 

<!-- dummy comment line for breaking list -->

- 早速ubuntuに突っ込んでみる。

		# libsdlを入れる。
		$ make
- 実行

		$ tinybasic prime-vanilla.tb
		This is vanilla TinyBASIC Two (2.1 23/12/2012)
		loaded 18 lines
		Ready
		&gt; list
		10 REM Find the first 1000 prime numbers in Vanilla Flavour
		20 REM P is a prime number candidate
		30 REM N is used to test primacy
		40 REM Z is the count of prime numbers found
		50 REM F is used to count the factors found of the prime candidate
		60 REM A.F.Lack, 8th Oct 2012
		100 let z=0
		110 let p=3
		120 let l=1
		150 z=z+1; print #1,z,") ",2," is prime"
		200    let f=0
		300    for n=2 to (p-1)
		400       if p%n=0 then f=f+1
		500    next n
		600    if f=0 then z=z+1; print #1,z,") ",P," is prime"
		700    p=p+2    ;REM the next candidate
		800 if z&lt;1000 goto 200
		999 stop
		&gt; run
		1) 2 is prime
		2) 3 is prime
		3) 5 is prime
		・・・
- 動くじゃん。すげーーーー。

<!-- dummy comment line for breaking list -->


続きを読む： [tinyBasic2](tinyBasic2.md) 

~
~
~
- - - -
## 参考リンク ：ソフトウェアＶＧＡの実装例 : 主にARM用

Arduino Due(ARM)用のVGA/PAL/NTSC出力ライブラリ
- https://github.com/stimmer/DueVGA/

<!-- dummy comment line for breaking list -->

STM32 VGA Text terminal (EN)
- [http://zalampu.ru/post/42830601388/stm32-vga-text-terminal-en](http://zalampu.ru/post/42830601388/stm32-vga-text-terminal-en) 

<!-- dummy comment line for breaking list -->

mbed New VGA 640x400 70Hz library
- https://developer.mbed.org/forum/mbed/topic/2459/

