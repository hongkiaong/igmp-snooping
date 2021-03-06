/************************************************************
  Copyright (C), 1988-2014, xxxx Tech. Co., Ltd.
  FileName:
  Author:  bain.wang@outlook.com      Version :  1.0        Date:2016-6-10
  Description:                   
  Version:                        
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
***********************************************************/

#include "bc_common_defs.h"

#ifndef __BC_ONU_TYPE_H__
#define __BC_ONU_TYPE_H__

/*onu 管理信息*/
typedef struct 
{
	bc_uint32 slotid; 
	bc_uint32 ponid;
	bc_uint32 llid;
} bc_onu_index_t;

/*ONU 类型*/
typedef enum
{
	ONU_TYPE_FIXED = 1,
	ONU_TYPE_CHASSISBASED = 2
} bc_onu_type_e;

/*ONU状态*/
typedef enum
{
	ONU_STATUS_UP = 1,
	ONU_STATUS_DOWN = 2,
	ONU_STATUS_AUTH_REJECT = 3,
	ONU_STATUS_POWER_DOWN = 4,
	ONU_STATUS_TESTING = 5
} bc_onu_status_e;

/*ONU 供应商类型*/
typedef enum
{
	ONU_VERDOR_KT = 1,               /*康特*/ 
	ONU_VERDOR_BD = 2,               /*博达*/
	ONU_VERDOR_XG = 3,               /*新格*/
	ONU_VERDOR_RC = 4,               /*Raisecom*/  
	ONU_VERDOR_UT = 5,               /*UT斯达康*/
	ONU_VERDOR_FH = 6,               /*烽火*/
	ONU_VERDOR_HW = 7,               /*华为*/
	ONU_VERDOR_ZX = 8,               /*中兴*/
	ONU_VERDOR_WL = 9,               /*上海未来宽带*/
	ONU_VERDOR_UNKNOWN,              /*未知*/  
}bc_onu_verdor_e;

/*ONU 加密模式*/
typedef enum
{
	ONU_ENCRPYT_MODE_AES128 = 1,
	ONU_ENCRPYT_MODE_CTC = 2,
	ONU_ENCRPYT_MODE_OTHER = 3
} bc_onu_encrpyt_mode_e;

/*ONU升级状态*/
typedef enum
{
	ONU_UPDATE_WAIT = 1,             /*等待升级*/
	ONU_UPDATE_FAILED = 2,           /*升级失败*/
	ONU_UPDATE_SUCCESSED = 3,        /*升级成功*/
	ONU_UPDATE_SWITCH_FINISHED = 4   /*分区切换完成*/
} bc_onu_update_status_e;


#endif


