*usbRS232 の紹介 [#m7401562]

これは、ＡＴｔｉｎｙ２３１３を使った、ＵＳＢシリアル変換器です。

CDC（コミュニケーション・デバイス・クラス）で実装していますので
Ｗｉｎｄｏｗｓ標準のＣＤＣ仮想ＣＯＭポートドライバーにより、
"COMx:" (xは1〜9まで) ポートに見えるはずです。

## 配線図
- 回路図は、ＨＩＤａｓｐ（ｘ）、ＨＩＤｓｐｈのものと共通です。

<!-- dummy comment line for breaking list -->

	             ATtiny2313
	             ___    ___
	RESET(RESET)[1  |__| 20] Vcc
	PD0(RxD)    [2       19] PB7(SCK)
	PD1(TxD)    [3       18] PB6(MISO)
	XTAL2       [4       17] PB5(MOSI)
	XTAL1       [5       16] PB4
	PD2         [6       15] PB3(BUSY LED)
	PD3(D+)     [7       14] PB2
	PD4(D-)     [8       13] PB1
	PD5(pullup) [9       12] PB0
	GND         [10      11] PD6
	             ~~~~~~~~~~~

- ・括弧()の書かれているpinと、GNC,Vccに配線をしてください。
- ・GND,Vcc間にはパスコン(10uF以上)を入れてください。
- ・PB5,PB6,PB7は、ISP(イン・システム・プログラミング)端子で、
- 　外部にあるＡＶＲライターとの接続が出来るようにすると、いつでも
- 　ファームウェアの更新が出来るので便利です。
- ・USB D+,D-は、68Ω〜150Ω程度の抵抗を経由してＵＳＢコネクタに配線します。
- ・USB D- のＵＳＢコネクタ側端子は1.5kΩで５Ｖもしくは3.3Ｖにpull upします。
    - D-のpullupを直接電源に繋ぐのではなく,PD5端子に繋ぐことで、AVR再起動時にUSB再接続を自動で行うことも出来ます。(Optional)
- ・Vccを５Ｖ駆動させる場合はUSB D+,D-のコネクタ端子側にツェナーDiを入れて3.6V程度
- にクランプします。
- ・RxD,TxDはいわゆるTTLレベルです。（通常はHiで、スタートビットがLoになります。）

<!-- dummy comment line for breaking list -->

	  ---------------------------------------
	  SPI:     PB7-5
	  USB:     PD4   ===> USB D-
	           PD3   ===> USB D+
	  XTAL:    XTAL1,2 => Crystal 12MHz
	  ---------------------------------------

## テスト

- ・w32term.exeはＷｉｎｄｏｗｓのコマンドラインで動作する簡易ターミナルソフトです。
- ・teraterm等でも試験は出来ます。

<!-- dummy comment line for breaking list -->

- ATtiny2313の pin 2(RxD)とpin 3(TxD) をショートさせると、エコーバック
- テストが出来ます。

<!-- dummy comment line for breaking list -->

- ・Windows Vistaでは、USBのLowSpeed Bulk 転送が許されていないため、そのままでは
- 　動作しません。

<!-- dummy comment line for breaking list -->


## 免責

- まだ、いろいろな評価を行っていませんので不具合等がある可能性が高いです。
- 送信バッファフルの処理がありません。
- フローコントロールがありません。

<!-- dummy comment line for breaking list -->

- Ｗｉｎｄｏｗｓに接続するとドライバーのinfファイルを要求されます。
    - まだ用意できていませんので、AVR-Doperに付属のavrdoper.inf を使用してみてください。

<!-- dummy comment line for breaking list -->

## ビルド時の注意

- ・ビルドには WinAVR-20060421 を使用します。最新のavr-gcc(gcc4ベースのもの全て)
- ではコードサイズが大きくなり2kBに入りません。

<!-- dummy comment line for breaking list -->

- ・hardware.h  の HW_LED_PIN の値を変えると、BUSY LEDをPB0〜3のいずれかに変更できます.
- 同じく

		   #define HWPIN_USB_IS_D_4_3	0
		   #define HWPIN_USB_IS_D_3_2	1
- の設定を変えると、USB D-,D+の接続を PORTD 3,2 に変更することができます。

<!-- dummy comment line for breaking list -->


## 実用速度

- ボーレート設定は600bpsから128000bpsの間の任意の整数（偶数）値を設定できますが、

<!-- dummy comment line for breaking list -->

- UCHIホスト(intel系)では38400bpsで文字落ちが確認されていますので、19200bpsが限界です。
- OHCIホスト(AMD,SiS,NEC系)では60000bpsで文字落ちが確認されていますので、57600bpsが限界です。

<!-- dummy comment line for breaking list -->


## 謝辞

- LowSpeed AVR-USBでCDCクラスの実現は、Recursion Co., Ltd.
- [http://www.recursion.jp/avrcdc/indexj.html](http://www.recursion.jp/avrcdc/indexj.html) 
- の田村修氏の手により最初に行われました。
- このもとになったアイディアは2005年末の石川恭輔氏の実験に遡ります。

<!-- dummy comment line for breaking list -->

- また、ＡＶＲ-ＵＳＢ( [http://www.obdev.at/products/avrusb/index.html](http://www.obdev.at/products/avrusb/index.html)  )
- のリファレンスプロジェクトのひとつである、
- AVR-Doper - an STK500 Compatible AVR Programmer
- のソースを今回のフレームワークに使わせていたきました。

<!-- dummy comment line for breaking list -->

- 　上記の先達の方々に深く敬意と感謝を述べたいと思います。

<!-- dummy comment line for breaking list -->



## ＤＯＷＮＬＯＡＤ

- [ソースアーカイブ一式：usbRS232-1024.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/usbRS232-1024.zip) 
    - こちらはusbRS232-1018.zipにWin2k不具合対策を施したものです

<!-- dummy comment line for breaking list -->


- [kuga様の改良版アーカイブ一式：usbRS232-1109.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/usbRS232-1109.zip) 

<!-- dummy comment line for breaking list -->

- [victoriesさんの修正点を反映したものです。12/16MHz両対応：usbRS232-2009-0914.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/usbRS232-2009-0914.zip) 

<!-- dummy comment line for breaking list -->

    - いずれも千秋ゼミからのミラー版です。

<!-- dummy comment line for breaking list -->

## 更新履歴

- 2008.10.25:
    - kuga様のご指摘によりWindows2000での不具合を改善いたしました。
    - どうもありがとうございました。

<!-- dummy comment line for breaking list -->

    - AVR-USBドライバーを最新版に差し替えました。ファームウェアのサイズが小さくなっています。
    - ＵＳＢハブの記述を削除いたしました。

<!-- dummy comment line for breaking list -->
（手持ちハブで試しましたところ、バルク転送パケットが通らずに動作しません）


- - - -
## その他

**普通にＭＡＸ２３２を実装するのであれば下記の簡易的な変換を考える必要はありません。**

- ELM CHaNさんのＨＰに、簡易的にＲＳ２３２Ｃレベルに変換するノウハウが書かれています。

<!-- dummy comment line for breaking list -->

- [http://elm-chan.org/docs/rs232c.html](http://elm-chan.org/docs/rs232c.html) 

<!-- dummy comment line for breaking list -->

- 昔はテレタイプ等いろいろな接続先の機器がありましたが、今はもうＰＣのマザーボードのシリアルか、

<!-- dummy comment line for breaking list -->
ＵＳＢシリアル変換機のシリアル（たいていはＭＡＸ２３２のような変換ＩＣ）に繋ぐことしかない
ので、
- ＲＳ２３２Ｃ規格を厳密に守る必要もなく、ＭＡＸ２３２等の特性に合わせて繋がればそれで

<!-- dummy comment line for breaking list -->
いいのかもしれません。

- 上記ＨＰの応用的になりますが、７４ＨＣの代わりに普通のＬＳ−ＴＴＬのインバータータイプで代用

<!-- dummy comment line for breaking list -->
したり、トランジスタ１個の簡単な反転器を入れてやることもあります。

- ＬＳ−ＴＴＬを使う場合は、入力のＧＮＤ−ＶＣＣ以上にスイングしないよう電圧クランプのダイオードを

<!-- dummy comment line for breaking list -->
入れる必要があります。
- 出力はＴＴＬレベルのままでもＭＡＸ２３２ならセーフでしょう。

<!-- dummy comment line for breaking list -->

接続対象がＴＴＬ(CMOS)レベルのマイコンで、TxD,RxDとクロス接続する場合ならば、RxDの内部プルアップもおそらく不要です。
（main.cのソース中に記述があります）


- - - -

