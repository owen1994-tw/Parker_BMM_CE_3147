/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _HYDCTRL_
#define _HYDCTRL_
#ifdef __cplusplus
extern "C" 
{
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
/* Constants */
#ifdef _REPLACE_CONST
 #define HYD_THICK_POINTS_MAX 100
#else
 #ifndef _GLOBAL_CONST
   #define _GLOBAL_CONST _WEAK const
 #endif
 _GLOBAL_CONST signed long HYD_THICK_POINTS_MAX;
#endif




/* Datatypes and datatypes of function blocks */
typedef struct HydMovePara_typ
{	float t_Jolt;
	float vC_pos;
	float vC_neg;
	float aAcc_pos;
	float aDec_pos;
	float aAcc_neg;
	float aDec_neg;
} HydMovePara_typ;

typedef struct HydSPFCos_Internal_typ
{	signed short step;
	signed short reserve;
	float Ts;
	float Td;
	float Timer;
	float TimerInternal;
	float T1;
	float T2;
	float S1;
	float S2;
	float V1;
	float V2;
	float s_act;
	float s_end;
	float sDelta;
	float s_accel;
	float s_const;
	float s_decel;
	float v_set;
	float v_0;
	float accel;
	float decel;
	float t_v_accel;
	float t_v_const;
	float t_v_decel;
	float a_out;
	float v_out;
	float s_out;
	float v;
} HydSPFCos_Internal_typ;

typedef struct HydSPFJerk_Internal_typ
{	signed short step;
	float s_end;
	float v_set;
	float v_0;
	float aAcc;
	float aAcc_0;
	float aAcc_1;
	float aDec;
	float aDec_0;
	float aDec_1;
	float t_Jolt_limit;
	float t_Jolt;
	float t_Jolt_Acc_0;
	float v_Jolt_Acc_0;
	float s_Jolt_Acc_0;
	float t_Jolt_Acc_1;
	float v_Jolt_Acc_1;
	float s_Jolt_Acc_1;
	float t_Jolt_Acc_2;
	float v_Jolt_Acc_2;
	float s_Jolt_Acc_2;
	float jeck_Acc;
	float t_v_const;
	float v_v_const;
	float s_v_const;
	float t_Jolt_Dec_0;
	float v_Jolt_Dec_0;
	float s_Jolt_Dec_0;
	float t_Jolt_Dec_1;
	float v_Jolt_Dec_1;
	float s_Jolt_Dec_1;
	float t_Jolt_Dec_2;
	float v_Jolt_Dec_2;
	float s_Jolt_Dec_2;
	float jeck_Dec;
	float sDelta;
	float sBrake;
	float Timer;
	float Ts;
	float TimerInternal;
	float T1;
	float T2;
	float S1;
	float S2;
	float V1;
	float V2;
	float s_out;
	float v_out;
	float a_out;
	float j_out;
	plcbit enable_old;
} HydSPFJerk_Internal_typ;

typedef struct Hyd_Node_typ
{	float X;
	float Y;
} Hyd_Node_typ;

typedef struct HydvProfJerk_Internal_typ
{	signed short step;
	signed short stepSub;
	float Timer;
	float Ts;
	float Tj;
	float Td;
	float t_accDec;
	float jeck_Dec;
	float v_set;
	float v_set_old;
	float v_0;
	float s_v_setChange;
	float s_v_destChange;
	float s_Jolt_Start;
	float v_Jolt_Dec_0;
	float s_Jolt_Dec_0;
	float t_Jolt_Dec_0;
	float v_Jolt_Dec_1;
	float s_Jolt_Dec_1;
	float t_Jolt_Dec_1;
	float v_Jolt_Dec_2;
	float s_Jolt_Dec_2;
	float t_Jolt_Dec_2;
	float T2;
	float S1;
	float S2;
	float V1;
	float V2;
	float accel;
	float decel;
	float decel_end;
	float v_out;
	float v_out_change;
	float sLimit;
	float s_act;
	float s_end;
	float sDelta;
	float sAcc;
	float v_old1;
	float v_old2;
	float v;
} HydvProfJerk_Internal_typ;

typedef struct LRamp_Internal_typ
{	signed long OldSet;
	signed long Accel;
} LRamp_Internal_typ;

typedef struct P2P_Internal_typ
{	float W;
	float W_old;
	float W_f;
	float W_f_old;
	float X;
	float X_old;
	float e;
	float e_old;
	float e_Tn;
	float Kp;
	float Kw;
	float Ts;
	float Tn;
	float Tn_pos;
	float Tn_neg;
	float dPar;
	float dPar_old;
	float D;
	float Tv;
	float Tf;
	float Y_min;
	float Y_max;
	float FF;
	float Tv_FF;
	float Tf_FF;
	float Tf_W;
	float Ytotal;
	float Ylim;
	float K_Fbk;
	float Y_Fbk_ext;
	unsigned char d_mode;
	unsigned char FF_mode;
	unsigned char run_mode;
	float dY_max;
	plcbit enable_old;
} P2P_Internal_typ;

typedef struct P2P_Para_typ
{	float Kp;
	float Tn_pos;
	float Tn_neg;
	float K_Fbk;
	float Tf_W;
	float Tv;
	float Tf;
	float dY_max;
	float Y_min;
	float Y_max;
	float Tv_FF;
	float Tf_FF;
	float e_Tn;
	unsigned char d_mode;
	unsigned char FF_mode;
	unsigned char run_mode;
} P2P_Para_typ;

typedef struct PID_Internal_typ
{	float WX_min;
	float WX_max;
	float W;
	float W_old;
	float W_f;
	float W_f_old;
	float X;
	float X_old;
	float e;
	float e_old;
	float Kp;
	float Kw;
	float Ts;
	float Tn;
	float dPar;
	float dPar_old;
	float D;
	float Tv;
	float Tf;
	float Y_min;
	float Y_max;
	float FF;
	float Tv_FF;
	float Tf_FF;
	float Ytotal;
	float Ylim;
	float K_Fbk;
	float Y_Fbk_ext;
	unsigned char d_mode;
	unsigned char FF_mode;
	float dY_max;
	plcbit enable_old;
} PID_Internal_typ;

typedef struct PID_Para_typ
{	float WX_min;
	float WX_max;
	float Kw;
	float Kp;
	float Tn;
	float K_Fbk;
	float Tv;
	float Tf;
	float dY_max;
	float Y_min;
	float Y_max;
	float Tv_FF;
	float Tf_FF;
	unsigned char d_mode;
	unsigned char FF_mode;
} PID_Para_typ;

typedef struct ProfGen_Internal_typ
{	float vSet;
	float vOut;
	float accel;
	float vCreep;
	float vLimit;
} ProfGen_Internal_typ;

typedef struct Profile_Internal_typ
{	float ET;
	float Tf;
	float V_set;
	float V_out;
	float V_out_old;
	float V_limit;
	float Div;
	float Curve;
	float Accel_out;
	unsigned short Step;
} Profile_Internal_typ;

typedef struct Ramp_Internal_typ
{	float ET;
	float Tf;
	float Set;
	float OldOut;
	float Out;
	float Div;
	plcbit Once;
} Ramp_Internal_typ;

typedef struct Thick_Internal_typ
{	float W;
	float W_old;
	float W_f;
	float W_f_old;
	float X;
	float X_old;
	float X_f;
	float X_f_old;
	float e;
	float e_old;
	float Ts;
	float Tf;
	float Tv_FF;
	float Kp;
	float Tv;
	float dY_max;
	float Y_min;
	float Y_max;
	float W_min;
	float W_max;
	float FF;
	float D;
	float Ylim;
	float Ytotal;
	plcbit enable_old;
} Thick_Internal_typ;

typedef struct Thick_Para_typ
{	float Ts;
	float Tf;
	float Tv_FF;
	float Kp;
	float Tv;
	float dY_max;
	float Y_min;
	float Y_max;
	float W_min;
	float W_max;
} Thick_Para_typ;

typedef struct Thick_Profile_Internal_typ
{	float Timer;
	float rIndexPoint;
	float Weight;
	float SetValue[101];
	float ActValue[101];
	float AccuValue;
	float StartAccu;
	float AccuOffset;
	float AccuStroke;
	float AccuBuffer;
	signed long Out;
	plcbit Start_old;
} Thick_Profile_Internal_typ;

typedef struct Thick_Profile_Para_typ
{	plcbit GapType;
	plcbit RunMode;
	signed short IndexOffset;
	float AccuStroke;
	float AccuBuffer;
	signed short AccuMax;
	signed short AccuMin;
	float ThickCycle;
	float Weight;
	float SetValue[101];
	signed short GapMax;
	signed short GapMin;
} Thick_Profile_Para_typ;

typedef struct HydvTimeProf_Internal_typ
{	unsigned short step;
	float Ts;
	float Timer;
	float t_accel;
	float t_const;
	float t_decel;
	float t_move;
	float v0;
	float v_set;
	float v_start;
	float v_end;
	float accel;
	float decel;
	float v_out;
} HydvTimeProf_Internal_typ;

typedef struct HydPumpOnOff
{
	/* VAR_INPUT (analog) */
	float tTask;
	float tOnDelay;
	float tOffDelay;
	float tProtection;
	float tAccuChargeOffDelay;
	/* VAR_OUTPUT (analog) */
	unsigned char ledMotor;
	/* VAR (analog) */
	unsigned long cntOffDelay;
	unsigned long cntOnDelay;
	unsigned long cntProtection;
	unsigned long cntAccuChargeOffDelay;
	/* VAR_INPUT (digital) */
	plcbit bButtonOnOff;
	plcbit bButtonOff;
	plcbit bFeedback;
	plcbit diMotorOn;
	/* VAR_OUTPUT (digital) */
	plcbit doContactor;
	plcbit doOn;
	plcbit doAccuCharge;
	/* VAR (digital) */
	plcbit bButtonOnOffOld;
	plcbit bdoOnOld;
} HydPumpOnOff_typ;

typedef struct HydvProfGen
{
	/* VAR_INPUT (analog) */
	float Ts;
	float Tj;
	float Td;
	float accel;
	float decel;
	float decel_end;
	float v_act;
	float v_start;
	float v_set;
	float v_end;
	float s_act;
	float s_end;
	float decel_gain;
	float v_zero;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	float v_out;
	/* VAR (analog) */
	struct HydvProfJerk_Internal_typ Internal;
	float s_start;
	float v_set_old;
	/* VAR_INPUT (digital) */
	plcbit init;
	plcbit end;
} HydvProfGen_typ;

typedef struct HydPT1
{
	/* VAR_INPUT (analog) */
	float Ts;
	float Tf;
	float u;
	/* VAR_OUTPUT (analog) */
	float y;
	float f0;
} HydPT1_typ;

typedef struct HydHT1
{
	/* VAR_INPUT (analog) */
	float Ts;
	float Tf;
	float u;
	/* VAR_OUTPUT (analog) */
	float y;
	float f0;
	/* VAR (analog) */
	float u_old;
} HydHT1_typ;

typedef struct HydBandPass
{
	/* VAR_INPUT (analog) */
	float Ts;
	float f0;
	float bandWidth;
	float u;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	float t1;
	struct HydPT1 PT1;
	float t2;
	struct HydHT1 HT1;
} HydBandPass_typ;

typedef struct HydBandStop
{
	/* VAR_INPUT (analog) */
	float Ts;
	float f0;
	float bandWidth;
	float u;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	float t1;
	struct HydPT1 PT1;
	float t2;
	struct HydHT1 HT1;
} HydBandStop_typ;

typedef struct HydvProfJerk
{
	/* VAR_INPUT (analog) */
	float Ts;
	float Tj;
	float accel;
	float decel;
	float decel_end;
	float v_set;
	float v_creep;
	float s_end;
	float s_act;
	/* VAR_OUTPUT (analog) */
	float v_out;
	unsigned short status;
	/* VAR (analog) */
	struct HydvProfJerk_Internal_typ Internal;
	float s_start;
	float sAcc;
	/* VAR_INPUT (digital) */
	plcbit init;
	plcbit end;
} HydvProfJerk_typ;

typedef struct HydSPF_PJerk
{
	/* VAR_INPUT (analog) */
	float Ts;
	struct HydMovePara_typ movePara;
	float s_act;
	float s_end;
	float v_accelMin;
	float v_creep;
	/* VAR_OUTPUT (analog) */
	float j_out;
	float a_out;
	float v_out;
	float s_out;
	signed char moveDir;
	signed char reached;
	unsigned short status;
	/* VAR (analog) */
	struct HydSPFJerk_Internal_typ internal;
	float s_start;
	float v_old;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit init;
	plcbit brake;
	plcbit stop;
	plcbit accelMode;
} HydSPF_PJerk_typ;

typedef struct HydSPF_PCos
{
	/* VAR_INPUT (analog) */
	float Ts;
	float Td;
	float accel;
	float decel;
	float s_act;
	float s_end;
	float v_accelMin;
	float v_set;
	float v_creep;
	float v_act;
	/* VAR_OUTPUT (analog) */
	float a_out;
	float v_out;
	float s_out;
	unsigned short status;
	/* VAR (analog) */
	struct HydSPFCos_Internal_typ internal;
	float s_start;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit init;
	plcbit brake;
	plcbit stop;
	plcbit accelMode;
} HydSPF_PCos_typ;

typedef struct HydSPF_TCos
{
	/* VAR_INPUT (analog) */
	float Ts;
	float accel;
	float decel;
	float s_act;
	float s_end;
	float v_set;
	/* VAR_OUTPUT (analog) */
	float a_out;
	float v_out;
	float s_out;
	unsigned short status;
	/* VAR (analog) */
	struct HydSPFCos_Internal_typ internal;
	float s_start;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit init;
	plcbit brake;
	plcbit stop;
} HydSPF_TCos_typ;

typedef struct HydProfSqrt
{
	/* VAR_INPUT (analog) */
	float Ts;
	signed char moveStatus;
	float vSetPercent;
	float vCreepPercent;
	float vMax;
	float accel;
	float decel;
	float decel_end;
	float s_target;
	float s_act;
	float s_creep;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	float vOutPercent;
	/* VAR (analog) */
	float vSetInternal;
	float vCreepInternal;
	float vOutInternal;
	/* VAR_INPUT (digital) */
	plcbit init;
} HydProfSqrt_typ;

typedef struct HydProfLin
{
	/* VAR_INPUT (analog) */
	float Ts;
	float vSet;
	float vCreep;
	float accel;
	float s_act;
	float s_target;
	float s_creep;
	float kpEnd;
	/* VAR_OUTPUT (analog) */
	signed char moveStatus;
	float vOut;
	/* VAR (analog) */
	struct ProfGen_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit init;
} HydProfLin_typ;

typedef struct HydFIFO
{
	/* VAR_INPUT (analog) */
	unsigned short Times;
	float u;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	unsigned short Count;
	float Buffer[200];
	float y_old;
	unsigned short OldTimes;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HydFIFO_typ;

typedef struct HydProf_Thick
{
	/* VAR_INPUT (analog) */
	float Ts;
	signed short aiGapValue;
	signed short aiAccuValue;
	struct Thick_Profile_Para_typ Parameter;
	/* VAR_OUTPUT (analog) */
	float ActValue[101];
	float Timer;
	signed long CurrentPoint;
	float Out;
	unsigned short Status;
	/* VAR (analog) */
	struct Thick_Profile_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit Start;
	plcbit End;
	plcbit Update;
	plcbit reserve;
} HydProf_Thick_typ;

typedef struct HydPID_Thick
{
	/* VAR_INPUT (analog) */
	struct Thick_Para_typ Parameter;
	float W;
	float X;
	/* VAR_OUTPUT (analog) */
	float e;
	float Yp;
	float Yi;
	float Yd;
	float Y_FF;
	float Y;
	unsigned short status;
	/* VAR (analog) */
	struct Thick_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HydPID_Thick_typ;

typedef struct HydLinear
{
	/* VAR_INPUT (analog) */
	float Y;
	unsigned short nNodes;
	struct Hyd_Node_typ* pNodes;
	/* VAR_OUTPUT (analog) */
	float X;
	float ratio;
	unsigned short errIndex;
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit update;
	/* VAR (digital) */
	plcbit enable_old;
} HydLinear_typ;

typedef struct HydActFlt
{
	/* VAR_INPUT (analog) */
	float u;
	float deltaMax;
	/* VAR_OUTPUT (analog) */
	float y;
	float y_avg;
	unsigned short status;
	/* VAR (analog) */
	unsigned short count;
	float diff;
	unsigned short lost;
	float buffer[10];
	float total;
	/* VAR_INPUT (digital) */
	plcbit enable;
} HydActFlt_typ;

typedef struct HydMidFlt
{
	/* VAR_INPUT (analog) */
	float u;
	unsigned char N;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	unsigned char oldN;
	unsigned long count;
	float buffer[100];
	float sort[100];
	/* VAR_INPUT (digital) */
	plcbit enable;
} HydMidFlt_typ;

typedef struct HydSPF_TJerk
{
	/* VAR_INPUT (analog) */
	float Ts;
	struct HydMovePara_typ movePara;
	float s_home;
	float s_end;
	/* VAR_OUTPUT (analog) */
	float j_out;
	float a_out;
	float v_out;
	float s_out;
	signed char moveDir;
	signed char reached;
	unsigned short status;
	/* VAR (analog) */
	struct HydSPFJerk_Internal_typ internal;
	float v_old;
	float s_start;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit init;
	plcbit brake;
	plcbit stop;
	plcbit setHome;
} HydSPF_TJerk_typ;

typedef struct HydMovAvgFlt
{
	/* VAR_INPUT (analog) */
	signed short u;
	unsigned short Times;
	/* VAR_OUTPUT (analog) */
	signed short y;
	unsigned short status;
	/* VAR (analog) */
	signed long Total;
	unsigned short Count;
	unsigned short OldTimes;
	signed short Buffer[100];
	/* VAR_INPUT (digital) */
	plcbit enable;
} HydMovAvgFlt_typ;

typedef struct HydP2P
{
	/* VAR_INPUT (analog) */
	float Ts;
	struct P2P_Para_typ Parameter;
	float s_set;
	float s_act;
	float Yadd;
	/* VAR_OUTPUT (analog) */
	float e;
	float Yp;
	float Yi;
	float Yd;
	float Y;
	unsigned short status;
	/* VAR (analog) */
	struct P2P_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Update;
} HydP2P_typ;

typedef struct HydPID
{
	/* VAR_INPUT (analog) */
	float Ts;
	struct PID_Para_typ Parameter;
	float W;
	float X;
	float Yadd;
	/* VAR_OUTPUT (analog) */
	float e;
	float Yp;
	float Yi;
	float Yd;
	float Y;
	unsigned short status;
	/* VAR (analog) */
	struct PID_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Update;
} HydPID_typ;

typedef struct HydPT2
{
	/* VAR_INPUT (analog) */
	float Ts;
	float T1;
	float T2;
	float u;
	/* VAR_OUTPUT (analog) */
	float f1;
	float f2;
	float y;
	/* VAR (analog) */
	struct HydPT1 PT1;
} HydPT2_typ;

typedef struct HydDT2
{
	/* VAR_INPUT (analog) */
	float u;
	float Ts;
	float Tv;
	float T1;
	float T2;
	/* VAR_OUTPUT (analog) */
	float y;
	/* VAR (analog) */
	float u_old;
	struct HydPT2 PT2;
	float D;
} HydDT2_typ;

typedef struct HydProfile
{
	/* VAR_INPUT (analog) */
	float Ts;
	float T_dead;
	float Accel;
	float Decel;
	float Decel_end;
	float V_target;
	float V_set;
	float S_act;
	float S_target;
	float V_act;
	/* VAR_OUTPUT (analog) */
	signed char MoveDir;
	float Accel_out;
	float V_out;
	signed char Reached;
	/* VAR (analog) */
	struct Profile_Internal_typ Internal;
	float S_start;
	float V_set_old;
	/* VAR_INPUT (digital) */
	plcbit Init;
} HydProfile_typ;

typedef struct HydSCurveRamp
{
	/* VAR_INPUT (analog) */
	float Set;
	float Accel;
	float Ts;
	/* VAR_OUTPUT (analog) */
	float Accel_out;
	float Out;
	/* VAR (analog) */
	float OldSet;
	struct Ramp_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit Once;
} HydSCurveRamp_typ;

typedef struct HydRampCos
{
	/* VAR_INPUT (analog) */
	float Ts;
	float Set;
	float Accel;
	/* VAR_OUTPUT (analog) */
	float Out;
	/* VAR (analog) */
	struct Ramp_Internal_typ Internal;
	float OldSet;
	/* VAR_INPUT (digital) */
	plcbit Once;
} HydRampCos_typ;

typedef struct HydLinRamp
{
	/* VAR_INPUT (analog) */
	signed short setVal;
	signed long accel;
	signed short Ts;
	/* VAR_OUTPUT (analog) */
	signed short out;
	/* VAR (analog) */
	signed long outOld;
	struct LRamp_Internal_typ Internal;
} HydLinRamp_typ;

typedef struct HydvTimeProf
{
	/* VAR_INPUT (analog) */
	float Ts;
	float t_move;
	float v_set;
	float v_start;
	float v_end;
	float accel;
	float decel;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	float v_out;
	/* VAR (analog) */
	struct HydvTimeProf_Internal_typ internal;
	/* VAR_INPUT (digital) */
	plcbit init;
	/* VAR_OUTPUT (digital) */
	plcbit end;
} HydvTimeProf_typ;

typedef struct HydvProfGenH
{
	/* VAR_INPUT (analog) */
	float Ts;
	float Tj;
	float Td;
	float accel;
	float decel;
	float decel_end;
	float v_act;
	float v_start;
	float v_set;
	float v_end;
	float s_act;
	float s_end;
	float decel_gain;
	float v_zero;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	float v_out;
	/* VAR (analog) */
	struct HydvProfJerk_Internal_typ Internal;
	float s_start;
	float v_set_old;
	/* VAR_INPUT (digital) */
	plcbit init;
	plcbit end;
} HydvProfGenH_typ;

typedef struct HydvProfGenL
{
	/* VAR_INPUT (analog) */
	float Ts;
	float Tj;
	float Td;
	float accel;
	float decel;
	float decel_end;
	float v_act;
	float v_start;
	float v_set;
	float v_end;
	float s_act;
	float s_end;
	float decel_gain;
	float v_zero;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	float v_out;
	/* VAR (analog) */
	struct HydvProfJerk_Internal_typ Internal;
	float s_start;
	float v_limit;
	float a_decel;
	float v_set_old;
	/* VAR_INPUT (digital) */
	plcbit init;
	plcbit end;
} HydvProfGenL_typ;



/* Prototyping of functions and function blocks */
_BUR_PUBLIC void HydPumpOnOff(struct HydPumpOnOff* inst);
_BUR_PUBLIC void HydvProfGen(struct HydvProfGen* inst);
_BUR_PUBLIC void HydPT1(struct HydPT1* inst);
_BUR_PUBLIC void HydHT1(struct HydHT1* inst);
_BUR_PUBLIC void HydBandPass(struct HydBandPass* inst);
_BUR_PUBLIC void HydBandStop(struct HydBandStop* inst);
_BUR_PUBLIC void HydvProfJerk(struct HydvProfJerk* inst);
_BUR_PUBLIC void HydSPF_PJerk(struct HydSPF_PJerk* inst);
_BUR_PUBLIC void HydSPF_PCos(struct HydSPF_PCos* inst);
_BUR_PUBLIC void HydSPF_TCos(struct HydSPF_TCos* inst);
_BUR_PUBLIC void HydProfSqrt(struct HydProfSqrt* inst);
_BUR_PUBLIC void HydProfLin(struct HydProfLin* inst);
_BUR_PUBLIC void HydFIFO(struct HydFIFO* inst);
_BUR_PUBLIC void HydProf_Thick(struct HydProf_Thick* inst);
_BUR_PUBLIC void HydPID_Thick(struct HydPID_Thick* inst);
_BUR_PUBLIC void HydLinear(struct HydLinear* inst);
_BUR_PUBLIC void HydActFlt(struct HydActFlt* inst);
_BUR_PUBLIC void HydMidFlt(struct HydMidFlt* inst);
_BUR_PUBLIC void HydSPF_TJerk(struct HydSPF_TJerk* inst);
_BUR_PUBLIC void HydMovAvgFlt(struct HydMovAvgFlt* inst);
_BUR_PUBLIC void HydP2P(struct HydP2P* inst);
_BUR_PUBLIC void HydPID(struct HydPID* inst);
_BUR_PUBLIC void HydPT2(struct HydPT2* inst);
_BUR_PUBLIC void HydDT2(struct HydDT2* inst);
_BUR_PUBLIC void HydProfile(struct HydProfile* inst);
_BUR_PUBLIC void HydSCurveRamp(struct HydSCurveRamp* inst);
_BUR_PUBLIC void HydRampCos(struct HydRampCos* inst);
_BUR_PUBLIC void HydLinRamp(struct HydLinRamp* inst);
_BUR_PUBLIC void HydvTimeProf(struct HydvTimeProf* inst);
_BUR_PUBLIC void HydvProfGenH(struct HydvProfGenH* inst);
_BUR_PUBLIC void HydvProfGenL(struct HydvProfGenL* inst);


#ifdef __cplusplus
};
#endif
#endif /* _HYDCTRL_ */

