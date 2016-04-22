/* hidasp.c
 * original: binzume.net
 * modify: senshu , iruka
 * 2008-09-22 : for HIDaspx.
 * 2008-11-07 : for HIDaspx, hidmon, bootmon88
 */

#define HIDMON			1
#define DEBUG 		   	0		// for DEBUG

#define DEBUG_PKTDUMP  	0		// HID Reportパケットをダンプする.
#define DUMP_PRODUCT   	0		// 製造者,製品名を表示.
#define	JTAG_DUMP		0		// JTAGコマンドをHEXダンプ.

#define CHECK_COUNT		1		// 4: Connect時の Ping test の回数.
#define BUFF_SIZE	256

#include <windows.h>
#include <stdio.h>
#include "usbhid.h"
#include "monit.h"
#include "hidasp.h"
#include "firmware/hidcmd.h"
#include "portlist.h"

#ifndef __GNUC__
#pragma comment(lib, "setupapi.lib")
#endif

//
//	cmd.
//
enum {
	CMD_PING 	= HIDASP_TEST,
	CMD_SETPAGE = HIDASP_SET_PAGE,
	CMD_POKE = HIDASP_POKE,
	CMD_PEEK = HIDASP_PEEK,
};


//  obdev
static int MY_VID = 0x16c0;				/* 5824 in dec, stands for VOTI */
static int MY_PID = 0x05df;				/* 1503 in dec, obdev's free PID */

#if	HIDMON88
	#define	MY_Manufacturer	"AVRetc"
	#define	MY_Product		"bmon"
#elif (HIDASPX || HIDMON)
	#define	MY_Manufacturer	"YCIT"
	#define	MY_Product		"HIDaspx"
#else
	#define	MY_Manufacturer	"obdev.at"
	#define	MY_Product		"HIDBoot"
#endif

//	MY_Manufacturer,MY_Product のdefine を外すと、VID,PIDのみの照合になる.
//	どちらかをはずすと、その照合を省略するようになる.
static int found_hidaspx;
static int serial_check_counts;

//extern 
int hidmon_mode=1;


// HID API (from w2k DDK)
_HidD_GetAttributes 		HidD_GetAttributes;
_HidD_GetHidGuid 			HidD_GetHidGuid;
_HidD_GetPreparsedData 		HidD_GetPreparsedData;
_HidD_FreePreparsedData 	HidD_FreePreparsedData;
_HidP_GetCaps 				HidP_GetCaps;
_HidP_GetValueCaps 			HidP_GetValueCaps;
_HidD_GetFeature 			HidD_GetFeature;
_HidD_SetFeature 			HidD_SetFeature;
_HidD_GetManufacturerString	HidD_GetManufacturerString;
_HidD_GetProductString 		HidD_GetProductString;
_HidD_GetSerialNumberString HidD_GetSerialNumberString;	// add by senshu

HINSTANCE hHID_DLL = NULL;		// hid.dll handle
HANDLE hHID = NULL;				// USB-IO dev handle
HIDP_CAPS Caps;

static	int dev_id      = 0;	// ターゲットID: 0x55 もしくは 0x5a だけを許容.
static	int have_isp_cmd = 0;	// ISP制御の有無.

//----------------------------------------------------------------------------
//--------------------------    Tables    ------------------------------------
//----------------------------------------------------------------------------
//  HID Report のパケットはサイズ毎に 3種類用意されている.
#if HIDMON88
	#define	REPORT_ID1			1	// 8  REPORT_COUNT(6)
	#define	REPORT_ID2			3	// 40 REPORT_COUNT(38)
	#define	REPORT_ID3			2	// 32 REPORT_COUNT(30)

	#define	REPORT_LENGTH1		7	// REPORT_COUNT(6)
	#define	REPORT_LENGTH2		39	// 40 REPORT_COUNT(38)
	#define	REPORT_LENGTH3		132	// REPORT_COUNT(131)

	//	最大の長さをもつ HID ReportID,Length
	#define	REPORT_IDMAX		REPORT_ID2
	#define	REPORT_LENGTHMAX	REPORT_LENGTH2
#else
	#define	REPORT_ID1			1	// 8  REPORT_COUNT(6)
	#define	REPORT_ID2			2	// 32 REPORT_COUNT(30)
	#define	REPORT_ID3			3	// 40 REPORT_COUNT(38)

	#define	REPORT_LENGTH1		7	// 8  REPORT_COUNT(6)
	#define	REPORT_LENGTH2		31	// 32 REPORT_COUNT(30)
	#define	REPORT_LENGTH3		39	// 40 REPORT_COUNT(38)

	#define	PAGE_WRITE_LENGTH	32	// Page Writeでは32byte単位の転送を心掛ける.
									// Length5より7バイト少ない値である必要がある.

	//	最大の長さをもつ HID ReportID,Length
	#define	REPORT_IDMAX		REPORT_ID3
	#define	REPORT_LENGTHMAX	REPORT_LENGTH3
#endif


#if	DEBUG_PKTDUMP
static	void memdump(char *msg, char *buf, int len);
#endif
/*
 * wrapper for HidD_GetFeature / HidD_SetFeature.
 */
//----------------------------------------------------------------------------
/*
 *	HIDデバイスから HID Report を取得する.
 *	受け取ったバッファは先頭の1バイトに必ずReportIDが入っている.
 *
 *	id と Length の組はデバイス側で定義されたものでなければならない.
 *
 *	戻り値はHidD_GetFeatureの戻り値( 0 = 失敗 )
 *
 */
static int hidRead(HANDLE h, char *buf, int Length, int id)
{
	int rc;
	buf[0] = id;
	rc = HidD_GetFeature(h, buf, Length);
#if	DEBUG_PKTDUMP
	memdump("RD", buf, Length);
	printf("id=%d Length=%d rc=%d\n",id,Length,rc);
#endif
	return rc;
}

int	hidReadPoll(char *buf, int Length, int id)
{
	int rc;
	buf[0] = id;
	rc = HidD_GetFeature(hHID, buf, Length);
#if	DEBUG_PKTDUMP
	memdump("RD", buf, Length);
#endif
	return rc;
}


/*
 *	HIDデバイスに HID Report を送信する.
 *	送信バッファの先頭の1バイトにReportID を入れる処理は
 *	この関数内で行うので、先頭1バイトを予約しておくこと.
 *
 *	id と Length の組はデバイス側で定義されたものでなければならない.
 *
 *	戻り値はHidD_SetFeatureの戻り値( 0 = 失敗 )
 *
 */
static int hidWrite(HANDLE h, char *buf, int Length, int id)
{
	int rc;
	buf[0] = id;
	rc = HidD_SetFeature(h, buf, Length);
#if	DEBUG_PKTDUMP
	memdump("WR", buf, Length);
#endif
	return rc;
}

/*
 *	hidWrite()を使用して、デバイス側に buf[] データを len バイト送る.
 *	長さを自動判別して、ReportIDも自動選択する.
 *
 *	戻り値はHidD_SetFeatureの戻り値( 0 = 失敗 )
 *
 */
int	hidWriteBuffer(char *buf, int len)
{
	int report_id = 0;
	int length    = 0;

	if (len <= REPORT_LENGTH1) {
		length= REPORT_LENGTH1;report_id = REPORT_ID1;
	} else if (len <= REPORT_LENGTH2) {
		length= REPORT_LENGTH2;report_id = REPORT_ID2;
	} else if (len <= REPORT_LENGTH3) {
		length= REPORT_LENGTH3;report_id = REPORT_ID3;
	}

	if( report_id == 0) {
		// 適切な長さが選択できなかった.
		fprintf(stderr, "Error at hidWriteBuffer. len=%d\n",len);
#if DLL
		return 0;
#else
		exit(1);
		return 0;
#endif
	} else {
		return hidWrite(hHID, buf, length, report_id);
	}
}

/*
 *	hidRead()を使用して、デバイス側から buf[] データを len バイト取得する.
 *	長さを自動判別して、ReportIDも自動選択する.
 *
 *	戻り値はHidD_GetFeatureの戻り値( 0 = 失敗 )
 *
 */
int	hidReadBuffer(char *buf, int len)
{
	int report_id = 0;
	int length    = 0;

	if (len <= REPORT_LENGTH1) {
		length= REPORT_LENGTH1;report_id = REPORT_ID1;
	} else if (len <= REPORT_LENGTH2) {
		length= REPORT_LENGTH2;report_id = REPORT_ID2;
	} else if (len <= REPORT_LENGTH3) {
		length= REPORT_LENGTH3;report_id = REPORT_ID3;
	}

	if( report_id == 0) {
		// 適切な長さが選択できなかった.
		fprintf(stderr, "Error at hidWriteBuffer. len=%d\n",len);
#if DLL
		return 0;
#else
		exit(1);
		return 0;
#endif
	} else {
		return hidRead(hHID, buf, length, report_id);
	}
}
/*
 *	hidWrite()を使用して、デバイス側に4バイトの情報を送る.
 *	4バイトの内訳は cmd , arg1 , arg2 , arg 3 である.
 *  ReportIDはID1を使用する.
 *
 *	戻り値はHidD_SetFeatureの戻り値( 0 = 失敗 )
 *
 */
int hidCommand(int cmd,int arg1,int arg2,int arg3)
{
	unsigned char buf[BUFF_SIZE];

	memset(buf , 0, sizeof(buf) );

	buf[1] = cmd;
	buf[2] = arg1;
	buf[3] = arg2;
	buf[4] = arg3;

	return hidWrite(hHID, buf, REPORT_LENGTH1, REPORT_ID1);
}

//
//	mask が   0 の場合は、 addr に data0 を1バイト書き込み.
//	mask が 非0 の場合は、 addr に data0 と mask の論理積を書き込む.
//		但し、その場合は mask bitが 0 になっている部分に影響を与えないようにする.
//
//	例:	PORTB の 8bit に dataを書き込む.
//		hidPokeMem( PORTB , data , 0 );
//	例:	PORTB の bit2 だけを on
//		hidPokeMem( PORTB , 1<<2 , 1<<2 );
//	例:	PORTB の bit2 だけを off
//		hidPokeMem( PORTB ,    0 , 1<<2 );
//
int hidPokeMem(int addr,int data0,int mask)
{
	unsigned char buf[BUFF_SIZE];
	memset(buf , 0, sizeof(buf) );

	buf[1] = HIDASP_POKE;
	buf[2] = 0;
	buf[3] = addr;
	buf[4] =(addr>>8);
	if( mask ) {
		buf[5] = data0 & mask;
		buf[6] = ~mask;
	}else{
		buf[5] = data0;
		buf[6] = 0;
	}
	return hidWrite(hHID, buf, REPORT_LENGTH1, REPORT_ID1);
}

int hidPeekMem(int addr)
{
	unsigned char buf[BUFF_SIZE];
	memset(buf , 0, sizeof(buf) );

	buf[1] = HIDASP_PEEK;
	buf[2] = 1;
	buf[3] = addr;
	buf[4] =(addr>>8);

	hidWrite(hHID, buf, REPORT_LENGTH1, REPORT_ID1);
	hidReadBuffer( buf, REPORT_LENGTH1 );
	return buf[1];
}

#if HIDMON88
static void hidSetDevCaps(void)
{
	hidCommand(HIDASP_GET_CAPS,0,0,0);	// bootloadHIDが取得すべき情報をバッファに置く.
}
#endif

#define	USICR			0x2d	//
#define	DDRB			0x37	// PB4=RST PB3=LED
#define	DDRB_WR_MASK	0xf0	// 制御可能bit = 1111_0000
#define	PORTB			0x38	// PB4=RST PB3=LED
#define	PORTB_WR_MASK	0		// 0 の場合はMASK演算は省略され、直書き.

#define HIDASP_RST		0x10	// RST bit

/*
 *	LEDの制御. (hidasp.h)
#define HIDASP_RST_H_GREEN	0x18	// RST解除,LED OFF
#define HIDASP_RST_L_BOTH	0x00	// RST実行,LED ON
#define HIDASP_SCK_PULSE 	0x80	// RST-L SLK-pulse ,LED ON	@@kuga
 */


static void hidSetStatus(int ledstat)
{
	static int once = 0;
	int ddrb;
	if (have_isp_cmd) {
		hidCommand(HIDASP_SET_STATUS,0,ledstat,0);	// cmd,portd(&0000_0011),portb(&0001_1111),0
	}else{
		if (once == 0 && !hidmon_mode) {
//			fprintf(stderr, "Warnning: Please update HIDaspx firmware.\n");
			fprintf(stderr, "Warnning: Please check HIDaspx mode.\n");
			once++;
		}

		if (hidmon_mode) {
			ddrb = 0xff;			// DDRB = 0xff : 1が出力ピン ただしbit3-0は影響しない.
			hidPokeMem(PORTB,ledstat,PORTB_WR_MASK);
			hidPokeMem(DDRB ,ddrb   ,DDRB_WR_MASK);
		} else if(ledstat & HIDASP_RST) {	// RST解除.
			ddrb = 0x10;			// PORTB 全入力.
			hidPokeMem(USICR,0      ,0);
			hidPokeMem(PORTB,ledstat,PORTB_WR_MASK);
			hidPokeMem(DDRB ,ddrb   ,DDRB_WR_MASK);
		}else{
			// RSTをLにする.
			ddrb = 0xd0;			// DDRB 1101_1100 : 1が出力ピン ただしbit3-0は影響しない.
			hidPokeMem(USICR,0      ,0);
			hidPokeMem(DDRB ,ddrb   ,DDRB_WR_MASK);
			hidPokeMem(PORTB,ledstat,PORTB_WR_MASK);
			hidPokeMem(PORTB,ledstat|HIDASP_RST,PORTB_WR_MASK);	// RSTはまだHi
			hidPokeMem(PORTB,ledstat,PORTB_WR_MASK);	// RSTはあとでLに.
			hidPokeMem(USICR,0x1a   ,0);
		}
	}
}

////////////////////////////////////////////////////////////////////////
//             hid.dll をロード
static int LoadHidDLL()
{
	hHID_DLL = LoadLibrary("hid.dll");
	if (!hHID_DLL) {
#if 1
		fprintf(stderr, "Error at Load 'hid.dll'\n");
#else
		MessageBox(NULL, "Error at Load 'hid.dll'", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetAttributes = (_HidD_GetAttributes)GetProcAddress(hHID_DLL, "HidD_GetAttributes");
	if (!HidD_GetAttributes) {
#if 1
		fprintf(stderr, "Error at HidD_GetAttributes\n");
#else
		MessageBox(NULL, "Error at HidD_GetAttributes", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetHidGuid = (_HidD_GetHidGuid)GetProcAddress(hHID_DLL, "HidD_GetHidGuid");
	if (!HidD_GetHidGuid) {
#if 1
		fprintf(stderr, "Error at HidD_GetHidGuid\n");
#else
		MessageBox(NULL, "Error at HidD_GetHidGuid", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetPreparsedData =	(_HidD_GetPreparsedData)GetProcAddress(hHID_DLL, "HidD_GetPreparsedData");
	HidD_FreePreparsedData = (_HidD_FreePreparsedData)GetProcAddress(hHID_DLL, "HidD_FreePreparsedData");
	HidP_GetCaps = (_HidP_GetCaps)GetProcAddress(hHID_DLL, "HidP_GetCaps");
	HidP_GetValueCaps =	(_HidP_GetValueCaps) GetProcAddress(hHID_DLL, "HidP_GetValueCaps");

//
	HidD_GetFeature = (_HidD_GetFeature)GetProcAddress(hHID_DLL, "HidD_GetFeature");
	HidD_SetFeature = (_HidD_SetFeature)GetProcAddress(hHID_DLL, "HidD_SetFeature");
	HidD_GetManufacturerString = (_HidD_GetManufacturerString)GetProcAddress(hHID_DLL, "HidD_GetManufacturerString");
	HidD_GetProductString = (_HidD_GetProductString)GetProcAddress(hHID_DLL, "HidD_GetProductString");
	HidD_GetSerialNumberString = (_HidD_GetSerialNumberString)GetProcAddress(hHID_DLL, "HidD_GetSerialNumberString");

#if	DEBUG
	printf("_HidD_GetFeature= %x\n", (int) HidD_GetFeature);
	printf("_HidD_SetFeature= %x\n", (int) HidD_SetFeature);
#endif
	return 1;
}

////////////////////////////////////////////////////////////////////////
// ディバイスの情報を取得
static void GetDevCaps()
{
	PHIDP_PREPARSED_DATA PreparsedData;
	HIDP_VALUE_CAPS *VCaps;
	char buf[1024];

	VCaps = (HIDP_VALUE_CAPS *) (&buf);

	HidD_GetPreparsedData(hHID, &PreparsedData);
	HidP_GetCaps(PreparsedData, &Caps);
	HidP_GetValueCaps(HidP_Input, VCaps, &Caps.NumberInputValueCaps,  PreparsedData);
	HidD_FreePreparsedData(PreparsedData);

#if DEBUG
	fprintf(stderr, " Caps.OutputReportByteLength = %d\n", Caps.OutputReportByteLength);
	fprintf(stderr, " Caps.InputReportByteLength = %d\n", Caps.InputReportByteLength);
#endif


#if	REPORT_LENGTH_OVERRIDE
	//複数のREPORT_COUNTが存在するときは下記Capsが 0 のままなので、上書きする.
	Caps.OutputReportByteLength = REPORT_LENGTH_OVERRIDE;
	Caps.InputReportByteLength = REPORT_LENGTH_OVERRIDE;
#endif
}

//----------------------------------------------------------------------------
/*
 * 	unicode を ASCIIに変換.
 */
static char *uni_to_string(char *t, unsigned short *u)
{
	char *buf = t;
	int c;
	// short を char 配列に入れなおす.
	while (1) {
		c = *u++;
		if (c == 0)
			break;
		if (c & 0xff00)
			c = '?';
		*t++ = c;
	}

	*t = 0;
	return buf;
}

//----------------------------------------------------------------------------
/*  Manufacturer & Product name check.
 *  名前チェック : 成功=1  失敗=0 読み取り不能=(-1)
 */
static int check_product_string(HANDLE handle, const char *serial, int list_mode)
{
	static int first = 1;
	int i;
	unsigned short unicode[BUFF_SIZE*2];
	char string1[BUFF_SIZE];
	char string2[BUFF_SIZE];
	char string3[BUFF_SIZE];
	char tmp[2][BUFF_SIZE];

	Sleep(20);
	if (!HidD_GetManufacturerString(handle, unicode, sizeof(unicode))) {
		return -1;
	}
	uni_to_string(string1, unicode);

	Sleep(20);
	if (!HidD_GetProductString(handle, unicode, sizeof(unicode))) {
		return -1;
	}
	uni_to_string(string2, unicode);

	// シリアル番号のチェックを厳密化 (2010/02/12 13:24:08)
	serial_check_counts = 0;
	if (serial[0]=='*') {
		//extern 
		int  f_auto_retry=2;		/* for HIDaspx (Auto detect, Retry MAX) */

		for (i=0; i<f_auto_retry; i++) {
			serial_check_counts++;
			Sleep(20);
			if (!HidD_GetSerialNumberString(handle, unicode, sizeof(unicode))) {
				return -1;
			}
			uni_to_string(tmp[i%2], unicode);
			if ((i>0) && ((i%2) == 1)) {
				if (strcmp(tmp[0], tmp[1])==0) {
					strcpy(string3, tmp[0]);	// OK
				} else {
					return -1;
				}
			}
		}
		if (list_mode) {
			if (first) {
				fprintf(stderr,	"VID=%04x, PID=%04x, [%6s], [%7s], serial=[%s] (*)\n",
						MY_VID, MY_PID, string1,  string2, string3);
				first = 0;
			} else {
				fprintf(stderr,	"VID=%04x, PID=%04x, [%6s], [%7s], serial=[%s]\n",
						MY_VID, MY_PID, string1,  string2, string3);
			}
		}
	} else {
		Sleep(20);
		if (!HidD_GetSerialNumberString(handle, unicode, sizeof(unicode))) {
			return -1;
		}
		uni_to_string(string3, unicode);
	}


#ifdef	MY_Manufacturer
	if (strcmp(string1, MY_Manufacturer) != 0)
		return 0;	// 一致せず
#endif

#ifdef	MY_Product
	if (strcmp(string2, MY_Product) != 0)
		return 0;	// 一致せず
#endif

	found_hidaspx++;

	/* Check serial number */
	if (found_hidaspx) {
		if (strcmp(string3, serial) == 0)
			return 1;		//合致した.
		else if (strcmp(serial ,"*") == 0)
			return 1;		//合致した.
	}

	return 0;	// 一致せず

}

////////////////////////////////////////////////////////////////////////
// HIDディバイス一覧からUSB-IOを検索
static int OpenTheHid(const char *serial, int list_mode)
{
	int f, i, rc;
	ULONG Needed, l;
	GUID HidGuid;
	HDEVINFO DeviceInfoSet;
	HIDD_ATTRIBUTES DeviceAttributes;
	SP_DEVICE_INTERFACE_DATA DevData;
	PSP_INTERFACE_DEVICE_DETAIL_DATA DevDetail;
	//SP_DEVICE_INTERFACE_DETAIL_DATA *MyDeviceInterfaceDetailData;

	DeviceAttributes.Size = sizeof(HIDD_ATTRIBUTES);
	DevData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	HidD_GetHidGuid(&HidGuid);
#if 1							/* For vista */
	DeviceInfoSet =
		SetupDiGetClassDevs(&HidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
#else
	DeviceInfoSet =
		SetupDiGetClassDevs(&HidGuid, "", NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
#endif

	f = i = 0;
	while ((rc = SetupDiEnumDeviceInterfaces(DeviceInfoSet, 0, &HidGuid, i++, &DevData))!=0) {
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DevData, NULL, 0, &Needed, 0);
		l = Needed;
		DevDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA *) GlobalAlloc(GPTR, l + 4);
		DevDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DevData, DevDetail, l, &Needed, 0);

		hHID = CreateFile(DevDetail->DevicePath,
						  GENERIC_READ | GENERIC_WRITE,
						  FILE_SHARE_READ | FILE_SHARE_WRITE,
						  NULL, OPEN_EXISTING,
//                        FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING,
						  0, NULL);

		GlobalFree(DevDetail);

		if (hHID == INVALID_HANDLE_VALUE)	// Can't open a device
			continue;
		HidD_GetAttributes(hHID, &DeviceAttributes);

		// HIDaspかどうか調べる.
		if (DeviceAttributes.VendorID == MY_VID && DeviceAttributes.ProductID == MY_PID) {
			int rc;
			rc = check_product_string(hHID, serial, list_mode);
			if ( rc == 1) {
				f++;				// 見つかった
				// 複数個をリストする (2010/02/12 13:19:38)
				if (list_mode == 0) {
					break;			// 自動認識時は、最初の候補を利用する
				}
			}
		} else {
			// 違ったら閉じる
			CloseHandle(hHID);
			hHID = NULL;
		}
	}
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
	return f;
}

#if	DEBUG_PKTDUMP
//----------------------------------------------------------------------------
//  メモリーダンプ.
void memdump(char *msg, char *buf, int len)
{
	int j;
	fprintf(stderr, "%s", msg);
	for (j = 0; j < len; j++) {
		fprintf(stderr, " %02x", buf[j] & 0xff);
		if((j & 0x1f)== 31)
			fprintf(stderr, "\n +");
	}
	fprintf(stderr, "\n");
}
#endif


int hidasp_list(char * exe_name)
{
	int r, rc;

	LoadHidDLL();
	found_hidaspx = 0;
	r = OpenTheHid("*", 1);
	if (r == 0) {
		rc = 1;
	} else {
		rc = 0;
	}
	if (found_hidaspx==0) {
		if (serial_check_counts != 0) {
			fprintf(stderr, "%s: HIDaspx(VID=%04x, PID=%04x) serial check error (%d).\n",
				exe_name, MY_VID, MY_PID, serial_check_counts);
		} else {
			fprintf(stderr, "%s: HIDaspx(VID=%04x, PID=%04x) isn't found.\n",
				exe_name, MY_VID, MY_PID);
		}
	}
	if (hHID_DLL) {
		FreeLibrary(hHID_DLL);
	}
	return rc;
}

//----------------------------------------------------------------------------
//  初期化.
//----------------------------------------------------------------------------
void chg_vid_pid(int type)
{
	if (type) {	/* NON-free (pro mode) */
		MY_VID = 0x20A0;				/* VID */
		MY_PID = 0x410e;				/* PID */
	} else {
		MY_VID = 0x16c0;				/* 5824 in dec, stands for VOTI */
		MY_PID = 0x05df;				/* 1503 in dec, obdev's free PID */
	}
}

int hidasp_init(int type, const char *serial)
{
	unsigned char rd_data[BUFF_SIZE];
	int i, r, result;

	result = 0;

	chg_vid_pid(type);
	LoadHidDLL();
	if (OpenTheHid(serial, 0) == 0) {
#if DEBUG
		fprintf(stderr, "ERROR: fail to OpenTheHid(%s)\n", serial);
#endif
		return HIDASP_MODE_ERROR;
	}

	GetDevCaps();
//	Sleep(100);

#if DEBUG
	fprintf(stderr, "HIDaspx Connection check!\n");
#endif

	for (i = 0; i < CHECK_COUNT; i++) {
		hidCommand(HIDASP_TEST,(i),0,0);	// Connection test
		r = hidRead(hHID, rd_data ,REPORT_LENGTH1, REPORT_ID1);
#if DEBUG
		fprintf(stderr, "HIDasp Ping(%2d) = %d\n", i, rd_data[1]);
#endif
		if (r == 0) {
			fprintf(stderr, "Error: fail to Read().\n");
			return HIDASP_MODE_ERROR;
		}
		dev_id = rd_data[1];
		if((dev_id != DEV_ID_FUSION)
		 &&(dev_id != DEV_ID_STD)
		 &&(dev_id != DEV_ID_MEGA88)
		 &&(dev_id != DEV_ID_MEGA88_USERMODE)) {
			fprintf(stderr, "Error: fail to ping test. (id = %x)\n",dev_id);
			return HIDASP_MODE_ERROR;
		}

		if (hidmon_mode) {
			fprintf(stderr, "HIDASP:TARGET DEV_ID=%x\n",dev_id);
		}

		if (rd_data[2] != i) {
			fprintf(stderr, "Error: fail to ping test. %d != %d\n", rd_data[2] , i);
			return HIDASP_MODE_ERROR;
		}
	}
	if (!hidmon_mode) {
		hidCommand(HIDASP_SET_STATUS,0,HIDASP_RST_H_GREEN,0);	// RESET HIGH
	}
#if	HIDMON88
		hidCommand(HIDASP_BOOT_RWW,0,0,0);	// Read-While-Write-Section Read Enable.
#endif
	r = hidRead(hHID, rd_data ,REPORT_LENGTH1, REPORT_ID1);
	if( rd_data[1] == 0xaa ) {				// ISPコマンド(isp_enable)が正常動作した.
		have_isp_cmd = HIDASP_ISP_MODE;		// ISP制御OK.
		result |= HIDASP_ISP_MODE;
	} else if( rd_data[1] == DEV_ID_FUSION ) {
		result |= HIDASP_FUSION_OK;			// NEW firmware
	} else if( rd_data[1] == DEV_ID_STD ) {
		result &= ~HIDASP_FUSION_OK;		// OLD firmware
	} else if( rd_data[1] == DEV_ID_MEGA88 ) {		// USB-IO mode
		result |= HIDASP_USB_IO_MODE;		// ISP制御NG.
	} else if( rd_data[1] == DEV_ID_MEGA88_USERMODE ) {		// USB-IO mode
		result |= HIDASP_USB_IO_MODE;		// ISP制御NG.
	}
#if DEBUG
	if (result & HIDASP_ISP_MODE) {
		fprintf(stderr, "[ISP CMD] ");
	}
	if (result & HIDASP_FUSION_OK) {
		fprintf(stderr, "[FUSION] ");
	}
	if (result & HIDASP_USB_IO_MODE) {
		fprintf(stderr, "[USB-IO mode] ");
	}
	if (result == 0) {
		fprintf(stderr, "ISP CMD Not support.\n");
	} else {
		fprintf(stderr, "OK.\n");
	}
#endif
	return result;
}

//----------------------------------------------------------------------------
//  終了.
//----------------------------------------------------------------------------
void hidasp_close()
{
#define HIDASP_NOP 			  0	//これは ISPのコマンドと思われる?

	if (hHID) {
		unsigned char buf[BUFF_SIZE];

		buf[0] = 0x00;
		buf[1] = HIDASP_NOP;
		buf[2] = 0x00;
		buf[3] = 0x00;
		hidasp_cmd(buf, NULL);					// AVOID BUG!
		if (!hidmon_mode) {
			hidSetStatus(HIDASP_RST_H_GREEN);		// RESET HIGH
		}
#if	HIDMON88
		hidSetDevCaps();
#endif
		CloseHandle(hHID);
	}
	if (hHID_DLL) {
		FreeLibrary(hHID_DLL);
	}
	hHID = NULL;
	hHID_DLL = NULL;
}

//----------------------------------------------------------------------------
//  ISPコマンド発行.
//----------------------------------------------------------------------------
int hidasp_cmd(const unsigned char cmd[4], unsigned char res[4])
{
	char buf[128];
	int r;

	memset(buf , 0, sizeof(buf) );
	if (res != NULL) {
		buf[1] = HIDASP_CMD_TX_1;
	} else {
		buf[1] = HIDASP_CMD_TX;
	}
	memcpy(buf + 2, cmd, 4);

	r = hidWrite(hHID, buf, REPORT_LENGTH1 , REPORT_ID1);
#if DEBUG
	fprintf(stderr, "hidasp_cmd %02X, cmd: %02X %02X %02X %02X ",
		buf[1], cmd[0], cmd[1], cmd[2], cmd[3]);
#endif

	if (res != NULL) {
		r = hidRead(hHID, buf, REPORT_LENGTH1 , REPORT_ID1);
		memcpy(res, buf + 1, 4);
#if DEBUG
		fprintf(stderr, " --> res: %02X %02X %02X %02X\n",
				res[0], res[1], res[2], res[3]);
#endif
	}

	return 1;
}


//----------------------------------------------------------------------------
//  ターゲットマイコンをプログラムモードに移行する.
//----------------------------------------------------------------------------
int hidasp_program_enable(int delay)
{
	unsigned char buf[BUFF_SIZE];
	unsigned char res[4];
	int i, rc;
	int tried;	//AVRSP と同様のプロトコルを採用

	rc = 1;
	hidSetStatus(HIDASP_RST_H_GREEN);			// RESET HIGH
	hidSetStatus(HIDASP_RST_L_BOTH);			// RESET LOW (H PULSE)
	hidCommand(HIDASP_SET_DELAY,delay,0,0);		// SET_DELAY
	Sleep(30);				// 30

	for(tried = 0; tried < 3; tried++) {
		for(i = 0; i < 32; i++) {

			buf[0] = 0xAC;
			buf[1] = 0x53;
			buf[2] = 0x00;
			buf[3] = 0x00;
			hidasp_cmd(buf, res);

			if (res[2] == 0x53) {
				rc = 0;					// AVRマイコンと同期を確認
				goto hidasp_program_enable_exit;
			}
			if (tried < 2) {			// 2回までは通常の同期方法
				break;
			}
			// AT90S用の同期方法で同期を取る
			hidSetStatus(HIDASP_SCK_PULSE);			// RESET LOW SCK H PULSE shift scan point
		}
	}

	hidasp_program_enable_exit:
#if DEBUG
	if (rc == 0) {
		fprintf(stderr, "hidasp_program_enable() == OK\n");
	} else  {
		fprintf(stderr, "hidasp_program_enable() == NG\n");
	}
#endif
	return rc;
}




//----------------------------------------------------------------------------
int		ddrb  ;
int		pinb  ;
int		portb ;
/*********************************************************************
 *	初期化
 *********************************************************************
 */
int UsbInit(int verbose,int enable_bulk, char *serial)
{
	int type = 0;
	if(	hidasp_init(type,serial) & HIDASP_MODE_ERROR) {
		if (verbose) {
	    	fprintf(stderr, "error: [%s] device not found!\n", serial);
    	}
    	return 0;
	} else {
		ddrb  = portAddress("ddrb");
		pinb  = portAddress("pinb");
		portb = portAddress("portb");
		return 1;	// OK.
	}
}

/*********************************************************************
 *	終了
 *********************************************************************
 */
int UsbExit(void)
{
	hidasp_close();
	return 0;
}

/*********************************************************************
 *	AVRデバイスに処理コマンドを送って、必要なら結果を受け取る.
 *********************************************************************
 *	cmdBuf   *cmd : 処理コマンド
 *	uchar    *buf : 結果を受け取る領域.
 *  int reply_len : 結果の必要サイズ. (0の場合は結果を受け取らない)
 *戻り値
 *		0	: 失敗
 *	   !0   : 成功
 */
static	int QueryAVR(cmdBuf *cmd,uchar *buf,int size,int reply_len)
{
	int rc = 0;
	int report_id = cmd->report_id;
	char *s;

	rc = hidWriteBuffer((char*)cmd , size);
	if(rc == 0) {
		printf("hidWrite error\n");
		exit(1);
	}
	if(reply_len) {
		rc = hidReadBuffer((char *)cmd , reply_len+1 );
		if(rc == 0) {
			//if(report_id != 4) 
			{
				printf("hidRead error %d\n",report_id);
				exit(1);
			}
		}
		s = (char*) cmd;
		memcpy(buf, s+1 ,reply_len);
	}
    return rc;
}
/*********************************************************************
 *	ターゲット側のメモリー内容を取得する
 *********************************************************************
 *	int            adr :ターゲット側のメモリーアドレス
 *	int          arena :ターゲット側のメモリー種別(現在未使用)
 *	int           size :読み取りサイズ.
 *	unsigned char *buf :受け取りバッファ.
 *	注意: HID Report Length - 1 より長いサイズは要求してはならない.
 */
int dumpmem(int adr,int arena,int size,unsigned char *buf)
{
	cmdBuf cmd;

	memset(&cmd,0,sizeof(cmdBuf));
	cmd.cmd   = CMD_PEEK ;	//| arena;
	cmd.size  = size;
	cmd.adr[0]= adr;
	cmd.adr[1]= adr>>8;

	if( QueryAVR(&cmd,buf,REPORT_LENGTH1,1+size) == 0) return 0;	//失敗.
	return size;
}

/*********************************************************************
 *	ターゲット側のメモリー内容(1byte)を取得する
 *********************************************************************
 */
int peekmem(int adr,int arena)
{
	uchar buf[64];
	dumpmem(adr,arena,1,buf);
	return buf[0];
}

/*********************************************************************
 *	ターゲット側のメモリー内容(1バイト)を書き換える.
 *********************************************************************
 *	int            adr :ターゲット側のメモリーアドレス
 *	int          arena :ターゲット側のメモリー種別(現在未使用)
 *	int          data0 :書き込みデータ.      (OR)
 *	int          data1 :書き込みビットマスク.(AND)
 *注意:
 *	ファーム側の書き込みアルゴリズムは以下のようになっているので注意.

	if(data1) {	//マスク付の書き込み.
		*adr = (*adr & data1) | data0;
	}else{			//べた書き込み.
		*adr = data0;
	}

 */
int	pokemem(int adr,int arena,int data0,int data1)
{
    cmdBuf cmd;
	char buf[16];	// dummy

	memset(&cmd,0,sizeof(cmdBuf));
	cmd.cmd   = CMD_POKE ;	//| arena;
	cmd.size  = 1;
	cmd.adr[0]= adr;
	cmd.adr[1]= adr>>8;
	cmd.data[0] = data0;
	cmd.data[1] = data1;
	return QueryAVR(&cmd,buf,REPORT_LENGTH1,0);
}

/*********************************************************************
 *	JTAGコマンド発行
 *********************************************************************
 */
static	uchar j_buf[128];
static	int	  j_index=0;
static	int	  j_anchor=0;

//#define	MAX_JCMD_SIZE	28
#define	MAX_JCMD_SIZE	(REPORT_LENGTH3-1)	//

/*********************************************************************
 *	JTAGコマンド発行
 *********************************************************************
 */
int jcmd_write_nb(uchar *stream,int size,uchar *result)
{
    cmdBuf cmd;
	char *t;
	int  rsize=0;
	if(result!=NULL) rsize = size;

#if	JTAG_DUMP
	{int i;
	printf("JTAG:");
	for(i=0;i<size;i++) {
		printf(" %02x",stream[i]);
	}
	printf("\n");
	}
#endif

	t = (char*)&cmd;
	memset(t,0,sizeof(cmdBuf));
	cmd.cmd   = HIDASP_JTAG_WRITE;
	memcpy(t+2,stream,size);

	return QueryAVR(&cmd,result,2+size,rsize);
}

/*********************************************************************
 *	
 *********************************************************************
 */
int jcmd_write_flash()
{
	if(	j_index ) {
		jcmd_write_nb(j_buf,j_index,NULL);
		j_index=0;
	}
	return 0;
}

/*********************************************************************
 *	JTAGコマンドを連結する処理.
 *********************************************************************
 */
int jcmd_add(uchar *stream,int size)
{
	int c1,c2;
	// 初回連結: 初期データが空の場合.
	if(	j_index == 0) {
		j_anchor = 0;				//直前のコントロールワードの場所.
		memcpy(j_buf,stream,size);	//丸コピーする.
		j_index = size;
		return size;
	}
	// 初回でない連結: 初期データが存在する場合.
	c1 = j_buf[j_anchor];
	c2 = stream[0];
	if((c1 & 0x80) && (c2 & 0x80)) {	//両方ともbitbangストリームの場合は１つのストリームにしてしまう.
		memcpy(j_buf+j_index-1,stream+1,size-1);	// 初期データのNULターミネート位置にbitbangをコピー.
		j_buf[j_anchor] = 0x80 | ((c1&0x7f)+(c2&0x7f));	//長さを書き直す.
		j_index += size-2;							// 接続部分のNUL文字とbitbang制御コードが１つづつ減るので-2
		// anchorの位置は変わらない.
		return j_index;
	}
	
	// 初回でない連結: bitbang同士で無い場合.
	{
		j_anchor = j_index-1;
		memcpy(j_buf+j_anchor,stream,size);		// 初期データのNULターミネート位置にコピー.
		j_index += size-1;						// 接続部分のNUL文字が１つ減るので-1
		return j_index;
	}
}

/*********************************************************************
 *	
 *********************************************************************
 */
int jcmd_write(uchar *stream,int size,uchar *result)
{
	//読み書きを伴う場合はバッファ不可.
	if(result) {
		jcmd_write_flash();
		return jcmd_write_nb(stream,size,result);
	}

	// 連結後のサイズがMAX_JCMD_SIZEを越えるのが分かっている場合は
	//   いったん吐き出す.
	if( (j_index+size)>=MAX_JCMD_SIZE ) {
		jcmd_write_flash();
	}

	// 連結を実行する.
	jcmd_add(stream,size);
	return j_index;
}

/*********************************************************************
 *	
 *********************************************************************
 */
int get_portb(void)
{
	jcmd_write_flash();
	return peekmem(pinb,AREA_RAM);
}
/*********************************************************************
 *	
 *********************************************************************
 */
void set_portb(int d)
{
	pokemem(portb,AREA_RAM,d,0);
}
/*********************************************************************
 *	
 *********************************************************************
 */
void set_ddrb(int d)
{
	pokemem(ddrb,AREA_RAM,d,0);
}

/*********************************************************************
 *	
 *********************************************************************
 */
