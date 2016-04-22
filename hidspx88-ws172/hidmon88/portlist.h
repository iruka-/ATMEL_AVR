/*
   0x0000 - 0x001F - AVR registers
   0x0020 - 0x005F - AVR I/O space
   0x0060 - 0x07FF - AVR data SRAM
 */

#define	_SFR_IO8( p )   (0x20+(p)),1
#define	_SFR_IO16( p )  (0x20+(p)),2

// Memory Mapped I/O (0x60～0xff)
#define	_SFR_MEM8( p )  ((p)),1
#define	_SFR_MEM16( p ) ((p)),2

#define	QQ	0x10	//良く参照するポートに印を付ける.

typedef	struct {
	char *name;
	int   adrs;
	int	  attr;	// bit0:8bit , bit1:16bit ,bit4:quickviewflag
} PortList;

/*********************************************************************
 *	ポート名称	,  アドレス , 属性（バイト幅、よく参照されるかどうか）
 *********************************************************************
 */

#if	0
#include "portlist_2313.h"
#else
#include "portlist_mega88.h"
#endif

/*********************************************************************
 *	大文字小文字を区別しない文字列比較＋bit属性検出
 *********************************************************************
 */
int str_casecmpx(char *s,char *t,int *pmask)
{
	int bit;
	while(*t) {
		if(tolower(*s) != tolower(*t)) return 1;	//不一致.
		s++;
		t++;
	}
	if(*s==0) {
		*pmask = 0; return 0;	//一致. 
	}
	if(*s=='.') {
		s++;
		if(s[1]==0) {
			bit = s[0] - '0';
			if((bit >= 0) && (bit < 8)) {
				*pmask = (1 << bit);
				return 0;		// port.bit (bitは0～7) 一致.
			}
		}
	}
	return 1;	//不一致.
}

/*********************************************************************
 *	ポート名称からアドレスを求める.
 *********************************************************************
 */
int	portAddress(char *s)
{
	PortList *p = portList;
	while(p->name) {
		if(stricmp(s,p->name)==0) return p->adrs;
		p++;
	}
	return 0;
}

/*********************************************************************
 *	ポート名称からアドレスとビットマスクを求める.
 *********************************************************************
 */
int	portAddress_b(char *s,int *pmask)
{
	PortList *p = portList;
	while(p->name) {
		if(str_casecmpx(s,p->name,pmask)==0) {
			return p->adrs;
		}
		p++;
	}
	return 0;
}

#define	COLS 4

/*********************************************************************
 *	ポート一覧表を表示する.
 *********************************************************************
 */
void PrintPortNameList(void)
{
	PortList *p = portList;
	int      i,k;
	int      m = 0;
	char     buf[128][128];

	memset(buf,0,sizeof(buf));
	
	while(p->name) {
		sprintf(buf[m],"%8s = 0x%02x ",p->name,p->adrs);
		m++;
		p++;
	}
	//段組みしたい.
	k = (m+COLS-1)/COLS;	//COLS段にした場合の行数.
	for(i=0;i<k;i++) {
		printf("%s%s%s%s\n",buf[i],buf[i+k],buf[i+k*2],buf[i+k*3]);
	}
}

/*********************************************************************
 *	数値 d を二進数文字列に変換し buf に格納する. 
 *		bufと同じアドレスを返す.
 *********************************************************************
 */
char *radix2str(char *buf,int d)
{
	char *rc = buf;
	int i,m,c=' ';
	m = 0x80;
	for(i=0;i<8;i++) {
		if(d & m) c='1';
		else      c='0';
		*buf++ = c;
		m >>= 1;
		if(i==3) *buf++ = '_';
	}
	*buf = 0;
	return rc;
}

/*********************************************************************
 *	段組表示によるポート値出力.
 *********************************************************************
 */
void PrintPortColumn(void)
{
	PortList *p = portList;
	int      i,k;
	int      m = 0;
	static   char     buf[128][128];	//結果バッファ（段組）

	char     tmp[128];		//２進数化するバッファ.
	
	uchar data[0x20 + 0x40];//ポート読み取り値.
	UsbRead(0x20,AREA_RAM,data+0x20,0x40);
//	memdump(data+0x20,0x40,0x20);return;

	memset(buf,0,sizeof(buf));
	
	while(p->name) {
		if(p->attr & 1) {
			sprintf(buf[m],"%6s = %s ",p->name,radix2str(tmp,data[p->adrs]));
			m++;
//			sprintf(buf[m],"%6s = %x ",p->name,p->adrs);
//			m++;
		}
		p++;
	}
	//段組みしたい.
	k = (m+COLS-1)/COLS;	//COLS段にした場合の行数.
	for(i=0;i<k;i++) {
		printf("%s%s%s%s\n",buf[i],buf[i+k],buf[i+k*2],buf[i+k*3]);
	}
}

/*********************************************************************
 *	ポート内容を全部ダンプする.
 *********************************************************************
 */
void PrintPortAll(int mask)
{
	PortList *p = portList;
	uchar data[0x20 + 0x40];//ポート読み取り値.
	UsbRead(0x20,AREA_RAM,data+0x20,0x40);
//	memdump(data+0x20,0x40,0x20);return;
	while(p->name) {
		if(p->attr & 1) {
			if((mask==0)||(p->attr & QQ)) {
				cmdPortPrintOne(p->name,p->adrs,data[p->adrs]);
			}
		}
		p++;
	}
}

/*********************************************************************
 *
 *********************************************************************
 */
