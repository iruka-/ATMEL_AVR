[2009-07]] 　[[2009-08]] 　[[PIC日記](2009-05PIC.md)  　[PIC18F2550](PIC18F2550.md) 　[PIC18F4550](PIC18F4550.md) 　[HIDmon-2550](HIDmon-2550.md) 

- - - -
目次



- - - -
### HIDmon for PIC18F14K50

[http://akizukidenshi.com/catalog/g/gI-03031/](http://akizukidenshi.com/catalog/g/gI-03031/) 

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg) 
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/hidmon1.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/hidmon1.png) 


内容
- HIDmonとはHIDクラスのUSBデバイス内に実装されたマイコン用の簡易モニタのことです。
- マイコン内のROM,RAM,I/O PORTをUSB経由でPC上から自由に参照、変更するツールです。
    - I/O PORTの操作については、Gainerや[汎用ＵＳＢ-ＩＯ](汎用ＵＳＢ-ＩＯ.md) 的な応用が可能です。
    - RAM参照が可能なので、ユーザープログラム実行後のダンプを見ることも可能です。
    - bootloaderとして使用することも出来ます。
- HIDmonに関する詳細は[HIDmon88](HIDmon88.md) の項目をあわせてお読みください。

<!-- dummy comment line for breaking list -->

- - - -
### 回路図

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/sch14k50.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/sch14k50.png) 

注意：LVP書き込みしたPICを使用する場合はPGM端子(RC3,pin7)を5kΩ程度で**プルダウン**してください。
- また、その場合RC3端子は他の用途に使用できなくなります。

<!-- dummy comment line for breaking list -->

- - - -

### ファームウェアDownload:

**新バージョンのご案内**
- これまでhidmon-2550とhidmon-14K50は別々のソースアーカイブで公開しておりましたが、ソース統合と高速化

<!-- dummy comment line for breaking list -->
を施したものを公開します。[HIDブートローダー](pic18boot.md) を参照してください。


旧バージョン：

[hidmon-14k08.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/hidmon-14k08.zip) 

[hidmon-14kの使い方](hidmon-14kの使い方.md) 




- 18F2550/4550ではXtalは20MHzで、それを５分周（4MHz)した後、PLLで48MHzにして利用していたと思います。
    - Xtalの分周比はfuse設定によって1〜5まで行けるので、4,8,12,16,20MHzのどれでも使えていました。

<!-- dummy comment line for breaking list -->

- 18F14K50ではPLLは x4固定で、入力に12MHzを与えない限り48MHzを取り出せないので、必然的にXtalは12MHz固定となります。
    - 勿論、水晶発振**子**ではなくて、水晶発振モジュール出力をPICに突っ込むことも出来ますが、その場合は

<!-- dummy comment line for breaking list -->
外部12MHzか、外部48MHzにして内部x4PLLをdisableするかの２択となります。
- 18F14K50の内部発振16MHzではどうやってもUSBが使えないような気がします。

<!-- dummy comment line for breaking list -->

- USB LowSpeed(1.5Mbps)を使う場合は、その４倍の6MHzのUSB Clockが必要になりますが、
    - その場合もXtalの選択は２択であり、6MHzを直接つなぐか、12MHzを内部２分周で使うかのどちらかです。
- もし6MHz Xtalを選択した場合、CPU Clockは x4PLLを通しても24MHzにしか上げられないので、
    - 結局12MHz Xtalを選択して、x4PLLでCPUを48MHz駆動し、USBモジュールに1/2の6MHzを与えるのが良いでしょう。

<!-- dummy comment line for breaking list -->

- USBを使う限り、内部発振の16MHzは使い道がありません。

<!-- dummy comment line for breaking list -->


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
- 書き込むファームウェアは、このページの**ファームウェアDownload: **にある、hidmon-14k*.zipを展開して入手してください。

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
- [Ｃ言語でリライトしたHIDmon](pic18spx.md) は、上記PICの３品種のどれにも対応しています。

<!-- dummy comment line for breaking list -->

~
- Flash容量の制約をそれほど受けず比較的自由にHIDmonのファンクション追加やホスト機能拡張が出来ます。
    - 現在のファームサイズは5kB程度なので18F14K50の場合でも9kBの空き領域が使えます。
- AVRに比べて性能は劣りますが、USBプロトコルエンジンがCPUと独立して動いてくれる（割り込みリソースを使わないで動作する）ので、他の割り込みとの競合もなくフルスピード(12Mbps)USBを使用することができます。

<!-- dummy comment line for breaking list -->

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
PIC16F84AなどのPIC18Fでないシリーズのチップに書き込みたい。

オレンジ電子さん作の「[Writer509](http://www.geocities.jp/orange_denshi/writer509.html) 」を18F14K50に移植された方がおられます。

- [http://matsutakousaku.blogspot.com/2011/01/pic_25.html](http://matsutakousaku.blogspot.com/2011/01/pic_25.html) 

<!-- dummy comment line for breaking list -->

- こちらも簡単な回路で高電圧書き込みのPICに書けるようです。
- 今ではPICkit2が充分安価に購入できますから自作以外の選択肢だってありです。

<!-- dummy comment line for breaking list -->

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
以下の情報は古い日記です。

~
- - - -
	*大変！ポートアドレスに互換性がありません 
- 真面目にデータシートを読んでいます。
- なんと0xf60〜0xfffのI/Oポートアドレスからはみ出しているポートがあるではないですか。
- あうあう。これでは動きません・・・（修正が必要です。）
- 0xf5xのポートを触るときは、BSRレジスタを0x0fに再設定するか、
- あるいはmovff命令でお茶を濁すしかなさそうです。

<!-- dummy comment line for breaking list -->

上記もろもろ修正してあります。

~

- fuse14k.inc にて LVP=ONになっていますが、気にしないでください。
    - （HVP書き込みライターをお持ちの方は、LVP=OFFのほうが良いと思います。）
    - LVP（５Ｖ単一書き込み）で書き込んで使用する場合は、PGM端子(Pin7)を5kΩ程度で**プルダウン**する必要があります。
    - また、PGM端子(RC3)はポートとしては使用できなくなります。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
	*picboot.exe の '-B'コマンドが役に立ちました。 
- 0番地オリジンのbootloaderから 800番地オリジンのbootloaderを書き込んで立ち上げます。
- 800番地オリジンのbootloaderを使って、0番地オリジンのbootloaderを更新することが出来ました。

		picboot.exe -B hexfiles/bootloader-0000.hex
- fuse的には特にプロテクトを掛けていませんが、picboot.exeは'-B'オプションを入れない限り、0〜7ff番地を書き換えることはありません。（保護しています）

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
	*PIC 18F14K50の使用上の注意。 
- 上記に書きましたが、ポートアドレスの一部が0xf60〜0xfffからはみ出しています。
- USB エンドポイントは８つしか持てません。(18F2550/4550では１６個)
- USB DUAL PORT RAMは256バイトしかありません。しかも18F2550/4550とはアドレスがずれています。(0x200〜0x2ff)
- RAMは512バイトしかありません(0x000〜0x1ff)
- ANSEL,ANSELHのRESET時初期値が'1'なので、ANSEL未設定時にデジタルポート(PORTCとか)を読むと常に'0'になります。
- ADCON1ポートの互換性がありません。各ビットの持つ意味も違います。

<!-- dummy comment line for breaking list -->

- フリーのCコンパイラsdccが18F14K50をサポートしていません。(gpasmはサポートしていました。)
    - サポートしていませんが、サンプルプログラムはsdccで強引に動かしました。

<!-- dummy comment line for breaking list -->

- sdccのsnapshotのページからＤＬ出来るコンパイラのほうでは18f14k50のサポートが含まれているようです。
    - [http://sdcc.sourceforge.net/snap.php](http://sdcc.sourceforge.net/snap.php) 
    - 一応動作確認いたしました。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## 今後の拡張予定
- ~~ブート対象プログラムの開始アドレスが何種類か存在する(0x800,0x1000,0x2000)問題に対処したい。~~
    - --->対応しました。

<!-- dummy comment line for breaking list -->

- ~~ドキュメントの整備。~~
    - --->回路図書きました。
- sdccはあきらめて、mcc18をサポートする。
    - 実はmcc18を使うのが面倒くさいので、sdccを使用中です。
    - sdccでLEDブリンクを書いてpicbootから書き込んで起動させるところまではＯＫ。
    - ~~putc()などが全然動作しない。何故？~~
        - ---> 今は動作しています。

<!-- dummy comment line for breaking list -->

- アナログ周りの機能追加
    - ぼちぼち

<!-- dummy comment line for breaking list -->

- Fuse(CONFIG)の書き換え機能
    - これがあるとLVPライターだけでも、やっていける。

<!-- dummy comment line for breaking list -->

- コマンドラインからのスクリプト起動。

<!-- dummy comment line for breaking list -->

- スクリプトの強化。
    - なんかいい組み込み言語ないかな。

<!-- dummy comment line for breaking list -->



~
~
~
~
- - - -
	*deBUG中 
- putc()などが全然動作しない理由の一つはbios_task()の先頭でチェックする、usb_initializedというフラグが壊れているから。
- そこは起動直後に問答無用でゼロクリアして、
- usb初期化されたら、0x55,0xaaという印をつける場所で、それ以外の用途では使用していません。
- bios_task()以外では利用していません。
- にもかかわらず、そこに0でもなし0x55,0xaaでもない、ある固有値が書き込まれています。

<!-- dummy comment line for breaking list -->

**謎すぎ!**

- 謎といえば他にもあって、0x200〜0x2ffは起動時にゼロクリアしているのですが、最後の１０バイトくらいに必ずごみが入っています。
- しかも、この値も固有値です。

<!-- dummy comment line for breaking list -->

- 最後の１０バイトくらいは、PICmonから初期化すると初期化できますが、手動リセット後にみると、また同じ固有値になります。

<!-- dummy comment line for breaking list -->

おおまかに推測すると、0x2c0〜0x2ff あたりは誰かがわざと壊しているとしか思えません。
- で、壊される領域に、usb_sm_ctrl_stateという大事な（？）変数がさしかかると、USBデバイス認識しなかったり、USB接続が切れたりするという感じです。

<!-- dummy comment line for breaking list -->

~

結論から言って、USB DualPort RAM (0x200〜0x2ff)の最後の0x30バイトあたりは破壊されやすいようです。
- HIDmonのワークを0x210〜0x23Fに移すと、何事もなかったように正しく動作します。
- 現在はUSBのinput packet bufferに割り当てて、ホストパケット受信後の短時間だけ信用するというやりかたで回避しています。

