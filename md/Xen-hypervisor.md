[2012-02](2012-02.md) 



## 仮想化のススメ：Xen-hypervisorを使ってみよう。

- デスクトップＯＳで使う仮想化ソフトはVMWare(Player)やVirtualPC(Windows7Pro)、Oracle VirtualBoxなどがあります。

<!-- dummy comment line for breaking list -->

- これは、一言で言うとOSの中で別のOSを動かすというものです。
- Windows7(64bit)のXPモードは、VirtualPCという仮想化ソフトからWindowsXPをほぼそのまま動かすようになっています。
- Linuxをちょとだけ使いたいときも、VMWare(Player)を使えばWindows(XP|Vista|7)のＯＳ内から簡単にLinuxをインストールして気軽に使うことができます。

<!-- dummy comment line for breaking list -->

~
仮想化の便利な点は、たくさんあります。
- 専用にハードウェアが要らない。
    - ゲストＯＳごとに実在ＰＣをわざわざ用意しなくてもよい。
- 仮想イメージの引越しが簡単。
    - 実在ＰＣにＯＳを物理インストールしたものは、マシン不調時やグレードアップ時にＨＤＤを移しても動作しなかったりデバイスドライバーをいちいち用意するのが面倒ですが、仮想イメージは単なるファイルなのでＯＳごとコピーしたりバックアップしたり、他のＰＣで動かしたりなどと簡単です。
- 失敗しても気軽に戻せる。
    - ＯＳのアップグレードや、大規模な設定変更など引き返せないような修正を入れたいときなども、仮想イメージのコピーさえとっておけば、失敗しても仮想ファイルを書き戻すだけで気軽に元に戻せます。
- ＯＳのインストール、再起動が速い。
    - 実在ＰＣで行なうよりも充分速いです。

<!-- dummy comment line for breaking list -->

~

## Xen-hypervisorを導入する。
- Xen-hypervisorはオープンソースの、Linuxホストで動作するＯＳ仮想化ソフトです。
- ubuntu 11.10 をお使いであれば、apt-get経由で簡単に導入できます。

		# apt-get install xen-hypervisor xen-utils virtinst virt-viewer vnc4server
- *xendデーモンの設定 [#c5efa4e4]
- /etc/xen/xend-config.sxp を編集

<!-- dummy comment line for breaking list -->

	(xend-http-server no)
	    ↓
	(xend-http-server yes)

- xendデーモンを再起動

		# service xend stop
		# service xend start
- *ubuntu上にVineLinux4.2 をインストールする。 [#cfa7d1e4]

		# dd if=/dev/zero of=/mydir/domains/vine4/disk.img bs=4096k count=0 seek=1024
		# virt-install \
		 --hvm \
		 --name=vine4 \
		 --ram=512 \
		 --vcpus=1 \
		 --file=/mydir/domains/vine4/disk.img \
		 --file-size=4 \
		 --nonsparse \
		 --network=network:default \
		 --network=bridge:xenbr0 \
		 --cdrom=/mydir/Vine42-i386.iso \
		 --os-type=linux \
		 --os-variant=generic26 \
		 --vnc \
		 --keymap=us
- Vine42-i386.iso は、VineLinux配布サイトから落としてくる。
- hvm（完全仮想化ＶＭ）をインストールするときはubuntuデスクトップ上で実行するか、リモートマシンの場合はあらかじめvncserverを立ち上げておいて、Windowsなどのvncクライアントから操作する。

<!-- dummy comment line for breaking list -->

## VineLinux4.2 hvmからpvm（準仮想化ＶＭ）へ移行する。

[手順概要]：Vine Linux 4をDomainUで実行させる方法については、以下のページが妙に**詳しい。**

- [http://www.os-museum.com/linux/xen/xen3vine4u/xen3vine4u.htm](http://www.os-museum.com/linux/xen/xen3vine4u/xen3vine4u.htm) 

<!-- dummy comment line for breaking list -->

- FC6のカーネルを利用する方法は自分の環境ではうまくいかなかったが、xen-3.0.3_0-install-x86_32p.tgz

<!-- dummy comment line for breaking list -->
を利用するやりかたではばっちり出来た。

- xen-3.0.3_0-install-x86_32p.tgzは以下のようにして取得する。

		$ wget http://bits.xensource.com/oss-xen/release/3.0.3-0/bin.tgz/xen-3.0.3_0-install-x86_32p.tgz
- *VineLinux4.2 物理ＨＤＤからpvm（準仮想化ＶＭ）へ移行する。 [#mba0c52a]
- ddで/dev/sda1を丸ごと引っ越す場合もあるかもしれないけれど、
- 今回は、稼動中のLinuxからtar でルートファイルシステムを１ディレクトリ単位で固めて引越ししてみる。

<!-- dummy comment line for breaking list -->

上記の応用。（上記pvmのイメージから始めます。）
- 上記で作成したdisk.imgをコピー。
- それを適当なマウントポイントにマウントしてubuntuから操作出来るようにする。

		# mkdir /mnt/vine42
		# mount -o loop,offset=32256 disk.img /mnt/vine42
- 要りそうなものは残して不要なものを全部消す。

		# cd /mnt/vine42
		# mv boot boot.pvm
		# mv lib/modules .
		# rm -rf bin sbin root ..... (要は/dev以外ほとんど消す)
- 実在マシンのルートファイルシステムをバックアップしたものを持ってきて展開する。

		# tar xpfz /my-machine/bin.tgz
		# tar xpfz /my-machine/sbin.tgz
		# tar xpfz /my-machine/root.tgz
		 ・・・
- /bootディレクトリは元のpvmのものに戻す。

		# mv boot.pvm boot
- /lib/modules も元のpvmのものに戻す。

		# cp -a modules /lib/
- /etc/sysconfig/hwconf を元のpvmから持ってくる。
- /etc/modules.conf を元のpvmから持ってくる。
- /etc/sysconfig/network-script/ifcfg-eth0 に記述されているHWADDRはコメントアウトする。あと必要ならstatic ipかdhcpかを都合に合わせて変更する。
- /etc/fstab で/以外のパーティションをいろいろマウントしている場合は、とりあえず起動させるために一時的にコメントアウトしておく。
- 終わったらアンマウントする。

		# sync
		# umount /mnt/vine42
- ~
- そして、

		# xm create -c xm-vine42.cfg
- のようにして起動すると物理マシンで動いていた環境をpvmに引っ越すことができた。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## おまけ：ubuntu32 on ubuntu64 (11.10 Oneiric Ocelot)

- （１）IPアドレスを外出しするのであれば、まずブリッジインターフェースxenbr0を作っておく。
- /etc/network/interface に追記

<!-- dummy comment line for breaking list -->

	auto xenbr0
	iface xenbr0 inet static
	       address 192.168.0.2 ★
	       netmask 255.255.255.0 ★
	       broadcast 192.168.0.255 ★
	       gateway 192.168.0.1 ★
	       dns-nameservers 192.168.0.1 ★
	       bridge_ports eth0
	       bridge_fd 9
	       bridge_hello 2
	       bridge_maxage 12
	       bridge_stp off
- ★の付いたところは各環境に合わせて差し替えが必要。
- eth0はちゃんとアドレスを持っていてもいいし、わざとIPを付与しないようにしてもいい。(その場合dom-0がxenbr0と同一アドレスになる。dom-Uはxenbr0とブリッジするので、192.168.0.x (ｘは3以上)とか）

<!-- dummy comment line for breaking list -->

- 外出ししないとか自力でNATするならubuntuが用意してくれているvirbr0を使うとよい。

<!-- dummy comment line for breaking list -->

- （２）4GBの空のdisk.imgを用意

<!-- dummy comment line for breaking list -->

	 # dd if=/dev/zero of=/mydir/domains/ubuntu32/disk.img bs=4096k count=0 seek=1024

- （３）ubuntu32.cfgを作る。

<!-- dummy comment line for breaking list -->

	memory = 512
	name = "oneiric"
	vif = ['mac=00:16:3e:a8:00:03, bridge=xenbr0']
	disk = ['file:/mydir/domains/ubuntu32/disk.img,xvda,w']
	
	#============================================================================
	# Debian Installer specific variables
	・・・以下略

- （４）ネットワークインストール

<!-- dummy comment line for breaking list -->

	# xm create -c ubuntu32.cfg install=true \
	   install-mirror="http://ja.archive.ubuntu.com/ubuntu" \
	   install-arch=i386 install-method=network

- インストールが完了したら即使える。（はず）ほんとか？

<!-- dummy comment line for breaking list -->

- ubuntu64 on ubuntu64にしたいときはinstall-arch=i386のところをamd64に変更する。
- cd-romイメージからインストールするときは、上のVineLinuxの項目にあるように、isoイメージのマウントした場所(ftpかhttpでアクセス出来るURL。名前解決しないでいいようにipアドレスでURLを書く)を指定して、 vert-installコマンドを使う。

<!-- dummy comment line for breaking list -->


~
~
~
## 2015-1-21追記: Ubuntu14.04LTSでpvmを新規作成して起動するには。

- pvmの作成方法は普通通り virt-install で作れる。

<!-- dummy comment line for breaking list -->
~
作成後、起動しない問題
- 基本的には /etc/xen/xlexample.pvlinuxをコピペして書き直せばOKなんだけど落とし穴がある

		name = "example.pvlinux"
		kernel = "/boot/vmlinuz"
		extra = "root=/dev/xvda1"
		memory = 128
		vcpus = 2
		vif = [ '' ]
		disk = [ '/dev/vg/guest-volume,raw,xvda,rw' ]
- これをこんな風にする

		name = "ubuntu1404"
		kernel = "/vmlinuz"
		extra = "root=/dev/xvda1"
		memory = 1024
		vcpus = 1
		vif = [ "mac=00:16:3e:00:73:78,bridge=xenbr0" ] &lt;==環境に合わせて.
		disk = ['file:/var/lib/xen/domains/disk.img,xvda,w'] &lt;==環境に合わせて.
- ~
- インストール後のイメージの中身を見たいならば

		# mkdir mnt
		# mount -o loop,offset=0x100000 disk.img mnt
		# ls mnt/
- ~
- ブートがうまくいかないときは /var/log/xen/ の下にできたログを読む

