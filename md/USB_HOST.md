[PIC32MX]]　[[PIC32でBluetooth](Bluetooth.md) 

## Tips
- USB_HOSTを試す場合は、デバッグメッセージ(print)をUART経由で拾う必要があります。
- [uartflash32](uartflash32.md) （シリアルローダー）を使用してプログラムを書き込むようにするか、PICKit2/3のようなもので書き込むほうが便利です。
- USB_Deviceなブートローダーでは、ROM容量が足りなかったり、USBの繋ぎ替えが頻繁に発生するので面倒です。

<!-- dummy comment line for breaking list -->



~


## PIC32MXをUSBホストにして、USBキーボードを繋いでみる。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg) 

    - MicroChipのUSBフレームワークの「Host - HID - Keyboard」を試してみます。

<!-- dummy comment line for breaking list -->


- 早速MX220F032Bに移植してみます。

<!-- dummy comment line for breaking list -->

~


移植途上版**ダウンロード**（とりあえず動きます）
- [usbhost.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/usbhost.zip) 

<!-- dummy comment line for breaking list -->

- とりあえず動作するようになりました。
- U1RX/U1TXにシリアルを繋いでデバッグ出来ます。
- コードサイズが24k程度ありますので、ブートローダーとの共存は出来ません。
- ファーム書き換えはPicKit2から行なったほうが効率が良いです。

<!-- dummy comment line for breaking list -->

- USBホストなので、いつもはPCに繋ぐUSBコネクタ(type-B)から一旦type-Aメスに変換（A-メス-メスの２股を用意します）してその先にUSBキーボードを繋ぎます。

<!-- dummy comment line for breaking list -->

- USBキーボードを繋いでキーを押すと、それに対応するHID Reportと押された文字をserial printします。

<!-- dummy comment line for breaking list -->

~
~
~
## PIC32MXをUSBホストにして、USBマウスを繋いでみる。


    - MicroChipのUSBフレームワークの「Host - HID - Mouse」を試してみます。

<!-- dummy comment line for breaking list -->


- 早速MX220F032Bに移植してみます。

<!-- dummy comment line for breaking list -->

~


**ダウンロード**（とりあえず動きます）
- [usbhost_mouse.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/usbhost_mouse.zip) 

<!-- dummy comment line for breaking list -->

- とりあえず動作するようになりました。
- U1RX/U1TXにシリアルを繋いでデバッグ出来ます。
- コードサイズが24k程度ありますので、ブートローダーとの共存は出来ません。
- ファーム書き換えはPicKit2から行なったほうが効率が良いです。

<!-- dummy comment line for breaking list -->

- USBホストなので、いつもはPCに繋ぐUSBコネクタ(type-B)から一旦type-Aメスに変換（A-メス-メスの２股を用意します）してその先にUSBマウスを繋ぎます。

<!-- dummy comment line for breaking list -->

- USBマウスを繋いで操作すると、それに対応するHID Reportをserial printします。

<!-- dummy comment line for breaking list -->

~
~
~




~
## Android ADKを試す。
- さらに調子にのって、Android ADK用のBasic Communication Demo - OpenAccessory Frameworkをビルドしてみます

<!-- dummy comment line for breaking list -->

移植途上版**ダウンロード**（動作テストは、まだです）
- [ADK.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/ADK.zip) 
- とりあえずビルドできるようになりました。

<!-- dummy comment line for breaking list -->

- Android端末には、[Microchip提供のADKサンプルアプリ](https://play.google.com/store/apps/developer?id=Microchip+Technology+Inc&amp;hl=ja) をGooglePlayストアからインストールしておきます。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/adkdemo.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/adkdemo.png) 

- Nexus-7(4.4KitKat)を繋いでこのファームウェアと接続すると、Android側は自動的にADKサンプルアプリが起動して、
- LED制御メッセージを受け取れることを確認しています。

<!-- dummy comment line for breaking list -->

- 動作はまだ不安定です。-->とりあえず動ているようです。

<!-- dummy comment line for breaking list -->


~
- 動作確認のためにはUARTへのprintfによる表示やデバッグが必須になります。

