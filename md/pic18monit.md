[pic18spx](pic18spx.md) 

## ■■■ 概要 ■■■

	 これは、MicroChipが提供している PIC18 のUSBアプリケーションフレームワークに
	 含まれているUSB Generic device をもとにして作成した PIC18F専用の簡易モニタです。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg) 

## ■■■ 特徴 ■■■

- libUSBを使用しています。（このため、Windows側にデバイスドライバー組み込みが必要です）

<!-- dummy comment line for breaking list -->

- USBバルク転送を用いるため、HIDデバイスやCDCデバイスに比べてデータ転送が比較的高速です。

<!-- dummy comment line for breaking list -->

- libUSBを用いたため、LinuxやOSXへの移植が比較的容易です。(Linuxは対応済みです)

<!-- dummy comment line for breaking list -->

## ■■■ ディレクトリ構成 ■■■

	pic18monit --+-- firmware /        main-18F14K50.hex もしくは main-18F2550.hex
	             |                     を対応するPICに焼きます。
	             |
	             +-- driver_inf /      WindowsXP用のUSB Generic デバイス用 inf ファイル
	             |
	             +-- picmonit /        WindowsXP側のコマンドラインツール picmonit.exe
	             |
	             +-- libusb /          picmonit.exe をビルドする場合に使用するlibusb-win32
	                                  ライブラリです。



## ■■■ ライセンス ■■■

	MicroChip提供USBフレームワークの著作権は MicroChip に帰属されています。
	
	それを除いた部分、Windows側ユーティリティはGPLライセンスです。

	PIC18用逆アセンブラのソース はGNU gputilsに由来しています。



## ■■■ DOWNLOAD ■■■



- [pic18monitソースファイル、ＨＥＸ、inf一式](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic18monit.zip) 

<!-- dummy comment line for breaking list -->

~
~


## ■■■ 詳細 ■■■

（１）Windows上での使い方:

	D:> picmonit.exe      ・・・起動
	PIC> help             ・・・HELP表示
	PIC> q                ・・・終了


- - - -


- - - -
（２）ビルド環境

- Windows上のコマンドラインツールは MinGW-gcc を使用してビルドします。
- PIC18Fファームウェアは MicroChip mcc18コンパイラを使用してビルドします。
    - どちらもビルドには make を使用します。

<!-- dummy comment line for breaking list -->



- - - -
（３）カスタマイズ

- PIC18Fファームウェアにユーザー専用の機能を簡単に追加するには、 usercmd.c

<!-- dummy comment line for breaking list -->
を書き換える方法があります。

- picmonit.exe から user <arg> コマンドにて実行されます。

<!-- dummy comment line for breaking list -->



- - - -
（４）I/Oポート監視

	PIC> p  コマンドでI/Oポートの状態監視と変更が可能です。
	
例： port状態一覧
	PIC> p
	
例： LEDの点灯
	PIC> p latc 3
	
例： LEDの消灯
	PIC> p latc 0
	
	  PORT[ABC] LAT[ABC] TRIS[ABC] の意味はデータシートを確認してください。
	
	
- - - -
（５）モニタースクリプト
	PIC> プロンプト状態でのコマンド投入をバッチファイルで自動化できます。
	コマンドシーケンスを適当なテキストファイル(SCRIPT.TXT)に書き込んで、

	D:>  picmonit.exe -iSCRIPT.TXT

	を実行すると、一連のシーケンスを実行します。


- - - -
（６）アナログオシロ

	 PIC> graph analog　　コマンドによって、低速アナログオシロになります。



- - - -
（７）回路図

	     pic18spx互換です。


- - - -
（８）Linux版

	     ubuntu,debian などの場合 libusb0-dev （もしくはlibusb-dev ）パッケージが必要です。
	     firmware は DOS/Linux共通です。
	     picmonit は 再コンパイルしてください。
	     picmonit の実行はルート権限で行うか、あるいは udev で
	     該当USBデバイスのアクセス権限をユーザーに許可するようにします。

	     Linux版はグラフィック部分が使用できません。

- - - -
（９）その他

　    赤外線リモコン解析機能も（たぶん）使えるはずです。pic18spxのドキュメントを
　　　参照してください。




- - - -

