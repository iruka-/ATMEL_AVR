[2009-06](2009-06.md) 

- - - -
目次



## 今作っているのは、**tiny2313のArduinoっぽいど**。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-a.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-a.jpg) 


- 例によってユニバーサルエリア全開。
- とりあえず、HIDaspxライターでヒューズとサンプルＨＥＸの書き込みには成功。
- ISP端子は基本６ｐｉｎなんだけれど、ケーブルが互換機のシリアルポート外だしケーブル(９ｐｉｎ）を切って使う都合上、１０ｐｉｎ用意している。
- 余った４本は時と場合によって、RxDとかTxDとかを勝手に追加して使っている。

<!-- dummy comment line for breaking list -->

~
- - - -
## Arduino2313っぽいブートローダー：DOWNLOAD


ソース,HEXファイル一式: [https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/tinyloader.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/tinyloader.zip) 

- ブートローダーとして一応使えます。

<!-- dummy comment line for breaking list -->


![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/wsn216.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/wsn216.jpg)    ((画像は [wsnak](http://www.wsnak.com/)  様が販売している [WSN216基板](http://www.wsnak.com/kit/216/index.htm)  　))

- **ハードウェアにHIDaspxをそのまま流用出来ます。**
    - USB-IOモードのジャンパーがそのまま BOOTMODE JUMPERになります。
    - READY,BUSY のLED を使ってアプリ起動試験などを行います。
    - 12MHz(何MHzであっても)の水晶は実装したままで構いませんが、
    - 常に内蔵ＲＣ発振（８ＭＨｚ）にて使用します。（仕様変更はもちろん可能です）
    - HIDaspxのＵＳＢは使用しません。（新たに自作される場合は配線不要です）

<!-- dummy comment line for breaking list -->

~
~
~

- - - -

裏側配線.

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-ura.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-ura.jpg) 

- シールド接続用のピンヘッダーは未配線

<!-- dummy comment line for breaking list -->

- - - -

ＵＳＢシリアル変換シールド（これもATtiny2313）

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-c.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-c.jpg) 

- - - -

ＵＳＢシリアル変換シールドを合体したところ。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-g.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-g.jpg) 


- - - -
実は上のＵＳＢシリアル変換シールドはＨ８用だったものを流用。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-b.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/tn2313-b.jpg) 


- - - -
## 開発日記


- とりあえず、ファームを書き始めました。
- ３２０バイト程度。残りは１７２０バイト程度。
- もしかしたらＡｔｍｅｌのアプリケーションノートに、tn2313（というかシリアル汎用の）のブートローダーはあるのかもしれません。

<!-- dummy comment line for breaking list -->

- でも、せっかく作り始めたので、続けることにします。

<!-- dummy comment line for breaking list -->

~
~

現在ブートコードは**２８６バイト（０ｘ１１Ｅ）**

**２５６以下にするのが夢**　無理かな。

- 動作試験はまだです。

<!-- dummy comment line for breaking list -->

- 19200bpsによるシリアルのエコーバック試験は完了。

<!-- dummy comment line for breaking list -->

- 仮組みしたコードを動かして、ファーム内容の読み込みを実施してみた。
    - 文字落ちしているようだ。
    - 原因はおそらくusbRS232（V-USB使用）側にありそう。デバッグ中

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## ホスト側ツールを作成
- とりあえずファームの読み出しまで実装。
- ~~書き込みは未実装。~~
    - 書き込みを実装。動作確認済み。
    - ＬＥＤチカチカのテストを書き、そのＨＥＸを書き込み実行。
    - ＬＥＤチカチカするところまでたどり着いた。
- しかし、アプリを１度書き込んでしまうと、２度とブートモードになれないのであった。
    - （理由はリセットベクターも含めて書き換え実行されたため。）

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## コードサイズを削減
- ３２０バイト-->２９０バイト
- グローバルな構造体をバラして、全部レジスタ変数化した。
- WDTによるリブート（は無駄なので）やめた。

<!-- dummy comment line for breaking list -->



~
~
~
~
- - - -
## 今後の予定

ホストＰＣ側：

- ~~ブート領域（0x06c0〜0x07ff）の保護処理~~ --済み
- ~~ブート領域（0x06c0〜0x07ff）からアプリ側へのjmpアドレス書き込み（ホストＰＣツール側で。）~~--済み
- ~~リセットベクターのみ保護して、アプリ領域(0x0002〜0x06bf)の書き込みの実装。~~ --済み
- ~~リブートコマンド実装~~--済み

<!-- dummy comment line for breaking list -->


ファーム側：
- ~~起動時ジャンパーＳＷチェックによるアプリ側へのjmp処理の追加。~~--済み。
- ~~WatchDog機能によるチップのリブート実装~~--現状はrjmp命令で実装。WDTではジャンパーを切り替えない限り無限リセットになる。

<!-- dummy comment line for breaking list -->

余力があれば：
- 起動時RxD信号レベルチェックか、もしくは
- 起動時一定時間内にRxD特定パタン受信によるブートローダーモードへの移行。

<!-- dummy comment line for breaking list -->


    - これらは、コードサイズ的に**無理。**

<!-- dummy comment line for breaking list -->






- Arduinoのパッチ
    - avrdude_wrapper.exe
    - boot2313.exe
    - パッチインストーラーなど。

<!-- dummy comment line for breaking list -->

~~だいぶ先は長い。~~ --- これからやります。

- Arduinoの2313サポート可否判定については、[2009-06](2009-06.md) に書きました。
- 結論は**「可能」**です。
- あと、avrdude.exe をexeラッパーに差し替えて自作ローダーを呼び出す方法もほぼ分かっています。
- しかし、出来ることが解ってしまうと急速に興味が失われていくのでした・・・。

<!-- dummy comment line for breaking list -->

なんといってもwiring言語の
	digitalWrite()
の実装とか、全般的にライブラリの実装がタコであることが分かってしまい、

**だったら、素のavr-gccとか、素のインラインアセンブラで今までどおりに作ってるほうがいいじゃん**
- という結論に達しました。

<!-- dummy comment line for breaking list -->

- しかし、ハードウェアの標準化とプロトタイピングの速さという点でArduinoは充分評価出来るものです。

<!-- dummy comment line for breaking list -->

~
~

- 例のJavaなGUIのままで、上記の目的「**素のavr-gccとか、素のインラインアセンブラで今までどおりに作る**」

<!-- dummy comment line for breaking list -->
こともあながち不可能ではない（つまり、**ライブラリを使わないとポートを上げ下げしてはいけないルール**は存在しない）ので、気が向いたときにぼちぼちコードをいじるかもしれません。


~
~
~
~
- - - -
## その後のArduino2313情報

・お得な情報

- - - -

Arduino統合環境の日本語化 0017用：スイッチサイエンスさん
- [http://www.switch-science.com/trac/wiki/Arduino-ja-jp](http://www.switch-science.com/trac/wiki/Arduino-ja-jp) 

<!-- dummy comment line for breaking list -->

~

senshuさんが、[Avrdudeのかわりにhidspxが使えるように](http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2Fnews46) してくれているので、

- [http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2Fnews46](http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2Fnews46) 

<!-- dummy comment line for breaking list -->
のほうを参照してみてください。


It is now possible to use ATtiny2313 microcontrollers with the modified Arduino IDE. 
- [http://hci.rwth-aachen.de/luminet](http://hci.rwth-aachen.de/luminet) 

<!-- dummy comment line for breaking list -->

- 上記サイトからDL可能 **attiny2313_core.zip** があなたの欲しているコア・ライブラリです。

<!-- dummy comment line for breaking list -->


~

結局、シリアル経由のブートローダーを書くよりは、ArduinoIDEからHIDaspx（AVRライター）経由
でスケッチを書き込むほうが使い勝手も良いし、Flashの全領域が使えます。
- このやりかたでのArduinoはATmega88のようなFlash容量の少ないマイコンにも応用が利きます。（8kBの全領域をスケッチに使えます）

<!-- dummy comment line for breaking list -->

## [dude-wrapper](http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2Fnews46) のメリット
- シリアル(FT232RL)が不要。
- これによりArduino基板は非常に簡素化される。（純正Arduinoを買ったり、ファーム焼き込み済チップを購入する必要が無い）
- というよりはISP端子を用意したAVR組み込み回路の全てにArduinoの開発環境を持ち込めるという美味しい話。

<!-- dummy comment line for breaking list -->

- ブートローダーは全く使用しないので、Arduinoスケッチからそのまま製品移行できる。ローダー起動ラグも無い。
- Flash領域を全てスケッチ側で使い切ることが出来る。
    - mega88なら8kBフルに使える。tiny2313は狭いながら2kB全部スケッチに使える。

<!-- dummy comment line for breaking list -->

- AVRのTx/Rx端子はＰＣとの通信が不要になるので他の目的に使用できる。
    - FT232の実装も不要。

<!-- dummy comment line for breaking list -->

デメリット
- ISP端子(MISO,MOSI,SCLK)を組み込み基板上の内部接続と競合しないように設計する必要がある。
    - （通電したままISP端子が使えること。）

<!-- dummy comment line for breaking list -->

- シリアル経由のprintfデバッグが出来ない。firmattaライブラリも使えない。ＰＣ側との連携アプリもちょっと無理。
- （SPI接続を使ったprintfは可能。瓶詰堂さんが公開されています）

