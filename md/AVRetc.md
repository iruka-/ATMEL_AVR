*ＡＶＲにまつわるエトセトラ [#g9b67197]
- - - -

- - - -
#### ＡＶＲって何だ？
ＡＶＲという**マイクロコントローラ**に関する説明。~
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/attiny2313.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/attiny2313.jpg) 

当Ｗｉｋｉでは主に**ＡＴｔｉｎｙ２３１３**という品種をネタとして扱います。
- - - -
- ８ビットＣＰＵ
- ＲＩＳＣアーキテクチャー
    - 多くの命令を１クロックで実行する。
    - 固定語長（１６ビット／命令）（一部の命令に例外有り）
    - ３２本の８ビットレジスタを持つ。
    - ＭＩＰＳのような遅延分岐や遅延ロードは無いので、アセンブラでも組みやすい。
- 一次情報源~

<!-- dummy comment line for breaking list -->
[http://www.atmel.com/](http://www.atmel.com/)  ~
[http://www.atmel.com/jp/products/avr/](http://www.atmel.com/jp/products/avr/) 
- ＥＬＭ　ＣｈａＮさんの解説~

<!-- dummy comment line for breaking list -->
[http://elm-chan.org/docs/avr.html](http://elm-chan.org/docs/avr.html)  ~

- - - -

#### Ｚ８０は知っている。ＡＶＲはどこがどう違う？
どこが良い？
- 圧倒的にSIZE(24){速い}
- Ｚ８０は可変語長（１〜４バイト／命令）
- Ｚ８０は１命令を４〜２０クロック掛けて実行する。
- Ｚ８０Ａは当時４ＭＨｚ動作。
- ＡＶＲは最高２０ＭＨｚ動作（５Ｖ）
- Ｚ８０Ａの平均ＣＰＩ（１命令実行に必要クロック数）＝６と仮定すると~

<!-- dummy comment line for breaking list -->
ＡＶＲの２０ＭＨｚはＺ８０ＡのSIZE(24){１２０ＭＨｚに相当}する。

どこが劣る？
- 汎用マイコンではない。
- 拡張性がない。
- アドレスバス、データバスが出ていない
    - ＡＶＲはマイクロコントローラなので、チップ内でほぼすべてが完結している。
    - 外部バスはない（一部品種除く）
    - ＡＴｔｉｎｙ２３１３に限って言えば、
        - 命令用フラッシュＲＯＭ２Ｋバイト（１Ｋステップまで）
        - データＳＲＡＭ１２８バイト
        - ＥＥＰＲＯＭ１２８バイト
        - 最大１８本のＩ/Ｏポート
- 命令書き換えとかいわゆる実行ファイルの読み込みという概念がない。
    - 代わりに、フラッシュＲＯＭに書き込んでおけば消えない。

<!-- dummy comment line for breaking list -->


- - - -
#### ＭｉｃｒｏＣｈｉｐのＰＩＣ　１６Ｆ８４Ａは知っている。ではＡＶＲの良いところは？
- ３２本の汎用レジスタを持つ。
- 命令がＲＩＳＣ風で、命令直交性がありプログラムしやすい。
- なんと言っても**ＧＣＣ**が使える。~

<!-- dummy comment line for breaking list -->
SIZE(24){全部アセンブラで書かなくていいんだ}
- ＰＩＣは１命令あたり４クロックで実行するので、普通に考えて**４倍速い**。
- しかしレジスタ数が多いのでその分も考慮するともっと速いコードが書ける。
- とっても**安い**
    - 秋月の通販でたったのSIZE(24){１２０円}だ！

<!-- dummy comment line for breaking list -->

- - - -
#### いや、ＣＰＵってＰｅｎｔｉｕｍとかアスロンくらいしか知らんのだが・・・
- Ｐｅｎｔｉｕｍを半田付け手配線してシステム作れますか？
- 出来合いのマザーボードにＰｅｎｔｉｕｍなんとかを挿したとしましょう。
- [ＢＩＯＳ書けますか](http://www.linuxlabs.com/linuxbios.shtml) ？

<!-- dummy comment line for breaking list -->
[ブートローダー書けますか](http://www.tsden.org/takamiti/extipl/index.html) ？
[ＯＳ書きますか](http://wiki.osdev.info/?projects) ？~
いやそんなもん普通書かんよ。~
[ゲイツＯＳ](http://ja.wikipedia.org/wiki/Microsoft_Windows_XP) 買ってくるだろ普通。

つまり、こういうことだ。
- ＣＰＵを理解して、プログラムしたいなら、
- いきなりＰｅｎｔｉｕｍのブートコードなんか書かずに
- もっとシンプルなＰＩＣとかＡＶＲがいいんじゃねー？

<!-- dummy comment line for breaking list -->

Ｚ８０でもいいけどね。もう終わっとるんよ。２０世紀にね。
- Ｗｉｎｄｏｗｓ上だけのプログラムなんかではＣＰＵの理解には到達できんと思う。
- 便利ではあるが、ＡＰＩの内側は全部ブラックボックスだ。ＣでゴリゴリやってもＥｘｃｅｌのＶＢＡ書いているのと本質的に変わらん。

<!-- dummy comment line for breaking list -->

- - - -
#### いったい、そんな高速でちっぽけなＣＰＵを何に使うのだ？
- 使い道はあなた次第だが
- とりあえず、~~ＡＶＲ-ＵＳＢ~~ （ＶＵＳＢに改名されました）で遊ぶなんていうのはどうだろう。
- そのまえに、書き込み器（ＡＶＲライタ）を作る必要がある。
- ＡＶＲライタ~

<!-- dummy comment line for breaking list -->
[http://elm-chan.org/works/avrx/report.html](http://elm-chan.org/works/avrx/report.html) 

#### [ＨＩＤａｓｐ](ＨＩＤａｓｐ.md) を作ってみる

- - - -
## ATtiny2313の遊び方指南

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/attiny2313.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/attiny2313.jpg) 

- （１）とりあえず[ＨＩＤａｓｐｘライター](ＨＩＤａｓｐｘ.md) を作ります。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/wsn216.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/wsn216.jpg)    ((画像は [wsnak](http://www.wsnak.com/)  様が販売している [WSN216基板](http://www.wsnak.com/kit/216/index.htm)  　))


- （２）[v-usb](http://www.obdev.at/products/vusb/index.html) のsample applicationやcommunity projectを参考に何かUSBデバイスを作ってみることが出来ます。

<!-- dummy comment line for breaking list -->

![http://www.obdev.at/Images/vusb/vusb-teaser.png](http://www.obdev.at/Images/vusb/vusb-teaser.png) 

[http://www.obdev.at/products/vusb/projects.html](http://www.obdev.at/products/vusb/projects.html) 


- （３）[KeyBoardマニア](KeyBoardマニア.md) を参考に、電子楽器を作ってみましょう。

<!-- dummy comment line for breaking list -->


- （４）[デジタルテスター](https://github.com/iruka-/ATMEL_AVR/blob/master/web/log/2009-03.html)  [HIDtester](HIDtester.md) を参考に、電圧計、抵抗計などを作ることが出来ます。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/rc-meter.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/rc-meter.jpg) 

- （５）[PICライター](PICspx.md)  や [ARMライター](hid_blaster.md) を作ることも出来ます。

<!-- dummy comment line for breaking list -->



わずか1024ステップ、RAM128バイトしかないATtiny2313だけでこれだけのものを作ることができます。




~
~
~
~
- - - -
## その他制作物一覧



~
~
~

- - - -
![screenshot](https://github.com/iruka-/ATMEL_AVR/blob/master/counter/dream.cgi?id=FrontPage&name=a.gif)

