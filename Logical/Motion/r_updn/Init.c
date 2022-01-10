
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <string.h>

/**************************************************************************************
                            Function Declarations
**************************************************************************************/
void ConfigureParID();


void _INIT ProgramInit(void)
{
	mcAxis.AxisID.Address = (UDINT)&(gAxisUpDnR); 	
	strcpy(mcAxis.AxisID.Name,"R_Robot up/down");
	mcAxis.AxisID.UnitFactor = 0.01;
	strcpy(mcAxis.AxisID.UnitName, "mm");
	mcAxis.AxisID.AxisType = ncAXIS;

	mcAxis.Parameter.Velocity 	        = 1000; /*velocity for movement*/
	mcAxis.Parameter.Acceleration 		= 4000; /*acceleration for movement*/
	mcAxis.Parameter.Deceleration 		= 4000; /*deceleration for movement*/
	mcAxis.Parameter.JogVelocity		= 400; 	/*velocity for jogging */
	mcAxis.Parameter.OverrideVelocity	= 1.0;
	mcAxis.Parameter.OverrideAcceleration=1.0;

	ConfigureParID();
	
	mcParID_Value.encoderPar.count_dir = ncSTANDARD;
	
	FixUpDnR.pidPar.kv_p = 400.0;		/* 480  */
	FixUpDnR.pidPar.kv_s = 0.35;    	/* 0.67 */  
	FixUpDnR.pidPar.tn_s = 0.1;
	
	FixUpDnR.pidPar.kpJunction  = 0.23;
	FixUpDnR.pidPar.kpMotorTemp = 0.23;

	FixUpDnR.limitPar.a1_pos = 960000;
	FixUpDnR.limitPar.a2_pos = 960000;
	FixUpDnR.limitPar.a1_neg = 960000;
	FixUpDnR.limitPar.a2_neg = 960000;

	FixUpDnR.limitPar.v_pos = 60 * 1600;		/*  3000RPM  */
	FixUpDnR.limitPar.v_neg = 60 * 1600;		/*  3000RPM  */
}

/* ------------- configure function ------------- */
void ConfigureParID()
{	
	/* -------------------- configure mc par id init -------------------- */	
	/* ------------- VCTRL_ENCOD_COUNT_DIR ---------------- */
	mcParID_RW.init[0].DataAddress = (UDINT) & mcParID_Value.encoderPar.count_dir;
	mcParID_RW.init[0].DataType    = ncPAR_TYP_USINT;
	mcParID_RW.init[0].ParID       = ACP10PAR_VCTRL_ENCOD_COUNT_DIR;	
	
	/* ------------- VCTRL_SCALE_LOAD_UNITS ---------------- */
	mcParID_RW.init[1].DataAddress = (UDINT) & mcParID_Value.encoderPar.load_units;
	mcParID_RW.init[1].DataType    = ncPAR_TYP_UDINT;
	mcParID_RW.init[1].ParID       = ACP10PAR_VCTRL_SCALE_LOAD_UNITS;	

	/* ------------- VCTRL_SCALE_LOAD_MOTREV ---------------- */
	mcParID_RW.init[2].DataAddress = (UDINT) & mcParID_Value.encoderPar.encoder_rev;
	mcParID_RW.init[2].DataType    = ncPAR_TYP_UDINT;
	mcParID_RW.init[2].ParID       = ACP10PAR_VCTRL_SCALE_LOAD_MOTREV;
		
	/* -------------------- configure mc par id write -------------------- */	
	/* ------------- AXLIM_V_POS ---------------- */
	mcParID_RW.Write[0].DataAddress = (UDINT) & mcParID_Value.limitPar.v_pos;
	mcParID_RW.Write[0].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[0].ParID       = ACP10PAR_AXLIM_V_POS;	
	
	/* ------------- AXLIM_V_NEG ---------------- */
	mcParID_RW.Write[1].DataAddress = (UDINT) & mcParID_Value.limitPar.v_neg;
	mcParID_RW.Write[1].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[1].ParID       = ACP10PAR_AXLIM_V_NEG;	

	/* ------------- AXLIM_A1_POS ---------------- */
	mcParID_RW.Write[2].DataAddress = (UDINT) & mcParID_Value.limitPar.a1_pos;
	mcParID_RW.Write[2].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[2].ParID       = ACP10PAR_AXLIM_A1_POS;	

	/* ------------- AXLIM_A1_NEG ---------------- */
	mcParID_RW.Write[3].DataAddress = (UDINT) & mcParID_Value.limitPar.a1_neg;
	mcParID_RW.Write[3].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[3].ParID       = ACP10PAR_AXLIM_A1_NEG;	

	/* ------------- AXLIM_A2_POS ---------------- */
	mcParID_RW.Write[4].DataAddress = (UDINT) & mcParID_Value.limitPar.a2_pos;
	mcParID_RW.Write[4].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[4].ParID       = ACP10PAR_AXLIM_A2_POS;	

	/* ------------- AXLIM_A2_NEG ---------------- */
	mcParID_RW.Write[5].DataAddress = (UDINT) & mcParID_Value.limitPar.a2_neg;
	mcParID_RW.Write[5].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[5].ParID       = ACP10PAR_AXLIM_A2_NEG;	
	
	/* ------------- POS_CTRL_KV ---------------- */
	mcParID_RW.Write[6].DataAddress = (UDINT) & mcParID_Value.pidPar.kv_p;
	mcParID_RW.Write[6].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[6].ParID       = ACP10PAR_POS_CTRL_KV;	

	/* ------------- POS_CTRL_TN ---------------- */
	mcParID_RW.Write[7].DataAddress = (UDINT) & mcParID_Value.pidPar.tn_p;
	mcParID_RW.Write[7].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[7].ParID       = ACP10PAR_POS_CTRL_TN;	
	
	/* ------------- SCTRL_KV ---------------- */
	mcParID_RW.Write[8].DataAddress = (UDINT) & mcParID_Value.pidPar.kv_s;
	mcParID_RW.Write[8].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[8].ParID       = ACP10PAR_SCTRL_KV;	
	
	/* ------------- SCTRL_TN ---------------- */
	mcParID_RW.Write[9].DataAddress = (UDINT) & mcParID_Value.pidPar.tn_s;
	mcParID_RW.Write[9].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[9].ParID       = ACP10PAR_SCTRL_TN;
	
	/* ------------- SCTRL_TI_FIL ---------------- */
	mcParID_RW.Write[10].DataAddress = (UDINT) & mcParID_Value.pidPar.tf_s;
	mcParID_RW.Write[10].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[10].ParID       = ACP10PAR_SCTRL_TI_FIL;
	
	/* ------------- VAR_R4_2+0 ---------------- */
	mcParID_RW.Write[11].DataAddress = (UDINT) & mcParID_Value.pidPar.kpJunction;
	mcParID_RW.Write[11].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[11].ParID       = ACP10PAR_VAR_R4_2+0;

	/* ------------- VAR_R4_3+0 ---------------- */
	mcParID_RW.Write[12].DataAddress = (UDINT) & mcParID_Value.pidPar.kpMotorTemp;
	mcParID_RW.Write[12].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[12].ParID       = ACP10PAR_VAR_R4_3+0;

	/* -------------------- configure mc par id read -------------------- */
	/* ------------- MOTOR_TORQ_RATED ---------------- */
	mcParID_RW.Read[0].DataAddress = (UDINT) & mcParID_Value.readPar.torqueRated;
	mcParID_RW.Read[0].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[0].ParID       = ACP10PAR_MOTOR_TORQ_RATED;
	
	/* ------------- MOTOR_CURR_RATED ---------------- */
	mcParID_RW.Read[1].DataAddress = (UDINT) & mcParID_Value.readPar.currentRated;
	mcParID_RW.Read[1].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[1].ParID       = ACP10PAR_MOTOR_CURR_RATED;
	
	/* ------------- MOTOR_COMMUT_OFFSET ---------------- */
	mcParID_RW.Read[2].DataAddress = (UDINT) & mcParID_Value.readPar.offset;
	mcParID_RW.Read[2].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[2].ParID       = ACP10PAR_MOTOR_COMMUT_OFFSET;
	
	/* ------------- UDC_ACT ---------------- */
	mcParID_RW.Read[3].DataAddress = (UDINT) & mcParID_Value.readPar.actUDC;
	mcParID_RW.Read[3].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[3].ParID       = ACP10PAR_UDC_ACT;
	
	/* ------------- ICTRL_ISQ_ACT ---------------- */
	mcParID_RW.Read[4].DataAddress = (UDINT) & mcParID_Value.readPar.actCurrent;
	mcParID_RW.Read[4].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[4].ParID       = ACP10PAR_ICTRL_ISQ_ACT;
		
	/* ------------- TEMP_MOTOR ---------------- */
	mcParID_RW.Read[5].DataAddress = (UDINT) & mcParID_Value.readPar.tempMotor;
	mcParID_RW.Read[5].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[5].ParID       = ACP10PAR_TEMP_MOTOR;

	/* ------------- TEMP_JUNCTION ---------------- */
	mcParID_RW.Read[6].DataAddress = (UDINT) & mcParID_Value.readPar.tempJunction;
	mcParID_RW.Read[6].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[6].ParID       = ACP10PAR_TEMP_JUNCTION;
	
	/* ------------- MOTOR_TORQ_MAX ---------------- */
	mcParID_RW.Read[7].DataAddress = (UDINT) & mcParID_Value.readPar.torqueMax;
	mcParID_RW.Read[7].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[7].ParID       = ACP10PAR_MOTOR_TORQ_MAX;

}/* void ConfigureParID() */
