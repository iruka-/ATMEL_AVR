/*********************************************************************
 *	H I D   B L A S T E R
 *********************************************************************
 *API:
 */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ERROR_OK					(0)
#define	LOG_ERROR(...)
#define	HAVE_STDINT_H

#include "helper/command.h"
#include "hidblast.h"

extern	struct jtag_interface hidblast_interface;

static	struct jtag_command **jtag_command_arg;

/*********************************************************************
 *	*jtag_command_arg 引数取得
 *********************************************************************
 */
struct  jtag_command *get_command_arg(void)
{
	return *jtag_command_arg;
}
/*********************************************************************
 *	初期化
 *********************************************************************
 */
DLL_int get_if_spec(struct jtag_command **q)
{
	jtag_command_arg = q;
	return (int) &hidblast_interface;
}
/*********************************************************************
 *
 *********************************************************************
 */

/*
 * DLLエントリポイント関数の型と典型的なスケルトンコード
 */
BOOL APIENTRY					/* int __stdcall */
DllMain(HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID pParam)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		/* ここにグローバル変数等の初期化コードを書く */
		/* ※loaddll でDLLをロードした時はここが実行される */
		break;

	case DLL_PROCESS_DETACH:
		/* ここにグローバル変数等の後始末コードを書く */
		/* ※freedll でDLLをアンロードした時はここが実行される */
		break;

	case DLL_THREAD_ATTACH:
		/* ここにスレッド毎に必要な変数等の初期化コードを書く */
		break;

	case DLL_THREAD_DETACH:
		/* ここにスレッド毎に必要な変数等の後始末コードを書く */
		break;
	}
	return TRUE;
}
/*********************************************************************
 *
 *********************************************************************
 */
