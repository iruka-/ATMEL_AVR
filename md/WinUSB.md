[PIC32MX](PIC32MX.md) 

## WinUSBについて

一次情報 Microsoft
- [http://msdn.microsoft.com/ja-jp/library/windows/hardware/gg487341.aspx](http://msdn.microsoft.com/ja-jp/library/windows/hardware/gg487341.aspx) 

<!-- dummy comment line for breaking list -->


- - - -
USB デバイスを製造するハードウェア メーカーは通常、アプリケーションがデバイスの機能にアクセスする方法を提供する必要があります。そのために、かつては、Windows Driver Model (WDM) を使用してデバイスのファンクション ドライバーを実装し、デバイス スタック内でシステム提供のプロトコル ドライバーの上にドライバーをインストールする必要がありました。現在、Windows Driver Foundation (WDF) が USB ドライバーの推奨モデルです。WDF は、USB デバイスへのアクセスを提供するため、下記の 3 つのオプションを提供します。

- WDF ユーザー モード ドライバー フレームワーク (UMDF) を使用して、ユーザー モード ドライバーを実装する

<!-- dummy comment line for breaking list -->

- WDF カーネル モード ドライバー フレームワーク (KMDF) を使用して、カーネル モード ドライバーを実装する

<!-- dummy comment line for breaking list -->

- デバイスのファンクション ドライバーとして WinUsb.sys をインストールし、WinUSB API を使用してデバイスにアクセスするアプリケーションを提供する
- - - -



~
    - いつものことながら、Microsoftの中の人は、何言ってるのかさっぱりわけわかんないので翻訳。

<!-- dummy comment line for breaking list -->

~

- Windowsで自作USBデバイスのドライバーを書くには、いままでは**DDK(デバイスドライバー開発キット)と格闘しながら苦労して**書く必要があったんだけど、
- WinUSB.DLLを使ってくれるなら、**デバドラ書かなくても**自作USBデバイスを制御できるよん。

<!-- dummy comment line for breaking list -->

以上。

~
- **え？それって、これまでの[[LibUSB-win32:http://sourceforge.jp/projects/sfnet_libusb-win32/]]とどこが違うの？**

<!-- dummy comment line for breaking list -->

    - 本質的な違いは無いです。
    - WinUSBはWindows Vista以降で使用できる、microsoftが自ら実装した**LibUSBもどき**です。
    - WindowsXPでもWinUSBドライバーをインストールすれば使えます。

<!-- dummy comment line for breaking list -->

~
~
~
## では、さっそく実践編

- その１：[PIC32MXでUSBカスタムデバイス](USB_CUSTOM.md) 

<!-- dummy comment line for breaking list -->

~
- その２：[USB簡易モニター](PIC32mon.md) 

<!-- dummy comment line for breaking list -->

~
- その３：[USBオシロスコープ](USBオシロスコープ.md) 

