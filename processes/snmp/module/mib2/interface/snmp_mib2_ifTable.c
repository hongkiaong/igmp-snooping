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
//                                                                            //
//																			  //
//								头   文   件								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
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

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/if_ether.h>

//#include "kt_modules.h"


#include "snmp_global.h"
#include "snmp_core_ex.h"
#include "snmp_mib2_ifTable.h"
#include "snmp_mib2_ifxTable.h"
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
#ifndef IF_NAMESIZE
#define IF_NAMESIZE 16
#endif

#ifndef SIOCGMIIPHY
#define SIOCGMIIPHY 0x8947
#endif

#ifndef SIOCGMIIREG
#define SIOCGMIIREG 0x8948
#endif

#define NOMINAL_LINK_SPEED 10000000

/* if you want caching enabled for speed retrieval purposes, set this to 5?*/
#define MINLOADFREQ 0                     /* min reload frequency in seconds */
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

static struct header_complex_index *mib2InterfaceStorage = NULL;
//static struct portVlanTrunkTable_entry portVlanTrunkTable;
static time_t update_time;
static struct ifnet *ifnetaddr_list;
struct ifnet *ifnetaddr;


snmp_find_var_callback   __snmp_mib2_if_handler;
WriteMethod     snmp_write_ifAdminStatus;


static oid ifTable_variables_oid[] = { 1, 3, 6, 1, 2, 1, 2, 2 };
struct variable2 ifTable_variables[] =
{
    {NETSNMP_IFINDEX, ASN_INTEGER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 1}},
    {NETSNMP_IFDESCR, ASN_OCTET_STR, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 2}},
    {NETSNMP_IFTYPE, ASN_INTEGER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 3}},
    {NETSNMP_IFMTU, ASN_INTEGER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 4}},
    {NETSNMP_IFSPEED, ASN_GAUGE, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 5}},
    {NETSNMP_IFPHYSADDRESS, ASN_OCTET_STR, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 6}},
    {NETSNMP_IFADMINSTATUS, ASN_INTEGER, NETSNMP_OLDAPI_RWRITE,
     __snmp_mib2_if_handler, 2, {1, 7}},
    {NETSNMP_IFOPERSTATUS, ASN_INTEGER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 8}},
    {NETSNMP_IFLASTCHANGE, ASN_TIMETICKS, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 9}},
    {NETSNMP_IFINOCTETS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 10}},
    {NETSNMP_IFINUCASTPKTS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 11}},
    {NETSNMP_IFINNUCASTPKTS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 12}},
    {NETSNMP_IFINDISCARDS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 13}},
    {NETSNMP_IFINERRORS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 14}},
    {NETSNMP_IFINUNKNOWNPROTOS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 15}},
    {NETSNMP_IFOUTOCTETS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 16}},
    {NETSNMP_IFOUTUCASTPKTS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 17}},
    {NETSNMP_IFOUTNUCASTPKTS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 18}},
    {NETSNMP_IFOUTDISCARDS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 19}},
    {NETSNMP_IFOUTERRORS, ASN_COUNTER, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 20}},
    {NETSNMP_IFOUTQLEN, ASN_GAUGE, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 21}},
    {NETSNMP_IFSPECIFIC, ASN_OBJECT_ID, NETSNMP_OLDAPI_RONLY,
     __snmp_mib2_if_handler, 2, {1, 22}}
};
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
struct snmp_if_entry *__snmp_create_mib2_if_data(void)
{
	BC_PT_FUN_TAG(BC_MODULE_SNMP);
	
    struct snmp_if_entry *StorageNew = NULL;
    StorageNew = SNMP_MALLOC_STRUCT(snmp_if_entry);
    if (StorageNew == NULL)
    {
    	return NULL;
    }

	memset(StorageNew, 0, sizeof(struct snmp_if_entry));
	
    StorageNew->ifIndex = 1;
    StorageNew->storageType = ST_NONVOLATILE;
    return StorageNew;
}

static void __snmp_free_mib2_if_data(struct snmp_if_entry *StorageDel)
{
	BC_PT_FUN_TAG(BC_MODULE_SNMP);
    netsnmp_assert(StorageDel);
#if 0	
    free(StorageDel->pingCtlOwnerIndex);
    free(StorageDel->pingCtlTestName);
    free(StorageDel->pingCtlTargetAddress);
    free(StorageDel->pingCtlDataFill);
    free(StorageDel->pingCtlTrapGeneration);
    free(StorageDel->pingCtlType);
    free(StorageDel->pingCtlDescr);
    free(StorageDel->pingCtlSourceAddress);
#endif
    free(StorageDel);
}

/*
 * __kt_snmp_qosGlobalSetTable_add(): adds a structure node to our data set 
 */
int __snmp_mib2_if_add(struct snmp_if_entry *thedata)
{
	BC_PT_FUN_TAG(BC_MODULE_SNMP);

    netsnmp_variable_list *vars = NULL;

    /*
     * add the index variables to the varbind list, which is 
     * used by header_complex to index the data 
     */
    snmp_varlist_add_variable(&vars, NULL, 0, ASN_INTEGER,
                              &thedata->ifIndex,
                              sizeof(thedata->ifIndex));
	
    header_complex_add_data(&mib2InterfaceStorage, vars, thedata);

    return SNMPERR_SUCCESS;
}

/*
 * var_snmpCommunityTable():
 *   Handle this table separately from the scalar value case.
 *   The workings of this are basically the same as for var_mteObjectsTable above.
 */
unsigned char *__snmp_mib2_if_handler(struct variable *vp,
                 oid * name,
                 size_t *length,
                 int exact, size_t *var_len, WriteMethod ** write_method)
{
	BC_PT_FUN_TAG(BC_MODULE_SNMP);

	if (vp)
	{
		BC_PT_DBG_FUN(BC_MODULE_SNMP, "magic = %d type = %d acl = %d namelen = %d\r\n", vp->magic, vp->type, vp->acl, vp->namelen);
	}
	
	bc_int32 i = 0;
	for (i = 0; i < vp->namelen; i++)
	{
		if (i == 0)
		{
			BC_PT_DBG_FUN(BC_MODULE_SNMP, "vp->name:%d", (bc_int32)vp->name[i]);
		}
		else
		{
			BC_PT_DBG_SMP(BC_MODULE_SNMP, ".%d", (bc_int32)vp->name[i]);
		}
	}
	if (vp->namelen)
	{
		BC_PT_DBG_SMP(BC_MODULE_SNMP, "\r\n");
	}

	/**/
	if (length)
	{
		BC_PT_DBG_FUN(BC_MODULE_SNMP, "length = %d\r\n", *length);
	}
	for (i = 0; i < *length; i++)
	{
		if (i == 0)
		{
			BC_PT_DBG_FUN(BC_MODULE_SNMP, "name:%d", (bc_int32)name[i]);
		}
		else
		{
			BC_PT_DBG_SMP(BC_MODULE_SNMP, ".%d", (bc_int32)name[i]);
		}
	}
	if (*length)
	{
		BC_PT_DBG_SMP(BC_MODULE_SNMP, "\r\n");
	}
	
    struct snmp_if_entry *StorageTmp = NULL;
	if (!snmp_mib2_interface_update())
	{
		BC_PT_DBG_FUN(BC_MODULE_SNMP, "snmp_mib2_interface_update() failed!!\r\n");
        return NULL;
	}

    /*
     * this assumes you have registered all your data properly
     */
    if ((StorageTmp = header_complex(mib2InterfaceStorage, vp, name, length, exact, var_len, write_method)) == NULL)
    {
#if 0    
        if (vp->magic == COLUMN_SNMPCOMMUNITYROWSTATUS)
        {
        	*write_method = kt_snmp_write_snmpCommunityRowStatus;
        }
#endif		
		BC_PT_FUN_TAG(BC_MODULE_SNMP);
        return NULL;
    }

#if 0
    /*
     * this is where we do the value assignments for the mib results.
     */
    switch (vp->magic)
	{
    case COLUMN_TRUNKVIDLIST:
        *write_method = kt_snmp_write_trunkVidList;
        *var_len = StorageTmp->trunkVidList_len;
        return (u_char *)StorageTmp->trunkVidList;
    default:
		BC_PT_DBG_FUN(BC_MODULE_SNMP, l_mdl_id, "colunm error!!\r\n");
    }
#endif

    /*
     * this is where we do the value assignments for the mib results.
     */     
    switch (vp->magic)
	{
    case NETSNMP_IFINDEX:
        long_return = StorageTmp->ifIndex;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFDESCR:
		memcpy(return_buf, StorageTmp->ifDescr, sizeof(StorageTmp->ifDescr));
        *var_len = strlen(StorageTmp->ifDescr);
        return (u_char *)return_buf;
    case NETSNMP_IFTYPE:
		long_return = StorageTmp->ifType;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFMTU:
        long_return = StorageTmp->ifMtu;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFSPEED:
        long_return = StorageTmp->ifSpeed;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFPHYSADDRESS:
		memcpy(return_buf, StorageTmp->ifPhysAddress, sizeof(StorageTmp->ifPhysAddress));
        *var_len = sizeof(StorageTmp->ifPhysAddress);
        return (u_char *)return_buf;
    case NETSNMP_IFADMINSTATUS:
        *write_method = snmp_write_ifAdminStatus;
        long_return = StorageTmp->ifAdminStatus;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFOPERSTATUS:
        long_return = StorageTmp->ifAdminStatus;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
	case NETSNMP_IFLASTCHANGE:
        long_return = StorageTmp->ifLastChange;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFINOCTETS:
        long_return = StorageTmp->ifInOctets;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFINUCASTPKTS:		
        long_return = StorageTmp->ifInUcastPkts;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFINNUCASTPKTS:		
        long_return = StorageTmp->ifInNUcastPkts;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFINDISCARDS:
        long_return = StorageTmp->ifInDiscards;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFINERRORS:
        long_return = StorageTmp->ifInErrors;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFINUNKNOWNPROTOS:
        long_return = StorageTmp->ifInUnknownProtos;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFOUTOCTETS:
        long_return = StorageTmp->ifOutOctets;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFOUTUCASTPKTS:
        long_return = StorageTmp->ifOutUcastPkts;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFOUTNUCASTPKTS:
        long_return = StorageTmp->ifOutNUcastPkts;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFOUTDISCARDS:
        long_return = StorageTmp->ifOutDiscards;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
    case NETSNMP_IFOUTERRORS:
        long_return = StorageTmp->ifOutErrors;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
	case NETSNMP_IFOUTQLEN:
        long_return = StorageTmp->ifOutQLen;
		*var_len = sizeof(long_return);
        return (u_char *) & long_return;
	case NETSNMP_IFSPECIFIC:
		memcpy(return_buf, StorageTmp->ifSpecific, StorageTmp->ifSpecific_len);
		*var_len = StorageTmp->ifSpecific_len;
        return (u_char *) & long_return;		
    default:
        return 0;
    }
	
    return NULL;
}

bc_int32 snmp_write_ifAdminStatus(int action,
								                     u_char * var_val,
								                     u_char var_val_type,
								                     size_t var_val_len,
								                     u_char * statP, oid * name, size_t name_len)
{
	BC_PT_FUN_TAG(BC_MODULE_SNMP);
	BC_PT_DBG_FUN(BC_MODULE_SNMP, "action = %d var_val_type = %d var_val_len = %d name_len = %d\r\n", action, var_val_type, var_val_len, name_len);
	bc_int32 i = 0;	
	
	if (name)
	{
		for (i = 0; i < name_len; i++)
		{
			if (i == 0)
			{
				BC_PT_DBG_FUN(BC_MODULE_SNMP, "name:%d", (bc_int32)name[i]);
			}
			else
			{
				BC_PT_DBG_SMP(BC_MODULE_SNMP, ".%d", (bc_int32)name[i]);
			}
		}
		BC_PT_DBG_SMP(BC_MODULE_SNMP, "\r\n");
	}
	
    static bc_int32 tmpvar;
    struct snmp_if_entry *StorageTmp = NULL;
    size_t  newlen =  name_len - (sizeof(ifTable_variables_oid) / sizeof(oid) + 3 - 1);

    if ((StorageTmp =  header_complex(mib2InterfaceStorage, NULL, &name[sizeof(ifTable_variables_oid) / sizeof(oid) + 3 - 1], &newlen, 1, NULL, NULL)) == NULL)
	{
		BC_PT_FUN_TAG(BC_MODULE_SNMP);
		return SNMP_ERR_NOSUCHNAME;     /* remove if you support creation here */
    }	

    if (StorageTmp && StorageTmp->storageType == ST_READONLY) 
	{
        return SNMP_ERR_NOTWRITABLE;
    }

#if 0
    if (StorageTmp && StorageTmp->snmpCommunityRowStatus == RS_ACTIVE)
	{
        return SNMP_ERR_NOTWRITABLE;
    }
#endif	

    switch (action)
	{
    case RESERVE1:
        if (var_val_type != ASN_INTEGER) 
		{
			BC_PT_FUN_TAG(BC_MODULE_SNMP);
            return SNMP_ERR_WRONGTYPE;
        }

		/*
		if (*(bc_int32*)var_val < DEVICEBASEQOSMAPRULE_COS || *(bc_int32*)var_val > DEVICEBASEQOSMAPRULE_DIFFSERV)
		{
			BC_PT_FUN_TAG(BC_MODULE_SNMPl_mdl_id);
			return SNMP_ERR_BADVALUE;
		}
		*/
		
        break;
    case RESERVE2:
        /*
         * memory reseveration, final preparation... 
         */
        break;
    case FREE:
        /*
         * Release any resources that have been allocated 
         */
        break;
    case ACTION:
        /*
         * The variable has been stored in objid for
         * you to use, and you have just been asked to do something with
         * it.  Note that anything done here must be reversable in the UNDO case 
         */
        tmpvar = StorageTmp->ifAdminStatus;
        StorageTmp->ifAdminStatus = *((long *) var_val);
        break;
    case UNDO:
        /*
         * Back out any changes made in the ACTION case 
         */
         StorageTmp->ifAdminStatus = tmpvar;
        break;
    case COMMIT:
        /*
         * Things are working well, so it's now safe to make the change
         * permanently.  Make sure that anything done here can't fail! 
         */
        snmp_store_needed(NULL);
        break;
    }
	
    return SNMP_ERR_NOERROR;
}

bc_boolean snmp_mib2_interface_update()
{	
	if (snmp_is_update(&update_time))
	{
		struct	snmp_if_entry interface_mib[MIB2_SNMP_INTERFACE_COUNT];
		bc_int32 	count = 0;
		bc_int32	index = 0;
		struct header_complex_index *mib2_if_tmp = NULL;
		struct snmp_if_entry *StorageTmp = NULL;

		memset(interface_mib, 0, sizeof(interface_mib));
		if (!snmp_interface_mib_get(interface_mib, &count))
		{
			BC_PT_DBG_FUN(BC_MODULE_SNMP, "snmp_interface_mib_get() failed!\r\n");
			return FALSE;
		}
		
		BC_PT_DBG_FUN(BC_MODULE_SNMP, "count = %d\r\n", count);
		for (index = 0; index < count; index++)
		{
			BC_PT_DBG_FUN(BC_MODULE_SNMP, "index = %d ifDescr:%s\r\n", interface_mib[index].ifIndex, interface_mib[index].ifDescr);
		}
		
		for (index = 0; index < count; index++)
		{
			mib2_if_tmp = mib2InterfaceStorage;
			StorageTmp = NULL;
			
			while (mib2_if_tmp)
			{
				StorageTmp = (struct snmp_if_entry *)mib2_if_tmp->data;
				
				if (StorageTmp->ifIndex == interface_mib[index].ifIndex)
				{
					memcpy(StorageTmp, &interface_mib[index], sizeof(interface_mib[index]));
					break;
				}

				mib2_if_tmp = mib2_if_tmp->next;
			}	

			if (!mib2_if_tmp)
			{
				StorageTmp = __snmp_create_mib2_if_data();
				memcpy(StorageTmp, &interface_mib[index], sizeof(interface_mib[index]));
    			StorageTmp->storageType = ST_NONVOLATILE;
				__snmp_mib2_if_add(StorageTmp);
			}
		}

		mib2_if_tmp = mib2InterfaceStorage;
		while (mib2_if_tmp)
		{
			StorageTmp = (struct snmp_if_entry *)mib2_if_tmp->data;
			for(index = 0; index < count; index++)
			{
				if (StorageTmp->ifIndex == interface_mib[index].ifIndex)
				{
					break;
				}
			}	

			if (index == count)
			{
	            struct header_complex_index *hciptrn = mib2_if_tmp->next;
	            struct snmp_if_entry *StorageDel = header_complex_extract_entry(&mib2InterfaceStorage, hciptrn);
	            __snmp_free_mib2_if_data(StorageDel);

				mib2_if_tmp = hciptrn;
			}
			else
			{
				mib2_if_tmp = mib2_if_tmp->next;
			}			
		}
	}	

	return TRUE;
}

/*
 * if_type_from_name
 * Return interface type using the interface name as a clue.
 * Returns 1 to imply "other" type if name not recognized. 
 */
static int _if_type_from_name(const char *pcch)
{
    typedef struct _match_if
	{
        int             mi_type;
        const char     *mi_name;
    } *pmatch_if, match_if;

    static match_if lmatch_if[] =
	{
        {24, "lo"},
        {6, "eth"},
        {9, "tr"},
        {23, "ppp"},
        {28, "sl"},
        {0, 0}                  /* end of list */
    };

    int             len;
    register pmatch_if pm;

    for (pm = lmatch_if; pm->mi_name; pm++)
	{
        len = strlen(pm->mi_name);
        if (0 == strncmp(pcch, pm->mi_name, len))
		{
            return (pm->mi_type);
        }
    }
    return (1);                 /* in case search fails */
}

/**
 * Determines network interface speed from MII
 */
unsigned long long _netsnmp_linux_interface_get_if_speed(int fd, const char *name, unsigned long long defaultspeed)
{
    unsigned long long retspeed = defaultspeed;
    struct ifreq ifr;

    /* the code is based on mii-diag utility by Donald Becker
     * see ftp://ftp.scyld.com/pub/diag/mii-diag.c
     */
    ushort *data = (ushort *)(&ifr.ifr_data);
    unsigned phy_id;
    int mii_reg, i;
    ushort mii_val[32];
    ushort bmcr, bmsr, nway_advert, lkpar;
    const unsigned long long media_speeds[] = {10000000, 10000000, 100000000, 100000000, 10000000, 0};
    /* It corresponds to "10baseT", "10baseT-FD", "100baseTx", "100baseTx-FD", "100baseT4", "Flow-control", 0, */

    strlcpy(ifr.ifr_name, name, sizeof(ifr.ifr_name));
    data[0] = 0;
    
    /*
     * SIOCGMIIPHY has been defined since at least kernel 2.4.10 (Sept 2001).
     * It's probably safe to drop the interim SIOCDEVPRIVATE handling now!
     */
    if (ioctl(fd, SIOCGMIIPHY, &ifr) < 0) 
	{
    //    DEBUGMSGTL(("mibII/ifTable", "SIOCGMIIPHY on %s failed\n",
    //                ifr.ifr_name));
        return retspeed;
    }

    /* Begin getting mii register values */
    phy_id = data[0];
    for (mii_reg = 0; mii_reg < 8; mii_reg++)
	{
        data[0] = phy_id;
        data[1] = mii_reg;
        if(ioctl(fd, SIOCGMIIREG, &ifr) <0)
		{
        //    DEBUGMSGTL(("mibII/ifTable", "SIOCGMIIREG on %s failed\n", ifr.ifr_name));
        }
        mii_val[mii_reg] = data[3];		
    }
    /*Parsing of mii values*/
    /*Invalid basic mode control register*/
    if (mii_val[0] == 0xffff  ||  mii_val[1] == 0x0000)
	{
    //    DEBUGMSGTL(("mibII/ifTable", "No MII transceiver present!.\n"));
        return retspeed;
    }
    /* Descriptive rename. */
    bmcr = mii_val[0]; 	  /*basic mode control register*/
    bmsr = mii_val[1]; 	  /* basic mode status register*/
    nway_advert = mii_val[4]; /* autonegotiation advertisement*/
    lkpar = mii_val[5]; 	  /*link partner ability*/
    
    /*Check for link existence, returns 0 if link is absent*/
    if ((bmsr & 0x0016) != 0x0004)
	{
    //    DEBUGMSGTL(("mibII/ifTable", "No link...\n"));
        retspeed = 0;
        return retspeed;
    }
    
    if(!(bmcr & 0x1000) )
	{
    //    DEBUGMSGTL(("mibII/ifTable", "Auto-negotiation disabled.\n"));
        retspeed = bmcr & 0x2000 ? 100000000 : 10000000;
        return retspeed;
    }
    /* Link partner got our advertised abilities */	
    if (lkpar & 0x4000)
	{
        int negotiated = nway_advert & lkpar & 0x3e0;
        int max_capability = 0;
        /* Scan for the highest negotiated capability, highest priority
           (100baseTx-FDX) to lowest (10baseT-HDX). */
        int media_priority[] = {8, 9, 7, 6, 5}; 	/* media_names[i-5] */
        for (i = 0; media_priority[i]; i++)
		{
            if (negotiated & (1 << media_priority[i]))
			{
                max_capability = media_priority[i];
                break;
            }
        }
        if (max_capability)
        {
            retspeed = media_speeds[max_capability - 5];
        }
        else
        {        	
            //DEBUGMSGTL(("mibII/ifTable", "No common media type was autonegotiated!\n"));
        }
    }
	else if(lkpar & 0x00A0)
	{
        retspeed = (lkpar & 0x0080) ? 100000000 : 10000000;
    }
    return retspeed;
}

/**
* Determines network interface speed. It is system specific. Only linux
* realization is made. 
*/
unsigned int _getIfSpeed(int fd, struct ifreq ifr, unsigned int defaultspeed)
{
    return _netsnmp_linux_interface_get_if_speed(fd, ifr.ifr_name, defaultspeed);
}

bc_boolean _interface_mib_get(struct snmp_if_entry interface_mib[MIB2_SNMP_INTERFACE_COUNT], bc_int32 *count)
{
    char            line[256], ifname_buf[64], *ifname, *ptr;
    struct ifreq    ifrq;
    struct ifnet  **ifnetaddr_ptr;
    FILE           *devin;
    int             i, fd;
	bc_int32		interface_mib_count = 0;
//    conf_if_list   *if_ptr;
    /*
     * scanline_2_2:
     *  [               IN                        ]
     *   byte pkts errs drop fifo frame cmprs mcst |
     *  [               OUT                               ]
     *   byte pkts errs drop fifo colls carrier compressed
     */
    uintmax_t       rec_pkt, rec_oct, rec_err, rec_drop;
    uintmax_t       snd_pkt, snd_oct, snd_err, snd_drop, coll;
    const char     *scan_line_2_2 =
        "%"   SCNuMAX " %"  SCNuMAX " %"  SCNuMAX " %"  SCNuMAX
        " %*" SCNuMAX " %*" SCNuMAX " %*" SCNuMAX " %*" SCNuMAX
        " %"  SCNuMAX " %"  SCNuMAX " %"  SCNuMAX " %"  SCNuMAX
        " %*" SCNuMAX " %"  SCNuMAX;
    const char     *scan_line_2_0 =
        "%"   SCNuMAX " %"  SCNuMAX " %*" SCNuMAX " %*" SCNuMAX
        " %*" SCNuMAX " %"  SCNuMAX " %"  SCNuMAX " %*" SCNuMAX
        " %*" SCNuMAX " %"  SCNuMAX;

    const char     *scan_line_to_use;

    /*
     * free old list: 
     */
    while (ifnetaddr_list)
	{
        struct ifnet *old = ifnetaddr_list;
        ifnetaddr_list = ifnetaddr_list->if_next;
        free(old->if_name);
        free(old->if_unit);
        free(old);
    }

    ifnetaddr = 0;
    ifnetaddr_ptr = &ifnetaddr_list;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		BC_PT_DBG_FUN(BC_MODULE_SNMP, "socket() failed.\r\n");
        return FALSE;
    }

    /*
     * build up ifnetaddr list by hand: 
     */

    /*
     * at least linux v1.3.53 says EMFILE without reason... 
     */
    if (!(devin = fopen("/proc/net/dev", "r"))) 
	{
		BC_PT_DBG_FUN(BC_MODULE_SNMP, "cannot open /proc/net/dev.\r\n");
        close(fd);
        return FALSE;
    }

    i = 0;

    /*
     * read the second line (a header) and determine the fields we
     * should read from.  This should be done in a better way by
     * actually looking for the field names we want.  But thats too
     * much work for today.  -- Wes 
     */
    fgets(line, sizeof(line), devin);
    fgets(line, sizeof(line), devin);
    if (strstr(line, "compressed")) 
	{
        scan_line_to_use = scan_line_2_2;
    } 
	else 
	{
        scan_line_to_use = scan_line_2_0;
    }

    while (fgets(line, sizeof(line), devin))
	{
        struct ifnet   *nnew;
        char           *stats, *ifstart = line;

        if (line[strlen(line) - 1] == '\n')
        {
        	line[strlen(line) - 1] = '\0';
        }

        while (*ifstart && *ifstart == ' ')
        {
        	ifstart++;
        }

        if (!*ifstart || ((stats = strrchr(ifstart, ':')) == NULL))
		{
            continue;
        }
		
        if ((scan_line_to_use == scan_line_2_2) && ((stats - line) < 6)) 
		{
			BC_PT_DBG_FUN(BC_MODULE_SNMP, "/proc/net/dev data format error, line ==|%s|\r\n", line);
        }

        *stats   = 0;
        strlcpy(ifname_buf, ifstart, sizeof(ifname_buf));
        *stats++ = ':';
        while (*stats == ' ')
        {
        	stats++;
        }

        if ((scan_line_to_use == scan_line_2_2 &&
             sscanf(stats, scan_line_to_use, &rec_oct, &rec_pkt, &rec_err,
                    &rec_drop, &snd_oct, &snd_pkt, &snd_err, &snd_drop,
                    &coll) != 9) || (scan_line_to_use == scan_line_2_0
                                     && sscanf(stats, scan_line_to_use,
                                               &rec_pkt, &rec_err,
                                               &snd_pkt, &snd_err,
                                               &coll) != 5)) {
            if ((scan_line_to_use == scan_line_2_2)
                && !strstr(line, "No statistics available"))
//                snmp_log(LOG_ERR,
//                         "/proc/net/dev data format error, line ==|%s|",
//                         line);
            continue;
        }

        nnew = (struct ifnet *) calloc(1, sizeof(struct ifnet));
        if (nnew == NULL)
        {
            break;              /* alloc error */
        }

        /*
         * chain in: 
         */
        *ifnetaddr_ptr = nnew;
        ifnetaddr_ptr = &nnew->if_next;
        i++;

        /*
         * linux previous to 1.3.~13 may miss transmitted loopback pkts: 
         */
        if (!strcmp(ifname_buf, "lo") && rec_pkt > 0 && !snd_pkt)
            snd_pkt = rec_pkt;

        nnew->if_ipackets = rec_pkt & 0xffffffff;
        nnew->if_ierrors = rec_err;
        nnew->if_opackets = snd_pkt & 0xffffffff;
        nnew->if_oerrors = snd_err;
        nnew->if_collisions = coll;
        if (scan_line_to_use == scan_line_2_2)
		{
            nnew->if_ibytes = rec_oct & 0xffffffff;
            nnew->if_obytes = snd_oct & 0xffffffff;
            nnew->if_iqdrops = rec_drop;
            nnew->if_snd.ifq_drops = snd_drop;
        }
		else
		{
            nnew->if_ibytes = (rec_pkt * 308) & 0xffffffff;
            nnew->if_obytes = (snd_pkt * 308) & 0xffffffff;
        }

        /*
         * ifnames are given as ``   eth0'': split in ``eth'' and ``0'': 
         */
        for (ifname = ifname_buf; *ifname && *ifname == ' '; ifname++);

        /*
         * set name and interface# : 
         */
        nnew->if_name = (char *) strdup(ifname);
        for (ptr = nnew->if_name; *ptr && (*ptr < '0' || *ptr > '9');
             ptr++);
        nnew->if_unit = strdup(*ptr ? ptr : "");
        *ptr = 0;

        strlcpy(ifrq.ifr_name, ifname, sizeof(ifrq.ifr_name));
        if (ioctl(fd, SIOCGIFADDR, &ifrq) < 0)
            memset((char *) &nnew->if_addr, 0, sizeof(nnew->if_addr));
        else
            nnew->if_addr = ifrq.ifr_addr;

        strlcpy(ifrq.ifr_name, ifname, sizeof(ifrq.ifr_name));
        if (ioctl(fd, SIOCGIFBRDADDR, &ifrq) < 0)
            memset((char *) &nnew->ifu_broadaddr, 0,
                   sizeof(nnew->ifu_broadaddr));
        else
            nnew->ifu_broadaddr = ifrq.ifr_broadaddr;

        strlcpy(ifrq.ifr_name, ifname, sizeof(ifrq.ifr_name));
        if (ioctl(fd, SIOCGIFNETMASK, &ifrq) < 0)
            memset((char *) &nnew->ia_subnetmask, 0,
                   sizeof(nnew->ia_subnetmask));
        else
            nnew->ia_subnetmask = ifrq.ifr_netmask;

        strlcpy(ifrq.ifr_name, ifname, sizeof(ifrq.ifr_name));
        nnew->if_flags = ioctl(fd, SIOCGIFFLAGS, &ifrq) < 0
            ? 0 : ifrq.ifr_flags;

        nnew->if_type = 0;

        /*
         * NOTE: this ioctl does not guarantee 6 bytes of a physaddr.
         * In particular, a 'sit0' interface only appears to get back
         * 4 bytes of sa_data.
         */
        memset(ifrq.ifr_hwaddr.sa_data, (0), IFHWADDRLEN);
        strlcpy(ifrq.ifr_name, ifname, sizeof(ifrq.ifr_name));
        if (ioctl(fd, SIOCGIFHWADDR, &ifrq) < 0)
            memset(nnew->if_hwaddr, (0), IFHWADDRLEN);
        else
		{
            memcpy(nnew->if_hwaddr, ifrq.ifr_hwaddr.sa_data, IFHWADDRLEN);

#ifdef ARPHRD_LOOPBACK
            switch (ifrq.ifr_hwaddr.sa_family) 
			{
            case ARPHRD_ETHER:
                nnew->if_type = 6;
                break;
            case ARPHRD_TUNNEL:
            case ARPHRD_TUNNEL6:
#ifdef ARPHRD_IPGRE
            case ARPHRD_IPGRE:
#endif
            case ARPHRD_SIT:
                nnew->if_type = 131;
                break;          /* tunnel */
            case ARPHRD_SLIP:
            case ARPHRD_CSLIP:
            case ARPHRD_SLIP6:
            case ARPHRD_CSLIP6:
                nnew->if_type = 28;
                break;          /* slip */
            case ARPHRD_PPP:
                nnew->if_type = 23;
                break;          /* ppp */
            case ARPHRD_LOOPBACK:
                nnew->if_type = 24;
                break;          /* softwareLoopback */
            case ARPHRD_FDDI:
                nnew->if_type = 15;
                break;
            case ARPHRD_ARCNET:
                nnew->if_type = 35;
                break;
            case ARPHRD_LOCALTLK:
                nnew->if_type = 42;
                break;
#ifdef ARPHRD_HIPPI
            case ARPHRD_HIPPI:
                nnew->if_type = 47;
                break;
#endif
#ifdef ARPHRD_ATM
            case ARPHRD_ATM:
                nnew->if_type = 37;
                break;
#endif
                /*
                 * XXX: more if_arp.h:ARPHDR_xxx to IANAifType mappings... 
                 */
            }
#endif
        }

        strlcpy(ifrq.ifr_name, ifname, sizeof(ifrq.ifr_name));
        nnew->if_metric = ioctl(fd, SIOCGIFMETRIC, &ifrq) < 0
            ? 0 : ifrq.ifr_metric;

#ifdef SIOCGIFMTU
        strlcpy(ifrq.ifr_name, ifname, sizeof(ifrq.ifr_name));
        nnew->if_mtu = (ioctl(fd, SIOCGIFMTU, &ifrq) < 0)
            ? 0 : ifrq.ifr_mtu;
#else
        nnew->if_mtu = 0;
#endif

//        if_ptr = netsnmp_access_interface_entry_overrides_get(ifname);
//        if (if_ptr) {
//            nnew->if_type = if_ptr->type;
//            nnew->if_speed = if_ptr->speed;
//        }
//		else
        {
            /*
             * do only guess if_type from name, if we could not read
             * * it before from SIOCGIFHWADDR 
             */
            unsigned int defaultspeed = NOMINAL_LINK_SPEED;
            if (!(nnew->if_flags & IFF_RUNNING))
			{
                /* 
                 * use speed 0 if the if speed cannot be determined *and* the
                 * interface is down
                 */
                defaultspeed = 0;
            }

            if (!nnew->if_type)
            {
            	nnew->if_type = _if_type_from_name(nnew->if_name);
            }
            switch(nnew->if_type) 
			{
            case 6:
                nnew->if_speed = _getIfSpeed(fd, ifrq, defaultspeed);
                break;
            case 24:
                nnew->if_speed = 10000000;
                break;
            case 9:
                nnew->if_speed = 4000000;
                break;
            default:
                nnew->if_speed = 0;
            }
			
            /*Zero speed means link problem*/
            if(nnew->if_speed == 0 && nnew->if_flags & IFF_UP)
			{
                nnew->if_flags &= ~IFF_RUNNING;
            }
        }
#if 0
		/*printf*/
		printf("\r\n\r\nif_name:%s\r\n", nnew->if_name);
		printf("if_unit:%s\r\n", nnew->if_unit);
		printf("if_mtu:%d\r\n", nnew->if_mtu);
		printf("if_flags:%d\r\n", nnew->if_flags);
		printf("if_metric:%d\r\n", nnew->if_metric);
		int i = 0;
		for (i = 0; i < 6; i++)
		{
			if (i == 0)
			{
				printf("if_hwaddr:%02x-", nnew->if_hwaddr[i]);
			}
			else if (i < 5)
			{
				printf("%02x-", nnew->if_hwaddr[i]);
			}
			else
			{
				printf("%02x\r\n", nnew->if_hwaddr[i]);
			}
		}
		printf("if_type:%d\r\n", nnew->if_type);
		printf("if_speed:%d\r\n", (int)nnew->if_speed);
#if 0		
         u_long          if_speed;      /* interface speed: in bits/sec */

         struct sockaddr if_addr;       /* interface's address */
         struct sockaddr ifu_broadaddr; /* broadcast address */
         struct sockaddr ia_subnetmask; /* interface's mask */

         struct ifqueue {
             int             ifq_len;
             int             ifq_drops;
         } if_snd;              /* output queue */
#endif		 
		printf("if_ibytes:%d\r\n", (int)nnew->if_ibytes);
		printf("if_ipackets:%d\r\n", (int)nnew->if_ipackets);
		printf("if_ierrors:%d\r\n", (int)nnew->if_ierrors);
		printf("if_iqdrops:%d\r\n", (int)nnew->if_iqdrops);
		printf("if_opackets:%d\r\n", (int)nnew->if_opackets);
		printf("if_oerrors:%d\r\n", (int)nnew->if_oerrors);
		printf("if_collisions:%d\r\n", (int)nnew->if_collisions);
#endif		
    }                          

    ifnetaddr = ifnetaddr_list;	
    fclose(devin);
    close(fd);

	/*interface_mib*/
	/*
	bc_char		ifDescr[256];
	bc_int32	ifType;
	bc_int32	ifMtu;
	bc_int32	ifSpeed;
	kt_mac		ifPhysAddress;
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
	oid			ifSpecific;

	ifInOctets			if_ibytes
	ifInUcastPkts		if_ipackets
	ifInNUcastPkts		no
	ifInDiscards		if_iqdrops
	ifInErrors			if_ierrors
	ifInUnknownProtos	no

	ifOutOctets			if_obytes
	ifOutUcastPkts		if_opackets
	ifOutNUcastPkts		no
	ifOutDiscards		if_snd.ifq_drops
	ifOutErrors			if_oerrors
	ifOutQLen			if_snd.ifq_len
	*/	
	while (ifnetaddr)
	{
		memset(&interface_mib[interface_mib_count], 0, sizeof(interface_mib[interface_mib_count]));
		interface_mib[interface_mib_count].ifIndex = interface_mib_count + 1;
		sprintf(interface_mib[interface_mib_count].ifDescr, "%s%s", ifnetaddr->if_name, ifnetaddr->if_unit);
//		strcpy(interface_mib[interface_mib_count].ifDescr, ifnetaddr->if_name);
		interface_mib[interface_mib_count].ifType = ifnetaddr->if_type;
		interface_mib[interface_mib_count].ifMtu = ifnetaddr->if_mtu;
		interface_mib[interface_mib_count].ifSpeed = ifnetaddr->if_speed;
		memcpy(interface_mib[interface_mib_count].ifPhysAddress, ifnetaddr->if_hwaddr, sizeof(ifnetaddr->if_hwaddr));
		interface_mib[interface_mib_count].ifAdminStatus = 1;
		interface_mib[interface_mib_count].ifOperStatus = 1;
		interface_mib[interface_mib_count].ifLastChange = 0;
		
		interface_mib[interface_mib_count].ifInOctets = ifnetaddr->if_ibytes;
		interface_mib[interface_mib_count].ifInUcastPkts = ifnetaddr->if_ipackets;
		interface_mib[interface_mib_count].ifInNUcastPkts = 0;
		interface_mib[interface_mib_count].ifInDiscards = ifnetaddr->if_iqdrops;
		interface_mib[interface_mib_count].ifInErrors = ifnetaddr->if_ierrors;
		interface_mib[interface_mib_count].ifInUnknownProtos = 0;
		
		interface_mib[interface_mib_count].ifOutOctets = ifnetaddr->if_obytes;
		interface_mib[interface_mib_count].ifOutUcastPkts = ifnetaddr->if_opackets;
		interface_mib[interface_mib_count].ifOutNUcastPkts = 0;
		interface_mib[interface_mib_count].ifOutDiscards = ifnetaddr->if_snd.ifq_drops;
		interface_mib[interface_mib_count].ifOutErrors = ifnetaddr->if_oerrors;
		interface_mib[interface_mib_count].ifOutQLen = ifnetaddr->if_snd.ifq_len;
		interface_mib[interface_mib_count].ifSpecific_len = 2;
	
		ifnetaddr = ifnetaddr->if_next;
		interface_mib_count++;
	}	

	*count = interface_mib_count;
    ifnetaddr = ifnetaddr_list;	
	return TRUE;
}

bc_boolean if_index_get(bc_char ifDescr[256], bc_int32 *if_index)
{
	if (NULL == ifDescr || NULL == if_index)
	{
		return FALSE;
	}
	
	return TRUE;
}

bc_boolean kt_snmp_interface_count_get(bc_int32 *count)
{
	if (NULL == count)
	{
		return FALSE;
	}

	*count = 19;
	return TRUE;
}

bc_boolean snmp_interface_mib_get(struct snmp_if_entry interface_mib[MIB2_SNMP_INTERFACE_COUNT], bc_int32 *count)
{	
	bc_int32 interface_mib_count = 0;	
	
	if (NULL == interface_mib || count == NULL)
	{
		return FALSE;
	}

	_interface_mib_get(interface_mib, &interface_mib_count);

#if 0	
//	interface_mib_count = 0;
	for (index = 0; index < port_count; index++)
	{
		memset(&interface_mib[interface_mib_count], 0, sizeof(interface_mib[interface_mib_count]));
		interface_mib[interface_mib_count].ifIndex = OLT_DEV_ID;		
		interface_mib[interface_mib_count].ifIndex <<= 8;
		interface_mib[interface_mib_count].ifIndex |= 0x01;
		interface_mib[interface_mib_count].ifIndex <<= 8;
		interface_mib[interface_mib_count].ifIndex |= pon_base_info[index].slot_id;
		interface_mib[interface_mib_count].ifIndex <<= 8;
		interface_mib[interface_mib_count].ifIndex |= pon_base_info[index].port_id;
//		interface_mib[interface_mib_count].ifIndex = interface_mib_count + 1;

		sprintf(interface_mib[interface_mib_count].ifDescr, "p%d/%d", pon_base_info[index].slot_id, pon_base_info[index].port_id);
		interface_mib[interface_mib_count].ifType = 117;
		interface_mib[interface_mib_count].ifMtu = pon_base_info[index].mtu;
		interface_mib[interface_mib_count].ifSpeed = 1000000000;
		memcpy(interface_mib[interface_mib_count].ifPhysAddress, pon_base_info[index].port_mac, sizeof(pon_base_info[index].port_mac));
		interface_mib[interface_mib_count].ifAdminStatus = pon_base_info[index].status;
		interface_mib[interface_mib_count].ifOperStatus = pon_base_info[index].status;
		interface_mib[interface_mib_count].ifLastChange = 0;
		interface_mib[interface_mib_count].ifSpecific.subid_list_length = 2;
		interface_mib_count++;
	}
	
	bc_int32 index = 0;
	memset(&pon_base_info, 0, sizeof(pon_base_info));
	memset(&sni_attrbt, 0, sizeof(sni_attrbt));
	memset(&cur_stats_info, 0, sizeof(cur_stats_info));
	
	api_ret = ktep_get_bc_pon_info_list(pon_base_info, &port_count);
	if (api_ret != KTEP_NO_ERROR)
	{
		PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_get_bc_pon_info_list() failed:api_ret = %x\r\n", api_ret);
		return FALSE;
	}
	
	ktep_pon_port_base_info_t pon_base_info[MAX_OLT_PON_PORT_COUNT];
	ktep_sni_attrbt_t sni_attrbt[MAX_OLT_SNI_PORT_COUNT];
	ktep_cur_info_t cur_stats_info[MAX_OLT_PORT_COUNT];
	bc_int32 port_count = 0;	
	bc_char		ifDescr[256];
	bc_int32	i = 0;
	ktep_errcode_e api_ret = KTEP_NO_ERROR;	
	
	api_ret = ktep_get_sni_attrbt_list(sni_attrbt, &port_count);
	if (api_ret != KTEP_NO_ERROR)
	{
		PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_get_sni_attrbt_list() failed:api_ret = %x\r\n", api_ret);
		return FALSE;
	}
	
	for (index = 0; index < port_count; index++)
	{
		memset(&interface_mib[interface_mib_count], 0, sizeof(interface_mib[interface_mib_count]));
		interface_mib[interface_mib_count].ifIndex = OLT_DEV_ID;
		interface_mib[interface_mib_count].ifIndex <<= 8;
		interface_mib[interface_mib_count].ifIndex |= 2;
		interface_mib[interface_mib_count].ifIndex <<= 8;
		interface_mib[interface_mib_count].ifIndex |= sni_attrbt[index].slot_id;
		interface_mib[interface_mib_count].ifIndex <<= 8;
		interface_mib[interface_mib_count].ifIndex |= sni_attrbt[index].port_id;
//		interface_mib[interface_mib_count].ifIndex = interface_mib_count + 1;
		
		sprintf(interface_mib[interface_mib_count].ifDescr, "s%d/%d", sni_attrbt[index].slot_id, sni_attrbt[index].port_id);
		interface_mib[interface_mib_count].ifType = 117;
		interface_mib[interface_mib_count].ifMtu = sni_attrbt[index].mtu;
		interface_mib[interface_mib_count].ifSpeed = 1000000000;
		memcpy(interface_mib[interface_mib_count].ifPhysAddress, sni_attrbt[index].port_mac, sizeof(sni_attrbt[index].port_mac));
		interface_mib[interface_mib_count].ifAdminStatus = sni_attrbt[index].status;
		interface_mib[interface_mib_count].ifOperStatus = sni_attrbt[index].status;
		interface_mib[interface_mib_count].ifLastChange = 0;
		interface_mib[interface_mib_count].ifSpecific.subid_list_length = 2;
		interface_mib_count++;
	}
	
	api_ret = ktep_get_all_cur_stats(OLT_DEV_ID, cur_stats_info, &port_count);
	if (api_ret != KTEP_NO_ERROR)
	{
		PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_get_all_cur_stats() failed:api_ret = %x\r\n", api_ret);
		return FALSE;
	}	
		
	for (index = 0; index < port_count; index++)
	{
		sprintf(ifDescr, "%s%d/%d", cur_stats_info[index].port_id.port_type == PORT_TYPE_SNI ? "s" : "p",
									cur_stats_info[index].port_id.slot_id, cur_stats_info[index].port_id.port_id);		
		for (i = 0; i < interface_mib_count; i++)
		{
			if (strcmp(interface_mib[i].ifDescr, ifDescr) == 0)
			{
				interface_mib[i].ifInOctets = cur_stats_info[index].stats_info.in_octets + 1;
				interface_mib[i].ifInUcastPkts = cur_stats_info[index].stats_info.in_pkts + 1;
				interface_mib[i].ifInNUcastPkts = cur_stats_info[index].stats_info.in_brdcst_pkts + cur_stats_info[index].stats_info.in_mltcst_pkts + 1;
				interface_mib[i].ifInDiscards = cur_stats_info[index].stats_info.in_drop_evnts + 1;
				interface_mib[i].ifInErrors =  cur_stats_info[index].stats_info.in_undersize_pkts + cur_stats_info[index].stats_info.in_oversize_pkts + cur_stats_info[index].stats_info.in_crc_err_pkts + 1;
				interface_mib[i].ifInUnknownProtos = 0;
				
				interface_mib[i].ifOutOctets = cur_stats_info[index].stats_info.out_octets + 1;
				interface_mib[i].ifOutUcastPkts = cur_stats_info[index].stats_info.out_pkts + 1;
				interface_mib[i].ifOutNUcastPkts = cur_stats_info[index].stats_info.out_brdcst_pkts + cur_stats_info[index].stats_info.out_mltcst_pkts + 1;
				interface_mib[i].ifOutDiscards = cur_stats_info[index].stats_info.out_drop_evnts + 1;
				interface_mib[i].ifOutErrors = cur_stats_info[index].stats_info.out_undersize_pkts + cur_stats_info[index].stats_info.out_oversize_pkts + cur_stats_info[index].stats_info.out_crc_err_pkts + 1;
				interface_mib[i].ifOutQLen = 0;
				break;
			}
		}											
	}
#endif

	*count = interface_mib_count;
	
	return TRUE;
}

bc_boolean snmp_ifx_mib_get(struct snmp_ifx_entry ifx_mib[MIB2_SNMP_INTERFACE_COUNT], bc_int32 *count)
{
	struct snmp_if_entry interface_mib[MIB2_SNMP_INTERFACE_COUNT];
	bc_int32	interface_mib_count = 0;
	bc_int32	index = 0;

	memset(interface_mib, 0, sizeof(interface_mib));
	_interface_mib_get(interface_mib, &interface_mib_count);

	for (index = 0; index < interface_mib_count; index++)
	{
		memset(&ifx_mib[index], 0, sizeof(ifx_mib[index]));
		ifx_mib[index].ifIndex = interface_mib[index].ifIndex;
		strcpy(ifx_mib[index].ifName, interface_mib[index].ifDescr);
		ifx_mib[index].ifHighSpeed = interface_mib[index].ifSpeed;
		strcpy(ifx_mib[index].ifAlias, interface_mib[index].ifDescr);
	}
	*count = interface_mib_count;

	
#if 0
	ktep_pon_port_base_info_t pon_base_info[MAX_OLT_PON_PORT_COUNT];
	ktep_sni_attrbt_t sni_attrbt[MAX_OLT_SNI_PORT_COUNT];
	ktep_cur_info_t cur_stats_info[MAX_OLT_PORT_COUNT];
	kt_snmp_interface_mib_t interface_mib[MIB2_SNMP_INTERFACE_COUNT];
	bc_int32	interface_mib_count = 0;	
	bc_int32	port_count = 0;	
	bc_int32	index = 0;
	bc_int32	ifIndex = 0;
	bc_int32	i = 0;
	ktep_errcode_e api_ret = KTEP_NO_ERROR;	
	
	if (NULL == ifx_mib || count == NULL)
	{
		return FALSE;
	}

	memset(&pon_base_info, 0, sizeof(pon_base_info));
	memset(&sni_attrbt, 0, sizeof(sni_attrbt));
	memset(&cur_stats_info, 0, sizeof(cur_stats_info));
	memset(&interface_mib, 0, sizeof(interface_mib));

	_interface_mib_get(interface_mib, &interface_mib_count);

	for (index = 0; index < interface_mib_count; index++)
	{
		memset(&ifx_mib[index], 0, sizeof(ifx_mib[index]));
		ifx_mib[index].ifIndex = interface_mib[index].ifIndex;
		strcpy(ifx_mib[index].ifName, interface_mib[index].ifDescr);
		ifx_mib[index].ifHighSpeed = interface_mib[index].ifSpeed;
		strcpy(ifx_mib[index].ifAlias, interface_mib[index].ifDescr);
	}
	
	api_ret = ktep_get_bc_pon_info_list(pon_base_info, &port_count);
	if (api_ret != KTEP_NO_ERROR)
	{
		PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_get_bc_pon_info_list() failed:api_ret = %x\r\n", api_ret);
		return FALSE;
	}
	for (index = 0; index < port_count; index++)
	{
		memset(&ifx_mib[interface_mib_count], 0, sizeof(ifx_mib[interface_mib_count]));
		ifx_mib[interface_mib_count].ifIndex = OLT_DEV_ID;		
		ifx_mib[interface_mib_count].ifIndex <<= 8;
		ifx_mib[interface_mib_count].ifIndex |= 0x01;
		ifx_mib[interface_mib_count].ifIndex <<= 8;
		ifx_mib[interface_mib_count].ifIndex |= pon_base_info[index].slot_id;
		ifx_mib[interface_mib_count].ifIndex <<= 8;
		ifx_mib[interface_mib_count].ifIndex |= pon_base_info[index].port_id;
//		interface_mib[interface_mib_count].ifIndex = interface_mib_count + 1;

		strcpy(ifx_mib[interface_mib_count].ifName, pon_base_info[index].port_name);
		strcpy(ifx_mib[interface_mib_count].ifAlias, pon_base_info[index].port_name);
		interface_mib_count++;
	}
	
	api_ret = ktep_get_sni_attrbt_list(sni_attrbt, &port_count);
	if (api_ret != KTEP_NO_ERROR)
	{
		PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_get_sni_attrbt_list() failed:api_ret = %x\r\n", api_ret);
		return FALSE;
	}
	
	for (index = 0; index < port_count; index++)
	{
		memset(&ifx_mib[interface_mib_count], 0, sizeof(ifx_mib[interface_mib_count]));
		ifx_mib[interface_mib_count].ifIndex = OLT_DEV_ID;		
		ifx_mib[interface_mib_count].ifIndex <<= 8;
		ifx_mib[interface_mib_count].ifIndex |= 0x02;
		ifx_mib[interface_mib_count].ifIndex <<= 8;
		ifx_mib[interface_mib_count].ifIndex |= sni_attrbt[index].slot_id;
		ifx_mib[interface_mib_count].ifIndex <<= 8;
		ifx_mib[interface_mib_count].ifIndex |= sni_attrbt[index].port_id;
//		interface_mib[interface_mib_count].ifIndex = interface_mib_count + 1;

		strcpy(ifx_mib[interface_mib_count].ifName, sni_attrbt[index].port_name);
		strcpy(ifx_mib[interface_mib_count].ifAlias, sni_attrbt[index].port_name);
		interface_mib_count++;
	}
	
	api_ret = ktep_get_all_cur_stats(OLT_DEV_ID, cur_stats_info, &port_count);
	if (api_ret != KTEP_NO_ERROR)
	{
		PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_get_all_cur_stats() failed:api_ret = %x\r\n", api_ret);
		return FALSE;
	}	
		
	for (index = 0; index < port_count; index++)
	{
		ifIndex = OLT_DEV_ID;		
		ifIndex <<= 8;
		if (cur_stats_info[index].port_id.port_type == PORT_TYPE_SNI)
		{
			ifIndex |= 0x02;
		}
		else
		{
			ifIndex |= 0x01;
		}
		ifIndex <<= 8;
		ifIndex |= cur_stats_info[index].port_id.slot_id;
		ifIndex <<= 8;
		ifIndex |= cur_stats_info[index].port_id.port_id;
#if 0		
		printf("ifIndex = %d index = %d slot_id = %d port_type = %d port_id = %d\r\n", 
			ifIndex, index, cur_stats_info[index].port_id.slot_id, cur_stats_info[index].port_id.port_type, cur_stats_info[index].port_id.port_id);
		
		printf("in_mltcst_pkts = %llu in_brdcst_pkts = %llu out_mltcst_pkts = %llu out_brdcst_pkts = %llu\r\n", 
			cur_stats_info[index].stats_info.in_mltcst_pkts, cur_stats_info[index].stats_info.in_brdcst_pkts, 
			cur_stats_info[index].stats_info.out_mltcst_pkts, cur_stats_info[index].stats_info.out_brdcst_pkts);
		
		printf("in_octets = %llu in_pkts = %llu\r\n", 
			cur_stats_info[index].stats_info.in_octets, cur_stats_info[index].stats_info.in_pkts);
		
		printf("out_octets = %llu out_pkts = %llu\r\n", 
			cur_stats_info[index].stats_info.out_octets, cur_stats_info[index].stats_info.out_pkts);
#endif
		for (i = 0; i < interface_mib_count; i++)
		{
			if (ifx_mib[i].ifIndex == ifIndex)
			{
				ifx_mib[i].ifInMulticastPkts = cur_stats_info[index].stats_info.in_mltcst_pkts;
				ifx_mib[i].ifInBroadcastPkts = cur_stats_info[index].stats_info.in_brdcst_pkts;
				
				ifx_mib[i].ifOutMulticastPkts = cur_stats_info[index].stats_info.out_mltcst_pkts;
				ifx_mib[i].ifOutBroadcastPkts = cur_stats_info[index].stats_info.out_brdcst_pkts;
				ifx_mib[i].ifHCInOctets = cur_stats_info[index].stats_info.in_octets;
				ifx_mib[i].ifHCInUcastPkts = cur_stats_info[index].stats_info.in_pkts;
				ifx_mib[i].ifHCInMulticastPkts = cur_stats_info[index].stats_info.in_mltcst_pkts;
				ifx_mib[i].ifHCInBroadcastPkts = cur_stats_info[index].stats_info.in_brdcst_pkts;
				ifx_mib[i].ifHCOutOctets = cur_stats_info[index].stats_info.out_octets;
				ifx_mib[i].ifHCOutUcastPkts = cur_stats_info[index].stats_info.out_pkts;
				ifx_mib[i].ifHCOutMulticastPkts = cur_stats_info[index].stats_info.out_mltcst_pkts;
				ifx_mib[i].ifHCOutBroadcastPkts = cur_stats_info[index].stats_info.out_brdcst_pkts;
				
				ifx_mib[i].ifLinkUpDownTrapEnable = 1;
				ifx_mib[i].ifHighSpeed = 1000000000;
				ifx_mib[i].ifPromiscuousMode = 2;
				ifx_mib[i].ifConnectorPresent = 1;
				ifx_mib[i].ifCounterDiscontinuityTime = 0;
				break;
			}
		}
	}
	
	*count = interface_mib_count;
#endif	
	return TRUE;
}

bc_boolean snmp_interface_ifAdminStatus_set(bc_int32 ifIndex, bc_int32 ifAdminStatus)
{
#if 0
	ktep_port_index_t 	port_id;
	bc_boolean			enable = TRUE;
	ktep_errcode_e		api_ret = KTEP_NO_ERROR;

	memset(&port_id, 0, sizeof(port_id));
	
	if (1 == ifAdminStatus)
	{
		enable = TRUE;
	}
	else if (2 == ifAdminStatus)
	{
		enable = FALSE;
	}
	
	port_id.slot_id = (ifIndex >> 8) & 0xff;
	port_id.port_id = ifIndex  & 0xff;
	if (((ifIndex >> 8) & 0xff) == 1)
	{
		port_id.port_type = PORT_TYPE_OLT_PON;
		api_ret = ktep_set_bc_pon_enable(port_id, enable);
		if (api_ret != KTEP_NO_ERROR)
		{
			PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_set_bc_pon_enable() failed:api_ret = %x\r\n", api_ret);
			return FALSE;
		}
	}
	else if (((ifIndex >> 8) & 0xff) == 2)
	{
		port_id.port_type = PORT_TYPE_SNI;
		api_ret = ktep_set_sni_attrbt_port_enable(port_id, enable);
		if (api_ret != KTEP_NO_ERROR)
		{
			PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_set_sni_attrbt_port_enable() failed:api_ret = %x\r\n", api_ret);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
#endif	
	return TRUE;
}

bc_boolean snmp_ifx_ifAlias_set(bc_int32 ifIndex, bc_char ifAlias[64])
{
#if 0
	ktep_port_index_t 	port_id;
	ktep_errcode_e		api_ret = KTEP_NO_ERROR;
	ktep_pon_port_base_info_t pon_port_base_info;

	memset(&port_id, 0, sizeof(port_id));

	if (NULL == ifAlias)
	{
		return FALSE;
	}	
	
	port_id.slot_id = (ifIndex >> 8) & 0xff;
	port_id.port_id = ifIndex & 0xff;
	if (((ifIndex >> 16) & 0xff) == 1)
	{
		port_id.port_type = PORT_TYPE_OLT_PON;
		api_ret = ktep_get_bc_pon_info(port_id, &pon_port_base_info);
		if (api_ret != KTEP_NO_ERROR)
		{
			PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_get_bc_pon_info() failed:api_ret = %x\r\n", api_ret);
			return FALSE;
		}

		strcpy(pon_port_base_info.port_name, ifAlias);		
		api_ret = ktep_set_bc_pon_info(port_id, &pon_port_base_info);
		if (api_ret != KTEP_NO_ERROR)
		{
			PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_set_bc_pon_info() failed:api_ret = %x\r\n", api_ret);
			return FALSE;
		}
	}
	else if (((ifIndex >> 16) & 0xff) == 2)
	{
		port_id.port_type = PORT_TYPE_SNI;
		api_ret = ktep_set_sni_attrbt_port_name(port_id, ifAlias);
		if (api_ret != KTEP_NO_ERROR)
		{
			PT_LOG(LVL_DBGSMPL, API_STATS_MODULE, "ktep_set_sni_attrbt_port_name() failed:api_ret = %x\r\n", api_ret);
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
#endif	
	return TRUE;
}

void snmp_mib2_ifTable_init(void)
{
	BC_PT_FUN_ENTER(BC_MODULE_SNMP);

	SNMP_REGISTER_TABLE(ifTable);

	SNMP_MIB_FFLUSH_INIT();

	BC_PT_FUN_LEAVE(BC_MODULE_SNMP);
}
