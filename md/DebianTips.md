[2009-11](2009-11.md) 

## debian lennyの中でubuntuを飼うやりかた。

Linuxを使っていると、時々自分専用のテスト環境が欲しいと思うことがあります。
- 自分専用の環境というのは、テスト版ツールに差し替えてビルド＆動作確認するとかそんな感じです。
- システムの根幹部分を差し替えてテストするときなどは、単に実行パスを設定しなおすとかでは対応出来ないので・・・。

<!-- dummy comment line for breaking list -->

選択肢としては、
- （１）それ専用にマシンを組み立てる。
- （２）VMWareを使ってWindowsの中でLinuxを飼う。
- （３）Xenを使ってLinuxマシンを仮想化し、複数のLinuxを入れる。
- （４）既存のLinux上でschrootを使ってユーザーランドだけ差し替える。

<!-- dummy comment line for breaking list -->

~
（１）はそれなりに費用と設置場所が掛かります。一時的に使うのに場所と電気を占有するのは気が引けます。

（２）は手軽なのですがマシンパワーが不足します。普通のサーバーマシンでフルビルドに１時間くらい掛かる程度のことをWindows上で行うと遅いです。

（３）については今のところ仮想化についての詳しい知識がありませんので保留です。既存のLinuxにXenを導入するのに少しためらいがあります。

（４）は一番簡単です。作成したサンドボックスをバックアップしたり破棄したりもわりと簡単そうです。使わないときは場所も電気も消費しません。

#### 今回は、schrootを使って、既存のDebian lennyの中にubunt-9.10 を入れてみました。

~
#### やりかた

- debootstrapとschrootをインストールする。

		# apt-get install debootstrap schroot
- ubuntu(kermic)のシンボリックリンクを作っておく。

		# cd /usr/share/debootstrap/scripts
		# ln -s gutsy karmic
- ここにすでにkarmicが存在している場合は不要です。
- ~
- ~
- ubuntu-9.10 を /guestos/ubuntu/ にインストールする.

		# cd /
		# mkdir /guestos
		# mkdir /guestos/ubuntu
		# debootstrap --arch i386 karmic  /guestos/ubuntu/ \
		              http://jp.archive.ubuntu.com/ubuntu/
- ネットワーク経由のインストールになります。
- ubuntu-9.10を入れたところ、約３００ＭＢほどのディスクスペースを消費しました。
- ~
- /etc/schroot/schroot.conf に追記する。

		[ubuntu]
		description=ubuntu 9.10
		location=/guestos/ubuntu
		priority=3
		users=自分のユーザー名
		groups=自分のグループ名
		root-groups=root
- schrootしてみる。

		# schroot -c ubuntu
- ゲストubuntuの中から、自分のアカウントを作る。

		# adduser ユーザー名 --uid ＜ホストOS上の自分のuid＞
		# passwd ユーザー名
- uid,gidはホストＯＳと同じにしておきます。
- ユーザー権限でubuntuにchrootする。

		$ schroot -c ubuntu
- これでlennyの中のubuntuに切り替わります。
- 自分がどちらの環境にいるか分からなくなりますので、ホスト名を変える

<!-- dummy comment line for breaking list -->
( edit /guestos/ubuntu/etc/hostname )とかプロンプトを書き換えます。

~
~

ここまでで問題点
- procファイルシステムが存在しません。
    - ホストOS側の procを別のところ(/guestos/ubuntu/proc )にmount ？
    - えーと、ubuntu内から procfsをマウントすると出来ました。

<!-- dummy comment line for breaking list -->

	# mount -t proc proc /proc

- debootstrapで導入したubuntuは最小構成に近いので、パッケージが不足です。
    - ubuntu内から /etc/apt/source.list を書き換えて、フルパッケージのapt-getが出来るようにします。

<!-- dummy comment line for breaking list -->



- いくつかのパッケージ依存がおかしいです。

		rsyslog,ubuntu-minimal
    - 以下の処置をした後、# apt-get update を実行すると直ります。

<!-- dummy comment line for breaking list -->

- syslogが起動できないのでapt-getがエラーします。

		(CHROOTED-ENV) /etc/init.d/syslog
- の最初のほうに

		exit 0;
- これで、apt-get出来るようになりました。

