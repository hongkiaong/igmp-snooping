/*************************************************
  Copyright (C), 1988-2020, xxxxx Tech. Co., Ltd.
  File name:      sys_message.h
  Author:  bain.wang@outlook.com     Version:  v1.0      Date: 2013/10/08
  Description:    Define internal interface and data structure of sys module
  Others:         none
  Function List:  none
  History:          
    1. Date: 2013/10/08
       Author: bain.wang@outlook.com       
       Modification: create
  
*************************************************/

#ifndef __BC_SESSION_DEF_H__
#define __BC_SESSION_DEF_H__

#include "bc_err.h"
#include "bc_common_defs.h"
#include "bc_ipc_def.h"
#include "bc_ipc_client.h"

#define BC_SESSION_NO_AGE   0xFFFFFFFF                           /*���Ĭ��Session �ϻ�ʱ��*/
#define BC_SESSION_DEF_TICK_INTERVAL 60   		/* unit: second */
#define BC_SESSION_DEF_TIMEOUT (BC_SESSION_DEF_TICK_INTERVAL)*5
#define BC_SESSION_MAX                  32                /*���SESSION ��*/    

/*Session �ṹ*/
typedef struct
{ 
	bc_boolean    				is_used;          	 /*�Ƿ�ʹ�� TRUE��ʹ�� FALSE��δʹ��*/ 

	bc_uint32             			sessionid;        	/*�ỰID*/
	bc_ipc_client_type_e  		client_type;      	/*�ͻ�������*/

	bc_uint32               		agging;          	 /*�ϻ�ʱ�� ��*/
	bc_uint32           			time_tick;       	 /*��ʱ�Ӽ���*/

	bc_boolean          		is_srv_cfg;      		/*�Ƿ�����Server para*/  
	bc_ipc_server_data_t	srv_para;        		 /*�ͻ���IPC server����*/ 
} bc_session_info_t;

#endif
