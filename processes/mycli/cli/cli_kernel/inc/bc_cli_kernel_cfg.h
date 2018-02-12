
/*******************************************************************************
  -------------------------------------------------------------------------
  [File name]  : \\BC\cli\cli_kernel\bc_cli_kernel_cfg.h
  [Author]	   : bain.wang@outlook.com
  [Version]    : 1.0
  [Date]	   : 2017-09-20
  [Description]:

  [Others]	   :

  [Function List]:	
	1. ....
	2. ....
  [History]:
	 D a t e	Modification										  Initials
	---------- --------------										 ----------
	2016-04-21	Modified 											  bain.wang@outlook.com
	2017-09-28	Modified 											  bain.wang@outlook.com
	            添加调试打印宏定义
	2017-09-20  Created                                               bain.wang@outlook.com
*******************************************************************************/

#ifndef __INC_BC_CLI_CFG_H__
#define __INC_BC_CLI_CFG_H__

#include "bc_proj_defs.h"


#define DEF_LANGUAGE					(CLI_LAN_EN)
#define DEF_HOSTNAME					"BINCHIE"
#define DEF_TIMEOUT						(300) /* second */

#define BNR_MAX_LEN 					32
#define BNR_CPU_NAME_MAX_LEN			16

#define DEV_FLSH_BRAND_NAME_LEN			16
#define DEV_FLSH_SYS_DESC_LEN			16
#define DEV_FLSH_VERSION_LEN			16
#define DEV_FLSH_MODULE_NAME_LEN		16

#define CLI_API_MAX_CMD_LEN				1024
#define MIB_CST_MAX_VLAN_ID				4094

#define DEV_NAME BC_SYSTEM_NAME

#define CLI_VERSION						"2.0.2017.0602"
#define CLI_COMPANY 					"BINCHIE"

#define CLI_UTL_PRINT_MAX_PAGE_LINES  	23
#define CLI_CMD_MAX_WORD            	80

#define CLI_DIR_WD_MAX 					50 /* max length of the directory */
#define CLI_DIR_PROMPT_MAX 				64 /* max length of the prompt */

#if 0
#define USR_CFG_FILE_NAME_LEN		1024

#define SYS_PATH_RAM				"/tmp/"
#define SYS_PATH_FLASH				"/usr/cfg/"

#define DEFAULT_CFG_FILE_NAME_TEMP  SYS_PATH_RAM"def_cfg_temp.in"
#define USR_CFG_FILE_NAME_TEMP		SYS_PATH_RAM"usr_cfg_temp.in"

//#define CFG_BZIP2
//#define CFG_GZ
#define CFG_NOTZIP

#if defined CFG_BZIP2
#define DEFAULT_CFG_FILE_NAME       SYS_PATH_FLASH"def_cfg.tar.bzip2"
#define USR_CFG_FILE_NAME_OLD		SYS_PATH_FLASH"usr_cfg_old.tar.bzip2"
#define USR_CFG_FILE_NAME			SYS_PATH_FLASH"usr_cfg.tar.bzip2"
#elif defined CFG_GZ
#define DEFAULT_CFG_FILE_NAME       SYS_PATH_FLASH"def_cfg.tar.gz"
#define USR_CFG_FILE_NAME_OLD		SYS_PATH_FLASH"usr_cfg_old.tar.gz"
#define USR_CFG_FILE_NAME			SYS_PATH_FLASH"usr_cfg.tar.gz"
#else // CFG_NOTZIP
#define DEFAULT_CFG_FILE_NAME       SYS_PATH_FLASH"def_cfg.txt"
#define USR_CFG_FILE_NAME_OLD		SYS_PATH_FLASH"usr_cfg_old.txt"
#define USR_CFG_FILE_NAME			SYS_PATH_FLASH"usr_cfg.txt"
#endif

#define CLI_OUT_FILE 				"/tmp/cli_out"
#endif

/*
 * If define CLI_BRACE_AS_OPTIONAL, Braces'{}' are used to parentheses optional
 * elements, and square brackets'[]' are used to parentheses required elements.
 * If not define it, square brackets'[]' are used to parentheses *optional
 * elements, and Braces '{}' are used to parentheses required elements.
 * If not define it,
 */
// #define CLI_BRACE_AS_OPTIONAL

#define CLI_MAX_ALIAS_NAME 			500 // max alias name supported by CLI

/*
 * If word spaces used to register a syntax command or execute a user command
 * are not enough, CLI kernel wll allocates extra CLI_CFG_STXCMD_WORD_INC
 * word spaces each time.
 */
#define CLI_CFG_CMD_WORD_INC 		50

/*
 * Defines max lines and columns in a standard terminal
 */
#define CLI_CFG_TERMINAL_LINE 		23
#define CLI_CFG_TERMINAL_COLUMN 	80

#define STACKING_STRING_MAX_UNIT 	"8"



////////////////////////////////////////////////////////////////////////////////
//																			  //
//					结 构 体 、联 合 体、枚 举 定 义						  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//																			  //
//						外 部 变 量、 函 数 声 明							  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



#endif /* __INC_BC_CLI_CFG_H__ */

