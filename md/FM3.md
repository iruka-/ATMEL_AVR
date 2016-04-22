[2012-04](2012-04.md) 

## FM3付録基板

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/FM3img.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/FM3img.jpg) 

とりあえず
- [armon/armboot](armon.md) （のUSBカスタムデバイス版）を移植してみる。

<!-- dummy comment line for breaking list -->

~


- - - -
## ＦＭ３開発環境の入手（gcc)


- Maple IDE に 安定版（おすすめ）の CodeSourcery G++ Liteが含まれています
- 下記ページから

<!-- dummy comment line for breaking list -->

~
    - [http://leaflabs.com/docs/maple-ide-install.html](http://leaflabs.com/docs/maple-ide-install.html) 
    
    		Download¶
    		------------------
    		 Choose the correct version for your operating system:
    		 Platform Status
    		 Windows XP Tested on 32-bit Windows XP
    		 ~~~~~~~~~~
- をクリックして、maple-ide-0.0.12-windowsxp32.zip をダウンロードします。
- OS環境がWindowsでない場合は、適宜それぞれのバイナリーを入手してください。

<!-- dummy comment line for breaking list -->

#### 入手したら、（例えば）D:\ドライブ直下に展開します。

<!-- dummy comment line for breaking list -->

- そうすると、以下のディレクトリにgccの実行ファイル(arm-none-eabi-gcc.exeなど)が出来ます。

		D:\maple-ide-0.0.12-windowsxp32\hardware\tools\arm\bin\
- そのディレクトリに実行パスを通します。

		D:\&gt; path D:\maple-ide-0.0.12-windowsxp32\hardware\tools\arm\bin;%PATH%
		     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- 実行パスの通し方は、コントロールパネル->システム->詳細設定->環境変数　を選んで、ユーザーの環境変数PATH

<!-- dummy comment line for breaking list -->
を新規に作成して、値を
	D:\maple-ide-0.0.12-windowsxp32\hardware\tools\arm\bin
- にするか、上記の下線部を書いた短いファイル名のバッチファイルを、すでに実行パスが通っている場所に置いて、DOSプロンプトを開いた後で実行するか、どちらでもＯＫです。

<!-- dummy comment line for breaking list -->


- - - -
## ＦＭ３開発環境でのコンパイルのしかた。
- 下記の[fm3vcom.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/fm3vcom.zip) を入手して、同じようにD:\ドライブ直下に展開してください。
- そしてDOSプロンプトから、該当のディレクトリに移動します。

		D:&gt; cd \fm3vcom\src
		D:&gt; cs-make
- cs-makeでは、最初に .dep/ ディレクトリが無いというエラーが出る場合がありますので、そのときは、

		D:&gt; cd \fm3vcom\src
		D:&gt; mkdir .dep
		D:&gt; cs-make
- という風に .depという名前の依存ファイル用のテンポラリディレクトリを用意しておくとうまくいきます。
- cs-make以外の、普通のGNU make( MinGWやWinAVRなどに付属のもの)では .depは自動作成されるようです。

<!-- dummy comment line for breaking list -->






~
~
~
~

- - - -
## USBカスタムファーム・ダウンロード

これは、FM3基板をUSBカスタム(BULK)デバイスにしてしまうファームウェアです

ファームウェア(16kB)、Windows用クライアント(コマンドラインツール)、ソースファイル一式
- [armboot.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/FM3/armboot.zip) 

<!-- dummy comment line for breaking list -->

~
- 急ごしらえなので、微妙にだめなところも残っています。
    - ~~例えば、printfの結果をPCに転送する処理がバルク転送対応出来ていないのでuser関数のprintとか動きません。~~ ---動いているみたいです。

<!-- dummy comment line for breaking list -->

- なぜかarmboot.exeまで動いています。**不思議。**３時間のやっつけ仕事なのに・・・

<!-- dummy comment line for breaking list -->

- コマンドラインツールの実行例です。

		E:\armboot\src&gt;armon
		TARGET DEV_ID=f3 VER=1.1(Bootloader) FLASH USED=7560,TOTAL=100000
		ARM&gt; d
		00000000 00 ff 00 20 5d 03 00 00  01 02 00 00 05 02 00 00
		00000010 09 02 00 00 0d 02 00 00  11 02 00 00 15 02 00 00
		00000020 19 02 00 00 1d 02 00 00  21 02 00 00 25 02 00 00
		00000030 29 02 00 00 2d 02 00 00  31 02 00 00 35 02 00 00
		00000040 39 02 00 00 3d 02 00 00  41 02 00 00 45 02 00 00
		00000050 49 02 00 00 4d 02 00 00  51 02 00 00 55 02 00 00
		00000060 59 02 00 00 5d 02 00 00  61 02 00 00 65 02 00 00
		00000070 69 02 00 00 6d 02 00 00  71 02 00 00 75 02 00 00
		ARM&gt; l 35c
		0000035c b508           push    {r3, lr}
		0000035e 4b22           ldr     r3, [pc, #136]  ; r3 = #$1fff0238
		00000360 4a22           ldr     r2, [pc, #136]  ; r2 = #$55aa0f00
		00000362 6819           ldr     r1, [r3, #0]
		00000364 4291           cmp     r1, r2
		00000366 d10c           bne.n   $00000382
		00000368 6858           ldr     r0, [r3, #4]
		0000036a 3201           adds    r2, #1
		0000036c 4290           cmp     r0, r2
		0000036e d108           bne.n   $00000382
		00000370 6899           ldr     r1, [r3, #8]
		00000372 3201           adds    r2, #1
		00000374 4291           cmp     r1, r2
		00000376 d104           bne.n   $00000382
		00000378 68d8           ldr     r0, [r3, #12]
		0000037a 2100           movs    r1, #0
		0000037c 6019           str     r1, [r3, #0]
		0000037e 6843           ldr     r3, [r0, #4]
		00000380 4798           blx     r3
		00000382 2069           movs    r0, #105
		ARM&gt; p
		  FM3_GPIO.PDIR0(0x40033300)     0x03bf 00000011_10111111
		  FM3_GPIO.PDIR1(0x40033304)     0xfffe 11111111_11111110
		  FM3_GPIO.PDIR2(0x40033308)     0x03ff 00000011_11111111
		  FM3_GPIO.PDIR3(0x4003330c)     0xffff 11111111_11111111
		  FM3_GPIO.PDOR0(0x40033400)     0x0000 00000000_00000000
		  FM3_GPIO.PDOR1(0x40033404)     0x0000 00000000_00000000
		  FM3_GPIO.PDOR2(0x40033408)     0x0000 00000000_00000000
		  FM3_GPIO.PDOR3(0x4003340c)     0x0000 00000000_00000000
		ARM&gt; q
		Bye.
- メモリーダンプ、逆アセンブル、ポート参照/変更など出来ます。

<!-- dummy comment line for breaking list -->

- - - -
#### コマンドラインツール：ブートローダーの使い方
- 4000番地〜に作成したプログラム（HEXファイル）をFLASHに転送して実行する。

		D:&gt; armboot -r FM3-4000.hex
- ~
- 既に書き込み済みのプログラム(4000番地)を起動する。

		D:&gt; armboot -r
- もしくは

		D:&gt; armon
		ARM&gt; boot 4000
- armonのコマンドでは、任意の番地から始まる(そこにvectorを置いている)ファームウェアを起動できます。
- ただし、armbootで書き込むときは、ページ単位でEraseされますので、vectorの開始番地はFLASHのページ先頭に置かないと消されてしまう場合があります。

<!-- dummy comment line for breaking list -->

|FLASHのページ先頭アドレス|サイズ|
|0|16kB|
|0x4000|16kB|
|0x8000|96kB|
|0x20000|128kB|
|0x40000|128kB|
|0x60000|128kB|
|0x80000|128kB|
|0xa0000|128kB|
|0xc0000|128kB|
|0xe0000|128kB|

~
- ブートローダーを4000番地あるいはそれ以外の番地から始まるようにビルドしたアプリケーションを試している場合であれば、ブートローダーとアプリケーションを互いにbootコマンドで行き来できます。

		D:&gt; armon
		TARGET DEV_ID=f3 VER=1.1(Bootloader) FLASH USED=7560,TOTAL=100000
		ARM&gt; boot 4000  ＜＝＝＝アプリケーションを起動する。
		bye
		D:&gt; armon
		TARGET DEV_ID=f3 VER=1.1(Application) FLASH USED=7560,TOTAL=100000
		ARM&gt; boot 0  ＜＝＝＝ブートローダーに戻る。
		bye
- ~
- アプリケーションが起動している状態からでも、4000番地以降のアプリの書き換えが出来ます。

		D:&gt; armboot -r FM3-4000.hex
- これは、実際には内部的にboot 0 を実行した後、USB再接続して4000番地〜のFLASH書き換えと実行を行ないます。
- ただし、アプリケーションはブートローダーと同じプロトコルを解するUSBカスタムデバイスの機能を持っている必要があります。（単に番地を変えてビルドするだけで可能です。）

<!-- dummy comment line for breaking list -->


~
#### Linuxでの使用
- ファームウェアはLinux上でも普通にビルドできます。
- FLASH USB Direct ProgrammerはWindows版しか提供されていないため、WindowsかWineのようなもので焼きこむしかありません。
- Windows側コマンドラインツールは、そのままLinuxでビルドできます。(libusb-devなどを導入する必要があります）
- armon/armbootを実行する場合はusbデバイスのアクセス権が必要です。
    - root権限で、/etc/udev/rules.d/95-my-permissions.rules に以下の内容を書き込むか、あるいは直接rootユーザーでarmon/armbootを起動してください。

<!-- dummy comment line for breaking list -->

	SUBSYSTEM=="usb",SYSFS{idVendor}=="04c5",SYSFS{idProduct}=="4242",MODE="0666"

	$ sudo /etc/init.d/udev restart

- 実在Linuxでない場合、たとえばVMWarePlayer内のLinuxからはアクセスがうまくいきませんが今のところ原因は分かっていません。

<!-- dummy comment line for breaking list -->



~
#### 残念なお知らせ。
- VectorTableのベースアドレスを設定するVTORレジスタの存在を見落としていました。
- ~~VTORを使用するように修正すれば、上記r2,r3破壊問題は簡単に解決します。~~
- ~~ついでにブートローダーのコードサイズは512byte以上縮めることも可能です。~~ ---処置しました。

<!-- dummy comment line for breaking list -->

VectorTableのベースアドレスを設定するレジスタはてっきりNVIC内にあるものだと思い込んでいました。



~
~
~
- - - -
## その２：仮想ＣＯＭポート作成サンプル
ダウンロード
- [fm3vcom.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/fm3vcom.zip) 

<!-- dummy comment line for breaking list -->

- これも急ごしらえです。
- ~~（いろいろブートローダー系のコードが残ったままです。）~~--修正済み

<!-- dummy comment line for breaking list -->

- 富士通謹製のサンプル mb9bf506r-usbfunc-msc-v11.zip （ＭＳＣデバイス）を基に作成しています。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## その３：FM3でprintfを使ってみる

上記のFM3カスタムファームウェアを使用します。

ファームウェア(16kB)、Windows用クライアント(コマンドラインツール)、ソースファイル一式
- [armboot.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/FM3/armboot.zip) 

<!-- dummy comment line for breaking list -->

- 上記zipファイルを D:\ に展開します。
- armboot/src/FM3-0000.hex を GUIの「USB Direct Programmer」(富士通サイトから入手)により焼きこみます。
- BOOTジャンパーをoffにして、リセットします。
- armboot/monitor/armon.exe を起動して、FM3基板に接続できることを確認しておきます。
- mapleIDEをダウンロードして、ARM用gccが使えるようにしておいてください。

<!-- dummy comment line for breaking list -->

- では、ビルドを行なってみます。

		D:\armboot\src\&gt; cs-make   　　（もしくは MinGWをインストールしているなら make）
		D:\armboot\src\&gt; w.bat         （FM3-4000.hex をブートローダー経由でFM3基板に書き込んで、即実行します）
		#
		D:\armboot\src\&gt; a.bat         （armon.exeを起動して、script というテキストファイルに書かれた処理を実行します）
		D:\armboot\src\&gt;..\monitor\armon.exe -i script
		TARGET DEV_ID=f3 VER=1.1(Application) FLASH USED=106b0,TOTAL=10000
		ARM&gt; user
		* Start Program
		Hello World
		Hello : !"#$%&amp;'()*+,-./0123456789:;&lt;=&gt;?@ABCDEFGHIJKLMNOPQ
		Hello : !"#$%&amp;'()*+,-./0123456789:;&lt;=&gt;?@ABCDEFGHIJKLMNOPQ
		p=0x1fff9120
		q=0x1fff9528
		sin(                0 ) =                0
		sin(         0.628319 ) =         0.587785
		sin(          1.25664 ) =         0.951057
		・・・
- printfを行なっているプログラムは、armboot/src/monitor/usercmd.c にあります。

<!-- dummy comment line for breaking list -->

- I/Oポートのコントロールは pinMode(),digitalRead(),digitalWrite()という関数が用意されているので、

<!-- dummy comment line for breaking list -->
それらを呼び出すことで簡単な処理を書くことも出来ます。

- make , w.bat (FLASH書き込み) , a.bat (armonによるユーザー関数の実行) を一連に実行するようなバッチファイル

<!-- dummy comment line for breaking list -->
を用意することで、Arduino風（プログラムを書き換えて即実行）のコマンドラインな開発が出来ます。




~
~
~
~
- - - -
## 予定稿１：Ｌチカサンプルを書いてみる？
- USBファンクションを取り去った状態で、Arduinoっぽい関数(digitalWriteなど)を実装して、最小限のLEDブリンクを書いてみました。

<!-- dummy comment line for breaking list -->

ダウンロード
- [led.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/FM3/led.zip) 

<!-- dummy comment line for breaking list -->


- KEILとかIARの環境では、startupがアセンブリで書かれていて融通が利かないため、ここをＣ言語（crt0.c）で書いてメンテしやすくしております。
- ビルドには、gcc ( CodeSourcery G++ Lite を推奨 ) とmakeを使用します。

<!-- dummy comment line for breaking list -->








~
~
~
~
- - - -
## 予定稿２：VGA出力テスト。
- はたして気力が続くかどうか・・・

<!-- dummy comment line for breaking list -->

？？？




~
~
~
~
- - - -
## 以下、雑談

- せっかくFujitsuなので、FM8とかFM7のエミュレータを動かしてみたいのだけれど、バイナリー持ってません。
- 当時のMZ-****とかPC-8***とかのバイナリーもほとんど持ってないです。

<!-- dummy comment line for breaking list -->

- バイナリー持っていたとして、エミュを書いたとしても、BIOS ROMの抜き取りするためには**どうしても実機が必要になります。**

<!-- dummy comment line for breaking list -->

そういうわけでエミュ遊びはおあずけです。

~
せっかくだから[Lua](http://www.lua.org/) とか[gforce](http://www.gnu.org/software/gforth/) とか動かしてみようかなとか思ったのですが、
- LuaはWindows版がうまく使いこなせなかったのでパス。（もともとPascal好きじゃないし）
- Forthは昔やってたけれど、やっぱり人間が読むコードじゃない感じなのでパス。（実行速度は速いんだけどねぇ・・・）
- 文法的にはC言語に良く似ていて、内部的にはForthインタプリタの [slang](http://www.s-lang.org/)  あたりが一番良いのかなぁ・・・とっつきやすさ的には。

<!-- dummy comment line for breaking list -->



~
~
~
- - - -
## 参考リンク
電子工作マスターへの歩み jujurouさん
- [http://jujurou.blog34.fc2.com/](http://jujurou.blog34.fc2.com/) 

<!-- dummy comment line for breaking list -->

~
流石、対応速いです。

~
~
~
独り言：
- こちらでも、一応1MB Flash対応のflash/nor/fm3.cは書いてみました。（つもり）
- だけど、この石、USB bootloaderのROMが載っているので、OpenOCDは要らないのでした。
- armon/armbootのブートローダーでごちゃごちゃやってます。(NXP用語で言うところのISP/IAP (In Application Programing)ってやつ)

