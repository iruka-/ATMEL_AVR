[PIC32MX](PIC32MX.md) 

## PICKit3を使ってHEXファイルを書き込む

![http://akizukidenshi.com/img/goods/L/M-03608.jpg](http://akizukidenshi.com/img/goods/L/M-03608.jpg) 

## 注意点

秋月のHPには以下のFAQが用意されています。
- [http://akizukidenshi.com/catalog/faq/goodsfaq.aspx?goods=M-03608](http://akizukidenshi.com/catalog/faq/goodsfaq.aspx?goods=M-03608) 

<!-- dummy comment line for breaking list -->

~
~
~
## 私が嵌ったPICKit3の罠について

- 買ってきたばかりのPICKit3には、最新のファームが書かれていないか、あるいはブートローダーのみ生きているような状態でした。
- この状態でMPLAB-Xに付属のMPLAB IPEを使用してPICKit3に接続しようとしても、

		*****************************************************
		Connection Failed
- というエラーが出て、使用できない状態でした。
- Advancedモードに移行(パスワードは下に表示されています。)して、Manual Download Firmwareを選択しても、ファーム更新しているふり

<!-- dummy comment line for breaking list -->
だけして、結局、
	*****************************************************
	Connection Failed
- となってしまいました。

<!-- dummy comment line for breaking list -->

~
対策：
- PICKit3をUSB接続したあと、すかさず「connect」ボタンを押すと、ファームウェアの自動アップデート・フェーズに入る場合があります。
- ファームウェアが正しく書き込まれれば、それ以降は「connect」に失敗することはないようです。
- 一説によると、PICKit3のボタンを押したままUSBケーブルを挿すとよいという話もあります。これもいろいろ試したのですが、結局USB接続して一定時間(数秒)を過ぎてしまうと、ファームウェアの更新チェックは行われないか、更新できない状態に移行するようです。（まるで、昔のArduinoのブートローダーのようですね）

<!-- dummy comment line for breaking list -->

~
~
~
## このサイトに登録されているHEXファイルをPICKit3で焼く場合の注意点

- MPLAB IPEでは、KSEG1のアドレスに出力されたHEXレコードは丸無視するようです。

<!-- dummy comment line for breaking list -->

    - PIC32のメモリーマップです。
    - 物理割り当てされているエリアは 0000_0000 〜 2000_0000 の512MBです。
    - 物理割り当てされている512MBと全く同じものが KSEG0とKSEG1にもマップされます。
    - KSEG0とKSEG1の違いはキャッシュ無効/有効で分けられています。
    
    		#
    		FFFF_FFFF +---------------+
    		          |               |
    		          | Reserved      |
    		          |               |
    		C000_0000 +---------------+
    		          | KSEG1(論理)   | Cacheなし.
    		A000_0000 +---------------+
    		          | KSEG0(論理)   | Cacheあり.
    		8000_0000 +---------------+
    		          |               |
    		          | Reserved      |
    		          |               |
    		          |               |
    		          |               |
    		2000_0000 +---------------+
    		          | 物理メモリー  | ROM/RAM/PORT
    		0000_0000 +---------------+
- ですので、HEXファイルのレコードで、0xBXXX_XXXXで吐き出されたレコードのアドレスを0x9XXX_XXXXに書き換える必要があります。
- 簡便な方法としては、リンカースクリプト(*.ld)に指定してあるアドレスの0xb******* を0x9******* に全部変更します。

<!-- dummy comment line for breaking list -->

~
~
## HEXファイル変換ツール

hex2pickit3
- HEXファイルのアドレス指定がKSEG0(論理),KSEG1(論理)のいずれの場合にも、物理アドレスに置換します。
- MinGW gccあるいは Linux gccなどでコンパイルしてください。

<!-- dummy comment line for breaking list -->

- ダウンロード:[hex2pickit3.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/hex2pickit3.zip) 

