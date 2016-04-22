[2011-09](2011-09.md) 

- - - -
## STBee/Miniのファームをとりあえず復旧したいメモ。


|PA9|USART1_TX|
|PA10|USART1_RX|
|PA13|JTMS/SWDIO/UserLED|
|PA14|JTCK/SWCLK/USB_D+のPullUPと兼用|
|PA15|JTDI/UserLED|
|PB2|BOOT1|
|PB3|JTDO|
|PB4|(JNTRST)|

USBコネクタ＜＝ ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/stbeeminipin.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/stbeeminipin.png) 

部品面の配置図。左側がUSB Mini-Bコネクタになっている。

- - - -

チップの向きはこれでいいはず。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/stbeeminisch.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/stbeeminisch.png) 


- - - -
## STBeeのファーム復旧について

- [ここ（苺）を見る](http://strawberry-linux.com/stbee/bb?c=&amp;s=&amp;offset=50)  USART経由でのフラッシュ書き込みについて

<!-- dummy comment line for breaking list -->

- （BOOT0=1, BOOT1=0）になるようにする（つまりBOOT0のジャンパーを1-2間に移す）
- Flash loader demonstrator を起動して、あとはGUIの指示通り。

<!-- dummy comment line for breaking list -->

STBeeは復旧に成功！

- - - -
STBeeMiniの復旧方法

- （１）PB2をGNDに（適当な抵抗1kΩ〜10kΩで）プルダウン。
- （２）BOOT0を（CPUの足を直接）3.3Vに繋ぐ。

<!-- dummy comment line for breaking list -->

この状態でRESETボタンを押して、
- Flash loader demonstrator を起動して、あとはGUIの指示通り。

<!-- dummy comment line for breaking list -->

- 操作（２）の難易度がやや高いが、一応成功。

