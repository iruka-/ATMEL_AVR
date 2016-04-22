関連リンク　[Pinguinoで遊ぼう](Pinguino.md) 　[ブートローダーを作る](HIDBootX.md) 　[シリアルブートローダーを作る](uartflash32.md) 　[USB仮想シリアル](PIC32MX_USBCDC.md) 　[USBカスタムデバイス](USB_CUSTOM.md) 　[USB簡易モニター](PIC32mon.md) 　
[USBホスト](USB_HOST.md) 
　[USBオシロスコープ](USBオシロスコープ.md) 
　[PIC32MXでVGA出力](pic32vga.md) 
　[PIC32MXでBASICを動かす](tinyBasic2.md) 
[PIC32でBluetooth](Bluetooth.md) 
　　[USBStudy](USBStudy.md) 　　
[2013-02](2013-02.md) 
　[2013-03](2013-03.md) 



## PIC32MXについて（概要）

**PIC32MX220F032B**

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg) 

- MicroChip社製の32bitマイクロコントローラー(MCU)です。
- 28pin DIPの廉価パッケージが存在します。([秋月](http://akizukidenshi.com/catalog/g/gI-05852/) で220円)
- 命令セットは往年のMIPS32/mips16互換です。(といっても32bitMIPS命令セットとしては比較的新しいものを取り込んでいます)
- USBデバイス/ホストのどちらにもなれます。
- I/Oペリフェラルの仕様はPIC24F~~の使い回しです。~~のそれと非常に良く似ています。

<!-- dummy comment line for breaking list -->

~
- - - -
目次


- - - -

## 攻略法
- 純正環境なら、microchipが配布している統合IDEのMPLABXとXC32コンパイラ(中身はMIPS32用 gcc)となっていますが、
- 無償版のXC32コンパイラはmips16オプションが使えなかったり、最適化オプションの使用に制限があります（というか最適化出来ません）。
- MicroChip社製の少し古い開発環境(MPLAB v8.89)に付属するpic32-gccはもう更新が止まっていて古いのですが、一応mips16オプションが使用可能です。（但し、mips16オプションと最適化オプションが使えるのは試用版の60日間のみです((関数アトリビュート __attribute__（（mips16）） を使用して関数単位でmips16化するという裏技は試用期間を過ぎた試用版やフリー版でも有効のようです。)) ）

<!-- dummy comment line for breaking list -->

ということなので、今回は**思いきって**

- コンパイラやフレームワークに[Pinguino](http://wiki.pinguino.cc/index.php/Main_Page/ja) を使用します。
- [Pinguino](http://wiki.pinguino.cc/index.php/Main_Page/ja) のmips-gccには試用期間や最適化オプション制限

<!-- dummy comment line for breaking list -->
などは存在せず、フルスペックを（というか普通のgccとして）安心して無料で使用できます。
- Pinguinoには、Windows版だけでなくLinux版とMacOS版の開発環境(gcc含む)も公開されています。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## PIC32MXにファームウェア（プログラム）を書き込む５つの方法

### （１）[PicKit3](PicKit3.md) を買ってくる。
- [秋月](http://akizukidenshi.com/catalog/g/gM-03608/) で3900円で売っています。（注：消費税増税前、2013年時点での価格です。）
- [PICKit3でHEXを書き込む場合の注意点](PicKit3.md) 

<!-- dummy comment line for breaking list -->

### （２）[シンセ・アンプラグド](http://d.hatena.ne.jp/pcm1723/20121230/1356849249) さんのLPC1114FN28ライターを使う。

- LPC1114FN28というDIPタイプのARMマイコンと、USB-serial変換器（もしくはリアルRS-232Cを3.3Vにレベル変換）を用意します。
- LPC1114FN28は110円程度で売っていますし、DIPなのでブレッドボード組みでも動かせます。
- LPC1114FN28へのファームウェア書き込みはUSB-serialからデータを流し込むだけなので、特別なライター冶具は不要です。

<!-- dummy comment line for breaking list -->

~

- まず、LPC1114FN28に書き込み用のファームウェアを焼きます。
- 続いて、Microchipのアプリケーションノート1388番のPIC32_Bootloaders_v2012_10_05.zip に含まれているPIC32UBL.exeを使用してLPC1114経由でPIC32MXにプログラムを書き込みます。

<!-- dummy comment line for breaking list -->

### （２.１）[シンセ・アンプラグド](http://d.hatena.ne.jp/pcm1723/20121230/1356849249) さんのLPC1114FN28ライターをMARY基板で使う。
- MARY基板を使う場合は、USB-serial変換器やDIP ICを配線する手間が省けます。
- が、MARY基板はDTR信号でリセットが掛かる仕様なので、PIC32UBL.exeを(DTR制御しないように改造して)リビルドする一手間が余計に掛かります。
    - ビルド済みのexeを[ここ](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/PIC32UBL.zip) に置いておきます。

<!-- dummy comment line for breaking list -->

### （３）FTDIのチップ FT232やFT2232D を使って書き込む。
- pic32progを改造することでそのようなことが可能です。
- 詳細は、以下の、すzさんのＨＰを参考にしてください。
    - [すzのAVR研究：FT232Rを PIC32MXライタに](http://suz-avr.sblo.jp/article/57754140.html) 

<!-- dummy comment line for breaking list -->

~
- オリジナルpic32progにはadapter-mpsse.cというソースが含まれていてFT2232DからJTAGピン経由で書き込めるようなプログラムの痕跡があるようですが、自分が試した限りでは、JTAG経由でうまくいきませんでした。
    - デバイスIDの取得まではOKですが、config fuseの読み出しやファーム書き込みは駄目なようです。
- OpenOCDはMIPSをサポートしているらしいので、普通にFT2232DからJTAGでデバッグ出来そうな感じではあります。が、PIC32MXのファームウェア書き込みまでサポートされているかどうかは分かりません（要確認）。

<!-- dummy comment line for breaking list -->

### （４）PicKit2を使って書き込む。
- PicKit2はPIC32MXに対応していませんが、[pic32prog](http://code.google.com/p/pic32prog/) というオープンソースのユーティリティを使用することで、PicKit2経由でもPIC32MXに書き込むことができます。
- ファームの読み出しやベリファイも可能です。

<!-- dummy comment line for breaking list -->

### （５）なんちゃってPicKit2を使って書き込む。
- PIC18F2550のチップを持っているならば、PicKit2互換の書き込み器を即席で作ることが出来ます。

<!-- dummy comment line for breaking list -->

以下にそのやり方を記します。


~

## PIC18F2550([pic18spx](pic18spx.md) 基板)を用いて、PIC32MX220に接続してみた。

**概要**
- PIC18F2550にPickit2ファームウェアを焼いて、即席Pickit2を作り、それを使ってpic32progを動作させた。

<!-- dummy comment line for breaking list -->

ハードウェア概観

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/pic32prog1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/pic32prog1.jpg) 

- 左基板： PIC18F2550(掟破りの3.3V動作)による[pic18spx](pic18spx.md) 基板（AVR/PIC18F/PIC24F/ARM JTAG　どれでもライター）
- 右基板： 今回制作したPIC32MX220F032B 基板

<!-- dummy comment line for breaking list -->

- PIC18F2550からPIC32MX220F032Bへの配線図

<!-- dummy comment line for breaking list -->

	PIC18F2550           PIC32MX220F032B
	RA0(pin2)-10k-Vdd
	RA1(pin3)-10k-Vdd
	RA2(pin4)------------PGD(pic32)
	RA3(pin5)------------PGC(pic32)
	                  (C)
	              |／---MCLR(pic32)
	RA5(pin7)-10k-|  
	           (B)|＼
	     2SC1815    ↓(E)
	                GND



ファームウェア
- [pic18spx](pic18spx.md) ブートローダーpic18bootを利用してブートする.
- microchipのサイトから、PicKit2の最新ファームHEXを入手する.
- それをテキストエディタで編集して、0x2000番地以前のデータ(0000-1fff)を削除しておく.(このエリアはPickit2のHIDブートローダーになっている)
- pic18bootでそのHEXを書き込む。

		D:&gt; pic18boot.exe -s2000 PK2V023200.hex
- PIC18F2550基板のBOOTスイッチ(ジャンパー)をOFFにしてUSBケーブルを再接続する。
    - Windows PCからPicKit2と認識されるのを確認しておく。

<!-- dummy comment line for breaking list -->

- pic32progのソースをコンパイルして実行する。

		PICkit2: invalid status = 0046.
- 本物のPickit2のアナログ系回路を全部省いてしまっているため、このエラーが出る。
- なので、pic32progのinvalid statusが出てreturn する処理をコメントアウトして再コンパイル。

<!-- dummy comment line for breaking list -->

	pic32prog ダウンロード:[[source.zip:https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/source.zip]]


~
- もう一度トライする。

<!-- dummy comment line for breaking list -->

こんな感じ。
	E:\pic32prog\src>pic32prog
	Programmer for Microchip PIC32 microcontrollers, Version 1.75
	   Copyright: (C) 2011-2012 Serge Vakulenko
	     Adapter: PICkit2 Version 2.32.0
	PICkit2: invalid status = 0046.
	   Processor: MX220F032B (id 04A00053)
	Flash memory: 32 kbytes
	 Boot memory: 3 kbytes
	Configuration:
	   DEVCFG0 = 7fffffee
	                    2 Debugger disabled
	                   08 Use PGC3/PGD3
	   DEVCFG1 = ff60ce5b
	                    3 Primary oscillator with PLL
	                  2   Primary oscillator: HS
	                 0    Peripheral bus clock: SYSCLK / 1
	                 4    Fail-safe clock monitor disable
	                 8    Clock switching disable
	                0     Watchdog postscale: 1/1
	   DEVCFG2 = fff979d9
	                    1 PLL divider: 1/2
	                   5  PLL multiplier: 20x
	                  1   USB PLL divider: 1/2
	                      Enable USB PLL
	                1     PLL postscaler: 1/2
	   DEVCFG3 = ffffffff
	                7     Assign irq priority 7 to shadow set
	              1       Ethernet MII enabled
	              2       Default Ethernet i/o pins
	              4       Default CAN i/o pins
	             4        USBID pin: controlled by USB
	             8        VBuson pin: controlled by USB
	
	E:\pic32prog\src>
- 一応デバイスIDとfuse bitの読み出しまで成功！

<!-- dummy comment line for breaking list -->

~

~

追記
- PIC32MX220F032B用に作られたLED点滅プログラムを書き込んで動作するのを確認しました。
- ~~Pinguino用のUSBブートローダーを動作させるには、まだ至っていません。~~ --- Pinguinoのブートローダーを書き込んで、スケッチの実行にも成功しました。

<!-- dummy comment line for breaking list -->



~
~
~
~
- - - -
## Pinguinoのgccコンパイラとフレームワークを使ってＬチカをやってみる。

**やってみました。**

- ダウンロード:[pled.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pled.zip) 

<!-- dummy comment line for breaking list -->

~
~

### 続編・・・
**CDCprintfとタイマー割り込みとＬチカの同時実行です**

ソースはかなり強引です。

- ダウンロード:[cdcint.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/cdcint.zip) 

<!-- dummy comment line for breaking list -->


~
- コツ： USBDeviceTasks()相当品をポーリングで呼び出す場合は、最近のPCだと125uS間隔で呼ばないとデバイスエラーが起きやすいようです。1mSだと不明なデバイスになって接続できませんでした。

<!-- dummy comment line for breaking list -->


**Serial Portを使用したprintfテスト**

スケッチのまんまです。
- ダウンロード:[serial.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/serial.zip) 

<!-- dummy comment line for breaking list -->


~

利点
- 制限付きのXC32コンパイラ(Microchip)ではなく
- Pinguinoのgccコンパイラを使用すれば、最適化オプションの自由な使用やmips16のコード出力なども可能になります。

<!-- dummy comment line for breaking list -->


弱点
- XC32で書かれたソースとの互換性に乏しいです。（主にMicrochipのヘッダーや#pragmaなどの使用が不可）
- そのかわり、Pinguinoスケッチのソースとの互換性を取ることができます。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## pic32progで（物理アドレス指定にしていない）fuseをちゃんと焼けるように改造する。
- pic32progは、1FC0_0000〜1FC0_xxxxの物理アドレス領域にあるboot flashとconfig fuseは正しく焼けますが、
- それが論理アドレス(BFC0_xxxxとか9FC0_xxxx)になっている場合はそのHEXレコードは完全無視されます。

<!-- dummy comment line for breaking list -->

この挙動を直すには、以下のように改造を施します。

- pic32prog.c: 260行目あたり

		if (record_type == 4) {
		    /* Extended address. */
		           if (bytes != 2) {
		               fprintf (stderr, _("%s: invalid HEX linear address record length\n"),
		                   filename);
		               exit (1);
		           }
		    high = data[0] &lt;&lt; 8 | data[1];
		#
		#if	1	// これを追加.
			high &amp;= 0x1fff;		// 9fc0_xxxx -&gt; 1fc0_xxxx
		#endif
		    continue;
		}
- 対処済みpic32prog.exeのダウンロード:[source.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/source.zip) 
- bootエリアとconfigエリアが物理アドレスでないHEXを扱うことが可能。
- devcfg0<bit1:0>が'10'のときに気を利かせて'11'にすることで、PinguinoのブートローダーHEXをそのまま書き込んで動作ＯＫ。
- 即席版PicKit2に対応。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## pic32progを使って、PinguinoX.3に付属のHIDブートローダーを焼くには？

	C:/PinguinoX.3/extra/bootloaders/32bit/PIC32-Pinguino_HIDBoot_MX220.hex
- これをそのままpic32progで焼いても、どういうわけか、うんともすんとも言いません。

<!-- dummy comment line for breaking list -->

**対策**
- HEXファイルの９行目を差し替えます。
- diff patch.hex PIC32-Pinguino_HIDBoot_MX220.hex

		9c9
		&lt; :040BFC00FFFFFF7F79
		---
		&gt; :040bfc00eeffff7f8a
- 差し替えたpatch.hexをpic32progで焼くと、MX220F032B(DIPパッケージ)でHID bootloaderが使えるようになります。
- 上記の対処済み[pic32prog.exe](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/source.zip) を使用する場合はHEXのパッチも不要です。

<!-- dummy comment line for breaking list -->

## Pinguinoブートローダーの使い方。
- RB15にLEDを、また、RB7を抵抗pullupして、GNDに落とすBOOT-SWを実装します。
- RESET-SWも近くに実装します。
- Pinguinoのスケッチを書き込みたいときは、BOOT-SWを押したまま、RESET-SWをぽちっと押します。
- するとRB15のLEDが点滅を始めるので、Pinguinoのスケッチ書き込みを実行します。

<!-- dummy comment line for breaking list -->


~
~
- - - -
## PinguinoのスケッチでprintfしたものをUSB経由のteratermで表示したい。

- 01.basic にある Printf.pdeをGUIでそのまま試すとサイズオーバーでlinkエラーになるようです。
- 対処としては、printfやprintlnを適度に削ってはビルドしてサイズが16K以内になるように調整します。
- うまくいくとCDCデバイスとしてteratermなどから会話的に文字表示が出来ます。

<!-- dummy comment line for breaking list -->

~
~
- - - -
## MIPS32のHEXファイルから逆アセンブルリストを作りたい

- こんな感じ。

		D:&gt; mips-objdump -m mips:isa32r2 -b ihex -D 1fc0.hex &gt;1fc0.asm
- DISRというMIPS用の逆アセンブラはコメント欄に2命令で生成された絶対アドレスを表示したり、遅延スロット命令に -s- 印を付けてくれて見やすいのだが・・・R4000に対応していないのであった（爆

<!-- dummy comment line for breaking list -->

~
~
- - - -
## HID-BOOTLOADERのソース？
- 探索中
- とりあえず、Mchipのサイトから、AN1388というUSBを含む多様なブートローダーのアプリケーションノートが入手できます。
- しかし、MX220F032Bには直接は対応していません。
- htmlのドキュメント内に、MX220への対応のさせ方が書かれているようです。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## HID-BOOTLOADERのソースを探す。
- AN1388はPinguinoのブートローダーと互換性がありません。
- 一番近いのはUBW32のサイトにある、HIDBoot.X.zipのようです。

<!-- dummy comment line for breaking list -->
### PIC32-Pinguino_HIDBoot_MX220.hex を再構築してみるテスト

- ~~とりあえず、コンパイル可能にして、HIDデバイス認識するところまで漕ぎ着けました。~~
- 完成しました。サイズは8kB(+BOOT3kB)以内に収まっています。
- 拡張機能(RAM/PORTの読み書き、任意アドレス実行)付です。

<!-- dummy comment line for breaking list -->


~
## HIDBoot.XをPinguino付属のmips32-gccで動かす。

- 同様のファームウェアをMicroChipのpic32-gccではなく、Pinguino付属のmips-gccでビルドします。
- '-mips16'オプションが有効です。
- こちらは、Mchipコンパイラを使用しないため、60日制限はありません。
- crt0.Sを改造してありますので、さらにコンパクトです。

<!-- dummy comment line for breaking list -->

- **ダウンロード:**[HIDBoot_Mips32gcc.X.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/HIDBoot_Mips32gcc.X.zip) 

<!-- dummy comment line for breaking list -->


## HIDBoot.Xを内蔵ＲＣ発振(FRC)で動かす。

- 8MHz水晶を実装しない選択を用意しました。
- 但しRA4に4MHz FRC出力を指定して、それをOSC1に入力する配線が必要になります。(RA4はGPIOとしては使えなくなります)代わりにOSC2が空きます。

<!-- dummy comment line for breaking list -->

- **ダウンロード:**[HIDBoot.X.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/HIDBoot.X.zip) 

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## PIC32MX220F032B のオーバークロッキングに挑戦

- Pinguino版のビルドで試してみました。
- config.c

		#define	CONFIG_FPLLIDIV			IDIV_2	/* 8 MHz -&gt;	4 MHz */
		#define	CONFIG_FPLLMUL			MUL_20	/* 4 MHz -&gt;	80 MHz */
		#define	CONFIG_FPLLODIV			ODIV_2	/* 80 MHz -&gt; 40	MHz	*/
- 水晶は8MHzを使用しています。
- 8MHz以外の水晶を使用する場合は、CONFIG_FPLLIDIVの分周比を調整して、PLLの入力クロックが常に4MHz

<!-- dummy comment line for breaking list -->
になるようにします。
- つまり、水晶の周波数は、4/8/12/16/20MHzのどれでも使えるように出来ます。

<!-- dummy comment line for breaking list -->

参考図

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic32pll.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic32pll.png) 

- そして、PLLの逓倍の倍率は、FPLLMULで設定します。

		#define	CONFIG_FPLLMUL			MUL_20	/* 4 MHz -&gt;	80 MHz */
- 20倍すると80MHzです。PLL出力は80MHzになります。

<!-- dummy comment line for breaking list -->

- それを２分周して、CPU Clockの40MHzを得ます。

		#define	CONFIG_FPLLODIV			ODIV_2	/* 80 MHz -&gt; 40	MHz	*/
- ~
- では、クロックアップを試みます。

		#define	CONFIG_FPLLMUL			MUL_16	/* 4 MHz -&gt;  64 MHz */
		#define	CONFIG_FPLLODIV			ODIV_1	/* 64 MHz -&gt; 64 MHz */
- これでHIDBoot_mips32gcc.X.zipをビルドして焼きます。

<!-- dummy comment line for breaking list -->

- 64MHzは余裕でした。

<!-- dummy comment line for breaking list -->

~

- では、一気に80MHz

		#define	CONFIG_FPLLMUL			MUL_20	/* 4 MHz -&gt;  80 MHz */
		#define	CONFIG_FPLLODIV			ODIV_1	/* 80 MHz -&gt; 80 MHz */
- これでも比較的動いています。USB-HIDデバイスとして動作して、hidmon32.exeを実行すると

<!-- dummy comment line for breaking list -->
内部メモリーをコンソールにダンプしてくれます。
- ＬＥＤの点滅速度は倍になっています。
- 信頼性は、まあ、無いでしょうね。**定格の倍速動作**なので

<!-- dummy comment line for breaking list -->

~

- さらに84MHz

		#define	CONFIG_FPLLMUL			MUL_21	/* 4 MHz -&gt;  84 MHz */
		#define	CONFIG_FPLLODIV			ODIV_1	/* 84 MHz -&gt; 84 MHz */
- 残念。LED点滅しませんでした。

<!-- dummy comment line for breaking list -->

- というわけで、DIPなのでたいしてクロックアップできないと思っていたら、意外と実力はあるみたいです。
- 電圧喝入れ！は、DIP内部にレギュレータを持っているので、無理でした。

<!-- dummy comment line for breaking list -->





~
~
~
- - - -
## PIC32MX220F032B のオーバークロッキングの次はアンダークロッキング

- アンダークロックはPLL出力に対して、分周比を大きく取ります。
- ODIVは1/2/4/8/16/32/64/256の中から選びます。

		#define	CONFIG_FPLLMUL			MUL_16	/* 4 MHz -&gt;  64 MHz */
		#define	CONFIG_FPLLODIV			ODIV_4	/* 64 MHz -&gt; 16 MHz */
- 16MHzが下限でした。
- これより低くするとLED点滅しませんし、USBデバイス認識もしないようです。
- USBスタックのどこかにDelayタイマーのような処理が埋まっていて、あまり遅くするとデバイス認識出来ないようになるのかもしれませんが、原因はまだ突き止めていません。

<!-- dummy comment line for breaking list -->

~
- HIDBoot.Xのかわりに:[pled.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pled.zip) で試したところ、

		#define	CONFIG_FPLLMUL			MUL_16	/* 4 MHz -&gt;  64 MHz */
		#define	CONFIG_FPLLODIV			ODIV_256	/* 64 MHz -&gt; 250 kHz */
- 250kHzクロックでも正しくLED点滅していましたので、USB周りの速度限界が存在する可能性があります。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## PIC32MX220F032Bの水晶を8MHz以外で使うには
- config.c の以下のdefineを同時に書き換えます。

		#define	CONFIG_FPLLIDIV			IDIV_2	/* 8 MHz -&gt;	4 MHz */
		#define	CONFIG_UPLLIDIV			IDIV_2	/* 8 MHz -&gt;	4 MHz */
- PLLの入力が常に4MHzになるように分周比を合わせます。

<!-- dummy comment line for breaking list -->
,水晶,分周比,FPLLIDIV,UPLLIDIV
,4MHz,1/1,IDIV_1,IDIV_1
,8MHz,1/2,IDIV_2,IDIV_2
,12MHz,1/3,IDIV_3,IDIV_3
,16MHz,1/4,IDIV_4,IDIV_4
,20MHz,1/5,IDIV_5,IDIV_5

~
~
~
- - - -
## PIC32MXで仮想シリアルポートを実装

- お約束のUSB-CDCデバイス化です。

<!-- dummy comment line for breaking list -->

もっと読む: [PIC32MX_USBCDC](PIC32MX_USBCDC.md) 

~
~
~

- - - -
## サンプルソース移植・改造のコツ

- MicroChipのサンプルをどうやって動かすかというテーマです。

<!-- dummy comment line for breaking list -->

もっと読む: [勝手に改蔵](PIC32MXcust.md) 

~
~
~

- - - -
## PIC32MXアーキテクチャーメモ

- リファレンスマニュアルへのリンクなど。

<!-- dummy comment line for breaking list -->

もっと読む: [PIC32MXアーキテクチャーメモ](PIC32memo.md) 

~
~
~


- - - -
## 参考リンク



Pinguino
- [http://wiki.pinguino.cc/index.php/Main_Page/ja](http://wiki.pinguino.cc/index.php/Main_Page/ja) 

<!-- dummy comment line for breaking list -->

マイコン風雲録 - 秋月でPIC32MX　28pin DIP版出る

- [http://mpu.seesaa.net/article/399685689.html](http://mpu.seesaa.net/article/399685689.html) 

<!-- dummy comment line for breaking list -->


すzのAVR研究: pic32progの改造


- [http://suz-avr.sblo.jp/article/57118986.html](http://suz-avr.sblo.jp/article/57118986.html)  - キャッシュ

<!-- dummy comment line for breaking list -->


<blockquote><p>
2012/07/18 &#8211; PIC32MX のライタは、純正の pickit3 を使うのが普通らしい。だが、pic32prog を使うと pickit2 が使えるばかりではなく、>FT2232 系の Olimex ARM-USB-TINY Jtag ケーブルが使えるらしい。ソースコードも公開されているので、改造すれば ...
</p></blockquote>




すzのAVR研究: PIC32MX220F032B メモ


- [http://suz-avr.sblo.jp/article/57033087.html](http://suz-avr.sblo.jp/article/57033087.html)  - キャッシュ

<!-- dummy comment line for breaking list -->


<blockquote><p>
2012/07/13 &#8211; pic32prog: PIC32MX220をPickit2/Pickkit3で書き込む. これは ... ソースコード を GPLv2 で配布しているではないか。MPSSE(Olimex ARM-USB-Tiny JTAG adapter) 用のコードもある。 ちょっと見てみよう。FT232RL に対応できるかも知れ ...
</p></blockquote>









すzのAVR研究: FT232Rを PIC32MXライタに


- [http://suz-avr.sblo.jp/article/57754140.html](http://suz-avr.sblo.jp/article/57754140.html)  - キャッシュ

<!-- dummy comment line for breaking list -->


電子ブロック工房 KM-Z80 mini
    - [http://kandk.cafe.coocan.jp/blog/mycom/index.php](http://kandk.cafe.coocan.jp/blog/mycom/index.php) 

<!-- dummy comment line for breaking list -->


PIC32MX220F032B　クリスタル無し、内蔵FRCでUSB 

- [http://minkara.carview.co.jp/userid/1110950/blog/28071764/](http://minkara.carview.co.jp/userid/1110950/blog/28071764/) 

