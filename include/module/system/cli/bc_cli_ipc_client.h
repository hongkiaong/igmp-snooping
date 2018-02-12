/*******************************************************************************
  Copyright (C), 2010-2030, xxxxx Tech. Co., Ltd.
  [File name]  : \\DEV\Include\Common\Ipc\bc_ipc_bc_client.h
  [Author]     : bain.wang@outlook.com
  [Version]    : 1.0
  [Date]       : 2016-04-10
  [Description]:

  [Others]     :

  [Function List]:
	1. ....
	2. ....
  [History]:
	 D a t e	Modification										  Initials
	---------- --------------										 ----------
	2016-04-10	Created 											  bain.wang@outlook.com
*******************************************************************************/

#ifndef __BC_CLI_IPC_CLIENT_H__
#define __BC_CLI_IPC_CLIENT_H__

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								ͷ   ��   ��								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#include "bc_err.h"
#include "bc_ipc_def.h"
#include "bc_ipc_package.h"
#include "bc_ipc_client.h"

/*DEV���Ľ��� client ����*/
extern bc_err_e bc_ipc_cli_client_request(bc_uint32 session_id, bc_uint32 functionid, bc_ipc_package_t *data);

/*���� IPC client*/
extern bc_err_e bc_ipc_cli_client_create(bc_ipc_client_type_e client_type);

extern bc_err_e bc_ipc_cli_client_release(void);

/*xx client �ͷ�*/
extern void bc_ipc_cli_client_dump(void);


#endif /* __BC_IPC_DEV_CLIENT_H__ */
