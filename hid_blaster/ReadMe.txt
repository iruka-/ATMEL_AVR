■ 概要

   これは、OpenOCD の DLL ハック（実験）です。

   即ち、外部に DLL を置いて、JTAGアダプターのドライバーの分離実装を試みています。

■ 現在のステータス

   非常に低速ですが、とりあえず、ATtiny2313を使用したJTAGアダプターが動いています。


■ 試し方

   WindowsXPを用います。

   HIDaspx ハードウェアを用意して、ファームウェアのみ、下記のものに差し替えておきます。

     hid_blaster/firmware/main-12.hex

   結線は、
   ATtiny2313        ARM基板 JTAG端子
          PB7  ------------- TCK
          PB6  ------------- TDI
          PB5  ------------- TDO
          PB4  ------------- TMS
  となります。これ以外のnTRSTピンなどはまだサポートしていません。(USB-Blasterと同様)
   (上記４本の線は直結でも動作しましたが、100Ω程度の直列抵抗を入れてショート事故を防ぐようにする
　　ほうが安全です)

   ARM側は3.3Vなので、AVR側の電圧に注意してください。(ATtiny2313側も3.3Vにする必要があります)

   hidblast/ ディレクトリの ocd.bat あるいは、 ocd2.bat を起動して、openocd.exeの吐き出すメッセージ
   を確認することが出来ます。

   正常に接続出来ているようでしたら、telnetで localhost:4444 番に接続して、OpenOCDコマンドを実行して
   みてください。


■ ディレクトリ構成

 hid_blaster-+- ソース.
             |
             +--helper\   ヘッダーファイル.
             +--jtag\     ヘッダーファイル.
             |
             +--firmware\ ATtiny2313側のファームウェア (main-12.hexを焼きます)
             |
             +--openocd_patch\  openocd本体側作成用の改造点


■ プログラムの再ビルド方法

   WindowsXP上のMinGW-gccコンパイラを用いてmakeしてください。
   makeすると、hidblast.dll が作成されます。

   openocd.exe本体を再ビルドする方法は、以下のURLを参照してください。

-http://hp.vector.co.jp/authors/VA000177/html/2010-09.html
   
   今回の改造部分ソースはopenocd_patch/ ディレクトリに置いています。

   Linux上でのビルドオプションは、こんな感じです。
   $ ./configure \
       --build=i686-pc-linux-gnu \
       --host=i586-mingw32msvc \
       --enable-dummy

   出来上がった openocd.exe 本体は、ドライバーとして、同一ディレクトリに存在する hidblast.dll を
   起動時に呼び出します。(存在しなければ、dummyドライバーのみが組み込まれます)


■ 現状の問題点

   まだ遅いです。
   HIDmon / HIDaspx の機能を残してはいますが、コードサイズ節約による速度低下があります。

■ ライセンス

   OpenOCDの配布ライセンスに準じます。


■ 展望

   hidblast.dll ファイルを(自力で)差し替えるだけで、自作デバイスがサポート可能になります。

   hidblast.dll のエントリーポイントは、
      DLL_int get_if_spec(struct jtag_command **q);
   だけです。引数のstruct jtag_command **qのqには、openocd本体のjtag_command_queueという
   グローバル変数のアドレスを渡します。
   戻り値は、(intになっていますが) ドライバー記述構造体のアドレスになります。


