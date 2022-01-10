/* Automation Studio generated header file */
/* Do not edit ! */
/* AsHydCon 3.10.0 */

#ifndef _ASHYDCON_
#define _ASHYDCON_
#ifdef __cplusplus
extern "C" 
{
#endif
#ifndef _AsHydCon_VERSION
#define _AsHydCon_VERSION 3.10.0
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
#ifdef _SG4
		#include "brsystem.h"
		#include "sys_lib.h"
#endif



/* Constants */
#ifdef _REPLACE_CONST
 #define HCR_ERR_DT12S_D_NEG 32250U
 #define HCR_ERR_DT12S_T1_NEG 32251U
 #define HCR_ERR_DT12S_T1_T2_ZERO 32252U
 #define HCR_ERR_DT12S_T2_NEG 32253U
 #define HCR_ERR_DT12S_TF_NEG_ZERO 32254U
 #define HCR_ERR_DT12S_TV_NEG 32255U
 #define HCR_ERR_FIFO_MEM 32256U
 #define HCR_ERR_FIFO_T_FIFO_NEG 32257U
 #define HCR_ERR_JFREE_TSWITCH_NEG 32258U
 #define HCR_ERR_LIN_NODEVEC_EMPTY 32259U
 #define HCR_ERR_LIN_X_NOT_ASC_DESC 32260U
 #define HCR_ERR_LIN_Y_NOT_ASC_DESC 32261U
 #define HCR_ERR_PID_DY_MAX_NEG 32262U
 #define HCR_ERR_PID_D_MODE_INVALID 32263U
 #define HCR_ERR_PID_KW_0_1 32264U
 #define HCR_ERR_PID_K_FBK_NEG 32265U
 #define HCR_ERR_PID_TF_NEG 32266U
 #define HCR_ERR_PID_TF_ZERO 32267U
 #define HCR_ERR_PID_TN_NEG_NEG 32268U
 #define HCR_ERR_PID_TN_POS_NEG 32269U
 #define HCR_ERR_PID_TV_NEG 32270U
 #define HCR_ERR_PID_WX_MAX_MIN 32271U
 #define HCR_ERR_PID_Y_MAX_MIN 32272U
 #define HCR_ERR_PT12_T1_NEG 32273U
 #define HCR_ERR_PT12_T2_NEG 32274U
 #define HCR_ERR_PT1CP_T1_NEG 32275U
 #define HCR_ERR_PT2S_D_NEG 32276U
 #define HCR_ERR_PT2S_TF_NEG_ZERO 32277U
 #define HCR_ERR_SERVO_Y_MIN 32278U
 #define HCR_ERR_SYSTEM_A_CYL 32279U
 #define HCR_ERR_SYSTEM_P_DIFF 32280U
 #define HCR_ERR_SYSTEM_P_SUPPLY 32281U
 #define HCR_ERR_SYSTEM_P_TANK 32282U
 #define HCR_ERR_VALVE_P_DELTA_NOM 32283U
 #define HCR_ERR_VALVE_Q_NOM 32284U
 #define HCR_ERR_VALVE_Y_0 32285U
 #define HCR_ERR_RAMP_T_FALL_NEG 32286U
 #define HCR_ERR_RAMP_T_HOLD_NEG 32287U
 #define HCR_ERR_RAMP_T_RISE_NEG 32288U
 #define HCR_ERR_SPF_A_ACC_NEG_NEG_ZERO 32289U
 #define HCR_ERR_SPF_A_ACC_POS_NEG_ZERO 32290U
 #define HCR_ERR_SPF_A_DEC_NEG_NEG_ZERO 32291U
 #define HCR_ERR_SPF_A_DEC_POS_NEG_ZERO 32292U
 #define HCR_ERR_SPF_NIL_PTR 32293U
 #define HCR_ERR_SPF_T_JOLT_NEG 32294U
 #define HCR_ERR_SPF_VC_NEG_NEG_ZERO 32295U
 #define HCR_ERR_SPF_VC_POS_NEG_ZERO 32296U
 #define HCR_ERR_SYSTEM_H_CYL 32297U
 #define HCR_ERR_SYSTEM_M_CYL 32298U
 #define HCR_ERR_SYSTEM_NIL_PTR 32299U
 #define HCR_ERR_SYSTEM_P_SUPPLY_MAX 32300U
 #define HCR_ERR_SYSTEM_V0_NEG 32301U
 #define HCR_ERR_SYSTEM_V0_POS 32302U
 #define HCR_ERR_SERVO_MODE_FY 32303U
 #define HCR_ERR_SERVO_MODE_VY 32304U
 #define HCR_ERR_PID_NIL_PTR 32305U
 #define HCR_ERR_VALVE_NIL_PTR 32306U
 #define HCR_ERR_NOTCH_B_NEG_ZERO 32307U
 #define HCR_ERR_NOTCH_F0_NEG_ZERO 32308U
 #define HCR_ERR_OMEGA0_X0_CYL 32309U
 #define HCR_ERR_OMEGA0_X_CYL_MIN_MAX 32310U
 #define HCR_ERR_IT1_T1_NEG 32313U
 #define HCR_ERR_IT1_TN_NEG_ZERO 32314U
 #define HCR_ERR_SFG_T_DELTA_NEG_ZERO 32315U
 #define HCR_ERR_SFG_T_DIFF 32316U
 #define HCR_ERR_SFG_T_JOLT_NEG 32317U
 #define HCR_ERR_DT12S_TF_NEG 32318U
 #define HCR_WARN_DT12S_T1_TS 32370U
 #define HCR_WARN_DT12S_T1_ZERO 32371U
 #define HCR_WARN_DT12S_T2_TS 32372U
 #define HCR_WARN_DT12S_T2_ZERO 32373U
 #define HCR_WARN_DT12S_TF_TS 32374U
 #define HCR_WARN_DT12S_TV_TS 32375U
 #define HCR_WARN_DT12S_TV_ZERO 32376U
 #define HCR_WARN_FIFO_T_FIFO_INT 32377U
 #define HCR_WARN_FIFO_T_FIFO_TS 32378U
 #define HCR_WARN_JFREE_TSWITCH_5TS 32379U
 #define HCR_WARN_JFREE_TSWITCH_ZERO 32380U
 #define HCR_WARN_LIN_CRIT_RNESS 32381U
 #define HCR_WARN_LIN_NB_NODE 32382U
 #define HCR_WARN_LIN_X_NOT_ASC_DESC_S 32383U
 #define HCR_WARN_LIN_Y_NOT_ASC_DESC 32384U
 #define HCR_WARN_LIN_Y_NOT_ASC_DESC_S 32385U
 #define HCR_WARN_PID_TF_TS 32386U
 #define HCR_WARN_PID_TN_NEG_TS 32387U
 #define HCR_WARN_PID_TN_POS_TS 32388U
 #define HCR_WARN_PID_TV_TS 32389U
 #define HCR_WARN_PID_YADD_100 32390U
 #define HCR_WARN_PID_YI_MAX 32391U
 #define HCR_WARN_PID_YI_MIN 32392U
 #define HCR_WARN_PID_YMAN_MAX 32393U
 #define HCR_WARN_PID_YMAN_MIN 32394U
 #define HCR_WARN_PT12_T1_TS 32395U
 #define HCR_WARN_PT12_T1_ZERO 32396U
 #define HCR_WARN_PT12_T2_TS 32397U
 #define HCR_WARN_PT12_T2_ZERO 32398U
 #define HCR_WARN_PT1CP_T1_TS 32399U
 #define HCR_WARN_PT1CP_T1_ZERO 32400U
 #define HCR_WARN_PT2S_TF_TS 32401U
 #define HCR_WARN_SERVO_FF_CYL_NEG 32402U
 #define HCR_WARN_SERVO_FF_CYL_POS 32403U
 #define HCR_WARN_SERVO_P_CYL_NEG_HIGH 32404U
 #define HCR_WARN_SERVO_P_CYL_NEG_LOW 32405U
 #define HCR_WARN_SERVO_P_CYL_NEG_NEG 32406U
 #define HCR_WARN_SERVO_P_CYL_POS_HIGH 32407U
 #define HCR_WARN_SERVO_P_CYL_POS_LOW 32408U
 #define HCR_WARN_SERVO_P_CYL_POS_NEG 32409U
 #define HCR_WARN_SERVO_V_CYL_MAX 32410U
 #define HCR_WARN_SERVO_Y_MAX 32411U
 #define HCR_WARN_SPF_MEM 32412U
 #define HCR_WARN_AUTOTUNE_KP_INVALID 32413U
 #define HCR_WARN_NOTCH_F0_FS 32414U
 #define HCR_WARN_IT1_T1_TS 32416U
 #define HCR_WARN_IT1_T1_ZERO 32417U
 #define HCR_WARN_IT1_TN_TS 32418U
 #define HCR_WARN_SFG_FP_END 32419U
 #define HCR_WARN_SFG_T_DELTA_ACT 32420U
 #define HCR_ERR_DISABLED 65534U
#else
 #ifndef _GLOBAL_CONST
   #define _GLOBAL_CONST _WEAK const
 #endif
 _GLOBAL_CONST unsigned short HCR_ERR_DT12S_D_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_DT12S_T1_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_DT12S_T1_T2_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_DT12S_T2_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_DT12S_TF_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_DT12S_TV_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_FIFO_MEM;
 _GLOBAL_CONST unsigned short HCR_ERR_FIFO_T_FIFO_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_JFREE_TSWITCH_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_LIN_NODEVEC_EMPTY;
 _GLOBAL_CONST unsigned short HCR_ERR_LIN_X_NOT_ASC_DESC;
 _GLOBAL_CONST unsigned short HCR_ERR_LIN_Y_NOT_ASC_DESC;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_DY_MAX_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_D_MODE_INVALID;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_KW_0_1;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_K_FBK_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_TF_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_TF_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_TN_NEG_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_TN_POS_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_TV_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_WX_MAX_MIN;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_Y_MAX_MIN;
 _GLOBAL_CONST unsigned short HCR_ERR_PT12_T1_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_PT12_T2_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_PT1CP_T1_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_PT2S_D_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_PT2S_TF_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_SERVO_Y_MIN;
 _GLOBAL_CONST unsigned short HCR_ERR_SYSTEM_A_CYL;
 _GLOBAL_CONST unsigned short HCR_ERR_SYSTEM_P_DIFF;
 _GLOBAL_CONST unsigned short HCR_ERR_SYSTEM_P_SUPPLY;
 _GLOBAL_CONST unsigned short HCR_ERR_SYSTEM_P_TANK;
 _GLOBAL_CONST unsigned short HCR_ERR_VALVE_P_DELTA_NOM;
 _GLOBAL_CONST unsigned short HCR_ERR_VALVE_Q_NOM;
 _GLOBAL_CONST unsigned short HCR_ERR_VALVE_Y_0;
 _GLOBAL_CONST unsigned short HCR_ERR_RAMP_T_FALL_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_RAMP_T_HOLD_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_RAMP_T_RISE_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_SPF_A_ACC_NEG_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_SPF_A_ACC_POS_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_SPF_A_DEC_NEG_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_SPF_A_DEC_POS_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_SPF_NIL_PTR;
 _GLOBAL_CONST unsigned short HCR_ERR_SPF_T_JOLT_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_SPF_VC_NEG_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_SPF_VC_POS_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_SYSTEM_H_CYL;
 _GLOBAL_CONST unsigned short HCR_ERR_SYSTEM_M_CYL;
 _GLOBAL_CONST unsigned short HCR_ERR_SYSTEM_NIL_PTR;
 _GLOBAL_CONST unsigned short HCR_ERR_SYSTEM_P_SUPPLY_MAX;
 _GLOBAL_CONST unsigned short HCR_ERR_SYSTEM_V0_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_SYSTEM_V0_POS;
 _GLOBAL_CONST unsigned short HCR_ERR_SERVO_MODE_FY;
 _GLOBAL_CONST unsigned short HCR_ERR_SERVO_MODE_VY;
 _GLOBAL_CONST unsigned short HCR_ERR_PID_NIL_PTR;
 _GLOBAL_CONST unsigned short HCR_ERR_VALVE_NIL_PTR;
 _GLOBAL_CONST unsigned short HCR_ERR_NOTCH_B_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_NOTCH_F0_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_OMEGA0_X0_CYL;
 _GLOBAL_CONST unsigned short HCR_ERR_OMEGA0_X_CYL_MIN_MAX;
 _GLOBAL_CONST unsigned short HCR_ERR_IT1_T1_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_IT1_TN_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_SFG_T_DELTA_NEG_ZERO;
 _GLOBAL_CONST unsigned short HCR_ERR_SFG_T_DIFF;
 _GLOBAL_CONST unsigned short HCR_ERR_SFG_T_JOLT_NEG;
 _GLOBAL_CONST unsigned short HCR_ERR_DT12S_TF_NEG;
 _GLOBAL_CONST unsigned short HCR_WARN_DT12S_T1_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_DT12S_T1_ZERO;
 _GLOBAL_CONST unsigned short HCR_WARN_DT12S_T2_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_DT12S_T2_ZERO;
 _GLOBAL_CONST unsigned short HCR_WARN_DT12S_TF_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_DT12S_TV_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_DT12S_TV_ZERO;
 _GLOBAL_CONST unsigned short HCR_WARN_FIFO_T_FIFO_INT;
 _GLOBAL_CONST unsigned short HCR_WARN_FIFO_T_FIFO_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_JFREE_TSWITCH_5TS;
 _GLOBAL_CONST unsigned short HCR_WARN_JFREE_TSWITCH_ZERO;
 _GLOBAL_CONST unsigned short HCR_WARN_LIN_CRIT_RNESS;
 _GLOBAL_CONST unsigned short HCR_WARN_LIN_NB_NODE;
 _GLOBAL_CONST unsigned short HCR_WARN_LIN_X_NOT_ASC_DESC_S;
 _GLOBAL_CONST unsigned short HCR_WARN_LIN_Y_NOT_ASC_DESC;
 _GLOBAL_CONST unsigned short HCR_WARN_LIN_Y_NOT_ASC_DESC_S;
 _GLOBAL_CONST unsigned short HCR_WARN_PID_TF_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_PID_TN_NEG_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_PID_TN_POS_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_PID_TV_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_PID_YADD_100;
 _GLOBAL_CONST unsigned short HCR_WARN_PID_YI_MAX;
 _GLOBAL_CONST unsigned short HCR_WARN_PID_YI_MIN;
 _GLOBAL_CONST unsigned short HCR_WARN_PID_YMAN_MAX;
 _GLOBAL_CONST unsigned short HCR_WARN_PID_YMAN_MIN;
 _GLOBAL_CONST unsigned short HCR_WARN_PT12_T1_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_PT12_T1_ZERO;
 _GLOBAL_CONST unsigned short HCR_WARN_PT12_T2_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_PT12_T2_ZERO;
 _GLOBAL_CONST unsigned short HCR_WARN_PT1CP_T1_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_PT1CP_T1_ZERO;
 _GLOBAL_CONST unsigned short HCR_WARN_PT2S_TF_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_SERVO_FF_CYL_NEG;
 _GLOBAL_CONST unsigned short HCR_WARN_SERVO_FF_CYL_POS;
 _GLOBAL_CONST unsigned short HCR_WARN_SERVO_P_CYL_NEG_HIGH;
 _GLOBAL_CONST unsigned short HCR_WARN_SERVO_P_CYL_NEG_LOW;
 _GLOBAL_CONST unsigned short HCR_WARN_SERVO_P_CYL_NEG_NEG;
 _GLOBAL_CONST unsigned short HCR_WARN_SERVO_P_CYL_POS_HIGH;
 _GLOBAL_CONST unsigned short HCR_WARN_SERVO_P_CYL_POS_LOW;
 _GLOBAL_CONST unsigned short HCR_WARN_SERVO_P_CYL_POS_NEG;
 _GLOBAL_CONST unsigned short HCR_WARN_SERVO_V_CYL_MAX;
 _GLOBAL_CONST unsigned short HCR_WARN_SERVO_Y_MAX;
 _GLOBAL_CONST unsigned short HCR_WARN_SPF_MEM;
 _GLOBAL_CONST unsigned short HCR_WARN_AUTOTUNE_KP_INVALID;
 _GLOBAL_CONST unsigned short HCR_WARN_NOTCH_F0_FS;
 _GLOBAL_CONST unsigned short HCR_WARN_IT1_T1_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_IT1_T1_ZERO;
 _GLOBAL_CONST unsigned short HCR_WARN_IT1_TN_TS;
 _GLOBAL_CONST unsigned short HCR_WARN_SFG_FP_END;
 _GLOBAL_CONST unsigned short HCR_WARN_SFG_T_DELTA_ACT;
 _GLOBAL_CONST unsigned short HCR_ERR_DISABLED;
#endif




/* Datatypes and datatypes of function blocks */
typedef struct HCRSystemPara_typ
{	float pSupply_max;
	float pSupply;
	float pTank;
	float A_Cyl_pos;
	float A_Cyl_neg;
} HCRSystemPara_typ;

typedef struct HCRSystemParaAdd_typ
{	float V0_pos;
	float V0_neg;
	float h_Cyl;
	float m_Cyl;
} HCRSystemParaAdd_typ;

typedef struct HCRValvePara_typ
{	float Qnom_pos;
	float Qnom_neg;
	float pDelta_nom;
	float yValve_0;
	plcbit Neg_y;
} HCRValvePara_typ;

typedef struct HCRMovePara_typ
{	float t_Jolt;
	float vC_pos;
	float vC_neg;
	float aAcc_pos;
	float aDec_pos;
	float aAcc_neg;
	float aDec_neg;
} HCRMovePara_typ;

typedef struct HCRNode_Internal_typ
{	float X;
	float Y;
	float X1;
	float X2;
	float Rness_crit;
	unsigned long X1_dbl_1;
	unsigned long X1_dbl_2;
	unsigned long X2_dbl_1;
	unsigned long X2_dbl_2;
	unsigned long deltaX_dbl_1;
	unsigned long deltaX_dbl_2;
	unsigned long deltaY_dbl_1;
	unsigned long deltaY_dbl_2;
	unsigned long k_dbl_1;
	unsigned long k_dbl_2;
	unsigned long d_dbl_1;
	unsigned long d_dbl_2;
} HCRNode_Internal_typ;

typedef struct HCRNode_typ
{	float X;
	float Y;
	struct HCRNode_Internal_typ Internal;
} HCRNode_typ;

typedef struct HCRPIDPara_typ
{	float WX_min;
	float WX_max;
	float Kw;
	float Kp;
	float Tn_pos;
	float Tn_neg;
	float K_Fbk;
	unsigned char d_mode;
	float Tv;
	float Tf;
	float dY_max;
	float Y_min;
	float Y_max;
} HCRPIDPara_typ;

typedef struct HCRPIDPara_s_typ
{	float s_min;
	float s_max;
	float Kw;
	float Kp;
	float Tn_pos;
	float Tn_neg;
	float K_Fbk;
	unsigned char d_mode;
	float Tv;
	float Tf;
	float dv_max;
	float v_min;
	float v_max;
	plcbit UseAsSpeedCont;
} HCRPIDPara_s_typ;

typedef struct HCRPIDPara_F_typ
{	float Fp_min;
	float Fp_max;
	float Kw;
	float Kp;
	float Tn;
	float K_Fbk;
	unsigned char d_mode;
	float Tv;
	float Tf;
	float dQ_max;
	float Q_min;
	float Q_max;
} HCRPIDPara_F_typ;

typedef struct HCRP2P_Cont_AddPar_typ
{	float Y_P2P_min;
	float Y_P2P_max;
	float dY_max;
	float Tn_pos;
	float Tn_neg;
	unsigned char I_Switch;
	float Tv;
	float Tf;
} HCRP2P_Cont_AddPar_typ;

typedef struct HCRs_Cont_InVar_typ
{	unsigned char I_Switch;
	float v_man;
	float v_add;
	float v_set;
	float Fp_Cyl;
	float Ff_Cyl_pos;
	float Ff_Cyl_neg;
	float Qy_Oset_ext;
	unsigned char Oset_Mode;
} HCRs_Cont_InVar_typ;

typedef struct HCRs_Cont_p_InVar_typ
{	unsigned char I_Switch;
	float v_man;
	float v_add;
	float v_set;
	float pCyl_pos;
	float pCyl_neg;
	float Qy_Oset_ext;
	unsigned char Oset_Mode;
} HCRs_Cont_p_InVar_typ;

typedef struct HCRF_Cont_InVar_typ
{	unsigned char I_Switch;
	float Q_man;
	float Q_add;
	float v_act;
	float Qy_Oset_ext;
	unsigned char Oset_Mode;
} HCRF_Cont_InVar_typ;

typedef struct HCRF_Cont_p_InVar_typ
{	unsigned char I_Switch;
	float Q_man;
	float Q_add;
	float dFp_set;
	float x_Cyl;
	float v_Cyl;
	float pCyl_pos;
	float pCyl_neg;
	float v_act;
	float Qy_Oset_ext;
	unsigned char Oset_Mode;
} HCRF_Cont_p_InVar_typ;

typedef struct HCRPT1_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	float V_old;
	float T1_old;
	unsigned char Set_y_old;
	unsigned char Set_y_2;
	unsigned long x_dbl_1;
	unsigned long x_dbl_2;
	unsigned long Exp_Ts_T1_dbl_1;
	unsigned long Exp_Ts_T1_dbl_2;
	unsigned long V_Exp_dbl_1;
	unsigned long V_Exp_dbl_2;
	unsigned long Bootkey;
} HCRPT1_Internal_typ;

typedef struct HCRPT2_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	float V_old;
	float T1_old;
	float T2_old;
	unsigned char Set_y_old;
	unsigned char Set_y_2;
	unsigned long x1_dbl_1;
	unsigned long x1_dbl_2;
	unsigned long x2_dbl_1;
	unsigned long x2_dbl_2;
	unsigned long Exp_Ts_T1_dbl_1;
	unsigned long Exp_Ts_T1_dbl_2;
	unsigned long Exp_Ts_T2_dbl_1;
	unsigned long Exp_Ts_T2_dbl_2;
	unsigned long Exp_Ts_T1_T2_dbl_1;
	unsigned long Exp_Ts_T1_T2_dbl_2;
	unsigned long Exp_T1_T2_dbl_1;
	unsigned long Exp_T1_T2_dbl_2;
	unsigned long V1_Exp_dbl_1;
	unsigned long V1_Exp_dbl_2;
	unsigned long V2_Exp_dbl_1;
	unsigned long V2_Exp_dbl_2;
	unsigned long Bootkey;
} HCRPT2_Internal_typ;

typedef struct HCRPT2s_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	float V_old;
	float D_old;
	float Tf_old;
	unsigned char Set_y_old;
	unsigned char Set_y_2;
	unsigned long x1_dbl_1;
	unsigned long x1_dbl_2;
	unsigned long x2_dbl_1;
	unsigned long x2_dbl_2;
	unsigned long Sqrt_D_1_dbl_1;
	unsigned long Sqrt_D_1_dbl_2;
	unsigned long Ts_Tf_dbl_1;
	unsigned long Ts_Tf_dbl_2;
	unsigned long Exp_D_Ts_Tf_dbl_1;
	unsigned long Exp_D_Ts_Tf_dbl_2;
	unsigned long a0_dbl_1;
	unsigned long a0_dbl_2;
	unsigned long a1_dbl_1;
	unsigned long a1_dbl_2;
	unsigned long b0_dbl_1;
	unsigned long b0_dbl_2;
	unsigned long b1_dbl_1;
	unsigned long b1_dbl_2;
	unsigned long Bootkey;
} HCRPT2s_Internal_typ;

typedef struct HCRDT1_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	float Tv_old;
	float Tf_old;
	unsigned char n_Avg_old;
	float u_Buf[100];
	signed char u_BufCnt;
	unsigned long u_Sum_dbl_1;
	unsigned long u_Sum_dbl_2;
	unsigned long u_Avg_dbl_1;
	unsigned long u_Avg_dbl_2;
	unsigned char Reset_y_old;
	unsigned char Reset_y_2;
	unsigned long x_dbl_1;
	unsigned long x_dbl_2;
	unsigned long Tv_Tf_dbl_1;
	unsigned long Tv_Tf_dbl_2;
	unsigned long Exp_Ts_Tf_dbl_1;
	unsigned long Exp_Ts_Tf_dbl_2;
	unsigned long Tv_Tf_Exp_dbl_1;
	unsigned long Tv_Tf_Exp_dbl_2;
	unsigned long Bootkey;
} HCRDT1_Internal_typ;

typedef struct HCRDT2_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	float Tv_old;
	float T1_old;
	float T2_old;
	unsigned char n_Avg_old;
	float u_Buf[100];
	signed char u_BufCnt;
	unsigned long u_Sum_dbl_1;
	unsigned long u_Sum_dbl_2;
	unsigned long u_Avg_dbl_1;
	unsigned long u_Avg_dbl_2;
	unsigned char Reset_y_old;
	unsigned char Reset_y_2;
	unsigned long x1_dbl_1;
	unsigned long x1_dbl_2;
	unsigned long x2_dbl_1;
	unsigned long x2_dbl_2;
	unsigned long Tv_T1_dbl_1;
	unsigned long Tv_T1_dbl_2;
	unsigned long Tv_T2_dbl_1;
	unsigned long Tv_T2_dbl_2;
	unsigned long Exp_Ts_T1_dbl_1;
	unsigned long Exp_Ts_T1_dbl_2;
	unsigned long Exp_Ts_T2_dbl_1;
	unsigned long Exp_Ts_T2_dbl_2;
	unsigned long Exp_T1_T2_dbl_1;
	unsigned long Exp_T1_T2_dbl_2;
	unsigned long a0_dbl_1;
	unsigned long a0_dbl_2;
	unsigned long a1_dbl_1;
	unsigned long a1_dbl_2;
	unsigned long b0_dbl_1;
	unsigned long b0_dbl_2;
	unsigned long b1_dbl_1;
	unsigned long b1_dbl_2;
	unsigned long Bootkey;
} HCRDT2_Internal_typ;

typedef struct HCRDT2s_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	float D_old;
	float Tv_old;
	float Tf_old;
	unsigned char Reset_y_old;
	unsigned char Reset_y_2;
	unsigned long x1_dbl_1;
	unsigned long x1_dbl_2;
	unsigned long x2_dbl_1;
	unsigned long x2_dbl_2;
	unsigned long Sqrt_D_1_dbl_1;
	unsigned long Sqrt_D_1_dbl_2;
	unsigned long Tv_Tf_dbl_1;
	unsigned long Tv_Tf_dbl_2;
	unsigned long Ts_Tf_dbl_1;
	unsigned long Ts_Tf_dbl_2;
	unsigned long Exp_D_Ts_Tf_dbl_1;
	unsigned long Exp_D_Ts_Tf_dbl_2;
	unsigned long a0_dbl_1;
	unsigned long a0_dbl_2;
	unsigned long a1_dbl_1;
	unsigned long a1_dbl_2;
	unsigned long b0_dbl_1;
	unsigned long b0_dbl_2;
	unsigned long b1_dbl_1;
	unsigned long b1_dbl_2;
	unsigned long Bootkey;
} HCRDT2s_Internal_typ;

typedef struct HCRIT1_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	float V_old;
	float Tn_old;
	float T1_old;
	unsigned char Set_y_old;
	unsigned char Set_y_2;
	unsigned long x1_dbl_1;
	unsigned long x1_dbl_2;
	unsigned long x2_dbl_1;
	unsigned long x2_dbl_2;
	unsigned long Exp_Ts_T1_dbl_1;
	unsigned long Exp_Ts_T1_dbl_2;
	unsigned long V_Tn_dbl_1;
	unsigned long V_Tn_dbl_2;
	unsigned long a0_dbl_1;
	unsigned long a0_dbl_2;
	unsigned long a1_dbl_1;
	unsigned long a1_dbl_2;
	unsigned long b0_dbl_1;
	unsigned long b0_dbl_2;
	unsigned long b1_dbl_1;
	unsigned long b1_dbl_2;
	unsigned long Bootkey;
} HCRIT1_Internal_typ;

typedef struct HCRNotch_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	unsigned long fs_dbl_1;
	unsigned long fs_dbl_2;
	float V_old;
	float f0_old;
	float B_old;
	unsigned char Set_y_old;
	unsigned char Set_y_2;
	float fg_upper;
	float fg_lower;
	float B_act;
	float D;
	float Tf;
	unsigned long x1_dbl_1;
	unsigned long x1_dbl_2;
	unsigned long x2_dbl_1;
	unsigned long x2_dbl_2;
	unsigned long D_Ts_Tf_dbl_1;
	unsigned long D_Ts_Tf_dbl_2;
	unsigned long Sqrt_D_1_dbl_1;
	unsigned long Sqrt_D_1_dbl_2;
	unsigned long a0_dbl_1;
	unsigned long a0_dbl_2;
	unsigned long a1_dbl_1;
	unsigned long a1_dbl_2;
	unsigned long b0_dbl_1;
	unsigned long b0_dbl_2;
	unsigned long b1_dbl_1;
	unsigned long b1_dbl_2;
	unsigned long Bootkey;
} HCRNotch_Internal_typ;

typedef struct HCRPT1cP_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	float T1_intern;
	unsigned long nCnt;
	unsigned char Set_y_old;
	unsigned char Set_y_2;
	unsigned long x_dbl_1;
	unsigned long x_dbl_2;
	unsigned long Exp_5_dbl_1;
	unsigned long Exp_5_dbl_2;
	unsigned long a0_dbl_1;
	unsigned long a0_dbl_2;
	unsigned long b0_dbl_1;
	unsigned long b0_dbl_2;
	unsigned long Bootkey;
} HCRPT1cP_Internal_typ;

typedef struct HCRJoltFree_Internal_typ
{	plcstring statusTxt[101];
	plcbit uSwitch_old;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRJoltFree_Internal_typ;

typedef struct HCRFIFO_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	signed long t_FIFO_max;
	unsigned long MemSize;
	unsigned long MemSize_REAL;
	plcbit Set_y_2;
	unsigned long pFIFO_Buf;
	unsigned long pFIFO_OutIn;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRFIFO_Internal_typ;

typedef struct HCRRampGen_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned char RampGenPhase;
	float t_rise;
	float t_hold;
	float t_fall;
	float y_start;
	float y_hold;
	float y_end;
	float y;
	float dy;
	float dy_ramped;
	plcbit Finished;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	unsigned long Timer_dbl_1;
	unsigned long Timer_dbl_2;
	unsigned long k_Ramp_dbl_1;
	unsigned long k_Ramp_dbl_2;
	unsigned long y_dbl_1;
	unsigned long y_dbl_2;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRRampGen_Internal_typ;

typedef struct HCRSPF_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	unsigned char ProfileNb;
	unsigned char ProfileID;
	signed char MovementCase;
	unsigned char MovementPhase;
	unsigned char MovementStep;
	float s_end;
	unsigned long q_set_0_dbl_1;
	unsigned long q_set_0_dbl_2;
	unsigned long s_set_0_dbl_1;
	unsigned long s_set_0_dbl_2;
	unsigned long v_set_0_dbl_1;
	unsigned long v_set_0_dbl_2;
	unsigned long a_set_0_dbl_1;
	unsigned long a_set_0_dbl_2;
	float v0;
	unsigned long v0_dbl_1;
	unsigned long v0_dbl_2;
	unsigned char v0_intrude;
	unsigned char v0_state;
	float q_set;
	float q_set_Jolt;
	unsigned long q_set_dbl_1;
	unsigned long q_set_dbl_2;
	float s_set;
	float s_set_Jolt;
	unsigned long s_set_dbl_1;
	unsigned long s_set_dbl_2;
	unsigned long s_set_add_dbl_1;
	unsigned long s_set_add_dbl_2;
	float v_set;
	float v_set_Jolt;
	unsigned long v_set_dbl_1;
	unsigned long v_set_dbl_2;
	float a_set;
	float a_set_Jolt;
	unsigned long a_set_dbl_1;
	unsigned long a_set_dbl_2;
	unsigned long vC_pos_dbl_1;
	unsigned long vC_pos_dbl_2;
	unsigned long vC_neg_dbl_1;
	unsigned long vC_neg_dbl_2;
	unsigned long aAcc_pos_dbl_1;
	unsigned long aAcc_pos_dbl_2;
	unsigned long aDec_pos_dbl_1;
	unsigned long aDec_pos_dbl_2;
	unsigned long aAcc_neg_dbl_1;
	unsigned long aAcc_neg_dbl_2;
	unsigned long aDec_neg_dbl_1;
	unsigned long aDec_neg_dbl_2;
	float aDec;
	plcbit aDec_intrude;
	float t_Jolt_max;
	float t_Jolt_limit;
	float t_Jolt;
	unsigned long t_Jolt_dbl_1;
	unsigned long t_Jolt_dbl_2;
	float sDelta;
	unsigned long sDelta_dbl_1;
	unsigned long sDelta_dbl_2;
	float sBrake;
	unsigned long sBrake_dbl_1;
	unsigned long sBrake_dbl_2;
	float vTop;
	unsigned long vTop_dbl_1;
	unsigned long vTop_dbl_2;
	float MoveParaVec[6];
	unsigned long AccVec_dbl[40];
	unsigned long ConstVec_dbl[40];
	unsigned long DecVec_dbl[40];
	float AccVec[20];
	float ConstVec[20];
	float DecVec[20];
	unsigned long pBuf_q_set;
	unsigned long pBuf_s_set;
	unsigned long pBuf_v_set;
	unsigned long pBuf_a_set;
	signed long BufCntMax;
	signed long BufCnt;
	signed long BufCnt_t_Jolt;
	signed long Cnt_t_Jolt;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	unsigned long Ts_2_dbl_1;
	unsigned long Ts_2_dbl_2;
	unsigned long Ts_4_dbl_1;
	unsigned long Ts_4_dbl_2;
	float Timer;
	unsigned long Timer_dbl_1;
	unsigned long Timer_dbl_2;
	float Timer_Ts;
	unsigned long Timer_Ts_dbl_1;
	unsigned long Timer_Ts_dbl_2;
	plcbit Init;
	plcbit Brake;
	plcbit enable_old;
	plcbit constTCycle;
	unsigned short counterState;
	unsigned long musecs_old;
	unsigned long SysTicks_old;
	unsigned long Bootkey_T;
	unsigned long Bootkey;
} HCRSPF_Internal_typ;

typedef struct HCRSFG_Internal_typ
{	plcstring statusTxt[101];
	struct HCRMovePara_typ SFG_Para;
	unsigned char Profile_Case;
	float Fp_delta;
	float Fp_brake;
	float Fp_set_0;
	float dFp_set_0;
	float dFp_home;
	unsigned char dFp_0_intrude;
	float dFp_const;
	float dFp_const_avg;
	float dFp_thresh[3];
	float t_Jolt;
	float t_delta;
	float t_delta_Jolt;
	float t_delta_3_Jolt;
	unsigned char Calc_t_delta;
	float tx;
	float tx_limit;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRSFG_Internal_typ;

typedef struct HCRAutoTune_Internal_typ
{	plcstring statusTxt[101];
	float E;
	float V0_pos;
	unsigned long V0_pos_mod_dbl_1;
	unsigned long V0_pos_mod_dbl_2;
	float V0_neg;
	unsigned long V0_neg_mod_dbl_1;
	unsigned long V0_neg_mod_dbl_2;
	unsigned long s_Cyl_min_dbl_1;
	unsigned long s_Cyl_min_dbl_2;
	unsigned long s_Cyl_max_dbl_1;
	unsigned long s_Cyl_max_dbl_2;
	float h_Cyl;
	unsigned long h_Cyl_dbl_1;
	unsigned long h_Cyl_dbl_2;
	unsigned long E_A_pos_2_dbl_1;
	unsigned long E_A_pos_2_dbl_2;
	unsigned long E_A_neg_2_dbl_1;
	unsigned long E_A_neg_2_dbl_2;
	float s0_Cyl;
	unsigned long s0_Cyl_dbl_1;
	unsigned long s0_Cyl_dbl_2;
	float c0_Cyl;
	unsigned long c0_Cyl_dbl_1;
	unsigned long c0_Cyl_dbl_2;
	float omega0_Cyl;
	unsigned long omega0_Cyl_dbl_1;
	unsigned long omega0_Cyl_dbl_2;
	float vCyl_sup;
	unsigned long Kp_dbl_1;
	unsigned long Kp_dbl_2;
	float Kp_physical;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRAutoTune_Internal_typ;

typedef struct HCROmega0_Internal_typ
{	plcstring statusTxt[101];
	float E;
	float Kp_xCorr_Lim;
	float A_Cyl_pos;
	float A_Cyl_neg;
	float V0_pos;
	float V0_neg;
	float h_Cyl;
	unsigned long x_Cyl_min_dbl_1;
	unsigned long x_Cyl_min_dbl_2;
	unsigned long x_Cyl_max_dbl_1;
	unsigned long x_Cyl_max_dbl_2;
	unsigned long E_A_pos_2_dbl_1;
	unsigned long E_A_pos_2_dbl_2;
	unsigned long E_A_neg_2_dbl_1;
	unsigned long E_A_neg_2_dbl_2;
	unsigned long x0_Cyl_dbl_1;
	unsigned long x0_Cyl_dbl_2;
	float c0_Cyl;
	unsigned long c0_Cyl_dbl_1;
	unsigned long c0_Cyl_dbl_2;
	float c_Cyl_Lim_L;
	unsigned long c_Cyl_Lim_L_dbl_1;
	unsigned long c_Cyl_Lim_L_dbl_2;
	float c_Cyl_Lim_R;
	unsigned long c_Cyl_Lim_R_dbl_1;
	unsigned long c_Cyl_Lim_R_dbl_2;
	float c_Cyl;
	unsigned long c_Cyl_dbl_1;
	unsigned long c_Cyl_dbl_2;
	float Kp_xCorr_Lim_L;
	float Kp_xCorr_Lim_R;
	plcbit enable_old;
	unsigned long Bootkey;
} HCROmega0_Internal_typ;

typedef struct HCRvyServo_Internal_typ
{	plcstring statusTxt[101];
	float Alpha;
	float vCyl_sup;
	float vCyl_max;
	float vCyl_sum;
	float vCyl_thresh;
	float vCyl_nOverlap_pos;
	float vCyl_nOverlap_neg;
	float vCyl_old;
	float pSupply_max;
	float pSupply;
	float pTank;
	float Qnom_pos;
	float Qnom_neg;
	float A_Cyl_pos;
	float A_Cyl_neg;
	float yValve_zero;
	float yValve_min;
	plcbit Neg_y;
	float yValve_0;
	unsigned long yValve_0_dbl_1;
	unsigned long yValve_0_dbl_2;
	unsigned long K_pos_dbl_1;
	unsigned long K_pos_dbl_2;
	unsigned long K_neg_dbl_1;
	unsigned long K_neg_dbl_2;
	float kV;
	unsigned long kV_dbl_1;
	unsigned long kV_dbl_2;
	float k4_v_set;
	float K_yValve;
	unsigned long K_yValve_dbl_1;
	unsigned long K_yValve_dbl_2;
	unsigned long K_nOverlap_pos_dbl_1;
	unsigned long K_nOverlap_pos_dbl_2;
	unsigned long K_nOverlap_neg_dbl_1;
	unsigned long K_nOverlap_neg_dbl_2;
	unsigned long S_nOverlap_dbl_1;
	unsigned long S_nOverlap_dbl_2;
	unsigned long pDelta_min_dbl_1;
	unsigned long pDelta_min_dbl_2;
	unsigned long vCyl_sup_100_dbl_1;
	unsigned long vCyl_sup_100_dbl_2;
	unsigned char vMode;
	float vCyl_lim;
	unsigned char Update_old;
	unsigned char Update_2;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRvyServo_Internal_typ;

typedef struct HCRvyServo_Fp_Internal_typ
{	plcstring statusTxt[101];
	float Alpha;
	float vCyl_sup;
	float vCyl_max;
	float vCyl_sum;
	float vCyl_thresh;
	float vCyl_nOverlap_pos;
	float vCyl_nOverlap_neg;
	float vCyl_old;
	float pSupply_max;
	float pSupply;
	float pTank;
	float Qnom_pos;
	float Qnom_neg;
	float A_Cyl_pos;
	float A_Cyl_neg;
	float pCyl_pos;
	float pCyl_neg;
	float yValve_zero;
	float yValve_min;
	plcbit Neg_y;
	float yValve_0;
	unsigned long yValve_0_dbl_1;
	unsigned long yValve_0_dbl_2;
	unsigned long Supply_Tank_pos_dbl_1;
	unsigned long Supply_Tank_pos_dbl_2;
	unsigned long Supply_Tank_neg_dbl_1;
	unsigned long Supply_Tank_neg_dbl_2;
	unsigned long Qn_pos_2_A_neg_2_dbl_1;
	unsigned long Qn_pos_2_A_neg_2_dbl_2;
	unsigned long Qn_neg_2_A_pos_2_dbl_1;
	unsigned long Qn_neg_2_A_pos_2_dbl_2;
	unsigned long Qn_pos_2_A_neg_3_dbl_1;
	unsigned long Qn_pos_2_A_neg_3_dbl_2;
	unsigned long Qn_neg_2_A_pos_3_dbl_1;
	unsigned long Qn_neg_2_A_pos_3_dbl_2;
	unsigned long K_dbl_1;
	unsigned long K_dbl_2;
	unsigned long K_pos_dbl_1;
	unsigned long K_pos_dbl_2;
	unsigned long K_neg_dbl_1;
	unsigned long K_neg_dbl_2;
	float kV;
	unsigned long kV_dbl_1;
	unsigned long kV_dbl_2;
	float k4_v_set;
	float K_yValve;
	unsigned long K_yValve_dbl_1;
	unsigned long K_yValve_dbl_2;
	unsigned long K_nOverlap_dbl_1;
	unsigned long K_nOverlap_dbl_2;
	unsigned long K_nOverlap_pos_dbl_1;
	unsigned long K_nOverlap_pos_dbl_2;
	unsigned long K_nOverlap_neg_dbl_1;
	unsigned long K_nOverlap_neg_dbl_2;
	unsigned long S_nOverlap_dbl_1;
	unsigned long S_nOverlap_dbl_2;
	unsigned long pCyl_pos_Numer_pos_dbl_1;
	unsigned long pCyl_pos_Numer_pos_dbl_2;
	unsigned long pCyl_neg_Numer_pos_dbl_1;
	unsigned long pCyl_neg_Numer_pos_dbl_2;
	unsigned long pCyl_pos_Numer_neg_dbl_1;
	unsigned long pCyl_pos_Numer_neg_dbl_2;
	unsigned long pCyl_neg_Numer_neg_dbl_1;
	unsigned long pCyl_neg_Numer_neg_dbl_2;
	unsigned long pCyl_Denom_dbl_1;
	unsigned long pCyl_Denom_dbl_2;
	unsigned long F_Delta_min_dbl_1;
	unsigned long F_Delta_min_dbl_2;
	unsigned long vCyl_sup_100_dbl_1;
	unsigned long vCyl_sup_100_dbl_2;
	unsigned char vMode;
	float vCyl_lim;
	unsigned char Update_old;
	unsigned char Update_2;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRvyServo_Fp_Internal_typ;

typedef struct HCRFyServo_Internal_typ
{	plcstring statusTxt[101];
	float E;
	float Alpha;
	float pSupply_max;
	float pSupply;
	float pTank;
	float Qnom_pos;
	float Qnom_neg;
	float A_Cyl_pos;
	float A_Cyl_neg;
	float V0_pos;
	float V0_neg;
	plcbit Neg_y;
	float yValve_0;
	float dF_Cyl_sup;
	float dF_Cyl_sum;
	float x_Cyl;
	float h_Cyl;
	unsigned long h_Cyl_dbl_1;
	unsigned long h_Cyl_dbl_2;
	unsigned long K_pos_dbl_1;
	unsigned long K_pos_dbl_2;
	unsigned long K_neg_dbl_1;
	unsigned long K_neg_dbl_2;
	float K_yValve;
	unsigned long K_yValve_dbl_1;
	unsigned long K_yValve_dbl_2;
	unsigned long V0_A_Cyl_pos_dbl_1;
	unsigned long V0_A_Cyl_pos_dbl_2;
	unsigned long V0_A_Cyl_neg_dbl_1;
	unsigned long V0_A_Cyl_neg_dbl_2;
	unsigned long A_Cyl_pos_1000_dbl_1;
	unsigned long A_Cyl_pos_1000_dbl_2;
	unsigned long A_Cyl_neg_1000_dbl_1;
	unsigned long A_Cyl_neg_1000_dbl_2;
	unsigned long E_10_dbl_1;
	unsigned long E_10_dbl_2;
	unsigned long pDelta_min_dbl_1;
	unsigned long pDelta_min_dbl_2;
	unsigned long dF_Cyl_sup_100_dbl_1;
	unsigned long dF_Cyl_sup_100_dbl_2;
	float K_v_Cyl;
	unsigned char Update_old;
	unsigned char Update_2;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRFyServo_Internal_typ;

typedef struct HCRFyServo_Fp_Internal_typ
{	plcstring statusTxt[101];
	float Alpha;
	float pSupply_max;
	float pSupply;
	float pTank;
	float Qnom_pos;
	float Qnom_neg;
	float A_Cyl_pos;
	float A_Cyl_neg;
	plcbit Neg_y;
	float yValve_0;
	unsigned long Supply_Tank_pos_dbl_1;
	unsigned long Supply_Tank_pos_dbl_2;
	unsigned long Supply_Tank_neg_dbl_1;
	unsigned long Supply_Tank_neg_dbl_2;
	unsigned long K_numer_dbl_1;
	unsigned long K_numer_dbl_2;
	float K_yValve;
	unsigned long K_yValve_dbl_1;
	unsigned long K_yValve_dbl_2;
	unsigned long F_Delta_min_dbl_1;
	unsigned long F_Delta_min_dbl_2;
	unsigned char Update_old;
	unsigned char Update_2;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRFyServo_Fp_Internal_typ;

typedef struct HCRLinSpline_Internal_typ
{	plcstring statusTxt[101];
	signed char AscendingX_o;
	signed char AscendingXs_o;
	signed char AscendingY_o;
	signed char AscendingYs_o;
	signed char AscendingX_d;
	signed char AscendingXs_d;
	signed char AscendingY_d;
	signed char AscendingYs_d;
	float Rness_crit;
	float RnessP;
	float RnessR;
	unsigned long _Smooth1_dbl_1;
	unsigned long _Smooth1_dbl_2;
	unsigned long _1Smooth_dbl_1;
	unsigned long _1Smooth_dbl_2;
	unsigned long _1Smooth1_dbl_1;
	unsigned long _1Smooth1_dbl_2;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRLinSpline_Internal_typ;

typedef struct HCRLinValve_Internal_typ
{	plcstring statusTxt[101];
	plcbit Neg_Qy;
	plcbit Neg_Uy;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRLinValve_Internal_typ;

typedef struct HCRPID_Internal_typ
{	plcstring statusTxt[101];
	signed long Ts;
	float WX_min;
	float WX_max;
	float WX_min_max;
	unsigned long _100_WX_delta_dbl_1;
	unsigned long _100_WX_delta_dbl_2;
	unsigned long W_norm_dbl_1;
	unsigned long W_norm_dbl_2;
	unsigned long X_norm_dbl_1;
	unsigned long X_norm_dbl_2;
	plcbit Invert;
	unsigned long e_norm_dbl_1;
	unsigned long e_norm_dbl_2;
	float e_DTi;
	float Kp;
	float Kw;
	unsigned long Kp_Kw_1_dbl_1;
	unsigned long Kp_Kw_1_dbl_2;
	float Kp_intrude;
	plcbit P_intrude;
	unsigned long Yp_dbl_1;
	unsigned long Yp_dbl_2;
	unsigned long Ts_dbl_1;
	unsigned long Ts_dbl_2;
	unsigned long Tn_pos_dbl_1;
	unsigned long Tn_pos_dbl_2;
	unsigned long Tn_neg_dbl_1;
	unsigned long Tn_neg_dbl_2;
	unsigned long Kp_Ts_Tn_pos_dbl_1;
	unsigned long Kp_Ts_Tn_pos_dbl_2;
	unsigned long Kp_Ts_Tn_neg_dbl_1;
	unsigned long Kp_Ts_Tn_neg_dbl_2;
	float Yi_load;
	signed char I_load;
	float Y_bias;
	unsigned char PID_bias;
	float Ypi_limit;
	unsigned char PI_limit;
	unsigned long Yi_dbl_1;
	unsigned long Yi_dbl_2;
	unsigned long Yi_pos_last_dbl_1;
	unsigned long Yi_pos_last_dbl_2;
	unsigned long Yi_neg_last_dbl_1;
	unsigned long Yi_neg_last_dbl_2;
	signed char I_Switch_Sign;
	signed char I_Switch;
	signed char I_Switch_old;
	signed char I_Switch_off;
	unsigned char d_mode;
	float D;
	unsigned long Tv_dbl_1;
	unsigned long Tv_dbl_2;
	unsigned long Tf_dbl_1;
	unsigned long Tf_dbl_2;
	unsigned long x1d_dbl_1;
	unsigned long x1d_dbl_2;
	unsigned long x2d_dbl_1;
	unsigned long x2d_dbl_2;
	unsigned long a0d_dbl_1;
	unsigned long a0d_dbl_2;
	unsigned long a1d_dbl_1;
	unsigned long a1d_dbl_2;
	unsigned long b0d_dbl_1;
	unsigned long b0d_dbl_2;
	unsigned long b1d_dbl_1;
	unsigned long b1d_dbl_2;
	unsigned long Yd_dbl_1;
	unsigned long Yd_dbl_2;
	unsigned char Reset_Yd;
	float Yadd;
	float Y_min;
	float Y_max;
	unsigned long Ytotal_dbl_1;
	unsigned long Ytotal_dbl_2;
	unsigned long Ylim_dbl_1;
	unsigned long Ylim_dbl_2;
	float Y_Fbk_ext;
	unsigned char Fbk_mode;
	unsigned long K_Fbk_Kp_dbl_1;
	unsigned long K_Fbk_Kp_dbl_2;
	unsigned long e_Fbk_dbl_1;
	unsigned long e_Fbk_dbl_2;
	float dY_max;
	unsigned long dY_max_Ts_dbl_1;
	unsigned long dY_max_Ts_dbl_2;
	unsigned long Y_dbl_1;
	unsigned long Y_dbl_2;
	plcbit UpdateValid;
	plcbit enable_old;
	unsigned long Bootkey_CM;
	unsigned long Bootkey;
} HCRPID_Internal_typ;

typedef struct HCRP2P_Cont_Internal_typ
{	plcstring statusTxt[101];
	signed char MoveDir;
	float s_set_old;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRP2P_Cont_Internal_typ;

typedef struct HCRs_Cont_Internal_typ
{	plcstring statusTxt[101];
	float vCyl_sup;
	float s_set_old;
	plcbit UseAsSpeedCont;
	float kV;
	unsigned long kV_dbl_1;
	unsigned long kV_dbl_2;
	float WX_min_max;
	float k_v_set[2];
	unsigned char PID_s_bias;
	float Qy_s_bias;
	float yValve_bias;
	float K_yValve;
	float vCyl_bias;
	float vCyl_sum;
	float vCyl_abs;
	float vCyl_rel;
	float Y_bias;
	unsigned char Oset_Mode;
	float Qy_Oset_s_pos;
	float Qy_Oset_s_neg;
	unsigned char Qy_I_Switch;
	float Qy_I;
	signed char MoveDir_I;
	unsigned char vMode;
	float v_lim;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRs_Cont_Internal_typ;

typedef struct HCRs_Cont_p_Internal_typ
{	plcstring statusTxt[101];
	float vCyl_sup;
	float s_set_old;
	plcbit UseAsSpeedCont;
	float kV;
	unsigned long kV_dbl_1;
	unsigned long kV_dbl_2;
	float WX_min_max;
	float k_v_set[2];
	unsigned char PID_s_bias;
	float Qy_s_bias;
	float yValve_bias;
	float K_yValve;
	float vCyl_bias;
	float vCyl_sum;
	float vCyl_abs;
	float vCyl_rel;
	float Y_bias;
	unsigned char Oset_Mode;
	float Qy_Oset_s_pos;
	float Qy_Oset_s_neg;
	unsigned char Qy_I_Switch;
	float Qy_I;
	signed char MoveDir_I;
	unsigned char vMode;
	float v_lim;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRs_Cont_p_Internal_typ;

typedef struct HCRF_Cont_Internal_typ
{	plcstring statusTxt[101];
	float Qnom_pos;
	float Qnom_neg;
	unsigned char Oset_Mode;
	float Qy_Oset_F_pos;
	float Qy_Oset_F_neg;
	unsigned char PID_F_bias;
	float Qy_F_bias;
	float yValve_bias;
	float K_yValve;
	float u_bias;
	float u_sum;
	float u_abs;
	float u_rel;
	float Y_bias;
	signed char MoveDir;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRF_Cont_Internal_typ;

typedef struct HCRF_Cont_p_Internal_typ
{	plcstring statusTxt[101];
	float Qnom_pos;
	float Qnom_neg;
	unsigned char Oset_Mode;
	float Qy_Oset_F_pos;
	float Qy_Oset_F_neg;
	unsigned char PID_F_bias;
	float Qy_F_bias;
	float yValve_bias;
	float K_yValve;
	float dF_Cyl_bias;
	float dF_Cyl_v;
	float dF_Cyl_sum;
	float dF_Cyl_abs;
	float dF_Cyl_rel;
	float Y_bias;
	signed char MoveDir;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRF_Cont_p_Internal_typ;

typedef struct HCRsF_Cont_Internal_typ
{	plcstring statusTxt[101];
	unsigned char PID_s_bias;
	float Qy_s_bias;
	unsigned char PID_F_bias;
	float Qy_F_bias;
	float tSwitch;
	unsigned char uSwitch_old;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRsF_Cont_Internal_typ;

typedef struct HCRsF_Cont_p_Internal_typ
{	plcstring statusTxt[101];
	unsigned char CTRLmode;
	unsigned char F_Cont_d_mode;
	float F_Cont_Qy;
	unsigned char PID_s_bias;
	float Qy_s_bias;
	unsigned char PID_F_bias;
	float Qy_F_bias;
	float tSwitch;
	unsigned char uSwitch_old;
	plcbit enable_old;
	unsigned long Bootkey;
} HCRsF_Cont_p_Internal_typ;

typedef struct HCRPT1
{
	/* VAR_INPUT (analog) */
	float V;
	float T1;
	float u;
	unsigned char Set_y;
	float y_set;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRPT1_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRPT1_typ;

typedef struct HCRPT2
{
	/* VAR_INPUT (analog) */
	float V;
	float T1;
	float T2;
	float u;
	unsigned char Set_y;
	float y_set;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRPT2_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRPT2_typ;

typedef struct HCRPT2s
{
	/* VAR_INPUT (analog) */
	float V;
	float D;
	float Tf;
	float u;
	unsigned char Set_y;
	float y_set;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRPT2s_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRPT2s_typ;

typedef struct HCRDT1
{
	/* VAR_INPUT (analog) */
	float Tv;
	float Tf;
	float u;
	unsigned char n_Avg;
	unsigned char Reset_y;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRDT1_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRDT1_typ;

typedef struct HCRDT2
{
	/* VAR_INPUT (analog) */
	float Tv;
	float T1;
	float T2;
	float u;
	unsigned char n_Avg;
	unsigned char Reset_y;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRDT2_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRDT2_typ;

typedef struct HCRDT2s
{
	/* VAR_INPUT (analog) */
	float D;
	float Tv;
	float Tf;
	float u;
	unsigned char Reset_y;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRDT2s_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRDT2s_typ;

typedef struct HCRIT1
{
	/* VAR_INPUT (analog) */
	float V;
	float Tn;
	float T1;
	float u;
	unsigned char Set_y;
	float y_set;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRIT1_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRIT1_typ;

typedef struct HCRNotch
{
	/* VAR_INPUT (analog) */
	float V;
	float f0;
	float B;
	float u;
	unsigned char Set_y;
	float y_set;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRNotch_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRNotch_typ;

typedef struct HCRPT1cP
{
	/* VAR_INPUT (analog) */
	float T1;
	float u;
	unsigned char Set_y;
	float y_set;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRPT1cP_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRPT1cP_typ;

typedef struct HCRJoltFree
{
	/* VAR_INPUT (analog) */
	float tSwitch;
	float u1;
	float u2;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRJoltFree_Internal_typ Internal;
	struct HCRPT1cP PT1cP;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit uSwitch;
} HCRJoltFree_typ;

typedef struct HCRFIFO
{
	/* VAR_INPUT (analog) */
	signed long t_FIFO;
	float x;
	float y_set;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRFIFO_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Set_y;
} HCRFIFO_typ;

typedef struct HCRRampGen
{
	/* VAR_INPUT (analog) */
	float t_rise;
	float t_hold;
	float t_fall;
	float y_start;
	float y_hold;
	float y_end;
	/* VAR_OUTPUT (analog) */
	float y;
	float dy;
	float dy_ramped;
	unsigned short status;
	/* VAR (analog) */
	struct HCRRampGen_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Init;
	/* VAR_OUTPUT (digital) */
	plcbit Finished;
} HCRRampGen_typ;

typedef struct HCRSPF
{
	/* VAR_INPUT (analog) */
	struct HCRMovePara_typ* pMove_Para;
	float s_home;
	float s_end;
	/* VAR_OUTPUT (analog) */
	float s_set;
	float v_set;
	float a_set;
	signed char MoveDir;
	signed char ApproachDir;
	unsigned short status;
	/* VAR (analog) */
	struct HCRSPF_Internal_typ Internal;
	struct SysInfo SystemInfo;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Stop;
	plcbit Set_home;
	plcbit Init;
	plcbit Brake;
	/* VAR_OUTPUT (digital) */
	plcbit Reached;
} HCRSPF_typ;

typedef struct HCRSFG
{
	/* VAR_INPUT (analog) */
	float t_Jolt;
	float dFp_home;
	float Fp_home;
	float t_delta;
	float Fp_end;
	/* VAR_OUTPUT (analog) */
	float Fp_set;
	float dFp_set;
	float t_delta_act;
	unsigned short status;
	/* VAR (analog) */
	struct HCRSFG_Internal_typ Internal;
	struct HCRSPF SPF;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Stop;
	plcbit Set_home;
	plcbit Init;
	/* VAR_OUTPUT (digital) */
	plcbit Reached;
} HCRSFG_typ;

typedef struct HCRAutoTune
{
	/* VAR_INPUT (analog) */
	float pSupply_max;
	float A_Cyl_pos;
	float A_Cyl_neg;
	float Qnom_pos;
	float Qnom_neg;
	float pDelta_nom;
	float V0_pos;
	float V0_neg;
	float h_Cyl;
	float m_Cyl;
	float s_min;
	float s_max;
	unsigned char Update;
	/* VAR_OUTPUT (analog) */
	float Kp;
	unsigned short status;
	/* VAR (analog) */
	struct HCRAutoTune_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRAutoTune_typ;

typedef struct HCROmega0
{
	/* VAR_INPUT (analog) */
	float A_Cyl_pos;
	float A_Cyl_neg;
	float V0_pos;
	float V0_neg;
	float h_Cyl;
	float x_Cyl_min;
	float x_Cyl_max;
	unsigned char Update;
	float x_Cyl;
	/* VAR_OUTPUT (analog) */
	float x0_Cyl;
	float Kp_xCorr;
	unsigned short status;
	/* VAR (analog) */
	struct HCROmega0_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCROmega0_typ;

typedef struct HCRvyServo
{
	/* VAR_INPUT (analog) */
	float pSupply_max;
	float pSupply;
	float pTank;
	float A_Cyl_pos;
	float A_Cyl_neg;
	float Qnom_pos;
	float Qnom_neg;
	float pDelta_nom;
	float yValve_0;
	float yValve_min;
	unsigned char Update;
	float pCyl_pos;
	float pCyl_neg;
	float vCyl_abs;
	float vCyl_rel;
	/* VAR_OUTPUT (analog) */
	float Qy_abs;
	float Qy_rel;
	float Qy;
	float yValve;
	signed char MoveDir;
	unsigned short status;
	/* VAR (analog) */
	struct HCRvyServo_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Neg_y;
	plcbit Zero_yValve;
} HCRvyServo_typ;

typedef struct HCRvyServo_Fp
{
	/* VAR_INPUT (analog) */
	float pSupply_max;
	float pSupply;
	float pTank;
	float A_Cyl_pos;
	float A_Cyl_neg;
	float Qnom_pos;
	float Qnom_neg;
	float pDelta_nom;
	float yValve_0;
	float yValve_min;
	unsigned char Update;
	float Fp_Cyl;
	float Ff_Cyl_pos;
	float Ff_Cyl_neg;
	float vCyl_abs;
	float vCyl_rel;
	/* VAR_OUTPUT (analog) */
	float Qy_abs;
	float Qy_rel;
	float Qy;
	float yValve;
	signed char MoveDir;
	unsigned short status;
	/* VAR (analog) */
	struct HCRvyServo_Fp_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Neg_y;
	plcbit Zero_yValve;
} HCRvyServo_Fp_typ;

typedef struct HCRFyServo
{
	/* VAR_INPUT (analog) */
	float pSupply_max;
	float pSupply;
	float pTank;
	float A_Cyl_pos;
	float A_Cyl_neg;
	float Qnom_pos;
	float Qnom_neg;
	float pDelta_nom;
	float yValve_0;
	float V0_pos;
	float V0_neg;
	float h_Cyl;
	unsigned char Update;
	float x_Cyl;
	float v_Cyl;
	float pCyl_pos;
	float pCyl_neg;
	float dF_Cyl_abs;
	float dF_Cyl_rel;
	/* VAR_OUTPUT (analog) */
	float Qy;
	float yValve;
	unsigned short status;
	/* VAR (analog) */
	struct HCRFyServo_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Neg_y;
} HCRFyServo_typ;

typedef struct HCRFyServo_Fp
{
	/* VAR_INPUT (analog) */
	float pSupply_max;
	float pSupply;
	float pTank;
	float A_Cyl_pos;
	float A_Cyl_neg;
	float Qnom_pos;
	float Qnom_neg;
	float yValve_0;
	unsigned char Update;
	float Fp;
	float u;
	/* VAR_OUTPUT (analog) */
	float Qy;
	float yValve;
	unsigned short status;
	/* VAR (analog) */
	struct HCRFyServo_Fp_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Neg_y;
} HCRFyServo_Fp_typ;

typedef struct HCRLinSpline
{
	/* VAR_INPUT (analog) */
	unsigned short NbNodes;
	struct HCRNode_typ* pNodeVec;
	unsigned char Smooth;
	float Rness;
	signed char DisMode;
	float x;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRLinSpline_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Invert;
	plcbit Update;
} HCRLinSpline_typ;

typedef struct HCRLinValve
{
	/* VAR_INPUT (analog) */
	unsigned short NbNodes;
	struct HCRNode_typ* pNodeVec;
	signed char DisMode;
	float Qy;
	/* VAR_OUTPUT (analog) */
	float Uy;
	unsigned short status;
	/* VAR (analog) */
	struct HCRLinValve_Internal_typ Internal;
	struct HCRLinSpline LinSpline;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Neg_Qy;
	plcbit Neg_Uy;
	plcbit Update;
} HCRLinValve_typ;

typedef struct HCRPID
{
	/* VAR_INPUT (analog) */
	struct HCRPIDPara_typ Parameter;
	unsigned char Update;
	float W;
	float X;
	signed char I_Switch;
	unsigned char CTRLmode;
	float Yman;
	float Yadd;
	/* VAR_OUTPUT (analog) */
	float e;
	float Yp;
	float Yi;
	float Yd;
	float Y;
	unsigned short status;
	/* VAR (analog) */
	struct HCRPID_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Invert;
} HCRPID_typ;

typedef struct HCRP2P_Cont
{
	/* VAR_INPUT (analog) */
	float s_set;
	float s_act;
	float Kp;
	signed char P2Pmode;
	struct HCRP2P_Cont_AddPar_typ* pAddPar;
	/* VAR_OUTPUT (analog) */
	float Y_P2P;
	signed short Y_P2P_int;
	unsigned short status;
	/* VAR (analog) */
	struct HCRP2P_Cont_Internal_typ Internal;
	struct HCRPID PID;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HCRP2P_Cont_typ;

typedef struct HCRs_Cont
{
	/* VAR_INPUT (analog) */
	struct HCRSystemPara_typ* pSystem_Para;
	struct HCRValvePara_typ* pValve_Para;
	struct HCRPIDPara_s_typ* pPID_Para_s;
	unsigned char vyServoMode;
	unsigned char Update;
	float s_set;
	float s_act;
	unsigned char I_Switch;
	unsigned char CTRLmode;
	float v_man;
	float v_add;
	float v_set;
	float Fp_Cyl;
	float Ff_Cyl_pos;
	float Ff_Cyl_neg;
	float Qy_Oset_ext;
	unsigned char Oset_Mode;
	/* VAR_OUTPUT (analog) */
	float Qy_s;
	float Qy_Oset_s;
	float Qy;
	float yValve;
	signed char MoveDir;
	unsigned short status;
	/* VAR (analog) */
	struct HCRs_Cont_Internal_typ Internal;
	struct HCRPID PID;
	struct HCRvyServo_Fp vyServo_Fp;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Invert;
} HCRs_Cont_typ;

typedef struct HCRs_Cont_p
{
	/* VAR_INPUT (analog) */
	struct HCRSystemPara_typ* pSystem_Para;
	struct HCRValvePara_typ* pValve_Para;
	struct HCRPIDPara_s_typ* pPID_Para_s;
	unsigned char vyServoMode;
	unsigned char Update;
	float s_set;
	float s_act;
	unsigned char I_Switch;
	unsigned char CTRLmode;
	float v_man;
	float v_add;
	float v_set;
	float pCyl_pos;
	float pCyl_neg;
	float Qy_Oset_ext;
	unsigned char Oset_Mode;
	/* VAR_OUTPUT (analog) */
	float Qy_s;
	float Qy_Oset_s;
	float Qy;
	float yValve;
	signed char MoveDir;
	unsigned short status;
	/* VAR (analog) */
	struct HCRs_Cont_p_Internal_typ Internal;
	struct HCRPID PID;
	struct HCRvyServo vyServo;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Invert;
} HCRs_Cont_p_typ;

typedef struct HCRF_Cont
{
	/* VAR_INPUT (analog) */
	struct HCRSystemPara_typ* pSystem_Para;
	struct HCRValvePara_typ* pValve_Para;
	struct HCRPIDPara_F_typ* pPID_Para_F;
	unsigned char FyServoMode;
	unsigned char Update;
	float Fp_set;
	float Fp_act;
	unsigned char I_Switch;
	unsigned char CTRLmode;
	float Q_man;
	float Q_add;
	float v_act;
	float Qy_Oset_ext;
	unsigned char Oset_Mode;
	/* VAR_OUTPUT (analog) */
	float Qy_F;
	float Qy_Oset_F;
	float Qy;
	float yValve;
	unsigned short status;
	/* VAR (analog) */
	struct HCRF_Cont_Internal_typ Internal;
	struct HCRPID PID;
	struct HCRFyServo_Fp FyServo_Fp;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Invert;
} HCRF_Cont_typ;

typedef struct HCRF_Cont_p
{
	/* VAR_INPUT (analog) */
	struct HCRSystemPara_typ* pSystem_Para;
	struct HCRSystemParaAdd_typ* pSystem_ParaAdd;
	struct HCRValvePara_typ* pValve_Para;
	struct HCRPIDPara_F_typ* pPID_Para_F;
	unsigned char FyServoMode;
	unsigned char Update;
	float Fp_set;
	float Fp_act;
	unsigned char I_Switch;
	unsigned char CTRLmode;
	float Q_man;
	float Q_add;
	float dFp_set;
	float x_Cyl;
	float v_Cyl;
	float pCyl_pos;
	float pCyl_neg;
	float v_act;
	float Qy_Oset_ext;
	unsigned char Oset_Mode;
	/* VAR_OUTPUT (analog) */
	float Qy_F;
	float Qy_Oset_F;
	float Qy;
	float yValve;
	unsigned short status;
	/* VAR (analog) */
	struct HCRF_Cont_p_Internal_typ Internal;
	struct HCRPID PID;
	struct HCRFyServo FyServo;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Invert;
} HCRF_Cont_p_typ;

typedef struct HCRsF_Cont
{
	/* VAR_INPUT (analog) */
	struct HCRSystemPara_typ* pSystem_Para;
	struct HCRValvePara_typ* pValve_Para;
	struct HCRPIDPara_s_typ* pPID_Para_s;
	struct HCRPIDPara_F_typ* pPID_Para_F;
	unsigned char vyServoMode;
	unsigned char FyServoMode;
	float tSwitch;
	unsigned char Update;
	float s_set;
	float s_act;
	struct HCRs_Cont_InVar_typ s_Cont_InVar;
	float Fp_set;
	float Fp_act;
	struct HCRF_Cont_InVar_typ F_Cont_InVar;
	unsigned char CTRLmode;
	unsigned char uSwitch;
	/* VAR_OUTPUT (analog) */
	float Qy_s;
	float Qy_Oset_s;
	float Qy_F;
	float Qy_Oset_F;
	float Qy;
	float yValve;
	signed char MoveDir;
	unsigned short status;
	/* VAR (analog) */
	struct HCRsF_Cont_Internal_typ Internal;
	struct HCRs_Cont s_Cont;
	struct HCRF_Cont F_Cont;
	struct HCRJoltFree JoltFree;
	/* VAR_INPUT (digital) */
	plcbit enable;
	/* VAR_OUTPUT (digital) */
	plcbit InForceCtrl;
} HCRsF_Cont_typ;

typedef struct HCRsF_Cont_p
{
	/* VAR_INPUT (analog) */
	struct HCRSystemPara_typ* pSystem_Para;
	struct HCRSystemParaAdd_typ* pSystem_ParaAdd;
	struct HCRValvePara_typ* pValve_Para;
	struct HCRPIDPara_s_typ* pPID_Para_s;
	struct HCRPIDPara_F_typ* pPID_Para_F;
	unsigned char vyServoMode;
	unsigned char FyServoMode;
	float tSwitch;
	unsigned char Update;
	float s_set;
	float s_act;
	struct HCRs_Cont_p_InVar_typ s_Cont_p_InVar;
	float Fp_set;
	float Fp_act;
	struct HCRF_Cont_p_InVar_typ F_Cont_InVar;
	unsigned char CTRLmode;
	unsigned char uSwitch;
	/* VAR_OUTPUT (analog) */
	float Qy_s;
	float Qy_Oset_s;
	float Qy_F;
	float Qy_Oset_F;
	float Qy;
	float yValve;
	signed char MoveDir;
	unsigned short status;
	/* VAR (analog) */
	struct HCRsF_Cont_p_Internal_typ Internal;
	struct HCRs_Cont_p s_Cont_p;
	struct HCRF_Cont F_Cont;
	struct HCRF_Cont_p F_Cont_p;
	struct HCRvyServo vyServo;
	struct HCRJoltFree JoltFree;
	/* VAR_INPUT (digital) */
	plcbit enable;
	/* VAR_OUTPUT (digital) */
	plcbit InForceCtrl;
} HCRsF_Cont_p_typ;



/* Prototyping of functions and function blocks */
_BUR_PUBLIC void HCRPT1(struct HCRPT1* inst);
_BUR_PUBLIC void HCRPT2(struct HCRPT2* inst);
_BUR_PUBLIC void HCRPT2s(struct HCRPT2s* inst);
_BUR_PUBLIC void HCRDT1(struct HCRDT1* inst);
_BUR_PUBLIC void HCRDT2(struct HCRDT2* inst);
_BUR_PUBLIC void HCRDT2s(struct HCRDT2s* inst);
_BUR_PUBLIC void HCRIT1(struct HCRIT1* inst);
_BUR_PUBLIC void HCRNotch(struct HCRNotch* inst);
_BUR_PUBLIC void HCRPT1cP(struct HCRPT1cP* inst);
_BUR_PUBLIC void HCRJoltFree(struct HCRJoltFree* inst);
_BUR_PUBLIC void HCRFIFO(struct HCRFIFO* inst);
_BUR_PUBLIC void HCRRampGen(struct HCRRampGen* inst);
_BUR_PUBLIC void HCRSPF(struct HCRSPF* inst);
_BUR_PUBLIC void HCRSFG(struct HCRSFG* inst);
_BUR_PUBLIC void HCRAutoTune(struct HCRAutoTune* inst);
_BUR_PUBLIC void HCROmega0(struct HCROmega0* inst);
_BUR_PUBLIC void HCRvyServo(struct HCRvyServo* inst);
_BUR_PUBLIC void HCRvyServo_Fp(struct HCRvyServo_Fp* inst);
_BUR_PUBLIC void HCRFyServo(struct HCRFyServo* inst);
_BUR_PUBLIC void HCRFyServo_Fp(struct HCRFyServo_Fp* inst);
_BUR_PUBLIC void HCRLinSpline(struct HCRLinSpline* inst);
_BUR_PUBLIC void HCRLinValve(struct HCRLinValve* inst);
_BUR_PUBLIC void HCRPID(struct HCRPID* inst);
_BUR_PUBLIC void HCRP2P_Cont(struct HCRP2P_Cont* inst);
_BUR_PUBLIC void HCRs_Cont(struct HCRs_Cont* inst);
_BUR_PUBLIC void HCRs_Cont_p(struct HCRs_Cont_p* inst);
_BUR_PUBLIC void HCRF_Cont(struct HCRF_Cont* inst);
_BUR_PUBLIC void HCRF_Cont_p(struct HCRF_Cont_p* inst);
_BUR_PUBLIC void HCRsF_Cont(struct HCRsF_Cont* inst);
_BUR_PUBLIC void HCRsF_Cont_p(struct HCRsF_Cont_p* inst);


#ifdef __cplusplus
};
#endif
#endif /* _ASHYDCON_ */

