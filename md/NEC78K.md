[2008-07](2008-07.md) 

## ＮＥＣ７８Ｋ用　逆アセンブラ
かなり制作途上です。
- [dis78k-2010-0118.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/dis78k-2010-0118.zip) 
- 入力ファイルは、トラ技ＢＩＯＳのダンプ出力もしくはインテルＨＥＸ形式のテキストとなります。
- まだ、実デバイスとの接続は実装していません。

<!-- dummy comment line for breaking list -->

- ソース付き。
- MinGW-gccまたはLinux標準のgccでビルドします。

<!-- dummy comment line for breaking list -->


Ｔｏｄｏ：
- ~~0x00〜0x7fのベクターのdw表示~~
- ~~callt の判定ミス~~
- ~~0x1234形式か1234H形式の選択可能~~
- Saddr , Sfr の正しい表記
- クロック数

<!-- dummy comment line for breaking list -->



COLOR(#ddeeff){ここにあるＷ３２＿ｔｅｒｍを改造して、トラ技ＢＩＯＳと接続するモニタを書き中}~


~
~
~
しかしあれだな、２５６ｋＢの広大なメモリーと６０ＭＨｚのＲＩＳＣ　ＣＰＵよりも、~
わずか８Ｋのバイト単位のちまちました非力ＣＰＵのほうが俄然やる気が出るのは何故だろう。~
やっぱり、最初に触ったＳＣ/ＭＰ][（１２８バイトＲＡＭ）に近いからかな？~
~
しかもＣ言語なんか使わずにＨＥＸ入力モニターを書いてハンドアセンブル（というよりほとんど命令コード暗記に近い）萌え〜だったりするから始末が悪い。（というかＺ８０はほとんどそのスタイルで書くのが当たり前だ）~
構造化アセンブラとか書いちゃうぞ！



- - - -
## 関連リンク
なひたふＪＴＡＧ日記
- 78KマイコンUSBプログラマ
    - [http://nahitafu.cocolog-nifty.com/nahitafu/2008/07/78kusb_cbf0.html](http://nahitafu.cocolog-nifty.com/nahitafu/2008/07/78kusb_cbf0.html) 
- トラ技78K基板でUSB-JTAGを作ってみた
    - [http://nahitafu.cocolog-nifty.com/nahitafu/2008/07/78kusbjtag_45ee.html](http://nahitafu.cocolog-nifty.com/nahitafu/2008/07/78kusbjtag_45ee.html) 
- 〜トラ技付録 78K0基板 私的サポートページ〜
    - [http://www.nahitech.com/nahitafu/trg78k/index.html](http://www.nahitech.com/nahitafu/trg78k/index.html) 

<!-- dummy comment line for breaking list -->



でんし研
- 78K0基板を使ったAVRプログラマ「AVR-ISPライター78K0版」
    - [http://homepage2.nifty.com/denshiken/AVW020.html](http://homepage2.nifty.com/denshiken/AVW020.html) 

<!-- dummy comment line for breaking list -->

なんでも作っちゃう、かも。
- USBマイコン基板で作るPICプログラマ
    - [http://arms22.blog91.fc2.com/blog-entry-168.html](http://arms22.blog91.fc2.com/blog-entry-168.html) 

<!-- dummy comment line for breaking list -->

- - - -

## トラ技７８Ｋ基板は各種ライターの素になる運命かな？
- ７８ＫのＡＶＲライターは、レガシーライタを先に作らなくても焼けるので、鶏/卵問題が解決できてＧｏｏｄですね。
- なひたふさんの７８Ｋで７８Ｋを焼くライターは基板が都合２枚要ります。

<!-- dummy comment line for breaking list -->

- - - -
![screenshot](https://github.com/iruka-/ATMEL_AVR/blob/master/counter/dream.cgi?id=NEC78K&name=a.gif)

