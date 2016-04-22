[2009-08](2009-08.md) 　[Arduino400](Arduino400.md) 

- - - -
目次




- - - -
## PIC 18F14K50 + Arduino ATmega8 計画

**Arduinoなんて、やったこともないのに、無謀にもUSB-PIC＋ATmega8という組み合わせで挑戦中です。**


時系列
- ATmega8(秋月で200円)の電源とTx/Rxを配線.
- MCHIPのUSBシリアルエミュレータのHEXを焼く.
- 通電
- MCHIPのUSB infをインストール
- Arduinoを起動
- スケッチを流し込む
- シリアルが停止エラー

<!-- dummy comment line for breaking list -->
シリアルポートが(Windowsの)エラーを吐いてとまる。

しかたがないので、
- ATmega8を抜く。
- ２番ピンと３番ピン(Rx/Tx)をショート。
- エコーバックテスト中。

<!-- dummy comment line for breaking list -->

エコーバックしません。

- Tx/Rxをテスターで計るとほぼ５Ｖ。
- teratermのコンソールでキーを入力すると微妙に電圧が変わるので、Txは行われているっぽい。
- エコーバックが帰ってこない。どんなボーレートでも。

<!-- dummy comment line for breaking list -->

PICはヘタレです。

~
~
~
~
- - - -
## なんとなくエコーバックまでパス。
- その後、ＰＣを取り替えただけでUSB-シリアルファームはエコーバックを返すようになりました。
- Arduinoをインストール、日本語化したのち、LEDブリンクを書き込んでみました。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/ard-err1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/ard-err1.jpg) 

- 残念です。

<!-- dummy comment line for breaking list -->

- 普通にTeratermで接続してキーを打っても、文字は何も返ってきません。
- これでは、何が悪いのかさっぱり・・・。

<!-- dummy comment line for breaking list -->

- ATmega8のRxDはちゃんと＋５Ｖになっていますが、TxDは通電後数秒で０Ｖになります。
- 現在ATmega8の動作を疑っているところ。

<!-- dummy comment line for breaking list -->

ただし、
- ここでヘタれてリアルＲＳ２３２ＣをATmega8に繋いだら負けです。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## ATmega8もなんとなく動いているようで・・・
- SCK(pin19)にテスターを当てると、１秒単位で上下している。
- ローダーは生きているっぽい。
- 水晶は16MHzを与えている。
- fuseはたぶん合っている。
- シリアル経由でいじっても返答なし。
- board.txtを見る限りだと19200bps

<!-- dummy comment line for breaking list -->

- ブートローダーのソース見るしかないのかな？

<!-- dummy comment line for breaking list -->

ソースを見ながら見よう見まね。

	E:\Program files\arduino-0016>w32term -b19200 -c5
	19200
	Entering TerminalMode: Escape=^A ＜＝＝ここですばやく '1' ' ' を入力すると、
	 AVR ISP 
おお、返答は帰ってくる。

### Arduino IDEからuploadを試みた。
- 確率は低いが、たまにアップロードがうまくいくことがある。
- しかし、プログラムが動いたのかどうか分からない。
    - AVRライターで読んで調べてみるとしよう。
- 調べてみた。
    - 何かを書き込んでいる形跡はあった。
    - が、AVRライターで読み出したHEXとは似ても似つかない。字化け？それとも消さずに書いてる？？？？
    - たぶん実行不可だろう。

<!-- dummy comment line for breaking list -->

つまり、今の状態を一言で表すならば、
- **とてつもなく不安定!**

<!-- dummy comment line for breaking list -->



~
~
~
~
- - - -
## とてつもなく不安定なArduinoを飼いならす。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/arduino14k1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/arduino14k1.jpg) 
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/arduino14k2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/arduino14k2.jpg) 

- 写真はPIC 18F14K50とATmega8の混載基板（Arduino機能互換をめざして奮闘中の奴）

<!-- dummy comment line for breaking list -->

~

- とりあえず、USB-PICの問題を切り離したいと思った。
- PIC 18F14K50をＩＣソケットから外す。
- シールド接続用に用意した２０ピンのピンヘッダー（メス）に電源とRx/Txを配線して、AVR-USB(V-USB)仕立てのUSB-シリアル変換シールドを接続できるようにしてみた。

<!-- dummy comment line for breaking list -->

ＵＳＢシリアル変換シールド（ATtiny2313）
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-c.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-c.jpg) 

- Arduino IDEに接続。
- LEDブリンクをビルドして焼く。
- 一応動いているようです。

<!-- dummy comment line for breaking list -->

~
### USB-シリアル変換シールド上でのATmega8の挙動ですが、どうも理解できない点がありました。
- Arduinoのブートローダー（hardware/bootloaders/atmega8/ATmegaBOOT.hex）をそのまま焼いていますが、
- こいつは起動直後にSCKのLEDを非常に短く(10回位)点灯したのち消灯します。
- 19200bpsで待ち受けます。
- 「1」「空白」という文字を受け取ったら、'AVR ISP'という返答を返します。（ブートローダーが生きている証拠。）

<!-- dummy comment line for breaking list -->

- 文字を送っても（送らなくても）１０秒くらいたつと、勝手にアプリを起動してしまいます。
- アプリが起動してしまうと、ブートローダーとして機能しないので、Arduino IDEからは制御できません。

<!-- dummy comment line for breaking list -->


こんなもんなんでしょうか？

あと、気になったのは、
- Arduino IDEのアップロードは成功してもメッセージを出さない（LED点灯は開始される）ことがあって、転送が終わったのかわかりにくい。

<!-- dummy comment line for breaking list -->

- 転送が終わってもプログラムがすぐに開始されるわけでなく、ブートローダーのタイムアウトを待っている（１０秒くらいたって起動する）という動作が非常に緩慢というか転送が遅いのと誤解される。

<!-- dummy comment line for breaking list -->

- 比較的サイズの大きいアプリ(Melody)は転送に失敗することが多い。
- いまだにMelodyが正常実行できていない。

<!-- dummy comment line for breaking list -->

- Melodyの出力ポートがpin9(Xtal)になっている（バグ？？？）

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## とてつもなく不安定なArduinoの謎
ブートローダーのソースを読んでみる。
- ATmegaBOOT.c

<!-- dummy comment line for breaking list -->

概要
- getch()は６秒間なにも文字が来なければ、勝手に0番地にジャンプ（アプリを実行）してしまう仕様になっていた。
- それ以外に、アプリを起動する方法が記述されていなかった。（app_start()を明示的に行うコマンドなど存在しない。）
- BOOTモードとアプリモードの切り替えジャンパーなどは存在しない。
- えーと、割り込みベクターを０番地側に戻すレジスタがあるはずなんだけど、ノータッチのようだ。
- 長いスケッチの書き込みが失敗する原因については不明。

<!-- dummy comment line for breaking list -->

なんかこのブートローダーはすごく投げやりなつくりになっている気がするのは気のせい？
- もっとも、avrdude.exeから見てこいつはライターに見えないといけないので、start()が出来ないのは仕様なのかもしれないけれど・・・。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## 他のArduinoではどうなっている？

[http://arduino.cc/en/uploads/Main/](http://arduino.cc/en/uploads/Main/) 

[http://arduino.cc/en/uploads/Main/Arduino-Diecimila-schematic.pdf](http://arduino.cc/en/uploads/Main/Arduino-Diecimila-schematic.pdf) 

[http://arduino.cc/en/uploads/Main/Arduino-Pro-Mini-schematic.pdf](http://arduino.cc/en/uploads/Main/Arduino-Pro-Mini-schematic.pdf) 

- USB式のやつは、ＤＴＲ信号でリセットを掛けることが出来るので、アプリを強制中断してスケッチの書き換えが可能。
- DTRを0.1uF経由でRESETに繋いでる。イタリア人は野蛮だ。RTS#は_NMなので実装しないらしい。

<!-- dummy comment line for breaking list -->

つまり、DTRコントロールが無いUSB-PIC18F経由の場合は、スケッチの書き込み直前にUSB再接続とか、手動リセットが必要なわけだね。

~
~
~
~
- - - -
## ATmegaBOOT_168.cを読んでみる。
- ATmega168ではブートジャンパーが設定されている。
    - PIND6だ。
    - ATnmega1280ではPINF7とPINF6らしい。
    - PIND6は設定されているが、ATmega168ではapp_start()コールはコメントアウトだった。

<!-- dummy comment line for breaking list -->

- ATmegaBOOT_168.cでは、ブートローダーのコマンドに合致しない文字が５回以上来たら、強制的にapp_start()するようにもなっている。
- おそらく、シリアル未接続のゴミ文字捨てなんだろう。

<!-- dummy comment line for breaking list -->

- また、'j'コマンドでも app_start()するので、少しはましか。

<!-- dummy comment line for breaking list -->

- 全体的にATmega8は放置プレイ中らしい・・・。

<!-- dummy comment line for breaking list -->

秋月に18F14K50を頼んだときについでに買っておいたATmega168があったなぁ・・・。
- ３００円なので、５００円Arduinoになってしまうけど。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## 仕組みが分かってしまえば、動かすのはカンタン。
- DTRによるリセットがPICでは不可能なので、
- Arduinoにuploadする直前(6秒の猶予しかない)にUSB接続してuploadすればブートローダーは受け付けてくれる。
- 一応LEDブリンクとFirmataのサンプルが書き込めて、なんとなく実行出来ているようだ。
- USB-シリアルはPIC 18F14K50に戻した。

<!-- dummy comment line for breaking list -->

- PICのファームは、とてもやる気ない実装だった。
- RS232Cの処理に割り込みやFIFOが一切使われていない。
- これだと取りこぼしが発生するのではないかと思ったが、受信FIFO(ハードウェア)は２バイト用意されているらしい。
- Arduinoの19200bpsならギリギリセーフかもしれない。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## Arduino-0017がリリースされました。[#t1d57fa3]
- [http://www.arduino.cc/](http://www.arduino.cc/) 
- 0017に入れ替えました。
- シリアルコンソールが少し変わって独立Windowになったようです。
- 手製のArduino互換機はリセットがちゃんと掛かるようになったので、とても安定に動作しています。
- シリアル関係のExampleをいろいろ試してみたのですが、115200bpsだとPICが反応しないようです。
- 57600bpsまでは一応動作しているようでした。

<!-- dummy comment line for breaking list -->

で、相変わらずスケッチ転送後の実行が６秒待たされます。
- ATmegaBOOT_168_ng.hexのタイムスタンプは変更されていますがHEXの内容は同一でした。
- ATmegaBOOT_168.cの内容も同一でした。

<!-- dummy comment line for breaking list -->



~
## 改良（案）

- ~~ATmega8からATmega168に換装する。~~ --- 済み
    - これによりブートローダーは'j'コマンドですぐにapp_start()するようになる。（？）
    - と、思ったが、改善しない。
    - ATmega_BOOT_168_ng.hex を焼いた。
    - ボード選択も168_NGを選択した。

<!-- dummy comment line for breaking list -->

- PIC18FのSerialEmulator（ファーム）を鍛えあげる。
    - せめて割り込み駆動に。
    - ~~最低でもDTR信号をどれかのｐｉｎに出せるように。~~
    - RB6端子にDTR信号を出すファームをコンパイルしてPIC側ブートローダーで書き込みました。

<!-- dummy comment line for breaking list -->

- ~~DTRリセットを配線する。~~(0.1uF接続というところが気になる。イタリア人は野蛮だ)
    - 配線しました。一応ArduinoIDEからリセットが掛かるようになりました。

<!-- dummy comment line for breaking list -->


- ボーレートをdefaultの19200から、57600とかもっと上にする。
    - これは実際のところ、Flashの書き込みWaitがあるのであんまり上げても速くはならない。

