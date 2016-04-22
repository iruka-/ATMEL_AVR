[2014-08](2014-08.md) 

## PIC32MX で btstack を試す。

**[[btstack:https://code.google.com/p/btstack/]]というオープンソースのBluetoothスタック**を試します。


- https://code.google.com/p/btstack/issues/detail?id=407

<!-- dummy comment line for breaking list -->

- すでに、移植されていらっしゃる方がおられましたので、この稿では力弱く、再現テストのような追っかけになっております。

<!-- dummy comment line for breaking list -->

~
**目次**

~
~
~

- - - -
## Bluetooth（  btstack for PIC32　）のビルド環境を用意します

- 説明のため、以下のようなディレクトリ構成を用意します。(Windows)

<!-- dummy comment line for breaking list -->

	D:/work/pic32mx/btstack/

~
~
#### （１） btstack のソースファイルを上記ディレクトリの下に(svnで)取得します。
	D:/work/pic32mx/btstack/> svn checkout http://btstack.googlecode.com/svn/trunk/
- svnコマンドのインストール方法は[後述](Bluetooth#svn.md) します。

<!-- dummy comment line for breaking list -->

~

#### （２） PIC32MXに移植されたコードをtrunkの下にZIP展開します。
- [上記URL](https://code.google.com/p/btstack/issues/detail?id=407) から、PIC32.ZIPをダウンロードした後、

		D:/work/pic32mx/btstack/trunk/&gt; unzip PIC32.zip
- unzip コマンドのかわりにzipファイルをダブルクリックで仮展開して、trunkの下に、PIC32/ancs/ と来るようにコピーして構いません

<!-- dummy comment line for breaking list -->

~
~
#### （３） D:/work/ の下に、MLA(mchip Library) , xc32(compiler) , Pinguino をそれぞれ展開、もしくはインストールします。


,D:/work/microchip_solutions_v2013-06-15/,MLA, Microchip Library for Application Ver 2013-06-15 を展開。
,D:/work/microchip/xc32/v1.33/,XC32, XC32 Ver 1.33 をインストール、もしくはインストール済ディレクトリーから丸コピー。
,D:/work/pinguinoX.4-rev959/,Pinguino, Pinguino rev959 あるいはそれより新しいVerを展開。

- 違う場所にインストールしている場合は、丸コピーする方法を取るか、Makefileを書き換えるかどちらでもＯＫです。
- XC32コンパイラーは無償版（あるいはお試し版の期限切れ）でＯＫです。
- mips-gccはPinguino同梱のものを使用します。

<!-- dummy comment line for breaking list -->
~
~
#### （４） D:/work/pic32mx/btstack/trunk/PIC32/ancs/Makefile を書き換え

Makefile: の最初のところ
	MCS=../../../../../microchip_solutions_v2013-06-15
	MPBASE=../../../../../microchip/xc32/v1.33
	PINPATH=../../../../../pinguinoX.4-rev959
	PROGDIR=../../../pic32prog

- インストールディレクトリが異なる場合はそれに合わせてください。

<!-- dummy comment line for breaking list -->

~
#### （５） D:/work/pic32mx/btstack/trunk/PIC32/ancs/ で makeを実行。
- Pinguinoにはパスを通してください。

		D:/work/pic32mx/btstack/trunk/PIC32/ancs/&gt; PATH D:\work\pinguinoX.4-rev959\win32\p32\bin;%PATH%
		D:/work/pic32mx/btstack/trunk/PIC32/ancs/&gt; make
- ~
- ~
- ~
- ***トラブルシュート [#vf9e224b]
- Windowsでない場合は、 上記説明から「D:\」を抜いてください。( ~/workとかにします )

<!-- dummy comment line for breaking list -->

- Windows OS にPythonを入れていない場合は、ancs.gattからancs.h が作れないので、ここからコピーしてください。

<!-- dummy comment line for breaking list -->

ancs.h
	
	// ancs.h generated from ancs.gatt for BTstack
	
	// binary representation
	// attribute size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)
	
	#include <stdint.h>
	
	const uint8_t profile_data[] =
	{
	   // 0x0001 PRIMARY_SERVICE-GAP_SERVICE
	   0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18, 
	   // 0x0002 CHARACTERISTIC-GAP_DEVICE_NAME-READ
	   0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x02, 0x03, 0x00, 0x00, 0x2a, 
	   // 0x0003 VALUE-GAP_DEVICE_NAME-READ-'ANCS Demo'
	   0x11, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x2a, 0x41, 0x4e, 0x43, 0x53, 0x20, 0x44, 0x65, 0x6d, 0x6f, 
	   // 0x0004 CHARACTERISTIC-GAP_APPEARANCE-READ | WRITE | DYNAMIC
	   0x0d, 0x00, 0x02, 0x00, 0x04, 0x00, 0x03, 0x28, 0x0a, 0x05, 0x00, 0x01, 0x2a, 
	   // 0x0005 VALUE-GAP_APPEARANCE-READ | WRITE | DYNAMIC-''
	   0x08, 0x00, 0x0a, 0x01, 0x05, 0x00, 0x01, 0x2a, 
	// GAP Peripheral Privacy Flag
	   // 0x0006 CHARACTERISTIC-2A02-READ | WRITE | DYNAMIC
	   0x0d, 0x00, 0x02, 0x00, 0x06, 0x00, 0x03, 0x28, 0x0a, 0x07, 0x00, 0x02, 0x2a, 
	   // 0x0007 VALUE-2A02-READ | WRITE | DYNAMIC-'00'
	   0x09, 0x00, 0x0a, 0x01, 0x07, 0x00, 0x02, 0x2a, 0x00, 
	
	   // 0x0008 PRIMARY_SERVICE-GATT_SERVICE
	   0x0a, 0x00, 0x02, 0x00, 0x08, 0x00, 0x00, 0x28, 0x01, 0x18, 
	   // 0x0009 CHARACTERISTIC-GATT_SERVICE_CHANGED-READ
	   0x0d, 0x00, 0x02, 0x00, 0x09, 0x00, 0x03, 0x28, 0x02, 0x0a, 0x00, 0x05, 0x2a, 
	   // 0x000a VALUE-GATT_SERVICE_CHANGED-READ-''
	   0x08, 0x00, 0x02, 0x00, 0x0a, 0x00, 0x05, 0x2a, 
	   // END
	   0x00, 0x00, 
	}; // total size 74 bytes 
	
	
	//
	// list mapping between characteristics and handles
	//
	#define ATT_CHARACTERISTIC_GAP_DEVICE_NAME_01_VALUE_HANDLE 0x0003
	#define ATT_CHARACTERISTIC_GAP_APPEARANCE_01_VALUE_HANDLE 0x0005
	#define ATT_CHARACTERISTIC_2A02_01_VALUE_HANDLE 0x0007
	#define ATT_CHARACTERISTIC_GATT_SERVICE_CHANGED_01_VALUE_HANDLE 0x000a

~
~
~
- - - -
### ビルド成功したら、main32.hexが出来上がります。

- Firmwareサイズは約48kBになります。
- PIC32MX220のFlash容量32kBに収めることはほぼ不可能です。
- なので、今回はPIC32MX250を使用します。

<!-- dummy comment line for breaking list -->

~
~
~


~
~
~


- - - -
## SubVersion(svn)のインストールについて
- Windows7/8ではSlikSvnがお勧めです
    - https://www.sliksvn.com/en/download

<!-- dummy comment line for breaking list -->

- Windows OS のbit数 32bit/64bit に合うほうをDLします。
- 実行パスを通します。

<!-- dummy comment line for breaking list -->

~
- そして、実はsvnコマンドはsshコマンドを呼び出すので、sshも必要です。
- これは、MinGW ( [http://www.mingw.org](http://www.mingw.org)  )をインストール(installerをDLして実行)して、setupから、sshを入れます。

<!-- dummy comment line for breaking list -->

~

~
~
~


- - - -
## 動作テスト
- これから書きます。

