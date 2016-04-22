#contents

## ARMクロスコンパイラ構築
- Windows版arm-gccを作るのはしんどいので、Linux( ubuntu 11.10 )上で作成。

<!-- dummy comment line for breaking list -->

### ビルドのコツ
- 最初にＣだけ作って、あとでＣ＋＋を作るよろし。

<!-- dummy comment line for breaking list -->


~
~
~
**手順**
- - - -
### Linuxマシンを用意する。ubuntu10.4が今回のターゲット。
- マシンを用意しなくても、VMWare上で実行すればホストOSはWindowsでもよい。

<!-- dummy comment line for breaking list -->


~
- いろいろビルドに必要なパッケージをインストールしておく。

<!-- dummy comment line for breaking list -->

	# apt-get install build-essential
	# apt-get install libtool
	# apt-get install autoconf
	# apt-get install automake
	# apt-get install texinfo
	# apt-get install libmpfr-dev
	# apt-get install libmpc-dev
	# apt-get install libgmp3-dev
	# apt-get install libcloog-ppl-dev
	
	# apt-get install bison
	# apt-get install flex
	# apt-get install zlib1g-dev


- 無意識にインストール済みなやつも多いので、もしかしたら、抜けがあるかもしれない。

<!-- dummy comment line for breaking list -->


### 作業ディレクトリを掘る

	$ mkdir build
	$ cd build

### ソースTARボールを入手する。

- wget.shを用意

		#!/bin/sh
		wget -o wget1.log http://aist.ring.gr.jp/archives/GNU/binutils/binutils-2.21.tar.bz2
		wget -o wget2.log http://aist.ring.gr.jp/archives/GNU/gcc/gcc-4.5.2/gcc-4.5.2.tar.bz2
		wget -o wget3.log ftp://sources.redhat.com/pub/newlib/newlib-1.18.0.tar.gz
- 実行

		./wget.sh
- **取得できたら展開 [#z9792e6e]
- extract.sh

		#!/bin/sh
		tar xvfj binutils-2.21.tar.bz2
		tar xvfj gcc-4.5.2.tar.bz2
		tar xvfz newlib-1.18.0.tar.gz
- 実行

		./extract.sh
- **では、一気に行こうか。 [#ee0f6836]
- build-all.sh

		#!/bin/sh
		#
		echo '# =========================================================='
		echo '# setenv'
		echo '# =========================================================='
		#
		BINUTILS=binutils-2.21
		GCC4=gcc-4.5.2
		NEWLIB=newlib-1.18.0
		BUILDTARGET='--target=arm-none-eabi --prefix=/usr/local/arm '
		#
		#
		#   第一引数がエラー値だったらexitする
		print_error () {
		    if [ $1 -ne 0 ]
		    then
		        exit $1
		    fi
		}
		#
		export PATH=$PATH:/usr/local/arm/bin
		#
		mkdir _binutils
		mkdir _gccbuild
		mkdir _newlib
		#
		echo '# =========================================================='
		echo '# _binutils'
		echo '# =========================================================='
		#
		cd _binutils
		../$BINUTILS/configure $BUILDTARGET
		print_error $?
		#
		make
		print_error $?
		#
		sudo make install
		#
		# 次、gccのＣ言語のみ。config内容はWindowsのWinARMコマンドラインのhelpから取得
		#
		cd ..
		#
		#
		echo '# =========================================================='
		echo '# _gccbuild'
		echo '# =========================================================='
		#
		cd _gccbuild
		#
		../$GCC4/configure $BUILDTARGET \
		--disable-nls --disable-shared --disable-threads \
		--with-gcc --with-gnu-ld --with-gnu-as --enable-languages=c,c++ \
		--enable-interwork --enable-multilib --disable-libssp --disable-libstdcxx-pch \
		--with-dwarf2 --program-prefix=arm-none-eabi- --with-newlib \
		--disable-newlib-supplied-syscalls --disable-libunwind-exceptions \
		--disable-newlib-atexit-dynamic-alloc \
		--disable-zlib
		print_error $?
		#
		make LANGUAGES=c all-gcc
		print_error $?
		#
		sudo make LANGUAGES=c install-gcc
		#
		#次はNewlib
		#
		cd ..
		#
		echo '# =========================================================='
		echo '# _newlib'
		echo '# =========================================================='
		#
		cd _newlib
		../$NEWLIB/configure $BUILDTARGET
		print_error $?
		#
		make
		print_error $?
		#
		#sudo make install ？？？あれ?
		sudo PATH=$PATH:/usr/local/arm/bin  make install
		#
		#最後に、残りのＣ＋＋も含める。
		#
		cd ..
		#
		echo '# =========================================================='
		echo '# gcc ALL'
		echo '# =========================================================='
		#
		cd _gccbuild
		make
		print_error $?
		#
		sudo make install
		cd ..
- だいたいこんな感じ。
- zlibは -m64でエラーする・・・。--> --disable-zlibでごまかす。

<!-- dummy comment line for breaking list -->

~
- たいていは途中でつまずくので、これを切り出して個別に実行したほうが良いかも。
- gcc,binutils,newlibのバージョンは**旬というもの**があるので、環境変数にしてみました。

<!-- dummy comment line for breaking list -->

~
- - - -
- 出来たのは良いけれど、armonをビルドして試してみると、"syscalls.c"のビルドが通らなくなっていた。
- SVC命令が-mthumbでは使えない、というアセンブラメッセージだった。
- もしかしたら、ThumbではSVCが使えないのに、そのようなasm文が落ちる設定になっていたのかも。
- 古いbinutilsではノーチェックなのか？？？？
    - いや、SVCは使えるはずだ。少なくともThumbでは。
    - ただ、SVCの引数は24bitなくて8bitになるらしい。

<!-- dummy comment line for breaking list -->

~
- - - -
- 実は、CodeSourcery G++ LiteのＤＬサイトを良く見たら、Windows/Linux版だけでなく、ソースもＤＬ出来るようになっている。
- ソースをＤＬしてみた。

<!-- dummy comment line for breaking list -->

arm-2010.09-51-arm-none-eabi% ls
	arm-2010.09-51-arm-none-eabi.sh
	binutils-2010.09-51.tar.bz2
	cloog-2010.09-51.tar.bz2
	coreutils-2010.09-51.tar.bz2
	expat-2010.09-51.tar.bz2
	gcc-2010.09-51.tar.bz2
	gdb-2010.09-51.tar.bz2
	gmp-2010.09-51.tar.bz2
	gnu-2010.09-51-arm-none-eabi.txt
	libelf-2010.09-51.tar.bz2
	libiconv-2010.09-51.tar.bz2
	make-2010.09-51.tar.bz2
	mpc-2010.09-51.tar.bz2
	mpfr-2010.09-51.tar.bz2
	newlib-2010.09-51.tar.bz2
	ppl-2010.09-51.tar.bz2
	zlib-2010.09-51.tar.bz2

- 必要物全部入っとるやんけ。
- arm-2010.09-51-arm-none-eabi.sh はビルドスクリプトになっていて、Linux版とWindows版の両方を、（たぶん）GNU/Linux OS上でビルド出来るようになっている。
- Windows版のexeファイルはLinux上のクロスMinGW-gccで作成出来る。便利だ。

<!-- dummy comment line for breaking list -->

- というわけで、**今のところ** CodeSourcery G++ Liteの成果を使わせていただくのが最も便利で楽勝だ。という結論に達した。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## 別解：summon-arm-toolchainを使ってLinux上でarm-gcc(elf)を作る。

summon-arm-toolchain
- https://github.com/esden/summon-arm-toolchain/
- 上記ページのDOWNLOADリンクをクリックして、zipファイルを取得する。
- 展開すると、summon-arm-toolchainというシェルスクリプトが出てくるので、これを、たとえばubuntu11.04上

<!-- dummy comment line for breaking list -->
で走らせる。（もちろんWindows上のVMimageなubuntuでも構わない）
- 走らせる前に、なるべくなら、build-essential とか gcc , git , libmpfr-dev , libmpc-dev ,libftdi-dev , texinfo などはインストールしておいたほうが良い。( apt-get install 〜 を使う)

<!-- dummy comment line for breaking list -->


あとはほぼ自動でダウンロード＆クロスビルドを行ってくれる。

- できたブツは ~/sat/ 以下に生成されるようだ。 一応ユーザー権限で全部作成された。(含: OpenOCD , gdb7 etc)

<!-- dummy comment line for breaking list -->

- ものぐさな人には便利かも。（全自動でビルドしてくれるので）

