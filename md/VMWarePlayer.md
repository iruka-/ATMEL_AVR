[2009-06](2009-06.md) 

## VMWarePlayerに他のＬｉｎｕｘを入れる方法：

#### VMWare de Debian
- VMWarePlayerにデビアンを入れる方法
- すぐにやり方を忘れてしまうのでメモ。

<!-- dummy comment line for breaking list -->


#### VMWarePlayerで配布しているBrowser-Appliance.vmx をダウンロードしてきて展開する。

	D:¥VMWare> unzip Browser-Appliance-*.zip

#### これを普通にダブルクリックすると英語版最小LinuxなUbuntuのデスクトップが立ち上がる。

#### これから始めてDebian-Etchにする方法についてのメモ。

	VMWareのメニュー-->トラブルシューティング-->リセット

により、Ubuntuを再起動掛ける。

#### ブートローダーである、grubのメニューが出るので、[ESC]を押す。

#### rebootを入れて、再度grubが起動する前に[F2]を長押しする。

#### するとPhoenix BIOSのメニューが出る。

#### ブートデバイスの優先順位をＣＤＲＯＭ（ＤＶＤＲＯＭ）が先頭になるように変更する。（CDROMを選んで＋を押すと上位に移動）

#### ＢＩＯＳをSave & Exitで抜ける。

#### この状態でＤＶＤＲＯＭにDebianのインストールＣＤ（ＤＶＤ）を入れておけばDebianのインストーラーが立ち上がる。

#### ＤＶＤを焼くのが面倒な人は、Browser-Appliance.vmx をテキストエディタで編集し、ＣＤＲＯＭデバイスを実デバイスからＩＳＯイメージファイル名に変えておく。

	ide1:0.present = "TRUE"
	ide1:0.fileName = "D:\vmware\debian-40r4a-i386-DVD-1.iso"
	ide1:0.deviceType = "cdrom-image"

#### ＰｈｏｅｎｉｘＢＩＯＳの出し方＝grubで[ESC]、rebootで[F2]というのがミソだ

- 他はだいたい誰でも知っているはず。 [#idc332d9]

<!-- dummy comment line for breaking list -->

- grubメニューが出る前にフォーカスＯＮにして[F2]が押せる人は、このガイダンスは不要。

<!-- dummy comment line for breaking list -->


### インストール後は、Linux側にVMWare-Toolsを入れておくと、画面解像度がWindowにあわせて自動追従になったりサスペンド復帰後に時計を同期してくれたりと、とても便利になるのでお勧め。

- 画面解像度が一定以上に上がらない場合は、*.vmx を編集して、svgaの最大スクリーンサイズを自分の好きな大きさに変えておく。

