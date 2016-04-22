[2014-08](2014-08.md) 

## PIC32MX で USB Audio Speaker を試す。

- MicroChipのサンプルそのまんまですが、Pinguino付属のgccでビルドします。

<!-- dummy comment line for breaking list -->


- MicroChip提供のサンプルソースはPIC32MX220や250には対応していないので、デバイス依存（ピン割り付けとかPWMリソース)の変更を行う必要があります。（変更済です）

<!-- dummy comment line for breaking list -->



## とりあえずダウンロード

- ファームウェアソース(speaker)：[speaker.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/speaker.zip) 
- ファームウェアソース(microphone)：[microphone.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/microphone.zip) 

<!-- dummy comment line for breaking list -->

## speaker サンプルの挙動

- Windowsからは、USB Speakerデバイスとして認識されます。

<!-- dummy comment line for breaking list -->

- このデバイスに、Windows上からMediaPlayer等で適当な音源を流し込んでください。

<!-- dummy comment line for breaking list -->

- RB3(pin7)にオーディオアンプを接続することで、USB SpeakerのPWM出力を聴くことが

<!-- dummy comment line for breaking list -->
できます。

- アンプは直結ではなく、コンデンサ等で直流をデカップリングしてください。

<!-- dummy comment line for breaking list -->

## microphone サンプルの挙動

- RB7(pin16)に接続されたBOOT_SW (GND接続,10k Ohm PullUp) を押すと、USB仮想microphoneデバイス

<!-- dummy comment line for breaking list -->
から短い音声が出力されます。(Windowsで聴くことが出来ます)

- 音声データは audio.h にベタデータで収録されているようです。
- フォーマット --- The function emulates a PCM, 8Khz, 16bits/Second, Mono Microphone.

<!-- dummy comment line for breaking list -->

- usb_config.h にあるとおり、 USB_POLLINGモードが選択されており、割り込みリソースはタイマー

<!-- dummy comment line for breaking list -->
等も含めて全く使われていないようです。



~
~
~
~
~

