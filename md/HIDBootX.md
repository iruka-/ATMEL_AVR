関連リンク　[PIC32MX]]　[[Pinguinoで遊ぼう](Pinguino.md) 　[ブートローダーを作る](HIDBootX.md) 　[USB仮想シリアル](PIC32MX_USBCDC.md) 　[USBカスタムデバイス](USB_CUSTOM.md) 　[USBホスト](USB_HOST.md) 　[2013-02](2013-02.md) 　[2013-03](2013-03.md) 



- - - -
## ブートローダーって、何？

- PICマイコンに最初にプログラムを書き込むには、必ずPICライター(PicKit2/3)が必要です。
- しかし、あらかじめ小さなプログラムを書き込んでおいて、そのプログラムの助けにより、残った領域に別のプログラムを転送して実行する、ということが可能だとしたら、２回目以降の開発ではPICライターが不要になるというメリットがあります。

<!-- dummy comment line for breaking list -->

- そのような手助けをする（比較的）小さなプログラムのことを**ブートローダー**と呼んでいます。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg) 


- - - -
## 目次

- - - -
## ブートローダーのメリット/デメリット
- 先述のとおり、PICマイコンにブートローダーをあらかじめ書き込んでおけば、PicKit2/3が無くてもプログラムを開発したり、実行したり出来るのがメリットです。

<!-- dummy comment line for breaking list -->

- しかし、ブートローダーが常駐している分だけ、使用可能なFlashメモリーが少なくなる、というデメリットが出てきます。
- なので、ブートローダーは極力小さいほうが望ましいです。

<!-- dummy comment line for breaking list -->

- PICマイコンにおいては、Config設定とよばれる、チップの初期設定ビットが32bit x 4だけ存在していて、その設定が

<!-- dummy comment line for breaking list -->
正しくないとうまくマイコンが動作しないという問題がありますが、ブートローダーを経由した起動では、Config設定を全く気にすることなく起動できますので、Config設定トラブルに会わずに済む、というメリットもあります。

~
ArduinoやPinguinoのようなお手軽開発環境は、ブートローダー使用が前提になっていることが多いです。
- そのブートローダーの出来不出来が、作業効率や、ひいては開発の楽しさに大きく影響します。

<!-- dummy comment line for breaking list -->

~
~
- - - -
## PIC32MX220用のブートローダー

,名前,説明
,AN1388,アプリケーション・ノート1388番のサンプルプログラムとしてMicroChip社が提供している。USB - HIDデバイス以外にSerial、TCP/IP、sdcardなど、さまざまなデバイスからブートするようにカスタマイズできる。ソースファイルなどはMicroChipのサイトからダウンロード可能。
,旧HID BOOTLOADER,過去にMicroChip社が提供していたBOOTLOADER。PIC18F/PIC24F用。PIC32MX用が今も入手できるかどうかは不明。
,HIDBoot.X,[UBW32](http://www.schmalzhaus.com/UBW32/) 用のブートローダー。
,HIDBoot.X改,このサイトで使用しているHIDBoot.XをPIC32MX220 F032Bチップ用に改造してシュリンクしたもの。[PIC32MX](PIC32MX.md) を参照。

~
~
~

## ブートローダーのための開発環境を用意する。
まず、コンパイラを入手するところから始めます。
#### （１）[Pinguino4.X](http://wiki.pinguino.cc/index.php/Main_Page/ja)
    - ダウンロード: [http://code.google.com/p/pinguino32/downloads/list](http://code.google.com/p/pinguino32/downloads/list) 
    - 安定版のPinguino-X4-EasyPack-Win32-Rev999-v01-2014-04.7z をダウンロードして D:\x4-easy-rev999\ に展開してください。

<!-- dummy comment line for breaking list -->

#### （２）MicroChip MPLAB
    - 紛らわしいのですが、新しいほうの**MPLABXは駄目です**
    - [MicroChipのサイト](http://www.microchip.com/) から、[MPLAB_IDE_8_89.zip](http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&amp;nodeId=1406&amp;dDocName=en019469&amp;part=SW007002) を入手してインストールします。

<!-- dummy comment line for breaking list -->

    - インストール時にPIC32用のコンパイラも同時に選択してインストールするようにします。

<!-- dummy comment line for breaking list -->

#### （３）MicroChip USBフレームワーク（アプリケーションライブラリ）
- Microchip Libraries for Applications v2013-06-15 Windows を入手してインストールします。

<!-- dummy comment line for breaking list -->

[このあたり](http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&amp;nodeId=2680&amp;dDocName=en547784) からmicrochip-application-libraries-v2013-06-15-windows-installer.exeを入手してインストールします。




~
## ブートローダーのソースを入手する。
- 元ネタはUBW32のHIDBoot.Xですが、MX220用に改造したものを用意しています。

<!-- dummy comment line for breaking list -->

- **ダウンロード:**[HIDBoot_Mips32gcc.X.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/HIDBoot_Mips32gcc.X.zip) 

<!-- dummy comment line for breaking list -->


~
## ブートローダーをコンパイルしてみる。
- 上記アーカイブはすでにPinguino用gccでビルド出来るようにしています。
- Pinguino用gccに実行パスを通します。(setenv.bat)

		PATH C:\PinguinoX.3\win32\p32\bin;%PATH%
- Makefileに記述されているMPLABのディレクトリを各自環境に合わせます。

		# FIXME!
		#   MPLAB C32 Suite Dir ( IMPORTANT! --&gt;  NOT MPLAB-X )
		MPLAB	= C:\Program files\MPLAB\MPLAB C32 Suite
- MicroChipのUSBフレームワークのソースが、HIDBoot_Mips32gcc.Xと並存する位置にくるようにします。

<!-- dummy comment line for breaking list -->

	D:\MyWorkingDir\HIDBoot_Mips32gcc.X\ ------ HIDブートローダーのソース
	D:\MyWorkingDir\MicroChip\ ------Microchip Libraries for Applications v2012-10-15 Windowsの中のMicroChipディレクトリをここに配置。


<!-- dummy comment line for breaking list -->

~
## ブートローダーをチップに書き込む。
- \HIDBoot_Mips32gcc.X\ ディレクトリでmakeした後、w.batを実行。

<!-- dummy comment line for breaking list -->

~
~
## ブートローダーを使ってPIC32MXの中を覗いてみる。
- HIDBoot_Mips32gcc.X のファームウェアにはPIC32MXのメモリーを読み書きするコマンドが含まれています。
- これを利用すると、PIC32MXの中を簡単に覗くことができます。
- やってみましょう。

<!-- dummy comment line for breaking list -->

- HIDBoot_Mips32gcc.X/hidmon32/hidmon32.exe を起動します。

<!-- dummy comment line for breaking list -->

	E:\HIDBoot_Mips32gcc.X\hidmon32>hidmon32.exe
	USB HID device found: 24576 bytes free
	MIPS> d a0000000
	a0000000 00 00 00 00 01 00 00 00  04 03 09 04 e4 16 00 9d
	a0000010 20 04 00 a0 30 04 00 a0  00 00 00 a0 00 00 db 13
	a0000020 01 01 00 00 ff ff ff ff  00 80 00 1d 20 00 00 00
	a0000030 38 00 00 a0 00 00 00 00  01 01 07 05 01 03 40 00
	a0000040 00 00 00 00 20 00 00 00  00 01 00 00 10 04 00 a0
	a0000050 38 04 00 a0 01 00 00 00  01 00 00 00 01 00 00 00
	a0000060 00 00 00 00 20 04 00 a0  00 00 00 00 08 04 00 a0
	a0000070 00 00 00 00 00 00 00 00  80 06 00 02 00 00 29 00
	a0000080 00 00 02 00 00 04 00 a0  01 00 00 00 00 00 00 00
	a0000090 14 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000a0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000b0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000c0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000d0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000e0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	a00000f0 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
	MIPS> l bfc00000
	bfc00000 3c1a9fc0       lui     k0,0x9fc0
	bfc00004 275a0010       addiu   k0,k0,16
	bfc00008 03400008       jr      k0
	bfc0000c 00000000       nop
	bfc00010 401a6000       mfc0    k0,$12
	bfc00014 7f5a04c0       ext     k0,k0,0x13,0x1
	bfc00018 13400002       beqz    k0,$bfc00024
	bfc0001c 00000000       nop
	bfc00020 00000000       nop
	bfc00024 3c1da000       lui     sp,0xa000
	bfc00028 27bd2000       addiu   sp,sp,8192
	bfc0002c 3c1ca001       lui     gp,0xa001
	bfc00030 279c8000       addiu   gp,gp,-32768
	bfc00034 40096002       mfc0    t1,$12,2
	bfc00038 01205820       add     t3,t1,zero
	bfc0003c 7d2a1e80       ext     t2,t1,0x1a,0x4
	bfc00040 7d494984       ins     t1,t2,0x6,0x4
	bfc00044 40896002       mtc0    t1,$12,2
	bfc00048 41dce000       wrpgpr  gp,gp
	bfc0004c 408b6002       mtc0    t3,$12,2
	MIPS> p
	          TRISA(0xbf886010) 0x0000001f 00000000_00000000_00000000_00011111
	          PORTA(0xbf886020) 0x00000010 00000000_00000000_00000000_00010000
	           LATA(0xbf886030) 0x00000008 00000000_00000000_00000000_00001000
	          TRISB(0xbf886110) 0x00002fbf 00000000_00000000_00101111_10111111
	          PORTB(0xbf886120) 0x000001b0 00000000_00000000_00000001_10110000
	           LATB(0xbf886130) 0x00008b16 00000000_00000000_10001011_00010110
	MIPS>q
- 逆アセンブル、メモリーダンプ、ポートの状態監視が出来ます。
- mips16の命令列を逆アセンブルしたい場合は、l（エル)コマンドの引数に与える開始番地のLSBを立ててください。（奇数番地にします）-- ARM<->Thumbと同様です。

<!-- dummy comment line for breaking list -->

- PIC18spxのhidmonと同機能になる（予定）です。
- 今のところ、メモリー/ポート書き換えが未実装です。
- ポートアドレスをポート名から知りたいときは、その名称の途中までをタイプすると教えてくれます。

		MIPS&gt; p port
		          PORTA(0xbf886020)
		       PORTACLR(0xbf886024)
		       PORTASET(0xbf886028)
		       PORTAINV(0xbf88602c)
		          PORTB(0xbf886120)
		       PORTBCLR(0xbf886124)
		       PORTBSET(0xbf886128)
		       PORTBINV(0xbf88612c)
- ポートアドレスの全一覧は、

		MIPS&gt; p ?
- ポート内容一覧は、

		MIPS&gt; p *
- という具合です。

<!-- dummy comment line for breaking list -->

~
- - - -
## ブートローダーを使って、ユーザープログラムをデバッグする。
- 今のところ、デバッグ用の関数やコマンドは用意していません。
- しかし、ユーザープログラムが走った後のメモリー状態をブートローダーでダンプすることは可能です。
- ブートローダーとワークエリアが重なっていると上書きされますので、ユーザープログラムのSRAM使用番地を少し後ろにずらすことで破壊されないようにしてメモリー内容を観察します。

<!-- dummy comment line for breaking list -->

- - - -
## MIPS32とmips16の混在について
基本的にgccなのでMicroChipのpic32-gccと、Pinguinoのmips-gccはほぼ同じです。
- 但し、#pragma で始まる指令は、pic32-gcc限定です。mips-gccではほぼ丸無視されます。
- MPLAB-Xと組み合わせて使うxc32のほうは、うわさによると'-mips16'オプションが使えません。（無償版で）

<!-- dummy comment line for breaking list -->

~
では、混在のさせ方ですが、
~
- Ｃソース単位で混在させる。
    - mips16でコンパイルしたいソースは '-mips16' オプション付きで、そうでないＣソースは-mips16オプション無しでコンパイル。
- 関数単位で指定する。

		__attribute__（（mips16））
		__attribute__（（nomips16））
- を関数アトリビュートで付けます。

<!-- dummy comment line for breaking list -->
~

しかし、以下のようにしたほうが便利です。

- 基本的に、全体のコンパイルモードはMakefile中のビルドオプションに'-mips16'を付けるか付けないかで決めます。
- Flashの容量が許すなら'-mips16'コンパイルオプションをつける必要はありません。
- 容量を節約したい場合のみ、全体に'-mips16'コンパイルオプションをつけてコンパイルします。

<!-- dummy comment line for breaking list -->

~
さて、そこで'-mips16'をつけるとアセンブラ段階でエラーする関数が存在することに気づきます。
- それらの関数は、中にasm命令か、それと同義のマクロ（例えばコプロセッサレジスタを操作する命令）が含まれて居ます。
- 同じ働きをするmips16のasm命令に置き換えるのは面倒だったり、そもそも不可能だったり、あるいはコンパイルオプションにあわせてasm命令を差し替えるというのは現実的ではありません。
- なので、その関数を丸ごとMIPS32指定にします。

		#define _MIPS32  __attribute__（（nomips16））
- のようなdefineを書いて、

		int _MIPS32 SystemInit()
		{
		   ・・・
		}
- のようにします。

<!-- dummy comment line for breaking list -->

~
- gccの困るところとして、最適化のため勝手にインライン関数化されることがあり、それによって、たとえばSystemInit()が別の関数に埋め込まれてしまうことがあります。
- そうすると、別の関数まで _MIPS32をあたえないといけなくなります。
- これを避けるには、__attribute__（（nomips16,noinline）） のように記述します。

<!-- dummy comment line for breaking list -->

~
あと、原則の**お約束**ですが、
- リセットベクターと割り込みベクターはMIPS32でなければなりません。
- リセットハンドラー、割り込みハンドラーから呼び出される関数はmips16でも構いません。
- もちろん、それ以外の関数はMIPS32とmips16が混在して、相互に呼び出しても構いません。

<!-- dummy comment line for breaking list -->

~
~
- - - -
## コードサイズ削減の技
#### ここでは、コードサイズを4kB(FLASH領域)+3kB(BOOT領域)に収める手法を紹介いたします。

■ ファームサイズ縮小のための禁断テクニック

~
- crt0.Sの改造.
    - BFC0_0000(kseg1) から 9FC0_0010(kseg0)へのlong jumpはどうしても	16byte掛かりますが、
    
    		_reset:
    		       la      k0, _startup  # ２命令必要.
    		       jr      k0            # Jump to startup code
    		       nop	            # 遅延スロット(Jump命令が実行されたにも拘らず、パイプライン処理の都合上実行されてしまう命令)
    - nopを省略することで、12byteにしています。
    - nopの部分には
    
    		_startup:
    		       la      sp,_stack
    - の前半の命令(lui) が入ります。
    - すなわち、lui sp,high(_stack) が、遅延スロットと飛び先で、都合２回実行されます。
    - （実害はありません）

<!-- dummy comment line for breaking list -->

~

- while() {} ループを do {} while() ループに格下げ
    - .bssのクリアと .data のコピーのループを do while ループに格下げしています。
    - .bssと.data が零バイトの場合問題がありますが、零バイトでないことがあらかじめ
    - 確定しているので実害はありません。

<!-- dummy comment line for breaking list -->

~

- ramfuncコピーの削除
    - ramfunc(RAM上で実行される関数)は一切定義していませんので、省略します。

<!-- dummy comment line for breaking list -->

~

- main()の呼び出しの簡略化

		       la      k0, main
		       jr      k0                      # Jump to startup code
			nop
    - を	jalx	main に置き換えています。
    - main関数は常に mips16であることを仮定しています。
    - mainの引数(argc,argv)の設定も省略しています。
    - また、main()から制御が戻ることはないので無限ループも省略しています。

<!-- dummy comment line for breaking list -->

~
- 各種例外ベクターの省略
    - HID bootloaderは一切の割り込みを使用していませんのでベクターエリアを全部省略しています。
    - バスエラー等も端折っています。
    - またINTxxxx() 系のシステム関数は di,ei以外ダミーにして、付随する大きなテーブルのリンク	を防いでいます。

<!-- dummy comment line for breaking list -->

~
- mips16化と、１回しか呼ばれない小さな関数の適切なinline化、
    - それから、コンパイルオプションで、-ffunction-sections -fdata-sectionsを指定して、
    - リンカオプションで、不要関数のリンク抑止 -Wl,--gc-sections を忘れずに入れておきます。

<!-- dummy comment line for breaking list -->


~

- コンパイラに与える -Os -G4 オプション (gpアクセス)。
    - -G4 を与えると、絶対番地アクセスがgp相対になり、１命令づつ縮みます。
    - -G4 より大きな (-G8 -G16等)を与えると逆にコードサイズが増えます。

<!-- dummy comment line for breaking list -->

~
- 9fc0_0000〜9fc0_0bf0 までの領域に詰め込み
    - _BOOTROM_ アトリビュートを与えます。
    - 但し、inline化されることがわかっている関数については、配置がそうなることが確定
    - していても _BOOTROM_ は与えずに、static inline とだけ記述します。

<!-- dummy comment line for breaking list -->

    - 確実にinline化したい場合は、MACRO記述に置き換えるか、そのまま手書きでinlineします。

<!-- dummy comment line for breaking list -->

~
- USB descripterの省略
    - 長い文字列は全部１文字にしました。
    - そうしないと合計 7kBに入らないからです。

<!-- dummy comment line for breaking list -->





- - - -
以下、予定稿

## ブートローダーの内部関数説明

~
## ブートローダーでＬチカ。
- ポートの書き換えをhidmon32.exeから対話的に行なうことが出来ます。
- hidmon32.exeは、この対話実行をバッチで行なうことも可能になっています。
- なので、Ｌチカを行なうスクリプトをhidmon32.exeに与えることで、Ｌチカを実行できます。

<!-- dummy comment line for breaking list -->

（coming soon....）

~
~
~
- - - -
## 参考リンク

以下のリンクは両方とも熟読すること。でないとMIPSとかさわっちゃだめ。
#### とても参考になる。

『はじめて読む MIPS(リローデッド)』 by 中森章 - CQ出版社 
- www.cqpub.co.jp/interface/TechI/Vol39/app/mips_asm.pdf

<!-- dummy comment line for breaking list -->

PIC32ファミリ　リファレンスマニュアル　ＣＰＵ（日本語）
- [http://ww1.microchip.com/downloads/jp/DeviceDoc/61113D_JP.pdf](http://ww1.microchip.com/downloads/jp/DeviceDoc/61113D_JP.pdf) 

