[PIC18F2550](PIC18F2550.md) 

- - - -
### HIDboot

- AE-18F2550用のHID bootloader です。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ae18f2550.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ae18f2550.jpg) 

- - - -

### ファームウェアDownload:

[hidboot-0430.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/hidboot-0430.zip) 

- まだホスト側を制作していません。
- 急いで使用したい方はdiolanのbootloaderに含まれているfw_updateツール

<!-- dummy comment line for breaking list -->
を改造して（暗号処理を削除して）使ってみてください。

内容
- diolan製のPIC18F4455用 HID bootloader を 秋月AE-18F2550に移植したものです。
- MCHIP純正ブートローダーからも実行できる（XINST=0で動作する）ように改造済みです。
- ORG 番地を 0x000 , 0x800 から選択できます。
- 容量の関係上、暗号化機能を削除しています。

<!-- dummy comment line for breaking list -->



- - - -
### ToDo
- ~~ホストＰＣ側のツールを作る。~~
    - 実装の力点を[HIDmon-2550](HIDmon-2550.md) のほうに移しましたので、こちらのサポートはありません。あくまでもdiolanオリジナル互換のbootloaderを研究したい方用のfirmwareということに留めます。

<!-- dummy comment line for breaking list -->

- ~~HIDレポートの送り方をinterrupt転送からcontrol転送に差し替える。~~
    - これは実装予定はありません。

<!-- dummy comment line for breaking list -->

- 現在、**[[HIDmon-2550]]にbootloaderの機能を実装済み**です

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
### diolan製bootloaderが正常動作しない原因究明
- 結論

		 subfsr FSR2 , 3
- とか

		 addfsr FSR2 , 4
- が全く機能していません！
- 調べ方。

		usb_sm_prepare_next_setup_trf
- というアセンブラ関数があります。
- これは、USB END POINT 0 のLENGTHとBUFFER ADDRES などをセットアップする機能を持っています。
- 正しくセットアップされたなら LENGTH=0x40 BUFFER ADDRESS=0x0410 になる筈ですが、そうなっていないことが分かりました。

<!-- dummy comment line for breaking list -->

- さらにくわしく調べてみると、上記命令コードだけが無視されていることがわかりました。
- おそらくdiolan社では、上記命令が正しく動作する種類のチップが使われていると考えられます。
- 秋月のもの（あるいは18F2550）だけ動作しないなんて・・・**とても信じられません**

<!-- dummy comment line for breaking list -->

- ICEのようなデバッガがないので、テストルーチンを書いては実行し、知りたいメモリー内容をFlashの空き領域にコピーして再起動し、bootloaderで読み込むという方法でデバッグしました。

<!-- dummy comment line for breaking list -->

- と調べていったら、データシートに、以下の記述がありました。

<!-- dummy comment line for breaking list -->


	ADDFSR f, k       Add literal to FSR
	ADDULNK k         Add literal to FSR2 and return
	CALLW             Call subroutine using WREG
	MOVSF zs ,fd (source) to1st word Movez   (destination)2nd word Movez
	MOVSS zs ,zd  (source) to1st word  (destination)2nd word
	PUSHL k           Store literal at FSR2,  1 decrement FSR2
	SUBFSR f, k       Subtract literal from FSR
	SUBULNK k         Subtract literal from FSR2 and 2


	The additional features of the extended instruction set
	are disabled by default. To enable them, users must set
	the XINST configuration bit.

- がっくりです。

<!-- dummy comment line for breaking list -->

~
~
~
~

