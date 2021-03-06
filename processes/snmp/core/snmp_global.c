/*******************************************************************************
  [Author]	   : bain.wang@outlook.com.uestc@gmail.com
  [Version]    : 
  [Date]       : 
  [Description]:

  [Others]     :

  [History]:
	 Date          Modification 							    Initials
	----------     ----------                                   -------
	2017-02-22      Created								   		bain.wang@outlook.com
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//																			  //
//								头   文   件								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/snmp_vars.h>


#include <net-snmp/agent/agent_trap.h>
#include <net-snmp/agent/agent_callbacks.h>
#include <net-snmp/agent/table.h>
#include <net-snmp/agent/table_iterator.h>
#include <net-snmp/agent/table_data.h>
#include <net-snmp/agent/table_dataset.h>

#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>     
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "bc_common_defs.h"
#include "snmp_global.h"
#include "snmp_core_ex.h"



///////////////////////////////////////////////////////////////////////////////
//                                                               			 //
//																			 //
//						外 部 变 量、 函 数 引 用					         //
//																			 //
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//																			 //
//						结   构   体   宏   定   义							 //
//																			 //
///////////////////////////////////////////////////////////////////////////////


/*
 *		当源文件需要定义宏，而头文件又与其它模块头文件中的宏定义有重复定义嫌疑
 *	时，在此处定义。
 */


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//																			 //
//							全 局 变 量 定 义							     //
//																			 //
///////////////////////////////////////////////////////////////////////////////

/* 
 *	外部全局变量，提供整个工程调用，在头文件中提供调用接口(外部引用声明)。
 */

/* 
 *	内部全局变量，只提供给该文件内部函数调用，不在头文件中提供调用接口。
 */



///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//																			 //
//								函  数  声  明								 //
//																			 //
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//																			 //
//								函  数  定  义								 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
bc_boolean snmp_is_update(time_t *last_update_time)
{
	struct timeval tv;
	struct timezone tz;	
	gettimeofday(&tv, &tz);	
	
	if (difftime(tv.tv_sec, (*last_update_time)) < 0 
		|| difftime(tv.tv_sec, (*last_update_time)) >= SNMP_UPDATE_INTERVAL)
	{
		*last_update_time = tv.tv_sec;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bc_int32 snmp_get_addr_by_name(bc_char net_name[SNMP_DEV_NAME_LEN], bc_char ipstr[SNMP_IP_ADDR_LEN])
{
	int sock;
	struct   sockaddr_in   sin;   
	struct   ifreq   ifr;

	sock   =   socket(AF_INET,   SOCK_DGRAM,   0);   
	if   (sock   ==   -1)   
	{   
		perror("socket");   
		return   -1;   
	}   
	strncpy(ifr.ifr_name,   net_name,   IFNAMSIZ);   
	ifr.ifr_name[IFNAMSIZ   -   1]   =   0;   

	if   (ioctl(sock,   SIOCGIFADDR,   &ifr)   <   0)   
	{   
		perror("ioctl");   
		return   -1;   
	}   
	memcpy(&sin,   &ifr.ifr_addr,   sizeof(sin));   
	strcpy(ipstr, inet_ntoa(sin.sin_addr));
	return TRUE;
}

static bc_boolean __snmp_get_ip(bc_char ip[SNMP_IP_ADDR_COUNT][SNMP_IP_ADDR_LEN], bc_int32 *ip_count)
{
	bc_int32		fd;         						/* 套接字 */
	bc_int32 		if_len;     					/* 接口数量 */
	struct ifreq		buf[MAXINTERFACES];    			/* ifreq结构数组 */
	struct ifconf		ifc;                  					/* ifconf结构 */
	bc_int32		count = 0;

	if (NULL == ip || ip_count == NULL)
	{
		return FALSE;
	}
	
    /* 建立IPv4的UDP套接字fd */
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        BC_PT_DBG_FUN(BC_MODULE_SNMP, "socket(AF_INET, SOCK_DGRAM, 0)");
        return FALSE;
    }
	
    /* 初始化ifconf结构 */
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t)buf;

    /* 获得接口列表 */
    if (ioctl(fd, SIOCGIFCONF, (char *) &ifc) == -1)
    {
        BC_PT_DBG_FUN(BC_MODULE_SNMP, "SIOCGIFCONF ioctl");
        return FALSE;
    }
	
    if_len = ifc.ifc_len / sizeof(struct ifreq); 		/* 接口数量 */
    BC_PT_DBG_FUN(BC_MODULE_SNMP, "接口数量:%d\r\n", if_len);
    while (if_len-- > 0) 								/* 遍历每个接口 */
    {
#if 1    
        BC_PT_DBG_FUN(BC_MODULE_SNMP,  "接口：%s\r\n", buf[if_len].ifr_name); /* 接口名称 */
		
        /* 获得接口标志 */
        if (!(ioctl(fd, SIOCGIFFLAGS, (char *) &buf[if_len])))
        {
            /* 接口状态 */
            if (buf[if_len].ifr_flags & IFF_UP)
            {
                BC_PT_DBG_FUN(BC_MODULE_SNMP,  "接口状态: UP\r\n");
            }
            else
            {
                BC_PT_DBG_FUN(BC_MODULE_SNMP,  "接口状态: DOWN\r\n");
            }
        }
        else
        {
            BC_PT_DBG_FUN(BC_MODULE_SNMP,  "SIOCGIFFLAGS ioctl %s", buf[if_len].ifr_name);
        }
#endif		
	/* IP地址 */
	if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[if_len])))
	{
        	strcpy(ip[count], (bc_char *)inet_ntoa(((struct sockaddr_in*) (&(buf[if_len].ifr_addr)))->sin_addr));
		if (strcmp(ip[count], "127.0.0.1") != 0)
		{
			count++;
		}		
			
     //   	BC_PT_DBG_FUN(BC_MODULE_SNMP,  "IP地址:%s\r\n", (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
	}
        else
        {
            BC_PT_DBG_FUN(BC_MODULE_SNMP,  "SIOCGIFADDR ioctl %s\r\n", buf[if_len].ifr_name);
        }
#if 0
        /* 子网掩码 */
        if (!(ioctl(fd, SIOCGIFNETMASK, (char *) &buf[if_len])))
        {
            BC_PT_DBG_FUN(BC_MODULE_SNMP,  "子网掩码:%s\r\n",
                    (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
        }
        else
        {
            BC_PT_DBG_FUN(BC_MODULE_SNMP, "SIOCGIFADDR ioctl %s", buf[if_len].ifr_name);
        }

        /* 广播地址 */
        if (!(ioctl(fd, SIOCGIFBRDADDR, (char *) &buf[if_len])))
        {
        	BC_PT_DBG_FUN(BC_MODULE_SNMP, "广播地址:%s\r\n",
                    (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
        }
        else
        {
            BC_PT_DBG_FUN(BC_MODULE_SNMP, "SIOCGIFADDR ioctl %s\r\n", buf[if_len].ifr_name);
        }

        /*MAC地址 */
        if (!(ioctl(fd, SIOCGIFHWADDR, (char *) &buf[if_len])))
        {
            BC_PT_DBG_FUN(BC_MODULE_SNMP,  "MAC地址:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[0],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[1],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[2],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[3],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[4],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[5]);
        }
        else
        {
            BC_PT_DBG_FUN(BC_MODULE_SNMP, "SIOCGIFHWADDR ioctl %s\r\n", buf[if_len].ifr_name);
        }
#endif		
    }//–while end

    /*关闭socket*/
    close(fd);
    *ip_count = count;
    
	return TRUE;
}

/*************************************************
  Function: snmp_save_config_file
  Description: 
  Input:
  Output:
  Return:
  		true false
  Note: 
  History: 
*************************************************/
//extern void snmpd_free_trapsinks();

bc_boolean snmp_save_config_file()
{
	bc_char 	ip[SNMP_IP_ADDR_COUNT][SNMP_IP_ADDR_LEN];
	bc_int32	ip_count = 0;
	bc_int32	i = 0;
	bc_int32	j = 0;
	bc_char	file_name[256];
	bc_char	content[256];
	bc_int32	local = 0;
	FILE 		*confile_file = NULL;

	memset(ip, 0, sizeof(ip));	
	memset(file_name, 0, sizeof(file_name));	
	if (!__snmp_get_ip(ip, &ip_count))
	{
		BC_PT_DBG_FUN(BC_MODULE_SNMP,  "__snmp_get_ip() failed!\r\n");
		return FALSE;
	}
	
	BC_PT_DBG_FUN(BC_MODULE_SNMP,  "ip_count = %d\r\n", ip_count);
	for (i = 0; i < ip_count; i++)
	{
		BC_PT_DBG_FUN(BC_MODULE_SNMP,  "ip-%d:%s\r\n", i+1, ip[i]);
	}

	sprintf(file_name, "%s%s.conf", SNMP_CONFIG_FILE_DIR, SNMP_CONFIG_FILE_NAME);
	BC_PT_DBG_FUN(BC_MODULE_SNMP,  "file name:%s\r\n", file_name);

	confile_file = fopen(file_name, "w");
	if (!confile_file)
	{
		BC_PT_DBG_FUN(BC_MODULE_SNMP,  "fopen() failed!\r\n");
		return FALSE;
	}
	
	for (i = 0; i < ip_count; i++)
	{
	//	BC_PT_DBG_FUN(BC_MODULE_SNMP,  "ip:%s\r\n", ip[i]);
		for (j = 0; j < strlen(ip[i]); j++)
		{
			if (ip[i][j] == '.')
			{
				local = j;
			}
		}

		ip[i][local] = '\0';
		sprintf(content, "rwcommunity public %s.0/24\r\n", ip[i]);
		
		BC_PT_DBG_FUN(BC_MODULE_SNMP,  "content:%s\r\n", content);

		fputs(content, confile_file);
	}
	
	fflush(confile_file);
	fclose(confile_file);	

	snmpd_free_trapsinks();
	snmpd_free_trapcommunity();
	update_config();
	return TRUE;
}

#if 1
void snmp_time_to_datatime(time_t time_tick,bc_uint8 date_and_time[8])
{
	bc_int8 timezone = 8;
	
	time_tick += (timezone * 3600);
	
	struct tm *p = gmtime(&time_tick);
	
	BC_PT_DBG_FUN(BC_MODULE_SNMP, "%d-%02d-%02d %02d:%02d:%02d\r\n", (bc_int32)(p->tm_year + 1900), p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

	// year
	bc_uint16 year = p->tm_year + 1900;
	date_and_time[0] = 0xFF & (year >> 8);
	date_and_time[1] = 0xFF & year;
	// month
	date_and_time[2] = 0xFF & (p->tm_mon + 1);
	// day
	date_and_time[3] = 0xFF & p->tm_mday;
	// hour
	date_and_time[4] = 0xFF & p->tm_hour;
	// minute
	date_and_time[5] = 0xFF & p->tm_min;
	// sec
	date_and_time[6] = 0xFF & p->tm_sec;
	// deci-sec
	date_and_time[7] = 0;
}
#else
bc_boolean snmp_time_to_datetime(time_t timep, bc_char datetime[8])
{
	struct tm *p = localtime(&timep);

	// year
	bc_uint16 year = p->tm_year + 1900;
	datetime[0] = 0xFF & (year >> 8);
	datetime[1] = 0xFF & year;
	// month
	datetime[2] = 0xFF & (p->tm_mon + 1);
	// day
	datetime[3] = 0xFF & p->tm_mday;
	// hour
	datetime[4] = 0xFF & p->tm_hour;
	// minute
	datetime[5] = 0xFF & p->tm_min;
	// sec
	datetime[6] = 0xFF & p->tm_sec;
	// deci-sec
	datetime[7] = 0;

	return TRUE;
}

#endif


/*************************************************
  Function: bc_platform_time_sleep
  Description:Suspend calling thread for a specified number of seconds
  Input: 
  		sec:number of seconds to suspend

  Output:
  Return:
		void
  Notes:Other tasks are free to run while the caller is suspended.
  History: 
*************************************************/

void snmp_time_sleep(bc_uint32 sec)
{
    struct timeval tv;
    tv.tv_sec = (time_t) sec;
    tv.tv_usec = 0;
    select(0, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &tv);
}

/*************************************************
  Function: bc_platform_time_usleep
  Description:Suspend calling thread for a specified number of microseconds.
  Input: 
  		usec:number of microseconds to suspend

  Output:
  Return:
		void
  Notes:	The actual delay period depends on the resolution of the
 	Unix select routine, whose precision is limited to the
 	the period of the scheduler tick, generally 1/60 or 1/100 sec.
 	Other tasks are free to run while the caller is suspended.
  History: 
*************************************************/
void snmp_time_usleep(bc_uint32 usec)
{
    struct timeval tv;

    tv.tv_sec = (time_t) (usec / SECOND_USEC);
    tv.tv_usec = (long) (usec % SECOND_USEC);
    select(0, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &tv);
}

