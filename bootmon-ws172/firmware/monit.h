
static void cmd_caps(void)
{
#if	0
	static uchar caps[6]={
		SPM_PAGESIZE & 0xff, SPM_PAGESIZE >>	8,

		( (long)FLASHEND + 1)        & 0xff,
		(((long)FLASHEND + 1) >> 8)	 & 0xff,
		(((long)FLASHEND + 1) >> 16) & 0xff,
		(((long)FLASHEND + 1) >> 24) & 0xff
	};
	memcpy(usbData+1,caps,6);
#else
	register uchar *t asm("r28") = usbData;
	t[1] = SPM_PAGESIZE & 0xff;
	t[2] = SPM_PAGESIZE >>	8;
	t[3] =		( (long)FLASHEND + 1)        & 0xff;
	t[4] =		(((long)FLASHEND + 1) >> 8)	 & 0xff;
	t[5] =		(((long)FLASHEND + 1) >> 16) & 0xff;
	t[6] =		(((long)FLASHEND + 1) >> 24) & 0xff;
#endif
}



#if	INCLUDE_MONITOR_CMD

#include "hidcmd.h"

//#define	DEV_ID	0x5a
//#define	DEV_ID	0x55
#define	DEV_ID	0x88

enum {
	AREA_RAM    = 0   ,
	AREA_EEPROM = 0x20,
	AREA_PGMEM  = 0x40,
//	reserved      0x60
	AREA_MASK	= 0x60,
	COUNT_MASK	= 0x1f,
};

//	メモリーを読み書きするコマンド構造.
typedef struct {
	uchar cmd;			// 62=poke 63=peek
	uchar count;		// 読み込みバイト数.
	uchar *addr;		// 読み書きアドレス.
	uchar memdata[8];	// 読み込みデータ. もしくは書き込みデータ[0] マスク[1]
} MonCommand_t;


//メモリー連続読み出し.
static void cmd_peek(MonCommand_t *cmd)
{
	uchar *p =cmd->addr;
	uchar *t =usbData+1;
	uchar cnt =cmd->count & COUNT_MASK;
	uchar area=cmd->count & AREA_MASK;
	if(area == AREA_RAM) {
#if	0
		memcpy(t,p,cnt);
#else
		uchar i=0;
//		for(i=0;i<cnt;i++) t[i]=p[i];
		do {
			t[i]=p[i];i++;
		}while(i<cnt);
#endif
	}else
#if	INCLUDE_EEPROM_RW
	if(area == AREA_EEPROM) {
		eeprom_read_block(t,p,cnt);
	}else
#endif
	if(area == AREA_PGMEM) {
		memcpy_P(t,p,cnt);
	}
}
//メモリー書き込み.(1バイト)
static void cmd_poke(MonCommand_t *cmd)
{
	uchar *p =cmd->addr;
	uchar data=cmd->memdata[0];
	uchar mask=cmd->memdata[1];

#if	INCLUDE_EEPROM_RW
	uchar area=cmd->count & AREA_MASK;
	if(area == AREA_EEPROM) {
		eeprom_write_byte(p,data);
		return;
	}
#endif
	if(mask) {	//マスク付の書き込み.
		*p = (*p & mask) | data;
	}else{			//べた書き込み.
		*p = data;
	}
}

//POLLアドレスの設定.
static void cmd_page(MonCommand_t *cmd)
{
	page_addr = cmd->addr;
}

//
static void cmd_jmp(MonCommand_t *cmd)
{
	void (*usrfunc)();

	int p = ((int) cmd->addr);
	usrfunc = (void*) p;
	usrfunc();
}

/* ------------------------------------------------------------------------- */
/* ---------------------------   monitor main   ---------------------------- */
/* ------------------------------------------------------------------------- */

void monit_main(uchar *data)
{
	data++;		// skip ReportID.
	switch( *data ) {
	 case HIDASP_TEST:
		usbData[1] = DEV_ID;
		usbData[2] = data[1];
		break;
	 case HIDASP_GET_CAPS:
		cmd_caps();
		break;
	 case HIDASP_BOOT_EXIT:
		exitMainloop = 1;
		break;
	 case HIDASP_BOOT_RWW:
		boot_rww_enable();
		break;
	 case HIDASP_SET_PAGE:
		cmd_page((MonCommand_t *)data);
		break;
	 case HIDASP_JMP:
		cmd_jmp((MonCommand_t *)data);
		break;
	 case HIDASP_PEEK:
		cmd_peek((MonCommand_t *)data);
		break;
	 case HIDASP_POKE:
		cmd_poke((MonCommand_t *)data);
		break;
	 default:
		break;
	}
}


#endif	//INCLUDE_MONITOR_CMD

