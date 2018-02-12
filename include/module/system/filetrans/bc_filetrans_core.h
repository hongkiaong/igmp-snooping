/*******************************************************************************
  -------------------------------------------------------------------------
  [File name]  : 
  [Author]     : bain.wang@outlook.com
  [Version]    : 1.0
  [Date]       : 2017-06-11
  [Description]:

  [Others]     :

  [Function List]:  
    1. ....
    2. ....
  [History]:
	 D a t e    Modification 										  Initials
	---------- --------------                                        ----------
	2016-08-04  Created												  bain.wang@outlook.com
*******************************************************************************/

#ifndef __BC_FILETRANS_CORE_H__
#define __BC_FILETRANS_CORE_H__

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								头   文   件								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#include "bc_err.h"
#include "bc_filetrans_def.h"
#include "bc_modules_com_para.h"

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								宏   定   义								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////


/*
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
#define FLASH_ERASEALL_DEV_MTD2	"flash_eraseall /dev/mtd2"
*/


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

/******************************************************************************\
  Function   : bc_filetrans_get_list_core
  Description: 获取文 件的传输信息
  Return     : bc_err_e  成功、失败
  Others     : 
\******************************************************************************/
bc_err_e bc_filetrans_get_list_core(bc_filetrans_cfg_t paras[BC_FILETRANS_MAX], bc_int32 *num);

/******************************************************************************\
  Function   : bc_filetrans_get_info_core
  Description: 获取某个文 件的传输信息
  Return     : bc_err_e  成功、失败
  Others     : 
\******************************************************************************/
bc_err_e bc_filetrans_get_info_core(bc_filetrans_function_e trans_fun, bc_filetrans_cfg_t *para);

/******************************************************************************\
  Function   : bc_filetrans_set_info_core
  Description: 设置传输信息
  Return     : bc_err_e  成功、失败
  Others     : 
\******************************************************************************/
bc_err_e bc_filetrans_set_info_core(bc_filetrans_function_e trans_fun, bc_filetrans_cfg_t para);

//上传文 件
bc_err_e bc_filetrans_upload_file_core(bc_filetrans_function_e trans_fun);

//下载文 件
bc_err_e bc_filetrans_download_file_core(bc_filetrans_function_e trans_fun);

//初始化
bc_err_e bc_filetrans_init_core(void);

//初始化
bc_err_e bc_filetrans_init(void);


bc_err_e bc_filetrans_core_switch_over_kernel();
bc_err_e bc_filetrans_core_switch_over_file_system();

#endif /* __BC_FILETRANS_CORE_H__ */




