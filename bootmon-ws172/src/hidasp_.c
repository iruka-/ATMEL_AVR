/* hidasp.c
 * original: binzume.net
 * modify: senshu , iruka
 * 2008-09-22 : for HIDaspx.
 */

#define DEBUG 		   	0		// for DEBUG

#define DEBUG_PKTDUMP  	0		// HID Reportパケットをダンプする.
#define DUMP_PRODUCT   	0		// 製造者,製品名を表示.

#define CHECK_COUNT		2		// 4: Connect時の Ping test の回数.

#include <windows.h>
#include <stdio.h>
#include "usbhid.h"
#include "hidasp.h"
#include "avrspx.h"

#include "../firmware/hidcmd.h"

#ifndef __GNUC__
#pragma comment(lib, "setupapi.lib")
#endif

//  obdev
#define MY_VID 0x16c0				/* 5824 in dec, stands for VOTI */
#define MY_PID 0x05df				/* 1503 in dec, obdev's free PID */

#if	1
#define	MY_Manufacturer	"AVRetc"
#define	MY_Product		"bmon"
#else
#define	MY_Manufacturer	"obdev.at"
#define	MY_Product		"HIDBoot"
#endif
//	MY_Manufacturer,MY_Product のdefine を外すと、VID,PIDのみの照合になる.
//	どちらかをはずすと、その照合を省略するようになる.

static int found_hidaspx;

extern int hidmon_mode;


// HID API (from w2k DDK)
_HidD_GetAttributes HidD_GetAttributes;
_HidD_GetHidGuid HidD_GetHidGuid;
_HidD_GetPreparsedData HidD_GetPreparsedData;
_HidD_FreePreparsedData HidD_FreePreparsedData;
_HidP_GetCaps HidP_GetCaps;
_HidP_GetValueCaps HidP_GetValueCaps;
_HidD_GetFeature HidD_GetFeature;
_HidD_SetFeature HidD_SetFeature;
_HidD_GetManufacturerString HidD_GetManufacturerString;
_HidD_GetProductString HidD_GetProductString;

HINSTANCE hHID_DLL = NULL;		// hid.dll handle
HANDLE hHID = NULL;				// USB-IO dev handle
HIDP_CAPS Caps;

static	int dev_id      = 0;	// ターゲットID: 0x55 / 0x5a / 0x88 を許容.
static	int have_ledcmd = 0;	// LED制御の有無.

//----------------------------------------------------------------------------
//--------------------------    Tables    ------------------------------------
//----------------------------------------------------------------------------
//  HID Report のパケットはサイズ毎に ３種類用意されている.
#define	REPORT_ID1			1	// 8  REPORT_COUNT(6)
#define	REPORT_ID2			3	// 40 REPORT_COUNT(38)
#define	REPORT_ID3			2	// 32 REPORT_COUNT(30)

#define	REPORT_LENGTH1		7	// REPORT_COUNT(6)
#define	REPORT_LENGTH2		39	// 40 REPORT_COUNT(38)
#define	REPORT_LENGTH3		132	// REPORT_COUNT(131)

//	最大の長さをもつ HID ReportID,Length
#define	REPORT_IDMAX		REPORT_ID2
#define	REPORT_LENGTHMAX	REPORT_LENGTH2


#if	DEBUG_PKTDUMP
static	void memdump(char *msg, char *buf, int len);
#endif
/*
 * wrapper for HidD_GetFeature / HidD_SetFeature.
 */
//----------------------------------------------------------------------------
/*
 *	HIDデバイスから HID Report を取得する.
 *	受け取ったバッファは先頭の１バイトに必ずReportIDが入っている.
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
 *	HIDデバイスに HID Report を送信するする.
 *	送信バッファの先頭の１バイトにReportID を入れる処理は
 *	この関数内で行うので、先頭１バイトを予約しておくこと.
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
		exit(1);
		return 0;
	}

	return hidWrite(hHID, buf, length, report_id);
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
		exit(1);
		return 0;
	}

	return hidRead(hHID, buf, length, report_id);
}
/*
 *	hidWrite()を使用して、デバイス側に４バイトの情報を送る.
 *	４バイトの内訳は cmd , arg1 , arg2 , arg 3 である.
 *  ReportIDはID1を使用する.
 *
 *	戻り値はHidD_SetFeatureの戻り値( 0 = 失敗 )
 *
 */
int hidCommand(int cmd,int arg1,int arg2,int arg3)
{
	unsigned char buf[256];

	memset(buf , 0, sizeof(buf) );

	buf[1] = cmd;
	buf[2] = arg1;
	buf[3] = arg2;
	buf[4] = arg3;

	return hidWrite(hHID, buf, REPORT_LENGTH1, REPORT_ID1);
}

//
//	mask　が   0 の場合は、 addr に data0 を１バイト書き込み.
//	mask　が 非0 の場合は、 addr に data0 と mask の論理積を書き込む.
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
	unsigned char buf[256];
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
	unsigned char buf[256];
	memset(buf , 0, sizeof(buf) );

	buf[1] = HIDASP_PEEK;
	buf[2] = 1;
	buf[3] = addr;
	buf[4] =(addr>>8);

	hidWrite(hHID, buf, REPORT_LENGTH1, REPORT_ID1);
	hidReadBuffer( buf, REPORT_LENGTH1 );
	return buf[1];
}
static void hidSetDevCaps(void)
{
	hidCommand(HIDASP_GET_CAPS,0,0,0);	// bootloadHIDが取得すべき情報をバッファに置く.
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
	HidD_GetAttributes =
		(_HidD_GetAttributes) GetProcAddress(hHID_DLL,
											 "HidD_GetAttributes");
	if (!HidD_GetAttributes) {
#if 1
		fprintf(stderr, "Error at HidD_GetAttributes\n");
#else
		MessageBox(NULL, "Error at HidD_GetAttributes", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetHidGuid =
		(_HidD_GetHidGuid) GetProcAddress(hHID_DLL, "HidD_GetHidGuid");
	if (!HidD_GetHidGuid) {
#if 1
		fprintf(stderr, "Error at HidD_GetHidGuid\n");
#else
		MessageBox(NULL, "Error at HidD_GetHidGuid", "ERR", MB_OK);
#endif
		return 0;
	}
	HidD_GetPreparsedData =
		(_HidD_GetPreparsedData) GetProcAddress(hHID_DLL,
												"HidD_GetPreparsedData");
	HidD_FreePreparsedData =
		(_HidD_FreePreparsedData) GetProcAddress(hHID_DLL,
												 "HidD_FreePreparsedData");
	HidP_GetCaps =
		(_HidP_GetCaps) GetProcAddress(hHID_DLL, "HidP_GetCaps");
	HidP_GetValueCaps =
		(_HidP_GetValueCaps) GetProcAddress(hHID_DLL, "HidP_GetValueCaps");

//
	HidD_GetFeature =
		(_HidD_GetFeature) GetProcAddress(hHID_DLL, "HidD_GetFeature");
	HidD_SetFeature =
		(_HidD_SetFeature) GetProcAddress(hHID_DLL, "HidD_SetFeature");
	HidD_GetManufacturerString = (_HidD_GetManufacturerString)
		GetProcAddress(hHID_DLL, "HidD_GetManufacturerString");
	HidD_GetProductString = (_HidD_GetProductString)
		GetProcAddress(hHID_DLL, "HidD_GetProductString");

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
	HidP_GetValueCaps(HidP_Input, VCaps, &Caps.NumberInputValueCaps,
					  PreparsedData);
	HidD_FreePreparsedData(PreparsedData);

#if DEBUG
	fprintf(stderr, " Caps.OutputReportByteLength = %d\n",
			Caps.OutputReportByteLength);
	fprintf(stderr, " Caps.InputReportByteLength = %d\n",
			Caps.InputReportByteLength);
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
static int check_product_string(HANDLE handle)
{
	unsigned short unicode[512];
	char string1[256];
	char string2[256];

	if (!HidD_GetManufacturerString(handle, unicode, sizeof(unicode))) {
		return -1;
	}
	uni_to_string(string1, unicode);

	if (!HidD_GetProductString(handle, unicode, sizeof(unicode))) {
		return -1;
	}
	uni_to_string(string2, unicode);

#if	DUMP_PRODUCT
	fprintf(stderr, "iManufacturer:%s\n", string1);
	fprintf(stderr, "iProduct:%s\n", string2);
#endif

#ifdef	MY_Manufacturer
	if (strcmp(string1, MY_Manufacturer) != 0)
		return 0;
#endif

#ifdef	MY_Product
	if (strcmp(string2, MY_Product) != 0)
		return 0;
#endif

	return 1;					//合致した.
}

////////////////////////////////////////////////////////////////////////
// HIDディバイス一覧からUSB-IOを検索
static int OpenTheHid()
{
	int f = 0;
	int i = 0;
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
		SetupDiGetClassDevs(&HidGuid, NULL, NULL,
							DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
#else
	DeviceInfoSet =
		SetupDiGetClassDevs(&HidGuid, "", NULL,
							DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
#endif

	while (SetupDiEnumDeviceInterfaces
		   (DeviceInfoSet, 0, &HidGuid, i++, &DevData)) {
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DevData, NULL, 0,
										&Needed, 0);
		l = Needed;
		DevDetail =
			(SP_DEVICE_INTERFACE_DETAIL_DATA *) GlobalAlloc(GPTR, l + 4);
		DevDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DevData, DevDetail,
										l, &Needed, 0);

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
		if (DeviceAttributes.VendorID == MY_VID
			&& DeviceAttributes.ProductID == MY_PID
			&& (check_product_string(hHID) == 1)) {
			f = 1;				// 発見された.
			break;
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


int hidasp_list(char *string)
{
	int r, rc;

	LoadHidDLL();
	found_hidaspx = 0;
	r = OpenTheHid("*");
	if (r == 0) {
		rc = 1;
	} else {
		rc = 0;
	}
	if (found_hidaspx==0) {
		fprintf(stderr, "%s not found.\n", string);
	}
	if (hHID_DLL) {
		FreeLibrary(hHID_DLL);
	}
	return rc;
}

//----------------------------------------------------------------------------
//  初期化.
//----------------------------------------------------------------------------
int hidasp_init(const char *string)
{
	unsigned char rd_data[256];
	int i, r;

	LoadHidDLL();
	if (OpenTheHid() == 0) {
#if DEBUG
		fprintf(stderr, "ERROR: fail to OpenTheHid()\n");
#endif
		return 1;
	}

	GetDevCaps();

#if DEBUG
	fprintf(stderr, "HIDASP Connection check!\n");
#endif

	for (i = 0; i < CHECK_COUNT; i++) {
		hidCommand(HIDASP_TEST,(i),0,0);	// Connection test
		r = hidRead(hHID, rd_data ,REPORT_LENGTH1, REPORT_ID1);
#if DEBUG
		fprintf(stderr, "HIDasp Ping(%2d) = %d\n", i, rd_data[1]);
#endif
		if (r == 0) {
			fprintf(stderr, "ERR. fail to Read().\n");
			return 1;
		}
		dev_id = rd_data[1];
		if((dev_id != DEV_ID_FUSION)
		 &&(dev_id != DEV_ID_STD)
		 &&(dev_id != DEV_ID_MEGA88)
		 &&(dev_id != DEV_ID_MEGA88_USERMODE)) {
			fprintf(stderr, "ERR. fail to ping test. (id = %x)!!!\n",dev_id);
			return 1;
		}
		if (rd_data[2] != i) {
			fprintf(stderr, "ERR. fail to ping test. %d != %d\n", rd_data[2] , i);
			return 1;
		}
	}
//	hidCommand(HIDASP_SET_STATUS,0,HIDASP_RST_H_GREEN,0);	// RESET HIGH
	hidCommand(HIDASP_BOOT_RWW,0,0,0);	// Read-While-Write-Section Read Enable.
	r = hidRead(hHID, rd_data ,REPORT_LENGTH1, REPORT_ID1);
	if( rd_data[0] == 0xaa ) {
		have_ledcmd = 1;	// LED制御OK.
	}

#if DEBUG
	if (have_ledcmd) {
		fprintf(stderr, "LED OK.\n");
	}else{
		fprintf(stderr, "Don't have LED COMMAND.\n");
	}
#endif

#if DEBUG
	fprintf(stderr, "OK.\n");
#endif
	return 0;
}

//----------------------------------------------------------------------------
//  終了.
//----------------------------------------------------------------------------
void hidasp_close()
{
	if (hHID) {
		hidSetDevCaps();
		CloseHandle(hHID);
	}
	if (hHID_DLL) {
		FreeLibrary(hHID_DLL);
	}
	hHID = NULL;
	hHID_DLL = NULL;
}

//----------------------------------------------------------------------------
