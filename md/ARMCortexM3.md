[ARM](ARM.md) 



![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32F103VBT6.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32F103VBT6.jpg) 



## 諸元
|CPU|ARM Cortex-M3(Thumb2命令セットのみ。ARM32は実行できない)|
|CPU CLOCK|72MHz|
|Flash ROM|128kB|
|SRAM|20kB|
|Peripheral|12bitA/D,USB,CAN,PWM,SPI,I2C,USART|

ジャンパー設定

|J6ジャンパー|起動番地|備考|
|open|0800_3000|アプリケーション起動|
|close|0800_0000|DFU(USBブートローダー)起動|

ブートモード

|BOOT1|BOOT0|ブートモード|備考|
|−|0|ユーザーFlash|0800_0000ユーザーFlashからのブート（この基板ではこのモードになっている(両方PullDown)）|
|0|1|システムメモリー|1fff_f000からのブート(シリアルブート?uart1から?)|
|1|1|内蔵SRAM|内蔵SRAMからのブート(内蔵SRAM?誰が書き込むんだろう？JTAGかな)|

## メモリーマップ

|0800_0000〜0801_ffff|128kB|Flash領域|
|上記のうち、0800_0000〜0800_2fff|12kB|DFU(ブートローダー)が使用|
|1fff_f000〜1fff_f7ff|2kB|システムメモリー(Flashダウンローダー)|
|2000_0000〜2000_4fff|20kB|内蔵SRAM|

## その他

困った点:
- 開発環境はIARかKeil、いずれも有償。
- gccの開発環境は提供されていない（ように見受けられる）-->DWM６月号にgccの記事があるらしい。

<!-- dummy comment line for breaking list -->

で、もう２年も前の廃刊雑誌なので、開発環境をどうしようか思案中。
- たぶんWinARMが一番良いかな？
- USBフレームワークはどこにあるのだろう・・・。

<!-- dummy comment line for breaking list -->

アプリケーションノートはこのへん
- [http://www.st.com/mcu/modules.php?name=mcu&file=familiesdocs&fam=110&doc=3](http://www.st.com/mcu/modules.php?name=mcu&file=familiesdocs&fam=110&doc=3) 

<!-- dummy comment line for breaking list -->

DFUのプロトコルはこのへん
- [http://www.st.com/stonline/products/literature/an/17068.pdf](http://www.st.com/stonline/products/literature/an/17068.pdf) 

<!-- dummy comment line for breaking list -->

USBライブラリはこのへん
- [http://www.st.com/mcu/modules.php?name=mcu&file=familiesdocs&fam=110&doc=76](http://www.st.com/mcu/modules.php?name=mcu&file=familiesdocs&fam=110&doc=76) 

<!-- dummy comment line for breaking list -->

DFUのソースがないんだけど・・・
- --> 雑誌付属CDROMのsoft/ST/USB関連サンプル・マニュアル/STM32F10xUSBLib/USBLib/demos/Device_Firmware_Upgrade/project/ にあるらしい。

<!-- dummy comment line for breaking list -->

DFU馬鹿じゃね？
- HEXかmotorora-Sを *.dfuファイルに変換。
- DFuSe ツールから *.dfuをデバイスに転送。
- なんで普通にHEXを読まないのか意味不明。

<!-- dummy comment line for breaking list -->

~
~
~

実はDFUはUSB.orgで定義されている。*.dfuファイルのフォーマットもドキュメントされていた。
- [http://www.usb.org/developers/devclass_docs/usbdfu10.pdf](http://www.usb.org/developers/devclass_docs/usbdfu10.pdf) 

<!-- dummy comment line for breaking list -->

ここの  STM32F10xxx USB-FS-Device development kit (UM0424)に最新版のDFUソースが含まれているらしい。
- [http://www.st.com/mcu/familiesdocs-110.html#Application%20Note](http://www.st.com/mcu/familiesdocs-110.html#Application%20Note) 

<!-- dummy comment line for breaking list -->

~

## STM32F103の遊び方
- USB-FS-Device development kitというのが、まるでモスバーガー屋のUSB Application Library と同じような作りなのでびっくり。

<!-- dummy comment line for breaking list -->

遊び方としては、
- （１）Custom_HIDクラスを流用して、そのまんまPICmonを実装する。
- （２）Device_Firmware_Upgrade クラスをGeneric USBに書き換えて、高速版を実装する。
- （３）Virtual_COM_Port に見せかけたPICmonを実装する。

<!-- dummy comment line for breaking list -->

どれでもよいかも。

- しかし、Makefileが全く用意されていないところといい、コンパイラが複数ベンダーから供給されていて互換性がないところといい、**安全玩具屋はモスバーガー屋そっくりだな。**

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/burger.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/burger.png) 

~

## CMSIS
Cortex Microcontroller Software Interface Standard (CMSIS) 
- [http://www.onarm.com/download/download395.asp](http://www.onarm.com/download/download395.asp) 
- チップメーカー毎にペリフェラル操作方法が異なるのはどうよってことで統一規格を作っているらしい。（旗振りはARM）
- しかし、Cortex-M0,M3限定だった。LPC2388用なんて**無い**
- USB関係がSTM32とLPC1xxxで共通化出来るのかと思って見に行ったら全然そんな風ではなかった---期待外れ。

<!-- dummy comment line for breaking list -->


~
## とりあえず遊び方（１）からやってみる。

- これまた全然ビルドが通る気配がない。
- ヘッダーファイルが散らばりすぎ。

<!-- dummy comment line for breaking list -->

~
~
~
これはかなりの難物。手強さで言うとＰＩＣを遥かに超える。
- ＡＲＭって、もっと素直じゃなかったのか？

<!-- dummy comment line for breaking list -->

- 一応ビルドできた。
- さて、hexファイルはどこかな？

		#
		D:&gt;dir main.*
		ドライブ O のボリューム ラベルは です
		ボリューム シリアル番号は 049D-0411 です
		2010/04/13  23:13            25,503 main.elf
		2010/04/13  23:13            36,811 main.map
		2010/04/13  23:13             2,015 main.sym
		2010/04/13  23:13                34 main.hex
- さすがはＡＲＭだ。仮想ＣＯＭポートのファームウェアのＨＥＸがたったの３４バイトだなんて**凄すぎ**
- いやーThumb2命令は縮むなぁ・・・

<!-- dummy comment line for breaking list -->

- 念のため、HEXの中身をチェックだ。

<!-- dummy comment line for breaking list -->

	D:>type main.hex

	:0400000508002000CF
	:00000001FF

	  ______
	（・Ｖ・） ?？

	     ______
	？?（・Ｖ・）

- だめだこりゃ。

<!-- dummy comment line for breaking list -->

~
~
~
原因判明。
- リンカーオプションのgc-sections（不要関数のガベコレ有効オプション）で全部削り取られていた。
- gc-sectionsを外してリンクすると、うじゃうじゃ未解決参照が出てくる。
- まず、スタートアップファイルを探して入れてみたが・・・うまくいかない <----今ここ

<!-- dummy comment line for breaking list -->
~
~
先は長そうだ。

~
- 思うに、これ（つまりgcc用の環境をわざと外してある）ってIARかKeilの有償コンパイラを売りたいがための嫌がらせにしか見えない。

<!-- dummy comment line for breaking list -->

- そんなことしなくったって、**コンパイラの性能で競うべき**なんじゃないのだろうか？

<!-- dummy comment line for breaking list -->

- PICコンパイラの世界にもいろいろあったけど、ARMのほうが酷いのか。

<!-- dummy comment line for breaking list -->

## ダウンロード
~
ステータス：とりあえずビルドだけ通ります。

- [vcom.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/ARM/vcom.zip)  --- 仮想COM
- [dfu.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/ARM/dfu.zip)  --- DFUブートローダー
- [hid.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/ARM/hid.zip)  --- Custom HIDクラス

<!-- dummy comment line for breaking list -->

- hidとvcomはデバイス認識ＯＫになりました。
- ただし、STmicroサンプルのままなので、ファームの内容はかなりやる気のないものです。
- hid(Custom_HID)のPC側appletというかexeファイルかそのソースが見当たりません。配布元ファイルのum0424.zipの中を探したのですが・・・。

<!-- dummy comment line for breaking list -->

- dfu.zipは、まだNVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);を入れていないので0x8003000番地では動きません。
- というよりはdfuなので、0x8000000に置かないとだめなような気がします。

<!-- dummy comment line for breaking list -->

- 各zipファイルに含まれるHW/ディレクトリの内容は同一のはずです。(自分のLinux上ではシンボリックリンクにより共通場所を指す様にしています)

<!-- dummy comment line for breaking list -->

~
~

## ビルド環境の構築方法
- [WinARMビルド環境の構築方法](WinARM.md) 
- [ARMクロスコンパイラ構築（Linux）](2010-04#b817f226.md) 

<!-- dummy comment line for breaking list -->



~
~
~
## 作りかけのHIDmon

- [armon.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/armon.zip)  --- PICmonみたいなやつ。
- armonは今のところ、STBEE , STBEE-Mini ,CQ-STARM , STM8S-DiscoveryのST-Link側, の４種類で動くようになりました。

<!-- dummy comment line for breaking list -->


~
read more : [STM32ブートローダー](armon.md) 


~


~
~
~
~
~
~
~
~
~
~
~
~

- - - -
## リンク
ＣＱ出版DesignWave ５月号
- [http://www.cqpub.co.jp/DWM/contents/dwm0126i.htm](http://www.cqpub.co.jp/DWM/contents/dwm0126i.htm) 

<!-- dummy comment line for breaking list -->

若松通商Design Wave Magazine 5月号 
- [http://www.wakamatsu-net.com/cgibin/biz/pageshousai.cgi?code=63130001&CATE=6313](http://www.wakamatsu-net.com/cgibin/biz/pageshousai.cgi?code=63130001&CATE=6313) 

<!-- dummy comment line for breaking list -->

ARMv7 Cortex-M3 Projects
- [http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/index_cortex.html](http://www.siwawi.arubi.uni-kl.de/avr_projects/arm_projects/index_cortex.html) 

<!-- dummy comment line for breaking list -->

CQ-STARM基板をいじる
- [http://www.geocities.jp/altshibabou/win/cqstarm.html](http://www.geocities.jp/altshibabou/win/cqstarm.html) 

<!-- dummy comment line for breaking list -->

TOPPERS/JSP for CORTEX-M3
- [http://sourceforge.jp/projects/jsp4cm3/devel/](http://sourceforge.jp/projects/jsp4cm3/devel/) 

<!-- dummy comment line for breaking list -->

(別室)ＡＲＭマイコンで遊ぼう 
- [http://homepage3.nifty.com/ARTWEB/arm1.htm](http://homepage3.nifty.com/ARTWEB/arm1.htm) 

