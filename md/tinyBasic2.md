[PIC32MX](PIC32MX.md) 

## PIC32MX上でtinyBasic2インタプリタを動かそう

[PIC32MXでVGA出力](pic32vga.md) の続き
- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/basic1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/basic1.jpg) 

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## The Tiny Basic2
元ネタ： 1990-1-1
- [http://www.programmersheaven.com/download/16060/Download.aspx](http://www.programmersheaven.com/download/16060/Download.aspx) 

<!-- dummy comment line for breaking list -->

Raspberry PiにTinyBasicが移植された。
- [http://www.staff.city.ac.uk/afl/tinybasic/downloads.html](http://www.staff.city.ac.uk/afl/tinybasic/downloads.html) 

<!-- dummy comment line for breaking list -->

これをPIC32MXで動かしてみました。
- Raspberry Pi版は、コードサイズで64kB程ありますので、Flash容量32kBのPIC上で動かすのは無理でした。
- が、その元ネタになっているほうのBASINT.cは14kB程度（Ｃソース、exeファイルともに）ですので、32kBのFlashに入ります。

<!-- dummy comment line for breaking list -->

~
- 実際に動かしてみると、BASIC INTERPRETER自体の占有ファームウェアサイズはmips16で4kB程度、MIPS32でコンパイルしても8kB程度に収まっています。
- putch(int c)相当の関数さえあれば動作しますので、PIC32MXに限らず他のマイコンへの移植も簡単です。

<!-- dummy comment line for breaking list -->


- - - -
## ダウンロード

ファームウェアソース：[mxvga.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/mxvga.zip) 

~
ライセンス
- basintのほうはオリジナルサイトを見てもライセンスはunknownとなっています。
- tinyBasic 2のほうはGPL v2でした。
- 投げ遣りですが、この版も、GPL v2で宜しいのではないかと考えております。

<!-- dummy comment line for breaking list -->


~
ソースコードをぱっと見たところでは、
- 手書きのパーサで、あんまり宜しくない。
- 処理もかなり不備がある

<!-- dummy comment line for breaking list -->

ように見受けられますので、たぶん、読者の貴方が１から書き直したほうがずっと良いBASICが作れると
思います。

- test.bat を起動すると、MinGWのgccを使って、Win32コマンドライン版の実行を行います。
- PIC32MXへの移植では、printf,scanf(現状はダミー)が必要になった程度で、ほぼ手直しなしで動いてしまいました。
- BASICソースはＣ言語中に文字列で格納してありますが、改行コードが
- オリジナル版では CR+LFのところを LFだけでも動作するように改造するのに結構手間取りました。（その必要性は薄かったのですが）

<!-- dummy comment line for breaking list -->

~
- また、tinyBasic2の素数プログラムを走らせようとすると、basintのBASIC文法実装が足りていなかったので文法エラーで実行できませんでした。
- そこで、いくつかのステートメント処理はtinyBasic2からバックポートさせて頂きました。

<!-- dummy comment line for breaking list -->

~
~
~

- - - -
## 課題
- ~~BASIC動作中は画面にジッターが出ます。~~ --- COLOR(RED){ジッターは対策しました。}
    - 終了するとジッターは消えます。
- ~~原因は究明中ですが、よくわかりません。~~ --- 実はよくわかっていません。割り込み応答が２〜３クロック後、というマニュアルの記述が気になるところです。遅延スロット？それともmips16/MISP32遷移？
    - MIPS32ビルドは試しましたが、症状は変わりませんでした。
- div,mul命令を使わないようにしてみたのですが、直りませんでした。
    - 乗除命令を使わずに32bit div / mul を書きました。今は使っていません。

<!-- dummy comment line for breaking list -->


~
答えは、2クロック命令だそうです。(kats_me様、ありがとうございます)
- RISCなのに2クロックって、知りませんでした。単なるストールとは違うのかな？

<!-- dummy comment line for breaking list -->


~
- 単体BASICコンピュータとして完結させるためには、キーボード入力部分(PS/2かな？)やセーブロード部分(SDカードかな？)

<!-- dummy comment line for breaking list -->
などの増設の必要性がありそうです。
~
~
~

- - - -
## 応用
- 単体BASICコンピュータ
- basint.c のみを再利用して、別のマイコン(ARMとか)へ移植。
- basint.c を拡張。というかFlash容量さえあれば、tinyBasic2をそのまま利用でもOK.
- RS-232C TTY端末。例えば、Linuxのコンソールに使う。(玄箱のttyコンソールとか)
- basic以外の言語を動かす（ForthとかLuaとか）

<!-- dummy comment line for breaking list -->

~
- CG-ROMを適当に拡張して、SD-Card上の漢字dotを使用して漢字混じり表示。
    - １画面に出せる漢字の種類(個数)に上限を設ければ可能。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## USB経由のBASICコンピュータを目指す
- KeyBoardやsdcardを繋げるのが面倒なので、とりあえずUSB-CDCデバイスまで取り込んで、
- WindowsPCからBASICが使えるようにしようという試みです。

<!-- dummy comment line for breaking list -->


## （仮）ダウンロード

USB入りファームウェアソース：[cdcbasic.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/cdcbasic.zip) 

- 現状のものは、USB-CDCデバイスとbasicインタプリタを同居させてはいますが、
- basicインタプリタはまだ対話型に拡張されておりません。
- とりあえずprime_vanilla.cを実行したあとは、Teratermなどから、VGA画面にエコーバックのみ行なえます。

<!-- dummy comment line for breaking list -->

~
~
~

