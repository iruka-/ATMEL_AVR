[PIC32MX_USBCDC](PIC32MX_USBCDC.md) 


- - - -
## PIC32MXでUSB高速カスタムデバイスを実装してみる。
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/pic32mx220.jpg) 


    - MicroChipのUSBフレームワークの「USB Device - WinUSB - High Bandwidth 」を試してみます。
- これは、Windowsで規定されているどのクラスドライバーにも属さない、いわゆるバルクデバイスになります。
- Windowsドライバーは標準のものが存在しないのでカスタムで作るしかないのですが、WinUSBというWindowsに用意されたDLLを呼び出すことで、LibUSBとほぼ同じように手軽にアプリケーションを書くことも出来ます。

<!-- dummy comment line for breaking list -->

- では、早速MX220F032Bに移植してみました。

<!-- dummy comment line for breaking list -->

~


**ダウンロード**
- [usbcustom.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/usbcustom.zip) 

<!-- dummy comment line for breaking list -->

- 上記アーカイブは例によってブートローダー起動、PicKit2ライター直接書き込みの両対応HEXになっています。

<!-- dummy comment line for breaking list -->

- 書き込んだら、Windows_Infにあるドライバーを組み込んで、HighBandwidthWinUSB.exe を起動します。

<!-- dummy comment line for breaking list -->

- ＧＵＩのボタンを押すとスループットを計測してくれます。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/winusb-bench.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/winusb-bench.png) 

- 毎秒１ＭＢのスループットが実現できました。
- FullSpeed USBの信号の変調速度は12M b.p.sなので、ほぼ理論値に近い転送が行なわれています。
    - 実際にはUSBパケットのプリアンブルやチェックサムなどのオーバヘッドと、連続した1が並んだ場合のbit stuffingが行なわれますので、それを加味しても充分に速いといえます。

