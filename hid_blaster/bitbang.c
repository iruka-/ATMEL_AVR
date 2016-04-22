/***************************************************************************
 *   Copyright (C) 2005 by Dominic Rath                                    *
 *   Dominic.Rath@gmx.de                                                   *
 *                                                                         *
 *   Copyright (C) 2007,2008 ﾃ�yvind Harboe                                 *
 *   oyvind.harboe@zylin.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "bitbang.h"
#include <jtag/interface.h>
#include <jtag/commands.h>

#include "monit.h"
#include "hidasp.h"


#define	VERBOSE	0
#define	if_V	if(VERBOSE)

#define	DUMP_PIN_STATE	0

#define	JCMD_BITCOUNT_MASK	0x3f
#define	JCMD_INIT_TMS		0x80
#define	JCMD_LAST_TMS		0x40

//
//	USBでの TDI/TDOストリームのlengthは6bit表現なので、56bit以内に制限.
//
#define	BANG_MAX_BYTES	7					// JTAGコマンドで送受するTDI/TDOストリームの最大長さ(byte)
#define	BANG_MAX_BITS	(BANG_MAX_BYTES*8)	// JTAGコマンドで送受するTDI/TDOストリームの最大長さ(bit)
//

/*
int  get_portb(void);
void set_portb(int d);
*/

#if	0
JTAG_SCAN         = 1,
JTAG_TLR_RESET    = 2,
JTAG_RUNTEST      = 3,
JTAG_RESET        = 4,
JTAG_PATHMOVE     = 6,
JTAG_SLEEP        = 7,
JTAG_STABLECLOCKS = 8,
JTAG_TMS          = 9,
#endif
static char *jtag_cmd_name[]= {
	"???",
	"JTAG_SCAN        " ,
	"JTAG_TLR_RESET   " ,
	"JTAG_RUNTEST     " ,
	"JTAG_RESET       " ,
	"JTAG_PATHMOVE    " ,
	"JTAG_SLEEP       " ,
	"JTAG_STABLECLOCKS" ,
	"JTAG_TMS         " ,
};


#if	DUMP_PIN_STATE
char scan_dump_buf[3][32768];
#endif

/**
 * Function bitbang_stableclocks
 * issues a number of clock cycles while staying in a stable state.
 * Because the TMS value required to stay in the RESET state is a 1, whereas
 * the TMS value required to stay in any of the other stable states is a 0,
 * this function checks the current stable state to decide on the value of TMS
 * to use.
 */
static void bitbang_stableclocks(int num_cycles);

struct jtag_command *get_command_arg(void);

//struct bitbang_interface *bitbang_interface;

/* DANGER!!!! clock absolutely *MUST* be 0 in idle or reset won't work!
 *
 * Set this to 1 and str912 reset halt will fail.
 *
 * If someone can submit a patch with an explanation it will be greatly
 * appreciated, but as far as I can tell (ﾃ路) DCLK is generated upon
 * clk = 0 in TAP_IDLE. Good luck deducing that from the ARM documentation!
 * The ARM documentation uses the term "DCLK is asserted while in the TAP_IDLE
 * state". With hardware there is no such thing as *while* in a state. There
 * are only edges. So clk => 0 is in fact a very subtle state transition that
 * happens *while* in the TAP_IDLE state. "#&ﾂ､"#ﾂ､&"#&"#&
 *
 * For "reset halt" the last thing that happens before srst is asserted
 * is that the breakpoint is set up. If DCLK is not wiggled one last
 * time before the reset, then the breakpoint is not set up and
 * "reset halt" will fail to halt.
 *
 */
#define CLOCK_IDLE() 0

/*********************************************************************
 *
 *********************************************************************
 */
//{
static char bang_buf[256];
static char bang_result[256];
static int  bang_index;
static int  bang_type ;
//}


#if	DUMP_PIN_STATE
/*********************************************************************
 *
 *********************************************************************
 */
static	void print_c(int d)
{
	if(d) 	printf("1");
	else	printf("0");
}
#endif

/*********************************************************************
 *	BitBang USBストリームを開始する.
 *********************************************************************
 */
static void bang_start(int type,int tms0,int tms1)
{
	bang_type = type;
	bang_index= 0;
}
/*********************************************************************
 *	BitBang USBストリームの読み取り結果をbuffer[ bit_cnt ]に書き戻す.
 *********************************************************************
 *	buffer : TDOの読み取り結果をbitで詰め込む返却バッファ
 *	bit_cnt: buffer[] ビット配列のビットポジション.
 *	bdata  : 読み取り値( 1 bitのみ )
 */
static void	set_bit(uint8_t *buffer, int bit_cnt,int bdata)
{
	int bytec = bit_cnt/8;
	int bcval = 1 << (bit_cnt % 8);

	if(bdata) {
		buffer[bytec] |= bcval;
	} else {
		buffer[bytec] &= ~bcval;
	}
}
/*********************************************************************
 *	BitBang USBストリームの読み取り結果の n bit目を取り出す.
 *********************************************************************
 *	buffer : TDOの読み取り結果をbitで詰め込む返却バッファ
 *	bit_cnt: buffer[] ビット配列のビットポジション.
 *	return : 読み取り値( 1 bitのみ )
 */
static int get_bit(uint8_t *buffer, int bit_cnt)
{
	int bytec = bit_cnt/8;
	int bcval = 1 << (bit_cnt % 8);

	return buffer[bytec] & bcval;
}
/*********************************************************************
 *	読み込んだbit列をbufferの所定位置に返す.
 *********************************************************************
 *	buffer : TDOの読み取り結果をbitで詰め込む返却バッファ
 *	result : USBストリームの読み取り結果(1tick が 1byteに対応 bitTDOのみ有効)
 *	count  : 変換したいビット数.
 *	readcnt: buffer[]に既に読み込み済みのビット数*2
 *				( x2 なのは TCKが 0 -> 1 と2tick要するため)
 */
static void	bang_get_result(uint8_t *buffer,uchar *result,int count)
{
	int i,b;
	if(buffer!=NULL) {
		for(i=0; i<count; i++) {
			b = get_bit(result,i);
				set_bit(buffer,i,b);
		}
	}
}
/*********************************************************************
 *	bang_indexが残っていたら、それをデバイスに送出する.
 *********************************************************************
 *	bang_typeがSCAN_OUT以外であれば、
 *		さらに読み込んだbit列をbufferの所定位置に返す.
 */
static void bang_flush(uint8_t *buffer,int scan_size)
{
	uchar *result = NULL;

	if (bang_type != SCAN_OUT) {
		result = bang_result;
	}

	if(	bang_index ) {
		bang_buf[bang_index++]=0;
		jcmd_write(bang_buf,bang_index,result);
		if(result) {
			bang_get_result(buffer,result,scan_size);
		}
		bang_index= 0;
	}
}

/*********************************************************************
 *	bang_indexが残っていたら、それをデバイスに送出する.
 *********************************************************************
 *	bang_typeがSCAN_OUT以外であれば、
 *		さらに読み込んだbit列をbufferの所定位置に返す.
 */
static void bang_write_flush(void)
{
	if( bang_index >= 32 ) {
		printf("FATAL: bang_write_flush(%d) OVERRUN ERROR\n",bang_index);
		bang_index= 0;
		return;
	}

	if(	bang_index ) {
		bang_buf[0]=(bang_index-1) | JCMD_INIT_TMS;
		bang_buf[bang_index++]=0;
		jcmd_write(bang_buf,bang_index,NULL);
		bang_index= 0;
	}
}


/*********************************************************************
 *	1tick分の情報(tck,tms,tdi)をUSBストリームに書き出す.
 *********************************************************************
 */
static inline void bang_write_buf(int tdi)
{
	bang_buf[bang_index++]=tdi;
}

/*********************************************************************
 *	1tick分の情報(tck,tms,tdi)をUSBストリームに書き出す.
 *********************************************************************
 */
static inline int pack_bit(int tck,int tms,int tdi)
{
	int 	 c  = JCMD_bitTDO;			// PullUP
	if (tck) c |= JCMD_bitTCK;
	if (tms) c |= JCMD_bitTMS;
	if (tdi) c |= JCMD_bitTDI;
	return c;
}

/*********************************************************************
 *	1tick分の情報(tck,tms,tdi)をUSBストリームに書き出す.
 *********************************************************************
 */
static inline void bang_write_hi(int tck,int tms,int tdi)
{
	int c = pack_bit(tck,tms,tdi);
	bang_buf[bang_index]=c;
}

/*********************************************************************
 *	1tick分の情報(tck,tms,tdi)をUSBストリームに書き出す.
 *********************************************************************
 */
static inline void bang_write_lo(int tck,int tms,int tdi)
{
	int c = pack_bit(tck,tms,tdi);
	bang_buf[bang_index] |= (c>>4);
	bang_index++;
}

/*********************************************************************
 *	2tick分の情報(tck,tms,tdi)をUSBストリームに書き出す.
 *********************************************************************
 */
static inline void bang_write_hilo(int tck,int tms,int tdi)
{
	bang_write_hi(tck,tms,tdi);
	bang_write_lo(tck,tms,tdi);
}
/*********************************************************************
 *	1tick分の情報(tck,tms,tdi)をUSBストリームに書き出す.
 *********************************************************************
 */
static inline void bang_write1(int tck,int tms,int tdi)
{
	int c = pack_bit(tck,tms,tdi);
	set_portb(c);
}

/*********************************************************************
 *	1tick分の情報(tck,tms,tdi)をUSBストリームに書き出す.
 *********************************************************************
 */
static inline int bang_read(void)
{
	if( get_portb() & bitTDO ) return 1;
	return 0;
}

/* my private tap controller state, which tracks state for calling code */
static tap_state_t dummy_state = TAP_RESET;

static int dummy_clock;         /* edge detector */

static void bang_reset(int trst, int srst)
{
	dummy_clock = 0;

	if (trst || (srst && (jtag_get_reset_config() & RESET_SRST_PULLS_TRST))) {
		dummy_state = TAP_RESET;
	}
//	LOG_DEBUG("reset to: %s", tap_state_name(dummy_state));
}


/*********************************************************************
 *
 *********************************************************************
 */

/* The bitbang driver leaves the TCK 0 when in idle */
static void bitbang_end_state(tap_state_t state)
{
	if (tap_is_state_stable(state))
		tap_set_end_state(state);
	else {
		LOG_ERROR("BUG: %i is not a valid end state", state);
		exit(-1);
	}
}

/*********************************************************************
 *	
 *********************************************************************
 */
static void bitbang_state_move(int skip)
{
	int i = 0, tms = 0;
	uint8_t tms_scan = tap_get_tms_path(tap_get_state(), tap_get_end_state());
	int    tms_count = tap_get_tms_path_len(tap_get_state(), tap_get_end_state());

	if_V printf("=*=bitbang_state_move(%d) scan=0x%x count=%d\n",skip,tms_scan,tms_count);

	bang_start(SCAN_OUT,0,0);
	bang_write_buf(JCMD_INIT_TMS);			// dummy.
	for (i = skip; i < tms_count; i++) {
		tms = (tms_scan >> i) & 1;
		bang_write_hi(0, tms, 0);
		bang_write_lo(1, tms, 0);
	}
//	bang_write1(CLOCK_IDLE(), tms, 0);
	bang_write_hilo(CLOCK_IDLE(), tms, 0);
	bang_write_flush();

	tap_set_state(tap_get_end_state());
}



/**
 * Clock a bunch of TMS (or SWDIO) transitions, to change the JTAG
 * (or SWD) state machine.
 */
static int bitbang_execute_tms(struct jtag_command *cmd)
{
	unsigned	num_bits = cmd->cmd.tms->num_bits;
	const uint8_t	*bits = cmd->cmd.tms->bits;

	DEBUG_JTAG_IO("TMS: %d bits", num_bits);

	int tms = 0;

	bang_start(SCAN_OUT,0,0);
	bang_write_buf(JCMD_INIT_TMS);			// dummy.
	for (unsigned i = 0; i < num_bits; i++) {
		tms = ((bits[i/8] >> (i % 8)) & 1);
		bang_write_hi(0, tms, 0);
		bang_write_lo(1, tms, 0);
	}
	bang_write_hilo(CLOCK_IDLE(), tms, 0);
	bang_write_flush();

	return ERROR_OK;
}


/*********************************************************************
 *
 *********************************************************************
 */
static void bitbang_path_move(struct pathmove_command *cmd)
{
	int num_states = cmd->num_states;
	int state_count;
	int tms = 0;

	state_count = 0;

	bang_start(SCAN_OUT,0,0);
	bang_write_buf(JCMD_INIT_TMS);			// dummy.
	while (num_states) {
		if (tap_state_transition(tap_get_state(), false) == cmd->path[state_count]) {
			tms = 0;
		} else if (tap_state_transition(tap_get_state(), true) == cmd->path[state_count]) {
			tms = 1;
		} else {
			LOG_ERROR("BUG: %s -> %s isn't a valid TAP transition", tap_state_name(tap_get_state()), tap_state_name(cmd->path[state_count]));
			exit(-1);
		}
		bang_write_hi(0, tms, 0);
		bang_write_lo(1, tms, 0);
		tap_set_state(cmd->path[state_count]);
		state_count++;
		num_states--;
	}
	bang_write_hilo(CLOCK_IDLE(), tms, 0);
	bang_write_flush();

	tap_set_end_state(tap_get_state());
}

/*********************************************************************
 *
 *********************************************************************
 */
static void bitbang_runtest(int num_cycles)
{
	int i;

	tap_state_t saved_end_state = tap_get_end_state();

	/* only do a state_move when we're not already in IDLE */
	if (tap_get_state() != TAP_IDLE) {
		bitbang_end_state(TAP_IDLE);
		bitbang_state_move(0);
	}

	if(	num_cycles >= (BANG_MAX_BITS-2) ) {
		num_cycles  = (BANG_MAX_BITS-2);
	}

	bang_start(SCAN_OUT,0,0);
	bang_write_buf(JCMD_INIT_TMS);			// dummy.
	/* execute num_cycles */
	for (i = 0; i < num_cycles; i++) {
		bang_write_hi(0, 0, 0);
		bang_write_lo(1, 0, 0);
	}
	bang_write_hilo(CLOCK_IDLE(), 0, 0);
	bang_write_flush();

	/* finish in end_state */
	bitbang_end_state(saved_end_state);
	if (tap_get_state() != tap_get_end_state()) {
		bitbang_state_move(0);
	}
}


/*********************************************************************
 *
 *********************************************************************
 */
static void bitbang_stableclocks(int num_cycles)
{
	int tms = (tap_get_state() == TAP_RESET ? 1 : 0);
	int i;

	/* send num_cycles clocks onto the cable */
	bang_start(SCAN_OUT,0,0);
	bang_write_buf(JCMD_INIT_TMS);			// dummy.
	for (i = 0; i < num_cycles; i++) {
		bang_write_hi(1, tms, 0);
		bang_write_lo(0, tms, 0);
	}
	bang_write_flush();
}

/*********************************************************************
 *
 *********************************************************************
 */
static void bitbang_scan_custom(bool ir_scan, enum scan_type type, uint8_t *buffer, int scan_size,int eod)
{
//	tap_state_t saved_end_state = tap_get_end_state();
	int byte_cnt;
	int tdi_byte;
	int byte_size = (scan_size+7)/8 ;

	if_V {
		int i;
		printf("=CUSTOM(%x %x) ",type,scan_size);
		for(i=0; i<scan_size/8; i++) {
			printf("%02x ",buffer[i]);
		}
		printf("\n");
	}

	bang_start(type,0,1);

	bang_write_buf( scan_size | eod );

	for (byte_cnt = 0; byte_cnt < byte_size; byte_cnt++) {
		tdi_byte = 0;
		if (type != SCAN_IN) {
			tdi_byte = buffer[byte_cnt];
		}
		bang_write_buf(tdi_byte);
	}
	bang_flush(buffer,scan_size);
}

/*********************************************************************
 *
 *********************************************************************
 */
static void bitbang_scan(bool ir_scan, enum scan_type type, uint8_t *buffer, int scan_size)
{
	int bit_cnt;

	tap_state_t saved_end_state = tap_get_end_state();

	if (!((!ir_scan && (tap_get_state() == TAP_DRSHIFT)) || (ir_scan && (tap_get_state() == TAP_IRSHIFT)))) {
		if (ir_scan) {
			bitbang_end_state(TAP_IRSHIFT);
		} else {
			bitbang_end_state(TAP_DRSHIFT);
		}
		bitbang_state_move(0);
		bitbang_end_state(saved_end_state);
	}

	if_V {
		int i;
		printf("=SCAN(%x %x) ",type,scan_size);
		for(i=0; i<scan_size/8; i++) {
			printf("%02x ",buffer[i]);
		}
		printf("\n");
	}

	if((scan_size < BANG_MAX_BITS)) {
		bitbang_scan_custom(ir_scan,type,buffer,scan_size,JCMD_LAST_TMS );
		if (tap_get_state() != tap_get_end_state()) {
			bitbang_state_move(1);
		}
		return;
	}

	int size_once;
	int eod = 0;
	while(scan_size) {	//分割転送.
		size_once = scan_size;
		if(	size_once>= BANG_MAX_BITS) {
			size_once = BANG_MAX_BITS;
		}
		scan_size -= size_once;
		if( scan_size == 0) {eod = JCMD_LAST_TMS;}
		bitbang_scan_custom(ir_scan,type,buffer,size_once,eod);
		buffer    += BANG_MAX_BYTES;
	}

	if (tap_get_state() != tap_get_end_state()) {
		bitbang_state_move(1);
	}
}

/*********************************************************************
 *
 *********************************************************************
 */
int bitbang_execute_queue(void)
{
//	struct jtag_command *cmd = jtag_command_queue; /* currently processed command */
	struct jtag_command *cmd = get_command_arg();
	int  scan_size;
	enum scan_type type;
	uint8_t *buffer;
	int  retval;

	/* return ERROR_OK, unless a jtag_read_buffer returns a failed check
	 * that wasn't handled by a caller-provided error handler
	 */
	retval = ERROR_OK;

//	if (bitbang_interface->blink)
//		bitbang_interface->blink(1);

	while (cmd) {
		char *name=jtag_cmd_name[cmd->type];
		if_V printf("=*=bitbang_execute_queue(cmd=%x) %s\n",cmd->type,name);

		switch (cmd->type) {
		case JTAG_RESET:
#ifdef _DEBUG_JTAG_IO_
			LOG_DEBUG("reset trst: %i srst %i", cmd->cmd.reset->trst, cmd->cmd.reset->srst);
#endif
			if ((cmd->cmd.reset->trst == 1) || (cmd->cmd.reset->srst && (jtag_get_reset_config() & RESET_SRST_PULLS_TRST))) {
				tap_set_state(TAP_RESET);
			}
//			bitbang_interface->reset(cmd->cmd.reset->trst, cmd->cmd.reset->srst);
			bang_reset(cmd->cmd.reset->trst, cmd->cmd.reset->srst);
			break;
		case JTAG_RUNTEST:
#ifdef _DEBUG_JTAG_IO_
			LOG_DEBUG("runtest %i cycles, end in %s", cmd->cmd.runtest->num_cycles, tap_state_name(cmd->cmd.runtest->end_state));
#endif

			bitbang_end_state(cmd->cmd.runtest->end_state);
			bitbang_runtest(cmd->cmd.runtest->num_cycles);
			break;

		case JTAG_STABLECLOCKS:
			/* this is only allowed while in a stable state.  A check for a stable
			 * state was done in jtag_add_clocks()
			 */
			bitbang_stableclocks(cmd->cmd.stableclocks->num_cycles);
			break;

		case JTAG_TLR_RESET:
#ifdef _DEBUG_JTAG_IO_
			LOG_DEBUG("statemove end in %s", tap_state_name(cmd->cmd.statemove->end_state));
#endif
			bitbang_end_state(cmd->cmd.statemove->end_state);
			bitbang_state_move(0);
			break;
		case JTAG_PATHMOVE:
#ifdef _DEBUG_JTAG_IO_
			LOG_DEBUG("pathmove: %i states, end in %s", cmd->cmd.pathmove->num_states,
			          tap_state_name(cmd->cmd.pathmove->path[cmd->cmd.pathmove->num_states - 1]));
#endif
			bitbang_path_move(cmd->cmd.pathmove);
			break;
		case JTAG_SCAN:
#ifdef _DEBUG_JTAG_IO_
			LOG_DEBUG("%s scan end in %s",  (cmd->cmd.scan->ir_scan) ? "IR" : "DR", tap_state_name(cmd->cmd.scan->end_state));
#endif
			bitbang_end_state(cmd->cmd.scan->end_state);

			scan_size = jtag_build_buffer(cmd->cmd.scan, &buffer);
			     type = jtag_scan_type(cmd->cmd.scan);
			bitbang_scan(cmd->cmd.scan->ir_scan, type, buffer, scan_size);
			if (jtag_read_buffer(buffer, cmd->cmd.scan) != ERROR_OK)
				retval = ERROR_JTAG_QUEUE_FAILED;
			if (buffer)
				free(buffer);
			break;

		case JTAG_SLEEP:
#ifdef _DEBUG_JTAG_IO_
			LOG_DEBUG("sleep %" PRIi32, cmd->cmd.sleep->us);
#endif
			jtag_sleep(cmd->cmd.sleep->us);
			break;
		case JTAG_TMS:
			retval = bitbang_execute_tms(cmd);
			break;
		default:
			LOG_ERROR("BUG: unknown JTAG command type encountered");
			exit(-1);
		}
		cmd = cmd->next;
	}
//	if (bitbang_interface->blink)
//		bitbang_interface->blink(0);

	return retval;
}
static int dummy_khz(int khz, int *jtag_speed)
{
	if (khz == 0) {
		*jtag_speed = 0;
	} else {
		*jtag_speed = 64000/khz;
	}
	return ERROR_OK;
}

static int dummy_speed_div(int speed, int *khz)
{
	if (speed == 0) {
		*khz = 0;
	} else {
		*khz = 64000/speed;
	}

	return ERROR_OK;
}

static int dummy_speed(int speed)
{
	return ERROR_OK;
}

static int dummy_init(void)
{
//	bitbang_interface = &dummy_bitbang;

	printf("=*= dummy_init(void)\n");

	UsbInit(1,0,"*");
	set_ddrb( dirTDI|dirTCK|dirTMS );	// TDOは入力.
	return ERROR_OK;
}

static int dummy_quit(void)
{
	printf("=*= dummy_quit(void)\n");

	UsbExit();

	return ERROR_OK;
}

/*********************************************************************
 *
 *********************************************************************
 */
static const struct command_registration dummy_command_handlers[] = {
	{
		.name = "dummy",
		.mode = COMMAND_ANY,
		.help = "dummy interface driver commands",

//		.chain = hello_command_handlers,
	},
	COMMAND_REGISTRATION_DONE,
};

/*********************************************************************
 *
 *********************************************************************
 */
const char *jtag_only[] = { "jtag", NULL, };

/* The dummy driver is used to easily check the code path
 * where the target is unresponsive.
 */
struct jtag_interface hidblast_interface = {
	.name = "hid_blaster",

//	.supported = DEBUG_CAP_TMS_SEQ,
	.commands  = dummy_command_handlers,
	.transports = jtag_only,

	.execute_queue = &bitbang_execute_queue,

	.speed = &dummy_speed,
	.khz   = &dummy_khz,
	.speed_div = &dummy_speed_div,

	.init = &dummy_init,
	.quit = &dummy_quit,
};
/*********************************************************************
 *
 *********************************************************************
 */
