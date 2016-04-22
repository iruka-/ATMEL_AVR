[2009-05PIC](2009-05PIC.md) 


- - - -

## PICkit2

私はPICのライター環境やMCHIP純正開発環境のことをほとんど何も知りません。
- が、意外な純正ツールを発見致しました。
- [ＰＩＣにプログラムを書き込むには](http://www.ne.jp/asahi/air/variable/picmel/integration/write/index.htm) 

<!-- dummy comment line for breaking list -->

    - [ＰICｋｉｔ2の使用方法](http://www.ne.jp/asahi/air/variable/picmel/integration/write/pickit2/index.html) 

<!-- dummy comment line for breaking list -->

- [PICkit-2のロジック・アナライザ機能を試す（前編）エレキジャック](http://www.eleki-jack.com/mycom2/2008/06/pickit2_4.html) 

<!-- dummy comment line for breaking list -->

- [PICkit2CLONEの作り方](http://www.geocities.jp/te2tr6n5x/PIC/007.html) 

<!-- dummy comment line for breaking list -->

そうです。PICkit2というMicroChip純正のちっぽけなツールがあったのです。

MicroChip
- [http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1406&dDocName=en023805](http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1406&dDocName=en023805) 

<!-- dummy comment line for breaking list -->

![http://media.digikey.com/photos/Microchip%20Tech%20Photos/DV164120.jpg](http://media.digikey.com/photos/Microchip%20Tech%20Photos/DV164120.jpg) 


秋月で3500円で売ってます。（PICKit2のみの値段）
- [http://akizukidenshi.com/catalog/g/gM-02508/](http://akizukidenshi.com/catalog/g/gM-02508/) 

<!-- dummy comment line for breaking list -->


なんとISP端子が、そのまま1Msps程度のロジアナにもなるとか。

クローンも簡単に作れそうです。中身は18F2550ですから。

PIC焼きのための昇圧回路の配線がやや面倒ですが、ここも忠実に再現しなくていいのなら
秋月の５０円のスイッチングＩＣで作れちゃいます。

- [http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1406&dDocName=en023805](http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1406&dDocName=en023805) 
- 上記MicroChipのサイトから最新版のソフトPICkit 2 v2.61がＤＬできるようです。

<!-- dummy comment line for breaking list -->


とりあえず自作の18F4550基板とか秋月AE-18F2550をPICkit2化出来るかどうか試してみようと思います。
- そうやってライターばかり増やしてどうするんだという声も聞かれますが・・・。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## ダメもとでPICKit2のツールをダウンロードする。

- [http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1406&dDocName=en023805](http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1406&dDocName=en023805) 
- 上記MicroChipのサイトから最新版のソフトPICkit 2 v2.61をＤＬします。

<!-- dummy comment line for breaking list -->

- 展開（インストール）すると、展開先に"PK2V023200.hex" というHEXが出来ます。
- これをw509ライターなどでPIC 18F4550に焼きます。
- 水晶の周波数が違っていたら動かないよなーとか思いながらも、USBを接続します。
- なんと、PICKit2と認識してしまいました。

<!-- dummy comment line for breaking list -->

    - VID:PIDは 04D8:0033 でＨＩＤデバイスです。
    - HEXファイルを見ると、ブートローダーは４Ｋ弱ありますので、MCHIP純正のHID Bootloaderが先頭に焼かれているようです。

<!-- dummy comment line for breaking list -->

- PICKit2のツールを起動すると、HVP書き込みのための高電圧が足りないと言われました。
    - それは、回路が存在しないからでしょう。
    - 回路はUBW互換のままであり、水晶も20MHzのままなのにそのまま認識して使えてしまいました。
    - **便利すぎだろ**なりすまし野郎め！
- ＴｏｏｌメニューのなかにUARTツールとLogicツールがあり、
    - UARTツールを使うと38400bpsまでの内蔵シリアル（？）を使った端末が使え、
    - Logicツールを使うと1Mspsまでの（３入力）ロジアナになるようです。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## PICKit2のUARTについて。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/uarttool.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/uarttool.png) 

- 300bpsから38400bpsまでメニューセレクトと150bpsから38400bpsまでの任意のボーレート数値入力が出来ます。
- 回路図によるとTxD/RxDはISP端子と全く接続されていませんので、ソフトウェアUARTで実現していると思われます。
- もちろんUSB-HIDデバイスでシリアルなんていうプロトコルは存在しませんので、内蔵の独自ターミナルソフトだけの世界です。
- ASCII表示のほかにHEX表示も出来ますので、それなりにマイコン側のダンプを受け止める場合の都合が良いのかもしれません。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## PICKit2のロジアナについて。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/logictool.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/logictool.png) 

- これがまた、自作１８Ｆ４５５０ボード（ＵＢＷ互換）に何の改造も加えずにそのまま動作してしまいました。
- 開いているサンプリングメニューを見てもらうと一目瞭然なのですが、このロジアナは１０００サンプルしか取得しません。
- ３ビットですが、１ｋサンプルです。ＰＩＣには２ｋＢの内蔵ＲＡＭがあるので１０００サンプルを（１バイト単位で）取っても１ｋＢで足ります。実際のところHIDmonでさえ、５１２バイトもあれば動きますので、メモリー的には余裕です。
- 最初は、1Mspsのロジアナと聞いて、（つまり３Ｍbit/ｓのホストＰＣへの帯域確保と定速サンプリングの両立をどのようにやっているのだろうかと興味津々でした。
- しかし、なんのことはありません。サンプリングは内蔵ＲＡＭの範囲内で全力サンプリングなのです。

<!-- dummy comment line for breaking list -->

- これなら作れそうな気がしてきました。
- LowSpeed USBのAVRでもMega644のようにＲＡＭが４Ｋもあればもっと高性能なロジアナが作れるでしょう。
- この場合ホストへの転送速度はあまり関係ないのです。

<!-- dummy comment line for breaking list -->

しかし、PICkit2のファームとLogicツールがＵＢＷ互換ハード上でそのまま動作することがわかった以上、もうロジアナっぽいソフトを作る必要はないのかもしれません。（ＧＵＩツールは作るのが面倒ですから）

~
~
~
~
- - - -
## PICkit2のファームその後
- 外部ライターを使ってHIDmon bootloaderとPICkit2の焼き直しを行うのが面倒くさくなってきたので
- PICkit2からMCHIPブートローダーを抜いたＨＥＸを（テキストエディタで）作ってみました。
- PICkit2のファームの本体部分は0x2000から始まっているので、

		org 0x800         //reset
		     goto 0x2000
		org 0x808         //low_isr
		     goto 0x2008
		org 0x818         //high_isr
		     goto 0x2018
- みたいな６行アセンブラを書いて８００番地に焼き、

<!-- dummy comment line for breaking list -->

- テキストエディタで切り取ったPICkit2ファームを２０００番地から焼きます。

<!-- dummy comment line for breaking list -->

    - 焼くのは外部ライターからではなくHIDmonのファームに内蔵されたブートローダーを使用してpicboot.exeで焼きます。

<!-- dummy comment line for breaking list -->

- HIDmonはXINST=0でコンフィグしてあります。
- HIDmonをアプリモードでリセットすると、新しいUSBデバイスをPICkit2の名前で認識し
- なぜかロジアナまで、すんなり動いてしまいました。

<!-- dummy comment line for breaking list -->

**18F2550/4550（どちらも４００円ＩＣ）は、便利すぎです。**

ほんとにPICkit2互換昇圧回路作ってPICkit2クローンにしたくなってしまいました。

と、思ったら、互換品を制作されていらっしゃる方を発見。
- [http://iizukakuromaguro.web.fc2.com/145_hobopickit2/145_hobopickit2.html](http://iizukakuromaguro.web.fc2.com/145_hobopickit2/145_hobopickit2.html) 

<!-- dummy comment line for breaking list -->

- 秋月で３５００円で買える今となっては、価格面でのメリットはありませんが、
- 中日電工ＮＤ８０Ｚ（と言って分かる人はいるのかな）的な手作り感のぬくもりが感じられます。

