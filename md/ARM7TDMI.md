[ARM](ARM.md) 

## interface 2009-05付録基板で遊ぶ



![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/cq-frk-nxp.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/cq-frk-nxp.jpg) 


- とりあえず、付録基板を手に入れてみたものの、どうすればよいか分からず途方にくれました。
- LEDチカチカのHEXファイルは、

<!-- dummy comment line for breaking list -->
[interfaceダウンロードページ](http://www.cqpub.co.jp/interface/download/contents.htm) 
の2009-４月号第三章のCQ.lzhを用います。
- ARM基板に書き込むツールは[FlashMagic](http://www.flashmagictool.com/) を使います。
    - ボーレートが不明だったのですが、とりあえず230400bpsの設定で書き込めました。マニュアル等では9600bps

<!-- dummy comment line for breaking list -->
であるように書かれていて、よくわかっていません。
    - FlashMagic以外では、avrspみたいな書き込みツール[lpcsp](http://elm-chan.org/works/sp78k/report.html) があるそうです。

<!-- dummy comment line for breaking list -->



- コンパイラは~~GNUARM~~[WinARM](http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/#winarm) 

<!-- dummy comment line for breaking list -->
を使う予定です。interfaceのサンプルはIAR用に書かれていますが、IARは有償です。
    - 30日試用版か32kB制限版は無償ですが、ダウンロード時に個人情報を記入する必要があります。

<!-- dummy comment line for breaking list -->

- NXP LPC-2388用のヘッダーファイルやサンプルプロジェクトが欲しいのですが・・・どこにあるのか分かりません。

<!-- dummy comment line for breaking list -->

    - [このへん](http://www.standardics.nxp.com/support/documents/microcontrollers/all/?scope=LPC2388) にありました。

<!-- dummy comment line for breaking list -->


リンク切れてますね。[このへん？](http://ics.nxp.com/support/documents/microcontrollers/?search=LPC2388&amp;Search.x=0&amp;Search.y=0) 

## 目標

短期的なもの
- WinARMをインストールする。 --- 済み
- LEDチカチカをWinARMでビルド。--- 済み
- その他サンプルをWinARMでビルド。 --- 済み

<!-- dummy comment line for breaking list -->

DownLoad: [LEDチカチカのWinARM版](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/ARM/ledtest.zip) を用意致しました。
~
~
~
中期目標 
- 簡易的なシリアル接続のmonitor(H8用)を移植する。--- 済み

<!-- dummy comment line for breaking list -->

- OpenOCDを使ってみたい。
- [NXPのHP](http://www.standardics.nxp.com/support/documents/microcontrollers/all/?scope=LPC2388) にあるHIDサンプルとかUSB仮想COMポートのサンプルをWinARMでビルドしたい。

<!-- dummy comment line for breaking list -->

DownLoad: [H8TinymonのWinARM版](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/ARM/ledmon.zip) を用意致しました。
~
~
~
長期的目標 
- オーディオ帯域のオシロ＆ロジアナアプリ。
- 各種ＵＳＢデバイスを書く練習。
- ＰＳ２キーボードをプロトコル変換して、ＵＳＢの謎デバイス(?)に接続する。

<!-- dummy comment line for breaking list -->



~
~
~
~
## Installメモ
[WinARM](http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/#winarm) から、WinARM_20080331_testing.zip をダウンロード。
	C:\> unzip WinARM_20080331_testing.zip
	C:\> path %path%;C:\WinARM\bin;C:\WinARM\utils\bin
- C:\に展開すると、C:\WinARM\ 以下にコンパイラがインストールされる。
- 実行パスを C:\WinARM\bin と C:\WinARM\utils\bin の両方に通す。
- ここまではWinAVRとほとんど同じ使い勝手です。

<!-- dummy comment line for breaking list -->

- コンパイラはarm-elf-gcc ではなく arm-eabi-gcc になります。

<!-- dummy comment line for breaking list -->

参考：ARM gcc バッドノウハウ集
- [http://jr0bak.homelinux.net/~imai/linux/arm_gcc_badknowhow/arm_gcc_badknowhow.html#toc8](http://jr0bak.homelinux.net/~imai/linux/arm_gcc_badknowhow/arm_gcc_badknowhow.html#toc8) 

<!-- dummy comment line for breaking list -->



## 遊び方
- ELMさんのサイトの[I/F誌付録LPC2000基板の試食](http://elm-chan.org/junk/cq_lpc/report.html) にある、ファームウェア(WinARMプロジェクト)をダウンロード、展開します。

<!-- dummy comment line for breaking list -->

- Makefile中に定義されているｇｃｃコマンド名をarm-elf-gcc などから、arm-eabi-gcc のように書き換えます。

<!-- dummy comment line for breaking list -->

- 普通にMakeして、同じくELMさんのサイトにある、[lpcsp](http://elm-chan.org/works/sp78k/report.html) を使って付録基板にHexを焼きこみます。
- ブートジャンパーを外してＵＳＢを繋ぎ直すか、基板をリセットします。
- teraterm等で基板のCOMポートに接続します。ボーレートは230400でＯＫでした。

<!-- dummy comment line for breaking list -->

'>'プロンプトが出たところで、
	>md 0
とやるとメモリーダンプを実行出来ます。

メモリーダンプ以外に、SDカード上のFATファイルをダンプしたりディレクトリを切ったりする便利コマンドが含まれているようです。

## ELMさんのファームウェア
- mp3プレイヤーのタスクと、SDカードの読み出し機能を含む対話型シリアル端末のタスクを

<!-- dummy comment line for breaking list -->
asmfunc.S内のタスクディスパッチャーで切り替えている。
- タスクの登録はStartTask()関数に、関数アドレスとスタックエリアを与えて行っている。
- タスクのディスパッチはmain()関数内の無限ループ

			/* Drive threads */
			for (;;) {
				DispatchTask(TaskContext[0]);
				DispatchTask(TaskContext[1]);
			}
- で行っているようです。
- タスクの切り替えはタスク内からSleep()関数を呼び出すことで、main()関数のループ内

<!-- dummy comment line for breaking list -->
に戻るようになっています。Sleepの引数が非ゼロの場合は所定Tickの間だけタスクの実行を遅らせているようです。



- - - -
## ARM7TDMI:NXP LPC2388用の適当なサンプルフレームワークはどれがいい？
- 一応、[ARM_Project](http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/#winarm) 

<!-- dummy comment line for breaking list -->
のＨＰにある、
    - GPIO, UART and interrupt example for the NXP LPC2378, NXP LPC2368 and other NXP LPC23xx/24xx devices
    - lpc2378_demo1_20070629.zip

<!-- dummy comment line for breaking list -->

- がお勧め。
- LPC2378とLPC2388の違いは、RAMの容量(32kB/64kB)の違いとUSBがOn the Goかどうかだけ、のようだ。
- だから、リンカスクリプト(Common/LPC2378-ROM.ld)だけ書き換えて使えば良い。

<!-- dummy comment line for breaking list -->

- USBを含む、そのようなお手本に関してはまだ見つからないが、上記の通り[NXP](http://www.standardics.nxp.com/support/documents/microcontrollers/all/?scope=LPC2388) の

		Sample Code Bundle for LPC23xx/LPC24xx Peripherals using Keil's μVision, V1.60 (Mar 10, 2009)
		(code.bundle.lpc23xx.lpc24xx.uvision.zip )
- を適当に改造して使うことになりそうだ。
- KeilとWinARMでは割り込みハンドラーの扱いがかなり異なっているような気がするので、要注意なとこだけか。
- 結局asmソース(Startup.S irq.S)の移植は不要。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## ARM基板を何に使うかという読者向けネタ振り。
- 速度、Flash容量、SRAM容量とも充分にある。
- ペリフェラルも豊富過ぎるくらいある（EtherとかUSB On the Goまである)
- 出来ないことと言えば、BeagleBoardのようにLinuxを動かしたりPDAにするのはちょっと無理っぽい。

<!-- dummy comment line for breaking list -->

~
では、どんなことをしたら面白いだろうか？思いついたことを書いてみる（勿論、実行するわけではない）
- Arduinoもどき。**ARMduinoっぽい**みたいな感じのもの。
    - リソースはArduino MEGAを遥かに凌ぐ。
    - しかもフル３２ビット演算全開。

<!-- dummy comment line for breaking list -->

- Gainerもどき。 **Gainer ARM**とでも名付ける。
    - I/Oが100本くらい使用できる。
    - クリスマス電飾を全部ソフトでコントロール。（いや、あれは１００本も制御点がない）
    - どうやってKONFIGURATIONしようか・・・？

<!-- dummy comment line for breaking list -->

- USBクラスをいろいろ実験
    - USB Audio / USB CDC / USB storage / USB HID / ...
    - 普通にStorageだとつまらないので、 SD/MMCデバイスをUSB FloppyやUSB CDROMに見せかけるドライバとか。
    - On the go がついているのでUSBブリッジっぽいことが出来る？
    - 例えばPC to PCのデータ転送ケーブル（昔あった）

<!-- dummy comment line for breaking list -->

- USB Midiクラスで楽器。
    - あの楽器？？？！
    - Midiフォーマットの復習には手ごろ。

<!-- dummy comment line for breaking list -->

- On the Goで何か面白いことが出来ないか考えてみる。
    - いろんなUSBデバイスをシリアルに変換する等。

<!-- dummy comment line for breaking list -->

- なつかＰＣのエミュレータ
    - 一応RAM64Kなので、Z80エミュとか入れるとＭＺ８０ＫやＸ１の低速エミュレータが作れるはず。
    - ビデオ周りやサウンド周りはさすがにFPGAのように高速にはいかないだろうけれど・・・。

<!-- dummy comment line for breaking list -->

- めちゃ古い機械接点キーボードなどをソフトスキャンしてUSB HID Keyクラス。
    - むしろPICでやるべきことか。

<!-- dummy comment line for breaking list -->

- USB接続のUV-EPROMライタ
    - いや、FlashROM全開の時代に完全に逆行してるんですけど。殺菌灯とか要るし。

<!-- dummy comment line for breaking list -->

- 外部バス(8bitミニバス アドレスは16bit)が出ているのでそこにVRAMを実装してミニVGA

<!-- dummy comment line for breaking list -->

- OpenOCD JTAG系のツール。
    - それこそAVRでAVRライタばかり作ってるのと同類になってしまう・・・。

<!-- dummy comment line for breaking list -->

- USBバスプロトコルアナライザ
    - マイコンでやるよりも48MHzくらいでFPGAかCPLDを回すのが正解だけれど、PCとやり取りするコントローラとしてはＯＫ。
    - むしろ解析ソフトを組むのが大変なので、大仕事になるとおもう。

<!-- dummy comment line for breaking list -->

- ＰＣ接続のロジアナ、オシロ系
    - これもFPGAかCPLDのほうが向いている。

<!-- dummy comment line for breaking list -->

いろいろ考えてみたけれど、512Kのフラッシュを満たせるような用途は思いつかなかった。
- むしろ流行のロボット系メカの脳みそ用

<!-- dummy comment line for breaking list -->



## [interface 2009-05付録基板で遊ぶPartII](ARM7mon.md) に続く・・・



~
~
~
~

## リンク集

interface 5月号付属ARMマイコン基板の使用上の注意点（重要！！）、よくあるご質問等
- [http://kumikomi.typepad.jp/interface_2009arm/](http://kumikomi.typepad.jp/interface_2009arm/) 

<!-- dummy comment line for breaking list -->

interfaceダウンロードページ
- [http://www.cqpub.co.jp/interface/download/contents.htm](http://www.cqpub.co.jp/interface/download/contents.htm) 

<!-- dummy comment line for breaking list -->

ARM_Project
- [http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/#winarm](http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/#winarm) 

<!-- dummy comment line for breaking list -->

アルゴ算法堂:ARMアセンブラ解説
- [http://homepage2.nifty.com/sampodo/craft/arm00.html](http://homepage2.nifty.com/sampodo/craft/arm00.html) 

<!-- dummy comment line for breaking list -->

ELM:I/F誌付録LPC2000基板の試食
- [http://elm-chan.org/junk/cq_lpc/report.html](http://elm-chan.org/junk/cq_lpc/report.html) 

<!-- dummy comment line for breaking list -->

ARMUZK
- [http://www002.upp.so-net.ne.jp/uratan/ArmUZK/](http://www002.upp.so-net.ne.jp/uratan/ArmUZK/) 
- UZK は、「キーボード・マウス共有器」、あるいは「延長器」、あるいは 「PC をまるごと USB キーボード・マウスにするためのブリッジ装置」です。

<!-- dummy comment line for breaking list -->

SYSLAB blog
- [http://syslab.asablo.jp/blog/cat/arm/](http://syslab.asablo.jp/blog/cat/arm/) 
- ARM以外にFR60とかV850とかH8/*とかR8CとかdsPICとかいろいろ網羅されている。

<!-- dummy comment line for breaking list -->

ARMプロセッサ対応 JTAG-ICE "DAIICE for ARM"
- [http://www.ucsr-japan.net/Products.html](http://www.ucsr-japan.net/Products.html) 
- [http://takekun.blogzine.jp/castle/](http://takekun.blogzine.jp/castle/) 
- FT2232のJTAGアダプタって、どうやって作ればいいんだろう・・・買ってくるしかないのかな？

<!-- dummy comment line for breaking list -->

FT2232のJTAGアダプタは、Amontec JTAG Keyが有名（２９ユーロ）
- [http://www.amontec.com/jtagkey-tiny.shtml](http://www.amontec.com/jtagkey-tiny.shtml) 
- [http://fenrir.naruoka.org/archives/000541.html](http://fenrir.naruoka.org/archives/000541.html) 

<!-- dummy comment line for breaking list -->

- 回路図は公開されているので自分で作れる。

<!-- dummy comment line for breaking list -->

~

各種JTAGアダプタを自作 COLOR(RED){New!}
- [ATtiny2313でJTAGアダプタを作る](hid_blaster.md) 
- [PIC18F2550/14K50でJTAGアダプタを作る](pic18blaster.md) 
- [STBEE_MINIでJTAGアダプタを作る](armblaster.md) 

<!-- dummy comment line for breaking list -->

~

ARM PACMAN (AVRでもPACMANを作成されている方)
- [http://www7.big.or.jp/~kerokero/arm/](http://www7.big.or.jp/~kerokero/arm/) 

