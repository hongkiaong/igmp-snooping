
/*******************************************************************************
  Copyright (C), 1988-2012, xxxxx Tech. Co., Ltd.
  [File name]  : \\BC\bc_common_defs.h
  [Author]	   : wangbin
  [Version]    : 1.0
  [Date]	   : 2012-06-01
  [Description]:

  [Others]	   :

  [Function List]:	
	1. ....
	2. ....
  [History]:
	 D a t e	Modification										  Initials
	---------- --------------						  ----------
	2014-2-10    Modified                                 wangbin
	          ɾ��û���õ��ĸ��ֶ���
	2012-12-27  Modified 											  wangbin
				����ͳһ��ӡ
				ͳһ�ϲ� olt onu �����汾
	2012-09-27  Modified 											  wangbin
				������������
				���������ӡ���ã��Թ��ض���
	2012-06-28	Modified 											  wangbin
	            ���ӵ��Դ�ӡ�궨��
	2012-06-01  Created                                               yangdf
*******************************************************************************/

#ifndef __INC_COMMON_DEFS_H__
#define __INC_COMMON_DEFS_H__

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              ͷ   ��   ��                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#if 0

typedef void    RETSIGTYPE;

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h> 
#include <sys/param.h>
#include <syslog.h>
#include <time.h>
#include <sys/uio.h>
#include <sys/utsname.h>

/* misc include group */
#include <stdarg.h>
#include <assert.h>

/* network include group */
/*
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <asm/ioctls.h>
*/
#endif


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                  �� �� �� ���� �� �塢ö �� �� ��                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
/*
 * SYS_UINT8,SYS_UINT16��SYS_UINT32�ֱ��ʾ�޷���8λ��16λ��32λ����;
 * SYS_CHAR(SYS_INT8),SYS_INT16��SYS_INT32�ֱ��ʾ�з���8λ��16λ��32λ���͡�
 * ��׺_PTR��ʾΪָ������
 */

typedef unsigned long long        bc_uint64;
typedef long long                     bc_int64;
typedef unsigned int                 bc_uint32;
typedef int                               bc_int32;
typedef unsigned short              bc_uint16;
typedef short	                           bc_int16;
typedef unsigned char              bc_uint8;
typedef signed char	                bc_int8;
typedef char				                bc_char;
typedef double                  	      bc_double;
typedef unsigned char       		  bc_mac[6] ;
typedef unsigned long int          bc_ip;
typedef unsigned long int          bc_ipv4;
typedef unsigned int                 bc_ipv6[4];
typedef unsigned long 				  bc_ulong;
typedef long								  bc_long;

typedef int					               bc_boolean;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif


#ifndef NULL
#define NULL	0
#endif


#ifndef UNUSED
#define UNUSED(x)                   ((x) = (x))
#endif

#ifndef NOWARNING
#define NOWARNING(x)				do { if (x) {} } while (0)
#endif

#ifndef INVALID
#define INVALID                     (~0)
#endif



#endif /* __INC_COMMON_DEFS_H__ */
