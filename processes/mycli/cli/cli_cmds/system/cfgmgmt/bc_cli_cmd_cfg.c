/************************************************************
  -------------------------------------------------------------------------
  FileName: bc_cli_cmd_cfg.c
  Author:  bain.wang@outlook.com      Version :  1.0        Date:2016-8-15
  Description: proj depend parameter define       
  Version:                        
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
***********************************************************/



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              头   文   件                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include "bc_cli.h"
#include "bc_cli_type.h"
#include "bc_cli_cmd_cfg.h"
#include "bc_cli_paratype.h"
#include "bc_cli_paratype.h"
#include "bc_cli_pseudo.h"
#include "bc_cli_text.h"
#include "bc_cli_base.h"
#include "bc_cli_type.h"
#include "bc_cli_vkey.h"
#include "bc_cli.h"

#include "bc_msg.h"
#include "bc_vfs.h"
#include "bc_print.h"
#include "bc_cfg_mgmt.h"
#include "bc_platform_mem.h"
#include "bc_cfg_mgmt_client.h"
//#include "bc_port_bitmap.h"



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                      外 部 变 量、 函 数 引 用                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//extern bc_boolean bc_cli_cmd_port_str_to_bmp(char *str, bc_port_bmp bmp);


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              宏   定   义                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#define BC_CFG_FILE_NAME_LEN	32  
#define BC_CFG_FILE_NUM			10

#define CLI_CFG_GET_LAN(lan)	\
	do{\
		lan = (bc_lan_e)bc_cli_get_lan();\
	}while(0)



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  结 构 体 、联 合 体、枚 举 定 义                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                          全 局 变 量 定 义                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
/* 
 *  外部全局变量，提供整个工程调用，在头文件中提供调用接口(外部引用声明)。
 */

/* 
 *  内部全局变量，只提供给该文件内部函数调用，不在头文件中提供调用接口。
 */
static bc_modules_e l_mdl_id = BC_MODULE_CFGMGMT;



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              函  数  声  明                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#if 1
static I32_T __save_cfg_file
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	);
static I32_T __show_running_cfg
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	);

static I32_T __clear_config_file
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	);

static I32_T __show_cfg_file_list
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	);

static I32_T __set_boot_load_config_file
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	);

static I32_T __no_set_boot_load_config_file
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	);

static I32_T __dump_cfgmgmt_info
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	);

static I32_T __config_module_reg_dbg
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	);

#endif



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              函  数  定  义                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/*************************************************
  Function: bc_cli_cmd_cfg_reg
  Description: 配置文件的命令注册接口
  Input:   
  		void
  Output:
  		NULL
  Return:
  		void
  Note: 
  History: 
*************************************************/
void bc_cli_cmd_cfg_reg(void)
{
	BOOL_T    ret_reg = 0;

	CLI_DBG_ENTER(l_mdl_id);

	/* 特权模式注册 */
	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_PRIVI_PATH
		"/write "
		"[ "
			"<string> "
		"] "
		,
		__save_cfg_file,
		CLI_ACC_LVL_USER,
		"",
		"",
		NULL
		);

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	/* 全局模式注册 */
	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_GLBCFG_PATH
		"/write "
		"[ "
			"<string> "
		"] "
		,
		__save_cfg_file,
		CLI_ACC_LVL_USER,
		"",
		"",
		NULL
		);

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_GLOBAL_PATH
		"/show "
		"running-config "	"#running config info#""#系统运行着的配置信息#"
		"[ "
		"{ "
			"<port-group> "
#if BC_PROJ_DEFS_CFG_EPON
			"| "
			"onu "
			"<onu-id> "
#endif
		"} "
		"] "
		,
		__show_running_cfg,
		CLI_ACC_LVL_USER,
		"",
		"",
		NULL
		);

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	/* 特权模式注册 */
	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_PRIVI_PATH
		"/clear "
		"config-file "						"#config file#""#配置文件#"
		"[ "
			"<string> "
		"] "
		,
		__clear_config_file,
		CLI_ACC_LVL_USER,
		"clear config file",
		"清除配置文件",
		NULL
		);

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	/* 全局模式注册 */
	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_GLBCFG_PATH
		"/clear "
		"config-file "						"#config file#""#配置文件#"
		"[ "
			"<string> "
		"] "
		,
		__clear_config_file,
		CLI_ACC_LVL_USER,
		"clear config file",
		"清除配置文件",
		NULL
		);

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_GLOBAL_PATH
		"/show "
		"config-file "					"#config file#""#配置文件#"
		"list "							"#file list#""#文件列表#"
		, 
		__show_cfg_file_list, 
		CLI_ACC_LVL_USER, 
		"show config file list", 
		"显示配置文件列表", 
		NULL
		);

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	/* 特权模式注册 */
	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_PRIVI_PATH
		"/boot-load "					"#boot load#""#开机加载#"
		"config-file "					"#configuration file#""#配置文件#"
		"<string> "
		, 
		__set_boot_load_config_file, 
		CLI_ACC_LVL_USER, 
		"set boot load config file", 
		"设置开机加载配置文件", 
		NULL
		);

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	/* 全局模式注册 */
	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_GLBCFG_PATH
		"/boot-load "					"#boot load#""#开机加载#"
		"config-file "					"#configuration file#""#配置文件#"
		"<string> "
		, 
		__set_boot_load_config_file, 
		CLI_ACC_LVL_USER, 
		"set boot load config file", 
		"设置开机加载配置文件", 
		NULL
		);

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	/* 特权模式注册 */
	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_PRIVI_PATH
		"/no "
		"boot-load "					"#boot load#""#开机加载#"
		"config-file "					"#configuration file#""#配置文件#"
		, 
		__no_set_boot_load_config_file, 
		CLI_ACC_LVL_USER, 
		"no set boot load config file", 
		"设置开机加载配置文件", 
		NULL
		);

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	/* 全局模式注册 */
	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_GLBCFG_PATH
		"/no"
		"boot-load "					"#boot load#""#开机加载#"
		"config-file "					"#configuration file#""#配置文件#"
		, 
		__no_set_boot_load_config_file, 
		CLI_ACC_LVL_USER, 
		"no set boot load config file", 
		"设置开机加载配置文件", 
		NULL
		);

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	/* debug模式 */
	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_DEBUG_PATH
		"/dump "						"#dump info#""#导出信息#"
		"cfgmgmt-info "					"#configuration info#""#配置信息#"
		,
		__dump_cfgmgmt_info,
		CLI_ACC_LVL_ADMIN,
		"dump configure info",
		"打印配置信息",
		NULL
		);

	ret_reg = bc_cli_reg_cmd(
		CLI_MOD_DEBUG_PATH
		"/cfgmgmt "						"#cfgmgmt info#""#cfgmgmt信息#"
		"module-register "				"#module register#""#模块注册#"
		"{ "
			"test-fun-1(1) "
			"| "
			"test-fun-2(2) "
			"| "
			"test-fun-3(3) "
			"| "
			"test-fun-4(4) "
			"| "
			"test-fun-5(5) "
		"} "
		"<module-id> "
		"<value 1-13> "
		"<value 1-64> "
		,
		__config_module_reg_dbg,
		CLI_ACC_LVL_ADMIN,
		"dump configure info",
		"打印配置信息",
		NULL
		);
	

	CLI_DBG_CPL(l_mdl_id, "ret_reg = %d\n", ret_reg);

	CLI_DBG_LEAVE(l_mdl_id);
}



/*************************************************
  Function: __save_cfg_file
  Description: 
  Input:   
  		sid					session id
  		ptrCliValue			命令行参数
  		ptrUserData			自定义用户数据
  Output:
  		NULL
  Return:
  		0: success
  		~0: fail
  Note: 
  History: 
*************************************************/
static I32_T __save_cfg_file
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
	I32_T trigger[] =
    {
        CLI_VKEY_CTRL_ENTER, CLI_VKEY_CR, 0
    };
	bc_char in_buf[1024] = {0};
	bc_err_e ret = BC_ERR_OK;
	cli_value_t *tmp_cmd_para = NULL;
	I8_T file_name[BC_CFG_FILE_NAME_LEN] = {0};
	cli_lan_t lan = CLI_LAN_EN;
	bc_cfg_file_info_t file_list[BC_CFG_LIST_MAX_NUM];
	bc_uint8 loop_cnt = 0;
	bc_uint32 actual_num = 0;

	BC_PT_COMMAND(l_mdl_id, "This will save the configuration to the flash memory.\r\n"); 
	BC_PT_COMMAND(l_mdl_id, "Are you sure?(y/n)\r\n");

	bc_cli_get_line(sid, 0, CLI_OUTPUT_NORMAL, in_buf, sizeof(in_buf), 60, trigger);
	if (!((1 == strlen(in_buf)) && (in_buf[0] == 'y')))
	{
		if ((1 == strlen(in_buf)) && (in_buf[0] == 'n'))
		{
			BC_PT_COMMAND(l_mdl_id, "\r\n");
			return CLI_CMD_FAIL_PRT_NEWLINE;
		}
		else
		{
			BC_PT_COMMAND(l_mdl_id, "\r\nInput character error, failed to save the user configuration.\r\n");
			return CLI_CMD_FAIL_PRT_NEWLINE;
		}
	}

	BC_PT_COMMAND(l_mdl_id, "\r\n");
	lan = bc_cli_get_lan();

	tmp_cmd_para = ptrCliValue;
	if (NULL == tmp_cmd_para)
	{
		ret = bc_cfg_save_client(NULL);
		if (BC_ERR_OK != ret)
		{
			BC_PT_DBG_CPL(l_mdl_id, "config save ipc error! ret = 0x%x\n", ret);
			return CLI_CMD_FAIL_PRT_NEWLINE;
		}
	}
	else
	{
		strncpy(file_name, (I8_T *)tmp_cmd_para->value, BC_CFG_FILE_NAME_LEN - 1);
		file_name[BC_CFG_FILE_NAME_LEN - 1] = '\0';
		BC_PT_DBG_CPL(l_mdl_id, "file_name: %s, strlen(file_name): %d\n", file_name, strlen(file_name));

		memset(file_list, 0x00, sizeof(bc_cfg_file_info_t) * BC_CFG_LIST_MAX_NUM);
		ret = bc_cfg_get_cfglist_client(file_list, &actual_num);
		if (ret != BC_ERR_OK)
		{
			CLI_API_Printf(sid, " %s\n", bc_msg_err_get(ret, (bc_lan_e)lan));
			return CLI_CMD_FAIL_PRT_NEWLINE;
		}

		for (loop_cnt = 0; loop_cnt < actual_num; loop_cnt++)
		{
			if (0 == strcmp(file_name, file_list[loop_cnt].name))
			{
				CLI_API_Printf(sid, " %s\n", bc_msg_err_get(BC_ERR_ALREADY_EXSIT, (bc_lan_e)lan));
				return CLI_CMD_FAIL_PRT_NEWLINE;
			}
		}
		
		ret = bc_cfg_save_client(file_name);
		if (BC_ERR_OK != ret)
		{
			if (BC_ERR_CFGMGMT_FILE_NUM_OUT == ret)
			{
				CLI_API_Printf(sid, " %s\n", bc_msg_err_get(ret, (bc_lan_e)lan));
				return CLI_CMD_FAIL_PRT_NEWLINE;
			}
			
			CLI_API_Printf(sid, " %s\n", bc_msg_err_get(ret, (bc_lan_e)lan));
			return CLI_CMD_FAIL_PRT_NEWLINE;
		}
	}	

	BC_PT_DBG_CPL(l_mdl_id, "config file save ok!\n");
	return CLI_CMD_SUCCESS_PRT_NEWLINE;
}



/*************************************************
  Function: __show_running_cfg
  Description: 
  Input:   
  		sid					session id
  		ptrCliValue			命令行参数
  		ptrUserData			自定义用户数据
  Output:
  		NULL
  Return:
  		0: success
  		~0: fail
  Note: 
  History: 
*************************************************/
static I32_T __show_running_cfg
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
/*
	bc_char parse_str[128] = {0};
	bc_port_bmp bmp;
	bc_lan_e lan;
	bc_port_index_t portlist[BC_PORT_MAX_COUNT];
	bc_int32 port_num = 0;
	cli_onuid_t onu;
	bc_onu_index_t onulist[BC_ONU_MAX_ONU_COUNTS];
	bc_int32 onu_num = 0;
	bc_cfg_run_t run_para;
	
	bc_err_e ret = BC_ERR_OK;
	bc_file *fp_run = NULL;
	bc_char *run_cfg_buf = NULL;

	CLI_CFG_GET_LAN(lan);
	memset(&run_para, 0x00, sizeof(bc_cfg_run_t));
	if (ptrCliValue)
	{
		if (ptrCliValue->next)
		{
			ptrCliValue = ptrCliValue->next;
			memset(&onu, 0x00, sizeof(cli_onuid_t));
			memset(onulist, 0x00, sizeof(bc_onu_index_t)*BC_ONU_MAX_ONU_COUNTS);
			memcpy(&onu, (cli_onuid_t *)ptrCliValue->value, sizeof(cli_onuid_t));
			onulist[0].slotid = onu.slot_id;
			onulist[0].ponid = onu.port_id;
			onulist[0].llid = onu.llid;
			onu_num = 1;
			run_para.para_enable = TRUE;
			run_para.onu_num = onu_num;
			memcpy(run_para.onulist, onulist, sizeof(bc_onu_index_t)*BC_ONU_MAX_ONU_COUNTS);
		}
		else
		{
			memcpy(parse_str, (I8_T *)ptrCliValue->value, 128);
			memset(bmp, 0x00, sizeof(bc_port_bmp));
			if (FALSE == bc_cli_cmd_port_str_to_bmp(parse_str, bmp))//already check
			{
				CLI_API_Printf(sid, " %s\n", bc_msg_err_get(BC_ERR_PORT_INVALID, lan));
				return CLI_CMD_FAIL_PRT_NEWLINE;
			}			
			memset(portlist, 0x00, sizeof(bc_port_index_t)*BC_PORT_MAX_COUNT);
			bc_port_bmp_to_portlist(bmp, portlist, &port_num);
			if (0 == port_num) 
		    {
		        BC_PT_ERROR(l_mdl_id, "Portlist invalid, num:%d\r\n", port_num);
		        return CLI_CMD_FAIL_PRT_NEWLINE;
		    }

			if (((PORT_TYPE_OLT_GEEPON == portlist[0].port_type) && (BC_PORT_PON_MAX_COUNT == port_num))
				|| ((PORT_TYPE_OLT_GESNI == portlist[0].port_type) && (BC_PORT_SNI_MAX_COUNT == port_num)))
			{
				run_para.all_port_enable = TRUE;
			}

			run_para.para_enable = TRUE;
			run_para.port_num = port_num;
			memcpy(run_para.portlist, portlist, sizeof(bc_port_index_t)*BC_PORT_MAX_COUNT);
		}
	}

	BC_PT_DBG_CPL(l_mdl_id, "para_enable:%d, all_port_enable:%d, port_num:%d, portlist[0].port_type:%d\n", 
		  run_para.para_enable, run_para.all_port_enable, run_para.port_num, run_para.portlist[0].port_type);
	BC_PT_DBG_CPL(l_mdl_id, "onu_num: %d\n", run_para.onu_num);
	if (BC_ERR_OK != (ret = bc_cfg_get_run_client(&run_para)))
	{
		BC_PT_ERROR(l_mdl_id, " %s\n", bc_msg_err_get(ret, lan));
		return CLI_CMD_FAIL_PRT_NEWLINE;
	}
	
	fp_run = bc_file_open(BC_CFG_RUNNING_CFG, BC_VFS_READ, BC_VFS_ROOT_F, l_mdl_id);
	if (NULL == fp_run)
	{
		BC_PT_ERROR(l_mdl_id, "open running cfg file failed!\n");
		return CLI_CMD_FAIL_PRT_NEWLINE;
	}

	run_cfg_buf = bc_platform_mem_malloc(l_mdl_id, BC_CFG_USR_CFG_SIZE);
	if (NULL == run_cfg_buf)
	{
		BC_PT_ERROR(l_mdl_id, "memory allocation failed!\n");
		return CLI_CMD_FAIL_PRT_NEWLINE;
	}

	bc_file_read(run_cfg_buf, BC_CFG_USR_CFG_SIZE, 1, fp_run);
	bc_file_close(fp_run, l_mdl_id, BC_CFG_RUNNING_CFG);
	
	BC_PT_DBG_CPL(l_mdl_id, "strlen(run_cfg_buf) = %d\n", strlen(run_cfg_buf));
	BC_PT_COMMAND(l_mdl_id, "%s\r\n", run_cfg_buf);

	bc_platform_mem_free(l_mdl_id, run_cfg_buf);
	run_cfg_buf = NULL;
*/
	return CLI_CMD_SUCCESS_PRT_NEWLINE;
}



/*************************************************
  Function: __clear_config_file
  Description: 
  Input:   
  		sid					session id
  		ptrCliValue			命令行参数
  		ptrUserData			自定义用户数据
  Output:
  		NULL
  Return:
  		0: success
  		~0: fail
  Note: 
  History: 
*************************************************/
static I32_T __clear_config_file
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
	I32_T trigger[] =
    {
        CLI_VKEY_CTRL_ENTER, CLI_VKEY_CR, 0
    };
	bc_char in_buf[1024] = {0};
	UI8_T               loop_cnt = 0;
	UI32_T				actual_num = 0;
	bc_err_e            ret = BC_ERR_OK;
	cli_value_t     	*tmp_cmd_arg = NULL;
	bc_cfg_file_info_t	file_list[BC_CFG_FILE_NUM];
	I8_T				file_name[BC_CFG_FILE_NAME_LEN] = {0};
	
	CLI_DBG_ENTER(l_mdl_id);

	BC_PT_COMMAND(l_mdl_id, "This will clear the configuration file from the flash memory.\r\n"); 
	BC_PT_COMMAND(l_mdl_id, "Are you sure?(y/n)\r\n");

	bc_cli_get_line(sid, 0, CLI_OUTPUT_NORMAL, in_buf, sizeof(in_buf), 60, trigger);
	if (!((1 == strlen(in_buf)) && (in_buf[0] == 'y')))
	{
		if ((1 == strlen(in_buf)) && (in_buf[0] == 'n'))
		{
			BC_PT_COMMAND(l_mdl_id, "\r\n");
			return CLI_CMD_FAIL_PRT_NEWLINE;
		}
		else
		{
			BC_PT_COMMAND(l_mdl_id, "\r\nInput character error, failed to clear the user configuration.\r\n");
			return CLI_CMD_FAIL_PRT_NEWLINE;
		}
	}

	BC_PT_COMMAND(l_mdl_id, "\r\n");
	
	tmp_cmd_arg = ptrCliValue;
	memset(file_list, 0x00, sizeof(bc_cfg_file_info_t) * BC_CFG_FILE_NUM);
	ret = bc_cfg_get_cfglist_client(file_list, &actual_num);
	if (ret != BC_ERR_OK)
	{
		BC_PT_DBG_CPL(l_mdl_id, "get config file list client error! ret = 0x%x\n", ret);
		return CLI_CMD_FAIL_PRT_NEWLINE;
	}

	BC_PT_DBG_CPL(l_mdl_id, "actual_num = %d\n", actual_num);
	if (NULL == tmp_cmd_arg)
	{
		for (loop_cnt = 0; loop_cnt < actual_num; loop_cnt++)
		{
			if ((ret = bc_cfg_clr_client(file_list[loop_cnt].name)) != BC_ERR_OK)
			{
				BC_PT_DBG_CPL(l_mdl_id, "clear config file ipc client error! ret = 0x%x\n", ret);
				return CLI_CMD_FAIL_PRT_NEWLINE;
			}
		}
	}
	else
	{
		strncpy(file_name, (I8_T *)tmp_cmd_arg->value, BC_CFG_FILE_NAME_LEN - 1);
		BC_PT_DBG_CPL(l_mdl_id, "file_name: %s, strlen(file_name): %d\n", file_name, strlen(file_name));

		for (loop_cnt = 0; loop_cnt < actual_num; loop_cnt++)
		{
			if (0 == strcmp(file_list[loop_cnt].name, file_name))
			{
				if ((ret = bc_cfg_clr_client(file_name)) != BC_ERR_OK)
				{
					BC_PT_DBG_CPL(l_mdl_id, "clear config file client error! ret = 0x%x\n", ret);
					return CLI_CMD_FAIL_PRT_NEWLINE;
				}
			}
		}
	}

	CLI_DBG_LEAVE(l_mdl_id);
	return CLI_CMD_SUCCESS_PRT_NEWLINE;
}



/*************************************************
  Function: __show_cfg_file_list
  Description: 
  Input:   
  		sid					session id
  		ptrCliValue			命令行参数
  		ptrUserData			自定义用户数据
  Output:
  		NULL
  Return:
  		0: success
  		~0: fail
  Note: 
  History: 
*************************************************/
static I32_T __show_cfg_file_list
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
	bc_err_e       		api_ret = BC_ERR_OK;
	bc_uint32			num = 0;
	bc_uint32 			actual_num = 0;
	bc_cfg_file_info_t	file_list[BC_CFG_LIST_MAX_NUM];

	CLI_DBG_ENTER(l_mdl_id);

	memset(file_list, 0x00, sizeof(bc_cfg_file_info_t) * BC_CFG_LIST_MAX_NUM);
	api_ret = bc_cfg_get_cfglist_client(file_list, &actual_num);
	if (BC_ERR_OK != api_ret)
	{
		BC_PT_DBG_CPL(l_mdl_id, "get config file list ipc error!\r\n");
		return CLI_CMD_FAIL_PRT_NEWLINE;
	}

	BC_PT_DBG_CPL(l_mdl_id, "actual_num = %d\r\n", actual_num);
	if (actual_num <= 0)
		return CLI_CMD_FAIL_PRT_NEWLINE;

	BC_PT_COMMAND(l_mdl_id, "\n");
	BC_PT_COMMAND(l_mdl_id, "%-4s  %-32s %-10s %-19s\n",  
				CMD_GET_STRING(CMD_MID_NUM), CMD_GET_STRING(CMD_MID_FILE_NAME), 
				CMD_GET_STRING(CMD_MID_FILE_SIZE), CMD_GET_STRING(CMD_MID_FILE_CREATE_DATA));
	BC_PT_COMMAND(l_mdl_id, "%-4s  %-32s %-10s %-19s\n", "----", "--------------------------------", "----------", "-------------------");	
	for (num = 0; num < actual_num; num++)
	{
		BC_PT_COMMAND(l_mdl_id, "%-4d  %-32s %-10d %-19s\r\n", num + 1, file_list[num].name, file_list[num].size, file_list[num].date);
	}

	CLI_DBG_LEAVE(l_mdl_id);
	return CLI_CMD_SUCCESS_PRT_NEWLINE;
}



/*************************************************
  Function: __set_boot_load_config_file
  Description: 
  Input:   
  		sid					session id
  		ptrCliValue			命令行参数
  		ptrUserData			自定义用户数据
  Output:
  		NULL
  Return:
  		0: success
  		~0: fail
  Note: 
  History: 
*************************************************/
static I32_T __set_boot_load_config_file
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
	I8_T				boot_file[BC_CFG_BOOT_NAME_LEN] = {0};
	UI32_T				actual_num 						= 0;
	UI32_T				cyc_count 						= 0;
	bc_cfg_file_info_t	file_list[BC_CFG_LIST_MAX_NUM];

	CLI_DBG_ENTER(l_mdl_id);

	BC_PT_DBG_CPL(l_mdl_id, "(I8_T *)ptrCliValue->value: %s\r\n", (I8_T *)ptrCliValue->value);
	strncpy(boot_file, (I8_T *)ptrCliValue->value, BC_CFG_BOOT_NAME_LEN);
	BC_PT_DBG_CPL(l_mdl_id, "boot_file: %s, strlen(boot_file) = %d\r\n", boot_file, strlen(boot_file));

	memset(file_list, 0x00, sizeof(bc_cfg_file_info_t));
	if (BC_ERR_OK != bc_cfg_get_cfglist_client(file_list, &actual_num))
	{
		BC_PT_DBG_CPL(l_mdl_id, "get config file list ipc error!\r\n");
		return CLI_CMD_FAIL_PRT_NEWLINE;
	}

	BC_PT_DBG_CPL(l_mdl_id, "actual_num = %d\r\n", actual_num);
	for (cyc_count = 0; cyc_count < actual_num; cyc_count++)
	{
		if (0 == strcmp(file_list[cyc_count].name, boot_file))
		{
			if (BC_ERR_OK != bc_cfg_set_boot_load_file_client(boot_file))
			{
				BC_PT_DBG_CPL(l_mdl_id, "set boot file name ipc error!\r\n");
				return CLI_CMD_FAIL_PRT_NEWLINE;
			}

			CLI_DBG_LEAVE(l_mdl_id);
			return CLI_CMD_SUCCESS_PRT_NEWLINE;
		}
	}

	BC_PT_COMMAND(l_mdl_id, "input boot file name error! no match!\r\n");
	return CLI_CMD_FAIL_PRT_NEWLINE;
}



/*************************************************
  Function: __no_set_boot_load_config_file
  Description: 
  Input:   
  		sid					session id
  		ptrCliValue			命令行参数
  		ptrUserData			自定义用户数据
  Output:
  		NULL
  Return:
  		0: success
  		~0: fail
  Note: 
  History: 
*************************************************/
static I32_T __no_set_boot_load_config_file
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{	
	CLI_DBG_ENTER(l_mdl_id);

	if (BC_ERR_OK != bc_cfg_no_boot_load_file_client())
	{
		BC_PT_DBG_CPL(l_mdl_id, "delete boot name file ipc client error!\r\n");
		return CLI_CMD_FAIL_PRT_NEWLINE;
	}

	CLI_DBG_LEAVE(l_mdl_id);
	return CLI_CMD_SUCCESS_PRT_NEWLINE;
}



/*************************************************
  Function: __dump_cfgmgmt_info
  Description: 
  Input:   
  		sid					session id
  		ptrCliValue			命令行参数
  		ptrUserData			自定义用户数据
  Output:
  		NULL
  Return:
  		0: success
  		~0: fail
  Note: 
  History: 
*************************************************/
static I32_T __dump_cfgmgmt_info
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
	CLI_DBG_ENTER(l_mdl_id);

	if (BC_ERR_OK != bc_cfg_dbg_dump_client())
	{
		BC_PT_DBG_CPL(l_mdl_id, "configure debug dump ipc error!\r\n");
		return CLI_CMD_FAIL_PRT_NEWLINE;
	}

	CLI_DBG_LEAVE(l_mdl_id);

	return CLI_CMD_SUCCESS_PRT_NEWLINE;
}


static I32_T __config_module_reg_dbg
	(
		IN const UI32_T 	sid,
	    IN cli_value_t 		*ptrCliValue,
	    IN void 			*ptrUserData	
	)
{
	cli_value_t *tmp_cli_arg = ptrCliValue;
	bc_uint32 fun_id = 0;
	bc_modules_e module_id = 0;
	bc_cfg_type_e mode = 0;
	bc_uint32 mdl_pri = 0;
	bc_err_e ret = BC_ERR_OK;
	bc_lan_e lan = BC_LAN_ENGLISH_E;

	if (!tmp_cli_arg)
		return CLI_CMD_FAIL_PRT_NEWLINE;

	fun_id = *(bc_uint32 *)tmp_cli_arg->value;
	BC_PT_DBG_CPL(l_mdl_id, "fun_id = %d\r\n", fun_id);

	tmp_cli_arg = tmp_cli_arg->next;
	module_id = *(bc_uint32 *)tmp_cli_arg->value;
	BC_PT_DBG_CPL(l_mdl_id, "module_id = 0x%x\r\n", module_id);

	tmp_cli_arg = tmp_cli_arg->next;
	mode = *(bc_uint32 *)tmp_cli_arg->value;

	tmp_cli_arg = tmp_cli_arg->next;
	mdl_pri = *(bc_uint32 *)tmp_cli_arg->value;

	CLI_CFG_GET_LAN(lan);
	BC_PT_DBG_CPL(l_mdl_id, "mode = %d, mdl_pri = %d\r\n", mode, mdl_pri);
	ret = bc_cfg_dbg_mdl_ins(fun_id, module_id, mode, mdl_pri);
	if (ret != BC_ERR_OK)
	{
		BC_PT_ERROR(l_mdl_id, "%s\n", bc_msg_err_get(ret, lan));
		return CLI_CMD_FAIL_PRT_NEWLINE;
	}
		
	return CLI_CMD_SUCCESS_PRT_NEWLINE;
}



#if 1

/*************************************************
  Function: bc_cfg_load_config_file
  Description: 加载用户配置文件
  Input:   
  		filename		文件名
  		flag			加载标记
  Output:
  		NULL
  Return:
  		void
  Note: 
  History: 
*************************************************/
void bc_cfg_load_config_file(I8_T *filename, BOOL_T flag)
{
	bc_uint32			actual_num = 0;
	bc_uint32			count      = 0;
	bc_cfg_file_info_t	file_list[BC_CFG_LIST_MAX_NUM];

	CLI_DBG_ENTER(l_mdl_id);


	if (TRUE == flag)
	{
		//命令行交互信息
	}
	else
	{
		//无交互信息
	}

	if (NULL != filename)
	{
		BC_PT_DBG_CPL(l_mdl_id, "filename: %s, strlen(filename) = %d\r\n", filename, strlen(filename));

		/* search load config file, execute config file command */
		memset(file_list, 0x00, sizeof(bc_cfg_file_info_t) * BC_CFG_LIST_MAX_NUM);
		if (BC_ERR_OK != bc_cfg_get_cfglist_client(file_list, &actual_num))
		{
			CLI_DBG_CPL(l_mdl_id, "get config file list ipc error!\r\n");
			return;
		}

		for (count = 0; count < actual_num; count++)
		{
			if (0 == strcmp(file_list[count].name, filename))
			{
				/* execute config file */
				//__run_execute_config_file(filename);
				break;
			}
		}
	}
	else
	{
		//__run_execute_config_file("bc_boot_usr_cfg");
	}

	CLI_DBG_LEAVE(l_mdl_id);
	return;
}



#endif




