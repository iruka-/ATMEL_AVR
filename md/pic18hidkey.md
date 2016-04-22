*PIC18Fを使ったUSBキーボード変換器の作成 [#ba242054]

~
工事中
~

元ねた
[mcc18](mcc18.md) 　[usbserial](usbserial.md) 


- - - -

**ダウンロード**（仮）
- [hidkey.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/hidkey.zip) 

<!-- dummy comment line for breaking list -->

現在のところ、
- [Microchip_Application_Libraries_2009_1118_Installer](http://www.microchip.com/microchip.www.securesoftwarelist/whatsnewsoftware.aspx?lang=en) の~~USBDevice-HID-Keyboard をそのままMakefileが通るように配置しただけのものです。~~

<!-- dummy comment line for breaking list -->

- 不完全ですが、PS/2 Keyboard to USB 変換アダプタとして一応機能するようになっています。

<!-- dummy comment line for breaking list -->

- - - -
元のサンプルにあった keyboard.c の動作内容は、以下のように、かなり投げ槍っぽいです。

- sw3 (HardwareProfile.h 経由で各マイコンごとに定義されているポート) が押されたら、キー番号 key をホストＰＣに送信し、key を +1 する。
- 変数 keyの初期値は4 で、40になったらまた4に戻す。

<!-- dummy comment line for breaking list -->

- - - -
## HID-Keyboardで使用されているHID Reportはどんなものなのか？

- １回あたり8バイトのデータ列になっている。
- 先頭バイトは「GUI,ALT,SHIFT,CTRL」 キーの押された状態を4bitであらわしたもの（押されていれば１）
- さらにその上位4bitは、キーボード右側にある、同名のシフト系キーの押された状態（並びは同じ）

<!-- dummy comment line for breaking list -->

- ２バイト目は使用しない。
- ３〜８バイト目は、押されているキーの固有番号が最高６個まで（同時押しに対応するため）詰め込まれて送信される。
- 押されていない、あるいは空の箱には０がセットされる。

<!-- dummy comment line for breaking list -->

- 各キーの固有番号についてはusb.orgのマニュアルを参照すること。

<!-- dummy comment line for breaking list -->



- - - -
## おまけ

- ついでなので hid-mouse , hid-joy , hid-generic , hid-bootloader も用意しました。

<!-- dummy comment line for breaking list -->

**ダウンロード**（仮）
- [hidmouse.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/hidmouse.zip) 

<!-- dummy comment line for breaking list -->

- [hidjoy.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/hidjoy.zip) 

<!-- dummy comment line for breaking list -->

- [hidgeneric.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/hidgeneric.zip) 

<!-- dummy comment line for breaking list -->

- [hidbootloader.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/hidbootloader.zip) 

<!-- dummy comment line for breaking list -->

hidbootloaderに関しては、とりあえずビルドが通るようになっているだけで、メモリー配置等を修正していませんので
HIDデバイスとしての動作はすると思いますがブートローダーとしての実用性はありません。

- - - -
## ApplicationLibrariesをMakefileでビルドするやりかた。



（例えば）ここのhidgeneric.zipを元にします。
- hidgeneric.zipを展開すると、Makefile , リンカスクリプト , main.c , fuse*.h などは

<!-- dummy comment line for breaking list -->
配置済みなので、残りのファイルの取得方法を書きます。

- (VerUp等で)必要なら、include/ , usb/ ディレクトリを元のApplicationLibrariesを展開した場所から

<!-- dummy comment line for breaking list -->
コピーします。
	C:> xcopy /s AMYDRIVE:\ApplicationLibraries\Microchip\Include include
	C:> xcopy /s AMYDRIVE:\ApplicationLibraries\Microchip\USB     usb
かなり不要なファイルもコピーされますので、あとで掃除します。

    - 不足ファイルや更新ファイルがない場合はコピー作業は不要です。

<!-- dummy comment line for breaking list -->

- ApplicationLibraries\USBDevice-HID-CustomDemos\ など、それぞれのファームソースがある場所から、 usb_config.h とか mouse.cのような元ソースを拾ってきます。
- main.cという名称だと、こちらのフレームワークのmain.cと被るので、適当にリネームします。
- Makefileを編集して、ターゲット名(hexファイルのファイル名)と、必要なソースファイルのリストを書き換えます。
- (例えば)mouse.c だったら、その中にある fuse設定をばっさりと削除します（ソースが見にくいので）
- ついでに、main()関数もmain.cにすでに記述しているので削除します。
- 割り込みベクターも特に何もしていなければ単に削除します。何かしているときはmain.c側に同じ変更を入れておきます。

<!-- dummy comment line for breaking list -->

だいたいこんな感じです。
- 最初からMakefileを用意してくれていれば、苦労することはないのに・・・。

<!-- dummy comment line for breaking list -->

~
~
~
~
~
- - - -
## 今後の課題
- 6 keyまでのロールオーバーの処理
- (PS/2コードで)E0が前置される拡張キー（のUSBコードへの変換）をちゃんとサポートする。
- リピート間隔の設定を有効にする。
- LEDインジケータを有効にする。
- 18F14K50への対応。
- 日本語キーボードへの対応。

