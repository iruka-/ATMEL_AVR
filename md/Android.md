**目次**




- [Android開発環境の準備](Android_Dev.md) 

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## Android端末でのshellの実行

- Android端末で shellやLinuxコマンド を動かすには、[Jackpal：Android-Terminal-Emulator](https://github.com/jackpal/Android-Terminal-Emulator) を使用します。

<!-- dummy comment line for breaking list -->

- これは、[GooglePlay](https://play.google.com/store/apps/details?id=jackpal.androidterm&amp;hl=ja)  から入手できます。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## [KBOX2](http://kevinboone.net/kbox2.html)  について
- KBOX2は、上記 Android-Terminal-Emulator 内で動作する、基本的なLinuxコマンド群(BusyBoxと呼ばれるもの)の実行環境です。

<!-- dummy comment line for breaking list -->

- 同じく、[GooglePlay](https://play.google.com/store/apps/details?id=com.monami_ya.onebox.manager&amp;hl=ja) から、インストーラーを入手して実行します。

<!-- dummy comment line for breaking list -->

- Android端末の /data/data/jackpal.androidterm/kbox2/ というディレクトリにインストールされます。

<!-- dummy comment line for breaking list -->

- Android-Terminal-Emulator の「設定」メニューを開き、初期コマンドに

		/data/data/jackpal.androidterm/kbox2/bin/kbox_shell
- を与えることで、Terminal-Emulator起動直後からKBOX2のシェルに入ることが出来ます。

<!-- dummy comment line for breaking list -->


- そしてインストール完了後は、インストーラーをアンインストールしても問題ありません。

<!-- dummy comment line for breaking list -->


- 他のBusyBoxとの違いは、libfakechroot.soを使用して、Linuxの基本的なディレクトリ構成を再現出来る事です。

<!-- dummy comment line for breaking list -->

~

- 環境変数はこんな感じになります。

		FAKECHROOT='true'
		FAKECHROOT_BASE='/data/data/jackpal.androidterm/kbox2'
		FAKECHROOT_EXCLUDE_PATH='/data/data/jackpal.androidterm/kbox2:/data/data/jackpal.androidterm/kbox2'
		FAKECHROOT_VERSION='2.16'
		HOME='/home/kbox'
		・・・
		KBOX='/data/data/jackpal.androidterm/kbox2'
		LD_LIBRARY_PATH='/data/data/jackpal.androidterm/kbox2/lib:/data/data/jackpal.androidterm/kbox2/usr/lib'
		LD_PRELOAD='/data/data/jackpal.androidterm/kbox2/lib/libfakechroot.so'
- ~
- **libfakechrootについて [#y7a3d637]
- root権限なしでchrootするツールです。(chrootというのは、ファイルシステムの'/' マウントポイントを現在のファイルシステムの任意のディレクトリ位置に再設定する機能)
- Debianとかで、dpkg作るときに使われるツール。
- cygwinもこの原理で動いているそうです。

<!-- dummy comment line for breaking list -->

~
### どうやって実現しているの？
- ld.so の LD_PRELOAD 機能を使って、 /lib/libfakechroot.so を libc.soよりも先に常駐（リンク）させておきます。
- libc.soの open や chdir , opendir 等をhookしてディレクトリ位置を置き換えます。

<!-- dummy comment line for breaking list -->

~

~
~
~
- - - -
## テキストエディタ jed の移植

- jedは、Emacsに近い操作性を持つ軽量なテキストエディタです。

<!-- dummy comment line for breaking list -->

- 早速 jed を移植してみました。

<!-- dummy comment line for breaking list -->

ダウンロード: [jed-android.tgz](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/jed-ja/jed-android.tgz) 

- 日本語を通すには、

		$ export LANG=ja_JP.UTF-8
- を実行するか、上記１行を /etc/profile に追記しておきます。
- jedは、UTF-8以外の文字コード（例えば、EUC,S-JIS）のサポートはありません。

<!-- dummy comment line for breaking list -->



~
~
~
- - - -
## テキストエディタ jed の移植手順に関するメモ
- まず、Ubuntu14.04LTSが走るLinux PCもしくはVMWarePlayerインスタンス等を用意します。
- [Android NDK](https://developer.android.com/tools/sdk/ndk/index.html) をインストールします。インストール先は単純化の為

		/usr/local/ndk/
- とします。（そこでない場合は、シンボリックリンクを張ることで代用可能）
- 次に NDK内の make-standalone-toolchain.sh を実行して、スタンドアロンなツールチェーンを $HOME/arm/toolchain に作ります。

		/usr/local/ndk/build/tools/make-standalone-toolchain.sh --platform=android-19 --toolchain=arm-linux-androideabi-4.6 --install-dir=$HOME/arm/toolchain
- ツールチェーンでビルドするための環境変数を設定します。 .bash_profile辺りの最後に追記でＯＫです。

		# ANDROID
		#
		export NDK_HOME=/usr/local/ndk
		export NDK_PREFIX=$HOME/arm/android/libs
		#
		export NDK_HOST_ARM=arm-linux-androideabi
		export NDK_TOOLCHAIN_ARM=$HOME/arm/toolchain
		#
		export PATH=$PATH:$NDK_HOME:$NDK_TOOLCHAIN_ARM/bin
- ~
- jedとslang2のソースを入手します。

		$ cd ~/arm
		$ apt-get source jed
		$ apt-get source slang2
- ~
- slang2からビルドします。

		$ cd slang2-2.2.4/
		$ ./configure --host=$NDK_HOST_ARM --prefix=$NDK_PREFIX/$NDK_HOST_ARM
		$ make
		$ make install
		$ cd ..
- インストール先は、$HOME/arm/android/libs/arm-linux-androideabi/ 以下になります。

<!-- dummy comment line for breaking list -->
~
- jedをビルドします。

		$ cd jed_0.99.19/
		$ ./configure --host=$NDK_HOST_ARM --prefix=$NDK_PREFIX/$NDK_HOST_ARM
		$ make
		$ make install
- 同じくインストール先は、$HOME/arm/android/libs/arm-linux-androideabi/ 以下になります。

		#
- ~
- 実際にやってみると、上記手順でビルドエラーがいろいろ発生するので、ソースやconfigの手直しが必要になります。
- ~
- ~
- ~

<!-- dummy comment line for breaking list -->
- - - -
## slang2のビルドで起きる問題と対策
- 最初に躓くのが、./configure 出来ないことです。
- これは autoconf/config.sub を別の新鮮なソースパッケージから取ってきて置き換えます。(vimとか)

<!-- dummy comment line for breaking list -->

- ソース手直し差分を置きます。
- [https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/jed-ja/diff.sl2.txt](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/jed-ja/diff.sl2.txt) 

<!-- dummy comment line for breaking list -->

- 動的リンクライブラリ(.so)にするよりもstaticリンクのほうがよい場合は、

		$ make static
- にて、libncurses.aを作って、そっちをリンクします。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## jedのビルドで起きる問題と対策
- slang2と同様、 autoconf/config.sub を別の新鮮なソースパッケージから取ってきて置き換えます。

<!-- dummy comment line for breaking list -->

- ソース手直し差分を置きます。
- [https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/jed-ja/diff.jed.txt](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/jed-ja/diff.jed.txt) 

<!-- dummy comment line for breaking list -->

~
- Ctrl+Sを押して検索するときに、require.slが無い、と言われる場合は、slangのslライブラリからrequire.slが読み込めていないのが原因です。
- とりあえず、require.slだけでも、/usr/lib/jed/lib/ 以下にコピーしておくと回避できます。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## jedの裏ワザ
- KBOX2_shell を使わない状態(chrootする前のAndroid shell) から、jedを起動することが出来るようになりました。

<!-- dummy comment line for breaking list -->

- これは、環境変数 JED_ROOT が未定義の場合のデフォルトのjedのディレクトリ：

		/usr/lib/jed
- が存在しない場合、

		/data/data/jackpal.androidterm/kbox2/usr/lib/jed
- を代替として参照するようにコードを追加しています。
- もちろん、環境変数 JED_ROOT で明示的に指定することも出来ます。その場合はディレクトリ配置は自由です。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## KBOX2の裏ワザ
- 同様に、KBOX2に含まれるBusyBoxや、その他のコマンドについても、KBOX_shellを使用せずに起動しても一部の機能は使えます。

		/data/data/jackpal.androidterm/kbox2/bin/ls　とか。
- また、ROOT権限取得済の端末から、

		$ su
- を行った後、

		/data/data/jackpal.androidterm/kbox2/bin/kbox_shell
- を起動することで、特権付きのKBOX_shellを使うことも出来るようです。( /android_root/ にchdirすれば、Android側のファイルシステムも触れます )
- ただし、root権限を持ったままの各種作業は、あまりお勧めできません。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## 物理キーボードは必要

- ただでさえ狭い端末画面はソフトウェアキーボードを表示させるとさらに狭くなりますし、キータッチストレスが溜まるので**物理キーボード**は必須です。

<!-- dummy comment line for breaking list -->

２択
- Bluetooth接続のキーボード（小型なので携帯しやすいけれど打ちにくい）
- USB micro-B to Aメス 変換ケーブルを使用して、USBキーボードを接続（いつも使っているUSBキーボードが使える）

<!-- dummy comment line for breaking list -->

~

- 物理キーボードが使えない状況では、[hackers keyboard](https://play.google.com/store/apps/details?id=org.pocketworkstation.pckeyboard&amp;hl=ja) がお勧め。
    - デフォルト設定では、Portlait modeではCTRLやTABが出ないので、設定を変えてやる必要がある。

<!-- dummy comment line for breaking list -->





~
~
~




~
~
~
- - - -
## 他のソフトウェアの入手

ここから：
- [http://kevinboone.net/kbox2_downloads.html](http://kevinboone.net/kbox2_downloads.html) 

<!-- dummy comment line for breaking list -->


日本語対応Vim
- https://sites.google.com/site/fudist/Home/qfixhowm/other-service/howm-android

<!-- dummy comment line for breaking list -->


~
~
~
~
~

- - - -
参考URL

,[android-memo](http://d.hatena.ne.jp/android-memo/201307) 
,[AndroidのNDKのツールを使って実行ファイルをビルドする簡単な方法](http://blog.kmckk.com/archives/2918551.html) 
,[AndroidのNDKツールを使ってrubyをビルドして動かす](http://blog.kmckk.com/archives/2918745.html) 
,[android NDKでCライブラリをクロスコンパイル](https://sites.google.com/site/fudist/Home/vim-nihongo-ban/android-vim-build/android-ndk-cross-compile) 
~
~
~
~

