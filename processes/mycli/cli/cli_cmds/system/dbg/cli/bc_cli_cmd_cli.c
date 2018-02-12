	/*******************************************************************************
  -------------------------------------------------------------------------
  [File name]  : \\BC\cli\cmds_cmds\system\alarm\bc_cli_cmd_alarm.h
  [Author]     : bin.wang
  [Version]    : 1.0
  [Date]       : 2016-08-26
  [Description]:
  [Others]     :

  [Function List]:
	1. ....
	2. ....
  [History]:
	 D a t e    Modification 										  Initials
	---------- --------------                                        ----------
	2016-08-26  Created												  bin.wang
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								ͷ   ��   ��								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>       
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "bc_proj_defs.h"
#include "bc_msg.h"

#include "bc_cli.h"
#include "bc_cli_type.h"
#include "bc_cli_text.h"
#include "bc_cli_base.h"

#include "bc_platform_thread.h"
#include "bc_platform_time.h"
#include "bc_platform_sem.h"
#include "bc_platform_mutex.h"
#include "bc_platform_mem.h"
#include "bc_platform_queue.h"

//#include "bc_snmp_ipc_client.h"

#include "bc_cli_para_lex_dnf.h"

#include "utili_bit.h"

static I32_T __cli_platform_dump
    (
    IN const UI32_T 	sid,
    IN cli_value_t 		*cli_para,
    IN void 			*user_data
    )
{
	bc_uint32 que_id = 0xffff;

	if (*((bc_uint32*)cli_para->value) == 0)
	{
		bc_platform_thread_show();
	}
	else if (*((bc_uint32*)cli_para->value) == 1)
	{
		bc_platform_timer_show();
	}
	else if (*((bc_uint32*)cli_para->value) == 2)
	{
		bc_platform_sem_show();
	}
	else if (*((bc_uint32*)cli_para->value) == 3)
	{
		bc_platform_mutex_show();
	}
	else if (*((bc_uint32*)cli_para->value) == 4)
	{
		bc_platform_mem_show();
	}
	else if (*((bc_uint32*)cli_para->value) == 5)
	{
		if (cli_para->next)
		{
			que_id = *(bc_uint32*)cli_para->value;
			bc_platform_queue_id_dump(que_id);
		}
		else
		{
			bc_platform_queue_show();
		}
	}
	else
	{
		
	}

	return CLI_CMD_SUCCESS_PRT_NEWLINE;
}

extern bc_err_e utili_block_ext_debug_dump(void);
extern bc_err_e utili_block_ext_debug_dump_single(bc_uint32 ofs);
static I32_T __cli_utili_ext_block_debug
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
	utili_block_ext_debug_dump();
	return 0;
}

static I32_T __cli_utili_ext_block_single_debug
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
	bc_uint32 ofs;

	ofs = *(bc_uint32*)ptrCliValue->value;
	utili_block_ext_debug_dump_single(ofs);
	return 0;
}

extern bc_err_e bc_cli_lex_lex_dbg(bc_char *match_str);
static I32_T __cli_lex_lex_debug
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
	bc_err_e ret;

	BC_PT_COMMAND(BC_MODULE_CLI, "in:%s \n\r", (bc_char*)ptrCliValue->value);

	if ((ret = bc_cli_lex_lex_dbg((bc_char*)ptrCliValue->value)) != BC_ERR_OK)
	{
		BC_PT_COMMAND(BC_MODULE_CLI,"err: %s\n\r", bc_msg_err_get(ret, BC_LAN_ENGLISH_E));
	}
	return 0;
}

extern bc_err_e bc_cli_para_lex_bnf_dump_lex_lex(void);
static I32_T __cli_lex_lex_dump_debug
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
	bc_cli_para_lex_bnf_dump_lex_lex();
	return 0;
}

/******************************************************************************\
  Function   : bc_cli_cmd_snmpcfg_reg
  Description: 
  Return     : 
  Others     : 
\******************************************************************************/
void bc_cli_cmd_cli_reg(void)
{
	bc_cli_reg_cmd(
		CLI_MOD_DEBUG_PATH
		"/cli "				
		"platform {thread(0) | timer(1) | sem(2) | mutex(3) | mem(4) | queue(5) [<uint>]}"				
		,
		__cli_platform_dump,
		CLI_ACC_LVL_USER,
		"",
		"",
		NULL);	

	bc_cli_reg_cmd(
		CLI_MOD_DEBUG_PATH
		"/cli utili block_ext",
		__cli_utili_ext_block_debug,
		CLI_ACC_LVL_ADMIN,
		"command use \"\" include",
		"",
		NULL
		);

	bc_cli_reg_cmd(
		CLI_MOD_DEBUG_PATH
		"/cli utili block_ext ofs <uint>",
		__cli_utili_ext_block_single_debug,
		CLI_ACC_LVL_ADMIN,
		"command use \"\" include",
		"",
		NULL
		);

	bc_cli_reg_cmd(
		CLI_MOD_DEBUG_PATH
		"/cli lex_lex <string>",
		__cli_lex_lex_debug,
		CLI_ACC_LVL_ADMIN,
		"command use \"\" include",
		"",
		NULL
		);

	bc_cli_reg_cmd(
		CLI_MOD_DEBUG_PATH
		"/cli lex_lex_dump",
		__cli_lex_lex_dump_debug,
		CLI_ACC_LVL_ADMIN,
		"command use \"\" include",
		"",
		NULL
		);
}

