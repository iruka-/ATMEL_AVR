[2011-05](2011-05.md) 

## leaflabs:maple IDE にSTBee , STBeeMini , STM32VL-Discovery 基板を追加する。

maple IDEは、[leaflabs](http://leaflabs.com/) 発売ARM基板用の[Arduino風統合開発環境](http://static.leaflabs.com/pub/leaflabs/maple-docs/latest/) です。

~

- （１）xshigeさん直々に0.0.11対応版を作っていただきました。こちらです。

<!-- dummy comment line for breaking list -->

    - ---> [https://github.com/iruka-/ATMEL_AVR/blob/master/web/cgi-bin/note/index.cgi?%b7%c7%bc%a8%c8%c4](https://github.com/iruka-/ATMEL_AVR/blob/master/web/cgi-bin/note/index.cgi?%b7%c7%bc%a8%c8%c4) 

<!-- dummy comment line for breaking list -->

    - 置き場は[こちら](https://skydrive.live.com/?cid=bbd4ba3beeca403e&amp;sc=documents&amp;id=BBD4BA3BEECA403E%21107) です。

<!-- dummy comment line for breaking list -->


~

- （２）senshuさんが作成されたavrdude_fake（機能強化版）はこちらからDLしてください。
    - [http://202.35.250.1/ken/senshu/sitedev/index.php?AVR%2Fnews66#qda3638a](http://202.35.250.1/ken/senshu/sitedev/index.php?AVR%2Fnews66#qda3638a) 

<!-- dummy comment line for breaking list -->

~
~
- （３） maple IDEを使用するかわりに maple IDEに含まれている**libmaple**という便利なライブラリを使ってコマンドライン(Linux or Windows)上からビルド＆ランすることも可能です。
    - 以下のリンク先をお読みください。

<!-- dummy comment line for breaking list -->

    - [libmapleを使いこなせ！！](2011-08.md)   --- libmaple に、CQ-STARM基板とSTM8S-Discovery基板も追加してみる。

<!-- dummy comment line for breaking list -->



~
~
~


~
~
~
- - - -
#### 古い記事
- - - -

[2011-03](2011-03.md) 

## leaflabs: maple IDE 0.0.9 の改造



- maple基板に最も近そうな（？）cq-starmを追加してみるテスト。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/mapleIDE.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/mapleIDE.png) 






![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32.jpg) 

~
**改造の要点**
- 好きな基板(今のところSTM32限定)を追加する。
- uploaderをdfu-util以外にして試してみる。
    - javaのソースから見て、dfu-utilかavrdudeしか受け付けないようだ。（そりゃ、呼び出しコマンドラインの生成手順がハードコーディングだからしかたないね。）

<!-- dummy comment line for breaking list -->

~
- uploaderは仮想COMデバイスか、リアルRS232C COMデバイスを相手にuploadする仕組みにする。
- bootloaderもそれに準ずるようにする。
- そうしないとUSBを積んでないARM (LPC1114) に対応できないのと、
- とりあえずシリアルを生かしておけば、USB bootloaderを書き込む前のデバイス (例えば LPC2388のシリアルブートモードを利用してブートとか、STM32の緊急時シリアルブートでFirmを書き込むとか）に対応できるというメリットがある。

<!-- dummy comment line for breaking list -->



~
~
~

### その１：基板名の追加

- ＜インストール先＞/maple-ide-0.0.9/hardware/leaflabs/boards.txt の最後あたりに追記する。

<!-- dummy comment line for breaking list -->

	##############################################################
	
	cqstarm.name=CQ-STARM to FLASH
	
	cqstarm.upload.file_type=bin
	cqstarm.upload.maximum_size=108000
	cqstarm.upload.ram.maximum_size=17000
	cqstarm.upload.flash.maximum_size=108000
	cqstarm.upload.usbID=1EAF:0003
	cqstarm.upload.altID=1
	cqstarm.upload.uploader=avrdude
	cqstarm.upload.auto_reset=true
	
	cqstarm.upload.protocol=stk500
	cqstarm.upload.speed=57600
	
	cqstarm.build.board=cq-starm
	cqstarm.build.mcu=STM32F103RB
	cqstarm.build.family=cortex-m3
	cqstarm.build.f_cpu=72000000L
	cqstarm.build.core=maple
	cqstarm.build.vect=VECT_TAB_FLASH
	cqstarm.build.linker=maple/flash.ld
	cqstarm.build.using=armcompiler

- RAM上にuploadする必要性ってあるのかな？
- H8じゃあるまいし（FLASH 100回でデバイスが寿命とか）
- RAM上にcodeを置くと、せっかくの(20k〜64K)SRAMがデータ領域として使えなくなるからARMを使う意味がない。

<!-- dummy comment line for breaking list -->



~
~
~

### その２：PIN_MAPの追記

- ＜インストール先＞/maple-ide-0.0.9/hardware/leaflabs/cores/maple/boards.h
- 385行目当たりの #else 手前に挿入する。
- 内容は、とりあえずmapleのまんま。

<!-- dummy comment line for breaking list -->

	#elif defined(BOARD_cqstarm)
	
	    #define CYCLES_PER_MICROSECOND  72
	    #define SYSTICK_RELOAD_VAL      71999 /* takes a cycle to reload */
	
	    #define BOARD_BUTTON_PIN   38
	    #define BOARD_LED_PIN      13
	
	    static __attribute__ ((unused)) PinMapping PIN_MAP[NR_GPIO_PINS] = {
	        /* D0/PA3 */
	        {GPIOA_BASE,  3,        ADC3, TIMER2_CH4_CCR,  EXTI_CONFIG_PORTA, TIMER2, 4},
	        /* D1/PA2 */
	        {GPIOA_BASE,  2,        ADC2, TIMER2_CH3_CCR,  EXTI_CONFIG_PORTA, TIMER2, 3},
	        /* D2/PA0 */
	        {GPIOA_BASE,  0,        ADC0, TIMER2_CH1_CCR,  EXTI_CONFIG_PORTA, TIMER2, 1},
	        /* D3/PA1 */
	        {GPIOA_BASE,  1,        ADC1, TIMER2_CH2_CCR,  EXTI_CONFIG_PORTA, TIMER2, 2},
	        /* D4/PB5 */
	        {GPIOB_BASE,  5, ADC_INVALID,              0,  EXTI_CONFIG_PORTB, TIMER_INVALID, TIMER_INVALID},
	        /* D5/PB6 */
	        {GPIOB_BASE,  6, ADC_INVALID, TIMER4_CH1_CCR,  EXTI_CONFIG_PORTB, TIMER4, 1},
	        /* D6/PA8 */
	        {GPIOA_BASE,  8, ADC_INVALID, TIMER1_CH1_CCR,  EXTI_CONFIG_PORTA, TIMER1, 1},
	        /* D7/PA9 */
	        {GPIOA_BASE,  9, ADC_INVALID, TIMER1_CH2_CCR,  EXTI_CONFIG_PORTA, TIMER1, 2},
	        /* D8/PA10 */
	        {GPIOA_BASE, 10, ADC_INVALID, TIMER1_CH3_CCR,  EXTI_CONFIG_PORTA, TIMER1, 3},
	        /* D9/PB7 */
	        {GPIOB_BASE,  7, ADC_INVALID, TIMER4_CH2_CCR,  EXTI_CONFIG_PORTB, TIMER4, 2},
	        /* D10/PA4 */
	        {GPIOA_BASE,  4,        ADC4,              0,  EXTI_CONFIG_PORTA, TIMER_INVALID, TIMER_INVALID},
	        /* D11/PA7 */
	        {GPIOA_BASE,  7,        ADC7, TIMER3_CH2_CCR,  EXTI_CONFIG_PORTA, TIMER3, 2},
	        /* D12/PA6 */
	        {GPIOA_BASE,  6,        ADC6, TIMER3_CH1_CCR,  EXTI_CONFIG_PORTA, TIMER3, 1},
	        /* D13/PA5 */
	        {GPIOA_BASE,  5,        ADC5,              0,  EXTI_CONFIG_PORTA, TIMER_INVALID, TIMER_INVALID},
	        /* D14/PB8 */
	        {GPIOB_BASE,  8, ADC_INVALID, TIMER4_CH3_CCR,  EXTI_CONFIG_PORTB, TIMER4, 3},
	
	        /* Little header */
	
	        /* D15/PC0 */
	        {GPIOC_BASE,  0,       ADC10,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D16/PC1 */
	        {GPIOC_BASE,  1,       ADC11,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D17/PC2 */
	        {GPIOC_BASE,  2,       ADC12,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D18/PC3 */
	        {GPIOC_BASE,  3,       ADC13,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D19/PC4 */
	        {GPIOC_BASE,  4,       ADC14,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D20/PC5 */
	        {GPIOC_BASE,  5,       ADC15,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	
	        /* External header */
	
	        /* D21/PC13 */
	        {GPIOC_BASE, 13, ADC_INVALID,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D22/PC14 */
	        {GPIOC_BASE, 14, ADC_INVALID,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D23/PC15 */
	        {GPIOC_BASE, 15, ADC_INVALID,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D24/PB9 */
	        {GPIOB_BASE,  9, ADC_INVALID, TIMER4_CH4_CCR,  EXTI_CONFIG_PORTB, TIMER_INVALID, TIMER_INVALID},
	        /* D25/PD2 */
	        {GPIOD_BASE,  2, ADC_INVALID,              0,  EXTI_CONFIG_PORTD, TIMER_INVALID, TIMER_INVALID},
	        /* D26/PC10 */
	        {GPIOC_BASE, 10, ADC_INVALID,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D27/PB0 */
	        {GPIOB_BASE,  0,        ADC8, TIMER3_CH3_CCR,  EXTI_CONFIG_PORTB, TIMER3, 3},
	        /* D28/PB1 */
	        {GPIOB_BASE,  1,        ADC9, TIMER3_CH4_CCR,  EXTI_CONFIG_PORTB, TIMER3, 4},
	        /* D29/PB10 */
	        {GPIOB_BASE, 10, ADC_INVALID,              0,  EXTI_CONFIG_PORTB, TIMER_INVALID, TIMER_INVALID},
	        /* D30/PB11 */
	        {GPIOB_BASE, 11, ADC_INVALID,              0,  EXTI_CONFIG_PORTB, TIMER_INVALID, TIMER_INVALID},
	        /* D31/PB12 */
	        {GPIOB_BASE, 12, ADC_INVALID,              0,  EXTI_CONFIG_PORTB, TIMER_INVALID, TIMER_INVALID},
	        /* D32/PB13 */
	        {GPIOB_BASE, 13, ADC_INVALID,              0,  EXTI_CONFIG_PORTB, TIMER_INVALID, TIMER_INVALID},
	        /* D33/PB14 */
	        {GPIOB_BASE, 14, ADC_INVALID,              0,  EXTI_CONFIG_PORTB, TIMER_INVALID, TIMER_INVALID},
	        /* D34/PB15 */
	        {GPIOB_BASE, 15, ADC_INVALID,              0,  EXTI_CONFIG_PORTB, TIMER_INVALID, TIMER_INVALID},
	        /* D35/PC6 */
	        {GPIOC_BASE,  6, ADC_INVALID,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D36/PC7 */
	        {GPIOC_BASE,  7, ADC_INVALID,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* D37/PC8 */
	        {GPIOC_BASE,  8, ADC_INVALID,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID},
	        /* PC9 (BUT) */
	        {GPIOC_BASE,  9, ADC_INVALID,              0,  EXTI_CONFIG_PORTC, TIMER_INVALID, TIMER_INVALID}
	    };
	
	    #define BOARD_INIT do {                           \
	        } while(0)

~
~
~ 
### その３：uploaderの追加

- ＜インストール先＞/maple-ide-0.0.9/hardware/tools/avr/bin/
- というディレクトリを掘る。（何故avrなのかは突っ込まない）

<!-- dummy comment line for breaking list -->

- そこに自作の avrdude.exe を置く。
- 内容は、cq-starmにupload出来るなら、どんなプログラムでもおＫ。

<!-- dummy comment line for breaking list -->

- 現在は未実装なので、とりあえずこんなやつを・・・

		#include &lt;stdio.h&gt;
		int main(int argc,char **argv) {
		int i;
		for(i=0;i&lt;argc;i++) {
			printf("%2d:&gt;%s&lt;\n",i,argv[i]);
		}
		    return 0;
		}
- ~
- *blinkのサンプルをuploadしてみる。 [#e9e6c25d]
- 結果：

		0:&gt;D:\INSTALL\maple-ide-0.0.9\hardware\tools\avr\bin\avrdude&lt;
		1:&gt;-CD:\INSTALL\maple-ide-0.0.9\hardware/tools/avr/etc/avrdude.conf&lt;
		2:&gt;-q&lt;
		3:&gt;-q&lt;
		4:&gt;-pSTM32F103RB&lt;
		5:&gt;-cstk500v1&lt;
		6:&gt;-P\\.\COM1&lt;
		7:&gt;-b57600&lt;
		8:&gt;-D&lt;
		9:&gt;-Uflash:w:C:\DOCUME~1\USERNAME\LOCALS~1\Temp\build6274997941406310107.tmp\Blink.cpp.hex:i&lt;
- まあこんな感じでエアーmaple完了。

<!-- dummy comment line for breaking list -->


~
~
~
- - - -
## CQ-STARMのDFUを[armboot](armon.md) に置き換える

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32F103VBT6.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/STM32F103VBT6.jpg) 

- まず、[苺からdfuwをＤＬ](http://strawberry-linux.com/stbee/dfuw) する。
- 次に、以下のCQ-STARM用のarmbootファームウェアを入手。

<!-- dummy comment line for breaking list -->



DOWNLOAD: [cq-starm用ファーム](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/STM/cqstarm.zip) 

- ReadMe.txtに従って操作。

<!-- dummy comment line for breaking list -->

- 自己責任でお願いします。

<!-- dummy comment line for breaking list -->

- armbootやcqstarm用のファームウェアのソースは[こちら](armon.md) 

<!-- dummy comment line for breaking list -->

- 別の記事：[ARMでArduino](2011-01#u70613a4.md)  --- maple IDEでビルドしたLED Blinkを STM8S-Discovery上で実行してみた。

<!-- dummy comment line for breaking list -->


~
~
~
~

