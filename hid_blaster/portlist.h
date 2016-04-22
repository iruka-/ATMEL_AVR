/* portlist.h */

#define	_SFR_IO8( p )   (0x20+(p)),1
#define	_SFR_IO16( p )  (0x20+(p)),2

#define	QQ	0x10	//良く参照するポートに印を付ける.

typedef	struct {
	char *name;
	int   adrs;
	int	  attr;	// bit0:8bit , bit1:16bit ,bit4:quickviewflag
} PortList;


/* portlist.c */
int portAddress (char *s);
