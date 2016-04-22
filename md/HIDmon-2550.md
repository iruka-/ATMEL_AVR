[PIC日記](https://github.com/iruka-/ATMEL_AVR/blob/master/web/log/2009-05PIC.html)  　[PIC18F2550](PIC18F2550.md) 　[PIC18F4550](PIC18F4550.md) 　[HIDmon-14K50](HIDmon-14K50.md) 

- - - -
### HIDmon for AE-18F2550

- AE-18F2550用のHIDmon です。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ae18f2550.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ae18f2550.jpg) 

内容
- HIDmonとはHIDクラスのUSBデバイス内に実装されたマイコン用の簡易モニタのことです。
- マイコン内のROM,RAM,I/O PORTをUSB経由でPC上から自由に参照、変更するツールです。
    - I/O PORTの操作については、Gainerや[汎用ＵＳＢ-ＩＯ](汎用ＵＳＢ-ＩＯ.md) 的な応用が可能です。
    - RAM参照が可能なので、ユーザープログラム実行後のダンプを見ることも可能です。
    - bootloaderとして使用することも出来ます。
- HIDmonに関する詳細は[HIDmon88](HIDmon88.md) の項目をあわせてお読みください。

<!-- dummy comment line for breaking list -->

- - - -

### ファームウェアDownload:

**新バージョンのご案内**
- これまでhidmon-2550とhidmon-14K50は別々のソースアーカイブで公開しておりましたが、ソース統合と高速化

<!-- dummy comment line for breaking list -->
を施したものを公開します。[HIDブートローダー](pic18boot.md) を参照してください。


旧バージョン：

[hidmon-0814.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/hidmon-0814.zip) 

[hidmon-14kの使い方](hidmon-14kの使い方.md)  -- 使い方は18F14K50用と同じです。

- 単独のブートローダーとして使用できるHEXと、他のブートローダーを使ってお試しが出来るHEXの両方が入っています。
- ホストＰＣ側のツールとして
    - HIDmon (picmon.exe) : 簡易モニタ
    - HIDboot(picboot.exe): ブートローダー書き込み＆実行ツール
    - とそれぞれのソースファイル(MinGW gcc用)が含まれています。

<!-- dummy comment line for breaking list -->

~
- - - -

### 変更履歴

- 2009-0814:'p.'コマンドの動作が間違っていたのを修正しました。
- 2009-0521:senshuさんのパッチを取り込みました。
- 2009-0514 2200: senshuさんのパッチを取り込みました。
- 2009-0514:      EEPROM に対するFill (fr adrs1 adrs2 data)がハングする問題を修正
- 2009-0514:      picboot.exe を更新。 -p:? オプションの有効化
- 2009-0513:      EEPROMのダンプが１バイトずれていたのを修正。
- 2009-0513:      コマンド番号を変更。バージョン表示を追加。
- 2009-0512 2000: test/ にデバッグ支援機能(vregdump)サンプルを追加しました。
- 2009-0512:      test/ にデバッグ支援機能(regdump)サンプルを追加しました。

<!-- dummy comment line for breaking list -->

~

- 2009-0511 2000: bios_exit()するときにputc()済みの文字を捨ててしまう問題を修正。
- 2009-0511: 逆アセンブル機能の細かいバグ修正

<!-- dummy comment line for breaking list -->

- 2009-0510: PICmon.exe に逆アセンブル機能を追加しました。ファームに変更はありません。

<!-- dummy comment line for breaking list -->

~

- 2009-0509 1300: メモリー節約。余白は３２６バイト。
- 2009-0509 1300: アプリ起動処理で USB停止処理がうまくいっていなかった。修正。
- 2009-0509 1300: コマンドの番号を整理。firm内での分岐方法を変更。

<!-- dummy comment line for breaking list -->

~

- 2009-0509 1200: メモリー節約。余白は２９６バイト。
- 2009-0509 1200: EEPROMに対する読み書きが出来なかったのを修正しました。
- 2009-0509 0900: アプリ単独起動にて puts などを使用した場合に起動出来ないバグを修正。

<!-- dummy comment line for breaking list -->

- - - -
### トラブルシュート

USBデバイスとして正常認識できない旨のWindows側メッセージが出る場合
- （１） VusbとGND間にパスコン(0.3uF程度)を入れ忘れていませんか？
- （２） brownout voltageが高すぎて、ブラウンアウトが多発していませんか？
    - その場合はLEDがデューティ50%ではない点滅を繰り返します。

<!-- dummy comment line for breaking list -->
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic2550borv.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic2550borv.png) 
    - USB +5V 電源とGND間に10uF程度の電解コンと0.1uF程度のパスコンを入れ忘れていれませんか？
    - それでも電圧が足りない（ホストPCからのUSB電圧が想定外に低い）場合はfuse.incの
    
    		BORV=1
    - を
    
    		BORV=2
    - 程度に変更してhexfileを作り直してみてください。
- あるいは、5V電源をUSB給電から切り離して、充分安定な外部5V電源を繋いで動作テストします。

<!-- dummy comment line for breaking list -->



- - - -
### ToDo

現在ToDoはありません。

~

- - - -

### 将来の拡張展望

これはToDoではありません。

~
- GNUpicのアセンブラ gpasm に対応させたい。 ---> たぶん、#define BDT_ADRH(x) (x+3) のようなマクロを手で展開すればOKではないかと思います。

<!-- dummy comment line for breaking list -->

- 逆アセンブラをホストツールに組み込み。 ---> たぶん簡単に出来ます。

<!-- dummy comment line for breaking list -->

- PGMEM (FlashROM) に対する１バイト単位の書き込み（パッチ当てともいう） ---> これで、ＨＥＸ打ちコード入力＆ハンドアセンブルをやれば、３０年前のTK80とかH68/TRのノスタルジに浸れます。

<!-- dummy comment line for breaking list -->


- 他のデバイスクラス(CDC,HID-key,HID-mouse)の雛形として使いたい。

<!-- dummy comment line for breaking list -->

- オンチップデバッガ
    - 夢ですな。

<!-- dummy comment line for breaking list -->

## 以下は現在、仮実装を行っています。

test/ディレクトリに、下記の機能を利用したサンプルを用意しています。

- sdccと共存できる形のバイナリーにして、BIOS的に使いたい。 ---> そうするとsdccで書く部分は純粋にアプリのみになり、USBドライバーがASMという棲み分けが可能です。

<!-- dummy comment line for breaking list -->

- NEC78Kのトラ技BIOS的な機能を追加
    - これは、USB周りのプログラミングは毎回面倒なので、getchar,putchar,やputs的なものをBIOS側で面倒見ようというものです。
    - つまり、0番地〜0x800番地のブートローダーのどこかにエントリーテーブル(jmp命令がいくつか並んだ箇所)を用意して、そこをＣ言語から呼べばUSB経由でホストＰＣに対してgetcharやputchar,puts的なことが出来るようにするというものです。
    - ただし、現状のHIDmon(というかUSBフレームワーク側)は、USB処理のステートマシン部分をmain_loop（ビジーループ）内で常時callする必要があるので、アプリ側は多少制約を受けると思います。
    - が、それでも、毎回USB周りを記述するよりずっと楽になるでしょう。
    - Arduinoでも init() と loop() を記述するので、これと同じmain()関数のフレームワークを用意してあげて、loop()呼び出しの元のビジーループにUSBステートマシン呼び出しを挟んでおくというのも手ではあります。

<!-- dummy comment line for breaking list -->



~
- - - -
他の研究テーマ
- 64byteを越えるサイズのパケットはPIC上で正しく分割転送出来るのか？
- その場合、速度はどの程度向上するのか？
- ping pongバッファを使うメリットは？

<!-- dummy comment line for breaking list -->

このPICはVID,PID,そして挙動を他のUSBデバイスに合わせることで「なりすまし」を行うことが出来ます。
- 簡単に言えば、HID-keyboardやHID-mouseなどであれば、VID,PIDをそのまま手持ちのデバイスからコピーしてきても、ＰＣホストから見たらまったく区別がつかないデバイスが作れるということです。

<!-- dummy comment line for breaking list -->

- AVRチップ用のHIDmonファームウェアと同じ挙動をするようにして（現時点でもほぼ同じ挙動ですが）VID.PIDまで同じにすればホストＰＣ側ツールを開発することなくHIDmonを使用することもある程度可能です。

