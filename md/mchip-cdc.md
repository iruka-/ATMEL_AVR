[mcc18](mcc18.md) 　[PIC18F2550](PIC18F2550.md) 　[PIC18F4550](PIC18F4550.md) 　[HIDmon-2550](HIDmon-2550.md) 　[HIDmon-14K50](HIDmon-14K50.md) 　[sdcc](sdcc.md) 

## USB-CDCシリアル変換ファームウェアの改良

- mcc18のインストール方法については[こちら](mcc18.md) を参照してください。

<!-- dummy comment line for breaking list -->

- [mcc18](mcc18.md) にあるmchip-cdc14k2のアーカイブを少し変更して、18F2550/18F4550にも対応させてみました。

<!-- dummy comment line for breaking list -->

**ダウンロード**
- [mchip-cdc.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/mchip-cdc.zip) 

<!-- dummy comment line for breaking list -->

~
ついでに、Linux環境でビルド出来るようにもしてあります。
- ただし、ビルドするためにはwine（とmcc18）を入れる必要があります。

		# apt-get install wine
- 動作確認はubuntu-9.10を使用しました。
- [mcc18](2009-09#v0d65843.md) はwindowsにインストールしたものからコピーする必要があります。

<!-- dummy comment line for breaking list -->



~
逆アセンブルリストを見やすくしてみました。
	$ make dumplst
すると、cdc-18F2550.src という逆アセンブルリストが作れます。



~
Todo:
- 割り込み受信したい。
- ボーレート上げたい。
    - Ｃソース中にアセンブラを書くには？
    - 割り込み処理をアセンブラで書くには？

<!-- dummy comment line for breaking list -->

## Ｃソース中にアセンブラを書くには？
答え:
	_asm  と  _endasm でアセンブラ記述を挟む。
１行に書いてもいいし、
	_asm
	  アセンブラ記述
	  ・・・
	_endasm
としても良い。
- _endasm の後ろにセミコロン不要。
- アセンブラ記述にも行末セミコロン不要。
- 引数無しのvoidな関数内にいきなり _asm と書き始めるもＯＫ。
    - Ｃ言語的にはプロローグコード、エピローグコードは無いので気にしなくて良い。
- return 命令はＣ言語側が挿入するので _endasmの手前のreturn は不要。

<!-- dummy comment line for breaking list -->

### 書き方の見本は？
- mcc18をインストールしたディレクトリにある。

		C:/mcc18/src/traditional/*/*.c
- **ここは見ておけ。 [#w4ef603c]
- asmでのマクロの書き方。

		C:\mcc18\src\traditional\stdclib\p18macro.inc
- strcpyのソース。

		C:\mcc18\src\traditional\stdclib\strcpy.asm
- mcc18の引数渡しは、FSR1を引数スタックポインタとして使用する。
- つまり、引数リストを後ろから順にFSR1にpushして、関数をcallする。
- char引数はshortに引き伸ばさずに、byte単位でスタックに積む。
    - すなわち関数を呼ぶ度にchar引数の符号拡張を入れなくて済む。

<!-- dummy comment line for breaking list -->

~

- 関数内で使用する（破壊する）レジスタの退避もFSR1をスタックにして使う。
- なので、関数内で引数を取り出したい場合のFSR1相対オフセット値は、退避サイズを加算して決めている。
- FSR1のpopは呼び出し側の責任。
- 戻り値はPRODL,PRODH（乗算結果レジスタ）に返すようになっている。(WREGは使わない)
- アセンブラ記述内から引数を取得するマクロはこんな感じ。

		Stk1CpyToReg macro Offset,Reg  &lt;=== 1byte取得。
		Stk2CpyToReg macro Offset,Reg  &lt;=== 2byte取得。
- Offsetが[SP+Offset]のFSR1相対オフセット。
- Regは取り出した引数を保存するメモリー領域。
- このマクロはFSR1を変化させない。
    - PLUSW1という[FSR1+WREG]のメモリーをフェッチしてくれるという変な（？）窓から、取り出し先[Reg]に
    - movff命令で転送を掛けている。

<!-- dummy comment line for breaking list -->

~
- 実際のところ、関数の中身を全部 _asm 〜 _endasmで記述するのであれば、
- 引数はスタックに積むよりも呼び出される側の関数に属するarg変数のようなものを固定的に用意したほうが少し効率が良い。
- FSR1は、破壊すると困るレジスタをpush,popするだけに使用する。

<!-- dummy comment line for breaking list -->

**８０８０や６８００の時代に逆戻り**だな。

ワークエリアに A,  B,C, D,E, H,L とかを定義しておいたらまるで８０８０だ。

- そんな感じで使えば、８０８０程度のコーディングは可能だ。
- FSRは12bitしかないので、値を保持するレジスタとして使う場合は注意する必要がある。

