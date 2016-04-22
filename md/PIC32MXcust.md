[PIC32MX](PIC32MX.md) 

**--*-- 勝手に改蔵 --*-- 勝手に改蔵 --*-- 勝手に改蔵 --*--**

## MicroChipのサンプルソースの動かし方・コツ

ここでは、MPLABやMicrohip Application Library (USB Framework)を**普通のgcc**と**普通のmake**を使ってコンパイルするコツを伝授します。

~

**その１）開発環境を準備する。**

- Pinguino、MPLAB v8.89、Microhip Application Library  の順にWindowsにインストールします。
- 準備できたら、[USBホスト](USB_HOST.md) のページにあるusbhost.zipを入手して展開し、ビルドが通ること

<!-- dummy comment line for breaking list -->
を確かめておきます。

~
**その２）MicroChipのサンプルソースを持ってくる。**

- usbhost.zipを展開したディレクトリに丸ごとソースを上書きします。
- HardwareProfile.hは上書きせずにusbhost.zipのものを置いておきます。（追記する必要はあるかもしれません）
- MPLABのプロジェクトファイル *.mcp を参考にしながら、Objs.mak を書き換えます。
- とりあえず、ここまで出来たらビルドしてみます。

<!-- dummy comment line for breaking list -->

~
**その３）MicroChipのpic32-gccと、Pinguinoのmips-gccの違い。**

Pinguinoのmips-gccは、

		#pragma が使えません。
    		#pragma config にてConfig Fuseを設定している部分については、 config.c に別の記述方法で書く必要があります。
    - しかし、Fuseの設定などはアプリごとにそんなに変わるものではありませんから、usbhost.zipのconfig.c をそのまま流用すれば大体ＯＫです。
    - Fuse以外のpragmaは使われていないようなので無視します。
    
    		#pragma で始まる行はmips32-gccでは丸無視しますので、わざわざ消さなくても大丈夫です。

<!-- dummy comment line for breaking list -->

~
- リンカースクリプトがやや違います。
    - これもアプリごとにそんなに変わるものではありませんから、usbhost.zipのリンカースクリプト をそのまま流用すれば大体ＯＫです。

<!-- dummy comment line for breaking list -->


~
- 割り込みベクターの記述が微妙に違います。
    - pic32-gccでは、関数 __attribute__ により、割り込み応答関数を割り込みベクターの番地に置くことが出来るようですが・・・
    - そもそも、割り込みベクターは1エントリー16byte程度しかありませんので関数実体をそこに置くのは無理があります。
    - ですので、mips-gccで書くときは割り込み応答関数には __attribute__（(interrupt,nomips16)）だけを与えて、

<!-- dummy comment line for breaking list -->
ROM上のどこにあっても良いようにしておきます。
    - 割り込みベクターから応答関数へのジャンプは、アセンブラで記述することにします。（現在はとりあえずcrt0.S内に書いています）

<!-- dummy comment line for breaking list -->

    - 参考ファイル:[vgagfx.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/vgagfx.zip)  の中にある、crt0.S を参考にしてください。
    - ~~BOOT-ROM領域からFLASH-ROM領域への(１命令) jmpは届きませんので、なんらかの工夫が必要です。~~
    - ~~($k0,$k1レジスタはコンパイラが使用しないことが一応保証されていますので、これを使用してセグメントを跨ぐ分岐を書きます。)~~
- 上記問題を回避するためは、

		_CP0_SET_EBASE(ebase_address);
- を実行して、割り込みベクターを（例えば）9D00_1000〜に置くか、BOOT_ROM上でもkseg0上(9FC0_0000〜)に置くようにすると、普通のjmp命令１個で分岐出来ます。
- （_CP0_SET_EBASEを呼ばない場合は、割り込みベースがBFC0_0000になっていると思いますので、$k0,$k1を使用して分岐するようなコードを書く必要があります。）

<!-- dummy comment line for breaking list -->
~
~
~


## Pinguinoのサンプル・スケッチをＣ言語の普通のソースツリーにして取り込む方法。

**その１）あらかじめPinguinoのGUIを起動して、スケッチをビルドしておく**
- C:\PinguinoX.3\source\ ディレクトリに、Ｃ言語に変換されたあとのスケッチが残りますので、これを拾ってきます。

		main32.c  &lt;=== メインルーチン
		user.c    &lt;=== main32.c がmain()関数の直前にインクルードする。
- そのままビルドツリーに突っ込んでもビルド出来ますが、（というかGUIはsource/ディレクトリで実際にビルドしているだけなので）
- 「ＣソースからＣソースのインクルード」が嫌いな人は、以下のようにします。

<!-- dummy comment line for breaking list -->

~

- C:\PinguinoX.3\include\pinguino\core\*.c の中で、main32.c がインクルードしているソースを個別に拾ってくる。

<!-- dummy comment line for breaking list -->

- main32.c でソースを全部インクルードするのはやめて、各ソースを個別にコンパイルするように改造する。

<!-- dummy comment line for breaking list -->

~

**その２）DIPマイコンPIC32MX220F032Bに対応させる**

- Pinguinoの現状の問題点として、MX220F032Bに完全には対応していないことがあります。
- Pinguinoの本家の基板はMX460とか795のような大規模Chipに対応しているようなので、MX220で動かそうとすると全然ピン数が足りなかったりとか、ピン割り当てが違ってるとか、ピンコントロールするsfrレジスタがそもそも無かったりとか微妙です。
- ですので、ビルドでエラーするところをMX220対応させるために手直しします。
- そして、実際に動かしてみると、いろいろチップの違いで動かないことがあったりピン割り当てが違ってたりするので、そこも手直ししていきます。

<!-- dummy comment line for breaking list -->

- やっと動くようになります。

<!-- dummy comment line for breaking list -->

~
- このチップ間移植問題は、Pinguinoのライブラリでも発生しますが、同様にMicroChipのサンプルソースでも発生します。
- コンパイラの変更や、ビルド環境の違いを吸収する作業よりも、チップの差異によるサンプルソース非互換問題を解決するほうがずっと手間が掛かります。

