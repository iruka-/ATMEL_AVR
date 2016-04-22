[PIC18F2550](PIC18F2550.md) 

- - - -
## PIC18Fシリーズの概要

|品種|Pin数(DIP品)|FlashROM容量|SRAM|EEPROM|
|18F14K50|20PinDIP|16kB|768B|256B|
|18F2550|28PinDIP|32kB|2kB|256B|
|18F4550|40PinDIP|32kB|2kB|256B|

## 大まかな概要
- 命令語は16bit/命令 固定長.但しMOVFF(メモリー間のコピー)のみ32bitになる。
- レジスタセットは W (ワーキングレジスタ) １個のみ(8bit幅)
- W以外はF (ファイル) と呼ばれる、すなわちメモリー
- メモリー空間は12bit(4096byte) 0x000 〜0xfff まで.
- 実装されているメモリーは

<!-- dummy comment line for breaking list -->

|品種|SRAM領域|USBRAM領域|レジスタファイル(I/O空間)|
|18F14K50|0x000〜0x1FF(512B)|0x200〜0x2FF(256B)|0x0f52〜0x0fff|
|18F2550/4550|0x000〜0x3FF(1kB)|0x400〜0x7FF(1kB)|0x0f60〜0x0fff|

