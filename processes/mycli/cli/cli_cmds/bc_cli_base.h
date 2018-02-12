/*******************************************************************************
  -------------------------------------------------------------------------
  [File name]  : \\BC\cli\cli_cmds\bc_cli_base.h
  [Author]     : bain.wang@outlook.com
  [Version]    : 1.0
  [Date]       : 2017-06-10
  [Description]:

  [Others]     :

  [Function List]:
	1. ....
	2. ....
  [History]:
	 D a t e	Modification										  Initials
	---------- --------------										 ----------
	2016-06-25	Created 									bain.wang@outlook.com
	2017-06-04	Created 									bain.wang@outlook.com
*******************************************************************************/

#ifndef __IN_BC_CLI_BASE_H__
#define __IN_BC_CLI_BASE_H__

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								ͷ   ��   ��								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#include "bc_cli.h"
#include "bc_cli_text.h"
//#include "l1/port/bc_port_def.h"
//#include "l2/fdb/bc_fdb_def.h"




////////////////////////////////////////////////////////////////////////////////
//																			  //
//								��   ��   ��								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#define CLI_LAN						(bc_cli_get_lan() == CLI_LAN_CN ? CMD_ML_CN : CMD_ML_EN)

#define CMD_GET_STRING(string_id)  	((const char *)bc_cli_menu[string_id][CLI_LAN]) // bc_cli_menu


#define CMD_HELP_KEY_EN_ETHTYPE_ipv4 			"#Internet Protocol version 4 (type 0x0800).#"
#define CMD_HELP_KEY_EN_ETHTYPE_ipv6 			"#Internet Protocol version 6 (type 0x86DD).#"
#define CMD_HELP_KEY_EN_ETHTYPE_arp 			"#Address Resolution Protocol (type 0x0806).#"
#define CMD_HELP_KEY_EN_ETHTYPE_rarp 			"#Reverse Address Resolution Protocol (type 0x8035).#"
#define CMD_HELP_KEY_EN_ETHTYPE_fr	 			"#Frame Relay (type 0x0808).#"
#define CMD_HELP_KEY_EN_ETHTYPE_ppp 			"#Point-to-Point Protocol (type 0x880B).#"
#define CMD_HELP_KEY_EN_ETHTYPE_gsmp 			"#General Switch Management Protocol (type 0x880C).#"
#define CMD_HELP_KEY_EN_ETHTYPE_mpls 			"#Multiprotocol Label Switching (type 0x8847).#"
#define CMD_HELP_KEY_EN_ETHTYPE_mps 			"#Multiprotocol Label Switching with Upstream-assigned Label (type 0x8848).#"
#define CMD_HELP_KEY_EN_ETHTYPE_mcap 			"#Multicast Channel Allocation Protocol (type 0x8861).#"
#define CMD_HELP_KEY_EN_ETHTYPE_pppoed 			"#PPP over Ethernet Discovery Stage (type 0x8863).#"
#define CMD_HELP_KEY_EN_ETHTYPE_pppoes 			"#PPP over Ethernet Session Stage (type 0x8864).#"
#define CMD_HELP_KEY_EN_ETHTYPE_ctag 			"#Customer VLAN Tag Type (type 0x8100).#"
#define CMD_HELP_KEY_EN_ETHTYPE_epon 			"#Ethernet Passive Optical Network (type 0x8808).#"
#define CMD_HELP_KEY_EN_ETHTYPE_pbnac 			"#Port-based network access control (type 0x888E).#"
#define CMD_HELP_KEY_EN_ETHTYPE_stag 			"#Service VLAN tag identifier (type 0x88A8).#"
#define CMD_HELP_KEY_EN_ETHTYPE_ethexp1 		"#Local Experimental Ethertype 1 (type 0x88B5).#"
#define CMD_HELP_KEY_EN_ETHTYPE_ethexp2 		"#Local Experimental Ethertype 2 (type 0x88B6).#"
#define CMD_HELP_KEY_EN_ETHTYPE_ethoui 			"#OUI Extended Ethertype (type 0x88B7).#"
#define CMD_HELP_KEY_EN_ETHTYPE_preauth 		"#Pre-Authentication (type 0x88C7).#"
#define CMD_HELP_KEY_EN_ETHTYPE_lldp 			"#Link Layer Discovery Protocol (type 0x88CC).#"
#define CMD_HELP_KEY_EN_ETHTYPE_macsec 			"#Media Access Control Security (type 0x88E5).#"
#define CMD_HELP_KEY_EN_ETHTYPE_mvrp 			"#Multiple VLAN Registration Protocol (type 0x88F5).#"
#define CMD_HELP_KEY_EN_ETHTYPE_mmrp 			"#Multiple Multicast Registration Protocol (type 0x88F6).#"
#define CMD_HELP_KEY_EN_ETHTYPE_frrr 			"#Fast Roaming Remote Request (type 0x890D).#"
#define CMD_HELP_ARG_EN_ETHTYPE 				"#Ethernet type. 2bytes length.#"

#define STR_CMD_RULE_ETH_TYPE \
			"{ " \
				"ipv4 " 				CMD_HELP_KEY_EN_ETHTYPE_ipv4 CMD_HELP_KEY_EN_ETHTYPE_ipv4 \
				"| " \
				"ipv6 " 				CMD_HELP_KEY_EN_ETHTYPE_ipv6 CMD_HELP_KEY_EN_ETHTYPE_ipv6 \
				"| " \
				"arp "					CMD_HELP_KEY_EN_ETHTYPE_arp CMD_HELP_KEY_EN_ETHTYPE_arp \
				"| " \
				"rarp " 				CMD_HELP_KEY_EN_ETHTYPE_rarp CMD_HELP_KEY_EN_ETHTYPE_rarp \
				"| " \
				"fr "					CMD_HELP_KEY_EN_ETHTYPE_fr CMD_HELP_KEY_EN_ETHTYPE_fr \
				"| " \
				"ppp "					CMD_HELP_KEY_EN_ETHTYPE_ppp CMD_HELP_KEY_EN_ETHTYPE_ppp \
				"| " \
				"gsmp " 				CMD_HELP_KEY_EN_ETHTYPE_gsmp CMD_HELP_KEY_EN_ETHTYPE_gsmp \
				"| " \
				"mpls " 				CMD_HELP_KEY_EN_ETHTYPE_mpls CMD_HELP_KEY_EN_ETHTYPE_mpls \
				"| " \
				"mps "					CMD_HELP_KEY_EN_ETHTYPE_mps CMD_HELP_KEY_EN_ETHTYPE_mps \
				"| " \
				"mcap " 				CMD_HELP_KEY_EN_ETHTYPE_mcap CMD_HELP_KEY_EN_ETHTYPE_mcap \
				"| " \
				"pppoe-d "				CMD_HELP_KEY_EN_ETHTYPE_pppoed CMD_HELP_KEY_EN_ETHTYPE_pppoed \
				"| " \
				"pppoe-s "				CMD_HELP_KEY_EN_ETHTYPE_pppoes CMD_HELP_KEY_EN_ETHTYPE_pppoes \
				"| " \
				"ctag " 				CMD_HELP_KEY_EN_ETHTYPE_ctag CMD_HELP_KEY_EN_ETHTYPE_ctag \
				"| " \
				"stag " 				CMD_HELP_KEY_EN_ETHTYPE_stag CMD_HELP_KEY_EN_ETHTYPE_stag \
				"| " \
				"ethexp1 "				CMD_HELP_KEY_EN_ETHTYPE_ethexp1 CMD_HELP_KEY_EN_ETHTYPE_ethexp1 \
				"| " \
				"ethexp2 "				CMD_HELP_KEY_EN_ETHTYPE_ethexp2 CMD_HELP_KEY_EN_ETHTYPE_ethexp2 \
				"| " \
				"ethoui "				CMD_HELP_KEY_EN_ETHTYPE_ethoui CMD_HELP_KEY_EN_ETHTYPE_ethoui \
				"| " \
				"preauth "				CMD_HELP_KEY_EN_ETHTYPE_preauth CMD_HELP_KEY_EN_ETHTYPE_preauth \
				"| " \
				"lldp " 				CMD_HELP_KEY_EN_ETHTYPE_lldp CMD_HELP_KEY_EN_ETHTYPE_lldp \
				"| " \
				"mac-sec "				CMD_HELP_KEY_EN_ETHTYPE_macsec CMD_HELP_KEY_EN_ETHTYPE_macsec \
				"| " \
				"mvrp " 				CMD_HELP_KEY_EN_ETHTYPE_mvrp CMD_HELP_KEY_EN_ETHTYPE_mvrp \
				"| " \
				"mmrp " 				CMD_HELP_KEY_EN_ETHTYPE_mmrp CMD_HELP_KEY_EN_ETHTYPE_mmrp \
				"| " \
				"frrr " 				CMD_HELP_KEY_EN_ETHTYPE_frrr CMD_HELP_KEY_EN_ETHTYPE_frrr \
				"| " \
				"<ethernet-protocol-type-hex> "  \
			"} " 


#if 0
2 IGMP Internet ����� 
3 GGP ���ض����� 
4 IP IP �е� IP����װ�� 
5 ST �� 
6 TCP ������� 
7 CBT CBT 
8 EGP �ⲿ����Э�� 
9 IGP �κ�ר���ڲ����� 
��Cisco �������� IGRP��
10 BBN-RCC-MON BBN RCC ���� 
11 NVP-II ��������Э�� 
12 PUP PUP 
13 ARGUS ARGUS 
14 EMCON EMCON 
15 XNET ���������� 
16 CHAOS Chaos 
17 UDP �û����ݱ� 
18 MUX ��·���� 
19 DCN-MEAS DCN ������ϵͳ 
20 HMP �������� 
21 PRM ���ݰ����߲��� 
22 XNS-IDP XEROX NS IDP 
23 TRUNK-1 �� 1 ���� 
24 TRUNK-2 �� 2 ���� 
25 LEAF-1 �� 1 Ҷ 
26 LEAF-2 �� 2 Ҷ 
27 RDP �ɿ�����Э�� 
28 IRTP Internet �ɿ����� 
29 ISO-TP4 ISO ����Э��� 4 �� 
30 NETBLT �������ݴ���Э�� 
31 MFE-NSP MFE �������Э�� 
32 MERIT-INP MERIT �ڵ��Э�� 
33 SEP ˳�򽻻�Э�� 
34 3PC ����������Э�� 
35 IDPR ������·��Э��
36 XTP XTP 
37 DDP ���ݱ�����Э�� 
38 IDPR-CMTP IDPR ������Ϣ����Э��
39 TP++ TP++ ����Э�� 
40 IL IL ����Э�� 
41 IPv6 Ipv6 
42 SDRP ԴҪ��·��Э�� 
43 IPv6-Route IPv6 ��·�ɱ�ͷ 
44 IPv6-Frag IPv6 ��Ƭ�ϱ�ͷ 
45 IDRP ���·��Э�� 
46 RSVP ����Э�� 
47 GRE ͨ��·�ɷ�װ 
48 MHRP �ƶ�����·��Э��
49 BNA BNA 
50 ESP IPv6 �ķ�װ��ȫ���� 
51 AH IPv6 ��������֤��ͷ 
52 I-NLSP ��������㰲ȫ�� TUBA 
53 SWIPE ���ü��ܵ� IP 
54 NARP NBMA ��ַ����Э�� 
55 MOBILE IP �ƶ��� 
56 TLSP ����㰲ȫЭ�� 
ʹ�� Kryptonet ��Կ����
57 SKIP SKIP 
58 IPv6-ICMP ���� IPv6 �� ICMP 
59 IPv6-NoNxt ���� IPv6 ������һ����ͷ 
60 IPv6-Opts IPv6 ��Ŀ��ѡ�� 
61 ���������ڲ�Э�� 
62 CFTP CFTP 
63 ���Ȿ������ 
64 SAT-EXPAK SATNET ���̨ EXPAK 
65 KRYPTOLAN Kryptolan 
66 RVD MIT Զ���������Э�� 
67 IPPC Internet Pluribus ���ݰ����� 
68 ����ֲ�ʽ�ļ�ϵͳ 
69 SAT-MON SATNET ���� 
70 VISA VISA Э�� 
71 IPCV Internet ���ݰ����Ĺ��� 
72 CPNX �����Э��������� 
73 CPHB �����Э�����ź� 
74 WSN ������������ 
75 PVP ���ݰ���ƵЭ�� 
76 BR-SAT-MON ��̨ SATNET ���� 
77 SUN-ND SUN ND PROTOCOL-Temporary 
78 WB-MON WIDEBAND ���� 
79 WB-EXPAK WIDEBAND EXPAK 
80 ISO-IP ISO Internet Э�� 
81 VMTP VMTP 
82 SECURE-VMTP SECURE-VMTP 
83 VINES VINES 
84 TTP TTP 
85 NSFNET-IGP NSFNET-IGP 
86 DGP ��������Э�� 
87 TCF TCF 
88 EIGRP EIGRP 
89 OSPFIGP OSPFIGP 
90 Sprite-RPC Sprite RPC Э�� 
91 LARP �켣��ַ����Э�� 
92 MTP �ಥ����Э�� 
93 AX.25 AX.25 ֡ 
94 IPIP IP �е� IP ��װЭ�� 
95 MICP �ƶ���������Э�� 
96 SCC-SP �ź�ͨѶ��ȫЭ�� 
97 ETHERIP IP �е���̫����װ 
98 ENCAP ��װ��ͷ 
99 ����ר�ü��ܷ��� 
100 GMTP GMTP 
101 IFMP Ipsilon ��������Э�� 
102 PNNI IP �ϵ� PNNI 
103 PIM ������Э��Ķಥ 
104 ARIS ARIS 
105 SCPS SCPS 
106 QNX QNX 
107 A/N ����� 
108 IPComp IP ����ѹ��Э�� 
109 SNP Sitara ����Э�� 
110 Compaq-Peer Compaq �Ե�Э�� 
111 IPX-in-IP IP �е� IPX 
112 VRRP ����·��������Э�� 
113 PGM PGM �ɿ�����Э�� 
114 ���� 0 ��Э�� 
115 L2TP �ڶ�������Э�� 
116 DDX D-II ���ݽ��� (DDX) 
117 IATP ����ʽ��������Э�� 
118 STP �ƻ�����Э�� 
119 SRP SpectraLink ����Э�� 
120 UTI UTI 
121 SMP ���ʼ�Э�� 
122 SM SM 
123 PTP ����͸��Э�� 
124 ISIS over IPv4 
125 FIRE 
126 CRTP Combat ���ߴ���Э�� 
127 CRUDP Combat �����û����ݱ� 
128 SSCOPMCE 
129 IPLT 
130 SPS ��ȫ���ݰ����� 
131 PIPE IP �е�ר�� IP ��װ 
132 SCTP �����ƴ���Э�� 
133 FC ����ͨ�� 
134-254 δ���� 
255 ����
#endif

#define CMD_HELP_KEY_EN_PROTOTYPE_EIGRP 	"#EIGRP (Enhanced Interior Gateway Routing Protocol) (type 88).#"
#define CMD_HELP_KEY_EN_PROTOTYPE_GRE 		"#GRE (Generic Routing Encapsulation) protocol (type 47).#"
#define CMD_HELP_KEY_EN_PROTOTYPE_IPINIP 	"#IPINIP protocol (type 4).#"
#define CMD_HELP_KEY_EN_PROTOTYPE_IGMP 		"#IGMP (Internet Group Management Protocol) (type 2).#"
#define CMD_HELP_KEY_EN_PROTOTYPE_OSPF 		"#OSPF (Open Shortest Path First) protocol (type 89).#"
#define CMD_HELP_KEY_EN_PROTOTYPE_ICMP 		"#ICMP (Internet Control Message Protocol) (type 1).#"
#define CMD_HELP_KEY_EN_PROTOTYPE_UDP 		"#UDP (User Datagram Protocol) (type 17).#"
#define CMD_HELP_KEY_EN_PROTOTYPE_TCP 		"#TCP (Transmission Control Protocol) (type 6).#"

#define STR_CMD_RULE_IP_PRO \
			"{ " \
				"eigrp "				CMD_HELP_KEY_EN_PROTOTYPE_EIGRP CMD_HELP_KEY_EN_PROTOTYPE_EIGRP \
				"| " \
				"gre "					CMD_HELP_KEY_EN_PROTOTYPE_GRE CMD_HELP_KEY_EN_PROTOTYPE_GRE \
				"| " \
				"ipinip "				CMD_HELP_KEY_EN_PROTOTYPE_IPINIP CMD_HELP_KEY_EN_PROTOTYPE_IPINIP \
				"| " \
				"igmp " 				CMD_HELP_KEY_EN_PROTOTYPE_IGMP CMD_HELP_KEY_EN_PROTOTYPE_IGMP \
				"| " \
				"ospf " 				CMD_HELP_KEY_EN_PROTOTYPE_OSPF CMD_HELP_KEY_EN_PROTOTYPE_OSPF \
				"| " \
				"icmp " 				CMD_HELP_KEY_EN_PROTOTYPE_ICMP CMD_HELP_KEY_EN_PROTOTYPE_ICMP \
				"| " \
				"udp "					CMD_HELP_KEY_EN_PROTOTYPE_UDP CMD_HELP_KEY_EN_PROTOTYPE_UDP \
				"| " \
				"tcp "					CMD_HELP_KEY_EN_PROTOTYPE_TCP CMD_HELP_KEY_EN_PROTOTYPE_TCP \
				"| " \
				"<ip-protocol-type> " \
			"} "



#define CMD_HELP_KEY_EN_ETHTYPE_TYPE "#Ethernet type.#"
#define CMD_HELP_KEY_CN_ETHTYPE_TYPE "#��̫������.#"
#define CMD_HELP_KEY_EN_PROTOTYPE_TYPE "#IP protocol type.#"
#define CMD_HELP_KEY_CN_PROTOTYPE_TYPE "#IP Э������.#"
				
#define CMD_HELP_KEY_EN_DSCP "#DSCP.#"
#define CMD_HELP_KEY_CN_DSCP "#DSCP.#"
#define CMD_HELP_ARG_EN_DSCP "#FORMAT: Decimal. Range: [0-63].#"
				
#define CMD_HELP_ARG_EN_MACWITHMASK	"#MAC with mask, FORMAT:XXXX.XXXX.XXXX:XXXX.XXXX.XXXX.#"
#define CMD_HELP_ARG_EN_IPWITHMASK 	"#IP with mask, FORMAT:DDD.DDD.DDD.DDD:DDD.DDD.DDD.DDD.#"


////////////////////////////////////////////////////////////////////////////////
//																			  //
//					�� �� �� ���� �� �塢ö �� �� ��				  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

typedef enum etag_vlan_map_type_type_t
{
	E_VLAN_MAP_TYPE_BIT,
	E_VLAN_MAP_TYPE_BYTE,
	E_VLAN_MAP_TYPE_MAX
} vlan_map_type_e;

#define PRINT_BC_RET(sid, ret)\
	do\
	{\
		if (ret != BC_ERR_OK)\
		{\
			CLI_API_Printf(sid, "Fail.%s!\r\n", bc_msg_err_get(ret, (bc_cli_get_lan() == CLI_LAN_CN ? BC_LAN_CHINESE_E : BC_LAN_ENGLISH_E)));\
		}\
	} while (0)


////////////////////////////////////////////////////////////////////////////////
//																			  //
//						�� �� �� ���� �� �� �� ��					  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
extern const char *bc_cli_menu[CMD_MID_MAXNUM][CMD_ML_MAXNUM];




#endif /* __IN_BC_CLI_BASE_H__ */
