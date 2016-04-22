;
;========================================================
;	ADBのキーボードの固有値(0～127)を
;	IBM PC/ATのキーコードに変換するテーブル.
;========================================================
;
#define	E0	0x80	; E0 + nn となるキー (MSBを識別に使用)

			; 実は 0x83 というキー値が存在するが、
			; これは例外的に E0 を前置しない.
			; E0 を前置するかどうかの判断は、キー値
			; が 0x90以上かどうかでおこなう.
			;(0x80～0x8fの範囲にE0を前置するキーが
			; 新設された場合や、
			; 0x90～0xffの範囲にE0を前置しないキーが
			; 新設された場合はこの表は破綻する)
			
#define	kFF	0x00	; 未定義 invalid

ADBtoAT:
	andlw	0x7f
	addwf	PC,F

;		PC/AT		ADB	[key]
	retlw	0x1c	;	00	A
	retlw	0x1b	;	01	S
	retlw	0x23	;	02	D
	retlw	0x2b	;	03	F
	retlw	0x33	;	04	H
	retlw	0x34	;	05	G
	retlw	0x1a	;	06	Z
	retlw	0x22	;	07	X
	retlw	0x21	;	08	C
	retlw	0x2a	;	09	V
	retlw	kFF	;	0a	
	retlw	0x32	;	0b	B
	retlw	0x15	;	0c	Q
	retlw	0x1d	;	0d	W
	retlw	0x24	;	0e	E
	retlw	0x2d	;	0f	R

	retlw	0x35	;	10	Y
	retlw	0x2c	;	11	T
	retlw	0x16	;	12	1
	retlw	0x1e	;	13	2
	retlw	0x26	;	14	3
	retlw	0x25	;	15	4
	retlw	0x36	;	16	6
	retlw	0x2e	;	17	5
	retlw	0x55	;	18	=
	retlw	0x46	;	19	9
	retlw	0x3d	;	1a	7
	retlw	0x4e	;	1b	-
	retlw	0x3e	;	1c	8
	retlw	0x45	;	1d	0
	retlw	0x5b	;	1e	]
	retlw	0x44	;	1f	O

	retlw	0x3c	;	20	U
	retlw	0x54	;	21	[
	retlw	0x43	;	22	I
	retlw	0x4d	;	23	P
	retlw	0x5a	;	24	RET
	retlw	0x4b	;	25	L
	retlw	0x3b	;	26	J
	retlw	0x52	;	27	'
	retlw	0x42	;	28	K
	retlw	0x4c	;	29	;
	retlw	0x5d	;	2a	￥
	retlw	0x41	;	2b	,
	retlw	0x4a	;	2c	/
	retlw	0x31	;	2d	N
	retlw	0x3a	;	2e	M
	retlw	0x49	;	2f	.

	retlw	0x0d	;	30	TAB
	retlw	0x29	;	31	SPACE
	retlw	0x0e	;	32	~
	retlw	0x66	;	33	BS
	retlw	kFF	;	34	
	retlw	0x76	;	35	ESC
	retlw	0x14	;	36	CTRL
	retlw	0x11	;	37	COMMAND(ALT)
	retlw	0x12	;	38	SHIFT
	retlw	0x58	;	39	CAPS LOCK(alternative)
	retlw	0x14	;	3a	OPTION
	retlw	0x6b|E0	;	3b	←
	retlw	0x74|E0	;	3c	→
	retlw	0x72|E0	;	3d	↓
	retlw	0x75|E0	;	3e	↑
	retlw	kFF	;	3f	

	retlw	kFF	;	40	
	retlw	0x71	;	41	.(TENKEY)
	retlw	kFF	;	42	
	retlw	0x7c	;	43	*(TENKEY)
	retlw	kFF	;	44	
	retlw	0x79	;	45	+(TENKEY)
	retlw	kFF	;	46	
	retlw	0x77	;	47	CLEAR(NUM LOCK)
	retlw	kFF	;	48	
	retlw	kFF	;	49	
	retlw	kFF	;	4a	
	retlw	0x4a|E0	;	4b	/(TENKEY)
	retlw	0x5a|E0	;	4c	ENTER(TENKEY)
	retlw	kFF	;	4d	
	retlw	0x7b	;	4e	-(TENKEY)
	retlw	kFF	;	4f	

	retlw	kFF	;	50	
	retlw	0x55	;	51	=(TENKEY)
	retlw	0x70	;	52	0(TENKEY)
	retlw	0x69	;	53	1(TENKEY)
	retlw	0x72	;	54	2(TENKEY)
	retlw	0x7a	;	55	3(TENKEY)
	retlw	0x6b	;	56	4(TENKEY)
	retlw	0x73	;	57	5(TENKEY)
	retlw	0x74	;	58	6(TENKEY)
	retlw	0x6c	;	59	7(TENKEY)
	retlw	kFF	;	5a	
	retlw	0x75	;	5b	8(TENKEY)
	retlw	0x7d	;	5c	9(TENKEY)
	retlw	kFF	;	5d	
	retlw	kFF	;	5e	
	retlw	kFF	;	5f	
	
	retlw	0x03	;	60	F5
	retlw	0x0b	;	61	F6
	retlw	0x83	;	62	F7
	retlw	0x04	;	63	F3
	retlw	0x0a	;	64	F8
	retlw	0x01	;	65	F9
	retlw	kFF	;	66	
	retlw	0x78	;	67	F11
	retlw	kFF	;	68	
	retlw	kFF	;	69	PRINT
	retlw	kFF	;	6a	
	retlw	0x7e	;	6b	SCREEN
	retlw	kFF	;	6c	
	retlw	0x09	;	6d	F10
	retlw	kFF	;	6e	
	retlw	0x07	;	6f	F12
	
	retlw	kFF	;	70	
	retlw	kFF	;	71	PAUSE
	retlw	0x70|E0	;	72	HELP(INS)
	retlw	0x6c|E0	;	73	HOME(*)
	retlw	0x7d|E0	;	74	ROLL UP
	retlw	0x71|E0	;	75	DEL(TENKEY)
	retlw	0x0c	;	76	F4
	retlw	0x69|E0	;	77	END(*)
	retlw	0x06	;	78	F2
	retlw	0x7a|E0	;	79	ROLL DOWN
	retlw	0x05	;	7a	F1
	retlw	kFF	;	7b	
	retlw	kFF	;	7c	
	retlw	kFF	;	7d	
	retlw	kFF	;	7e	
	retlw	kFF	;	7f	POWER ON(*)

; (*) = full size keyboard only
