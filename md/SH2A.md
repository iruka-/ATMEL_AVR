[2011-12](2011-12.md) 

## SH7262

IF 2010-06付録

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/sh2a.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/sh2a.jpg) 

- ジャンパーは上から、

<!-- dummy comment line for breaking list -->

|JPP2|NMI入力(ブートセレクト)|オープンでHEW起動/クローズで後半32kBのユーザーROM起動|
|JPP1|RESET|クローズでリセット|
|JPP3|SSL|ハンダ面のJP16パターン（ショート）と並列なので、常時クローズとなっている|

- JPP3はJP16パターンを切断したときに、基板上FlashROMを選択するためのジャンパー。オープンにすると基板外のFlashROMを接続出来るようになる。

<!-- dummy comment line for breaking list -->

- つまり、一番上の**JPP2だけ**が重要。HEWからSRAM上でデバッグするならはオープンのままでよい。

<!-- dummy comment line for breaking list -->

    - JPP2クローズするとFlashROM後半のユーザープログラム（初期状態ならＬチカ）が起動する。
    - JPP2オープンなら、FlashROM前半のHEWモニターが起動する。LEDは消灯のまま。

<!-- dummy comment line for breaking list -->


~
## 特徴
- なんと1MByteもある内蔵SRAM（FlashROMは外付け）
- 144MHz動作
- FPU内蔵
- スーパースカラー実行ユニットにより１クロックに２命令実行!
- USB HighSpeed (480Mbps ) host機能あり
- LCDコントローラー機能まであり。
- １年半も前の基板なのに超最新のSTM32F4を遥かに超える破壊力！

<!-- dummy comment line for breaking list -->

最強にして最凶（最も凶悪）な基板。

- PC9801のメモリー640kBから見たら、1MBですぜ！しかもD-RAMじゃなくて全部SRAM。
- 手のひらに乗るPC9801、というかセガサターン（SH2 x 2）ただしCPU部分だけ
    - **夢みたいだ**

<!-- dummy comment line for breaking list -->


~
で、どこが最凶なの？
- SRAM上であれば、HEWから統合環境を使ってプログラム＆デバッグできるし、性能は最強。
- だが、ROM化が大変。手順が複雑で年寄りには覚えきれない。しかも32kB以内に納めろというし。

<!-- dummy comment line for breaking list -->

- 複雑なシリアルＲＯＭブートを解説してくださっているＨＰ：
    - [http://white-clouds-in-sky.blogspot.com/2010/08/sh-2a-sdmmc.html](http://white-clouds-in-sky.blogspot.com/2010/08/sh-2a-sdmmc.html) 
    - [http://white-clouds-in-sky.blogspot.com/2010/05/sh-2arom.html](http://white-clouds-in-sky.blogspot.com/2010/05/sh-2arom.html) 

<!-- dummy comment line for breaking list -->

- さらにsh-elf-gccで作ったプログラムの場合は、そもそも統合環境デバッグできないっぽい（KPITなら出来るのだろうか？？？）
- それどころか、SRAMへのダウンロードもHEWが使えないので出来ないっぽいような気がする。（バイナリーかmotなら、できるのかな？でもＤＬしたあと走らせることができるのかどうか・・・）

<!-- dummy comment line for breaking list -->

~


- やはり、この基板のネックはブートローダー（ＨＥＷモニター）だ。
    - HEW上でやってる限りは完結しているが、
    - gccでやろうとするとUSBのHEWモニターは使えない。
    - HEW上でやるにしてもUSBを使うアプリの開発はできない（リソースが競合するため）。
    - シリアルポート経由のgdbスタブをブートＲＯＭの後半３２ｋＢに書いて、そっからブートしてgdb接続するという大昔の技を使うしかなさそうだ。
    - しかし、そうやって開発したコードをＲＯＭ化するすべがない。（かろうじてブートＲＯＭの後半３２ｋＢに置くことは出来るが、そうするとgdbスタブが消えてしまうし、そのまえにたったの３２ｋＢでどうするんじゃ？）
    - **1MBもの大容量ＳＲＡＭが泣いている。**

<!-- dummy comment line for breaking list -->



~
- セガサターンで思い出したけど、昔、MS-DOS6.2時代にdjgppというDOSエクステンダーなgccがあってだな、
- そのMS-DOS6.2上でsh-gccのクロスコンパイラをビルドしてた・・・ような気がする。
- たぶん気のせい。
    - OSはWindows95だったかもしれない。

<!-- dummy comment line for breaking list -->


- - - -
## 作業手順
- 基板はとくに手を加える必要はないけれど、動作不安定だった場合は、参考ＵＲＬのSYSLABさんのblogを参考に、電源周りの補強をしてみる。
- （１）Renesasのサイトに登録して、HEWとSH2A用のＣコンパイラを入手してインストール。
    - Ｃコンパイラは試用期間が過ぎたら128kB制限が掛かるけれど、ＰＩＣみたいに最適化ＯＦＦになるわけではないので、それなりに役に立つ。
- （２）CQのサイトから、SH2AStartUp.zipをダウンロードする。仮想COMのinfファイルはUSB接続時のドライバー

<!-- dummy comment line for breaking list -->
なのでUSBを繋いだときにインストールする。
- （３）同じくSH2AStartUp.zipにある、HEWmonitorUSB.zipを開いて、説明どおりにHEWをセットアップする。
    - これをセットアップしない限り、この基板にターゲット接続できない。
- （４）次に、同じくSH2AStartUp.zipにある、LチカサンプルをHEWプロジェクトで開いてビルド、実行してみる。
    - うまくいけばＬＥＤが点滅する。
- （５）最後に、同じ要領で、ROM_UpDate.zipを開き、中の説明どおりに実行するとHEWのファームを更新することが出来る。失敗すると文鎮基板の出来上がりだ。

<!-- dummy comment line for breaking list -->

- 自分のＰＣ(H61+Corei3)ではHEWと基板の接続や実行が全然うまくいかなかった。たぶんAMD系やOHCIホストのマザーでも同様かもしれない。結局USB1.1ハブを間に挟んで実行させたらうまくいった。

<!-- dummy comment line for breaking list -->

~
結構面倒いのよこれが。
- まあ、PICでもARMでも開発手順の構築はそれなりに面倒なんだけど、
- 一回やればわかる。
- でも、しばらくやらないと忘れる。

<!-- dummy comment line for breaking list -->

## メモリーマップ
- HEWでＤＬ＆実行（デバッグ）できるのはＲＡＭ上のプログラムのみ。

<!-- dummy comment line for breaking list -->

| 0x1C00_0000 〜 0x1C00_04FF |(1Mバイト大容量RAM領域)ユーザーベクター	|
| 0x1C00_0500 〜 0x1C0F_FFFF |(1Mバイト大容量RAM領域)ユーザプログラム,スタック領域	|
| 0xFFF8_0000 〜 0xFFF8_8FFF |(64Kバイト高速RAM領域)ユーザプログラム,スタック領域|
| 0xFFF8_9000 〜 0xFFF8_FFFF |(64Kバイト高速RAM領域)HEWモニタ領域,ダウンロード不可|

- シリアルEEPROMからブートしたブログラム(HEWか、自作ローダー)が64kバイト高速RAM領域を使用するので、直接的には高速RAMへロードできない。

<!-- dummy comment line for breaking list -->



- gccでビルドしたものを試したい場合は、UART2にシリアルを繋いで、以下のTinyMonからバイナリー

<!-- dummy comment line for breaking list -->
を流し込むようにする。
- [http://www.kumikomi.net/interface/editors/2010/05/gcc.php](http://www.kumikomi.net/interface/editors/2010/05/gcc.php) 

<!-- dummy comment line for breaking list -->



~
- - - -
## CQサイトにあるCygwin gcc(SH2A)でビルドする際の注意。

- 推奨： **CFLAGSに -m2a-nofpu を指定**すること。

<!-- dummy comment line for breaking list -->

    - そうしないと、割り込み発生後フォアグラウンド側のプロセスが死ぬ。
    - 理由は全くもって不明。
    - この回避方法をとった場合、残念ながらfpuが使えなくなる。
    
    		#The following options are target specific:
    		#  -m1                         Generate SH1 code
    		#  -m2                         Generate SH2 code
    		#  -m2a                        Generate SH2a code
    		#  -m2a-nofpu                  Generate SH2a FPU-less code
    		#  -m2a-single                 Generate default single-precision SH2a code
    		#  -m2a-single-only            Generate only single-precision SH2a code
    		#  -m2e                        Generate SH2e code
- なぜ死ぬのか？
    - タイマー割り込みハンドラーが以下のようにコンパイルされる。
    
    		1c002d20 &lt;INT_CMT0_CMI0&gt;:
    		1c002d20:	d1 18       	mov.l	1c002d84 &lt;INT_CMT0_CMI0+0x64&gt;,r1	! 1c009adc &lt;__fpscr_values&gt;
    		1c002d22:	2f e6       	mov.l	r14,@-r15
    		1c002d24:	71 04       	add	#4,r1
    		1c002d26:	4f 62       	sts.l	fpscr,@-r15
    		1c002d28:	41 66       	lds.l	@r1+,fpscr
    		1c002d2a:	ff 0b       	fmov	fr0,@-r15
    		1c002d2c:	71 fc       	add	#-4,r1
    		1c002d2e:	ff 2b       	fmov	fr2,@-r15
    		1c002d30:	ff 4b       	fmov	fr4,@-r15
    		1c002d32:	ff 6b       	fmov	fr6,@-r15
    		1c002d34:	ff 8b       	fmov	fr8,@-r15
    		1c002d36:	ff ab       	fmov	fr10,@-r15
    		1c002d38:	4f 02       	sts.l	mach,@-r15
    		1c002d3a:	4f 12       	sts.l	macl,@-r15
    		1c002d3c:	4f 52       	sts.l	fpul,@-r15
    		1c002d3e:	2f 06       	mov.l	r0,@-r15
    		1c002d40:	2f 16       	mov.l	r1,@-r15
    		1c002d42:	2f 26       	mov.l	r2,@-r15
    		1c002d44:	2f 36       	mov.l	r3,@-r15
    		1c002d46:	2f 46       	mov.l	r4,@-r15
    		1c002d48:	2f 56       	mov.l	r5,@-r15
    		1c002d4a:	2f 66       	mov.l	r6,@-r15
    		1c002d4c:	2f 76       	mov.l	r7,@-r15
    		1c002d4e:	d1 0e       	mov.l	1c002d88 &lt;INT_CMT0_CMI0+0x68&gt;,r1	! 1c000e68 &lt;int_cmt_cmi0&gt;
    		1c002d50:	4f 22       	sts.l	pr,@-r15
    		1c002d52:	41 0b       	jsr	@r1
    		1c002d54:	6e f3       	mov	r15,r14
    		1c002d56:	6f e3       	mov	r14,r15
    		1c002d58:	4f 26       	lds.l	@r15+,pr
    		1c002d5a:	67 f6       	mov.l	@r15+,r7
    		1c002d5c:	66 f6       	mov.l	@r15+,r6
    		1c002d5e:	65 f6       	mov.l	@r15+,r5
    		1c002d60:	64 f6       	mov.l	@r15+,r4
    		1c002d62:	63 f6       	mov.l	@r15+,r3
    		1c002d64:	62 f6       	mov.l	@r15+,r2
    		1c002d66:	61 f6       	mov.l	@r15+,r1
    		1c002d68:	60 f6       	mov.l	@r15+,r0
    		1c002d6a:	4f 56       	lds.l	@r15+,fpul
    		1c002d6c:	4f 16       	lds.l	@r15+,macl
    		1c002d6e:	4f 06       	lds.l	@r15+,mach
    		1c002d70:	fa f9       	fmov	@r15+,fr10
    		1c002d72:	f8 f9       	fmov	@r15+,fr8
    		1c002d74:	f6 f9       	fmov	@r15+,fr6
    		1c002d76:	f4 f9       	fmov	@r15+,fr4
    		1c002d78:	f2 f9       	fmov	@r15+,fr2
    		1c002d7a:	f0 f9       	fmov	@r15+,fr0
    		1c002d7c:	4f 66       	lds.l	@r15+,fpscr
    		1c002d7e:	6e f6       	mov.l	@r15+,r14
    		1c002d80:	00 2b       	rte
    		1c002d82:	00 09       	nop
    		1c002d84:	1c 00       	mov.l	r0,@(0,r12)
    		1c002d86:	9a dc       	mov.w	1c002f42 &lt;io_recvcheck+0x44&gt;,r10	! 2008
    		1c002d88:	1c 00       	mov.l	r0,@(0,r12)
    		1c002d8a:	0e 68       	.word 0x0e68
- プロローグコードでいきなりr1をぶっこわしているところを見ると、尋常ではない。

		1c002d20:	d1 18       	mov.l	1c002d84 &lt;INT_CMT0_CMI0+0x64&gt;,r1	! 1c009adc &lt;__fpscr_values&gt;
- 割り込んだらr1がぶっ壊れるなんて、そんなＣＰＵ聞いたことも無いよ。

<!-- dummy comment line for breaking list -->

- fpscrというレジスタは浮動小数演算レジスタの設定レジスタみたいなやつで、丸めモードとか**単精度32bitで使うかレジスタペアを倍精度64bitで使うかという重要なフラグ**とかが入っている。
- 割り込んだ瞬間には、どっちのモードか不定になるはずなので、__fpscr_valuesというわけわかんない変数からとってきて初期化するらしいんだが、・・・。もう何も言うまい。

<!-- dummy comment line for breaking list -->

~

- 少なくとも、gccの-m2aオプションと RESBANK無しモードの組み合わせでは使い物にならないということは**確定**。
- それから、gccの-m2aオプションと RESBANK有りモードで試した限りでも暴走することに変わりはない。
    - 理由は良く分からないが、__fpscr_valuesあたりのメモリー内容が0なのでそのせいかもしれない。
    - 回避策としては、割り込みハンドラーをＣ記述せずにアセンブラで書く。（下記ダウンロードにその例を入れた）

<!-- dummy comment line for breaking list -->



~
- - - -
## gccの選択
- Cygwin版では **-m2a-nofpu** （FPUを使わない）オプションを推奨。FPUは使用できない。
- KPIT版では、**-m2a** (FPUを使用、デフォルトは倍精度)を使用可能。確かめてはいないが、**-m2a-single**も可能だと思われる。

<!-- dummy comment line for breaking list -->

- Cygwin版でFPUを動かそうとすると、リンクすべき既定ライブラリもそのモードに合った(m2aかm2a-single)ものでリンクして、さらに、__fpscr_values[]の初期値と、fpscrレジスタの値を適切に設定しなければならないが、いまのところ成功していません。

<!-- dummy comment line for breaking list -->



~
- - - -
## 展望
- さいしょのうちはHEW+Renesas C で良いけれど、出来れば gccコマンドラインビルドがいいなぁ・・・。
- じゃあ32kBのユーザーランドに[cdcboot](libmapleでブートローダー.md) のようなものを乗せるしかない。
- で、この基板を使うには必ずホストPCとUSB接続して、SRAMに応用プログラムをダウンロード実行することになる。
- どうしても基板単独で使いたい場合は、外部にFlashROMを増設してそこからブートするようにするとか、SDカードを繋いで

<!-- dummy comment line for breaking list -->
何らかの方法でブートさせるとか、そんな感じ。




~
~
~
~
- - - -
## SH2Aオリジナルブートローダー：ダウンロード

仮）
- [sh2a.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/SH2A/sh2a.zip) 

<!-- dummy comment line for breaking list -->

- とりあえず、**gccでビルドできるブートローダー兼USB-CDCサンプル**（番地を変えることでブートされる側にもなれる）。
- CQのサイトにあるCygwin版gccを使ってビルド可能。
- SPIwriteUser.hwsを使ってSH2A-FFF82000.binをシリアルEEPROMに書き込んで使用する。
- 1MBのRAM空間にSH2A-1C000000.mot をブートすることで、いろいろ試すことも可能。
    - ブートすると、一時的にUSB-CDCが切断されて、自動でアプリケーション側のUSB-CDCに切り替わる。
- user_cmd()関数にテストを記述して、cdctool.exeから userコマンドで実行可能。
- cdctool.exeは両方のモードに対応している。
- cdcboot.exeを改造すれば、PICのブートローダーと同様に、アプリケーション実行中でも強制的にブートモードに戻したりも可能かもしれない。

<!-- dummy comment line for breaking list -->

    - ファームサイズは26.5kB

<!-- dummy comment line for breaking list -->






~
## 使い道。
- 480MbpsのUSBがあるので、(しかもHOSTになれる)
- たとえば、SDカードを繋いで（12Mbpsでない）実用速度の出るUSBメモリーとか。（笑うしかない）
- USBメモリーじゃなくてUSB-CDROMとかUSB-FDDをSDカード媒体でエミュレーションすれば、OSインストールやPCメンテナンス用の起動媒体になる（最近のマザーボードは普通のUSBメモリーで問題無くブートするので、古いマザーをメンテするときだけのニッチな需要だ。）

<!-- dummy comment line for breaking list -->



- - - -
参考ＵＲＬ

◆ CQ出版 インタフェース 2010/6月号 付録基板CQ-FRK-SH2A SH-2AにMESを移植する ◆
- [http://www.mai-denshi.co.jp/MAI-MES_SH2A.html](http://www.mai-denshi.co.jp/MAI-MES_SH2A.html) 

<!-- dummy comment line for breaking list -->

SYSLAB blog
- [http://syslab.asablo.jp/blog/cat/sh2/?offset=10](http://syslab.asablo.jp/blog/cat/sh2/?offset=10) 

<!-- dummy comment line for breaking list -->


SYSLAB blog SH2A問題点
- [http://syslab.asablo.jp/blog/cat/sh2/?offset=20](http://syslab.asablo.jp/blog/cat/sh2/?offset=20) 

<!-- dummy comment line for breaking list -->

SH-2A（SH72620)　開発用　GNU　GCC　環境整備中 
- [http://white-clouds-in-sky.blogspot.com/2010/06/sh-2ash72620gnugcc.html](http://white-clouds-in-sky.blogspot.com/2010/06/sh-2ash72620gnugcc.html) 

