[hid_blaster](hid_blaster.md) 

## OpenOCDのWindows版をLinux上からクロスビルドする方法。

実は**超簡単**です。

- 一応cygwinからexeファイルを作るのには成功していたけれど、cygwin1.dll依存になるのでMinGWベースのOpenOCDが欲しかった。

<!-- dummy comment line for breaking list -->

- cygwin上にはautoconfをはじめとするconfigureに必要なツールがほぼ揃っているけれど、Windows上のMinGWでそれをやるのは難しい。

<!-- dummy comment line for breaking list -->


#### 今回は、Linux(ubuntu 10.04LTS)上で行います。

- ものの１０分もあればexeが出来上がります。

<!-- dummy comment line for breaking list -->


まず、必要そうなパッケージをapt-getで取得します。

	# apt-get install git-core
	# apt-get install gcc-mingw32
	# apt-get install mingw32
	# apt-get install libconfig8
	# apt-get install libtool
	# apt-get install autoconf
	# apt-get install automake
	# apt-get install cmake
	# apt-get install texinfo

- 次にlibftd2xx(CDM20602.zip)をFTDIサイトから取得して展開します。

<!-- dummy comment line for breaking list -->

	$ mkdir libftd2xx-win32
	$ cd libftd2xx-win32
	$ unzip ../CDM20602.zip
	$ cd ..

ファイル名はバージョンが変わると変わるかもしれません。
- もしFTDI配布物がzipでなくexeだったら、Windows上で展開して持ってくるか、

<!-- dummy comment line for breaking list -->
wine(Windowsエミュレータ)を使って実行します。

~

- OpenOCDソースの取得

<!-- dummy comment line for breaking list -->

	$ git clone git://openocd.git.sourceforge.net/gitroot/openocd/openocd

- ビルド

<!-- dummy comment line for breaking list -->

	$ cd openocd
	$ ./bootstrap
	$ ./configure \
	       --build=i686-pc-linux-gnu \
	       --host=i586-mingw32msvc \
	       --enable-maintainer-mode \
	       --enable-ft2232_ftd2xx \
	       --with-ftd2xx-win32-zipdir=../libftd2xx-win32
	$ make

これでsrc/openocd.exe が出来上がります。



- 拙作[hid_blaster](hid_blaster.md) や[pic18blaster](pic18blaster.md) で使う場合は、hid_blaster/openocd_patch/ ディレクトリにあるパッチ修正

<!-- dummy comment line for breaking list -->
を施した後に、
- ./configureのオプションを以下のように指定してからmakeしてください。

<!-- dummy comment line for breaking list -->

	  $ ./configure \
	      --build=i686-pc-linux-gnu \
	      --host=i586-mingw32msvc \
	      --enable-maintainer-mode \
	      --enable-dummy
	  $ make

~
~

~
- - - -
## 追記：OpenOCD Version 0.5.0では[jimtcl](http://workware.net.au/jimtcl/JimTcl/Jim_Tcl.html) が必須になっているようです。

openocd/jimtclディレクトリが空のままになっていた場合、

(cross-)msysから openocdのjimtclディレクトリにうまくgit できない場合は手動で、
	git clone git://repo.or.cz/jimtcl.git
によりjimtclを取得してください。

~
~

## 最近の流行りはD2XXドライバーの代わりに、libftdi＋LibUSB(win32)でドライブ。

- （１）プロプライエタリーなD2XXドライバーを使用するか、
- （２）libFTDIというオープンソースのライブラリを使うか。

<!-- dummy comment line for breaking list -->

この選択は、OpenOCDコンパイル時に決めておく必要があるようです。

~


- libftdi 、LibUSB-win32の取得方法

<!-- dummy comment line for breaking list -->

libftdiのダウンロード
- [http://www.intra2net.com/en/developer/libftdi/download.php](http://www.intra2net.com/en/developer/libftdi/download.php) 

<!-- dummy comment line for breaking list -->

LibUSB-win32
- [http://sourceforge.net/apps/trac/libusb-win32/wiki](http://sourceforge.net/apps/trac/libusb-win32/wiki) 
- [http://sourceforge.net/projects/libusb-win32/files/libusb-win32-releases/](http://sourceforge.net/projects/libusb-win32/files/libusb-win32-releases/) 

<!-- dummy comment line for breaking list -->

- LibUSB-win32は、Version 0.1.12.2 をインストールします。

<!-- dummy comment line for breaking list -->

~
~

## libftdi LibUSB-win32のビルド方法

MinGWを使用してWindows上でビルドします。
- [http://www.mingw.org/](http://www.mingw.org/) 

<!-- dummy comment line for breaking list -->

ビルドするためには MSys 1.0 にもパスを通してください。
	set PATH=C:\MinGW\msys\1.0\bin;%PATH%

ビルド方法は普通に
	./configure
	make
ですが、libftdiは /usr/bin/libusb-config というshellスクリプトが必要です。
- このスクリプトの入手先が分からなかったのですが、ubuntuのlibusb-devパッケージには含まれている

<!-- dummy comment line for breaking list -->
ようですので、ubuntuから持ってくるのが一番早道です。

- LibUSB-win32は、ビルド済みのバイナリー（libusb-win32-device-bin-0.1.12.2.tar.gz など）があるならば、

<!-- dummy comment line for breaking list -->
とくにビルドする必要はありません。

両方の出来上がったライブラリとヘッダーを、ubuntuのmingwディレクトリーにコピーすれば、OpenOCDのビルド
を通すことが出来るようになります。

	# cp libusb.a  /usr/i586-mingw32msvc/lib/
	# cp libftdi.a /usr/i586-mingw32msvc/lib/
	
	# cp usb.h  /usr/i586-mingw32msvc/include/
	# cp ftdi.h /usr/i586-mingw32msvc/include/

## libusbドライバーの導入

- libftdiを使用する場合は、下位層ドライバーはLibUSBになります。
- まず、LibUSB-win32-bin-1.2.5.0.zipを入手して展開し、inf-wizard.exeを起動します。
- FT2232のUSBデバイスが（２個）そのVID:PIDとともにGUIに列挙されます。
- それぞれを選択して、infファイルを生成します。
- ついでに、その場でインストールを済ませてしまいます。（インストール・ボタンが出る）

<!-- dummy comment line for breaking list -->

libUSBの導入はたったそれだけです。
- 32bit/64bitのどちらのWindowsでも導入できます。

