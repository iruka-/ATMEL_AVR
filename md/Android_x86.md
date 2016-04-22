See [Android](Android.md) 




## [AndroVM](http://androvm.org/blog/) 改めGenyMotion

PC上でのAndroid開発やテスト動作はこれで決まり。

GenyMotion
- [http://www.genymotion.com/](http://www.genymotion.com/) 

<!-- dummy comment line for breaking list -->

Android-x86 (intelアーキテクチャー用にビルドされたAndroid OS)をさらにカスタマイズして
Oracle Virtual-Boxと統合してしまったようなAndroid VM環境動作ソフト。

Android 標準のSDKについてくるエミュレータでもx86ビルドのバイナリーは提供されているけど、
ここまで使いやすくはないし、（ARMバイナリーにくらべるとかなり速いんだけれどそれでも）遅い。

~
- 現時点(2013-11)ではGoogle Play (gapps) がインストールされたVM環境はダウンロード出来ない。
- （ちょっと前までは with googleappsのVMもＤＬ可能だった）

<!-- dummy comment line for breaking list -->

- ARM-Translation(x86ビルドでもARMバイナリーなアプリを動かす)とgapps(GooglePlay)の入手は、以下のＵＲＬの下のほうから。

<!-- dummy comment line for breaking list -->

- [http://stackoverflow.com/questions/17831990/how-do-you-install-google-frameworks-play-accounts-etc-on-a-genymotion-virtu/17856453](http://stackoverflow.com/questions/17831990/how-do-you-install-google-frameworks-play-accounts-etc-on-a-genymotion-virtu/17856453) 

<!-- dummy comment line for breaking list -->


~
~
~
~

~
~
~
~

~
~
~
~
以下古い情報
- - - -
[2011-11](2011-11.md) 　
[2012-04](2012-04.md) 


## Android-x86 4.0(ICS) を VMWarePlayer上 で飼ってみた。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ics2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ics2.jpg) 


**一次情報源**

- [Android-x86本家](http://www.android-x86.org/) 

<!-- dummy comment line for breaking list -->

**参考リンク**

- [VirtualBOX用のSDカード、LANの両方が使えるVmImage](http://sarl-tokyo.com/wiki/index.php?Android%20ICS%E3%82%92VirtualBox%E3%81%A7%E5%8B%95%E3%81%8B%E3%81%99) 

<!-- dummy comment line for breaking list -->


- [Android-x86_4.0.3（Ice Cream Sandwich）をVMWareFusion4の仮想マシンにインストール](http://www.paraches.com/archives/4425) 

<!-- dummy comment line for breaking list -->

~
~

- 以前VirtualBoxで試せるICSのISOイメージがあったのだが、今回のは[VMWarePlayerで試す奴](http://www.buildroid.org/Download/android-x86-vm-20120307.iso.gz) である。
- インストールのこつ
    - 自動インストールを避け、「OSをあとで入れる」、カーネルはOther Linux 2.6.x にする。
    - 仮想ハードディスクをSCSIからIDEに変更する。(8Gのままでよい)
    - VMに与えるメモリーは最低でも512M、出来れば1Gくらい必要。256MBだとなかなか起動しない。
    - CD(iso)起動直後の画面はすこし乱れているので、カーソル上下で選択メニューを出す。
    - IDEの8Gはext3で1パーティション丸々確保してよい。
    - パーティションのBOOTフラグを立てること。WriteしてからQuitすること。
    - インストール時にGRUBも入れる。Fake SDカードも入れる(2047MBデフォルトのままで良い)

<!-- dummy comment line for breaking list -->


![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ics0.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ics0.jpg) 

~
### Android-SDKのデバッグブリッジ - adb の繋ぎ方。

- アプリケーションiconの中にある「端末エミュレータを起動する」を選ぶ

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ics1.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ics1.png) 

- suして、system/xbin/ifconfigを実行する。
- VMWarePlayer内でのICSのipアドレスが分かるので、それを元にしてadbから接続する。

		#
		E:\Android-R18\android-sdk\platform-tools&gt;adb connect 192.168.79.134
		already connected to 192.168.79.134:5555
		#
		E:\Android-R18\android-sdk\platform-tools&gt;adb shell
		root@vm:/ # ls -l
		ls -l
		drwxr-xr-x root     root              2012-05-20 16:57 acct
		drwxrwx--- system   cache             2012-05-20 16:57 cache
		dr-x------ root     root              2012-05-20 16:57 config
		lrwxrwxrwx root     root              2012-05-20 16:57 d -&gt; sys/kernel/debug
		drwxrwx--x system   system            2012-05-17 08:25 data
		-rw-r--r-- root     root          116 2012-05-20 16:57 default.prop
		drwxr-xr-x root     root              2012-05-20 16:57 dev
		lrwxrwxrwx root     root              2012-05-20 16:57 etc -&gt; system/etc
		-rwxr-x--- root     root       187528 2012-05-20 16:57 init
		-rwxr-x--- root     root        19462 2012-05-20 16:57 init.rc
		lrwxrwxrwx root     root              2012-05-20 16:57 lib -&gt; system/lib
		drwxrwxr-x root     system            2012-05-20 16:57 mnt
		dr-xr-xr-x root     root              2012-05-20 16:57 proc
		drwxr-x--- root     root              2012-05-20 16:57 sbin
		lrwxrwxrwx root     root              2012-05-20 16:57 sdcard -&gt; mnt/sdcard
		drwxr-xr-x root     root              2012-05-20 16:57 sys
		drwxr-xr-x root     root              2012-05-17 08:24 system
		-rw-r--r-- root     root         3859 2012-05-20 16:57 ueventd.rc
		lrwxrwxrwx root     root              2012-05-20 16:57 vendor -&gt; system/vendor
		root@vm:/ #
- こんな感じ。

<!-- dummy comment line for breaking list -->

~
VMWarePlayer上のICSは、
- SDカード、ＬＡＮともに使用可能。
- 開発者向けオプションでGPUレンダリングを使用というチェックも使えるっぽい。
- （野良も含めて）Marketからのダウンロードはどうやら出来ないっぽい。（ずーっとＤＬ中のまま。もしかしたら何かアカウントとか手順がいるのかもしれない）
- 操作性が良い。（SDKのx86純正エミュレータよりもなぜか・・・）

<!-- dummy comment line for breaking list -->




~
~
~
~
~
~
- - - -

**過去ログ**



~
- - - -

## Android-x86 4.0(ICS) ビルド on ubuntu11.10(x64)

- eeePC版などでは LAN が使えなかったので、公開されていないgeneric_x86ビルドだとどうなるのか試してみました。

<!-- dummy comment line for breaking list -->

前準備
- ubuntu11.10(x64)／メモリー最低8GB出来れば12GB／disk消費22G程度を覚悟してください。

<!-- dummy comment line for breaking list -->


~

- （１） [http://www.android-x86.org/](http://www.android-x86.org/)  本家からgitリポジトリをcloneします。

<!-- dummy comment line for breaking list -->

- 参考はこのへんです。 -->  [http://www.android-x86.org/releases/build-20120101](http://www.android-x86.org/releases/build-20120101) 

<!-- dummy comment line for breaking list -->

    - cloneのやりかたですが、まず、 ~/bin/を作ってパスを通しておきます。
    - 次に repoを拾ってきます。

<!-- dummy comment line for breaking list -->

	$ curl https://dl-ssl.google.com/dl/googlesource/git-repo/repo > ~/bin/repo

    - そして、リポジトリをcloneします。

<!-- dummy comment line for breaking list -->

	$ mkdir ics
	$ cd ics
	$ repo init -u http://git.android-x86.org/manifest -b ics-x86 
	$ repo sync

    - 9GBくらい消費します。

<!-- dummy comment line for breaking list -->

- それから、x64のubuntuでビルドするためには、32bitのlibstdc++が必要になりますのでaptで取得します。

<!-- dummy comment line for breaking list -->

	# sudo apt-get install git-core gnupg flex bison gperf build-essential \
	  zip curl zlib1g-dev libc6-dev lib32ncurses5-dev ia32-libs \
	  x11proto-core-dev libx11-dev lib32readline6-dev lib32z-dev \
	  libgl1-mesa-dev g++-multilib mingw32 tofrodos python-markdown \
	  libxml2-utils

- さらに、isoイメージまで作ってくれますので、そのツールも入れておきます。

<!-- dummy comment line for breaking list -->

	# sudo apt-get install genisoimage syslinux

- （２） ビルドのための環境変数をセット

<!-- dummy comment line for breaking list -->

	$ source build/envsetup.sh

- （３）そしてビルド

<!-- dummy comment line for breaking list -->

	$ make iso_img TARGET_PRODUCT=generic_x86

    - マルチコアなCPUをお持ちの場合は、 '-j 4 'などの並列ビルドを試みてください。

<!-- dummy comment line for breaking list -->

- ビルドがうまくいけば、以下のディレクトリにisoファイルが出来上がります。

<!-- dummy comment line for breaking list -->

	out/target/product/generic_x86/

結果
- 試してみたところ、generic_x86版のisoはeeePC版と同じでSDカードも表示も問題なし。LANのみ接続できませんでした。

<!-- dummy comment line for breaking list -->


**ORZ**

こうすればいいそうです。
	# netcfg eth0 dhcp
	# setprop net.dns1 [DNSアドレス]

自分ちのDNSのIPアドレス覚えていない人はGoogle提供の 8.8.8.8とかでOK。


Android 3.2でLANを使うためには、ALT+F1、ifconfig .... とすると良いらしいです。

参考:
- [http://stackoverflow.com/questions/8227825/android-x86-porting-unable-to-make-it-work](http://stackoverflow.com/questions/8227825/android-x86-porting-unable-to-make-it-work) 

<!-- dummy comment line for breaking list -->

しかし、4.0ではALT+F1が効かない・・・困った。
- shellアプリがあるので実行してみたが、 # dhcpcd コマンドは受け付けられなかった。

<!-- dummy comment line for breaking list -->

~

Virtual BOXでよければ、SDカード,LANの両方が使えるvm imageが入手できる。
- [http://sarl-tokyo.com/wiki/index.php?Android%20ICS%E3%82%92VirtualBox%E3%81%A7%E5%8B%95%E3%81%8B%E3%81%99](http://sarl-tokyo.com/wiki/index.php?Android%20ICS%E3%82%92VirtualBox%E3%81%A7%E5%8B%95%E3%81%8B%E3%81%99) 

<!-- dummy comment line for breaking list -->


~
~
~
~

## Android 4.0 ICS VirtualBox イメージ

この辺にあった。

Ice Cream Sandwich-Testdrive in Virtualbox
- [http://www.borncity.com/blog/2011/12/01/experimenting-with-ics-in-virtualbox-vmlite-vmware/](http://www.borncity.com/blog/2011/12/01/experimenting-with-ics-in-virtualbox-vmlite-vmware/) 
- [http://www.borncity.com/blog/](http://www.borncity.com/blog/) 

<!-- dummy comment line for breaking list -->

非常に重い標準エミュを捨て、Android4.0 for x86でICSを快適体験(11/22追記)
- [http://d.hatena.ne.jp/td2sk/20111118/1321625830](http://d.hatena.ne.jp/td2sk/20111118/1321625830) 

<!-- dummy comment line for breaking list -->

~
自分もVirtualBoxで試してみた。
- 残念だがSD-CARDが使えない。でも上記ドイツ語サイトには解決方法とか書いてあるみたい。
- レスポンスはだいぶよくなっている。
- Flash Playerが使えないような気がする。どうなんだろ

<!-- dummy comment line for breaking list -->



<!-- dummy comment line for breaking list -->

~
~
~
- - - -


[2009-06](https://github.com/iruka-/ATMEL_AVR/blob/master/web/log/2009-06.html) 


## Android-x86

Nightly Buildサーバーが立てられたようです。isoファイルはここからＧｅｔ
- [http://android-x86.moonman.dk/](http://android-x86.moonman.dk/) 

<!-- dummy comment line for breaking list -->

~
~
~

- - - -

- [http://www.android-x86.org/](http://www.android-x86.org/) 

<!-- dummy comment line for breaking list -->


- ２０１０年８月にandroid v2.2 froyoのisoが公開されている。
- このisoは遅くて使えないので、自分でビルドするテスト

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/andsun.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/andsun.jpg) 

## DOWNLOAD(ISOファイル)

以下のURLから取得してください。

- [http://cid-e79d5ff1a8049089.office.live.com/browse.aspx/Android-x86](http://cid-e79d5ff1a8049089.office.live.com/browse.aspx/Android-x86) 

<!-- dummy comment line for breaking list -->

    - 約３０ＭＢ×２ファイルに分割しています。
    - [7zip](http://sevenzip.sourceforge.jp/) で圧縮されています。
    - 展開すると**generic_x86.iso**が出来上がります。
    - vmware-playerでの使い方の解説は、ReadMe.htmlを参照してください。

<!-- dummy comment line for breaking list -->

~
~
~
~

## DOWNLOAD(vmware-player用ファイル)

- isoファイルから[vmware-player](http://www.vmware.com/jp/products/player/) にインストールするのが面倒な場合は、下記vmxファイルを使用できます。

<!-- dummy comment line for breaking list -->

以下のURLから取得してください。

- [http://cid-e79d5ff1a8049089.office.live.com/browse.aspx/Android22-vmplayer](http://cid-e79d5ff1a8049089.office.live.com/browse.aspx/Android22-vmplayer) 

<!-- dummy comment line for breaking list -->

    - 約３５ＭＢ×２ファイルに分割しています。
    - [7zip](http://sevenzip.sourceforge.jp/) で圧縮されています。
    - 展開すると、以下のファイルが出来上がります。
    
    		Directory: Android22
    		           8,684 Android22.nvram
    		     190,775,296 Android22.vmdk
    		               0 Android22.vmsd
    		           2,209 Android22.vmx
    		             264 Android22.vmxf
    		             248 ReadMe.html
    - 中身はvmxファイルですので、VirtualBoxでは使用できません。
    - インストールはsda1=2G,sda2=6Gでパーティションを切っています。
- デフォルトインストール直後の状態から、
    - 言語を日本語にしています。
    - 谷歌IMEをOFF設定にしています。
    - 「提供元不明のアプリ」の許可ビットをチェック済みです。
    - AndAppStoreのクライアントのみインストール済みです。

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## 自分でビルドする方法。


参考はこの辺
- [http://172305071.blogspot.com/2010/08/itubuntu-androidx86-froyovirtualboxgene.html](http://172305071.blogspot.com/2010/08/itubuntu-androidx86-froyovirtualboxgene.html) 
- [http://labs.beatcraft.com/ja/index.php?Android%B3%AB%C8%AF%A5%B9%A5%BF%A1%BC%A5%BF%A5%AD%A5%C3%A5%C8%A5%DE%A5%CB%A5%E5%A5%A2%A5%EB](http://labs.beatcraft.com/ja/index.php?Android%B3%AB%C8%AF%A5%B9%A5%BF%A1%BC%A5%BF%A5%AD%A5%C3%A5%C8%A5%DE%A5%CB%A5%E5%A5%A2%A5%EB) 

<!-- dummy comment line for breaking list -->
~
## まず、ubuntu10.04 LTSを用意する。
- もちろん、Windowsマシンにvmware-playerを入れてubuntuをインストールしてもかまわない。

<!-- dummy comment line for breaking list -->

- ビルドに必要なＨＤＤ容量は20GBくらいは見ておいたほうが良い。

<!-- dummy comment line for breaking list -->

~
- （１）sunのjava5-jdkは、ubuntu10.*では普通にapt-getでヒットしないので、以下のようにして取得した。

<!-- dummy comment line for breaking list -->

	$ sudo add-apt-repository "deb http://us.archive.ubuntu.com/ubuntu/ jaunty multiverse"
	$ sudo add-apt-repository "deb http://us.archive.ubuntu.com/ubuntu/ jaunty-updates multiverse"
	$ sudo apt-get update
	$ sudo apt-get install sun-java5-jdk

- （２）ビルドに必要そうなパッケージをインストール

<!-- dummy comment line for breaking list -->

	$ sudo apt-get install git-core gnupg sun-java5-jdk flex bison gperf libsdl-dev \
	   libesd0-dev libwxgtk2.6-dev build-essential zip curl libncurses5-dev zlib1g-dev

- （３）repoを取得

<!-- dummy comment line for breaking list -->

	$ mkdir ~/bin
	$ export PATH=$PATH:~/bin
	$ curl https://android.git.kernel.org/repo > ~/bin/repo

- （４）android-x86を取得

<!-- dummy comment line for breaking list -->


	$ mkdir android-x86
	$ cd android-x86
	$ repo init -u git://git.android-x86.org/manifest.git -b froyo-x86
	$ repo sync

１時間くらい掛かる

- （５）ビルドする

<!-- dummy comment line for breaking list -->

	$ source build/envsetup.sh
	$ sudo make -j 4 iso_img TARGET_PRODUCT=generic_x86

２時間ぐらい掛かる((もしかしたらubuntuのshがdashなせいでビルド出来ないときは、/bin/sh -> /bin/bash にシンボリックリンクし直さないとだめかも。dash弱すぎ。))

- すると、android-x86/out/target/product/generic_x86/generic_x86.iso が出来上がる。
- これをvmware-playerのCD/DVDにマウントしてブートさせる。
- vm版をビルドするときは TARGET_PRODUCT=vm　にする。

<!-- dummy comment line for breaking list -->

これだけ大規模なソフトウェアなのに、誰でもgitから引っ張ってきて安易にビルド出来るというのは凄いことかもしれない。


~
## froyoを試した結論
- SDCARDのマウント方法が結局分からなかった。
    - 以下のv1.6でSDCARDを使う方法にて、/dev/sda2をext2,fat32の両方で試してみたが、両方とも認識しなかった。
    - もしかしたら、generic_x86でなくて、vmのほうでビルドしないといけないのかもしれない。
    - /dev/sda2をFAT32で予めフォーマットしておき
    
    		# newfs_msdos -F32 /dev/sda2
    - grubの起動時にコマンドラインを編集し SDCARD=/dev/sda2 を追加した場合、
    - 起動後コンソールから確認すると、/mnt/sdcard はちゃんとマウントされており、r/w可能になっている。が、設定アイコンのsdカード項目にいくと使用不可になっている。
    - vmのほうはHDDインストールではうまく起動しない。
    - vmのほうをLiveCD起動にするまえに、[TAB]を押してSDCARD=/dev/sda2 を追加しても認識してくれない。iso版も同じ。なんで？
- vmware-playerでなく、SunのVirtualBoxで試したら、iso版、自分のvm版ともにSDCARDを認識してくれる。
    - どうやら、Android内のVoldとかいうデーモンが勝手にSDCARDをマウントするようだ。grubのコマンドラインはノータッチでＯＫ。
- 結局、自分のところではvmware-playerでvm.iso版のSDCARDを認識させることが出来なかった・・・Orz
    - ---> generic_x86版だとOK . 下記を参照。

<!-- dummy comment line for breaking list -->

~
~
~
## vmware playerでSDCARD(/dev/sda2のFAT32パーティション)を認識できない件。

vold.fstabが決め撃ちなのではないか疑惑。
- android動作中に[ALT]+[F1]でvold.fstabを見ると、

		# cat /system/etc/vold.fstab
		dev_mount sdcard /mnt/sdcard auto /devices/pci0000:00/0000:00:01.1/host0/target0:0:0/0:0:0:0/block/sda
- となっている。
- けれど、

		/devices/pci0000:00/0000:00:01.1/
- というディレクトリが存在しない。(0000:00:01.1/は欠番になっている。かわりに他の枝番のディレクトリがたくさんある)

<!-- dummy comment line for breaking list -->

- 同じようにSDCARDをマウントできるVirtualBoxから見ると、

		/devices/pci0000:00/0000:00:01.1/host0/ ...
- というディレクトリは存在していて、アクセス可能になっているようだった。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/vmware-vold.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/vmware-vold.png) 

- vmware-playerでは/dev/sdaはpci0000:00/0000:00:07.1に来ている。

<!-- dummy comment line for breaking list -->


つまり、VirtualBoxのpciデバイスの枝番決め撃ちのvold.fstabが生成されているから（？）

- 良く分からない。
- donutでは、kernelの起動パラメータにSDCARD=/dev/sda2と書けば使えたけれど、~~froyoではVoldの言うとおりにしないと使えないようだ。~~ --これはvm版のみの挙動。

<!-- dummy comment line for breaking list -->


## vmware-playerでfroyoを試したいならば、以下のようにする。
- vm版ではなく、generic_x86版をビルドする。
- vmware-playerの仮想マシン作成時、scsiデバイスをideデバイスに変えておく。
- /dev/sda1をext3に、/dev/sda2をvfat(fat32)であらかじめフォーマットする。(installer起動後[ALT]+[F2]キーでシェルが使える)
- /dev/sda1にHDDインストールする。（そのほうが楽なので）
- インストール後、もう一度インストーラーを起動して、[ALT]+[F2]

		# mount /dev/sda1 /mnt
		# vi mnt/grub/menu.list にて、SDCARD=/dev/sda2 を追記する。
- viの追記方法はこのページの下のほうに説明がある。
- CDROMを切断しておいて、普通に起動する。

<!-- dummy comment line for breaking list -->

以上。




~
~
~


~
## AndAppStoreのアイコンが出ない。
- ~~出し方不明・・・Orz~~

<!-- dummy comment line for breaking list -->

ここからＤＬすればおＫ。但し、SDCARDがないと保存できない。
    - [http://andappstore.com/AndroidApplications/apps/7661](http://andappstore.com/AndroidApplications/apps/7661) 

<!-- dummy comment line for breaking list -->


- パフォーマンス問題はある程度改善されている。
    - ダイアログのようなウィンドウが出ているときは著しくマウスが遅くなる。

<!-- dummy comment line for breaking list -->

- vmware経由でLANに繋がり、chromeでWebを見たり、youtubeを見ることは出来た。
    - youtubeの動画はかなりコマ落ちする。(レディ・ガガのインタビュー動画を視聴したところ、10fps行ってないくらい遅い。ホストWindowsはCore2の2.5GHz程度）
    - ニコ動はflash playerのversionが古いといわれて、再生できない。

<!-- dummy comment line for breaking list -->

- Android-SDKのエミュレータ代わりに使える？
    - 未調査。たぶん使えるはずだけれど、接続方法とか知らない。そのまえにSDKの使い方も良く分かっていないので論外か。

<!-- dummy comment line for breaking list -->

    - このへん（[Googleの甘えのせいで、低スペックＰＣでAndroidエミュが起動しないときは](http://d.hatena.ne.jp/RC3/20101023/1287843279) ）を参考にすると良いかも。

<!-- dummy comment line for breaking list -->

- vm版もビルドしてみたが、iso版（froyo-vm-20100812.iso ）と同様の症状あり。
    - ＨＤＤにインストールすると、AndroidのGUIが起動せず、 '#'プロンプトで止まってしまう。
    
    		のところで ./init とやると、/dev/ttyがオープンできないと抜かす。/dev/ttyは存在するしパーミッションもある筈なんだけど、%%なぜ起動しないかは分からない。%%ＨＤＤインストールしないでLiveCD起動すれば使える。

<!-- dummy comment line for breaking list -->

    - LiveCD起動でも設定は一応永続するのでＨＤＤインストールから起動する必要はない。
    - 起動しない原因は、#プロンプトのところで、dmesgすればだいたい分かると思う。たぶん/dev/sda1のマウント問題(Vold側で/data/をマウントしてくれないとGUIが起動しないらしい)

<!-- dummy comment line for breaking list -->

~

~
~
~
## vm版をSunのVirtualBoxで試す場合の注意
- 「絶対座標指定のデバイスを有効化」のチェックボックスを外しておく。
    - あるいは、実行中にマウスが使えないときは、設定の「マウス統合を無効化」を行う。
- VirtualBoxのLAN接続が、WindowsXPのファイアウォールで阻止されている場合は、ファイアウォール設定の「例外」にVirtualBoxを追加する。
- VirtualBoxを使うときはVoldが勝手にマウントするので、SDCARD=/dev/sda2 などと書かなくて良い。というか書くと逆にSDCARDが使えない。

<!-- dummy comment line for breaking list -->



- 公開されている iso版は遅いので、自分でビルドしたvm版を使う。
- grubメニューが出たら、一番下のインストーラーを選ぶ。
- インストーラーが起動したところで[ALT]＋[F2]を押して'#'プロンプトを出す。

		# fdisk /dev/sda
		  ここで、/dev/sda1 をlinuxにして、 /dev/sda2 を0c (fat32)にする。サイズは適当（４Ｇ＋４Ｇくらい）
		# mdev -s
		# mke2fs -j /dev/sda1
		# newfs_msdos /dev/sda2
		[ALT]＋[F1]
- 再度、インストーラーを起動して、/dev/sda1 にAndroidをインストールしておく。

<!-- dummy comment line for breaking list -->

    - わざわざ/dev/sda1にインストールしておくのは、LiveCDから起動したとき用に、設定を永続化するためであって、HDD起動するためではないらしい。

<!-- dummy comment line for breaking list -->

- 再起動させて、こんどはメニューの一番上(LiveCD)で起動する。
    - （HDDインストールしたものを起動しても、'#'プロンプトから先へすすまない。）

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/andsun2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/andsun2.jpg) 

- 端末ロックのような画面が出るので、下の棒（鍵アイコン）を上方向にマウスドラッグして解除する。
- 設定メニューを出して、SDカードがマウントされていることを確認。
- ロケールを日本語にする。
- 「谷歌」と書かれた変なＩＭＥは（中国語？）邪魔なのでチェックを外しておく
- 「アプリケーション」設定で、「提供元不明のアプリ」にチェックを入れる。
- google検索で 「AndAppStore client」を探して、AndAppStoreをインストールする。
- AndAppStoreを起動して、アプリケーションをダウンロードできるようになる。

<!-- dummy comment line for breaking list -->


<!-- dummy comment line for breaking list -->


<!-- dummy comment line for breaking list -->

~
~
~

~
~

やや古い情報
- - - -

## Android-x86
- [http://www.android-x86.org/](http://www.android-x86.org/) 

<!-- dummy comment line for breaking list -->




- ２０１０年８月にandroid v2.2 froyoのisoが公開されている。
- このisoは遅くて使えないので、３月の**v1.6をVMWareで動かす**テスト。

<!-- dummy comment line for breaking list -->

~
結構長いかも。

- - - -
~
- （１）まず、VMWarePlayer(無償)3.0以上を入手。
- （２）android-x86-1.6-r2.iso　mar19-2010を上記downloadサイトから入手。
- （３）VMWarePlayerを起動して、新しい仮想マシンを作成する。
    - OSはOther Linux (カーネル2.6)を選ぶ。
    - OSをあとでインストールにする。
- （４）次に、作った仮想マシン設定の編集を選んで、(SCSI)ハードディスクを一旦削除して、(IDE)ハードディスクを作る。
- （５）CD/DVD-ディスクに自分がDLしたドライブ：ディレクトリ¥android-x86-1.6-r2.isoを選んで、「仮想マシンの再生」

<!-- dummy comment line for breaking list -->


~
~
- - - -
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and1.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and1.png) 

- （６）CD/DVDからGrubが起動するので、一番下のInstallを選ぶとLinuxのインストーラ画面になる。

<!-- dummy comment line for breaking list -->

- （７）ここで、[ALT]+[F2]を押すと、コマンドラインが使えるようになるので、

		# fdisk /dev/sda
- とやって、ext3パーティションをsda1,sda2の２つを４Ｇ＋４Ｇくらいのサイズで切る。
- ついでに

		# mke2fs -j /dev/sda1
		# mke2fs -j /dev/sda2
- までやっておく。
- （８） [ALT]+[F1]を押すと、元のLinuxインストーラー画面に戻るので、/dev/sda1にAndroidをインストールして再起動。
- （９）こんどはHDD起動のGrub画面になるので、デフォルト状態で起動すればAndroidが起動する。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and2.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and2.png) 

~
~
~
- - - -
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and4.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and4.png) 


- ここまででなんとなく使えているような感じがするが、SDCARDが未接続になってつまらないので、SDCARDを/dev/sda2に割り当てる処理を行う。

<!-- dummy comment line for breaking list -->


- （１０）Androidが動いている状態なら、[ALT]+[F1]を押してプロンプトに切り替え、

		# reboot -p
    - を実行する。（ちなみにプロンプトからGUIに戻るキーは[ALT]+[F7]）
- （１１）再度VMWareを立ち上げて、HDD起動直後のGrub画面で[C]キーを押す。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and2.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and2.png) 

- （１２）Grubコマンドが入力できる状態になるので、

		grub&gt; reboot
    - と打って、つぎに、**素早く**[F2]を（長押し気味に）押す。（互換機のＢＩＯＳ画面を出す）
    - 失敗したら、何回か[C] , reboot , [F2] を練習する。

<!-- dummy comment line for breaking list -->

- （１３）ＢＩＯＳ設定を操作して、起動デバイスにCDROMが最優先になるようにする。（カーソルをCDROMにあわせてテンキーの[＋]を押すと、起動デバイスの位置が１つ上にずれる）

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and6.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and6.png) 


- （１４）ＢＩＯＳを「save and exit」で抜ける。CDROMはさきほどのandroid-x86-1.6-r2.isoをマウント。
- （１５）ここまでで、最初のCDROM起動インストーラー画面にまで戻せたことになる。

<!-- dummy comment line for breaking list -->


~
- - - -
一休みして、



- （１６）もう一回、Installerを選んで、Linuxのインストーラーが起動したところで、また、[ALT]+[F2]を押す。
    - プロンプトが出るので、
    
    		# mkdir media
    		# mount /dev/sda1 media
    - とやって、インストール済みのルートディレクトリを覗けるようにする。

<!-- dummy comment line for breaking list -->

~
~
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and3.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and3.png) 

- （１７）media/grub/menu.list を **vi**で編集する。

		# vi media/grub/menu.list
    - linuxのカーネルコマンドのSRC=.... の後ろに1文字空白をあけて、
    
    		SDCARD=/dev/sda2
    - を追記する。（その行の右端で[I]をおして、カーソル右いれて、上記SDCARD=...を書く）
- （１８） [ESC] , [:] , [w] , [q] のようなviキー操作でファイルをセーブして終了。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and5.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and5.png) 

- （１９）[ALT]+[F1]でLinuxインストーラーにもどり、インストールはせずにCANCELを選んでリブート。
    - リブートするときはVMWare側でCDROMを切断しておくと、HDDからGrubが立ち上がる。

<!-- dummy comment line for breaking list -->

~
これでやっと4GB程度の（仮想的な）SDCARDが使えるようになるはず。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and4.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/and4.png) 

~
追伸：
- froyoの遅さをなんとかして欲しい。
- ↑Performance問題は修正が入っているようですが、isoファイルは公開されてないっぽいので、自分でビルドするしかなさそうです。

<!-- dummy comment line for breaking list -->







~
~
~
~
~
~

~
~
~
以下、古い情報
- - - -

2009-11-15: Google has pushed in Eclair source code to open source repo, now the Android-x86 source tree is totally broken. We are trying to fix it now.

だそうだ・・・。

~~あと、近々ChromeOSも出るらしい。~~ ---> ソースとVM Imageが公開されました。

選択肢としては、
- １年後にChromium OS搭載のネットブックが出る予定なのでそれを購入すると、たぶん**７秒起動のChromeブラウザだけ**がもれなく使用可能になる。HTML5対応になっているけれどブラウザ上のJava|JavaScriptアプリしか使えない。

<!-- dummy comment line for breaking list -->

- [Android-x86](http://www.android-x86.org/) にすると、Chromeの外側で、Androidアプリ(Java)も使える。軽量。但しx86限定だ。

<!-- dummy comment line for breaking list -->


- ubuntu上のChromeを使う。これが一番まとも。x86とARMの両方選べるはず。ARMは将来Atomより速くなって(2GHz2core)いるかも。

<!-- dummy comment line for breaking list -->






~
~
~


- - - -
## Android-x86 (Donut)

android-x86-20091024.iso の入手先
- [http://www.android-x86.org/](http://www.android-x86.org/) 

<!-- dummy comment line for breaking list -->

Android-x86で各種設定、アプリケーションインストール 
- [http://sarl-tokyo.com/wiki/index.php?Android-x86%E3%81%A7%E5%90%84%E7%A8%AE%E8%A8%AD%E5%AE%9A%E3%80%81%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB](http://sarl-tokyo.com/wiki/index.php?Android-x86%E3%81%A7%E5%90%84%E7%A8%AE%E8%A8%AD%E5%AE%9A%E3%80%81%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB) 

<!-- dummy comment line for breaking list -->

- VMWarePlayerで動かしてみた。
- AndroidはIDE0:0に仮想ドライブを作っておかないとインストールは出来ない。（Live起動なら出来る）
- VMWareでは通常のvmdkはscsiなので、VMX Builderなどで、IDEデバイスを作る必要がある。
- また、(VMWare内で使用する)BIOS設定でIDE DeviceをBoth ONにしておかないとIDE0を認識できない。

<!-- dummy comment line for breaking list -->

- このビルド（android-x86-20091024.iso）は非常に安定していて、マウスもちゃんと使えるしAndAppStoreもアクセスできるし、Etherも認識する。おすすめ。

<!-- dummy comment line for breaking list -->


- SD CARDも使える(grubの起動パラメータにSDCARD=/dev/sda2などと書く)らしいけれど、自分のところでまだうまくいっていない。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## 他のLinux
- ARMで動かしたいとか、googleのブラウザを使いたいとかではなくて
- 単に小さなLinuxが欲しいだけなら、いくらでもある。
- まず、VMWarePlayer上で試してみればよい。

<!-- dummy comment line for breaking list -->

- [http://distrowatch.com/](http://distrowatch.com/) 

<!-- dummy comment line for breaking list -->

~
~
以下のTinyCore というLinuxのisoファイルは、わずか10MBしかないけれどＸが使えるし、~
パッケージインストールも出来る。

- [http://distrowatch.com/?newsid=05757](http://distrowatch.com/?newsid=05757) 

<!-- dummy comment line for breaking list -->


~
~
~
~
~
~
~
~
~
~
~
~



以下、古い情報
- - - -

## Live-Androidをダウンロードする。

[http://code.google.com/p/live-android/](http://code.google.com/p/live-android/) 

- rarファイル３つをダウンロード。
- Linux上でunrarで展開する。

		$ unrar e androidv0.1.part1.rar
- unrar コマンドが無いときは

		# apt-get install unrar
- 残り２つのrarは勝手に見つけて展開してくれる。
- 展開されたファイルは

		-rw-r--r-- 1 user user 156487680 2009-05-08 06:23 androidv0.1.iso
- これをVMWareでマウントする。
- VMWareに登録してある適当なLinux（ubuntuとか)のvmxを少し書き換えて上記

<!-- dummy comment line for breaking list -->
isoをCDROMとしてマウントする。
- VMWareに登録してある適当なLinuxを再起動し、
- grubメニューが出たところで[ESC]とか'c' を押して grubのコマンド入力プロンプトにする。

		grub ....
		&gt; reboot
		  ~~~~~~下線部を入力してリターンを押した直後に素早く[F2]を押して
		  PHENIX BIOSのbIOSメニューを出す。
- BIOSのBOOTメニューの起動デバイスの順序を変えてCDROMブートにする。
    - CDROM をカーソルで選んでテンキーの「＋」を押すと起動順位が上がる。
    - BIOSの設定変更を有効にして再起動する。

<!-- dummy comment line for breaking list -->

以上でAndroidが起動する。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/android1.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/android1.png) 

~
- EeePC701ベースらしい。一応x86ビルド。
- isoイメージは156MB
- RAM64M以下だとGUIに来ない？？？512MBとかubuntuのままにしておくと速い。
- ALT+F1,ALT+F7でCUI,GUI切り替え。（これはLinuxと同じ）
- Development toolの中にbusyBox(lsとかcatとか必要最小限のコマンドを１つのelfに固めたもの)のようなものがあり、まるで**組み込みLinuxを使っている気分**。

<!-- dummy comment line for breaking list -->

- 要は、カーネル以外は**Linuxシステムではない**わけで、ユーザーランドはまるでbuffalo提供の玄箱LinuxとかNTTMEのLivingGateとかのユーザーランド程度のものだ。

<!-- dummy comment line for breaking list -->

	だから起動が速くて軽いわけで・・・。


疑問点
- 日本語化は出来る。(ロケールとかの設定)
- softキーボードタスクがすぐ死ぬ。softキーボードに一度もお目にかかれない。
- マウスが使えない。カーソルキーとENTER,ESCキー程度のみ使用できる。
- ＬＡＮが使えない。

<!-- dummy comment line for breaking list -->

ちゃんと動くVMWareのイメージは配布されていない？
- [http://www.android-group.jp/index.php?plugin=attach&refer=%A5%EF%A1%BC%A5%AD%A5%F3%A5%B0%A5%B0%A5%EB%A1%BC%A5%D7%2F%B6%E2%C2%F4%BB%D9%C9%F4%2F%C2%E8%B0%EC%B2%F3%CA%D9%B6%AF%B2%F1&openfile=VMWare%A4%C7x86%C8%C7Android.pdf](http://www.android-group.jp/index.php?plugin=attach&refer=%A5%EF%A1%BC%A5%AD%A5%F3%A5%B0%A5%B0%A5%EB%A1%BC%A5%D7%2F%B6%E2%C2%F4%BB%D9%C9%F4%2F%C2%E8%B0%EC%B2%F3%CA%D9%B6%AF%B2%F1&openfile=VMWare%A4%C7x86%C8%C7Android.pdf) 
- 作り方だけは上記pdfに記されている。

<!-- dummy comment line for breaking list -->

~
第1回　話題の携帯向けOS「Android」をx86パソコンで動かしてみよう 
- [http://itpro.nikkeibp.co.jp/article/COLUMN/20090219/325052/](http://itpro.nikkeibp.co.jp/article/COLUMN/20090219/325052/) 
- こちらで紹介されている日経のページからＤＬするほうのimageをUSBメモリーに焼いてみた。
- 書き込みにわざわざ生Linuxを立ち上げるのも面倒なので、Windowsで使えるddを使ってみた。

<!-- dummy comment line for breaking list -->

DD for Linux
- [http://www.si-linux.co.jp/wiki/silinux/index.php?DDforWindows#wcba113d](http://www.si-linux.co.jp/wiki/silinux/index.php?DDforWindows#wcba113d) 
- しかし、自分のマザーボードではUSBを/dev/sdbにすることが出来ないらしい。
    - ブートの優先順位で選択できずにSATAかUSBかの２択になっていて、USBだとたぶん/dev/sda/なのだろう。

<!-- dummy comment line for breaking list -->

- 起動させてみると、ブートの初期段階だけは実行するものの、/dev/sdbをマウントできずにハングする。
- そもそもroot=/dev/sdb決め打ちのビルドって、どういうことよ。

<!-- dummy comment line for breaking list -->




~
~
~
- - - -
## 比較対象は何？
これと比べるべきものはなんだろう。
- LinuxのようでLinuxではない。Linuxサブセット＋αのようなもの
- PDA向けOS

<!-- dummy comment line for breaking list -->

ということなら
- iPhone
- WindowsCE
- Palm
- Psion

<!-- dummy comment line for breaking list -->

この中でいうと、おそらく比喩としてWindows2000|XP ---> WindowsCE~
に対する Linux(ubuntu)-->Android~
という比較が一番近いと思われる。

- サブセットである。
- 携帯機器向けにカスタマイズされたものである。
- メモリー要求量やＣＰＵ性能もわりと近い。(ARM 400MHz程度が最低レベルである)

<!-- dummy comment line for breaking list -->

しかし、現在ではいわゆるNetBookではどちらもフルセットのほうのＯＳ(XPあるいはubuntu)がそれなりに動くようになったので、**あえてサブセットＯＳを使う場面があるのだろうか**という疑問もある。

ＯＳ自体の軽さ、とか起動の速さとか、メモリーリソースの制約などがそれほどきつくないのであれば、
普通にubuntuを入れて、**VMWare上でAndroidを楽しめば良い**だけなのではないかとさえ思ってしまう。

~
~
~
~
- - - -
## AndroidのCupcakeロードマップ公表—ビデオ録画、ステレオbluetoothなどを準備
- [http://jp.techcrunch.com/archives/20081218cupcake-roadmap-tells-tales-of-video-recording-stereo-bluetooth-and-more-on-the-way-for-android/](http://jp.techcrunch.com/archives/20081218cupcake-roadmap-tells-tales-of-video-recording-stereo-bluetooth-and-more-on-the-way-for-android/) 

<!-- dummy comment line for breaking list -->

- そもそもAndroidはARM系 CPU を採用した携帯電話もしくはPDA向けのＯＳなんだけれど
- x86なNetBook（たとえばEeePC）上でもちゃんと動くようにしようというのがcupcakeというブランチだ。

<!-- dummy comment line for breaking list -->

~
そいつがある程度成功すればＴＶのセットトップＢＯＸのＯＳとしても注目されるようになる（かもしれない）
- ＴＶのセットトップＢＯＸというのは、ＴＶでＷｅｂが見れたり、Ｗｅｂで提供されている動画（YouTubeなど）が見れたりする機能ＢＯＸのことだ。
- 現在販売されているもので代表的なのはＡｐｐｌｅのＡｐｐｌｅＴＶだ。

<!-- dummy comment line for breaking list -->

- このまま地上波のテレビ局が衰退していけば、いずれは**Ｗｅｂ上から見る動画を流すことがテレビの基本機能になる**と考えられる。

<!-- dummy comment line for breaking list -->

- セットトップＢＯＸのＣＰＵはむしろARMで充分なので、家電メーカーが採用する頃にはcupcakeから本家へのバックポートが利用されるだろうし、その頃にはARMと組み合わせて使えるビデオチップもこなれていることだろう。

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## Androidの応用例
必要なリソース
- ARMアーキテクチャーのCPU(MMU必須) クロックは400MHz程度
- SDRAM 64MB〜128MB程度
- FlashROM 128MB〜

<!-- dummy comment line for breaking list -->

何に使えるか
- PDA
- デジタルガジェット -- [Chumby](http://www.chumby.jp/) のようなもの
- 携帯電話 -- iPhoneのようなもの
- TVセットトップBOX -- Web TV
- 勝手SkypePhoneのようなもの
- Webラジオ(streaming)
- これが台風の目かも。「携帯ゲーム機」
    - OpenGL ESとLinuxが乗っているなら、即Mameとかエミュを動かしたくなるのが海外のハッカーさん達だ。
    - ARMで400MHz〜600MHzとかだったら下手するとSONYやNINTENDOの現行機よりも性能が上だ。((しかも、SONYやNINTENDOの現行機の（ハードメーカーへの）ロイヤリティビジネスモデルの縛りがないんだったら自由にゲームが出せるわけで・・・（ヤバイヨ）ただGoogleだってそこまで野放しにしないと思うけれど、勝手ゲーム機を中国のパチモンメーカーが作ってしまって大ヒットしたら日本は終わりかもね。))

<!-- dummy comment line for breaking list -->

とりあえず無線LANに繋いで
- Webブラウザーになったり
- 動画見たり
- するようなものを作るときのフリーＯＳとして便利かも。

<!-- dummy comment line for breaking list -->

しかしGoogleはどうやって元を取るつもりなんだろうか。
- 無償提供は社会貢献なのか？-- まあApacheライセンスだし、そもそも基本的な技術はLinuxとApacheから来ている。--まとめるのは大変だったと思うけれど。
- （将来有料になるかもしれない）各種検索サービスで元を取るつもりなのか？
- MicrosoftのOS寡占を嫌っているだけ？

<!-- dummy comment line for breaking list -->

~
~
~
~
- - - -
## 参考リンク
ソースコードから読み解く Android の実像 
- [http://coin.nikkeibp.co.jp/coin/ncc/semi/0812/Android_20081211_Tsuji.pdf](http://coin.nikkeibp.co.jp/coin/ncc/semi/0812/Android_20081211_Tsuji.pdf) 

<!-- dummy comment line for breaking list -->

これを使えばどんな遊びができる？Android Live CD「live-android」
- [http://www.moongift.jp/2009/06/live-android/](http://www.moongift.jp/2009/06/live-android/) 

<!-- dummy comment line for breaking list -->

第1回　話題の携帯向けOS「Android」をx86パソコンで動かしてみよう 
- [http://itpro.nikkeibp.co.jp/article/COLUMN/20090219/325052/](http://itpro.nikkeibp.co.jp/article/COLUMN/20090219/325052/) 

