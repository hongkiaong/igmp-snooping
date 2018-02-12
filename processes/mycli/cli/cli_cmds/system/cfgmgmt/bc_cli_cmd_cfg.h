/************************************************************
  -------------------------------------------------------------------------
  FileName: bc_cli_cmd_cfg.h
  Author:  bain.wang@outlook.com      Version :  1.0        Date:2016-8-15
  Description: proj depend parameter define       
  Version:                        
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
***********************************************************/
#ifndef __BC_CLI_CMD_CFG_H__
#define __BC_CLI_CMD_CFG_H__



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              ͷ   ��   ��                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include "bc_cli_type.h"


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
  Function: bc_cli_cmd_cfg_reg
  Description: �����ļ�������ע��ӿ�
  Input:   
  		void
  Output:
  		NULL
  Return:
  		void
  Note: 
  History: 
*************************************************/
void bc_cli_cmd_cfg_reg(void);



/*************************************************
  Function: bc_cfg_load_config_file
  Description: �����û������ļ�
  Input:   
  		filename		�ļ���
  		flag			���ر��
  Output:
  		NULL
  Return:
  		void
  Note: 
  History: 
*************************************************/
void bc_cfg_load_config_file(I8_T *filename, BOOL_T flag);








#endif		/* __BC_CLI_CMD_CFG_H__ */

