/*******************************************************************************
  Copyright (C), 2010-2030, xxxxx Tech. Co., Ltd.
  [File name]  : \\BCCommon\Ipc\bc_session_ipc_client.h
  [Author]     : bain.wang@outlook.com
  [Version]    : 1.0
  [Date]       : 2015-08-4
  [Description]:

  [Others]     :

  [Function List]:  
    1. ....
    2. ....
  [History]:
	 D a t e    Modification 										  Initials
	---------- --------------                                        ----------
	2015-08-4  Created												  bain.wang@outlook.com
*******************************************************************************/

#ifndef __BC_SESSION_IPC_CLIENT_H__
#define __BC_SESSION_IPC_CLIENT_H__

#include "bc_err.h"
#include "bc_session_def.h"
#include "bc_ipc_def.h"

extern bc_err_e bc_session_ipc_create(bc_ipc_client_type_e client_type, bc_uint32 agging, bc_uint32 *sessionid);
extern bc_err_e bc_session_ipc_release(bc_uint32 sessionid);

extern bc_err_e bc_session_ipc_time_tick(bc_uint32 sessionid);

extern bc_err_e bc_session_ipc_dump(void);
extern bc_err_e bc_session_ipc_msg_back(bc_uint32 sessionid);

#endif

