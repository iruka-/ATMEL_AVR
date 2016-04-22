[なんちゃってシリアル](なんちゃってシリアル.md)  ←設計編

- - - -
#### これは何ですか？
- SIZE(18){**ＡＴｔｉｎｙ２３１３で作るＵＳＢｔｏシリアル変換器です**}
- - - -
#### IgorPlugUSB ( AVR309 アプリケーションノート )とどこが違いますか？
**根底の思想は似たり寄ったりですが大半をＣで記述してあります**
- ＡＶＲＵＳＢライブラリを使用しています。
- AVR309では、Ｗｉｎｄｏｗｓ　ＤＤＫで作成された独自ドライバーソフトを使用しているようですが、AVR_termはlibusb(-win32) を使用します。
- お互いプロトコルの互換性はありません。

<!-- dummy comment line for breaking list -->

- - - -
#### どうしてＣＤＣクラスにしなかったのですか？
**２つの理由があります**
- （１）メモリー容量の関係でＡＴｔｉｎｙ２３１３には逆立ちしても入らない。
- （２）LowSpeed Bulkが通る環境は限られている。（ＷｉｎｄｏｗｓＸＰ使用時のみで、デバイスとの間にＨｉＳｐｅｅｄハブを挟まないこと）

<!-- dummy comment line for breaking list -->

- - - -
#### ダウンロード
とりあえず、エコーバックテストの範囲内ではうまくいっているようです。
- [upload/AVRterm.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/AVRterm.zip) 

<!-- dummy comment line for breaking list -->

#### 新バージョン：ダウンロード

- [upload/avrterm-2010-0104.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/avrterm-2010-0104.zip) 

<!-- dummy comment line for breaking list -->

新バージョンでは、HIDaspxハードウェア（回路図）上でそのまま動作します。

また、V-USBライブラリとlibusb.dllも更新しています。

- - - -
#### マニュアル
ＡＶＲ＿ｔｅｒｍ の使い方

（１）ATtiny2313 を用意します。

（２）src/main.hex を焼きます。

（３）ＡＶＲＵＳＢにある回路図通りに配線します。

（４）ＰＣに接続するとデバイスドライバーを要求しますので、
win32/windows-driver  にあるinfファイルを指定してインストールします。

（５）win32/term.exe を起動します。

（６）端末モードとして動作させる場合は、
	   AVR> t
脱出は CTRL + A です。

端末モードとして動作させ、ファイルをアップロードしたい場合は、
	   AVR> t filename
ファイルアップロード後も端末モードとして機能します。
脱出は CTRL + A です。


（７）端末モードの受信ログをファイルに保存したい場合は、
win32/term.exe の起動オプションに -l filename.log を与えます。

（８）ボーレートはデフォルトで9600ボーに設定されています。
設定を変える場合は
	   AVR> b 19200 
などとします。

（９）AVR_Monitに含まれるモニターコマンドはそのまま使用できます。
ただし、メモリー節約の関係上、EEPROMに対するアクセスは省かれています。

（１０）送受テスト

とりあえず以下のように操作してみます。
	   AVR> test 1000 10
リターンパケットをダンプします。

ATtiny2313の pin 2(RxD)とpin 3(TxD) をショートさせると、エコーバック
テストが出来ます。
- - - -
### 使用例
	AVR> ?
	* AVR_Monit Ver 0.1
	Command List
	 d  <ADDRESS1> <ADDRESS2>    Dump Memory(RAM)
	 dr <ADDRESS1> <ADDRESS2>    Dump Memory(EEPROM)
	 dp <ADDRESS1> <ADDRESS2>    Dump Memory(PGMEM)
	 e  <ADDRESS1> <DATA>        Edit Memory
	 f  <ADDRESS1> <ADDRESS2> <DATA> Fill Memory
	 p ?                         Print PortName-List
	 p .                         Print All Port (column format)
	 p *                         Print All Port (dump format)
	 p <PortName>                Print PortAddress and data
	 p <PortName> <DATA>         Write Data to PortName
	 sleep <n>                   sleep <n> mSec
	 bench <CNT> <PACKET_SIZE>   Bulk(or ctrl) Write Speed Test
	 b baudrate                  set baudrate
	 test count flag             RS232C send / recv test.
	 t                           RS232C Terminal mode.
	 t filename                  RS232C Upload file and Term mode.
	 q                           Quit to DOS
	AVR> b 19200
	cmdBaud(19200):UsbSetBaudRate : ubrr=0x4d,ucsrc=0xe TrueBaudRate=19230
	AVR> t
	Entering TerminalMode: Escape=^A
	・・・
	^A
	AVR> q
	Bye.
	C:>

- - - -
### 起動オプション
	Options
	 -t          ...  モニターコマンドを経ずに直接ターミナルモードに入る。
	 -t<baudrate>...  ボーレートを指定してターミナルモードに入る。
	 -l<logfile> ...  ログファイルを保存する。
	 -i<file>    ...  AVR_monitと互換のスクリプトファイルを読み込んで実行する。
	 -v          ...  USB接続詳細を表示する。

