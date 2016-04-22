[sdcc]]← 　[[PIC日記](2009-05PIC.md) ← 　→[sdccを飼い馴らす](PIC18F2550.md) 　 →[PIC18F4550](PIC18F4550.md) 



- - - -
**目次：**




~
~
~
共通のお約束
- 秋月AE-18F2550またはほぼ互換なハードウェア用（[PIC18F4550](PIC18F4550.md) でもＯＫ）です。
    - 参考：[ＵＢＷサイトにある回路図](ＵＢＷサイトにある回路図.md)  --> 水晶は20MHzに。(秋月のと同じファームが使えるので)。

<!-- dummy comment line for breaking list -->

- コンパイルには

<!-- dummy comment line for breaking list -->
[sdcc](http://sdcc.sourceforge.net/) 
と
[gputils](http://gputils.sourceforge.net/) 
を使います。

- PICへのファームウェア書き込みはブートローダーの使用をお勧めします。
- ツール、ブートローダーともに、[HIDmon-2550](HIDmon-2550.md) アーカイブに含まれています。

<!-- dummy comment line for breaking list -->

~
お断り
- ＰＩＣのアーキテクチャーが抱える問題と、sdccのやる気のなさにより、Ｃ言語で書かれたアプリケーションは肥大であり、低速です。
- しかしフルアセンブラで書くよりは、保守性が高いと思います。
- 部分的に高速化する手段は残されていますので、絶望しないでください。

<!-- dummy comment line for breaking list -->

	void sub1(void) __naked
	{
	 __asm
	   処理を記述
	   ・・・
	   return
	 __endasm ;
	}
こんな感じです。
- C:/sdcc/ にsdccをインストールしている場合、

<!-- dummy comment line for breaking list -->
以下のディレクトリにライブラリのソースがあります。
	C:/sdcc/lib/src/pic16/
- 一通りさらっと目を通しましょう。
- それに倣って書けば、PICなのに高速なアプリケーションが書ける様になります。

<!-- dummy comment line for breaking list -->



~
~
~
- - - -
## SDCC bootloader

■ 概要

- これはMicrochipが提供しているPIC 18F2550用のUSBブートローダー

<!-- dummy comment line for breaking list -->
のファームウェアを sdcc でコンパイル出来るように改造したものです。

■ 用途

- PICでのＵＳＢバルク転送ベンチマーク（ＵＳＢ転送高速化の研究）に使うことが出来ます。

<!-- dummy comment line for breaking list -->



■ Ｄｏｗｎｌｏａｄ
- [https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/sdccloader-0423.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/sdccloader-0423.zip) 

<!-- dummy comment line for breaking list -->

~

- 現在のところ、HIDmonのようにポートやメモリーを読み書きする機能はありません。
- ＵＳＢ転送のベンチマークだけを実行出来ます。
- PIDをFirmware-Bと同じにすれば、MyBootから操作できます。（但しFlash書き込みはsdccとC18の仕様に関わる問題により、今のところ出来ません）

<!-- dummy comment line for breaking list -->




~
~
~
- - - -

## UBW リメイク

■ 概要

- UBW Firmware-D 1.4.3 のリメイクです。
- MicroChipによるビルドディレクトリ構成が（深すぎて）気に入らなかったので、system/以下を平坦にしてみました。~

<!-- dummy comment line for breaking list -->
（--+勝手に改蔵+-勝手に改蔵+-勝手に改蔵+---）
- ブートローダーから起動するバイナリー(0x800 オリジン)と、外部PICライターを使って単独に書き込むバイナリー(0x0000オリジン)の両方が作成できます。

<!-- dummy comment line for breaking list -->

~

■ 用途

- ＰＣに接続すると仮想ＣＯＭポート接続によりファームウェアと交信出来ます。
- GainerのようにPICのI/Oの読み出しとコントロールが出来ます。
- Gainerに良く似た（しかしGainerより少しだけエレガントな）汎用USB-IOとして使えます。

<!-- dummy comment line for breaking list -->

~

■ Ｄｏｗｎｌｏａｄ
- [https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/sdccFW_D143.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/sdccFW_D143.zip) 

<!-- dummy comment line for breaking list -->




~
~
~
- - - -

## USB-CDCスケルトン

■ 概要

- UBWを改造して、CDCクラス（仮想ＣＯＭポートデバイス）のスケルトンを試します。

<!-- dummy comment line for breaking list -->

■ 用途

- 各種CDCデバイスを作成するフレームワークになります。

<!-- dummy comment line for breaking list -->

■ Ｄｏｗｎｌｏａｄ
- [https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/sdccUSB-cdc.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/sdccUSB-cdc.zip) 

<!-- dummy comment line for breaking list -->

- 仮想ＣＯＭポートから送られてきた文字をそのままエコーバックします。

