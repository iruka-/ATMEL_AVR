[2009-08](https://github.com/iruka-/ATMEL_AVR/blob/master/web/log/2009-08.html)  　[PIC日記](https://github.com/iruka-/ATMEL_AVR/blob/master/web/log/2009-05PIC.html)  　[PIC18F2550](PIC18F2550.md) 　[PIC18F4550](PIC18F4550.md) 　[HIDmon-2550](HIDmon-2550.md) 　[HIDmon-14K50](HIDmon-14K50.md) 　[sdcc](sdcc.md) 



- - - -
### MicroChip C18 Compilerを使いこなす

- MicroChip C18 Compiler は評価版と称して事実上無償配布されています。((これは適切な表現ではありません。))((Lite版は無償無期限で使用できます。評価版は60日だけ使用できます。期限の切れた評価版はLite版と同等の機能に格下げになります。))((評価版はVerup版が出るたびに古いものをアン・インストールして、インストールしなおすことが出来るので、改版のタイミングから２ヶ月は最適化ありの状態で使用できるといった具合です。))((Lite版と、期限の切れた評価版は最適化なしになりコードサイズが２割以上大きくなりますが、))((それ以外の制限は無く普通に使用できます。そういった意味では無償配布です。))
- MicroChip が用意しているUSBアプリケーションフレームワークは、基本的に C18 Compilerでコンパイルできるように記述されています。
- しかし、普通のMakeが通りません。Makefileも整備されていないようです。

<!-- dummy comment line for breaking list -->


![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg) 

- MicroChip が用意しているUSBアプリケーションフレームワークを[sdcc](sdcc.md) に移植するのは茨の道です。MicroChipのバージョンアップに追いつけません。
- 現状、[sdcc](sdcc.md) はＣコンパイラとしての基本性能に問題があります。もちろんC18にも問題はあります。
    - 結局、どちらがましか、という議論になると、C18のほうがましであるという結論に達しました。

<!-- dummy comment line for breaking list -->

[Audinさんのサイトのまとめ記事](http://mpu.seesaa.net/article/1187467.html) が非常に参考になります。


- - - -

目次



- - - -

## とりあえず、Make環境を整備。


■ ビルド環境の構築の手引き

（１）

まず、WinAVR(GNU Make) をインストールします。
	 http://sourceforge.net/projects/winavr/
インストール先は、通常なら C:\WinAVR\ になります。

WinAVR/utils/bin には、make.exe 以外にもunixシェルでよく使用する ls や cat , rm
などといったお馴染みのツールのWin32バイナリーが含まれているので、非常に便利です。

（２） 

次に、mcc18(Compiler)をインストールします。Standard-Eval VersionでOKです。
	 http://www.microchip.com/

ＵＲＬはその都度変わると思いますが、ここです。
- [http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW006011](http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW006011) 
- 一番下にある |＞Documentation & Software を展開すると、ダウンロードリストが出ます。

<!-- dummy comment line for breaking list -->


インストール先は、通常なら ~~C:\mcc18\~~ になります。
- ---> C:\Microchip\mplabc18\v3.47\ あたりになります。

<!-- dummy comment line for breaking list -->

（３） 

続いて、MicroChipのサイトから、下記USBアプリケーションライブラリ、
	 MicrochipApplicationLibrariesv2009-07-10.zip
もしくはそれより新しいバージョンを入手してください。

ＵＲＬはその都度変わると思いますが、ここです。
- [http://www.microchip.com/pagehandler/en-us/devtools/mla/](http://www.microchip.com/pagehandler/en-us/devtools/mla/) 
- Legacyの隣にある、Archivesのタブを開くと古いバージョンが入手できます。

<!-- dummy comment line for breaking list -->

準備が出来たら、パスを通します。
	 ・パスの通し方:
	   PATH %PATH%;C:\mcc18\bin;C:\mcc18\mpasm;C:\WinAVR\utils\bin;
	               ~~~~~~~~~    ~~~~~~~~ 下線部はmcc18のインストール先に応じて読み替えてください.

適当なバッチファイルを用意して呼び出すか、あるいはWindowsのシステムプロパティ
	 -->詳細-->環境変数-->ユーザーの環境変数

PATH に C:\mcc18\bin;C:\mcc18\mpasm;C:\WinAVR\utils\bin
を記述します。

インストール時にパスを通してしまった場合は、上記確認のみで結構です。




**ダウンロード**

- [mchip-hidboot.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/mchip-hidboot.zip) 

<!-- dummy comment line for breaking list -->

- 内容は[千秋ゼミさんのサイト](http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2Fnews43) に登録されているHIDBoot-14k50.zip とほぼ同じです。（千秋さん、ありがとうございました。）

<!-- dummy comment line for breaking list -->

- Makefileを少し変更して汎用性を持たせてあります。

<!-- dummy comment line for breaking list -->

- ReadMe.txtドキュメントを追加しました。

<!-- dummy comment line for breaking list -->


**これでやっとC18が使えるようになります**

- 今まではC18でのビルド方法が全く不明（GUIは苦手です）だったのでどうすることも出来ませんでした。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## CDC Serial Emulatorをビルドできるようにしたい。
出来ました。

**ダウンロード**
- [mchip-cdc14k.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/mchip-cdc14k.zip) 

<!-- dummy comment line for breaking list -->

- PIC 18F14K50 をUSB-シリアル変換チップとして使用するためのファームウェアです。
- そのまま外部PICライターで焼いてよし、HIDmonブートローダーで書き込んでもＯＫです。

<!-- dummy comment line for breaking list -->

- コードはMicroChipオリジナルのままなので、19200以上のボーレートでの使用に若干不安があります。（未評価です。どなたか評価してみてください）

<!-- dummy comment line for breaking list -->

- USBのポーリングとRxDデータのポーリングを交互に呼んでいるだけの構造なので、USBのパケット処理中にRxDデータが２バイト以上来るとハードウェアのUSART FIFOが溢れます。

<!-- dummy comment line for breaking list -->

- 19200bpsは１秒に約2000文字のレートでシリアルデータが流れますので、２文字時間＝1mSすなわちUSBの１フレーム相当になります。USBバルク転送が１フレームに１回は出来るはずなので、19200bpsで溢れることはないでしょう。

<!-- dummy comment line for breaking list -->

- しかし、その上の57600bpsでは２文字時間＝0.3mSになるので、怪しいことになります。

<!-- dummy comment line for breaking list -->


- ~~RTS/CTS/DTR/DSRなどの端子のサポートがありません。~~
    - よく調べたらありました。こいつです。この行のコメントアウトを取ります。
    
    		usb_config.h:130:#define USB_CDC_SUPPORT_HARDWARE_FLOW_CONTROL
    - DTRはRB6に割り当てられていました。
    - ANSELHのゼロクリアは行われていました。
    - UART_DTRの制御コードは書かれていました。
    - しかし、RB6はHi-Zのままでした。
    - UART_TRISDTRとUART_TRISRTSを０(出力)にセットするコードがどこにもありません。

<!-- dummy comment line for breaking list -->


- ~~つまり、ハードウェア・フロー制御のサポートもありません。~~（接続先のAVRチップなどにもないので、不要と言えば不要です。）

<!-- dummy comment line for breaking list -->

    - あるにはあるのですが、MicroChipのソースのままでは機能しません。
    
    		usb_config.h:130:#define USB_CDC_SUPPORT_HARDWARE_FLOW_CONTROL
- を有効にしても、バグにより機能しません。
- CTS,DSRは見てないような・・・。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## -*-勝手に改蔵-*-勝手に改蔵-*-勝手に改蔵-*-
	Serial Emulator : DTR/RTS出力可能版 

**ダウンロード**
- [mchip-cdc14k2.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/mchip-cdc14k2.zip) 

<!-- dummy comment line for breaking list -->

DTR信号をRB6端子に出力します。
~
~
~
~
- - - -
## PICのボーレート計算式
	              48,000,000            12,000,000
	baud = ------------------------- = ------------
	        4 x ([SPBRG:SPBRGH] + 1)    (UBRR + 1)

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/baud.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/baud.png) 

- つまり、115200bpsでも分周レジスタは103であり、
- 230kbps,460kbps,920kbpsくらいまでは楽勝のように見える。

<!-- dummy comment line for breaking list -->

- UBRR（と勝手に命名）を11にしたときに丁度1Mbps,5で2Mbps,1で6Mbps（！）
- まだまだいけるじゃん。

<!-- dummy comment line for breaking list -->

- しかしSerialEmulatorのファームは115200以上はエラー扱いにしている。
- （つまり115200まではこのファームでいけると言うのか？？？）

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## Bootloader-Tips

USB付きPIC18F用のBootLoaderは、今のところ４種類程度存在します。
- (1)MicroChip純正Bootloader(C18)
- (2)MicroChip純正HID Bootloader(C18)
- (3)diolan社がGPLで公開しているHID Bootloader(MPASM)
- (4)このサイトで公開している [HIDmon-2550](HIDmon-2550.md) /[HIDmon-14K50](HIDmon-14K50.md) (MPASM)

<!-- dummy comment line for breaking list -->

この４者は(残念ながら)**互いにプロトコルの互換性がありません。**

- (1)のみ、Windowsへのドライバー組み込みが必要です。(唯一、バルク転送を使用します)
    - (1)以外は、ＨＩＤデバイスなのでWindowsへのドライバー組み込み不要です。

<!-- dummy comment line for breaking list -->

- (2)のみ、ブートローダーのサイズが４ｋＢになります。他は２ｋＢに収まっています。
- (3)はPIC-18F4455のみサポートで14K50などには対応していません。
    - また、転送プロトコルを暗号化する機能があります
- (4)は18F2550/18F4550/18F14K50の３種類をサポートしています。
    - bootloader機能以外に**PIC BIOS(printデバッグ)機能や、HIDmon（汎用USB-I/O）としても使用できます。**

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## Bootloader-Tips2

アプリを書くときは当然エントリーアドレスと割り込みベクターを使用すると思いますが、
以下のように工夫することで、ブートローダー使用と単独書き込み実行の両対応が可能です。

- エントリー

		ORG 0
		GOTO 0x800
- 割り込み

		ORG  8
		GOTO 0x808
		ORG  0x18
		GOTO 0x818
- 実際のプログラムコード

		0800: プログラムエントリー番地
		0808: 割り込み処理0008のエントリー
		0818: 割り込み処理0018のエントリー
		 ・・・・
		3fff: 最大ここまで(18F14k50).
- ブートローダーは0x0000〜0x800に常駐しています。
- ブートローダーがアプリを書き込むときは自分自身と重なるアドレスのデータを無視します。
- PICkit2などの外部ライターで書き込むときは全領域を書き込んでくれます。

<!-- dummy comment line for breaking list -->

実際には、割り込み発生時に１命令だけ無駄なジャンプが発生しますが、そのかわりに公開するＨＥＸファイル
は１種類で良いので混乱が少ないです。

ブートローダー(2)を使用する場合のみ、プログラムコード開始番地を0x1000〜に移動する必要があります。

~
~
~
~
- - - -
## 番外編: mcc18をLinuxで動かす。

あまりお勧めできない方法ですが・・・。

- [mcc18をLinux上で動かす。](https://github.com/iruka-/ATMEL_AVR/blob/master/web/log/2009-09.html#v0d65843) 

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## 参考リンク
(PICマイコンの小部屋)USB開発環境の構築
- [http://www.mars.dti.ne.jp/~m7030/pic_room/usb_tool/](http://www.mars.dti.ne.jp/~m7030/pic_room/usb_tool/) 

<!-- dummy comment line for breaking list -->


<!-- dummy comment line for breaking list -->

電気実験室
- [http://www.geocities.jp/ii2duck/personal/s0744.html](http://www.geocities.jp/ii2duck/personal/s0744.html) 

<!-- dummy comment line for breaking list -->
~
~
~
~
- - - -
## 予定

- 汎用libusbデバイスのフレームワークもビルドしたい。

