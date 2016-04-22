[stm32f103](stm32f103.md) 　[ARM](ARM.md) 

## DFUとは何か？

DFUはUSB.orgで定義されている。

## .dfuファイルのフォーマットもドキュメントされていた。

- [http://www.usb.org/developers/devclass_docs/usbdfu10.pdf](http://www.usb.org/developers/devclass_docs/usbdfu10.pdf) 

<!-- dummy comment line for breaking list -->

遷移

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/dfustat1.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/dfustat1.png) 

キモになるのはこれだけ？

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/dfustat2.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/dfustat2.png) 

全部クラスリクエストで処理

~
~
~
## STM32のサンプルアプリケーション：dfuの概要

- USBディスクリプタの一部抜粋.

		   /************ Descriptor of DFU interface 0 Alternate setting 0 *********/
		   0x09,   /* bLength: Interface Descriptor size */
		   0x04,   /* bDescriptorType: */
		   /*      Interface descriptor type */
		   0x00,   /* bInterfaceNumber: Number of Interface */
		   0x00,   /* bAlternateSetting: Alternate setting */
		   0x00,   /* bNumEndpoints*/
		   0xFE,   /* bInterfaceClass: Application Specific Class Code */  ＜＝＝ここがそれ.
		   0x01,   /* bInterfaceSubClass : Device Firmware Upgrade Code */ ＜＝＝ここがそれ.
		   0x02,   /* nInterfaceProtocol: DFU mode protocol */
		   0x04,   /* iInterface: */
- bInterfaceClassは

		CDCなら2,
		HIDなら3,
		DFUなら0xFE
- らしい。

<!-- dummy comment line for breaking list -->

	
- エンドポイントは無い。

		bNumEndpoints=0
- 全部コントロール転送で済ませているらしい。

<!-- dummy comment line for breaking list -->

- (HIDの)インタラプト転送ではなくて、コントロール転送だけでファームの書き換えをやろう、っていうんだから~~多分遅いと思う。~~

<!-- dummy comment line for breaking list -->
~
- ↑ 確かめたところ、コントロール転送の後続パケットの長さを充分長く取っているので、インタラプト転送（64kB/s）とあまり変わらないようです。
- もしも後続パケットが64byte以下だったとしたら、この半分（32kB/s）しか転送できません。というのは、コントロール転送のパケットは1mSのフレーム時間に対して１回(64byte以下)しか送れないようになっているからです。（しかも最初のパケットにはデータが乗らないので）

<!-- dummy comment line for breaking list -->


- しかし、いずれにせよコントロール転送を使う限りは、最大転送速度は64kB/sを超えられません。(NECやSiSのUSBコントローラーを使う場合を除く)

