[PIC日記](2009-05PIC.md) ← 　→[SDCCを飼い馴らす](PIC18F2550.md) 　 →[PIC18F4550](PIC18F4550.md) 　 [mcc18](mcc18.md) 


## SDCC(Small Device C Compiler)とは

一次情報源：

[http://sdcc.sourceforge.net/](http://sdcc.sourceforge.net/) 
- Intel8051((元祖8bitMCUとも呼べる。祖先はi8048であり、IBM-PCのキーボードコントローラーとして使われた。今ももしかしたらintelのチップセットの片隅に残っているのかもしれない))((FPGAのIPコアとしても存在していて、100MHzオーバーで動くものもある))((CypressのEzUSBチップは8051アーキテクチャを採用している))マイコン用に作られた**オープンソースのＣコンパイラ**です。
- 現在ではIntel8051以外に、Maxim 80DS390, Z80 , 68HC08 がサポートされていて、

<!-- dummy comment line for breaking list -->
PIC16F , PIC18F シリーズもサポートされつつあります。 
- 2008-03-22にリリースされているバージョン sdcc-2.9.0 は PIC 18F14K50チップのサポートがありません。

<!-- dummy comment line for breaking list -->

- 以下の最新スナップには、18F14K50が含まれているようです。

<!-- dummy comment line for breaking list -->

[http://sdcc.sourceforge.net/snap.php](http://sdcc.sourceforge.net/snap.php)  

- - - -
目次




- - - -
## SDCCによるＵＳＢアプリケーションのビルド

read more : [SDCCによるＵＳＢアプリケーション](sdcc-usb.md) 

~
~
~


- - - -
## 続:SDCCを飼い馴らす。

この記事に
**たいした期待をしないでください**



- まず、

		strcpy(char *t,char *s) {
		   while( *t++ = *s++ );
		}
- のような関数を書いて、sdccに掛けます。
- 出来たアセンブルリスト(*.LST) を３分間眺めます。

<!-- dummy comment line for breaking list -->

- _gptrset1,_gptrget1 に興味を持ちます。

<!-- dummy comment line for breaking list -->

- それらのソースはsdccをインストールしているなら以下にあります。

<!-- dummy comment line for breaking list -->

	C:/sdcc/lib/src/pic16/libsdcc/gptr/*.c
    - インストール先がc:/sdcc/でない場合は読み替えてください。

<!-- dummy comment line for breaking list -->

- far ポインタは３バイト長です。 AVRの場合と違い、ROM/RAMをMSBで判別します。
- EEPROMは未実装です。

<!-- dummy comment line for breaking list -->


はい、ここまでで気づきますね。Ｃ言語ではまともなパフォーマンスが出ません。

	C:/sdcc/lib/src/pic16/
- 上記場所にライブラリのソースがあります。一通りさらっと目を通しましょう。

<!-- dummy comment line for breaking list -->

では、本題です。

- 関数名を定義したら、 __naked にしましょう。
    - 注意: __nakedにした場合は return もasm内に記述しないといけません。
    - もちろん、関数内にラベルを書いて、rcall することさえ可能です。
- 関数呼び出しは rcall で行うと＋-1024バイトまでの距離は分岐できます。
- ２Ｋを超えるプログラムを書くときは、どうやら呼びたい関数が近くにあることを保証できないようです。rcallは（局所的な使用以外は）諦めましょう。
    - asm内のラベルは、後ろに ':'(コロン)を書いて識別する必要があります。
    - (MPASMでは行頭に書けば':'を省略できるようですが・・・)
- 関数の中身は __asm と __endasm で囲みましょう。

<!-- dummy comment line for breaking list -->

	void sub1(void) __naked
	{
	  __asm
	    処理を記述
	    ・・・
	  __endasm ;
	}

- 変数は '_' (アンダースコア) を先頭につける必要があるようです。
- 上記のライブラリソースの書き方を見て真似をすると良いです。

<!-- dummy comment line for breaking list -->

- 引数渡しなどでユーザースタックが欲しくなったら、FSR1 を使います。
- FSR2はsdccではスタックフレームポインタとして使われています。
    - 関数のエントリーで FSR2L = FSR1L のようなレベルあわせが行われます。
    - FSR1H = FSR2H = 0x01 固定のようです。

<!-- dummy comment line for breaking list -->

**それってまんまASMで書こうって言ってるのと同じ・・・**

~
~
~
## Ｃ言語記述を使ってもいい処理だってある。
- I/Oポートの制御は一応ビット命令になりますのでASM記述とたいして変わりません。
- 8bit変数の操作や16bitカウンタなどの操作に関してもASM記述とたいして変わりません。
- Ｃ記述(__nakedを付けない関数)の場合、仮想的なレジスタ（使用する分だけ）のPUSH/POPが関数の入り口と出口で発生します。また、スタックポインタをスタックフレームにコピーする処理や、引数をスタックフレームから参照して仮想的なレジスタ(r0x01とか。実際はメモリー)にコピーする処理が無駄に入ります。
- それらが許容できる程度の問題であれば、Ｃ記述を使うことが出来ます。
- パフォーマンスを上げたい部分だけ __naked という選択肢だってあります。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## SDCCにもの申す
- いや、そんな。滅相もありません。
- むしろ、これだけコンパクトに記述されたコンパイラなのに
- きちんと動いていて、PIC18Fのようなへぼいマイコンをサポートして

<!-- dummy comment line for breaking list -->
くれていることに感謝しないわけにはいきません。

が、どうすればさらに良くなるか考察してみましょう。

~
- 仮想的なレジスタ(０番地から最大0x60バイトまで確保されるr0xXXという名前のワーク)をセーブレジスタとテンポラリレジスタに分ける。
- むしろテンポラリレジスタを積極的に使い、スタックには保存しないようにする。

<!-- dummy comment line for breaking list -->

    - 割り込み処理ではテンポラリレジスタの退避復帰が義務付けられてしまう。
    - このへんは普通のレジスタの多いCPUと事情は同じ。

<!-- dummy comment line for breaking list -->

- FSR2（スタックフレーム）を使わないようにする。
- FSR1（スタックポインタ）だけで出来る場合はなるべくFSR1相対だけでやる。（やれるような気がする）
    - gccでもomit-frame-pointerという最適化ですでに実現されている。
    - FSR2の退避復帰がなくなるだけでも少し軽くなる。

<!-- dummy comment line for breaking list -->

- FSR1のPOPを適度にサボる。（これはgccの最適化がそのようになっていて、引数スタックを戻す処理は遅延実行でまとめてやるのだ。）

<!-- dummy comment line for breaking list -->

- callのrcall化をやって欲しいなぁ・・・。（届く場合のみ）ただしこれはlinkerの仕事になる。(relax link)

<!-- dummy comment line for breaking list -->


~

