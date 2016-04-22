[2011-01](2011-01.md) 

## LPC1768/LPCXpresso
去年の年末に買い込んだ積みマイコンのうちの一つ
- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/LPC1768.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/LPC1768.jpg) 
- とりあえず基板切断してみた。
- 小さくなったユニバーサルエリアは、実はI/O pinがいろいろ繋がっていてフリーエリアではないらしい。

<!-- dummy comment line for breaking list -->

- なんとEtherPHYチップとシリアルEEPROM(たぶんMACアドレス記憶用?)が載っている。
- 時計用の32768Hzクリスタルも載っている。
- USB D+のプルアップ制御はTrでやっているようだ。LPC1343版と違い、ちゃんと配線されている。
- 電源は3.3Vを与える必要がある。LDOレギュレータは載っていない。

<!-- dummy comment line for breaking list -->



NXPのサイトはここ。
- [http://ics.nxp.com/lpcxpresso/~LPC1768/#LPC1768](http://ics.nxp.com/lpcxpresso/~LPC1768/#LPC1768) 

<!-- dummy comment line for breaking list -->

回路図はこの辺。
- [http://www.embeddedartists.com/products/lpcxpresso/lpc1769_xpr.php](http://www.embeddedartists.com/products/lpcxpresso/lpc1769_xpr.php) 

<!-- dummy comment line for breaking list -->

ちょっと抜粋してみた。
- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/lpc1768sch.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/lpc1768sch.png) 

<!-- dummy comment line for breaking list -->

~
~
~
予定稿
- 例によって、縛りの多いGUIな統合環境(Eclipse+gccなのにコードサイズ128kBまでしかというやつ)は、**使う気全く無し**
    - なので、LPC3154側はとりあえず要らない。
- 普通にCodeSourceryG++Liteとmakeでやっつける。
- LPC1343版と違って、BOOTROMサイズが8kしかなく、シリアルオンリーなので、最初はシリアルでブートローダーを書き込む（＆開発する）必要がある。
- LPC2388とLPC1768は周辺リソースがわりと似ているらしい。(逆にCPUはARM7からCortexM3に変わったせいで互換性はかなり薄い）
- 一応CMSISがサポートされている。
- Flashが512kもある。SRAMはLPC2388では64k+16k+16k=96kあったけれど、LPC1768では32k+16k+16k=64kに減らされている。

<!-- dummy comment line for breaking list -->
(16kというのはUSB,Ether,DMAなどのペリフェラル側に位置する共有SRAMらしい)

- （まだ通電してないのでなんとも言えないけれど）LPC2388のように熱くなったりしないのではないかという期待はある。
- LPC1768は100MHz動作。LPC1769では120MHz動作。多分クロックアップできる（？）

<!-- dummy comment line for breaking list -->

使い道
- とりあえずクロックアップでもするか・・・。

