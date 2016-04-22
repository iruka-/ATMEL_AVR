[PIC18F2550]]　[[PIC18F4550]]　[[HIDmon-2550]]　[[HIDmon-14K50]]　[[2009-07]] 　[[2009-08]]　[[2010-04]] 　[[PIC日記](2009-05PIC.md)  
- - - -
目次



- - - -
### PIC18F用のHIDブートローダー

[http://akizukidenshi.com/catalog/g/gI-03031/](http://akizukidenshi.com/catalog/g/gI-03031/) 

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg) 
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ae18f2550.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ae18f2550.jpg) 



内容
- HIDmonとはHIDクラスのUSBデバイス内に実装されたマイコン用の簡易モニタのことです。
- マイコン内のROM,RAM,I/O PORTをUSB経由でPC上から自由に参照、変更するツールです。
    - I/O PORTの操作については、Gainerや[汎用ＵＳＢ-ＩＯ](汎用ＵＳＢ-ＩＯ.md) 的な応用が可能です。
    - RAM参照が可能なので、ユーザープログラム実行後のダンプを見ることも可能です。
    - bootloaderとして使用することも出来ます。
- HIDmonに関する詳細は[HIDmon88](HIDmon88.md) の項目をあわせてお読みください。

<!-- dummy comment line for breaking list -->


![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/hidmon1.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/hidmon1.png) 

- - - -
### 回路図 (18F14K50)[#lbab7f45]

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/sch14k50.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/sch14k50.png) 


注意：LVP書き込みしたPICを使用する場合はPGM端子(RC3,pin7)を5kΩ程度で**プルダウン**してください。
- また、その場合RC3端子は他の用途に使用できなくなります。

<!-- dummy comment line for breaking list -->

- 18F2550/4550用の回路図はこちら [circuit.txt](circuit.txt.md)  を参照してください

<!-- dummy comment line for breaking list -->

- - - -

### ファームウェアDownload:

[pic18boot-2010-0406.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic18boot-2010-0406.zip) 

[hidmon-14kの使い方](hidmon-14kの使い方.md) 


~
~
~
~
- - - -
## AVRユーザーのためのPIC18F始めま専科。

- もし、あなたがPICkit2やPICkit3などのPICライターをお持ちであれば、以下の情報は不要です。
- PICライターは持っていないけれど、AVRライター(HIDaspxを想定)なら持っている、という人が対象です。

<!-- dummy comment line for breaking list -->

~
- PIC18Fを始める場合の初期投資は、PIC18F14K50（１個２００円）と12MHzのクリスタルだけです。

<!-- dummy comment line for breaking list -->

- PIC書き込み器は、なんと、HIDaspxがそのまま使えます。 --->[PICspx](PICspx.md) の項を参照してください。
- 書き込むファームウェアは、このページの**ファームウェアDownload: **にある、pic18boot-2010-*.zipを展開して入手してください。

<!-- dummy comment line for breaking list -->


- 書き込みには気の遠くなるような時間（１０分くらい）が掛かりますが、最初の１回だけです。
- あとは、USBに接続して、同梱ツール picboot.exe を使ってアプリケーションを書き換え出来ます。

<!-- dummy comment line for breaking list -->

- - - -
- picbootが使えるようになったら、こんどは、[Ｃ言語でリライトしたHIDmon](pic18spx.md) を試してみてください。
- [pic18spx](pic18spx.md) にはHIDmon機能だけではなく、HIDaspxのようにAVRマイコンの書き込みや、別のPICマイコンへの書き込み機能があります。

<!-- dummy comment line for breaking list -->

~
- PIC18F14K50ではI/Oピン数やSRAM領域が不足する場合は、[PIC18F2550](PIC18F2550.md) や[PIC18F4550](PIC18F4550.md) をお勧めします。
- Ｃ言語からはほぼ上位互換で使用できます。
- [Ｃ言語でリライトしたHIDmon](pic18spx.md) も、上記PICの３品種のどれにも対応しています。

<!-- dummy comment line for breaking list -->

~
- Flash容量の制約をそれほど受けず比較的自由にHIDmonのファンクション追加やホスト機能拡張が出来ます。
- AVRに比べて性能は劣りますが、USBプロトコルエンジンがCPUと独立して動いてくれる（割り込みリソースを使わないで動作する）ので、他の割り込みとの競合もなくフルスピード(12Mbps)USBを使用することができます。

<!-- dummy comment line for breaking list -->

~
~
~
~

