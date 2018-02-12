/*******************************************************************************
  Copyright (C), 1988-2015, xxxxx Tech. Co., Ltd.
  [File name]  : bc_filetrans_file.c
  [Author]     : wangbin
  [Version]    : 1.0
  [Date]       : 2015-01-20
  [Description]:

  [Others]     :

  [Function List]:  
    1. ....
    2. ....
  [History]:
	 D a t e    Modification 										  Initials
	---------- --------------                                        ----------
	2015-01-20  Created												  wangbin
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								头   文   件								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

#include "bc_filetrans_file.h"
//#include "bc_ipc_package.h"
#include "bc_err.h"
#include "bc_filetrans_def.h"
#include "bc_filetrans_file.h"
#include "bc_filetrans_core.h"
//#include "bc_func.h"
//#include "bc_modules_com_para.h"


////////////////////////////////////////////////////////////////////////////////
//																			  //
//						外 部 变 量、 函 数 引 用					          //
//																			  //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//																			  //
//						结   构   体   宏   定   义							  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
/*
 *		当源文件需要定义宏，而头文件又与其它模块头文件中的宏定义有重复定义嫌疑
 *	时，在此处定义。
 */


////////////////////////////////////////////////////////////////////////////////
//																			  //
//							全 局 变 量 定 义							      //
//																			  //
////////////////////////////////////////////////////////////////////////////////
/* 
 *	外部全局变量，提供整个工程调用，在头文件中提供调用接口(外部引用声明)。
 */

/* 
 *	内部全局变量，只提供给该文件内部函数调用，不在头文件中提供调用接口。
 */


////////////////////////////////////////////////////////////////////////////////
//																			  //
//								函  数  声  明							      //
//																			  //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//																			  //
//								函  数  定  义							      //
//																			  //
////////////////////////////////////////////////////////////////////////////////



/******************************************************************************\
Function   : bc_filetrans_init
Description: 初始化
Return     : ktep_errcode_e  成功、失败
Others     : 
\******************************************************************************/
bc_err_e bc_filetrans_init(void)
{
	return bc_filetrans_init_core();
}


/******************************************************************************\
  Function   : bc_filetrans_get_info
  Description: 获取文件传输参数
  Return     : bc_err_e  成功、失败
  Others     : 
\******************************************************************************/
bc_err_e bc_filetrans_get_info(bc_filetrans_function_e trans_fun, bc_filetrans_cfg_t *para)
{
	bc_err_e ret = BC_ERR_OK;

	if (NULL == para)
	{
		return BC_ERR_PARA;
	}

	if (BC_ERR_OK != (ret = bc_filetrans_get_info_core(trans_fun, para)))
	{
		return ret;
	}
		                        
	return ret;
}



/******************************************************************************\
  Function   : bc_filetrans_set_download_file
  Description: 下载设置
  Return     : bc_err_e  成功、失败
  Others     : 
\******************************************************************************/
bc_err_e bc_filetrans_set_download_file(bc_filetrans_cfg_t para)
{
	bc_err_e    ret = BC_ERR_OK;

	if (BC_ERR_OK != (ret = bc_filetrans_set_info_core(para.trans_fun, para)))
	{
		return ret;
	}

	if (BC_ERR_OK != (ret = bc_filetrans_download_file_core(para.trans_fun)))
	{
		return ret;
	}
								
	return ret;
}

/* 
 *	预留一个注释在文件最后
 */
/******************************************************************************\
  Function   : 
  Description: 
  Return     : 
  Others     : 
\******************************************************************************/


