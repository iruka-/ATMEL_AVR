[2011-12](2011-12.md) 


![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/RX62N/rx62n.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/RX62N/rx62n.jpg) 

- 今頃になって2011-05付録基板を開封す。

<!-- dummy comment line for breaking list -->

これ、最初にどうすりゃいいの？

- - - -

- （１）まず、BOOT-MD0 (JPP1)にピンヘッダーを立てる。
- （２）こいつ(JPP1)をショートした状態で、USBを接続する。
- （３）Renesasのサイトに登録して、FDT（フラッシュ書き込みツール）をＤＬしてインストールする。
- （４）FDT4.08 basicというソフトを立ち上げて、基板はGeneric USB boot(一番下)を選び、
    - クロック12MHz、逓倍８倍、周辺４倍にて接続する。
    - 書き込みたいmotファイルをダウンロードにより基板に書き込む。

<!-- dummy comment line for breaking list -->

- （５）JPP1をオープンに戻して、再度USBを繋げばユーザーアプリが起動する。

<!-- dummy comment line for breaking list -->

- - - -
ちなみに（２）で使うinfはFDTのインストール先に生成されているもの。~

~
~
~
- - - -
## gccでビルド可能な、USB-CDCエコーバックサンプルソース

<!-- dummy comment line for breaking list -->
ダウンロード：
- [USB-CDCを利用した簡易モニター](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/RENESAS/rx62n.zip) 
    - CDCですが、パケット式通信なので、host側ツールはcdctool.exeを使用します。
    - コンパイル環境は、特電さんが配布しているcygwin版 rx-elf-gcc
    - もしくは、Linux環境で普通に作成されたrx-elfなクロスgccを使用可。

<!-- dummy comment line for breaking list -->

- [USB-CDCエコーバックサンプルソース](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/RENESAS/rx62n-cdc.zip) 
    - ソースはやや古いです。
    - エコーバック速度のベンチマークソフト同梱
    - コンパイル環境は、特電さんが配布しているcygwin版 rx-elf-gcc
    - もしくは、Linux環境で普通に作成されたrx-elfなクロスgccを使用可。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## 今後の予定
- とりあえず[cdcboot](libmapleでブートローダー.md) 相当のものを移植して、HIDaspxとかPICspxを動かしてみる。

<!-- dummy comment line for breaking list -->

~
- - - -
## gccの入手
- [クミンズKPIT](http://www.kpitgnutools.com/index-ja.php) のサイトに登録して入手するか、特電の[サイト](http://www.tokudenkairo.co.jp/rx62n/download.html) のCygwin版を入手するか、
- あるいは自分でビルド(下記参考URLを参照のこと)する。

<!-- dummy comment line for breaking list -->

KPITから落としてきたGNURXとHEWは、まずHEWからインストールする。すると自動で後続のGNURX(gcc)もインストーラーが起動する。

~
いつものこまったちゃん。
- ARMやPICのときにもあったけれど、
- IARとかKEILのような商用コンパイラ用のソースをgccに掛けるといろいろ問題発生する。
    - ARMのときはコンパイラはgccのくせに、アセンブラ文法の非互換やLDスクリプト、セクション名非互換で悩まされた。makefileも無かったし。
    - RX62Nの場合、根本的にコンパイラが違うので、もっと大変。

<!-- dummy comment line for breaking list -->

~
- - - -
Renesasモニターはあるけど、たぶん使わない（使えない）
- なぜならば、RAM上のコードしかデバッグできないのと、
- USBデバイス開発時にUSBでデバッグなんか出来るわけ無いじゃん。
- もちろん実シリアルポートモードでRenesasモニタという手も無いではないが、普通シリアルprintfで充分。

<!-- dummy comment line for breaking list -->

- ユーザーの書き込むファームで書きつぶされるので（毎回FDTで書き込んで切り替える）面倒なことこのうえない。

<!-- dummy comment line for breaking list -->


- まさかFlash書き込み100回までだからRAM上で開発しろとかいうオチはないよなあ。（Ｈ８がもろにそれだった。）

<!-- dummy comment line for breaking list -->

~

## 参考URL

Build cross-gcc for Renesas RX
- [http://bravo-fpga.blogspot.com/2011/08/build-cross-gcc-for-renesas-rx.html](http://bravo-fpga.blogspot.com/2011/08/build-cross-gcc-for-renesas-rx.html) 

<!-- dummy comment line for breaking list -->

RX62NのUSB Boot mode 
- [http://bravo-fpga.blogspot.com/2011/08/rx62nusb-boot-mode.html](http://bravo-fpga.blogspot.com/2011/08/rx62nusb-boot-mode.html) 
- [http://bravo-fpga.blogspot.com/2011/08/plan-b.html](http://bravo-fpga.blogspot.com/2011/08/plan-b.html) 
- [http://bravo-fpga.blogspot.com/2011/08/rx62nusb-boot-mode-2.html](http://bravo-fpga.blogspot.com/2011/08/rx62nusb-boot-mode-2.html) 

<!-- dummy comment line for breaking list -->

特電　RX-ELF-GCC（Cygwin版)
	http://www.tokudenkairo.co.jp/rx62n/download.html

