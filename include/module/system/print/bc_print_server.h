/************************************************************
  -------------------------------------------------------------------------
  FileName: bc_print_srv.h
  Author:  bain.wang@outlook.com      Version :  1.0        Date:2016-6-23
  Description: proj depend parameter define       
  Version:                        
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
***********************************************************/
#ifndef __BC_PRINT_SRV_H__
#define __BC_PRINT_SRV_H__

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              ͷ   ��   ��                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include "bc_print.h"




////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                      �� �� �� ���� �� �� �� ��                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              ��   ��   ��                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  �� �� �� ���� �� �塢ö �� �� ��                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                          ȫ �� �� �� �� ��                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
/* 
 *  �ⲿȫ�ֱ������ṩ�������̵��ã���ͷ�ļ����ṩ���ýӿ�(�ⲿ��������)��
 */

/* 
 *  �ڲ�ȫ�ֱ�����ֻ�ṩ�����ļ��ڲ��������ã�����ͷ�ļ����ṩ���ýӿڡ�
 */



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              ��  ��  ��  ��                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



/*************************************************
  Function: bc_pt_prt_open_all_srv
  Description: �����д�ӡ����(���������д�ӡ
  				 ����ʹ�����ģ��),�����
  Input: 
  		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_prt_open_all_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_prt_close_all_srv
  Description: �����ӡ���ã�ȫ�ر�(�ر����еĴ�
  				 ӡ�����ģ��)
  Input: 
  		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_prt_close_all_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_prt_lvl_open_srv
  Description: �򿪴�ӡ�ĸü���(λ)
  Input: 
  		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_prt_lvl_open_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_prt_lvl_close_srv
  Description: �رմ�ӡ�ĸü���(λ)
  Input: 
  		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_prt_lvl_close_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_open_prj_all_srv
  Description: �򿪴�ӡ��������(������ӡ���е�ģ��)
  Input: 
  		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_open_prj_all_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_open_prj_mdl_srv
  Description: ��ĳ��ģ�������ļ�
  Input: 
  		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_open_prj_mdl_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_open_prj_mdl_file_srv
  Description: ��ĳ��ģ���ĳ���ļ�
  Input: 
   		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_open_prj_mdl_file_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_close_prj_all_srv
  Description: �ر���������(����ģ��������ļ�)��
  				 ��ӡ
  Input: 
   		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_close_prj_all_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_close_prj_mdl_srv
  Description: �ر�ĳ��ģ��������ļ��Ĵ�ӡ
  Input: 
   		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_close_prj_mdl_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_close_prj_mdl_file_srv
  Description: �ر�ĳ��ģ���ĳ���ļ��Ĵ�ӡ
  Input: 
   		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_close_prj_mdl_file_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_get_prt_lvl_srv
  Description: ��ȡ��ӡ�Ĵ�ӡ����
  Input: 
   		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_get_prt_lvl_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_get_prj_all_srv
  Description: ��ȡ��������(����ģ��)������Ϣ
  Input: 
   		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  History: 
*************************************************/
bc_err_e bc_pt_get_prj_all_srv(bc_ipc_package_t *data);




/*************************************************
  Function: bc_pt_dbg_save_cfg_srv
  Description: ��������������ݵ��ļ�
  Input: 
   		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_dbg_save_cfg_srv(bc_ipc_package_t *data);



/*************************************************
  Function: bc_pt_dbg_save_clear_srv
  Description: ������������ļ����ڴ��������
  Input: 
   		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_dbg_save_clear_srv(bc_ipc_package_t *data);


/*************************************************
  Function: bc_pt_dump_srv
  Description: dump�����ڴ�����
  Input: 
   		data	IPC����
  Output:
  		NULL
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
bc_err_e bc_pt_dump_srv(bc_ipc_package_t *data);




#endif		/* __BC_PRINT_SRV_H__ */
