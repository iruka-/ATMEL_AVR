;
;
;
#define	E0	0x80

pc88toAT:
	andlw	0x7f
	addwf	PC,F

	retlw	0x6b|E0	;	00	ç∂
	retlw	0x00	;	01	1
	retlw	0x75|E0	;	02	è„
	retlw	0x00	;	03	3
	retlw	0x79	;	04	4(+)
	retlw	0x59	;	05	5(=)
	retlw	0x5a	;	06	6(CR)
	retlw	0x71|E0	;	07	7(DEL)
	retlw	0x69|E0	;	08	8(END)
	retlw	0x7a|E0	;	09	9(ROLLDOWN)
	retlw	0x71	;	0a	.
	retlw	0x41	;	0b	,
	retlw	0x00	;	0c	
	retlw	0x72|E0	;	0d	â∫
	retlw	0x74|E0	;	0e	âE
	retlw	0x5a	;	0f	RETURN

	retlw	0x54	;	10	@
	retlw	0x1c	;	11	a
	retlw	0x32	;	12	b
	retlw	0x21	;	13	c
	retlw	0x23	;	14	d
	retlw	0x24	;	15	e
	retlw	0x2b	;	16	f
	retlw	0x34	;	17	g
	retlw	0x33	;	18	h
	retlw	0x43	;	19	i
	retlw	0x3b	;	1a	j
	retlw	0x42	;	1b	k
	retlw	0x4b	;	1c	l
	retlw	0x3a	;	1d	m
	retlw	0x31	;	1e	n
	retlw	0x44	;	1f	o

	retlw	0x4d	;	20	p
	retlw	0x15	;	21	q
	retlw	0x2d	;	22	r
	retlw	0x1b	;	23	s
	retlw	0x2c	;	24	t
	retlw	0x3c	;	25	u
	retlw	0x2a	;	26	v
	retlw	0x1d	;	27	w
	retlw	0x22	;	28	x
	retlw	0x35	;	29	y
	retlw	0x1a	;	2a	z
	retlw	0x54	;	2b	[
	retlw	0x5d	;	2c	Åè
	retlw	0x5b	;	2d	] 
	retlw	0x55	;	2e	^ 
	retlw	0x4e	;	2f	- 

	retlw	0x45	;	30	0
	retlw	0x16	;	31	1
	retlw	0x1e	;	32	2
	retlw	0x26	;	33	3
	retlw	0x25	;	34	4
	retlw	0x2e	;	35	5
	retlw	0x36	;	36	6
	retlw	0x3d	;	37	7
	retlw	0x3e	;	38	8
	retlw	0x46	;	39	9
	retlw	0x52	;	3a	:
	retlw	0x4c	;	3b	;
	retlw	0x41	;	3c	<
	retlw	0x49	;	3d	>
	retlw	0x4a	;	3e	?
	retlw	0x51	;	3f	_

	retlw	0x70|E0	;	40	ins
	retlw	0x0a	;	41	F8
	retlw	0x78	;	42	F11
	retlw	0x66	;	43	BS
	retlw	0x11	;	44	GRPH
	retlw	0x0e	;	45	KANA
	retlw	0x12	;	46	SHIFT
	retlw	0x14	;	47	CTRL
	retlw	0x00	;	48	STOP
	retlw	0x05	;	49	F1
	retlw	0x06	;	4a	F2
	retlw	0x04	;	4b	F3
	retlw	0x0c	;	4c	F4
	retlw	0x03	;	4d	F5
	retlw	0x29	;	4e	SPACE
	retlw	0x76	;	4f	ESC

	retlw	0x0d	;	50	TAB
	retlw	0x01	;	51	F9
	retlw	0x09	;	52	F10
	retlw	0x6c|E0	;	53	HOME
	retlw	0x00	;	54	*(COPY)
	retlw	0x7d|E0	;	55	ROLLUP
	retlw	0x07	;	56	F12
	retlw	0x58	;	57	CAPS
	retlw	0x0b	;	58	F6
	retlw	0x83	;	59	F7
	retlw	0x00	;	5a	
	retlw	0x00	;	5b	
	retlw	0x00	;	5c	
	retlw	0x00	;	5d	
	retlw	0x00	;	5e	
	retlw	0x00	;	5f	
	
	