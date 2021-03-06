/*******************************************************************************
  [Author]	   : bain.wang@outlook.com.uestc@gmail.com
  [Version]    : 1.0
  [Date]       : 2017-03-01
  [Description]:

  [Others]     :

  [Function List]:
	1. ....
	2. ....
  [History]:
	 D a t e	Modification										  Initials
	---------- --------------										 ----------
	2017-03-01	Created 										      bain.wang@outlook.com
*******************************************************************************/


////////////////////////////////////////////////////////////////////////////////
//																			  //
//								头   文   件								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
#ifndef __SNMP_MIB2_INTERFACE_H__
#define __SNMP_MIB2_INTERFACE_H__

#include <sys/timeb.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/header_complex.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/timeb.h>
#include <time.h>
#include <fcntl.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/queue.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/route.h>

#include "snmp_mib2_ifxTable.h"

#include "bc_common_defs.h"

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								宏   定   义								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
#define NETSNMP_IFINDEX         		0
#define NETSNMP_IFDESCR         		1
#define NETSNMP_IFTYPE          		2
#define NETSNMP_IFMTU           		3
#define NETSNMP_IFSPEED         		4
#define NETSNMP_IFPHYSADDRESS   	5
#define NETSNMP_IFADMINSTATUS   	6
#define NETSNMP_IFOPERSTATUS    	7
#define NETSNMP_IFLASTCHANGE    	8
#define NETSNMP_IFINOCTETS      		9
#define NETSNMP_IFINUCASTPKTS   	10
#define NETSNMP_IFINNUCASTPKTS  	11
#define NETSNMP_IFINDISCARDS    	12
#define NETSNMP_IFINERRORS      		13
#define NETSNMP_IFINUNKNOWNPROTOS 14
#define NETSNMP_IFOUTOCTETS     	15
#define NETSNMP_IFOUTUCASTPKTS  	16
#define NETSNMP_IFOUTNUCASTPKTS 	17
#define NETSNMP_IFOUTDISCARDS   	18
#define NETSNMP_IFOUTERRORS     	19
#define NETSNMP_IFOUTQLEN       		20
#define NETSNMP_IFSPECIFIC      		21

#define MIB2_SNMP_INTERFACE_COUNT	64

////////////////////////////////////////////////////////////////////////////////
//																			  //
//					结 构 体 、联 合 体、枚 举 定 义				  		  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
/*
 * Typical data structure for a row entry 
 */
struct snmp_if_entry
{
	bc_int32	ifIndex;
	bc_char	ifDescr[256];
	bc_int32	ifType;
	bc_int32	ifMtu;
	bc_int32	ifSpeed;
	bc_mac	ifPhysAddress;
	bc_int32	ifAdminStatus;
	bc_int32	ifOperStatus;
	bc_int32	ifLastChange;
	bc_int32	ifInOctets;
	bc_int32	ifInUcastPkts;
	bc_int32	ifInNUcastPkts;
	bc_int32	ifInDiscards;
	bc_int32	ifInErrors;
	bc_int32	ifInUnknownProtos;
	bc_int32	ifOutOctets;
	bc_int32	ifOutUcastPkts;
	bc_int32	ifOutNUcastPkts;
	bc_int32	ifOutDiscards;
	bc_int32	ifOutErrors;
	bc_int32	ifOutQLen;
	oid			ifSpecific[128];
	bc_int32	ifSpecific_len;
	
    bc_int32	storageType;
};

/*
 * this struct ifnet is cloned from the generic type and somewhat modified.
 * it will not work for other un*x'es...
 */
struct ifnet
{
     char           *if_name;       /* name, e.g. ``en'' or ``lo'' */
     char           *if_unit;       /* sub-unit for lower level driver */
     short           if_mtu;        /* maximum transmission unit */
     short           if_flags;      /* up/down, broadcast, etc. */
     int             if_metric;     /* routing metric (external only) */
     char            if_hwaddr[6];  /* ethernet address */
     int             if_type;       /* interface type: 1=generic,
                                     * 28=slip, ether=6, loopback=24 */
     u_long          if_speed;      /* interface speed: in bits/sec */

     struct sockaddr if_addr;       /* interface's address */
     struct sockaddr ifu_broadaddr; /* broadcast address */
     struct sockaddr ia_subnetmask; /* interface's mask */

     struct ifqueue
	 {
         int             ifq_len;
         int             ifq_drops;
     } if_snd;              /* output queue */
     u_long          if_ibytes;     /* octets received on interface */
     u_long          if_ipackets;   /* packets received on interface */
     u_long          if_ierrors;    /* input errors on interface */
     u_long          if_iqdrops;    /* input queue overruns */
	 
     u_long          if_obytes;     /* octets sent on interface */
     u_long          if_opackets;   /* packets sent on interface */
     u_long          if_oerrors;    /* output errors on interface */
     u_long          if_collisions; /* collisions on csma interfaces */
     /*
      * end statistics 
      */
     struct ifnet   *if_next;
};


/** other required module components */
config_require(header_complex);

////////////////////////////////////////////////////////////////////////////////
//																			  //
//						外 部 变 量、 函 数 声 明					          //
//																			  //
////////////////////////////////////////////////////////////////////////////////


bc_boolean snmp_mib2_interface_update();

bc_boolean if_index_get(bc_char ifDescr[256], bc_int32 *if_index);
bc_boolean snmp_interface_count_get(bc_int32 *count);
bc_boolean snmp_interface_mib_get(struct snmp_if_entry interface_mib[MIB2_SNMP_INTERFACE_COUNT], bc_int32 *count);
bc_boolean snmp_ifx_mib_get(struct snmp_ifx_entry ifx_mib[MIB2_SNMP_INTERFACE_COUNT], bc_int32 *count);
bc_boolean snmp_interface_ifAdminStatus_set(bc_int32 ifIndex, bc_int32 ifAdminStatus);
bc_boolean snmp_ifx_ifAlias_set(bc_int32 ifIndex, bc_char ifAlias[64]);

void snmp_mib2_ifTable_init(void);

#endif /*__SNMP_MIB2_INTERFACE_H__*/

