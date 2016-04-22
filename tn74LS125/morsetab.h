#define	MORSE(pat_,len_)	(pat_*8|len_)

PROGMEM uchar morse_tab[26+10]={
//	MORSE(0b00000	,0),	// ÉAÉãÉtÉ@ÉxÉbÉg
	MORSE(0b10000	,2),	// Ç`.-
	MORSE(0b01110	,4),	// Ça-...
	MORSE(0b01010	,4),	// Çb-.-.
	MORSE(0b01100	,3),	// Çc-..
	MORSE(0b10000	,1),	// Çd.
	MORSE(0b11010	,4),	// Çe..-.
	MORSE(0b00100	,3),	// Çf--.
	MORSE(0b11110	,4),	// Çg....
	MORSE(0b11000	,2),	// Çh..
	MORSE(0b10000	,4),	// Çi.---
	MORSE(0b01000	,3),	// Çj-.-
	MORSE(0b10110	,4),	// Çk.-..
	MORSE(0b00000	,2),	// Çl--
	MORSE(0b01000	,2),	// Çm-.
	MORSE(0b00000	,3),	// Çn---
	MORSE(0b10010	,4),	// Ço.--.
	MORSE(0b00100	,4),	// Çp--.-
	MORSE(0b10100	,3),	// Çq.-.
	MORSE(0b11100	,3),	// Çr...
	MORSE(0b00000	,1),	// Çs-
	MORSE(0b11000	,3),	// Çt..-
	MORSE(0b11100	,4),	// Çu...-
	MORSE(0b10000	,3),	// Çv.--
	MORSE(0b01100	,4),	// Çw-..-
	MORSE(0b01000	,4),	// Çx-.--
	MORSE(0b00110	,4),	// Çy--..
//	MORSE(0b00000	,0),	// êîéö
	MORSE(0b00000	,5),	// ÇO-----
	MORSE(0b10000	,5),	// ÇP.----
	MORSE(0b11000	,5),	// ÇQ..---
	MORSE(0b11100	,5),	// ÇR...--
	MORSE(0b11110	,5),	// ÇS....-
	MORSE(0b11111	,5),	// ÇT.....
	MORSE(0b01111	,5),	// ÇU-....
	MORSE(0b00111	,5),	// ÇV--...
	MORSE(0b00011	,5),	// ÇW---..
	MORSE(0b00001	,5),	// ÇX----.
//	MORSE(0b00000	,0),	// 
};

