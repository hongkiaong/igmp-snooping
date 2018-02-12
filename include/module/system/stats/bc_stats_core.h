#ifndef __BC_STATS_CORE_H__
#define __BC_STATS_CORE_H__


#include "bc_stats_def.h"
#include "bc_stats_db_threshold.h"
#include "bc_stats_db_perf.h"


#if 1
/******************************************************************************\
  Function   : bc_stats_core_temperature_get
  Description: ��ȡ�¶�
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_temperature_get(bc_uint16 *val);

/******************************************************************************\
  Function   : bc_stats_db_oam_port_get_list
  Description: ��ȡ����OAM�˿�
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_oam_port_get_info(int oam_port_id, oam_port_info_t *oam_port_info);

/******************************************************************************\
  Function   : bc_stats_core_oam_port_get_list
  Description: ��ȡȫ��OAM�˿�
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_oam_port_get_list(bc_stats_oam_port_info_t *oam_port_info);

/******************************************************************************\
  Function   : bc_stats_core_optical_get_info
  Description: ��ȡ������ģ��
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_optical_get_info(int optical_id, optical_info_t *optical_info);

/******************************************************************************\
  Function   : bc_stats_core_optical_get_list
  Description: ��ȡȫ����ģ��
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_optical_get_list(bc_stats_optical_info_t *optical_info);

/******************************************************************************\
  Function   : bc_stats_core_port_get_info
  Description: ��ȡ�����˿�
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_port_get_info(int port_id, port_info_t *port_info);

/******************************************************************************\
  Function   : bc_stats_core_port_get_list
  Description: ��ȡȫ���˿�
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_port_get_list(bc_stats_port_info_t *port_info);

/******************************************************************************\
  Function   : bc_stats_core_ptp_port_get_info
  Description: ��ȡ����PTP�˿�
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_ptp_port_get_info(int ptp_port_id, ptp_port_info_t *ptp_port_info);

/******************************************************************************\
  Function   : bc_stats_core_ptp_port_get_list
  Description: ��ȡȫ��PTP�˿�
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_ptp_port_get_list(bc_stats_ptp_port_info_t *ptp_port_info);

/******************************************************************************\
  Function   : bc_stats_core_ptp_get_info
  Description: ��ȡ����PTP
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_ptp_get_info(ptp_info_t *ptp_info);

/******************************************************************************\
  Function   : bc_stats_core_vp_get_info
  Description: ��ȡ����VP
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_vp_get_info(int vp_id, vp_info_t *vp_info);

/******************************************************************************\
  Function   : bc_stats_core_vp_get_list
  Description: ��ȡȫ��VP
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_vp_get_list(bc_stats_vp_info_t *vp_info);

/******************************************************************************\
  Function   : bc_stats_core_vc_get_info
  Description: ��ȡ����VC
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_vc_get_info(int vc_id, vc_info_t *vc_info);

/******************************************************************************\
  Function   : bc_stats_core_vc_get_list
  Description: ��ȡȫ��VC
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_vc_get_list(bc_stats_vc_info_t *vc_info);

/******************************************************************************\
  Function   : bc_stats_core_e1_get_info
  Description: ��ȡ����E1
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_e1_get_info(int e1_id, e1_info_t *e1_info);

/******************************************************************************\
  Function   : bc_stats_core_e1_get_list
  Description: ��ȡȫ��E1
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_e1_get_list(bc_stats_e1_info_t *e1_info);

/******************************************************************************\
  Function   : bc_stats_get_cur_info
  Description: ��ȡ��ǰͳ��
  Return     :
  Others     : per 1s fflush
\******************************************************************************/
int bc_stats_core_get_cur_info(bc_stats_info_t *stats_info);

/******************************************************************************\
  Function   : bc_stats_get_15min_info
  Description: ��ȡ���µ�15����ͳ��
  Return     :
  Others     : per 1s fflush
\******************************************************************************/
int bc_stats_core_get_15min_info(bc_stats_info_t *stats_info);

/******************************************************************************\
  Function   : bc_stats_get_24hour_info
  Description: ��ȡ���µ�24Сʱͳ��
  Return     :
  Others     : per 1s fflush
\******************************************************************************/
int bc_stats_core_get_24hour_info(bc_stats_info_t *stats_info);

/******************************************************************************\
  Function   : bc_stats_get_last_15min_incream
  Description: ��ȡ����15�����뵱ǰͳ�ƵĲ�ֵ
  Return     :
  Others     : per 1s fflush
\******************************************************************************/
int bc_stats_core_get_last_15min_incream(bc_stats_info_t *stats_info);

/******************************************************************************\
  Function   : bc_stats_get_last_24hour_incream
  Description: ��ȡ����24Сʱ�뵱ǰͳ�ƵĲ�ֵ
  Return     :
  Others     : per 1s fflush
\******************************************************************************/
int bc_stats_core_get_last_24hour_incream(bc_stats_info_t *stats_info);

/******************************************************************************\
  Function   : bc_stats_get_15min_item_num
  Description: ȡ15���Ӽ�¼����
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_get_15min_item_num(int *num);

/******************************************************************************\
  Function   : bc_stats_get_24hour_item_num
  Description: ȡ24Сʱ��¼����
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_get_24hour_item_num(int *num);

/******************************************************************************\
  Function   : bc_stats_get_15min_item_total_num
  Description: ȡ15���Ӽ�¼������
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_get_15min_item_total_num(int *num);

/******************************************************************************\
  Function   : bc_stats_get_24hour_item_total_num
  Description: ȡ24Сʱ��¼������
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_get_24hour_item_total_num(int *num);

/******************************************************************************\
  Function   : bc_stats_core_del_info
  Description: ������ͳ����Ϣ
  Return     :
  Others     : 
\******************************************************************************/
bc_err_e bc_stats_core_del_info();

/******************************************************************************\
  Function   : bc_stats_core_del_15min_info
  Description: ������ͳ����Ϣ
  Return     :
  Others     : 
\******************************************************************************/
bc_err_e bc_stats_core_del_15min_info();

/******************************************************************************\
  Function   : bc_stats_core_del_24hour_info
  Description: ������ͳ����Ϣ
  Return     :
  Others     : 
\******************************************************************************/
bc_err_e bc_stats_core_del_24hour_info();

#endif


/******************************************************************************\
  Function   : bc_stats_core_threshold_set_info
  Description: 
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_threshold_set_info(bc_stats_threshold_info_t threshold_info);

/******************************************************************************\
  Function   : bc_stats_core_threshold_get_info
  Description: 
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_threshold_get_info(bc_stats_threshold_info_t *threshold_info);

/******************************************************************************\
  Function   : bc_stats_core_ex_cc_get_info
  Description: 
  Return     :
  Others     : 
\******************************************************************************/
int bc_stats_core_ex_cc_get_info(bc_stats_ex_cc_info_t *info);

/******************************************************************************\
  Function   : bc_stats_update_polling
  Description: 
  Return     :
  Others     : 
\******************************************************************************/
void bc_stats_update_polling(void);

bc_err_e bc_stats_init();


#endif