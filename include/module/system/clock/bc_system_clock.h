/************************************************************
  -------------------------------------------------------------------------
  FileName:bc_system_clock.h
  Author:  bain.wang@outlook.com      Version :  1.0        Date:2016-6-9
  Description:  The system clock                  
  Version:                        
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
***********************************************************/
#ifndef _BC_CLOCK_H_
#define _BC_CLOCK_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "bc_common_defs.h"
#include "bc_err.h"


/*************************************************
  Function: bc_clock_date_set
  Description:set system clock
  Input: 
		tmtime:To set the time
  Output:
  Return:
		Success:BC_ERR_OK,Failure:other
  Notes:
  History: 
*************************************************/
bc_err_e bc_clock_date_set(struct tm *tmtime);

/*************************************************
  Function: bc_clock_date_get
  Description:get set system clock
  Input: 
  		tmtime:Save the time pointer
  Output:
  Return:
 		Success:BC_ERR_OK,Failure:other
  Note: 
  History: 
*************************************************/
bc_err_e bc_clock_date_get(struct tm *tmtime);


/*************************************************
  Function: bc_clock_tz_set
  Description: set system clock Time zone
  Input: 
  		tmtime:the Time zone
  Output:
  Return:
 		Success:BC_ERR_OK,Failure:other
  Note: 
  History: 
*************************************************/
bc_err_e bc_clock_tz_set(bc_int8 timezone);

/*************************************************
  Function: bc_clock_tz_get
  Description:get  system clock Time zone
  Input: 
  		tmtime:Save the Time zone pointer
  Output:
  Return:
 		Success:BC_ERR_OK,Failure:other
  Note: 
  History: 
*************************************************/
bc_err_e bc_clock_tz_get(bc_int8 *timezone);



#endif //_BC_CLOCK_H_

