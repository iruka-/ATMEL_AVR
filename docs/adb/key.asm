;======================================================================
;
;		ＡＤＢ to ＰＳ／２ キーボードアダプタ
;
;======================================================================
;
;	・MacintoshのキーボードをＡＴ互換機に接続する.
;
;１６Ｆ８４の配線:(たったこれだけ)
;----------------------------------------------------------------------
;  ADB側
;	PORTA	0	in/out	ADB data
;	PORTA	1	in/out	ADB reset(未使用なので配線必要なし)
;  PS/2側
;	PORTB	0	in/out	KBD data
;	PORTB	1	in/out	KBD clock
;
;	上記ピンは3.3kΩにて+5Vへプルアップ.
;
;	周波数は10MHz(水晶発振)
;
;	リセットはプルアップすること.
;
;
;主要ルーチンとワーク.
;----------------------------------------------------------------------
;	KeyScanMain()
;		keybuf		ＡＤＢから受信したApple側キーコード
;				MSB=1の場合はそのキーが離されたことになる.
;		KeyPos		0x00～0x7e  keybuf値のMSBをマスクしたもの.
;		KeyRow		KeyPos / 8  の値(bit table検索用)
;		KeyMask		0x01 .. 0x80 ... bitmask値
;
;		KeyBits[16]	127個分の各キーが押されている(1)か離れている(0)
;				かをbitで記憶するスコアボード
;
;
;The connector will be one of the following:
;5pin 180 deg DIN               6pin MINIDIN
;----------------               ------------
;                                
;                                    _
;       (2)      1 CLOCK          (5) (6)        1 DATA
;    (5)   (4)   2 DATA              |           2 
;   (3)     (1)  3              (3)     (4)      3 GND
;                4 GND                           4 +5V
;                5 5V             (1) (2)        5 CLOCK
;                                                6
;
;,-------,---,---,---,---,,---,---,---,---,,---,---,---,---,
;|ESC    |F1 |F2 |F3 |F4 ||F5 |F6 |F7 |F8 ||F9 |F10|F11|F12|
;|76     |05 |06 |04 |0c ||03 |0b |83 |0a ||01 |09 |78 |07 |
;'-------'---'---'---'---''---'---'---'---''---'---'---'---'
;,---,---,---,---,---,---,---,---,---,---,---,---,---,-----,
;|~  |1  |2  |3  |4  |5  |6  |7  |8  |9  |0  |-  |=  |BS   |
;|0e |16 |1e |26 |25 |2e |36 |3d |3e |46 |45 |4e |55 |66   |
;|---',--',--',--',--',--',--',--',--',--',--',--',--',----|
;|TAB |Q  |W  |E  |R  |T  |Y  |U  |I  |O  |P  |[  |]  |\   |
;|0d  |15 |1d |24 |2d |2c |35 |3c |43 |44 |4d |54 |5b |5d  |
;|----',--',--',--',--',--',--',--',--',--',--',--',--'----|
;|CAPS |A  |S  |D  |F  |G  |H  |J  |K  |L  |;  |,  |ENTER  |
;|58   |1c |1b |23 |2b |34 |33 |3b |42 |4b |4c |52 |5a     |
;|-----',--',--',--',--',--',--',--',--',--',--',----------|
;|SHIFT |Z  |X  |C  |V  |B  |N  |M  |,  |.  |/  |SHIFT     |
;|12    |1a |22 |21 |2a |32 |31 |3a |41 |49 |4a |59        |
;|----,-',--'--,'---'---'---'---'---'---'--,'--,'-----,----|
;|CTRL|  |ALT  |          SPACE            |ALT|      |CTRL|
;|14  |  |11   |          29               |e011      |e014|
;'----'--'-----'---------------------------'---'------'----'
; 
;,-----,-----,-----,
;|PSCRN|SLOCK|BREAK|
;|*2   |7e   |*4   |
;'-----'-----'-----'
;*2 is E012E07C
;*4 is E11477E1F014F077
;,-----,-----,-----,  ,-----,-----,-----,-----,
;|INS  |HOME |PGUP |  |NLOCK|/    |*    |-    |
;|e070 |e06c |e07d |  |77   |e04a |7c   |7b   |
;|-----|-----|-----|  |-----|-----|-----|-----|
;|DEL  |END  |PGDN |  |7    |8    |9    |+    |
;|e071 |e069 |e07a |  |6c   |75   |7d   |79   |
;'-----'-----'-----'  |-----|-----|-----|-----|
;                     |4    |5    |6    |     |
;                     |6b   |73   |74   |     |
;      ,-----,        |-----|-----|-----|-----|
;      |UP   |        |1    |2    |3    |ENTER|
;      |e075 |        |69   |72   |7a   |e05a |
;,-----|-----|-----,  |-----------|-----|     |
;|LEFT |DOWN |RIGHT|  |0          |.    |     |
;|e06b |e072 |e074 |  |70         |71   |     |
;'-----'-----'-----'  '-----------'-----'-----'
; 
;There are at least three options for the mapping of key numberss to
;keycodes.  The common usage seems to be as follows:
;
;F1 F2    `  1  2  3  4  5  6  7  8  9  0  -  =  \ BS   ESC NUML SCRL SYSR 
;-----   ---------------------------------------------  ------------------
;05 06   0E 16 1E 26 25 2E 36 3D 3E 46 45 4E 55 5D 66    76  77   7E   84
;
;F3 F4   TAB   Q  W  E  R  T  Y  U  I  O  P  [  ]       Home Up  PgUp PrtSc
;-----   -----------------------------------------      -------------------
;04 0C    0D  15 1D 24 2D 2C 35 3C 43 44 4D 54 5B        6C  75   7D   7C
;
;F5 F6   CNTL   A  S  D  F  G  H  J  K  L  ;  ' ENTER   Left  5  Right  -
;-----   --------------------------------------------   ------------------
;03 0B    14   1C 1B 23 28 34 33 38 42 4B 4C 52   5A     6B  73   74   7B
;
;F7 F8   LSHFT    Z  X  C  V  B  N  M  ,  .  /  RSHFT   End  Dn  PgDn   +
;-----   --------------------------------------------   ------------------
;83 0A     12    1A 22 21 2A 32 31 3A 41 49 4A   59      69  72   7A   79
;
;F9 F10  ALT                  SPC             CAPLOCK   Ins      Del
;------  --------------------------------------------   -------------
;01 09    11                   29               58       70       71


#include "p16f84.inc"
#include "custom.h"

	radix	DEC

;	.osc		hs		; オシレータＨＳ
;	.wdt		off		; ウォッチドッグＯＦＦ
;	.pwrt		on		; パワーアップタイマＯＮ
;	.protect	off		; プロテクトＯＦＦ


; KeyBoard 出力 は正論理
PinLO  macro P
	call	P
 endm


PinHI  macro P
	call	P
 endm


rz	  macro			   ; return if Zero
	    btfsc  STATUS, Z
	    return
	  endm

rnz	  macro			   ; return if Not Zero
	    btfss  STATUS, Z
	    return
	  endm

rc	  macro			   ; return if Carry
	    btfsc  STATUS, C
	    return
	  endm

rnc	  macro			   ; return if Not Carry
	    btfss  STATUS, C
	    return
	  endm

;--------------------------------------------------------------
;	スタート.
;--------------------------------------------------------------
	jmp	_main	;;0000
	jmp	_main	;;0001
	jmp	_main	;;0002
	jmp	_main	;;0003
	jmp	_intr	;;0004

;--------------------------------------------------------------
;	キースキャン・ワーク
;--------------------------------------------------------------
 New	KeyPos		;;0..88      key number
 New	KeyRow		;;0..11 ---  74159N select signal
;;; New	KeyCol		;;0..7  .... bitpos
 New	KeyMask		;;0x01 .. 0x80 ... bitmask
 New	KeyVal		;;KeyRowに対応するビットパタン.
 New	KeyWk		;;汎用ワーク.

 New	KeyRepCnt	;;キーリピートカウンタ.
 New	KeyRepFlag	;;キーリピートフラグ.
 New	KeyRepCode	;;キーリピートしたいキー.

 New	ScanMode	;;

 New	PORTBdir	;; 0:出力 1:入力
;--------------------------------------------------------------
;	シリアルワーク
;--------------------------------------------------------------
 New	SendWk		;; シリアル汎用.
 New	SendCnt		;; ８ビットまでのループを数える.
 New	SendParity	;; パリティ計算用(1bitで良い)
 New	SendByte	;; 送信バイト・バッファ.
 New	SendByteLast	;; 最後に送信したデータ(再送時に使用)
 New	RecByte		;; 受信バイト・バッファ.

;--------------------------------------------------------------
;	ホストコマンドフラグ
;--------------------------------------------------------------
 New	ResScanType	;; 01 02 03
 New	ResRateDelay	;; b7-unused. b6..5-Repeat delay (00=250 mS,11=1000mS)
			;; b4..0-Repeat rate (00000=30x/sec, 11111=2x/sec).
;--------------------------------------------------------------
;	ＡＤＢ用
;--------------------------------------------------------------
 New	Breg		;; 1バイト送信するデータ.
 New	Creg		;; 8bit分を数えるカウンタ.
 New	Ereg		;; 1バイト受信したデータ.
 New	keybuf		;; getkey受け渡し用.
;--------------------------------------------------------------
;	ビットの定義
;--------------------------------------------------------------
#define	 KeyREPCLR	KeyRepFlag,7	;; キー状態が変化した.
#define	 KeyREPRESS	KeyRepFlag,6	;; キーは１つ以上おされている.
					;; ADBにおいては、リピート可能なキーのメークが発生したら1
					;;    にし、リピート可能なキーのブレーク発生で0に戻す.

#define	 BrkOFF		ScanMode,0	;;ブレークコードは送らない.
#define	 RepOFF		ScanMode,1	;;キーリピートコードは送らない.

;--------------------------------------------------------------
;	ポート割り当ての定義
;--------------------------------------------------------------
#define  kbCLKi   	PORTB, 1	; clock line input
#define  kbCLKo   	PORTB, 1	; clock line output(inverted OpenCollector)
#define  kbDATAi  	PORTB, 0	; data line input
#define  kbDATAo  	PORTB, 0	; data line output(inverted OpenCollector)

#define	 ADBi		PORTA, 0

#define  ADBTIME	25		; 50uS がスレッショルドタイム.

#define	 ADBshift	KeyBits7,0	; ADBの [SHIFT] キー番号は 0x38 , bit位置はここ.
#define	 ADBctrl	KeyBits6,6	; ADBの [CTRL ] キー番号は 0x36 , bit位置はここ.

#define	 MARK		0x00
#define	 SPACE		0x01
;--------------------------------------------------------------
;	キースキャン・結果テーブル
;--------------------------------------------------------------
;	８ビット×１６Ｒｏｗ＝128キー分。押されていたら１が立つ.

 New	KeyBits
 New	KeyBits1
 New	KeyBits2
 New	KeyBits3
 New	KeyBits4
 New	KeyBits5
 New	KeyBits6
 New	KeyBits7
 New	KeyBits8
 New	KeyBits9
 New	KeyBits10
 New	KeyBits11
 New	KeyBits12
 New	KeyBits13
 New	KeyBits14
 New	KeyBits15


;--------------------------------------------------------------
;  ＡＤＢのスキャンコードを１０６配列のスキャンコードに変換
;--------------------------------------------------------------
#include "adb.inc"

;--------------------------------------------------------------
;  ＡＤＢのキーでリピートしたいビットに印を付ける.
;--------------------------------------------------------------
KeyRepMask:
	addwf	PC,F
	retlw	B'00000000'	; 0x00
	retlw	B'00000000'
	retlw	B'00000000'	; 0x10
	retlw	B'00000000'
	retlw	B'00000000'	; 0x20
	retlw	B'00000000'
	retlw	B'11000000'	; 0x30
	retlw	B'00000111'
	retlw	B'00000000'	; 0x40
	retlw	B'00000000'
	retlw	B'00000000'	; 0x50
	retlw	B'00000000'
	retlw	B'00000000'	; 0x60
	retlw	B'00000000'
	retlw	B'00000000'	; 0x70
	retlw	B'00000000'

;--------------------------------------------------------------
;	カラム番号から、入力PORTのマスクパターンへの変換.
;--------------------------------------------------------------
Col2Mask:
	addwf	PC,F
	retlw	B'00000001'
	retlw	B'00000010'
	retlw	B'00000100'
	retlw	B'00001000'
	retlw	B'00010000'
	retlw	B'00100000'
	retlw	B'01000000'
	retlw	B'10000000'

;--------------------------------------------------------------
;	ホストコマンド ED～FFへの応答ルーチンへ分岐.
;--------------------------------------------------------------
respond_W:
	addwf	PC,F
	jmp	respond_ED
	jmp	respond_EE
	jmp	respond_EF
	jmp	respond_F0
	jmp	respond_F1
	jmp	respond_F2
	jmp	respond_F3
	jmp	respond_F4
	jmp	respond_F5
	jmp	respond_F6
	jmp	respond_F7
	jmp	respond_F8
	jmp	respond_F9
	jmp	respond_FA
	jmp	respond_FB
	jmp	respond_FC
	jmp	respond_FD
	jmp	respond_FE
	jmp	respond_FF
;--------------------------------------------------------------
;↑↑↑	ジャンプテーブル類(addwf PC,F)は、なるべく0x100番地
;	以下に置く.そうしないと PCLATHを設定する必要性が出る.
;--------------------------------------------------------------

;--------------------------------------------------------------
;	ここが0x100番地を越えていたら要注意:
;--------------------------------------------------------------
H100fence:

;--------------------------------------------------------------
;	割り込み処理開始.
;--------------------------------------------------------------
_intr:
	nop
	bcf	INTCON,RBIF	;;割り込みフラグクリア
	retfie			;;割り込み復帰.
;--------------------------------------------------------------
;	メイン処理開始.
;--------------------------------------------------------------
_main:
	nop
	;;ポートの入出力方向を決める.
	SetRPage 1
	bcf	INTCON,GIE	;;割り込み禁止

	movlw	B'00000000'	;;PORTB pull UP
	movwf	OPTION_REG


	movlw	B'11100011'	;; bit 2,3,4のみ出力.他は入力
	movwf	PORTA

	movlw	B'00000011'	;; bit 0 ,1のみ入力.他は出力
	movwf	PORTBdir
	movwf	PORTB

	SetRPage 0


	;;出力ピンの値を初期化する.
	bsf	kbCLKo
	bsf	kbDATAo

	;;システム全体が安定するまで待つ.
	call	Wait400m

	;;デバッグのためのmain
	;;jmp	testmain

	;;ＡＤＢ初期化.
	movlw	0x00
	call	adb_cmd
	call	Wait400m

;--------------------------------------------------------------
;	Diagnostic 成功を互換機に伝える.
;--------------------------------------------------------------

	movlw	0xaa
	call	KeySendW	;;no problem.

	movlw	0x03
	movwf	ResScanType
	clrf	ScanMode

;--------------------------------------------------------------
;	メインループ
;--------------------------------------------------------------
KeyScanMain:
	call	KeyBitsInit

;;リピート初期化:
KeyScan_0:
	clrf	KeyRepFlag
	movl	KeyRepCnt,0x60	;;リピート開始カウントダウンカウンタ.

;;通常の初期化:
KeyScan_1:
	movl	KeyPos,0x7f	;;キーセンスポインタ.
	bcf	KeyREPCLR	;;キー変化フラグを初期化.

;;キーを全部センスするループの先頭:
KeyScan_2:
	call	KeyRecv1	;;なにか送られてきたら受信する.
	call	KeyScan1	;;ＡＤＢキーをセンスする.

;;一周回ったあとで、押されっぱなしキーがあれば、リピート処理をする.
	btfsc	KeyREPCLR
	jmp	KeyScan_0	;;変化があったので、リピートキャンセル.

	call	RepChk		;;KeyRepCode相当のキーは押されている？.
	jz	KeyScan_0	;;キーは何も押されていない.

	btfsc	RepOFF
	jmp	KeyScan_0	;;タイパマティック送信しない.

	decfsz	KeyRepCnt,F	;;リピート開始カウントに達するまでループ.
	jmp	KeyScan_1

;;リピートの実行.
	movfw	KeyRepCode
	movwf	KeyPos
	call	KeySend

	movl	KeyRepCnt,0xc	;;リピート開始カウントダウンカウンタ巻き戻し.
	jmp	KeyScan_1

;--------------------------------------------------------------
;	キースキャン・小物ルーチン
;--------------------------------------------------------------
;
;	待ち時間はクロック１０ＭＨｚとして設定.
;	1 Cycle = 4 / 10,000,000 = 0.4uS
;
;	KeyW_1() は ループ１回に付き、1.2uS 消費する.
;
;

Wait12uS:
	movlw	6		;10
	jmp	KeyWait
Wait20uS:
	movlw	14		;18
	jmp	KeyWait
w250us:
	movlw	0xf8		;248
;;;	jmp	KeyWait
KeyWait:
	movwf	SendWk		; 1
KeyW_1:
	decfsz	SendWk,F	; 1
	jmp	KeyW_1		; 2

	return			; 2

w800us:
	call	w250us
	call	w250us
	call	w250us
	movlw	50
	jmp	KeyWait
;--------------------------------------------------------------
;	
;--------------------------------------------------------------
Wait400m:
	movl	KeyWk,0		;1
_loop1:
	call	w800us
	call	w800us
	decfsz	KeyWk,F
	jmp	_loop1
	return


;--------------------------------------------------------------
;	KeyBits[KeyRow] テーブルをインデックス参照する為の準備.
;--------------------------------------------------------------
KeyBitAdr:
	movlw	KeyBits
	addwf	KeyRow,W
	movwf	FSR		;; 間接Register = KeyBits+KeyRow
	return

;--------------------------------------------------------------
;	KeyPosに当たるビットを検査する(Z フラグ).
;--------------------------------------------------------------
KeyBitChk:
	call	KeyBitAdr
	movfw	KeyMask
	andwf	INDF,W		;; メモリー間接参照.
	return

;--------------------------------------------------------------
;	KeyPosに当たるビットを立てる.
;--------------------------------------------------------------
KeyBitSet:
	call	KeyBitAdr
	movfw	KeyMask
	iorwf	INDF,F		;; メモリー間接参照.
	return

;--------------------------------------------------------------
;	KeyPosに当たるビットを倒す.
;--------------------------------------------------------------
KeyBitReset:
	call	KeyBitAdr
	movfw	KeyMask
	xorlw	0xff		;マスクを反転して＆を取る.
	andwf	INDF,F		;; メモリー間接参照.
	return

;--------------------------------------------------------------
;	キービット配列を全部消去する.
;--------------------------------------------------------------
KeyBitsInit:
	clrf	KeyRow
	movl	KeyWk,16

KeyBitsInit1:
	call	KeyBitAdr
	clrf	INDF
	incf	KeyRow,F
	decfsz	KeyWk,F
	jmp	KeyBitsInit1
	return


;--------------------------------------------------------------
;	現在スキャンしたキーがリピート不可だったら Not Zero にする
;--------------------------------------------------------------
KeyRepMaskChk:
	movfw	KeyRow
	call	KeyRepMask
	andwf	KeyMask,W
	return

;--------------------------------------------------------------
;	リピータブルなキーがまだ押されているか？
;--------------------------------------------------------------
KeyRepScan:
	movl	KeyWk,16
	movl	KeyVal,0
KeyRepScal:
	movlw	KeyBits
	addwf	KeyVal,W
	movwf	FSR		;; 間接Register = KeyBits+KeyVal

	movfw	KeyVal
	call	KeyRepMask	;; リピートしたくないキーが１のテーブル.
	xorlw	0xff		;マスクを反転して＆を取る.
	andwf	INDF,W		;; メモリー間接参照. 
	rnz		; ===＞リピータブルなキーが押されていた！

	incf	KeyVal,F
	decfsz	KeyWk,F
	jmp	KeyRepScal

	andlw	0x00
	return	; zero return 結局押されていなかった.

SetRowMask:
	movwf	KeyRow

;	副作用として、KeyRow,KeyCol,KeyMask がセットされる.
	;;; KeyRow = KeyPos / 8;
	rrf	KeyRow,F
	rrf	KeyRow,F
	rrf	KeyRow,W
	andlw	0x0f
	movwf	KeyRow

	;;; KeyCol = KeyPos & 7
	movfw	KeyPos
	andlw	0x07
	;;;movwf KeyCol
	
	;;; KeyMask = masktable[KeyCol]
	call	Col2Mask
	movwf	KeyMask
	return

CapsOnOff:
	call	KeySend		; [CAPS] On
	call	KeySendF0	; [CAPS] Off
	return

;--------------------------------------------------------------
;	KeyPos番目に当たるキーを１個検査する
;--------------------------------------------------------------
;	変化が有ったら、コードを互換機に送信する.
KeyScan1:
	call	get_key
	incf	keybuf,W	;; keybuf が 0xff以外なら入力あり.
	rz			;; 入力なしならリターン.

	;;; KeyPos = keybuf & 0x7f
	movfw	keybuf
	andlw	0x7f
	movwf	KeyPos		;;ＡＤＢのキー番号をKeyPosに記憶させる.
	call	SetRowMask	;; KeyPos から KeyRowとKeyMaskを計算.

;ビットの変化有り.
	bsf	KeyREPCLR	;;変化があったら、キー変化フラグをOn

;[CAPS]のみ、特殊処理
 if	0
	movlw	0x39
	subwf	KeyPos,W
	jz	CapsOnOff
 endif
;メークかブレークかの判定.
	btfsc	keybuf,7	;;MSBが1ならＡＤＢからのキー開放情報
	jmp	KeyTurnOff

;キーが押された.
KeyTurnOn:
	call	KeyBitSet
	call	KeySend
	call	KeyRepMaskChk
	rnz
;;リピート可能なキーの場合、そのコードをためる.
	movfw	KeyPos
	movwf	KeyRepCode
	return

;キーが離された.
KeyTurnOff:
	call	KeyBitReset

	btfss	BrkOFF		;;キーブレーク時にコードを送る? bitが立っていたら送らない.
	call	KeySendF0


	movfw	KeyPos
	subwf	KeyRepCode,W
	rnz			;;離されたキーは、リピートさせてたキーと同一だった.

	movl	KeyRepCode,0x7f	;リピートキーをとりあえず無効にしておく.
	return


;リピート可能なキーが、まだ押されているかどうかを判断する.
RepChk:
	movfw	KeyRepCode
	movwf	KeyPos
	call	SetRowMask	;; KeyPos から KeyRowとKeyMaskを計算.
	call	KeyBitChk

	return


;--------------------------------------------------------------
;	キーブレークコード送出
;--------------------------------------------------------------
KeySendF0:
	movfw	KeyPos
	call	ADBtoAT
	iorlw	0x00
	jz	kF0_00

	movwf	KeyWk
	
	movlw	0x90		; E0を前置するコードかどうか?
	subwf	KeyWk,W		; KeyWk - 0x90 = (W)
	jnc	KeySendF0_1	; 引けなかった.	引けないとき NC 

	movlw	0xE0		; E0 F0 xx にする.
	call	KeySendW

KeySendF0_1:
	movlw	0xf0
	call	KeySendW	; 0xf0

	jmp	KeySend_7Wk	; KeyWkの xx を MSB 倒して送出.

; 1バイト表で定義不可キーの処理.
kF0_00:
	movfw	KeyPos
	sublw	0x69		; PRINT SCREEN
	jz	kF0_69

	movfw	KeyPos
	sublw	0x71		; PAUSE
	jz	kF0_71
	return

; PRINT SCREEN
kF0_69:
	btfsc	ADBshift	; [SHIFT]が押されている?
	jmp	kF1_69
	btfsc	ADBctrl		; [CTRL ]が押されている?
	jmp	kF1_69

; 単独の PRINTSCREEN のリリースは E0 F0 7c E0 F0 12 を送出.
	movlw	0x7c
	call	kE0_F0_W		; E0 F0 7C
	movlw	0x12
	jmp	kE0_F0_W		; E0 F0 12

; [ SHIFT | CTRL ] + PRINTSCREEN のリリースは SysRq(E0 F0 7C) を送出.
kF1_69:
	movlw	0x7c
	jmp	kE0_F0_W		; E0 F0 7C

; PAUSE
kF0_71:
; 単独の PAUSE はリリースコードを返さなくてよい.
	return




; 1バイト表で定義不可キーの処理.
kM0_00:
	movfw	KeyPos
	sublw	0x69		; PRINT SCREEN
	jz	kM0_69

	movfw	KeyPos
	sublw	0x71		; PAUSE
	jz	kM0_71
	return

; PRINT SCREEN = E0 12 E0 7C
kM0_69:
	btfsc	ADBshift	; [SHIFT]が押されている?
	jmp	kM1_69
	btfsc	ADBctrl		; [CTRL ]が押されている?
	jmp	kM1_69

; 単独の PRINTSCREEN は E0 12 E0 7C を送出.

	movlw	0x12
	call	kE0_W		; E0 12

; [ SHIFT | CTRL ] + PRINTSCREEN は SysRq(E07C) を送出.
kM1_69:
	movlw	0x7c
	jmp	kE0_W		; E0 7C

; PAUSE = E1 14 77 E1 F0 14 F0 77
kM0_71:
	btfsc	ADBshift	; [SHIFT]が押されている?
	jmp	kM1_71
	btfsc	ADBctrl		; [CTRL ]が押されている?
	jmp	kM1_71

; 単独の PAUSE は E1 14 77   E1 F0 14 F0 77 を送出.
	movlw	0x14
	call	kE1_W		; E1 14
	
	movlw	0x77
	call	KeySendW	; 77
	movlw	0xe1
	call	KeySendW	; E1

	movlw	0x14
	call	kF0_W		; F0 14

	movlw	0x77
	call	kF0_W		; F0 77
	return

; [ SHIFT | CTRL ] + PAUSE は Break(E0 7E E0 F0 7E) を送出.
kM1_71:
	movlw	0x7e
	call	kE0_W		; E0 7E

	movlw	0x7e		; E0 F0 7E

; E0 F0 xx
kE0_F0_W:
	movwf	KeyWk

	movlw	0xE0
	call	KeySendW	; E0

	jmp	kF0_Wx

; F0 xx
kF0_W:
	movwf	KeyWk
kF0_Wx:
	movlw	0xF0
	jmp	kW_Wk		; F0 Wk

; E1 xx
kE1_W:
	movwf	KeyWk

	movlw	0xE1
	jmp	kW_Wk		; E1 Wk

; E0 xx
kE0_W:
	movwf	KeyWk
	movlw	0xE0
kW_Wk:
	call	KeySendW
	
	movfw	KeyWk
	call	KeySendW
	return


;--------------------------------------------------------------
;	キーメークコード送出
;--------------------------------------------------------------
KeySend:
	movfw	KeyPos
	call	ADBtoAT
	iorlw	0x00
	jz	kM0_00
	
	movwf	KeyWk
	
	movlw	0x90		; E0を前置するコードかどうか?
	subwf	KeyWk,W		; KeyWk - 0x90 = (W)
	jnc	KeySend_1	; 引けなかった.	引けないとき NC 

	movlw	0xE0
	call	KeySendW

KeySend_7Wk:
	movlw	0x7F
	andwf	KeyWk,F
	
KeySend_1:
	movfw	KeyWk

;--------------------------------------------------------------
;	(W)キーコード送出
;--------------------------------------------------------------
KeySendW:
	movwf	SendByteLast
	movwf	SendByte
KeySendRxChk:
 	btfss	kbCLKi		; CLKがLoなら受信やってみる.
	call	KeyRecStart

	call	SendBitLo	; send start bit
	movl   	SendParity,1	; set ODD parity
	movl	SendCnt,8

KeySendLoop:
	movfw	SendByte
	xorwf	SendParity,F
	rrf	SendByte,F
	call	SendBit
	decfsz	SendCnt,F
	jmp	KeySendLoop

	rrf	SendParity,F	; check Parity
	call	SendBit

	jmp	SendBitHi	; Stop Bit

SendBit:
	jnc	SendBitLo	;;skpnc

SendBitHi:
	PinHI	kbDATAhi	    ; assume bit is 1
	jmp	SendBitX	;;skpc			    ; if CF=1, dit already set

SendBitLo:
	PinLO	kbDATAlo	    ; otherwise, set pin to 0
SendBitX:
	call	Wait12uS

	PinLO	kbCLKlo		    ;

	call	Wait20uS

	PinHI	kbCLKhi		    ;

	return


;--------------------------------------------------------------
;	KeyRecv1	;なにかが送られてきたら応答する.
;--------------------------------------------------------------
KeyRecv1:
 	btfsc	kbCLKi	; CLKがHiなら何もせずにreturn.
	return

;; CLKがLoになっていたら...
KeyRecStart:
 	btfss	kbCLKi	; CLKがLoの間ループ.
	jmp	KeyRecStart	;

	btfsc	kbDATAi	; DATAもHiならエラーreturn
	return
	
;--------------------------------------------------------------
;	 ホスト応答パターン:
;--------------------------------------------------------------

	call	KeyRecByte
KeyRespond:
	movlw	0xed
	subwf	RecByte,W	; RecByte - 0xed = (W)
	jc	respond_W	; 引けた. ed～ffのパタンに応答.

;-------------------------------------------------------
;  undefined host commands.
respond_F1:
respond_EF:
	return			;;応答しない.

;-------------------------------------------------------
;Set/reset status indicators.  This command allows you to control
;the status LEDs on the keyboard.  Keyboard responds with ACK and
;waits for a option byte, bitmapped as follows: b0-Scrollock,
;b1-Numlock, b2-Capslock, b3..7=0.  A '1' bit turns the indicator
;ON.
respond_ED:
	call	respond_FA
	call	KeyRecData
	return
;-------------------------------------------------------
;Echo.  Responds with ECHO code (EE).
respond_EE:
	movlw	0xee
	jmp	KeySendW
;-------------------------------------------------------
; Select scan code set.  Responds with ACK, then waits for host to
; send a byte (01,02 or 03) specifying the scan code set to use.
; If 00 is sent, keyboard responds with ACK followed by the scan code
; set in use.
respond_F0:
	call	respond_FA
	call	KeyRecData
	movfw	RecByte
	jz	respond_F000
	movwf	ResScanType
	return
respond_F000:
	;...
	return

;-------------------------------------------------------
;Read ID.  Responds with ACK and two ID bytes (83,AB).  Resumes
;scanning even if previously disabled.
respond_F2:
	call	respond_FA
	movlw	0xab
	call	KeySendW
	movlw	0x83
	jmp	KeySendW

;-------------------------------------------------------
;Set autorepeat rate/delay.  Responds with ACK, then waits for an
;option byte that specifies the autorepeat delay and rate,
;bitmapped as follows: b7-unused. b6..5-Repeat delay (00=250 mS,
;11=1000mS). b4..0-Repeat rate (00000=30x/sec, 11111=2x/sec).
;Keyboard responds with ACK after reception of the option byte.
respond_F3:
	call	respond_FA
	call	KeyRecData
	movfw	RecByte
	movwf	ResRateDelay

;-------------------------------------------------------
;Enable.  Clears output buffer, enabled kbd, returns ACK.
respond_F4:
	jmp	respond_FA

;-------------------------------------------------------
;Default Disable.  Resets keyboard, returns ACK and suspends
;scanning, waiting for another command.  Does not affect the
;indicator LEDs.
respond_F5:
;-------------------------------------------------------
;Set default.  Responds as the 'Default Disable' command, but does
;not inhibit scanning.  Does not affect LED indicators.
respond_F6:
	call	respond_FA
	call	KeyRecData
	jmp	KeyRespond

;-------------------------------------------------------
;'Set all keys typematic'
respond_F7:
	bcf	RepOFF
	jmp	respond_FA

;-------------------------------------------------------
;'Set all keys make/break'
respond_F8:
	bcf	BrkOFF
	jmp	respond_FA

;-------------------------------------------------------
;'Set all keys make'
respond_F9:
	bsf	BrkOFF
	jmp	respond_FA

;-------------------------------------------------------
;'Set all keys typematic/make/break'
;I am not certain what these commands do.  I suspect they control the
;way the keyboard transmits scan codes.  All of the above commands
;respond with an ACK code.
respond_FA:
	movlw	0xfa
	jmp	KeySendW

;-------------------------------------------------------
;'Set key type typematic'
respond_FB:
	bcf	RepOFF
	jmp	respond_FA
;-------------------------------------------------------
;'Set key type make/break'
respond_FC:
	bcf	BrkOFF
	jmp	respond_FA

;-------------------------------------------------------
;'Set key type make'
respond_FD:
	bsf	BrkOFF
	jmp	respond_FA

;-------------------------------------------------------
;Resend.  Retransmit last sent scan code.
respond_FE:
	movfw	SendByteLast
	jmp	KeySendW
;-------------------------------------------------------
;Reset.  Resets keyboard CPU, starts power-on test.  Responds with
;power-on-test byte.
respond_FF:
	call	respond_FA
	movlw	0xaa
	jmp	KeySendW



;--------------------------------------------------------------
;	ホストからのデータを受信する.
;--------------------------------------------------------------
KeyRecData:
	movl	RecByte,0	;ダミーデータをセット.
KeyRecData0:
	btfsc	kbCLKi	; CLKがHiなら待機.
	jmp	KeyRecData0

	;; CLKがLoになっていたら...
KeyRecData1:
 	btfss	kbCLKi	; CLKがLoの間ループ.
	jmp	KeyRecData1

	btfsc	kbDATAi	; DATAもHiならエラーreturn
	return

	;; CLKがHiに戻っても、DATAがLoのままなら、送信要求とみなす.

KeyRecByte:
;; キーボード側からクロックを供給して受信を行う.
	bsf    	SendParity,7	; set ODD parity
	movl	SendCnt,8
KeyRecLoop:
		call	KeyRecBit
		rrf	RecByte,F	; 受信したbit(Carry)を右詰め記憶.
;; パリティ計算.
		movfw	RecByte
		andlw	0x80
		xorwf	SendParity,F

	decfsz	SendCnt,F
	jmp	KeyRecLoop

	call	KeyRecBit	; パリティ受信.
	call	KeyRecBit	; ストップ受信.
	
	call	Wait20uS
	PinLO	kbDATAlo	; ハンドシェイクbit送信
	call	Wait12uS
	PinLO	kbCLKlo		;
	call	Wait12uS
	PinHI	kbCLKhi		;
	PinHI	kbDATAhi	;
	return

KeyRecBit:
	call	Wait20uS
	PinLO	kbCLKlo		    ;
	call	Wait12uS
	PinHI	kbCLKlo		    ;
	clrc			    ; presume data=0
	btfsc	kbDATAi		    ; test data bit
	setc			    ; correct to 1
	return




; =============================================================
; これ以降はＡＤＢバスのドライブルーチン

;--------------------------------------------------------------
;	ADB SUPPORT ROUTINES
;--------------------------------------------------------------

;==============================
;	１ビット送信.
;==============================

;// Cflag=1 bit1を送信.
;// Cflag=0 bit0を送信.
sadbit:
	jnc	sadbit0

	;if(cy) {
	;//     -+   +--------+
	;//      +---+        +-
	;//      35us   65us
	;//
sadbit1:
	movl	PORTA,MARK
	call	w35us
	movl	PORTA,SPACE
;	jmp	w65us
w65us:
	movlw	52
	jmp	KeyWait
	;}else{
	;//     -+        +---+
	;//      +--------+   +-
	;//       65us     35us
	;//
sadbit0:
	movl	PORTA,MARK
	call	w65us
	movl	PORTA,SPACE
;	jmp	w35us
w35us:
	movlw	27
	jmp	KeyWait

;==============================
;	１バイト送信( Breg )
;==============================

sadbyte:
	movl	Creg,8
sadloop:
	;do {
		;rlc(b);		;// MSB -> cy flag;
		rlf	Breg,F	; Bregを左シフトし、MSBのあふれをCarryビットに移す.
		call	sadbit;
	;}while(--c);
	decfsz	Creg,F
	jmp	sadloop

	jmp	sadbit0		;// STOP BIT


;// アテンション(800us のマーク信号を送り、70us待つ)
adb_atn:
	movl	PORTA,MARK
	call	w800us
	movl	PORTA,SPACE
	jmp	w65us

adb_cmd:
	;;b=a
	movwf	Breg
	call	adb_out
	 call	adb_atn
	 call	sadbyte
	call	adb_inp
	return

;--------------------------------------------------------------
;	ＡＤＢ受信部
;--------------------------------------------------------------
;//	READ ADB by ONE BIT (Duty TIME=> C reg) 

; ADBポートが０になるまで待つ.
cadbitst:
	clrf	Creg
	clrc
cadbilop:
		btfss	ADBi	;ADBi ビットが 0(low)ならリターン
		return;

	decfsz	Creg,F
	jmp	cadbilop	; -- Lreg != 0 ならループ.
	setc
	return

;
;	タイムアウト猶予の長い1ビット受信.
;
radbit0:
	movl	Breg,2
radbit0_l:
		call	cadbitst
		jnc	cadbited

	decfsz	Breg,F
	jmp	radbit0_l	; -- Breg != 0 ならループ.
	setc
	return


;
;	１ビット受信 ==> Creg にMARKのタイムを記憶.
;		タイムオーバーで CarryFLAG=1にする.
;
radbit:
	call	cadbitst	;// '0'になるのを待つ.
	rc			;;if(cy) return;
;;;	jmp	cadbited	;// '1'になるのを待つ.

;
;	ループ１回＝5 clock = 2uS (10MHz)
;
cadbited:
	clrf	Creg
	clrc
cadbitel:
		btfsc	ADBi	;ADBi ビットが 1(high)ならリターン
		return;

	incfsz	Creg,F
	jmp	cadbitel	; -- Lreg != 0 ならループ.
	setc
	return



;
;	１バイト受信 ==> Ereg
;
radbyte:
	clrf	Ereg
	movl	Breg,8
radbytel:
;	e=0;
;	b=8;do {
		call	radbit
		rc		;;if(cy) return;

		movfw	Creg
		sublw	ADBTIME ;; ADBTIME - Creg => W , Carry
		rlf	Ereg,F	; 受信したbit(Carry)を左詰め記憶.
;		a=c;a-=0x0e;
;		a=e;adc(a,a);e=a;
;		b--;
;	}while(nz);
	decfsz	Breg,F
	jmp	radbytel

	clrc
	return
;
;
get_key:
	incf	Ereg,W
	jz	getkey1
	;; Ereg に 受信データが残っているので、その値を返す.
		movfw	Ereg
		movwf	keybuf
		movl	Ereg,0xff
		return

;	=======================================================
;	ＡＤＢにセンスコマンドを発行してデータを受信する.
;
;	結果は、keybuf と Ereg に入る. (２バイト分受信するため)
;
;	結果の値は各キーの固有値(0x00～0x7e)
;	 もしMSBが立っている場合は、そのキーが離されたイベント.
;	 0xff が返ってきた場合は、キーイベント発生なし.
;	 同時に２バイトの情報がくる場合もある（イベントが溜まっている場合）
;	 が、大抵の場合、keybufのみに入り、Eregは0xffであることが多い。
;	=======================================================
getkey1:
	movlw	0xff		; キーバッファを空にしておく.
	movwf	keybuf
	movwf	Ereg		; Eregもついでに.

	movlw	0x2c		; ＡＤＢセンスコマンド発行.
	call	adb_cmd

	call	radbit0		; スタートビット取得.
	rc			;;if(cy) {return;}
;	a=c;if(a>=0x0e) {return;}

	call	radbyte		; ８ビット受信
	jc	getkey_err
	movfw	Ereg
	movwf	keybuf		;	       =>keybuf

	call	radbyte		; ８ビット受信 =>Ereg
	jc	getkey_err
	call	radbit		; ストップビット
	return

getkey_err:
	movlw	0xff		; キーバッファを空にしておく.
	movwf	keybuf
	movwf	Ereg		; Eregもついでに.
	return

;===========================================
adb_inp:
	;;ポートの入出力方向を決める.
	SetRPage 1

	movlw	B'11100011'	;; bit 2,3,4のみ出力.他は入力
	movwf	PORTA

	SetRPage 0
	return

;===========================================
adb_out:
	;;ポートの入出力方向を決める.

	SetRPage 1

	movlw	B'11100010'	;; bit 2,3,4のみ出力.他は入力
	movwf	PORTA

	SetRPage 0

	return

;===========================================
kbCLKlo:
	bsf	STATUS, RP0	;;SetRPage 1
	bcf	PORTBdir,1
	movfw	PORTBdir
	movwf	PORTB
	bcf	STATUS, RP0	;;SetRPage 0
	movwf	PORTB
	return
;===========================================
kbCLKhi:
	bsf	STATUS, RP0	;;SetRPage 1
	bsf	PORTBdir,1
	movfw	PORTBdir
	movwf	PORTB
	bcf	STATUS, RP0	;;SetRPage 0
	return
;===========================================
kbDATAlo:
	bsf	STATUS, RP0	;;SetRPage 1
	bcf	PORTBdir,0
	movfw	PORTBdir
	movwf	PORTB
	bcf	STATUS, RP0	;;SetRPage 0
	movwf	PORTB
	return
;===========================================
kbDATAhi:
	bsf	STATUS, RP0	;;SetRPage 1
	bsf	PORTBdir,0
	movfw	PORTBdir
	movwf	PORTB
	bcf	STATUS, RP0	;;SetRPage 0
	return
;===========================================
ADBlo:
	movlw	MARK
	bsf	STATUS, RP0	;;SetRPage 1
	movwf	PORTA
	bcf	STATUS, RP0	;;SetRPage 0
	movwf	PORTA
	return
;===========================================
ADBhi:
	movlw	SPACE
	bsf	STATUS, RP0	;;SetRPage 1
	movwf	PORTA
	bcf	STATUS, RP0	;;SetRPage 0
	movwf	PORTA
	return
;===========================================

 if	0

;	ビープサウンドによるデバッグ
;	PORTA bit4 にスピーカー(アンプ等)をつなぐ

;	１周期分の矩形波出力.
beep1cyc:
	movl	PORTA,0x11
	call	w800us
	movl	PORTA,0x01
	jmp	w800us

;	短い発振音.
beep1:
	movl	Creg,20
	jmp	bloop

;	長い発振音.
beep:
	movl	Creg,0
bloop:
	call	beep1cyc
	decfsz	Creg,F
	jmp	bloop
	return

;	長い発振音と同じ長さの沈黙.
beep0:
	movl	Creg,0
bloop0:
	call	w800us
	call	w800us
	decfsz	Creg,F
	jmp	bloop0
	return



;--------------------------------------------------------------
;	
;--------------------------------------------------------------
testmain:
	call	beep
	movlw	0x00
	call	adb_cmd
	call	Wait400m
testloop:
	movlw	0x2c
	call	adb_cmd
	call	adb_data
	jmp	testloop

 endif

	end
;
