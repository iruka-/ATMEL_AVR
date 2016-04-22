;======================================================================
;
;		ＡＴ８８０１ｍｋII-ＳＲ
;
;======================================================================
;
;	・PC8801mkIISRのキーボードをＡＴ互換機に接続する.
;	・用意するパーツは、PIC 16F84 １個と 74LS158一個.
;	・抵抗 2.2kΩ×4個,10kΩ×2個、2SCタイプの小型Tr２個.
;
;１６Ｆ８４の配線:
;----------------------------------------------------------------------
;	PORTA	0 - 3   OUT     74159N  4-16 SELECT
;	PORTA	4	OUT     74LS158 A-B  SELECT
;
;	PORTB	0 - 3	IN      74LS158 0-3 OUT
;	PORTB	4	in	KBD data
;	PORTB	5	out	KBD data
;	PORTB	6	in	KBD clock
;	PORTB	7	out	KBD clock
;
;	Clock inは10kΩでプルアップ.(約7MHz発信)
;	又は7～8MHzの水晶を推奨.
;	周波数が変わった場合は、ウェイトループの値を要調整.
;
;	リセットはプルアップすること.
;
;PORTB 4 - 7 の正確な配線は以下を参照.
;----------------------------------------------------------------------
;
;			     vcc	vcc
;			      | 	 |
;			      \ 	-+-
;			      / 2K2	/_\  1N4148
;			      \ 	 |
; hoCLKi    ------------------o---o------o---------   kbd CLOCK
;			      |   |	 |
;			 2N2222   |50pF -+-
; hoCLKo    ______/\/\/\____|/	 ===	/_\
;		    2K2     |\>   |	 |
;			      |   |	 |
;			     /// ///	///
;
; kbCLKとkbDATAは、同一回路でOK. ダイオードとコンデンサは省略可.
; 2N2222 は2SC1845のようなTrでOK.
;
;主要ルーチンとワーク.
;----------------------------------------------------------------------
;	KeyScanMain()
;		KeyPos		0..88      key number
;		KeyRow		0..11 ---> 74159N select signal
;		KeyCol		0..7  .... bitpos
;		KeyMask		0x01 .. 0x80 ... bitmask
;		KeyVal		KeyRowに対応するビットパタン.
;
;	KeyBits[12]	KeyValの並び
;
;
;


#include "p16f84.inc"
#include "custom.h"

; KeyBoard 出力 PinにはTransisterが入っているので負論理です.
PinLO  macro P, B
     bsf  P, B
 endm


PinHI  macro P, B
     bcf  P, B
 endm


;--------------------------------------------------------------
;	スタート.
;--------------------------------------------------------------
	jmp	_main

;--------------------------------------------------------------
;	キースキャン・ワーク
;--------------------------------------------------------------
 New	KeyPos		;;0..88      key number
 New	KeyRow		;;0..11 ---> 74159N select signal
 New	KeyCol		;;0..7  .... bitpos
 New	KeyMask		;;0x01 .. 0x80 ... bitmask
 New	KeyVal		;;KeyRowに対応するビットパタン.
 New	KeyWk		;;汎用ワーク.

 New	KeyRepCnt	;;キーリピートカウンタ.
 New	KeyRepFlag	;;キーリピートフラグ.
 New	KeyRepCode	;;キーリピートしたいキー.

 New	ScanMode	;;

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
;	ビットの定義
;--------------------------------------------------------------
#define	 KeyREPCLR	KeyRepFlag,7	;; キー状態が変化した.
#define	 KeyREPRESS	KeyRepFlag,6	;; キーは１つ以上おされている.

#define	 BrkOFF		ScanMode,0	;;ブレークコードは送らない.
#define	 RepOFF		ScanMode,1	;;キーリピートコードは送らない.

;--------------------------------------------------------------
;	ポート割り当ての定義
;--------------------------------------------------------------
#define  kbCLKi   	PORTB, 6	; clock line input
#define  kbCLKo   	PORTB, 7	; clock line output(inverted OpenCollector)
#define  kbDATAi  	PORTB, 4	; data line input
#define  kbDATAo  	PORTB, 5	; data line output(inverted OpenCollector)


;--------------------------------------------------------------
;	キースキャン・結果テーブル
;--------------------------------------------------------------
;	８ビット×１２Ｒｏｗ＝９６キー分。押されていたら１が立つ.

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


;--------------------------------------------------------------
;  ＰＣ８８のスキャンコードを１０６配列のスキャンコードに変換
;--------------------------------------------------------------
#include "tbl.inc"

;--------------------------------------------------------------
;  ＰＣ８８のキーでリピートしたくないビットに印を付ける.
;--------------------------------------------------------------
KeyRepMask:
	addwf	PC,F
	retlw	B'00000000'	; 0x00
	retlw	B'00000000'
	retlw	B'00000000'	; 0x10
	retlw	B'00000000'
	retlw	B'00000000'	; 0x20
	retlw	B'00000000'
	retlw	B'00000000'	; 0x30
	retlw	B'00000000'
	retlw	B'11110000'	; 0x40
	retlw	B'00000000'
	retlw	B'10000000'	; 0x50
	retlw	B'00000000'

;--------------------------------------------------------------
;	カラム番号から、入力PORTのマスクパターンへの変換.
;--------------------------------------------------------------
Col2Mask:
	andlw	0x07
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
;	メイン処理開始.
;--------------------------------------------------------------
_main:
	;;ポートの入出力方向を決める.
	SetRPage 1
	clrw			;;	PORT Aは全出力
	movwf	PORTA
	movlw	B'01011111'	;; bit 5 ,7のみ出力.他は入力
	movwf	PORTB

	SetRPage 0

	;;出力ピンの値を初期化する.
	PinHI  kbCLKo
	PinHI  kbDATAo

	;;システム全体が安定するまで待つ.
	call	Wait1000mS

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
	clrf	KeyPos		;;キーセンスポインタ.
	bcf	KeyREPCLR	;;キー変化フラグを初期化.
	bcf	KeyREPRESS	;;キーがどれか押されているフラグ.


;;キーを全部センスするループの先頭:
KeyScan_2:
	call	KeyRecv1	;;なにか送られてきたら受信する.
	call	KeyScan1	;;キーを１個センスする.

	incf	KeyPos,F	;; ポインタ ＋１
	movfw	KeyPos
	xorlw	0x5a		;;最終コードに達するまで.
	jnz	KeyScan_2
;;キーを全部センスするループはここまで:


;;一周回ったあとで、押されっぱなしキーがあれば、リピート処理をする.
	btfsc	KeyREPCLR
	jmp	KeyScan_0	;;変化があったので、リピートキャンセル.
	btfss	KeyREPRESS
	jmp	KeyScan_0	;;キーは何も押されていない.
	btfsc	RepOFF
	jmp	KeyScan_0	;;タイパマティック送信しない.

	decfsz	KeyRepCnt,F	;;リピート開始カウントに達するまでループ.
	jmp	KeyScan_1

;;リピートの実行.
	movfw	KeyRepCode
	movwf	KeyPos
	call	KeySend

	movl	KeyRepCnt,0x0c	;;リピート開始カウントダウンカウンタ巻き戻し.
	jmp	KeyScan_1

;--------------------------------------------------------------
;	キースキャン・小物ルーチン
;--------------------------------------------------------------

;--------------------------------------------------------------
;	列アドレスをセットして、数μ秒(TTLが落ち着くまで)待つ.
;--------------------------------------------------------------
KeyRowOut:
	movwf	PORTA	;列アドレスをPORT Aの下位４ビット(74159N)に与える.
KeyW:
	movl	SendWk,8
KeyW_1:
	decfsz	SendWk,F	; 1
	jmp	KeyW_1		; 2
	ret
Wait12uS:
	movl	SendWk,6
	jmp	KeyW_1
Wait20uS:
	movl	SendWk,12
	jmp	KeyW_1
Wait400uS:
	movl	SendWk,0
	jmp	KeyW_1
;--------------------------------------------------------------
;	
;--------------------------------------------------------------
Wait1000mS:
	movl	KeyWk,0		;1
_loop1:
	call	Wait400uS
	decfsz	KeyWk,F
	jmp	_loop1
	ret


;--------------------------------------------------------------
;	KeyRowに当たるマトリクスデータを１バイト取ってくる
;--------------------------------------------------------------
KeyGetRow:
	movfw	KeyRow		;; Row出力.
	call	KeyRowOut

	movfw	PORTB		;; ４ビット取り込み.
	andlw	0x0f
	movwf	KeyVal

	movfw	KeyRow
	iorlw	0x10		;; LS158切り替え.
	call	KeyRowOut

	movfw	PORTB		;; ４ビット取り込み.
	andlw	0x0f
	movwf	KeyWk
	
	swapf	KeyWk,W		;; W = swap(KeyWk);
	iorwf	KeyVal,F	;; KeyVal |= W		4bit と 4bit を合成

	ret

;--------------------------------------------------------------
;	KeyBits[KeyRow] テーブルをインデックス参照する為の準備.
;--------------------------------------------------------------
KeyBitAdr:
	movlw	KeyBits
	addwf	KeyRow,W
	movwf	FSR		;; 間接Register = KeyBits+KeyRow
	ret

;--------------------------------------------------------------
;	KeyPosに当たるビットを検査する(Z フラグ).
;--------------------------------------------------------------
KeyBitChk:
	call	KeyBitAdr
	movfw	KeyMask
	andwf	INDF,W		;; メモリー間接参照.
	ret

;--------------------------------------------------------------
;	KeyPosに当たるビットを立てる.
;--------------------------------------------------------------
KeyBitSet:
	call	KeyBitAdr
	movfw	KeyMask
	iorwf	INDF,F		;; メモリー間接参照.
	ret

;--------------------------------------------------------------
;	KeyPosに当たるビットを倒す.
;--------------------------------------------------------------
KeyBitReset:
	call	KeyBitAdr
	movfw	KeyMask
	xorlw	0xff		;マスクを反転して＆を取る.
	andwf	INDF,F		;; メモリー間接参照.
	ret

;--------------------------------------------------------------
;	キービット配列を全部消去する.
;--------------------------------------------------------------
KeyBitsInit:
	clrf	KeyRow
	movl	KeyWk,12

KeyBitsInit1:
	call	KeyBitAdr
	clrf	INDF
	incf	KeyRow,F
	decfsz	KeyWk,F
	jmp	KeyBitsInit1
	ret


;--------------------------------------------------------------
;	現在スキャンしたキーがリピート不可だったら Not Zero にする
;--------------------------------------------------------------
KeyRepMaskChk:
	movfw	KeyRow
	call	KeyRepMask
	andwf	KeyMask,W
	ret

;--------------------------------------------------------------
;	KeyPosに当たるビットをKeyValにフェッチする.
;--------------------------------------------------------------
;	副作用として、KeyRow,KeyCol,KeyMask がセットされる.
KeyGetBit:
	;;; KeyRow = KeyPos >> 3;
	ld	KeyRow,KeyPos
	rrf	KeyRow,F
	rrf	KeyRow,F
	rrf	KeyRow,W
	andlw	0x0f
	movwf	KeyRow

	;;; KeyCol = KeyPos & 7
	movfw	KeyPos
	andlw	0x07
	movwf	KeyCol
	
	;;; KeyMask = masktable[KeyCol]
	call	Col2Mask
	movwf	KeyMask

	call	KeyGetRow
	movfw	KeyMask
	andwf	KeyVal,F

	;;; Repeat Key Pressed?
	jnz	KeyGetBit_REPON
	ret

KeyGetBit_REPON:
	call	KeyRepMaskChk
	skpnz
	bsf	KeyREPRESS	;;リピート可能なキーが押されている.
	ret

;--------------------------------------------------------------
;	KeyPos番目に当たるキーを１個検査する
;--------------------------------------------------------------
;	変化が有ったら、コードを互換機に送信する.
KeyScan1:
	call	KeyGetBit	;;マトリクスを1bit分スキャン.
	call	KeyBitChk	;;ワークエリアと比較.
	xorwf	KeyVal,W
	btfsc	STATUS,Z
	ret			;;変化無し：一致していたらreturn

;ビットの変化有り.
	bsf	KeyREPCLR	;;変化があったら、キー変化フラグをOn
	movfw	KeyVal
	jz	KeyTurnOff

;キーが押された.
KeyTurnOn:
	call	KeyBitSet
	call	KeySend
	call	KeyRepMaskChk
	skpz
	ret
;;リピート可能なキーの場合、そのコードをためる.
	movfw	KeyPos
	movwf	KeyRepCode
	ret

;キーが離された.
KeyTurnOff:
	call	KeyBitReset
	btfsc	BrkOFF		;;キーブレーク時にコードを送る?
	ret			;;bitが立っていたら送らない.
	jmp	KeySendF0

;--------------------------------------------------------------
;	キーブレークコード送出
;--------------------------------------------------------------
KeySendF0:
	movfw	KeyPos
	call	pc88toAT
	iorlw	0x00
	btfsc   STATUS, Z
	ret

	movwf	KeyWk
	
	movlw	0x80		; E0を前置するコードかどうか?
	subwf	KeyWk,W		; KeyWk - 0x80 --> (W)
	jnc	KeySendF0_1	; 引けなかった.	引けないとき NC 

	movlw	0xE0		; E0 F0 xx にする.
	call	KeySendW

KeySendF0_1:
	movlw	0xf0
	call	KeySendW	; 0xf0

	jmp	KeySend_7Wk	; KeyWkの xx を MSB 倒して送出.

;--------------------------------------------------------------
;	キーメークコード送出
;--------------------------------------------------------------
KeySend:
	movfw	KeyPos
	call	pc88toAT
	iorlw	0x00
	btfsc   STATUS, Z
	ret
	
	movwf	KeyWk
	
	movlw	0x80		; E0を前置するコードかどうか?
	subwf	KeyWk,W		; KeyWk - 0x80 --> (W)
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
KeySendLast:
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

	PinHI	kbDATAo		; Stop Bit
	jmp	SendBitX

SendBit:
	skpnc
	PinHI	kbDATAo		    ; assume bit is 1
	skpc			    ; if CF=1, dit already set
SendBitLo:
	PinLO	kbDATAo		    ; otherwise, set pin to 0
SendBitX:
	call	Wait12uS

	PinLO	kbCLKo		    ;

	call	Wait20uS

	PinHI	kbCLKo		    ;

	ret


;--------------------------------------------------------------
;	KeyRecv1	;なにかが送られてきたら応答する.
;--------------------------------------------------------------
KeyRecv1:
 	btfsc	kbCLKi	; CLKがHiなら何もせずにreturn.
	ret

;; CLKがLoになっていたら...
KeyRecStart:
 	btfss	kbCLKi	; CLKがLoの間ループ.
	jmp	KeyRecStart	;

	btfsc	kbDATAi	; DATAもHiならエラーreturn
	ret
	
;--------------------------------------------------------------
;	 ホスト応答パターン:
;--------------------------------------------------------------

	call	KeyRecByte
KeyRespond:
	movlw	0xed
	subwf	RecByte,W	; RecByte - 0xed --> (W)
	jc	respond_W	; 引けた. ed～ffのパタンに応答.

;-------------------------------------------------------
;  undefined host commands.
respond_F1:
respond_EF:
	ret			;;応答しない.

;-------------------------------------------------------
;Set/reset status indicators.  This command allows you to control
;the status LEDs on the keyboard.  Keyboard responds with ACK and
;waits for a option byte, bitmapped as follows: b0-Scrollock,
;b1-Numlock, b2-Capslock, b3..7=0.  A '1' bit turns the indicator
;ON.
respond_ED:
	call	respond_FA
	call	KeyRecData
	ret
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
	ret
respond_F000:
	;...
	ret

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
	movlw	SendByteLast
	jmp	KeySendLast
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
	ret

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
	PinLO	kbDATAo		; ハンドシェイクbit送信
	call	Wait12uS
	PinLO	kbCLKo		;
	call	Wait12uS
	PinHI	kbCLKo		;
	PinHI	kbDATAo		;
	ret

KeyRecBit:
	call	Wait20uS
	PinLO	kbCLKo		    ;
	call	Wait12uS
	PinHI	kbCLKo		    ;
	clrc			    ; presume data=0
	btfsc	kbDATAi		    ; test data bit
	setc			    ; correct to 1
	ret


;--------------------------------------------------------------
;	
;--------------------------------------------------------------
	end
;
