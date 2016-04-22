[2009-08](2009-08.md) 　[Arduinoに挑戦](Arduinoに挑戦.md) 　[HIDmon-14K50](HIDmon-14K50.md) 　[mcc18](mcc18.md) 

- - - -
目次




- - - -
## Arduino400とは：
**PIC18F14K50 + ATmega8 ＝ Arduino400**

- ４００円（Chipのみの値段）でArduino互換機を制作してみました。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/arduino14k1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/arduino14k1.jpg) 
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/arduino14k2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/arduino14k2.jpg) 

- 写真はPIC 18F14K50とATmega8の混載基板（Arduino機能互換）
- Arduino単独での動作試験を行ったため、20P拡張シールドコネクタに無駄な配線(TxD/RxD)が残っています。

<!-- dummy comment line for breaking list -->

マイコンチップ以外の主要部品：
|部品名|単価|個数|価格|
|秋月ユニバーサルＣ基板 ||1|70円|
|USB-Bコネクタ||1|50円|
|ポリスイッチ|(6個入り300円)|1|50円|
|12MHz水晶||1|50円|
|16MHz水晶||1|50円|
|LED|20円|3|60円|
|Ｃ，Ｒ少々。|||100円くらい?|


たぶん合計で１０００円はいかないと思います。（800円くらい）

但し、PICライターとAVRライターが必要です。（借りてくるとか誰かに焼いてもらうとか・・・）
- --->最も廉価な[ＨＩＤａｓｐｘ](ＨＩＤａｓｐｘ.md) ライターにて、両方のチップにファームを書き込むことが可能です。[PICspx](PICspx.md) を参照してください。

<!-- dummy comment line for breaking list -->


### 特徴
- プログラム可能デバイスが２個もあって~~大変です~~面白いです。
- それぞれにブートローダーが焼かれていて、それぞれのファームをブートローダー経由で書き込めます。

<!-- dummy comment line for breaking list -->

- PIC側だけで、HIDmon/HIDboot としての使い道もあるので、Arduinoに飽きたらPICで各種USBデバイスを書いて遊べます。
- その気になれば、Arduino側のファームとPIC側にUSBデバイスを書いてその中に仕込んだシリアル通信を経由することで
- 任意の各種USBデバイス＋Arduinoで書いたファームの協調動作のようなアプリまで書くことが出来ます。

<!-- dummy comment line for breaking list -->

それぞれのチップの面倒見るのが大変ですが。

|マイコン|PIC 18F14K50|18F2550||AVR ATmega8|AVR ATmega168|
|Flash ROM容量|16kB((同じ処理を書いてコードサイズを比較するとPIC18はAVRの２倍〜３倍くらいのFlash（ガソリン）を消費する。つまり燃費の悪い車のようなもの。ガソリンがぶのみして、なおかつ遅い。ぐずでのろまなマイコンだ。))|32kB||8kB|16kB|
|SRAM容量|768Byte((うち256byteはUSB SIE側との共有（DualPort RAM）になっている))|2kB((うち1kByteはUSB SIE側との共有（DualPort RAM）になっている))||1kB|←|
|EEPROM容量|256Byte|←||512Byte|←|
|動作クロック|48MHz(実質12MHz)((１命令実行に最低４クロック必要なので実質12MHz))|←||16MHz|16MHz(最大20MHz)((20MHz動作させる場合は水晶を20MHzに換装して、ブートローダーのクロック設定（シリアル接続ボーレート計算値）も書き換えないといけません。))|
|処理系|[C18](mcc18.md) /[sdcc](sdcc.md) /MPASM|←||gcc(WinAVR)|←|
|8bit演算MIPS値|12MIPS((たしかに12M Instruction/秒であるが、何故か実際のコード効率はAVRの1/3〜1/10程度しかない。おまけにFlash使用量は無駄に多い。元々PICというのはProgrammable ICだったわけで、16pinとか18pinのロジックICもどきにプログラム能力を持たせたのが間違いの元だろう。))|←||16MIPS|最大20MIPS|
|参考価格（秋月調べ）|200円|400円||200円|300円|

どこが面白い？
- PICとAVRは犬猿の仲なのに、PICをUSB-Serial仲介役にしてArduino互換を構成していること。
- すなおにFT232RLを使わないのは配線が微細で難しいのと、素直すぎて面白くないから。

<!-- dummy comment line for breaking list -->

- 実はArduino互換機を作るのが目的ではなくて、PICのUSB-Serial Emulatorの性能評価テストを行うのが目的で、

<!-- dummy comment line for breaking list -->
AVR側はシリアル転送試験ツールとして使用する予定だったが、ついでにArduinoになるので、そっちも試そうと・・・。


- - - -
### 回路図

PIC側

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/sch14k50.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/sch14k50.png) 

AVR側

[http://arduino.cc/en/uploads/Main/Arduino-Pro-schematic.pdf](http://arduino.cc/en/uploads/Main/Arduino-Pro-schematic.pdf) 

PICとAVRの接続方法

	PIC TxD -------------> AVR RxD
	PIC RxD -------------> AVR TxD
	PIC RB6(DTR) ---||---> AVR Reset
	               0.1uF

RB6(DTR信号)を0.1uF経由でAVRのReset端子に接続します。

## DTRリセット問題
- DTRをAVRのResetにつなぐ時にキャパシタを挟む方法では、AVRのReset端子に瞬間的に１０Ｖ近い電圧が掛かったり

<!-- dummy comment line for breaking list -->
負電圧が掛かるという問題があります。（これは本家Arduinoの設計上の問題だと思います。）
- なので、DTRをOC出力（抵抗PullUP）にしてAVR Resetにつなぐべきではありますが、これにも落とし穴があります。
- ＰＣと接続中（通信中）は通常DTRはHレベル(負論理の場合Lレベル)ですが、ArduinoをＰＣと接続せずスタンドアロン

<!-- dummy comment line for breaking list -->
動作させるときはDTRアサートされずLレベル（負論理の場合Hレベル)となり、両立しません。（どちらかの状態でリセットが掛かりっぱなしとなります）
- FT232RLではDTR#端子の論理反転をチップに記憶させることが出来るようです。（たぶんArduinoでは正論理TTL？？？）
- 現状のPICのファームウェアをそのままコンパイルするとDTRは負論理(TTLレベル)に固定されています。

<!-- dummy comment line for breaking list -->

改善案としては、
- （１）キャパシタ0.1uFと直列に100Ω程度を挟む。そして、AVRのReset端子をダイオードクランプして高電圧や負電圧から守る。
- （２）PICファームを改良してDTRを正論理TTL OC出力にするか、もしくはArduino Reset専用の出力端子を新設する。その場合はAVR Reset端子に直結して抵抗プルアップにする。

<!-- dummy comment line for breaking list -->



~
~
~
- - - -
## ファームウェアダウンロード


PIC側

- [usbserial-uno.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/usbserial-uno.zip) 

<!-- dummy comment line for breaking list -->

- 旧バージョン
    - [mchip-cdc14k2.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/mchip-cdc14k2.zip) 

<!-- dummy comment line for breaking list -->

AVR側
- [http://www.arduino.cc/](http://www.arduino.cc/) 
- Arduino-0017をダウンロードして、ＰＣにインストールします。

<!-- dummy comment line for breaking list -->

ATmega8に焼く場合は以下のHEXを使用します。
	arduino-0017/hardware/bootloaders/atmega8/ATmegaBOOT.hex

ATmega168に焼く場合は以下のHEXを使用します。
	arduino-0017/hardware/bootloaders/atmega/ATmegaBOOT_168_ng.hex

- ATmega168を選択した場合、Arduino400ではなくなります。(Arduino500になります)
- （Flashエリアが２倍にグレードアップします。）

<!-- dummy comment line for breaking list -->

- PIC 18F14K50の代わりにPIC 18F2550を使用することも可能です。
- （その場合はArduino700になります。PIC側のFlashエリアも２倍になります。）

<!-- dummy comment line for breaking list -->

- PIC 18F2550用のUSB-シリアル変換ファームは特に用意していませんが、ソースファイルを元に作ること

<!-- dummy comment line for breaking list -->
が出来ます。（変更箇所は２、３箇所程度で済むと思います。）

~
PIC側はUSB-シリアル変換器として動作させていますが、[HIDmon-14K50](HIDmon-14K50.md) ブートローダー兼デバッグモニタ
をあらかじめ焼いておくとPIC側のファームアップデートやUSB付きPICの学習にも対応できるのでお勧めです。

~
~
~
~
- - - -
## 予定

- [ＨＩＤａｓｐ](ＨＩＤａｓｐ.md) から各種ブートローダーが書き込めるようにしたい。
    - [PICspx](PICspx.md) ツールを作成しましたのでご利用ください。
- ATmega8のブートローダーをもう少しブラッシュアップしたい。
- シリアルのボーレートを上げたい。(115200をデフォに)
- [Arduino2313](Arduino2313.md) 計画。

<!-- dummy comment line for breaking list -->

- [Piclearno](Piclearno.md) 計画。

