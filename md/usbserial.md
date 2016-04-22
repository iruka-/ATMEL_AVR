* PIC 18Fシリーズを使ったUSB-シリアル変換器 [#da06a24a]

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg) 


元ねた
[mcc18](mcc18.md) 

**ダウンロード**
- [usbserial.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/usbserial.zip) 

<!-- dummy comment line for breaking list -->

**割り込み使用版ダウンロード**
- [usbserial_int.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/usbserial_int.zip) 

<!-- dummy comment line for breaking list -->

**senshuさんの差分を適応した版**
- [usbserial-2010-0816.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/usbserial-2010-0816.zip) 

<!-- dummy comment line for breaking list -->



mchipcdc-14k2.zipからの変更点
- PIC 18F14K50 / 18F2550 / 18F4550 の３品種に対応しました。
- セルフエコーバックのルーチンを入れてみました。
- シリアル転送のベンチマークが出来るようにしてみました。(w32term)

<!-- dummy comment line for breaking list -->

## 結論
- 専用ドライバーを用いずに、USB汎用のCDCデバイスクラスのままでは、それほど速度が出ないことが分かりました。

<!-- dummy comment line for breaking list -->

あくまでもターゲットデバイスがPIC 18Fの場合ですが、

- セルフエコーバックでUHCI直結の場合＝600Kbps程度が上限。
- セルフエコーバックでHubを挟んだ場合＝800Kbps程度が上限。
- Tx/Rx接続によるエコーバック＝200Kbps程度。
    - 但し、Tx/Rx接続の場合ProcessIO()関数内ではUSARTとのやりとりが１文字単位なので効率が悪いです。
    - USART送受信を割り込みで処理すれば多少は改善するかもしれません。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## おまけ：USBシリアルファームとpic18spxファームの共存について

- PIC 18F14K50などにはFlashメモリーが16kB乗っていますので、pic18spx簡易モニター兼ＰＩＣ/ＰＩＣ２４/ＡＶＲライターを全部詰め込むことも可能です。

<!-- dummy comment line for breaking list -->

- やり方は、まず、以下のusbシリアルファームを最初に焼いて、次にpic18spxを焼きます。
- 焼く方法はbootloader(picboot.exe)を使います。~~もしくは外部ライタのどちらでも構いません。~~
    - 外部ライタで焼く場合は２つのhexを適当なテキストエディタを使って切り貼りする必要があります。

<!-- dummy comment line for breaking list -->


- 後から焼いたほうが強いので、起動するのはpic18spxのほうです。

		C:picspx&gt; picmonitor.exe
		PIC&gt;
- そこで、usbシリアルを起動するために、以下のコマンドを実行します。

		PIC&gt; boot 2c00
		PIC&gt; q
		C:picspx&gt;
- この状態でUSBバスが一旦リセットされ、アドレス2c00から配置されたファームウェアに制御

<!-- dummy comment line for breaking list -->
が移っている（usbシリアル変換デバイスになっている）はずです。

- 同様にして、picmonitor.exeの状態から、bootloaderに制御を戻すには

		PIC&gt; boot 0
- で行うことが出来ます。（但しbootloaderはboot jumperを見ますのでboot 0を行うまえにjumperの設定が必要です）
- picboot.exe -r を使用してpicspxに制御を移している場合ならjumperそのままの状態でloaderに戻ります。

<!-- dummy comment line for breaking list -->




**usbシリアル:ダウンロード**（仮）
- [usbserial_2c00.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/usbserial_2c00.zip) 

<!-- dummy comment line for breaking list -->

**senshuさんの差分を適応した版**(上記アーカイブに差分usbserial-0407.diffを適用したものです。)
- [usbserial-2a00.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/usbserial-2a00.zip) 

<!-- dummy comment line for breaking list -->


**pic18spxダウンロード**（仮）
- [pic18spx-2010-0416.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic18spx-2010-0416.zip) 

