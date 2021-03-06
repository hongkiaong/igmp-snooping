/*******************************************************************************
  -------------------------------------------------------------------------
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
	2016-07-31  Created									bain.wang@outlook.com
*******************************************************************************/

#ifndef __BC_BINCHIE_DEF_H__
#define __BC_BINCHIE_DEF_H__

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								头   文   件								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#include "bc_err.h"
#include "bc_binchie_def.h"
#include "bc_modules_com_para.h"

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								宏   定   义								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



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

#define BC_BINCHIE_OP_PWD		"bain.wang@outlook.com"

#define BC_BINCHIE_TEL_LEN_MAX		12
#define BC_BINCHIE_QQ_LEN_MAX		12
#define BC_BINCHIE_FAX_LEN_MAX		12
#define BC_BINCHIE_JOB_LEN_MAX		12
#define BC_BINCHIE_EMAIL_LEN_MAX	32
#define BC_BINCHIE_WECHAT_LEN_MAX	32

#define BC_BINCHIE_NAME_LEN_MAX		32
#define BC_BINCHIE_ADDR_LEN_MAX		128


#define BC_BINCHIE_DEVELOPER_USER_NUM_MAX		1
#define BC_BINCHIE_TEST_USER_NUM_MAX			5
#define BC_BINCHIE_VINDICATOR_USER_NUM_MAX		10
#define BC_BINCHIE_USER_NUM_MAX					(BC_BINCHIE_DEVELOPER_USER_NUM_MAX+BC_BINCHIE_TEST_USER_NUM_MAX+BC_BINCHIE_VINDICATOR_USER_NUM_MAX)

#define BC_BINCHIE_DEVELOPER_IDX				0

#define BC_BINCHIE_DEVELOPER_NAME_DEF			"WangBin"
#define BC_BINCHIE_DEVELOPER_SEX_DEF			BC_BINCHIE_SEX_MAN
#define BC_BINCHIE_DEVELOPER_AGE_DEF			31
#define BC_BINCHIE_DEVELOPER_ADDR_DEF			"ChengDu"
#define BC_BINCHIE_DEVELOPER_CONTACT_TEL_DEF	"138xxxx4828"
#define BC_BINCHIE_DEVELOPER_CONTACT_FAX_DEF	"empty"
#define BC_BINCHIE_DEVELOPER_CONTACT_JOB_DEF	"empty"
#define BC_BINCHIE_DEVELOPER_CONTACT_WECHAT_DEF	"138xxxx4828"
#define BC_BINCHIE_DEVELOPER_CONTACT_EMAIL_DEF	"bain.wang@outlook.com"
#define BC_BINCHIE_DEVELOPER_CONTACT_QQ_DEF		"136335455"

typedef enum{
	BC_BINCHIE_SEX_MAN = 0,		/*男*/
	BC_BINCHIE_SEX_WOMAN,		/*女*/
	BC_BINCHIE_SEX_VALID
}bc_binchie_sex_e;

typedef enum{
	BC_BINCHIE_DUTY_DEVELOPER = 0,		/*开发者*/
	BC_BINCHIE_DUTY_TEST,				/*测试者*/
	BC_BINCHIE_DUTY_VINDICATOR,			/*维护者*/
	BC_BINCHIE_DUTY_DEVEP_TEST,			/*开发者与测试者*/
	BC_BINCHIE_DUTY_DEVEP_VINDI,		/*开发者与维护者*/
	BC_BINCHIE_DUTY_TEST_VINDI,			/*测试者与维护者*/
	BC_BINCHIE_DUTY_DEVEP_TEST_VINDI,	/*开发者与测试者与维护者*/
	BC_BINCHIE_DUTY_OTHER,				/*其它人员，如使用者*/
	BC_BINCHIE_DUTY_INVALID
}bc_binchie_duty_e;

typedef struct{
	char tel[BC_BINCHIE_TEL_LEN_MAX];			/*手机*/
	char qq[BC_BINCHIE_QQ_LEN_MAX];				/*QQ */
	char fax[BC_BINCHIE_FAX_LEN_MAX];			/*传真*/
	char email[BC_BINCHIE_EMAIL_LEN_MAX];		/*邮箱*/
	char wechat[BC_BINCHIE_WECHAT_LEN_MAX];		/*微信*/
	char job[BC_BINCHIE_JOB_LEN_MAX];			/*工号*/
}bc_binchie_contact_t;

typedef struct{
	char name[BC_BINCHIE_NAME_LEN_MAX];	/*姓名*/
	bc_binchie_sex_e sex;						/*性别*/
	int age;									/*年龄*/
	char addr[BC_BINCHIE_ADDR_LEN_MAX];			/*地址*/
	bc_binchie_duty_e duty;						/*职责*/
	bc_binchie_contact_t contact;				/*联系方式*/
	bc_boolean is_used;							/*此条信息是否有效*/
	bc_boolean is_continued;					/*是否依然维护中*/
}bc_binchie_info_t;


#endif /* __BC_BINCHIE_DEF_H__ */






