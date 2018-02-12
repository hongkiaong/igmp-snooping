/************************************************************
  -------------------------------------------------------------------------
  FileName:
  Author: bain.wang@outlook.com       Version : 1.0         Date: 2016-2-10
  Description: xx 进程启动文件
  Version: 1.0
  History: 
***********************************************************/

#ifndef __BC_STARTUP_POST_H__
#define __BC_STARTUP_POST_H__

#include "bc_startup.h"

typedef enum {
#ifdef BC_STARTUP_0
	BC_STARTUP_0_E,
#endif
#ifdef BC_STARTUP_1
        BC_STARTUP_1_E,
#endif
#if BC_STARTUP_2
        BC_STARTUP_2_E,
#endif
#if BC_STARTUP_3
        BC_STARTUP_3_E,
#endif
#if BC_STARTUP_4
        BC_STARTUP_4_E,
#endif
#if BC_STARTUP_5
        BC_STARTUP_5_E,
#endif
#if BC_STARTUP_6
        BC_STARTUP_6_E,
#endif
#if BC_STARTUP_7
        BC_STARTUP_7_E,
#endif
#if BC_STARTUP_8
        BC_STARTUP_8_E,
#endif
#if BC_STARTUP_9
        BC_STARTUP_9_E,
#endif
#if BC_STARTUP_10
        BC_STARTUP_10_E,
#endif
#if BC_STARTUP_11
        BC_STARTUP_11_E,
#endif
#if BC_STARTUP_12
        BC_STARTUP_12_E,
#endif
#if BC_STARTUP_13
        BC_STARTUP_13_E,
#endif
#if BC_STARTUP_14
        BC_STARTUP_14_E,
#endif
#if BC_STARTUP_15
        BC_STARTUP_15_E,
#endif
#if BC_STARTUP_16
        BC_STARTUP_16_E,
#endif
#if BC_STARTUP_17
        BC_STARTUP_17_E,
#endif
#if BC_STARTUP_18
        BC_STARTUP_18_E,
#endif
#if BC_STARTUP_19
        BC_STARTUP_19_E,
#endif
#if BC_STARTUP_20
        BC_STARTUP_20_E,
#endif
#if BC_STARTUP_21
        BC_STARTUP_21_E,
#endif
#if BC_STARTUP_22
        BC_STARTUP_22_E,
#endif
#if BC_STARTUP_23
        BC_STARTUP_23_E,
#endif
#if BC_STARTUP_24
        BC_STARTUP_24_E,
#endif
#if BC_STARTUP_25
        BC_STARTUP_25_E,
#endif
#if BC_STARTUP_26
        BC_STARTUP_26_E,
#endif
#if BC_STARTUP_27
        BC_STARTUP_27_E,
#endif
#if BC_STARTUP_28
        BC_STARTUP_28_E,
#endif
#if BC_STARTUP_29
        BC_STARTUP_29_E,
#endif
#if BC_STARTUP_30
        BC_STARTUP_30_E,
#endif
#if BC_STARTUP_31
        BC_STARTUP_31_E,
#endif
#if BC_STARTUP_32
        BC_STARTUP_32_E,
#endif
#if BC_STARTUP_33
        BC_STARTUP_33_E,
#endif
#if BC_STARTUP_34
        BC_STARTUP_34_E,
#endif
#if BC_STARTUP_35
        BC_STARTUP_35_E,
#endif
#if BC_STARTUP_36
        BC_STARTUP_36_E,
#endif
#if BC_STARTUP_37
        BC_STARTUP_37_E,
#endif
#if BC_STARTUP_38
        BC_STARTUP_38_E,
#endif
#if BC_STARTUP_39
        BC_STARTUP_39_E,
#endif
#if BC_STARTUP_40
        BC_STARTUP_40_E,
#endif
#if BC_STARTUP_41
        BC_STARTUP_41_E,
#endif
#if BC_STARTUP_42
        BC_STARTUP_42_E,
#endif
#if BC_STARTUP_43
        BC_STARTUP_43_E,
#endif
#if BC_STARTUP_44
        BC_STARTUP_44_E,
#endif
#if BC_STARTUP_45
        BC_STARTUP_45_E,
#endif
#if BC_STARTUP_46
        BC_STARTUP_46_E,
#endif
#if BC_STARTUP_47
        BC_STARTUP_47_E,
#endif
#if BC_STARTUP_48
        BC_STARTUP_48_E,
#endif
#if BC_STARTUP_49
        BC_STARTUP_49_E,
#endif
#if BC_STARTUP_50
        BC_STARTUP_50_E,
#endif
#if BC_STARTUP_51
        BC_STARTUP_51_E,
#endif
#if BC_STARTUP_52
        BC_STARTUP_52_E,
#endif
#if BC_STARTUP_53
        BC_STARTUP_53_E,
#endif
#if BC_STARTUP_54
        BC_STARTUP_54_E,
#endif
#if BC_STARTUP_55
        BC_STARTUP_55_E,
#endif
#if BC_STARTUP_56
        BC_STARTUP_56_E,
#endif
#if BC_STARTUP_57
        BC_STARTUP_57_E,
#endif
#if BC_STARTUP_58
        BC_STARTUP_58_E,
#endif
#if BC_STARTUP_59
        BC_STARTUP_59_E,
#endif
#if BC_STARTUP_60
        BC_STARTUP_60_E,
#endif
#if BC_STARTUP_61
        BC_STARTUP_61_E,
#endif
#if BC_STARTUP_62
        BC_STARTUP_62_E,
#endif
#if BC_STARTUP_63
        BC_STARTUP_63_E,
#endif
#if BC_STARTUP_64
        BC_STARTUP_64_E,
#endif
#if BC_STARTUP_65
        BC_STARTUP_65_E,
#endif
#if BC_STARTUP_66
        BC_STARTUP_66_E,
#endif
#if BC_STARTUP_67
        BC_STARTUP_67_E,
#endif
#if BC_STARTUP_68
        BC_STARTUP_68_E,
#endif
#if BC_STARTUP_69
        BC_STARTUP_69_E,
#endif
#if BC_STARTUP_70
        BC_STARTUP_70_E,
#endif
#if BC_STARTUP_71
        BC_STARTUP_71_E,
#endif
#if BC_STARTUP_72
        BC_STARTUP_72_E,
#endif
#if BC_STARTUP_73
        BC_STARTUP_73_E,
#endif
#if BC_STARTUP_74
        BC_STARTUP_74_E,
#endif
#if BC_STARTUP_75
        BC_STARTUP_75_E,
#endif
#if BC_STARTUP_76
        BC_STARTUP_76_E,
#endif
#if BC_STARTUP_77
        BC_STARTUP_77_E,
#endif
#if BC_STARTUP_78
        BC_STARTUP_78_E,
#endif
#if BC_STARTUP_79
        BC_STARTUP_79_E,
#endif
#if BC_STARTUP_80
        BC_STARTUP_80_E,
#endif
#if BC_STARTUP_81
        BC_STARTUP_81_E,
#endif
#if BC_STARTUP_82
        BC_STARTUP_82_E,
#endif
#if BC_STARTUP_83
        BC_STARTUP_83_E,
#endif
#if BC_STARTUP_84
        BC_STARTUP_84_E,
#endif
#if BC_STARTUP_85
        BC_STARTUP_85_E,
#endif
#if BC_STARTUP_86
        BC_STARTUP_86_E,
#endif
#if BC_STARTUP_87
        BC_STARTUP_87_E,
#endif
#if BC_STARTUP_88
        BC_STARTUP_88_E,
#endif
#if BC_STARTUP_89
        BC_STARTUP_89_E,
#endif
#if BC_STARTUP_90
        BC_STARTUP_90_E,
#endif
#if BC_STARTUP_91
        BC_STARTUP_91_E,
#endif
#if BC_STARTUP_92
        BC_STARTUP_92_E,
#endif
#if BC_STARTUP_93
        BC_STARTUP_93_E,
#endif
#if BC_STARTUP_94
        BC_STARTUP_94_E,
#endif
#if BC_STARTUP_95
        BC_STARTUP_95_E,
#endif
#if BC_STARTUP_96
        BC_STARTUP_96_E,
#endif
#if BC_STARTUP_97
        BC_STARTUP_97_E,
#endif
#if BC_STARTUP_98
        BC_STARTUP_98_E,
#endif
#if BC_STARTUP_99
        BC_STARTUP_99_E,
#endif
#if BC_STARTUP_100
        BC_STARTUP_100_E,
#endif
	BC_STARTUP_NUM_E
} bc_startup_pregress_e;


#endif

