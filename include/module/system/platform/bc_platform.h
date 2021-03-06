/************************************************************
  -------------------------------------------------------------------------
  FileName:bc_platform.h
  Author:  bain.wang@outlook.com      Version :  1.0        Date:2016-6-9
  Description:    Platform debugging srv                
  Version:                        
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
***********************************************************/
#ifndef _BC_PLATFORM_H_
#define _BC_PLATFORM_H_
#include "bc_common_defs.h"
#include "bc_err.h"

#define  MAX_I2C_BLOCK_LEN   32

#if 1

/*************************************************
  Function: bc_dev_thread_show
  Description:dump所有 线程信息
  Input: 

  Output:
  Return:
		void
  Notes:
  History: 
*************************************************/
extern bc_err_e bc_dev_thread_show();

/*************************************************
  Function: bc_bc_timer_show
  Description:dump 所有定时器相关信息
  Input: 
  
  Output:
  Return:
 		void
  Note: 
  History: 
*************************************************/
extern bc_err_e bc_bc_timer_show();

/*************************************************
  Function: bc_bc_sem_show
  Description:dump所有信号量的状态名称数量等信息
  Input:  
 		 
  Output:
  Return:
  		void
  Note: 
  History: 
*************************************************/
extern bc_err_e bc_bc_sem_show();

/*************************************************
  Function: bc_bc_mutex_show
  Description:dump 所有互斥锁的状态名称等信息
  Input: 
  
  Output:
  Return:
		void
  Note: 
  History: 
*************************************************/
extern bc_err_e bc_bc_mutex_show();


/*************************************************
  Function: bc_bc_mem_show
  Description:dump 所有内存分配的情况
  Input: 
	
  Output:
  Return:
  		void
  Note: 
  History: 
*************************************************/
extern bc_err_e bc_bc_mem_show();


/*************************************************
  Function: bc_bc_queue_show
  Description:dump所有队列的信息
  Input: 

  Output:
  		
  Return:
 		
  Note: 用于debug，查看队列是否占满
  History: 
*************************************************/ 
extern bc_err_e bc_bc_queue_show();

extern bc_err_e bc_bc_queue_dump_single(bc_uint32 que_id);

#endif

#endif //_BC_PLATFORM_H_

