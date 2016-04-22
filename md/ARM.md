-主に、廉価な**ARM Cortex-M3**系基板で遊んでみます。
- 遊び方は、メーカー系押し付け環境(GUI)を使わないこと。
    - GNU Cコンパイラ(**CodeSourcery G++ Lite版(無償)**)と**GNU Make**でビルドします。
    - コンパイル環境はDOS(Win32) コマンドラインもしくはLinuxコンソールです。
    - Flash書き込みソフトもメーカー系押し付け環境(DFUなど)を使わずに**HID Bootloader**を使用します。
    - HID Bootloaderには、ターゲット側メモリーの読み書き機能が付いているので、メモリー内容やI/O Portの確認、書き換え、逆アセンブル、あらかじめFlashに書き込んだブログラムの起動、などを行なうことができます。
    - HID Bootloaderのターゲット側メモリーの読み書き機能を対話的に行なうツール(armon/armboot)を使用することで簡易USB-I/Oとして使用できます。
    - バッチスクリプトの実行が出来るのでＬＥＤ点滅のようなことならスクリプトだけで行なうことが可能です。
    - I/O Portの確認、書き換え機能を使うと、プログラムを組まなくてもI/O Portの動作確認や配線チェックが可能です。(低速ですが[グラフィカルな信号表示機能](HIDmon88.md) もあります。)

<!-- dummy comment line for breaking list -->

~

    - HID Bootloaderのユーザーモード（ユーザーアドレス側で実行するモード）プログラムはブートローダーの機能＋アルファ([AVR/PIC/ARMライター機能付き](arm_blaster.md) )になっていて、自由にコマンド追加が可能です。
    - HID BootloaderはSTM32、LPC1343、LPC2388に移植済みです。
    - HID Bootloaderは[PIC18F2550/18F14K50用のもの](pic18boot.md) とプロトコル、操作方法に関して類似性があります。

<!-- dummy comment line for breaking list -->


- - - -

ARM 目次



- - - -

~
~

## [armon/armboot](armon.md)  移植済みの基板リスト

**[[armon/armboot>armon]]**は各種ARM基板用のHIDブートローダー兼、簡易モニターです。

**特徴**
- HIDデバイスなのでWindowsなどのドライバー組み込みが要らず、USBを差し込んだだけで即使えます。
- bootloaderの常駐量は8kBです。
- 任意のアプリケーション(hexファイル)をUSB経由でFlashに書き込んで起動することが出来ます。
    - dfuと異なり、hexファイルを直接書き込みするコマンドラインツール(Win32/Linux)を用意しています。
- メモリー内容やポート状態の読み出しや書き換えが可能です。
- 自分自身(bootloader)のアドレスをずらしたものをアプリケーションとして書き込むことで、好きなようにコマンドを増やしたり機能拡張出来ます。
- bootloader部分とアプリケーション部分の行き来(互いにbootさせること)や、互いに書き換えを行う(update)ことも可能です。
- ユーザー定義可能なコマンド(user)があらかじめ用意されていて、そのuser関数内でprintf埋め込みによる簡易デバッグが出来ます。

<!-- dummy comment line for breaking list -->


~
- - - -

- ほぼどれのＣＰＵでも同じように動くようになりました。
- LPC2388用はSTM32からバックポートしました。

<!-- dummy comment line for breaking list -->

|アーキテクチャー|ＣＰＵ(ベンダー)|基板名|FLASH容量|SRAM容量|
|Cortex-M3|STM32(STMicro)|[STM8S-Discovery](stm32f103.md) のSTM32側|64kB|20kB|
|||[CQ-STARM](armon.md) 　DesignWave 2008-05付録|128kB|20kB|
|||[STBEE](armon.md) 　ストロベリーリナックス|512kB|64kB|
|||[STBEE　Mini](armon.md) |128kB|20kB|
||LPC1343(NXP)|[LPCXpresso](LPCXpresso.md) 　NXPセミコンダクターズ|32kB|8kB|
||       |[TRZ1010N](LPCXpresso.md) 　トラ技増刊「ARMマイコン パーフェクト学習基板」|32kB|8kB|
|ARM7TDMI|LPC2388(NXP)|[CQ-FRK-NXP-ARM](ARM7mon.md) |512kB|64kB|

- - - -
各種ARM基板

- 設計会社であるARMはチップを１個も製造していないので、NXP(オランダ・元フィリップス)とかST-Micro(元SGS-Thomson)とかAtmelなどが製造するチップを使用することになります。

<!-- dummy comment line for breaking list -->




## STM32勢ぞろい

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/stbee.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/stbee.jpg) 

左から、
- [STBee　MINI](http://strawberry-linux.com/catalog/items?code=32105) 
- [STBee](http://strawberry-linux.com/catalog/items?code=32103) 
- [STM8S-Discovery](http://akizukidenshi.com/catalog/g/gM-03457/) のST-Link側のみ
- DesignWave2008-05 付録 CQ-STARM

<!-- dummy comment line for breaking list -->

- クロックは全部72MHz Cortex-M3
- Flash+SRAM容量は左から、(128K+20K),(512K+64K),(64K+20K),(128K+20K)

<!-- dummy comment line for breaking list -->

- - - -


## [⇒interface 2009-05付録基板で遊ぶ](ARM7TDMI.md)
- まずはＬチカから始めます。

<!-- dummy comment line for breaking list -->

## [⇒interface 2009-05付録基板で遊ぶPartII](LPCUSB.md)
- LPC-USBを動かしてみます。

<!-- dummy comment line for breaking list -->

## [⇒interface 2009-05付録基板で遊ぶPartIII](ARM7mon.md)
- HIDブートローダー兼モニターです。

<!-- dummy comment line for breaking list -->


![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/cq-frk-nxp.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/cq-frk-nxp.jpg) 

- LPC2388はNXP製。周辺機能、メモリーてんこ盛りの32bit ARMチップです。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -


## [⇒DesignWave 2008-05付録基板で遊ぶ](ARMCortexM3.md)

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32F103VBT6.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32F103VBT6.jpg) 

- STM32F103VBT6はST-Micro製のCortex-M3アーキテクチャー。
- Cortex-Mシリーズは命令コード16bitのthumb2限定です。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -


## [⇒STM8S-Discovery基板のST-Link側で遊ぶ](stm32f103.md)




![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/ST_Discovery.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/ST_Discovery.jpg) 

なんとびっくり価格。秋月などで７５０円です。

ＦＡＱ：[どうして安いのですか？](http://akizukidenshi.com/catalog/faq/goodsfaq.aspx?goods=M-03457) 

- [⇒STM8S-Discovery基板のST-Link側で遊ぶ](stm32f103.md) 

<!-- dummy comment line for breaking list -->
~
~
- - - -
## [⇒STM32ブートローダー](armon.md)

- [STM8S-Discovery付属のSTM32F103C8T6　CPU](stm32f103.md) 用のHIDクラス・ブートローダー 兼 簡易モニターです。
- STBEE , STBEE Mini , CQ-STARM でも動きます。
- DFUより使いやすく、コードサイズも小さくなっています。(8kB)
- ほぼ同じ機能のコードをアプリケーション側としてコンパイルして使用出来るので、自己拡張したり、HIDデバイスによる簡単なアプリ作成に使えます。(user関数からprintfなども使用できるようになっています)

<!-- dummy comment line for breaking list -->



~
~
~
- - - -


## [⇒LPCXpressoで遊ぶ](LPCXpresso.md)

LPCXpresso(LPC3154+LPC1343)
- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/xpresso2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/xpresso2.jpg) 
- [⇒LPCXpressoで遊ぶ](LPCXpresso.md) 

<!-- dummy comment line for breaking list -->
~

ARMマイコン パーフェクト学習基板（TRZ1010N）
- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/1343a.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/1343a.jpg) 

<!-- dummy comment line for breaking list -->

~
~
~
- - - -

## [⇒LPCXpresso/1768で遊ぶ](LPCXpresso1768.md)
- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/LPC1768.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/LPC1768.jpg) 
- [⇒LPCXpresso/1768で遊ぶ](LPCXpresso1768.md) 

<!-- dummy comment line for breaking list -->

~


~
~
~
- - - -
## [⇒Fujitsu-FM3で遊ぶ](FM3.md)
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/FM3img.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/FM3img.jpg) 

- [⇒Fujitsu-FM3で遊ぶ](FM3.md) 

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## 市販の格安ARMマイコン基板にはどんなものがあるか？

ＣＱ出版の雑誌付録基板（雑誌込みで３０００円前後）
- 基板はコストダウンのためか総じて電源周りが弱い。
- LDOレギュレータがUSBケーブルの挿抜で死んだりとか。
- 旬が過ぎたら追加発注は難しい。（追加発注するほどのものでもないけれど）

<!-- dummy comment line for breaking list -->

秋月扱いのARM関連 開発ボード（２８００円程度）
- [http://akizukidenshi.com/catalog/c/carm/](http://akizukidenshi.com/catalog/c/carm/) 

<!-- dummy comment line for breaking list -->

苺：STBee（３０００円程度）
- [http://strawberry-linux.com/catalog/items?code=32103](http://strawberry-linux.com/catalog/items?code=32103) 

<!-- dummy comment line for breaking list -->

『STBee Mini』(72MHz, 128K+20K)
- [http://strawberry-linux.com/catalog/items?code=32105](http://strawberry-linux.com/catalog/items?code=32105) 

<!-- dummy comment line for breaking list -->


ARM7TDMI/LPC2103モジュール（１６００円：USB機能は無い）
- [http://www.csun.co.jp/SHOP/2009080405.html](http://www.csun.co.jp/SHOP/2009080405.html) 

<!-- dummy comment line for breaking list -->

~
~
~
~
~
- - - -
## STM32系(ST-Micro製)とLPC1xxx系(NXP製)の違い。

どちらも**Cortex-M3**アーキテクチャー(**Thumb2命令セット**のみ実装されている)

- メモリーマップは異なる。(STM32のFlashは0800_0000〜、LPC1343は0番地〜。RAM空間も異なる。)
- GPIOなどI/Oポートのアドレスや関連レジスタは互いに**互換性がない。**
- ~~Flashの自己書き換えを行うユーザー関数がLPC1343には無い。(というかApplication内からのFlash書き換え方法がドキュメントされていない模様)~~
    - STM32ではFlashの自己書き換えを行う関数が用意されている。
    - LPC1343は、関数が用意されないかわりに、16kBのBOOT-ROM内のサービスルーチンが用意されている。

<!-- dummy comment line for breaking list -->

- USB関連のレジスタやサポート関数は互いに**互換性がない。**
- LPC1343はJTAGによるデバッグ、Flash書き込みのサポートが無い。(かわりにSWDを使う)

<!-- dummy comment line for breaking list -->

CMSISがあれば共通なソースが書けると思ったら**大間違い**。Ｌチカだけなら不可能ではない、というレベル。
- ペリフェラルの性能(例えばADCの分解能とか)も異なるし、そもそも無いもの(温度センサーとか)は無い場合があるので、共通化は無理!!

<!-- dummy comment line for breaking list -->

~
- LPC1343にはUSB/シリアルブートをサポートしたBOOT-ROM(16k)が内蔵されている。WindowsからはUSBマスストレージに

<!-- dummy comment line for breaking list -->
見えるので、Flashの書き換えはそれほど手間が要らない。
- ROMの内蔵ルーチンを呼び出す形のUSBデバイスサンプルが用意されているが、カスタマイズ上の制限が多く、実はあまり使い物にならない。
- LPC1343のＣ言語ライブラリ(libc)のスタートアップ(crt0.S相当)はアセンブラ記述ではなくＣ言語記述で提供されている。
- このため、（需要があるかどうかはともかく）比較的容易にスタートアップを変更できる。ただし初期化部分には一部インラインアセンブラが使われている。

<!-- dummy comment line for breaking list -->

- LPC1343はUSBクロックPLLとCPUクロックPLLが独立に設定できる。（オーバークロック可能？？？）
- STM32はUSBクロックの設定が｛CPUクロック÷1か、÷1.5｝の２択しか選べないので、（で、USB用のクロックは48MHz以外はありえないので）必然的にCPUクロックは48MHzか72MHzに固定される
    - もちろんUSBを使用しないのであれば内蔵RCだろうが、何GHzのXtalだろうが（動きさえすれば）突っ込んでいいけど。
    - どんなにオーバークロックしたくとも、内蔵のFlashROMの動作周波数の制約を受ける（そもそもこのクラスのマイコンは命令キャッシュを積んでいない）ので、そんなに速くは出来ない。

<!-- dummy comment line for breaking list -->

~



<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## ARMでArduinoしたい。

一番近いのがこれ:
~
maple
- [http://leaflabs.com/devices/maple/](http://leaflabs.com/devices/maple/) 
- ArduinoのIDEをそのまんまパクって、AVR-gccをCodeSourceryG++に差し替え、avrdudeをdfu-utilに置き換えたもの。
- 今のところSTM32限定。
- 用意されている基板はArduinoとシールドのピン配置が良く似ている。
- USBはFT232RLのようなUSB-シリアル変換チップを使用する代わりに、STM32の内蔵USBを使用している。

<!-- dummy comment line for breaking list -->

~
mbed
- [http://mbed.org/](http://mbed.org/) 
- こちらは、NXPのLPC1768が使用されている。
- USB-シリアル変換チップはFT232RLでなく、NXPのARM系のUSB内蔵MCUがやっているような感じ。mbedは実際には2CPU構成だ。価格も6000円近い。

<!-- dummy comment line for breaking list -->

- mbedはWebBrowser経由で開発環境（ARM純正のC++コンパイラー）を動かす。
- ローカルのPC上に開発環境をセットアップする必要がない。(仮にセットアップしたくても純正コンパイラは高価)

<!-- dummy comment line for breaking list -->


Audinさんの記事が詳しい
- [http://mpu.seesaa.net/article/1572447.html](http://mpu.seesaa.net/article/1572447.html) 

<!-- dummy comment line for breaking list -->

~
netduino
- [http://www.netduino.com/](http://www.netduino.com/) 
- これもArduinoと基板サイズやらシールドピンの位置を合わせている。
- 開発言語はC/C++のかわりにMicrosoft .net （Ｃ＃とか）の組み込みエディションを使う。
- なので、開発環境はWindows用のMicrosoftのＣ＃の統合環境がそのまま使用できるらしい。

