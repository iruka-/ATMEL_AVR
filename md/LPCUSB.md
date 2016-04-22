[ARM](ARM.md) 

## これはLPCUSBをinterface 2009-05付録基板で動かす実験です。

- LPCUSBはNXP LPC214xにしか対応していません。
- USBフレームワークのディレクトリ構成が、

		target/usb*.c , usb*.h
		target/exmple/*.c , *.h , Makefile
- となっていたものを

		project名/ ---+--- hwlib/    LPCxxxx固有のペリフェラルルーチン.
		              |
		              +--- usb/      USBフレームワーク.
- というふうに変更してあります。

<!-- dummy comment line for breaking list -->






## ダウンロード
~
ステータス：とりあえずビルドだけ通ります。

- [vcom.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/NXP/vcom.zip)  --- 仮想COM
- [hidjoy.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/NXP/hidjoy.zip)  --- HID Joyクラス
- [custom.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/NXP/custom.zip)  --- Custom(LibUSB-Win32) クラス
- [msc.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/NXP/msc.zip)  --- ストレージクラス

<!-- dummy comment line for breaking list -->



~
~
~
## example内容説明
内容はほぼexampleのままです。(vcomだけ少し変えました)

|サンプルアーカイブ名|機能|内容|
|vcom|仮想COM|仮想COMポートを生成してエコーバック。大文字を小文字に変換します。|
|hidjoy|HID Joystick|Joystickのつくりかけみたいなやつ。実際のJoystickとしては機能できないように見えます。カウンター値入りのダミーのレポートを返しているだけ（？）|
|custom|LibUSBカスタムデバイス|バルクin/out１組を持つ味気ないカスタムクラスです。メモリーのR/Wをやっているように見えますがホストPC側のソースは見当たらないので詳細不明です|
|msc|ブロックデバイス(ストレージクラス)|SPI接続のメモリーカードをマスストレージにしているように見えますが、例によってSPIポートがLPC214x決め撃ちアドレスなので多分そのままでは動作しないでしょう。|


寸評
- target-20070727.tar.gzが最終版で、更新が止まっている。
- LPC214x以外への対応が（ネットを見回しても）見当たらない。
- Ｃソース内でのポートアドレスの決め撃ちが多く、（ヘッダー分離していない）他の品種への対応に対して問題がある。
- ドキュメントが極端に少ない。doxygenの結果だけというのはつまらない。

<!-- dummy comment line for breaking list -->

- ホストＰＣ側のツールが存在しない。
- infは用意されているけれど、*.sysがない。結局C:/Windows/system32/drivers/を指定して、手持ちのsysを入れるはめになる。

<!-- dummy comment line for breaking list -->

- VendorIDが**FFFF**（！）

<!-- dummy comment line for breaking list -->




~
~

## ビルド環境の構築方法
- [WinARMビルド環境の構築方法](WinARM.md) 
- [ARMクロスコンパイラ構築（Linux）](2010-04#b817f226.md) 

<!-- dummy comment line for breaking list -->




## 作りかけのHIDmon

read more : [ARM7mon](ARM7mon.md) 

~


~
~
~
~

予定稿
- Interface2009.5基板でHIDmonを動かす。
- hid,cdcデバイスをWinARMでコンパイル

