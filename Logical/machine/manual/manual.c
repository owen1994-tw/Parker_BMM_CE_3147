/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: manual
 * File: manual.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program manual --- manual state control 
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "edge.h"               /*edge function                         */
#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"

/**************************************************************************************
                             Function Declarations
**************************************************************************************/
 void ManualCtrl( Mold_typ *pMold);
 void ManualRLCtrl( Mold_typ *pMold);
/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	t_init = 1;         	/* Disable init step: Don't run init step in first cycle */
	t_stop = 1;				/* Enable stop step:  Run stop step in first cycle */

	LeftOption  = 0;
	RightOption = 0;
	LeftRight   = 0;

} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	if (0 == t_init)        	/*init step: run only one time after resume   */
	{
		t_init = 1;				/* Disable init step */
		t_stop = 0;

		gTKmanual.Status = 1;
		Init            = 0;
		gTKmanual.Step++;

	}/* if(t_init == 0 ) */

	/*auto solution  */
	gTKmanual.Active++;      	/*  task auto status number */

	/* ------------------------------------------------------------------------- */
	if(1 == gMachineIn.MainControl && gPanelObj.GetPicture != PIC_INIT && gPanelObj.GetPicture != PIC_SCREEN_SAVE)
	{
		/* ---------------- 左右模选择 ---------------- */
		if(PIC_OPTION == gPanelObj.GetPicture)
		{
			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],PKeyMatric[S$KEYMATRIX_F12]))
			{
				RMold.AutoStep			= 30000;
				RMold.ResetStep			= 30000;
				RMold.LowBlow.Step		= 30000;
				RMold.HighBlow.Step		= 30000;
				RMold.BlowPin.Step		= 30000;
				RMold.BlowPin2.Step		= 30000;
				if(RMold.Carriage.Step < 18000 || RMold.Carriage.Step > 19000)RMold.Carriage.Step = 30000;
				RMold.Clamp.Step		   = 30000;
				//	RMold.CoolPin.Step	   = 30000;
				RMold.CoolPinBlow.Step	= 30000;
				RMold.BlowDeflash.Step	= 30000;
				RMold.Cutter.Step	   	= 30000;
				RMold.TopDeflash.Step	= 30000;
				RMold.BottomDeflash.Step= 30000;
				RMold.SupportAir.Step	= 30000;
				RMold.SupportSuckAir.Step	= 30000;
				RMold.ExtrLift.Step		= 30000;
				RMold.Lock.Step			= 30000;
				RMold.PullBottom.Step	= 30000;
				RMold.InnerPin.Step	   = 30000;
				RMold.CutNeck.Step	   = 30000;
				//	RMold.Robot.Step	      = 30000;
				RMold.SubMold.Step	   = 30000;
				RMold.ChuteDeflashBlow.Step = 30000;
				
				RMold.CoolPin2Blow.Step	= 30000;
				RMold.CoolPin3Blow.Step	= 30000;
				gMacOption.Station = 0;
			}

			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],PKeyMatric[S$KEYMATRIX_F13]))
			{
				LMold.AutoStep			  = 30000;
				LMold.ResetStep	      	= 30000;
				LMold.LowBlow.Step		= 30000;
				LMold.HighBlow.Step		= 30000;
				LMold.BlowPin.Step		= 30000;
				LMold.BlowPin2.Step		= 30000;
				LMold.CoolPinBlow.Step	= 30000;
				if(LMold.Carriage.Step < 18000 || LMold.Carriage.Step > 19000)LMold.Carriage.Step = 30000;
				LMold.Clamp.Step		   = 30000;
				//	LMold.CoolPin.Step	   = 30000;
				LMold.BlowDeflash.Step	= 30000;
				LMold.Cutter.Step		   = 30000;
				LMold.TopDeflash.Step	= 30000;
				LMold.BottomDeflash.Step= 30000;
				LMold.SupportAir.Step	= 30000;
				LMold.SupportSuckAir.Step	= 30000;
				LMold.ExtrLift.Step		= 30000;
				LMold.Lock.Step			= 30000;
				LMold.PullBottom.Step	= 30000;
				LMold.InnerPin.Step	   = 30000;
				LMold.CutNeck.Step	   = 30000;
				//	LMold.Robot.Step	      = 30000;
				LMold.ChuteDeflashBlow.Step = 30000;
				LMold.SubMold.Step	   = 30000;
				
				LMold.CoolPin2Blow.Step	= 30000;
				LMold.CoolPin3Blow.Step	= 30000;
				gMacOption.Station  = 1;
			}

			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],PKeyMatric[S$KEYMATRIX_F14]))
				{
				LMold.AutoStep			   = 30000;
				LMold.ResetStep		   = 30000;
				LMold.LowBlow.Step		= 30000;
				LMold.HighBlow.Step		= 30000;
				LMold.BlowPin.Step		= 30000;
				LMold.BlowPin2.Step		= 30000;
				if(LMold.Carriage.Step < 18000 || LMold.Carriage.Step > 19000)LMold.Carriage.Step = 30000;
				LMold.Clamp.Step		   = 30000;
				//	LMold.CoolPin.Step	   = 30000;
				LMold.CoolPinBlow.Step	= 30000;
				LMold.BlowDeflash.Step	= 30000;
				LMold.Cutter.Step		   = 30000;
				LMold.TopDeflash.Step	= 30000;
				LMold.BottomDeflash.Step= 30000;
				LMold.SupportAir.Step	= 30000;
				LMold.SupportSuckAir.Step	= 30000;
				LMold.ExtrLift.Step		= 30000;
				LMold.Lock.Step			= 30000;
				LMold.PullBottom.Step	= 30000;
				LMold.InnerPin.Step	   = 30000;
				LMold.CutNeck.Step	   = 30000;
				//	LMold.Robot.Step	      = 30000;
				LMold.ChuteDeflashBlow.Step	      = 30000;
				LMold.SubMold.Step	   = 30000;	
				
				LMold.CoolPin2Blow.Step	= 30000;
				LMold.CoolPin3Blow.Step	= 30000;
			
				RMold.AutoStep			   = 30000;
				RMold.ResetStep			= 30000;
				RMold.LowBlow.Step		= 30000;
				RMold.HighBlow.Step	   = 30000;
				RMold.BlowPin.Step		= 30000;
				RMold.BlowPin2.Step		= 30000;
				if(RMold.Carriage.Step < 18000 || RMold.Carriage.Step > 19000)RMold.Carriage.Step	= 30000;
				RMold.Clamp.Step		   = 30000;
				//	RMold.CoolPin.Step  	   = 30000;
				RMold.CoolPinBlow.Step	= 30000;
				RMold.BlowDeflash.Step	= 30000;
				RMold.Cutter.Step		   = 30000;
				RMold.TopDeflash.Step	= 30000;
				RMold.BottomDeflash.Step= 30000;
				RMold.SupportAir.Step	= 30000;
				RMold.SupportSuckAir.Step	= 30000;
				RMold.ExtrLift.Step		= 30000;
				RMold.Lock.Step			= 30000;
				RMold.PullBottom.Step	= 30000;
				RMold.InnerPin.Step	   = 30000;
				RMold.CutNeck.Step  	   = 30000;
				//	RMold.Robot.Step	      = 30000;
				RMold.ChuteDeflashBlow.Step = 30000;
				RMold.SubMold.Step	   = 30000;
				
				RMold.CoolPin2Blow.Step	= 30000;
				RMold.CoolPin3Blow.Step	= 30000;
				gMacOption.Station = 2;
			}
		}/* if(PIC_OPTION == gPanelObj.GetPicture) */
		
		if(1 == gMachineFix.Option.bHideLeftStation)
		{
			gMacOption.Station = 1;	
			RightOption = 1;
			LeftOption = 0;
		}
		
		/* ---------------- 左右模动作 ---------------- */
		if(1 == LeftOption && 0 == LeftOptionOld)
		{
			RightOption = 0;
			LeftRight   = 0;
			
			RMold.AutoStep			   = 30000;
			RMold.ResetStep			= 30000;
			RMold.LowBlow.Step		= 30000;
			RMold.HighBlow.Step		= 30000;
			RMold.BlowPin.Step		= 30000;
			RMold.BlowPin2.Step		= 30000;
			if(RMold.Carriage.Step < 18000 || RMold.Carriage.Step > 19000)RMold.Carriage.Step = 30000;
			RMold.Clamp.Step		   = 30000;
			//	RMold.CoolPin.Step	   = 30000;
			RMold.CoolPinBlow.Step	= 30000;
			RMold.BlowDeflash.Step	= 30000;
			RMold.Cutter.Step	   	= 30000;
			RMold.TopDeflash.Step	= 30000;
			RMold.BottomDeflash.Step= 30000;
			RMold.SupportAir.Step	= 30000;
			RMold.SupportSuckAir.Step	= 30000;
			RMold.ExtrLift.Step		= 30000;
			RMold.Lock.Step			= 30000;
			RMold.PullBottom.Step	= 30000;
			RMold.InnerPin.Step	   = 30000;
			RMold.CutNeck.Step	   = 30000;
			//	RMold.Robot.Step	      = 30000;
			RMold.SubMold.Step	   = 30000;
			RMold.ChuteDeflashBlow.Step = 30000;
			
			RMold.CoolPin2Blow.Step	= 30000;
			RMold.CoolPin3Blow.Step	= 30000;
			
			gMacOption.Station = 0;
		}
		
		LeftOptionOld = LeftOption;
		
		
		if(1 == RightOption && 0 == RightOptionOld)
		{
			LeftOption  = 0;
			LeftRight   = 0;
			
			LMold.AutoStep			   = 30000;
			LMold.ResetStep	      = 30000;
			LMold.LowBlow.Step		= 30000;
			LMold.HighBlow.Step		= 30000;
			LMold.BlowPin.Step		= 30000;
			LMold.BlowPin2.Step		= 30000;
			LMold.CoolPinBlow.Step	= 30000;
			if(LMold.Carriage.Step < 18000 || LMold.Carriage.Step > 19000)LMold.Carriage.Step = 30000;
			LMold.Clamp.Step		   = 30000;
			//	LMold.CoolPin.Step	   = 30000;
			LMold.BlowDeflash.Step	= 30000;
			LMold.Cutter.Step		   = 30000;
			LMold.TopDeflash.Step	= 30000;
			LMold.BottomDeflash.Step= 30000;
			LMold.SupportAir.Step	= 30000;
			LMold.SupportSuckAir.Step	= 30000;
			LMold.ExtrLift.Step		= 30000;
			LMold.Lock.Step			= 30000;
			LMold.PullBottom.Step	= 30000;
			LMold.InnerPin.Step	   = 30000;
			LMold.CutNeck.Step	   = 30000;
			//	LMold.Robot.Step	      = 30000;
			LMold.ChuteDeflashBlow.Step = 30000;
			LMold.SubMold.Step	   = 30000;
			
			LMold.CoolPin2Blow.Step	= 30000;
			LMold.CoolPin3Blow.Step	= 30000;
			gMacOption.Station  = 1;
		}
		
		RightOptionOld = RightOption;
		
		if(1 == LeftRight && 0 == LeftRightOld)
		{	
			LeftOption  = 0;
			RightOption = 0;
			
			LMold.AutoStep			   = 30000;
			LMold.ResetStep		   = 30000;
			LMold.LowBlow.Step		= 30000;
			LMold.HighBlow.Step		= 30000;
			LMold.BlowPin.Step		= 30000;
			LMold.BlowPin2.Step		= 30000;
			if(LMold.Carriage.Step < 18000 || LMold.Carriage.Step > 19000)LMold.Carriage.Step = 30000;
			LMold.Clamp.Step		   = 30000;
			//	LMold.CoolPin.Step	   = 30000;
			LMold.CoolPinBlow.Step	= 30000;
			LMold.BlowDeflash.Step	= 30000;
			LMold.Cutter.Step		   = 30000;
			LMold.TopDeflash.Step	= 30000;
			LMold.BottomDeflash.Step= 30000;
			LMold.SupportAir.Step	= 30000;
			LMold.SupportSuckAir.Step	= 30000;
			LMold.ExtrLift.Step		= 30000;
			LMold.Lock.Step			= 30000;
			LMold.PullBottom.Step	= 30000;
			LMold.InnerPin.Step	   = 30000;
			LMold.CutNeck.Step	   = 30000;
			//	LMold.Robot.Step	      = 30000;
			LMold.ChuteDeflashBlow.Step	      = 30000;
			LMold.SubMold.Step	   = 30000;
			LMold.CoolPin2Blow.Step	= 30000;
			LMold.CoolPin3Blow.Step	= 30000;
			
			RMold.AutoStep			   = 30000;
			RMold.ResetStep			= 30000;
			RMold.LowBlow.Step		= 30000;
			RMold.HighBlow.Step	   = 30000;
			RMold.BlowPin.Step		= 30000;
			RMold.BlowPin2.Step		= 30000;
			if(RMold.Carriage.Step < 18000 || RMold.Carriage.Step > 19000)RMold.Carriage.Step = 30000;
			RMold.Clamp.Step		   = 30000;
			//	RMold.CoolPin.Step  	   = 30000;
			RMold.CoolPinBlow.Step	= 30000;
			RMold.BlowDeflash.Step	= 30000;
			RMold.Cutter.Step		   = 30000;
			RMold.TopDeflash.Step	= 30000;
			RMold.BottomDeflash.Step= 30000;
			RMold.SupportAir.Step	= 30000;
			RMold.SupportSuckAir.Step	= 30000;
			RMold.ExtrLift.Step		= 30000;
			RMold.Lock.Step			= 30000;
			RMold.PullBottom.Step	= 30000;
			RMold.InnerPin.Step	   = 30000;
			RMold.CutNeck.Step  	   = 30000;
			//	RMold.Robot.Step	      = 30000;
			RMold.ChuteDeflashBlow.Step = 30000;
			RMold.SubMold.Step	   = 30000;
			RMold.CoolPin2Blow.Step	= 30000;
			RMold.CoolPin3Blow.Step	= 30000;
			
			gMacOption.Station = 2;
		}
		
		LeftRightOld = LeftRight;
		
		if(0 == gMacOption.Station)
		{
			LeftOption = 1;
			RightOption = 0;
			LeftRight = 0;
		}
		
		if(1 == gMacOption.Station)
		{
			LeftOption  = 0;
			RightOption = 1;
			LeftRight   = 0;
		}
		
		if(2 == gMacOption.Station)
		{
			LeftOption  = 0;
			RightOption = 0;
			LeftRight   = 1;
		}
		
		if((0 == RightOption) && (1 == LeftOption))
		{
			if(0 == LMold.ResetStep)
			{
				ManualCtrl(&LMold);
			}
		}
		
		if((1 == RightOption) && (0 == LeftOption))
		{
			if(0 == RMold.ResetStep)
			{
				ManualCtrl(&RMold);
			}
		}
		
		if ((0 == RightOption) && (0 == LeftOption))
		{
			if(0 == RMold.ResetStep)
			{
				ManualRLCtrl(&RMold);
			}
		}

		
		/* --------------------- 型芯上下 --------------------- */
		if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_DIEGAP_UP])
			{
			SealGapCount++;
			if(2 == (SealGapCount%5))
			{
				if(gThickProf.ShareSet.SealGap > 1)gThickProf.ShareSet.SealGap--;
			}
		}

		DieGapUp = gPanelObj.KeyMatric[S$KEYMATRIX_DIEGAP_UP];

			if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_DIEGAP_DN])
			{
			SealGapCount++;
			if(2 == (SealGapCount%5))
			{
				if(gThickProf.ShareSet.SealGap < 990)gThickProf.ShareSet.SealGap++;
			}
		}

		DieGapDn = gPanelObj.KeyMatric[S$KEYMATRIX_DIEGAP_DN];

			//			if(gThickProf.ShareSet.SealGap < 1  )gThickProf.ShareSet.SealGap = 1;
			if(gThickProf.ShareSet.SealGap > 995)gThickProf.ShareSet.SealGap = 995;
	}/* if(1 == gMachineIn.MainControl) */
	/* ------------------------------------------------------------------------- */

	if (1 == gTKmanual.Stop)   	/* enable stop step */
	{
		t_stop        = 1;
		gTKmanual.Stop = 0;
	}
	/* auto solution end */
	if(1 == t_stop)
	{
		//		memset(&LMold.ValveOut,0,sizeof(LMold.ValveOut));
		//		memset(&RMold.ValveOut,0,sizeof(RMold.ValveOut));
		//
		//		memset(&gMachineOut.LMold,0,sizeof(gMachineOut.LMold));
		//    	memset(&gMachineOut.RMold,0,sizeof(gMachineOut.RMold));

//		memset(&LMold.ValveOut.ClampCls,0,sizeof(LMold.ValveOut)-12);
//		memset(&RMold.ValveOut.ClampCls,0,sizeof(RMold.ValveOut)-12);

		LMold.BottomDeflash.Step = 30000;
		RMold.BottomDeflash.Step = 30000;
		
		LMold.HighBlow.Step = 30000;
		RMold.HighBlow.Step = 30000;
		
		LMold.LowBlow.Step = 30000;
		RMold.LowBlow.Step = 30000;

		LMold.BlowPin.Step = 30000;
		RMold.BlowPin.Step = 30000;
		
		LMold.BlowPin2.Step = 30000;
		RMold.BlowPin2.Step = 30000;

		LMold.Clamp.Step = 30000;
		RMold.Clamp.Step = 30000;

		LMold.Carriage.Step = 30000;
		RMold.Carriage.Step = 30000;
		
		LMold.TopDeflash.Step = 30000;
		RMold.TopDeflash.Step = 30000;
		
		LMold.Plate.Step = 30000;/*Albert*/
		RMold.Plate.Step = 30000;/*Albert*/

		LMold.Carriage.v_set = 0;
		RMold.Carriage.v_set = 0;

		LMold.Clamp.v_set = 0;
		RMold.Clamp.v_set = 0;
		
		LMold.Clamp.p_set = 0;/*Albert*/
		RMold.Clamp.p_set = 0;/*Albert*/

		LMold.BlowPin.v_set = 0;
		RMold.BlowPin.v_set = 0;
		
		LMold.TopDeflash.p_set = 0;/*Albert*/
		RMold.TopDeflash.p_set = 0;/*Albert*/
		
		LMold.Plate.p_set = 0;/*Albert*/
		RMold.Plate.p_set = 0;/*Albert*/

		LMold.BottomDeflash.p_set =0;
		RMold.BottomDeflash.p_set =0;
		
		LMold.SubMold.Step =30000;
		RMold.SubMold.Step =30000;

		LMold.BottomSubMold.Step =30000;
		RMold.BottomSubMold.Step =30000;

		LMold.SubMold.p_set =0;
		RMold.SubMold.p_set =0;

		LMold.BottomSubMold.p_set =0;
		RMold.BottomSubMold.p_set =0;
		
		LMold.RobotOpnCls.Step = 30000;
		RMold.RobotOpnCls.Step = 30000;
			
		LMold.Robot.Step = 30000;
		RMold.Robot.Step = 30000;

		t_init          = 0;    /* next resume ,enable init step */
		gTKmanual.Status = 0;
		ST_tmp_suspend(0);

	} /* if (t_stop==1) */
} /* end of _CYCLIC */

/* ------------------------------------------------------------------*/
void ManualCtrl( Mold_typ *pMold)
{
	if (1==gMacOption.Station) /*Right mold*/
	{
		/* -------------------  Carriage up   ----------------------*/
		/*Carriage forward 200-3000 and Carriage backward 10200-13000  && Button Off Delay  */
		if((pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200) || (pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200)|| pMold->Carriage.BtnOffDlyFlag)
		{
			/*Avoid press key many times*/
		}
		else
		{
			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_UP  ],PKeyMatric[S$KEYMATRIX_CARRIAGE_UP  ]))
			{
				if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
				{
					if( 1 == pMold->Clamp.OpnPos && ( 1 == pMold->BlowPin.UpPos && 1 == pMold->BlowPin2.UpPos ))
					{
						if( 1 == pMold->TransDIn.TopDeflashBwLimit 
							&& 1 == pMold->TransDIn.BottomDeflashBwLimit
							&& 1 == pMold->TransDIn.SubMoldClsLimit && 1 == pMold->TransDIn.BottomSubMoldClsLimit
							&& 1 == pMold->TransDIn.HookPinBwLimit
							&& 1 == pMold->TransDIn.ExtInMoldStickerBwLimit && pMold->TransDIn.ExtInMoldStickerAlarm)
						{	
							if(1 == pMold->OtherMoldBw)
							{
								if(1 == pMold->Carriage.CalOK)
								{
									pMold->Carriage.Step = 300;      	/* Carriage up  */
								}
								else
								{
									if(pMold == & LMold)
									{
										gImply.LCarriageCalErr = !pMold->Carriage.CalOK;
									}
									else
									{
										gImply.RCarriageCalErr = !pMold->Carriage.CalOK;
									}
								}
							}/* if(1 == pMold->OtherMoldBw) */
							else
							{
								if(pMold == & LMold)
								{
									gImply.RCarriageBw = !RMold.Carriage.BwPos;
								}
								else
								{
									gImply.LCarriageBw = !LMold.Carriage.BwPos;
								}
							}/* if(1 == pMold->OtherMoldBw) */
						}/* if(1 == pMold->TransDIn.TopDeflashBwLimit) */
						else
						{
							if(pMold == & LMold)
							{
								gImply.LTopDeflashBw 	= !pMold->TransDIn.TopDeflashBwLimit;
								gImply.LBottomDeflashBw = !pMold->TransDIn.BottomDeflashBwLimit;
								gImply.LSubMoldFw 		= !pMold->TransDIn.SubMoldClsLimit;
								gImply.LBottomSubMoldFw	= !pMold->TransDIn.BottomSubMoldClsLimit;
								gImply.LHookPinBw 		= !pMold->TransDIn.HookPinBwLimit;
								gImply.LExtInMoldStickerBw 		= !pMold->TransDIn.ExtInMoldStickerBwLimit;
								gImply.LExtInMoldStickerAlarm 	= !pMold->TransDIn.ExtInMoldStickerAlarm;
							}
							else
							{
								gImply.RTopDeflashBw 	= !pMold->TransDIn.TopDeflashBwLimit;
								gImply.RBottomDeflashBw = !pMold->TransDIn.BottomDeflashBwLimit;
								gImply.RSubMoldFw 		= !pMold->TransDIn.SubMoldClsLimit;
								gImply.RBottomSubMoldFw	= !pMold->TransDIn.BottomSubMoldClsLimit;
								gImply.RHookPinBw 		= !pMold->TransDIn.HookPinBwLimit;
								gImply.RExtInMoldStickerBw 		= !pMold->TransDIn.ExtInMoldStickerBwLimit;
								gImply.RExtInMoldStickerAlarm 	= !pMold->TransDIn.ExtInMoldStickerAlarm;
							}
						}
					}
					else
					{
						if(pMold == & LMold)
						{
							gImply.LClampOpn     = !pMold->Clamp.OpnPos;
							gImply.LPinUp    	 = !pMold->BlowPin.UpPos;
							gImply.LPin2Up    	 = !pMold->BlowPin2.UpPos;
						}
						else
						{
							gImply.RClampOpn     = !pMold->Clamp.OpnPos;
							gImply.RPinUp    	 = !pMold->BlowPin.UpPos;
							gImply.RPin2Up    	 = !pMold->BlowPin2.UpPos;
						}
					}
				}	
				else
				{
					gImply.SafeGate = !gMachineIn.SafeGate;
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
				}
			}

		}/* if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_UP  ],PKeyMatric[S$KEYMATRIX_CARRIAGE_UP  ])) */

		/*2020.9.17*/
		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_UP  ],NKeyMatric[S$KEYMATRIX_CARRIAGE_UP  ]))
		{
			pMold->Carriage.Step = 20000;
		}

		/* -------------------  Carriage down   ----------------------*/
		/*Carriage forward 200-3000 and Carriage backward 10200-13000  && Button Off Delay  */
		if((pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200) || (pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200)|| pMold->Carriage.BtnOffDlyFlag)
		{
			/*Avoid press key many times*/
		}
		else
		{
			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_DN  ],PKeyMatric[S$KEYMATRIX_CARRIAGE_DN  ]))
			{
				if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
				{
					if( 1 == pMold->BlowPin.UpPos && 1 == pMold->BlowPin2.UpPos )
					{
						if( 1 == pMold->TransDIn.TopDeflashBwLimit 
							&& 1 == pMold->TransDIn.BottomDeflashBwLimit
							&& 1 == pMold->TransDIn.CoolPinUpLimit 
							&& 1 == pMold->Robot.Transfer.BwPos
							&& 1 == pMold->TransDIn.ExtRobotUpLmit
							&& 1 == pMold->TransDIn.HookPinBwLimit
							&& 1 == pMold->TransDIn.ExtInMoldStickerBwLimit && pMold->TransDIn.ExtInMoldStickerAlarm) 
						{
							if(1 == pMold->Carriage.CalOK)
							{
								pMold->Carriage.Step = 10300;      	/* Carriage down  */
							}
							else
							{
								if(pMold == & LMold)
								{
									gImply.LCarriageCalErr = !pMold->Carriage.CalOK;
								}
								else
								{
									gImply.RCarriageCalErr = !pMold->Carriage.CalOK;
								}
							}
						}
						else
						{
							if(pMold == & LMold)
							{
								gImply.LTopDeflashBw = ! pMold->TransDIn.TopDeflashBwLimit;
								gImply.LBottomDeflashBw = ! pMold->TransDIn.BottomDeflashBwLimit;
								gImply.LCoolPinUp	= !pMold->TransDIn.CoolPinUpLimit; 
								gImply.LRobotBw		= !pMold->Robot.Transfer.BwPos; 
						
								gImply.LExtRobotUp 	= !pMold->TransDIn.ExtRobotUpLmit;
								gImply.LHookPinBw 		= !pMold->TransDIn.HookPinBwLimit;
								gImply.LExtInMoldStickerBw 		= !pMold->TransDIn.ExtInMoldStickerBwLimit;
								gImply.LExtInMoldStickerAlarm 	= !pMold->TransDIn.ExtInMoldStickerAlarm;
							}
							else
							{
								gImply.RTopDeflashBw = !pMold->TransDIn.TopDeflashBwLimit;
								gImply.RBottomDeflashBw = ! pMold->TransDIn.BottomDeflashBwLimit;
								gImply.RCoolPinUp	= !pMold->TransDIn.CoolPinUpLimit; 
								gImply.RRobotBw		= !pMold->Robot.Transfer.BwPos; 
					
								gImply.RExtRobotUp 	= !pMold->TransDIn.ExtRobotUpLmit;
								gImply.RHookPinBw 	= !pMold->TransDIn.HookPinBwLimit;
								gImply.RExtInMoldStickerBw 		= !pMold->TransDIn.ExtInMoldStickerBwLimit;
								gImply.RExtInMoldStickerAlarm 	= !pMold->TransDIn.ExtInMoldStickerAlarm;
							}
						}
					}
					else
					{
						if(pMold == & LMold)
						{
							gImply.LPinUp        = !pMold->BlowPin.UpPos;
							gImply.LPin2Up       = !pMold->BlowPin2.UpPos;
						}
						else
						{
							gImply.RPinUp        = !pMold->BlowPin.UpPos;
							gImply.RPin2Up       = !pMold->BlowPin2.UpPos;
						}
					}
				}	/* if(1 == gMachineIn.SafeGate) */
				else
				{
					gImply.SafeGate = !gMachineIn.SafeGate;
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
				}
			}/* if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_DN  ],PKeyMatric[S$KEYMATRIX_CARRIAGE_DN  ]))*/
		}

		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_DN  ],NKeyMatric[S$KEYMATRIX_CARRIAGE_DN  ]))
		{
			pMold->Carriage.Step = 20000;
		}
		
	}
	/*-----------------------------------------------------------------------------------------------------------------------*/
	if (0==gMacOption.Station) /*left mold*/
	{
		/* -------------------  Carriage up   ----------------------*/
		/*Carriage forward 200-3000 and Carriage backward 10200-13000  && Button Off Delay  */
		if((pMold->Carriage.Step < 3900 && pMold->Carriage.Step > 200) || (pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200)|| pMold->Carriage.BtnOffDlyFlag)
		{
			/*Avoid press key many times*/
		}
		else
		{
			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_DN  ],PKeyMatric[S$KEYMATRIX_CARRIAGE_DN  ]))
			{
				if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
				{
					if( 1 == pMold->Clamp.OpnPos && ( 1 == pMold->BlowPin.UpPos && 1 == pMold->BlowPin2.UpPos ))
					{
						if( 1 == pMold->TransDIn.TopDeflashBwLimit
							&& 1 == pMold->TransDIn.BottomDeflashBwLimit
							&& 1 == pMold->TransDIn.SubMoldClsLimit && 1 == pMold->TransDIn.BottomSubMoldClsLimit
							&& 1 == pMold->TransDIn.ExtInMoldStickerBwLimit && pMold->TransDIn.ExtInMoldStickerAlarm)	
						{	
							if(1 == pMold->OtherMoldBw)
							{
								if(1 == pMold->Carriage.CalOK)
								{
									pMold->Carriage.Step = 300;      	/* Carriage up  */
								}
								else
								{
									if(pMold == & LMold)
									{
										gImply.LCarriageCalErr = !pMold->Carriage.CalOK;
									}
									else
									{
										gImply.RCarriageCalErr = !pMold->Carriage.CalOK;
									}
								}
							}/* if(1 == pMold->OtherMoldBw) */
							else
							{
								if(pMold == & LMold)
								{
									gImply.RCarriageBw = !RMold.Carriage.BwPos;
								}
								else
								{
									gImply.LCarriageBw = !LMold.Carriage.BwPos;
								}
							}/* if(1 == pMold->OtherMoldBw) */
						}/* if(1 == pMold->TransDIn.TopDeflashBwLimit) */
						else
						{
							if(pMold == & LMold)
							{
								gImply.LTopDeflashBw 	= !pMold->TransDIn.TopDeflashBwLimit;
								gImply.LBottomDeflashBw = !pMold->TransDIn.BottomDeflashBwLimit;
								gImply.LSubMoldFw 		= !pMold->TransDIn.SubMoldClsLimit;
								gImply.LBottomSubMoldFw	= !pMold->TransDIn.BottomSubMoldClsLimit;
								gImply.LExtInMoldStickerBw 		= !pMold->TransDIn.ExtInMoldStickerBwLimit;
								gImply.LExtInMoldStickerAlarm 	= !pMold->TransDIn.ExtInMoldStickerAlarm;
							}
							else
							{
								gImply.RTopDeflashBw 	= !pMold->TransDIn.TopDeflashBwLimit;
								gImply.RBottomDeflashBw = !pMold->TransDIn.BottomDeflashBwLimit;
								gImply.RSubMoldFw 		= !pMold->TransDIn.SubMoldClsLimit;
								gImply.RBottomSubMoldFw	= !pMold->TransDIn.BottomSubMoldClsLimit;
								gImply.RExtInMoldStickerBw 		= !pMold->TransDIn.ExtInMoldStickerBwLimit;
								gImply.RExtInMoldStickerAlarm 	= !pMold->TransDIn.ExtInMoldStickerAlarm;
							}
						}
					}
					else
					{
						if(pMold == & LMold)
						{
							gImply.LClampOpn     = !pMold->Clamp.OpnPos;
							gImply.LPinUp        = !pMold->BlowPin.UpPos;
							gImply.LPin2Up       = !pMold->BlowPin2.UpPos;
						}
						else
						{
							gImply.RClampOpn     = !pMold->Clamp.OpnPos;
							gImply.RPinUp        = !pMold->BlowPin.UpPos;
							gImply.RPin2Up       = !pMold->BlowPin2.UpPos;
						}
					}
				}	/* if(1 == gMachineIn.SafeGate) */
				else
				{
					gImply.SafeGate = !gMachineIn.SafeGate;
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
				}

			}/* if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_DN  ],PKeyMatric[S$KEYMATRIX_CARRIAGE_DN  ])) */
		}
		/*2020.9.17*/
		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_DN  ],NKeyMatric[S$KEYMATRIX_CARRIAGE_DN  ]))
		{
			pMold->Carriage.Step = 20000;
		}

		/* -------------------  Carriage down   ----------------------*/
		/*Carriage forward 200-3000 and Carriage backward 10200-13000  && Button Off Delay  */
		if((pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200) || (pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200)|| pMold->Carriage.BtnOffDlyFlag)
		{
			/*Avoid press key many times*/
		}
		else
		{
			/*2020.9.17*/
			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_UP  ],PKeyMatric[S$KEYMATRIX_CARRIAGE_UP  ]))
				{
				if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
				{
					if( 1 == pMold->BlowPin.UpPos && 1 == pMold->BlowPin2.UpPos)
					{
						if( 1 == pMold->TransDIn.TopDeflashBwLimit 
							&& 1 == pMold->TransDIn.BottomDeflashBwLimit
							&& 1 == pMold->TransDIn.CoolPinUpLimit 
							&& 1 == pMold->Robot.Transfer.BwPos
							&& 1 == pMold->TransDIn.ExtRobotUpLmit
							&& 1 == pMold->TransDIn.ExtInMoldStickerBwLimit && pMold->TransDIn.ExtInMoldStickerAlarm) 
						{
							if(1 == pMold->Carriage.CalOK)
							{
								pMold->Carriage.Step = 10300;      	/* Carriage down  */
							}
							else
							{
								if(pMold == & LMold)
								{
									gImply.LCarriageCalErr = !pMold->Carriage.CalOK;
								}
								else
								{
									gImply.RCarriageCalErr = !pMold->Carriage.CalOK;
								}
							}
						}
						else
						{
							if(pMold == & LMold)
							{
								gImply.LTopDeflashBw = ! pMold->TransDIn.TopDeflashBwLimit;
								gImply.LBottomDeflashBw = ! pMold->TransDIn.BottomDeflashBwLimit;
								gImply.LCoolPinUp	= !pMold->TransDIn.CoolPinUpLimit; 
								gImply.LRobotBw		= !pMold->Robot.Transfer.BwPos; 
					
								gImply.LExtRobotUp 	= !pMold->TransDIn.ExtRobotUpLmit;
								gImply.LExtInMoldStickerBw 		= !pMold->TransDIn.ExtInMoldStickerBwLimit;
								gImply.LExtInMoldStickerAlarm 	= !pMold->TransDIn.ExtInMoldStickerAlarm;
							}
							else
							{
								gImply.RTopDeflashBw = !pMold->TransDIn.TopDeflashBwLimit;
								gImply.RBottomDeflashBw = ! pMold->TransDIn.BottomDeflashBwLimit;
								gImply.RCoolPinUp	= !pMold->TransDIn.CoolPinUpLimit; 
								gImply.RRobotBw		= !pMold->Robot.Transfer.BwPos; 
				
								gImply.RExtRobotUp 	= !pMold->TransDIn.ExtRobotUpLmit;
								gImply.RExtInMoldStickerBw 		= !pMold->TransDIn.ExtInMoldStickerBwLimit;
								gImply.RExtInMoldStickerAlarm 	= !pMold->TransDIn.ExtInMoldStickerAlarm;
							}
						}
					}
					else
					{
						if(pMold == & LMold)
						{
							gImply.LPinUp        = !pMold->BlowPin.UpPos;
							gImply.LPin2Up       = !pMold->BlowPin2.UpPos;
						}
						else
						{
							gImply.RPinUp        = !pMold->BlowPin.UpPos;
							gImply.RPin2Up       = !pMold->BlowPin2.UpPos;
						}
					}
				}	/* if(1 == gMachineIn.SafeGate) */
				else
				{
					gImply.SafeGate = !gMachineIn.SafeGate;
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
				}
			}/* if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_UP  ],PKeyMatric[S$KEYMATRIX_CARRIAGE_UP  ]))*/
		}
		/*2020.9.17*/
		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_CARRIAGE_UP  ],NKeyMatric[S$KEYMATRIX_CARRIAGE_UP  ]))
		{
			pMold->Carriage.Step = 20000;
		}
	}
	
	/* -------------------  open  clamp  ----------------------*/
	
	/*Clamp open 200-3900 and Clamp close 10200-13000  && Button Off Delay  */
	if((pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200) || (pMold->Clamp.Step < 3900 && pMold->Clamp.Step > 200)|| pMold->Clamp.BtnOffDlyFlag)
	{
		/*Avoid press key many times*/
	}
	else
	{
	
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CLAMP_OPN  ],PKeyMatric[S$KEYMATRIX_CLAMP_OPN  ]))
			{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				if(	1 == pMold->TransDIn.TopDeflashBwLimit 
					&& 1 == pMold->TransDIn.BottomDeflashBwLimit 
					&& 1 == pMold->TransDIn.ExtRobotUpLmit)
				{
					if(1 == pMold->Clamp.CalOK)
					{
						if( ACTUATOR_HYD  == gMachineFix.MoldR.Clamp.eActuatorType )  // Hydraulic
						{
							pMold->Clamp.Step = 12100;		/*   clamp release + open     */
						}
						else if(ACTUATOR_ELECTRIC  == gMachineFix.MoldR.Clamp.eActuatorType)	//Electric
						{
							pMold->Clamp.Step = 10300;		/*   clamp open    */
						}
					}
					else
					{
						if(pMold == & LMold)
						{
							gImply.LClampCalErr = !pMold->Clamp.CalOK;
						}
						else
						{
							gImply.RClampCalErr = !pMold->Clamp.CalOK;
						}
					}
				}
				else
				{
					if(pMold == & LMold)
					{
						gImply.LTopDeflashBw 	= !pMold->TransDIn.TopDeflashBwLimit;
						gImply.LBottomDeflashBw = !pMold->TransDIn.BottomDeflashBwLimit;
			
						gImply.LExtRobotUp 		= !pMold->TransDIn.ExtRobotUpLmit;
					}
					else
					{
						gImply.RTopDeflashBw 	= ! pMold->TransDIn.TopDeflashBwLimit;
						gImply.RBottomDeflashBw = ! pMold->TransDIn.BottomDeflashBwLimit;
			
						gImply.RExtRobotUp 		= !pMold->TransDIn.ExtRobotUpLmit;
					}
		
				}
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_CLAMP_OPN  ],NKeyMatric[S$KEYMATRIX_CLAMP_OPN  ]))
	{
		if(pMold == & LMold)
		{
			gLClampOpnFlag_Calib =0;
		}
		else
		{
			gRClampOpnFlag_Calib =0;
		}
		pMold->Clamp.Step = 20000;
	}

	/* -------------------  close  clamp  ----------------------*/
	
	/*Clamp open 200-3900 and Clamp close 10200-13000  && Button Off Delay  */
	if((pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200) || (pMold->Clamp.Step < 3900 && pMold->Clamp.Step > 200)|| pMold->Clamp.BtnOffDlyFlag)
	{
		/*Avoid press key many times*/
	}
	else
	{
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CLAMP_CLS  ],PKeyMatric[S$KEYMATRIX_CLAMP_CLS  ]))
		{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				if( 1 == pMold->BlowPin.UpPos && 1 == pMold->BlowPin2.UpPos)
				{
					if( 1 == pMold->TransDIn.CoolPinUpLimit 
						&& 1 == pMold->TransDIn.SubMoldClsLimit
						&& 1 == pMold->TransDIn.BottomSubMoldClsLimit
						&& 1 == pMold->Robot.Transfer.BwPos
						&& 1 == pMold->TransDIn.ExtInMoldStickerBwLimit && pMold->TransDIn.ExtInMoldStickerAlarm)	
					{
						if(1 == pMold->Clamp.CalOK)
						{
							pMold->Clamp.Step = 300;		/*   clamp close    */
						}
						else
						{
							if(pMold == & LMold)
							{
								gImply.LClampCalErr = !pMold->Clamp.CalOK;
							}
							else
							{
								gImply.RClampCalErr = !pMold->Clamp.CalOK;
							}
						}

					}
					else
					{
						if(pMold == & LMold)
						{
							gImply.LCoolPinUp = !pMold->TransDIn.CoolPinUpLimit;
		
							gImply.LSubMoldFw 	= !pMold->TransDIn.SubMoldClsLimit;
							gImply.LBottomSubMoldFw 	= !pMold->TransDIn.BottomSubMoldClsLimit;
							gImply.LRobotBw		= !pMold->Robot.Transfer.BwPos;
							gImply.LExtInMoldStickerBw 		= !pMold->TransDIn.ExtInMoldStickerBwLimit;
							gImply.LExtInMoldStickerAlarm 	= !pMold->TransDIn.ExtInMoldStickerAlarm;
						}
						else
						{
							gImply.RCoolPinUp = !pMold->TransDIn.CoolPinUpLimit;
			
							gImply.RSubMoldFw 	= !pMold->TransDIn.SubMoldClsLimit;
							gImply.RBottomSubMoldFw 	= !pMold->TransDIn.BottomSubMoldClsLimit;
							gImply.RRobotBw		= !pMold->Robot.Transfer.BwPos; 
							gImply.RExtInMoldStickerBw 		= !pMold->TransDIn.ExtInMoldStickerBwLimit;
							gImply.RExtInMoldStickerAlarm 	= !pMold->TransDIn.ExtInMoldStickerAlarm;
						}
					}
				}
				else
				{
					if(pMold == & LMold)
					{
						gImply.LPinUp        = !pMold->BlowPin.UpPos;
						gImply.LPin2Up       = !pMold->BlowPin2.UpPos;
					}
					else
					{
						gImply.RPinUp        = !pMold->BlowPin.UpPos;
						gImply.RPin2Up       = !pMold->BlowPin2.UpPos;
					}
				}/* if(1 == pMold->BlowPin.UpPos || 1 == pMold->Carriage.BwPos) */
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_CLAMP_CLS  ],NKeyMatric[S$KEYMATRIX_CLAMP_CLS  ]))
	{
		if(pMold == & LMold)
		{
			gLClampOpnFlag_Calib =0;
		}
		else
		{
			gRClampOpnFlag_Calib =0;
		}
		pMold->Clamp.Step = 20000;
	}

	/* -------------------   blow pin up  ----------------------*/
	/*BlowPin Down 200-3000 and BlowPin Up 10200-13000  && Button Off Delay  */
	if( ( (pMold->BlowPin.Step < 3000 && pMold->BlowPin.Step > 200) || (pMold->BlowPin.Step < 13000 && pMold->BlowPin.Step > 10200)|| pMold->BlowPin.BtnOffDlyFlag  )&&
		( (pMold->BlowPin2.Step < 3000 && pMold->BlowPin2.Step > 200) || (pMold->BlowPin2.Step < 13000 && pMold->BlowPin2.Step > 10200)|| pMold->BlowPin2.BtnOffDlyFlag  ))
	{
		/*Avoid press key many times*/
	}
	else
	{
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BLOWPIN_UP  ],PKeyMatric[S$KEYMATRIX_BLOWPIN_UP  ]))
		{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump) 
			{
				if(1 == pMold->BlowPin.CalOK && 1 == pMold->BlowPin2.CalOK)
				{
					if(pMold->Option.BlowPin)
					{
						pMold->BlowPin.Step = 10300;
					}
				
					if(pMold->Option.BlowPin2)
					{
						pMold->BlowPin2.Step = 10300;
					}
				}
				else
				{
					if(pMold == & LMold)
					{
						gImply.LBlowPinCalErr = !pMold->BlowPin.CalOK;
						gImply.LBlowPin2CalErr = !pMold->BlowPin2.CalOK;
					}
					else
					{
						gImply.RBlowPinCalErr = !pMold->BlowPin.CalOK;
						gImply.RBlowPin2CalErr = !pMold->BlowPin2.CalOK;
					}
				}	
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}/* if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BLOWPIN_UP  ],PKeyMatric[S$KEYMATRIX_BLOWPIN_UP  ])) */
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_BLOWPIN_UP  ],NKeyMatric[S$KEYMATRIX_BLOWPIN_UP  ]))
	{
		pMold->BlowPin.Step = 20000;
		pMold->BlowPin2.Step = 20000;
	}

	/* -------------------   blow pin down  ----------------------*/
	/*BlowPin Down 200-3000 and BlowPin Up 10200-13000  && Button Off Delay  */
	if( ( (pMold->BlowPin.Step < 3000 && pMold->BlowPin.Step > 200) || (pMold->BlowPin.Step < 13000 && pMold->BlowPin.Step > 10200)|| pMold->BlowPin.BtnOffDlyFlag  )&&
		( (pMold->BlowPin2.Step < 3000 && pMold->BlowPin2.Step > 200) || (pMold->BlowPin2.Step < 13000 && pMold->BlowPin2.Step > 10200)|| pMold->BlowPin2.BtnOffDlyFlag  ))
	{
		/*Avoid press key many times*/
	}
	else
	{
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BLOWPIN_DN  ],PKeyMatric[S$KEYMATRIX_BLOWPIN_DN  ]))
			{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				if(( 1 == pMold->Carriage.FwPos || 1 == pMold->Carriage.BwPos) )						
				{
					if(1 == pMold->TransDIn.SubMoldClsLimit 
						&& 1 == pMold->TransDIn.BottomSubMoldClsLimit )
					{
						if(1 == pMold->BlowPin.CalOK)
						{
							if(pMold->Option.BlowPin)
							{
								pMold->BlowPin.Step = 300;	/*  blow pin down  */
							}
			
							if(pMold->Option.BlowPin2)
							{
								pMold->BlowPin2.Step = 300;	/*  blow pin down  */
							}
						}
						else
						{
							if(pMold == & LMold)
							{
								gImply.LBlowPinCalErr = !pMold->BlowPin.CalOK;
								gImply.LBlowPin2CalErr = !pMold->BlowPin2.CalOK;
							}
							else
							{
								gImply.RBlowPinCalErr = !pMold->BlowPin.CalOK;
								gImply.RBlowPin2CalErr = !pMold->BlowPin2.CalOK;
							}
						}		
					}
					else
					{	
						if(pMold == & LMold)
						{
							gImply.LSubMoldFw 		= !pMold->TransDIn.SubMoldClsLimit;
							gImply.LBottomSubMoldFw = !pMold->TransDIn.BottomSubMoldClsLimit;
						}
						else
						{
							gImply.RSubMoldFw 		= !pMold->TransDIn.SubMoldClsLimit;
							gImply.RBottomSubMoldFw = !pMold->TransDIn.BottomSubMoldClsLimit;	
						}	
					}
				}
				else
				{	
					if(pMold == & LMold)
					{
						gImply.LCarriageFwBw = !(pMold->Carriage.FwPos || pMold->Carriage.BwPos);
					}
					else
					{		
						gImply.RCarriageFwBw = !(pMold->Carriage.FwPos || pMold->Carriage.BwPos);
					}
				}

			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}/* if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BLOWPIN_DN  ],PKeyMatric[S$KEYMATRIX_BLOWPIN_DN  ])) */
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_BLOWPIN_DN],NKeyMatric[S$KEYMATRIX_BLOWPIN_DN]))
	{
		pMold->BlowPin.Step = 20000;
	}

	/* ------------------------------  pin part forward ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PIN_PART_FW  ],PKeyMatric[S$KEYMATRIX_PIN_PART_FW  ]))
		{
		if(1 == gMachineIn.SafeGate)
		{
			if(pMold->Option.PinPart)
			{
				if(1 == pMold->BlowPin.UpPos)
				{
					pMold->PinPart.Step = 300;
				}
				else
				{
					if(pMold == & LMold)
					{
						gImply.LPinUp = !pMold->BlowPin.UpPos;
					}
					else
					{
						gImply.RPinUp = !pMold->BlowPin.UpPos;
					}
				}
			}
			else
			{
				gImply.NoFunction = 1;
			}
		}
		else
		{
			gImply.SafeGate = !gMachineIn.SafeGate;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PIN_PART_FW  ],NKeyMatric[S$KEYMATRIX_PIN_PART_FW  ]))
		{
		pMold->PinPart.Step = 20000;
	}

	/* ------------------------------  pin part backward ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PIN_PART_BW  ],PKeyMatric[S$KEYMATRIX_PIN_PART_BW  ]))
		{
		if(1 == gMachineIn.SafeGate)
		{
			if(pMold->Option.PinPart)
			{
				if(1 == pMold->BlowPin.UpPos)
				{
					pMold->PinPart.Step = 10300;
				}
				else
				{
					if(pMold == & LMold)
					{
						gImply.LPinUp = !pMold->BlowPin.UpPos;
					}
					else
					{
						gImply.RPinUp = !pMold->BlowPin.UpPos;
					}
				}
			}
			else
			{
				gImply.NoFunction = 1;
			}
		}
		else
		{
			gImply.SafeGate = !gMachineIn.SafeGate;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PIN_PART_BW  ],NKeyMatric[S$KEYMATRIX_PIN_PART_BW  ]))
		{
		pMold->PinPart.Step = 20000;
	}

	/* ------------------------------  Sub mold open ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_SUBMOLD_OPN  ],PKeyMatric[S$KEYMATRIX_SUBMOLD_OPN  ]))
	{
		if(1 == pMold->Option.SubMold )
		{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				if (1 == pMold->Clamp.OpnOverHalf && 1 == pMold->TransDIn.CarriageBwLimit ) // 大於微開模
				{
					pMold->SubMold.Step = 10300;
				}
				else
				{	
					if(pMold == & LMold)
					{
						gImply.L_SubmoldOut_Clamp = !pMold->Clamp.OpnOverHalf;
						gImply.LCarriageBw = !pMold->TransDIn.CarriageBwLimit;
					}
					else
					{
						gImply.R_SubmoldOut_Clamp = !pMold->Clamp.OpnOverHalf;
						gImply.RCarriageBw = !pMold->TransDIn.CarriageBwLimit;
					}
				}
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_SUBMOLD_OPN  ],NKeyMatric[S$KEYMATRIX_SUBMOLD_OPN  ]))
	{
		pMold->SubMold.Step = 20000;
	}

	/* ------------------------------  sub mold close ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_SUBMOLD_CLS  ],PKeyMatric[S$KEYMATRIX_SUBMOLD_CLS  ]))
	{
		if(1 == pMold->Option.SubMold )
		{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				pMold->SubMold.Step = 300;
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_SUBMOLD_CLS  ],NKeyMatric[S$KEYMATRIX_SUBMOLD_CLS  ]))
	{
		pMold->SubMold.Step = 20000;		
	}
	
	
	/* ------------------------------  Bottom Sub mold open ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BOTSUBMOLD_OPN  ],PKeyMatric[S$KEYMATRIX_BOTSUBMOLD_OPN  ]))
	{
		if(1 == pMold->Option.BottomSubMold )
		{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				if (1 == pMold->Clamp.OpnOverHalf && 1 == pMold->TransDIn.CarriageBwLimit ) // 大於微開模
				{
					pMold->BottomSubMold.Step = 10300;
				}
				else
				{	
					if(pMold == & LMold)
					{
						gImply.L_SubmoldOut_Clamp = !pMold->Clamp.OpnOverHalf;
						gImply.LCarriageBw = !pMold->TransDIn.CarriageBwLimit;
					}
					else
					{
						gImply.R_SubmoldOut_Clamp = !pMold->Clamp.OpnOverHalf;
						gImply.RCarriageBw = !pMold->TransDIn.CarriageBwLimit;
					}
				}
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_BOTSUBMOLD_OPN  ],NKeyMatric[S$KEYMATRIX_BOTSUBMOLD_OPN  ]))
	{
		pMold->BottomSubMold.Step = 20000;
	}

	/* ------------------------------ Bottom sub mold close ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BOTSUBMOLD_CLS  ],PKeyMatric[S$KEYMATRIX_BOTSUBMOLD_CLS  ]))
	{
		if(1 == pMold->Option.BottomSubMold )
		{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				pMold->BottomSubMold.Step = 300;
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_BOTSUBMOLD_CLS  ],NKeyMatric[S$KEYMATRIX_BOTSUBMOLD_CLS  ]))
	{
		pMold->BottomSubMold.Step = 20000;
	}

	/* ------------------------------  Screw forward ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_SCREW_FW  ],PKeyMatric[S$KEYMATRIX_SCREW_FW  ]))
	{
		if(1 == gMachineIn.SafeGate)
		{
			if(pMold->Option.Screw)
			{
				pMold->Screw.Step = 300;
			}
			else
			{
				gImply.NoFunction = 1;
			}
		}
		else
		{
			gImply.SafeGate = !gMachineIn.SafeGate;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_SCREW_FW  ],NKeyMatric[S$KEYMATRIX_SCREW_FW  ]))
	{
		pMold->Screw.Step = 20000;
	}

	/* ------------------------------  screw backward ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_SCREW_BW  ],PKeyMatric[S$KEYMATRIX_SCREW_BW  ]))
	{
		if(1 == gMachineIn.SafeGate)
		{
			if(pMold->Option.Screw)
			{
				pMold->Screw.Step = 10300;
			}
			else
			{
				gImply.NoFunction = 1;
			}
		}
		else
		{
			gImply.SafeGate = !gMachineIn.SafeGate;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_SCREW_BW   ],NKeyMatric[S$KEYMATRIX_SCREW_BW   ]))
	{
		pMold->Screw.Step = 20000;
	}
	/* ------------------------------  Top Deflasher forward ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_TOP_DEFLASH_FW   ],PKeyMatric[S$KEYMATRIX_TOP_DEFLASH_FW   ]))
	{
		if(pMold->Option.TopDeflash)
		{
			if( 1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				if(1 == pMold->Carriage.BwPos 
					&& 1 == pMold->Clamp.ClsPos
					&& 1 == pMold->TransDIn.TopDeflashOpnLimit
					&& 1 == pMold->TransDIn.CoolPinUpLimit)
				{
					if( ACTUATOR_HYD  == gMachineFix.MoldR.TopDeflash.eActuatorType )
					{
						pMold->TopDeflash.Step = 300; 
					}
					else
					{
						pMold->TopDeflash.Step = 5000;
					}
				}
				else
				{
					if(pMold == & LMold)
					{
						gImply.LCarriageBw	= !pMold->Carriage.BwPos;	
						gImply.LClampCls	= !pMold->Clamp.ClsPos;
						gImply.LTopDeflashOpn	= !pMold->TransDIn.TopDeflashOpnLimit;	
						gImply.LCoolPinUp	= !pMold->TransDIn.CoolPinUpLimit;
					}
					else
					{
						gImply.RCarriageBw	= !pMold->Carriage.BwPos;	
						gImply.RClampCls	= !pMold->Clamp.ClsPos;
						gImply.RTopDeflashOpn	= !pMold->TransDIn.TopDeflashOpnLimit;	
						gImply.RCoolPinUp	= !pMold->TransDIn.CoolPinUpLimit;	
					}
				}
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
		else
		{
			gImply.NoFunction = 1;
		}

	}

	if( ACTUATOR_HYD  == gMachineFix.MoldR.TopDeflash.eActuatorType )
	{
		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_TOP_DEFLASH_FW],NKeyMatric[S$KEYMATRIX_TOP_DEFLASH_FW]))
		{
			pMold->TopDeflash.Step = 20000;
		}
	}


	/* ------------------------------  Top Deflasher backward ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_TOP_DEFLASH_BW],PKeyMatric[S$KEYMATRIX_TOP_DEFLASH_BW]))
	{
		if(pMold->Option.TopDeflash)
		{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{	
				if(1 == pMold->TransDIn.TopDeflashOpnLimit)
				{
					if( ACTUATOR_HYD  == gMachineFix.MoldR.TopDeflash.eActuatorType )
					{
						pMold->TopDeflash.Step = 1500; 
					}
					else
					{
						pMold->TopDeflash.Step = 1500;
					}
				}
				else
				{
					if(pMold == & LMold)
					{
						gImply.LTopDeflashOpn	= !pMold->TransDIn.TopDeflashOpnLimit;	
					}
					else
					{
						gImply.RTopDeflashOpn	= !pMold->TransDIn.TopDeflashOpnLimit;	
					}
				
				}
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}

	if( ACTUATOR_HYD  == gMachineFix.MoldR.TopDeflash.eActuatorType )
	{
		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_TOP_DEFLASH_BW],NKeyMatric[S$KEYMATRIX_TOP_DEFLASH_BW]))
		{
			pMold->TopDeflash.Step = 20000;
		}
	}

	/* ------------------------------  Bottom Deflasher Forward ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BOT_DEFLASH_FW],PKeyMatric[S$KEYMATRIX_BOT_DEFLASH_FW]))
	{
		if(1 == gMachineIn.SafeGate)
		{
			if(pMold->Option.BottomDeflash)
			{
				if( 1 == pMold->Carriage.BwPos && 1 == pMold->Clamp.ClsPos )		
				{
					pMold->BottomDeflash.Step = 400;
				}
				else
				{
					if(pMold == & LMold)
					{
						gImply.LCarriageBw	= !pMold->Carriage.BwPos;			
						gImply.LClampCls	= !pMold->Clamp.ClsPos;
					
					}
					else
					{
						gImply.RCarriageBw	= !pMold->TransDIn.CarriageBwLimit;
						gImply.RClampCls	= !pMold->Clamp.ClsPos;
					}
				}
			}
			else
			{
				gImply.NoFunction = 1;
			}
		}
		else
		{
			gImply.SafeGate = !gMachineIn.SafeGate;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_BOT_DEFLASH_FW],NKeyMatric[S$KEYMATRIX_BOT_DEFLASH_FW]))
	{
		pMold->BottomDeflash.Step = 20000;
	}
		
	
	/* ------------------------------  Bottom Deflasher Backward ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BOT_DEFLASH_BW],PKeyMatric[S$KEYMATRIX_BOT_DEFLASH_BW]))
	{
		if(1 == gMachineIn.SafeGate)
		{
			if(pMold->Option.BottomDeflash)
			{

				if(0 == pMold->TransDIn.BottomDeflashBwLimit)
				{
					pMold->BottomDeflash.Step = 1600;
				}
				else
				{
					
				}
				
			}
			else
			{
				gImply.NoFunction = 1;
			}
		}
		else
		{
			gImply.SafeGate = !gMachineIn.SafeGate;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_BOT_DEFLASH_BW],NKeyMatric[S$KEYMATRIX_BOT_DEFLASH_BW]))
	{
		pMold->BottomDeflash.Step = 20000;
	}
	
	/*  ------------------------   cutter -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CUTTER_FW   ],PKeyMatric[S$KEYMATRIX_CUTTER_FW   ]))
		{
		//		gImply.DisableCutterCalibMode = 1;
		
		if (pMold->Option.Cutter)
		{
			if(1 == gMachineIn.SafeGate)
			{
				//			gThickIO.In.StartThick = 1;
				//			RMold.Cutter.Step = 100;
				////			gMachineOut.CutterFw = !gMachineOut.CutterFw;
				if (1 == gMachineOut.OilPump)
				{
					if(1 == gMachineFix.Option.bExtParisonCtrl)
					{
						gMachineInfo.ExtParisonCtrlStart = 1; /*   外部壁厚启动   */
					}
					else
					{
						gThickIO.In.StartThick = 1;      /*   壁厚启动   */
					}
					RMold.SupportAir.cntTime = gThickProf.TimSet.CycleTime;
					pMold->Cutter.Step = 300; //ipis20190131
					//	RMold.SupportAir.Step = 100;	 //ipis0304
				}	
				else
				{
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
				}
			}
			else
			{	
				gMachineOut.CutterFw = 0;
				gImply.SafeGate = 1;
			}
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}
	
	/* ------------------------------  Pull Bottom Forward ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PULL_BOTTOM_FW],PKeyMatric[S$KEYMATRIX_PULL_BOTTOM_FW]))
		{
		if(1 == gMachineIn.SafeGate)
		{
			if(pMold->Option.PullBottom)
			{
				if(1 == gMachineOut.OilPump)
				{
					if( 5000 == pMold->PullBottom.Step )
					{
						pMold->PullBottom.Step = 20000;
					}
					else
					{
						pMold->PullBottom.Step = 5000;
					}
				}
				else
				{
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
				}
			}
			else
			{
				gImply.NoFunction = 1;
			}
		}
		else
		{
			gImply.SafeGate = !gMachineIn.SafeGate;
		}
	}

	//	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PULL_BOTTOM_FW],NKeyMatric[S$KEYMATRIX_PULL_BOTTOM_FW]))
	//	{
	//		pMold->PullBottom.Step = 20000;
	//	}
	
	
	/* ------------------------------  Pull Bottom Backward ----------------------------- */
	//	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PULL_BOTTOM_BW],PKeyMatric[S$KEYMATRIX_PULL_BOTTOM_BW]))
	//	{
	//		if(pMold->Option.PullBottom)
	//		{
	//			if(1 == gMachineOut.OilPump)
	//			{
	//				pMold->PullBottom.Step = 15000;	
	//			}
	//			else
	//			{
	//				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
	//			}
	//		}
	//		else
	//		{
	//			gImply.NoFunction = 1;
	//		}
	//	}
	//	
	//	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PULL_BOTTOM_BW],NKeyMatric[S$KEYMATRIX_PULL_BOTTOM_BW]))
	//	{
	//		pMold->PullBottom.Step = 20000;
	//	}
	/* ------------------------------  cutter neck ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CUT_NECK],PKeyMatric[S$KEYMATRIX_CUT_NECK]))
		{
		if(1 == gMachineIn.SafeGate)
		{
			if(pMold->Option.CutNeck)
			{
				pMold->ValveOut.CutNeck = !pMold->ValveOut.CutNeck;
				pMold->ActInfo.CutNeck  = pMold->ValveOut.CutNeck;
			}
			else
			{
				gImply.NoFunction = 1;
			}
		}
		else
		{
			gImply.SafeGate = !gMachineIn.SafeGate;
		}
	}

	//	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BOT_DEFLASH_FW],PKeyMatric[S$KEYMATRIX_BOT_DEFLASH_FW]))
	//		{
	//		if(1 == gMachineIn.SafeGate)
	//		{
	//			if(pMold->Option.BottomDeflash)
	//			{
	//				if(0 == pMold->BottomDeflash.Step)
	//				{
	//					pMold->BottomDeflash.Step = 1000;
	//				}
	//				else
	//				{
	//					pMold->BottomDeflash.Step = 2000;
	//				}
	//			}
	//			else
	//			{
	//				gImply.NoFunction = 1;
	//			}
	//		}
	//		else
	//		{
	//			gImply.SafeGate = !gMachineIn.SafeGate;	
	//		}
	//	}	
	/* ------------------------------  Deflasher handle ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_HANDLE_PUNCHER   ],PKeyMatric[S$KEYMATRIX_HANDLE_PUNCHER   ]))
		{
		if(pMold->Option.PunchHandle)
		{
			if (1 == pMold->TransDIn.TopDeflashFwLimit)
			{
				pMold->ValveOut.TopDeflashCls = !pMold->ValveOut.TopDeflashCls;
			}
			else
			{
				if(pMold == & LMold)
				{
					gImply.LTopDeflashFw = 1;
				}
				else
				{
					gImply.RTopDeflashFw = 1;
				}
			}
					
		}
		else
		{
			gImply.NoFunction = 1;
			pMold->ValveOut.TopDeflashCls = 0;
		}
	}

	/* ------------------------------  Deflasher COOL ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_COOL_DEFLASH],PKeyMatric[S$KEYMATRIX_COOL_DEFLASH]))
		{
		if(0 == pMold->CoolDeflash.Step)
		{
			pMold->CoolDeflash.Step = 400;
		}
		else
		{
			pMold->CoolDeflash.Step = 20000;
		}
	}
	/*  ------------------------ Robot-Transfer ActuatorType -----------------------  */
	if( ACTUATOR_PNEU  == gMachineFix.MoldR.RobotTransfer.eActuatorType ) //Pneu
	{
		/*  ------------------------ Robot-Transfer Forward/Backward - Pneu -----------------------  */
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ROBOT_FW ],PKeyMatric[S$KEYMATRIX_ROBOT_FW ]))
			{
			if(pMold->Option.RobotTransfer)
			{
				if( 1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump )
				{
					if(0 == pMold->ValveOut.RobotFw)  // Fw 
					{
						if( /*1 == pMold->Carriage.BwPos*/
							/*&&*/ 1 == pMold->TransDIn.TopDeflashBwLimit 
							&& 1 == pMold->TransDIn.BottomDeflashBwLimit
							&& 1 == pMold->TransDIn.RobotOpnLimit)
						{
							pMold->Robot.Step = 5000;
						}
						else
						{
							if(pMold == & LMold)
							{	
								//								gImply.LCarriageBw		= !pMold->Carriage.BwPos;
								gImply.LTopDeflashBw 	= !pMold->TransDIn.TopDeflashBwLimit;
								gImply.LBottomDeflashBw = !pMold->TransDIn.BottomDeflashBwLimit;
								gImply.LRobotOpn 		= !pMold->TransDIn.RobotOpnLimit;
							
							}
							else
							{		
								//								gImply.RCarriageBw		= !pMold->Carriage.BwPos;
								gImply.RTopDeflashBw 	= !pMold->TransDIn.TopDeflashBwLimit;
								gImply.RBottomDeflashBw = !pMold->TransDIn.BottomDeflashBwLimit;
								gImply.RRobotOpn 		= !pMold->TransDIn.RobotOpnLimit;
							
							}
						}
					}
					else //Bw
					{
						if( 1 == pMold->Clamp.OpnPos
							&& 1 == pMold->TransDIn.CoolPin2UpLimit && 1==pMold->TransDIn.CoolPin3UpLimit
							&& 1 == pMold->TransDIn.RobotFwLimit)
						{
							pMold->Robot.Step = 15000;
						}
						else
						{
							if(pMold == & LMold)
							{
								gImply.LClampOpn 	= !pMold->Clamp.OpnPos;
								gImply.LCoolPin2Up	= !pMold->TransDIn.CoolPin2UpLimit; 
								gImply.LCoolPin3Up	= !pMold->TransDIn.CoolPin3UpLimit; 
							
							}
							else
							{
								gImply.RClampOpn 	= !pMold->Clamp.OpnPos;
								gImply.RCoolPin2Up	= !pMold->TransDIn.CoolPin2UpLimit; 
								gImply.RCoolPin3Up	= !pMold->TransDIn.CoolPin3UpLimit; 
							
							}
						}
					}
				}
				else
				{
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
					gImply.SafeGate = !gMachineIn.SafeGate;
				}
			}
			else
			{
				gImply.NoFunction = 1;
			}
		
		}
	}
	else  //Elec/Hyd
	{
		/*  ------------------------ Robot-Transfer Forward - Elec/Hyd -----------------------  */
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ROBOT_FW ],PKeyMatric[S$KEYMATRIX_ROBOT_FW ]))
			{
			if(pMold->Option.RobotTransfer)
			{
				if( 1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump )
				{
					//					if(1 == pMold->Carriage.BwPos)
					//					{
					if( 1 == pMold->TransDIn.TopDeflashBwLimit 
						&& 1 == pMold->TransDIn.BottomDeflashBwLimit
						&& 1 == pMold->TransDIn.RobotOpnLimit)
					{
						pMold->Robot.Step = 300;
					}
					else
					{
						if(pMold == & LMold)
						{	
							gImply.LTopDeflashBw 	= !pMold->TransDIn.TopDeflashBwLimit;
							gImply.LBottomDeflashBw = !pMold->TransDIn.BottomDeflashBwLimit;
							gImply.LRobotOpn 		= !pMold->TransDIn.RobotOpnLimit;
							
						}
						else
						{		
							gImply.RTopDeflashBw 	= !pMold->TransDIn.TopDeflashBwLimit;
							gImply.RBottomDeflashBw = !pMold->TransDIn.BottomDeflashBwLimit;
							gImply.RRobotOpn 		= !pMold->TransDIn.RobotOpnLimit;
								
						}
					}
					//					}
					//					else
					//					{
					//						if(pMold == & LMold)
					//						{
					//							gImply.LCarriageBw		= !pMold->Carriage.BwPos;			
					//						}
					//						else
					//						{
					//							gImply.RCarriageBw		= !pMold->Carriage.BwPos;			
					//						}
					//					}
				}
				else
				{
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
					gImply.SafeGate = !gMachineIn.SafeGate;
				}
			}
			else
			{
				gImply.NoFunction = 1;
			}
		
		}


		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_ROBOT_FW],NKeyMatric[S$KEYMATRIX_ROBOT_FW]))
			{
			pMold->Robot.Step = 20000;
		}
		
		/*  ------------------------ Robot-Transfer Backward - Elec/Hyd -----------------------  */
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ROBOT_BW],PKeyMatric[S$KEYMATRIX_ROBOT_BW]))
			{
			if(pMold->Option.RobotTransfer)
			{
				if( 1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
				{
					if(1 == pMold->Clamp.OpnPos)
					{
						if( 1 == pMold->TransDIn.CoolPin2UpLimit && 1==pMold->TransDIn.CoolPin3UpLimit
							&& 1 == pMold->TransDIn.RobotBwLimit)
						{
							pMold->Robot.Step = 10300;
						}
						else
						{
							if(pMold == & LMold)
							{
					
								gImply.LCoolPin2Up	= !pMold->TransDIn.CoolPin2UpLimit; 
								gImply.LCoolPin3Up	= !pMold->TransDIn.CoolPin3UpLimit; 
							
							}
							else
							{
		
								gImply.RCoolPin2Up	= !pMold->TransDIn.CoolPin2UpLimit; 
								gImply.RCoolPin3Up	= !pMold->TransDIn.CoolPin3UpLimit; 
							
							}
						}
					}
					else
					{
						if(pMold == & LMold)
						{
							gImply.LClampOpn = !pMold->Clamp.OpnPos;
						}
						else
						{
							gImply.RClampOpn = !pMold->Clamp.OpnPos;
						}
				
					}
				}
				else
				{
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
					gImply.SafeGate = !gMachineIn.SafeGate;
				}
			}
			else
			{
				gImply.NoFunction = 1;
			}
		}

		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_ROBOT_BW],NKeyMatric[S$KEYMATRIX_ROBOT_BW]))
			{
			pMold->Robot.Step = 20000;
		}
	}
	

	
	/*  ------------------------ robot transfer FW BW -----------------------  */
	//	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ROBOT_FWBW],PKeyMatric[S$KEYMATRIX_ROBOT_FWBW]))
	//		{
	//		if(pMold->Option.RobotTransfer)
	//		{
	//			
	//			if(1 == pMold->TransDIn.RobotFwLimit)
	//			//	if(1==pMold->ValveOut.RobotFw)	
	//			{
	//				if (1==pMold->TransDIn.ClampOpnLimit && 1==pMold->TransDIn.CoolPinUpLimit && 1==pMold->TransDIn.CoolPin2UpLimit 
	//					&& 1==pMold->TransDIn.CoolPin3UpLimit )
	//				{
	//					pMold->ActInfo.RobotFw   = 0;
	//					pMold->ValveOut.RobotFw  = 0;
	//				}
	//				else
	//				{
	//						
	//					if(pMold == & LMold)
	//					{
	//						gImply.LClampOpn	= !pMold->TransDIn.ClampOpnLimit; 
	//						gImply.LCoolPinUp	= !pMold->TransDIn.CoolPinUpLimit; 
	//						gImply.LCoolPin2Up	= !pMold->TransDIn.CoolPin2UpLimit; 
	//						gImply.LCoolPin3Up	= !pMold->TransDIn.CoolPin3UpLimit; 
	//					}
	//					else
	//					{
	//						gImply.RClampOpn	= !pMold->TransDIn.ClampOpnLimit; 
	//						gImply.RCoolPinUp	= !pMold->TransDIn.CoolPinUpLimit; 
	//						gImply.RCoolPin2Up	= !pMold->TransDIn.CoolPin2UpLimit; 
	//						gImply.RCoolPin3Up	= !pMold->TransDIn.CoolPin3UpLimit; 
	//					}
	//				}
	//				
	//			}
	//			if(1 == pMold->TransDIn.RobotBwLimit)
	//			//	if(0==pMold->ValveOut.RobotFw)	
	//			{
	//				if (1 == pMold->TransDIn.CarriageBwLimit && 1==pMold->TransDIn.BottomDeflashBwLimit 
	//					&& 1==pMold->TransDIn.TopDeflashBwLimit && 1 == pMold->TransDIn.RobotOpnLimitL && 1 == pMold->TransDIn.RobotOpnLimitR  )
	//				{
	//					pMold->ActInfo.RobotFw   = 1;
	//					pMold->ValveOut.RobotFw  = 1;
	//				}
	//				else
	//				{
	//					if(pMold == & LMold)
	//					{
	//						gImply.LCarriageBw = !pMold->TransDIn.CarriageBwLimit ;
	//						gImply.LBottomDeflashBw    = !pMold->TransDIn.BottomDeflashBwLimit;
	//						gImply.LTopDeflashBw = !pMold->TransDIn.TopDeflashBwLimit;
	//						gImply.LRobotOpn_L = !pMold->TransDIn.RobotOpnLimitL;
	//						gImply.LRobotOpn_R = !pMold->TransDIn.RobotOpnLimitR;
	//					
	//					}
	//					else
	//					{
	//						gImply.RCarriageBw = !pMold->TransDIn.CarriageBwLimit ;
	//						gImply.RBottomDeflashBw    = !pMold->TransDIn.BottomDeflashBwLimit;
	//						gImply.RTopDeflashBw = !pMold->TransDIn.TopDeflashBwLimit;
	//						gImply.RRobotOpn_L = !pMold->TransDIn.RobotOpnLimitL;
	//						gImply.RRobotOpn_R = !pMold->TransDIn.RobotOpnLimitR;
	//					}
	//					
	//				}
	//				
	//			}	
	//			if (0 == pMold->TransDIn.RobotFwLimit && 0 == pMold->TransDIn.RobotBwLimit )
	//			{
	//				pMold->ActInfo.RobotFw   = 0;
	//				pMold->ValveOut.RobotFw  = 0;
	//			}
	//			
	//			
	//			
	//		}
	//		else
	//		{
	//			gImply.NoFunction = 1;
	//		}
	//	}
	
	
	//	/*  ------------------------ robot down -----------------------  */
	//	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ROBOT_DN  ],PKeyMatric[S$KEYMATRIX_ROBOT_DN  ]))
	//		{
	//		if(pMold->Option.RobotUpDn)
	//		{
	//			if(1 == pMold->Robot.Transfer.BwPos)
	//			{
	//				pMold->ValveOut.RobotDn = !pMold->ValveOut.RobotDn;	
	//			}
	//			else if(1 == pMold->Robot.Transfer.FwPos)
	//			{
	//				if(pMold->TransDIn.TopDeflashBwLimit ) 
	//				{
	//					if(pMold->Clamp.OpnPos || pMold->Carriage.FwPos || pMold->Carriage.BwPos)
	//					{
	//						pMold->ValveOut.RobotDn = !pMold->ValveOut.RobotDn;
	//					}
	//					else
	//					{
	//						if(pMold == & LMold)
	//						{
	//							gImply.LClampOpn   = !pMold->Clamp.OpnPos;
	//							gImply.LCarriageBw = !pMold->Carriage.BwPos;
	//							gImply.LCarriageFw = !pMold->Carriage.FwPos;
	//
	//						}
	//						else
	//						{
	//							gImply.RClampOpn   = !pMold->Clamp.OpnPos;
	//							gImply.RCarriageBw = !pMold->Carriage.BwPos;
	//							gImply.RCarriageFw = !pMold->Carriage.FwPos;
	//						}
	//					}
	//				}
	//				else
	//				{
	//					if(pMold == & LMold)
	//					{
	//						gImply.LTopDeflashBw = 1;
	//					}
	//					else
	//					{
	//						gImply.RTopDeflashBw = 1;
	//					}
	//				}
	//			}
	//			else
	//			{
	//				gImply.RRobotFwOrBw = 1;
	//			}
	//		}
	//		else
	//		{
	//			gImply.NoFunction = 1;
	//		}
	//	}
	
	
	/*  ------------------------ Robot Open/Close  -----------------------  */
	if( ACTUATOR_PNEU  == gMachineFix.MoldR.RobotOpnCls.eActuatorType ) //Pneu
	{
		/*  ------------------------ Robot Open/Close - Pneu -----------------------  */
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ROBOT_CLS],PKeyMatric[S$KEYMATRIX_ROBOT_CLS]))
			{
			if(pMold->Option.RobotOpnCls)
			{
				if( 1 == gMachineOut.OilPump )
				{
					if(0 == pMold->ValveOut.RobotCls)  //Opn  
					{
						if(  1 == pMold->TransDIn.RobotFwLimit || 1 == pMold->TransDIn.RobotBwLimit)
						{
							pMold->RobotOpnCls.Step = 5000;
						}
						else
						{
							if(pMold == & LMold)
							{	
								gImply.LRobotFw = !pMold->TransDIn.RobotFwLimit;
								gImply.LRobotBw = !pMold->TransDIn.RobotBwLimit;
							}
							else
							{		
								gImply.RRobotFw = !pMold->TransDIn.RobotFwLimit;
								gImply.RRobotBw = !pMold->TransDIn.RobotBwLimit;
							}
						}
					}
					else //Cls
					{
						if(  1 == pMold->TransDIn.RobotFwLimit || 1 == pMold->TransDIn.RobotBwLimit)
						{
							pMold->RobotOpnCls.Step = 15000;
						}
						else
						{
							if(pMold == & LMold)
							{	
								gImply.LRobotFw = !pMold->TransDIn.RobotFwLimit;
								gImply.LRobotBw = !pMold->TransDIn.RobotBwLimit;
							}
							else
							{		
								gImply.RRobotFw = !pMold->TransDIn.RobotFwLimit;
								gImply.RRobotBw = !pMold->TransDIn.RobotBwLimit;
							}
						}
					}
				}
				else
				{
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
				}
			}
			else
			{
				gImply.NoFunction = 1;
			}
		
		}
	}
	
	/*  ------------------------ Product Aux close -----------------------  */
	//	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PRODUCTAUX_CLS  ],PKeyMatric[S$KEYMATRIX_PRODUCTAUX_CLS  ]))
	//		{
	//
	//		
	//		if (1==gUserPara.bProductAuxClamp)
	//		{
	//			pMold->ValveOut.ProductAuxClamp = !pMold->ValveOut.ProductAuxClamp;
	//			//	pMold->ActInfo.RobotCls  = pMold->ValveOut.ProductAuxClamp;
	//		}
	//		else
	//		{
	//			gImply.NoFunction = 1;		
	//		}
	//				
	//	}	
	//	
	
	/*  ------------------------   cool pin down -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_COOL_DN   ],PKeyMatric[S$KEYMATRIX_COOL_DN   ]))
	{
		if(pMold->Option.CoolPin)
		{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				if(1 == pMold->ValveOut.CoolPinDn) // Dn
				{
					pMold->CoolPin.Step = 15000;
				}
				else
				{
					pMold->CoolPin.Step = 5000;
				}
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
			
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}

	/*  ------------------------   cool pin blow -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_COOL_BLOW   ],PKeyMatric[S$KEYMATRIX_COOL_BLOW   ]))
	{
		if(pMold->Option.CoolPin)
		{
			if (1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				pMold->CoolPinBlow.Step = 5000;
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
		else
		{
			gImply.NoFunction = 1;
			pMold->ValveOut.CoolPinBlow = 0;
		}
	}
	
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_COOL_BLOW  ],NKeyMatric[S$KEYMATRIX_COOL_BLOW  ]))
	{
		pMold->CoolPinBlow.Step = 20000;
	}

	/*  ------------------------   cool pin 2 down -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_COOL2_DN   ],PKeyMatric[S$KEYMATRIX_COOL2_DN   ]))
	{
		if(pMold->Option.CoolPin2)
		{
			if (1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				if(1 == pMold->ValveOut.CoolPin2Dn) // Dn
				{
					pMold->CoolPin2.Step = 15000;
				}
				else
				{
					pMold->CoolPin2.Step = 5000;
				}	
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
			
		}
		else
		{
			gImply.NoFunction = 1;
			pMold->ValveOut.CoolPin2Dn = 0;
		}
	}
	/*  ------------------------   cool pin 2 Blow -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_COOL2_BLOW   ],PKeyMatric[S$KEYMATRIX_COOL2_BLOW   ]))
	{
		if(pMold->Option.CoolPin2)
		{
			if (1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				pMold->CoolPin2Blow.Step = 5000;
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
		else
		{
			gImply.NoFunction = 1;
			pMold->ValveOut.CoolPin2Blow = 0;
		}
	}
	
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_COOL2_BLOW  ],NKeyMatric[S$KEYMATRIX_COOL2_BLOW  ]))
	{
		pMold->CoolPin2Blow.Step = 20000;
	}
	
	
	
	/*  ------------------------   cool pin 3 down -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_COOL3_DN   ],PKeyMatric[S$KEYMATRIX_COOL3_DN   ]))
		{
		if(pMold->Option.CoolPin3)
		{
			if(1 == pMold->TransDIn.RobotBwLimit)
			{
				if(1 == pMold->ValveOut.CoolPin3Dn) // Dn
				{
					pMold->CoolPin3.Step = 15000;
				}
				else
				{
					pMold->CoolPin3.Step = 5000;
				}
			}
			else
			{
				if(pMold == & LMold)
				{
					gImply.RRobotBw = 1;
				}
				else
				{
					gImply.RRobotBw = 1;
				}
			}
			pMold->ValveOut.CoolPin3Dn = !pMold->ValveOut.CoolPin3Dn;
		}
		else
		{
			gImply.NoFunction = 1;
			pMold->ValveOut.CoolPin3Dn = 0;
		}
	}

	
	/*  ------------------------   cool pin 3 Blow -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_COOL3_BLOW   ],PKeyMatric[S$KEYMATRIX_COOL3_BLOW   ]))
	{
		if(pMold->Option.CoolPin3)
		{
			pMold->CoolPin3Blow.Step = 5000;
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}
	
		
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_COOL3_BLOW  ],NKeyMatric[S$KEYMATRIX_COOL3_BLOW  ]))
	{
		pMold->CoolPin3Blow.Step = 20000;
	}
	
	/*  ------------------------   Hook pin Fw -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_HOOK_PIN_FW   ],PKeyMatric[S$KEYMATRIX_HOOK_PIN_FW   ]))
	{
		if(pMold->Option.HookPin)
		{
			if(1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
			{
				if(1 == pMold->ValveOut.HookPinFw) // Fw
				{
					pMold->HookPin.Step = 15000;	
				}
				else
				{
					if(1 == pMold->Carriage.BwPos)
					{
						pMold->HookPin.Step = 5000;
					}
					else
					{
						if(pMold == & LMold)
						{
							gImply.LCarriageBw = !pMold->Carriage.BwPos;
						}
						else
						{
							gImply.RCarriageBw = !pMold->Carriage.BwPos;
						}
					}
				}
			}
			else
			{
				gImply.SafeGate = !gMachineIn.SafeGate;
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
			
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}
	
	
	/* ------------------------------ High blow air  -----------------------------*/ 
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BLOW],PKeyMatric[S$KEYMATRIX_BLOW]))
	{
		if (1 == gMachineIn.SafeGate && 1 == gMachineOut.OilPump)
		{
			pMold->HighBlow.Step = 5000;
		}
		else
		{
			gImply.SafeGate = !gMachineIn.SafeGate;
			gImply.OilPumpNotOn  = !gMachineOut.OilPump;
		}
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_BLOW],NKeyMatric[S$KEYMATRIX_BLOW]))
	{
		pMold->HighBlow.Step = 20000;
	}

	/* ------------------------------ Low blow air  ----------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BLOW_LOW   ],PKeyMatric[S$KEYMATRIX_BLOW_LOW   ]))
		{
		if(pMold->LowBlow.Step != 5000)
		{
			pMold->LowBlow.Step = 5000;
		}
		else
		{
			pMold->LowBlow.Step = 20000;
		}
	}

	/*if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_BLOW_LOW   ],NKeyMatric[S$KEYMATRIX_BLOW_LOW   ]))
	{
		pMold->LowBlow.Step = 20000;
	}*/
	
	/*  ------------------------   Extruder up -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_UP   ],PKeyMatric[S$KEYMATRIX_EXTRUDER_UP   ]))
	{
		if(pMold->Option.ExtrLift)
		{
			if(1 == gMachineIn.SafeGate)
			{
				if (1 == gMachineOut.OilPump)
				{
					if( ACTUATOR_HYD  == gMachineFix.ExtrLift.eActuatorType )  // Hydraulic
					{
						if( 400 == pMold->ExtrLift.Step || 450 == pMold->ExtrLift.Step)// up -> stop
						{
							pMold->ExtrLift.Step = 0;/*Extruder stop*/
						}
						else
						{
							if((0 == pMold->TransDIn.ExtruderLiftUpLimit && 0 == pMold->TransDIn.ExtruderLiftDnLimit)
								|| 1 == pMold->TransDIn.ExtruderLiftUpLimit)
							{
								if(pMold->ExtrLift.Step > 0 && pMold->ExtrLift.Step < 3000)
								{
									/*Extruder up*/
								}
								else
								{
									pMold->ExtrLift.Step = 10300;/*Extruder down*/ 
								}
							}
							else 
							{			
								if(pMold->ExtrLift.Step > 10000 && pMold->ExtrLift.Step < 13000)
								{
									/*Extruder down*/
								}
								else
								{
									pMold->ExtrLift.Step = 300;/*Extruder up*/
								}
							}
						}
					}
					else if(ACTUATOR_ELECTRIC  == gMachineFix.ExtrLift.eActuatorType)	//Electric
					{
						RMold.ExtrLift.Step = 300;/*Extruder up*/
					}
				}
				else
				{
					gImply.OilPumpNotOn   = !gMachineOut.OilPump;
				}	
			}
			else
			{
				gImply.LowerSafeFeed = 1;
			}
		}
		else
		{
			gImply.NoFunction = 1;	
		}
	}
	
	if( ACTUATOR_ELECTRIC  == gMachineFix.ExtrLift.eActuatorType )
	{
		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_UP],NKeyMatric[S$KEYMATRIX_EXTRUDER_UP]))
		{
			RMold.ExtrLift.Step = 20000;
		}
	}

	/*  ------------------------   Extruder down (Electric) -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_DN],PKeyMatric[S$KEYMATRIX_EXTRUDER_DN]))
		{
		if(pMold->Option.ExtrLift)
		{
			if (1 == gMachineOut.OilPump)
			{	
				if(ACTUATOR_ELECTRIC  == gMachineFix.ExtrLift.eActuatorType)
				{
					RMold.ExtrLift.Step = 10300;/*Extruder down*/
				}
			}
			else
			{
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}
	
	if( ACTUATOR_ELECTRIC  == gMachineFix.ExtrLift.eActuatorType )
	{
		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_DN],NKeyMatric[S$KEYMATRIX_EXTRUDER_DN]))
		{
			RMold.ExtrLift.Step = 20000;
		}
	}
	
	/* ------------------------- support air -------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_SUPPORT  ],PKeyMatric[S$KEYMATRIX_SUPPORT  ]))
	{
		gMachineOut.SupportAir_1 = !gMachineOut.SupportAir_1;
		gMachineOut.SupportAir_2 = !gMachineOut.SupportAir_1;
		
		gMachineOut.SupportAir_TimeVersion = !gMachineOut.SupportAir_TimeVersion;
	}

	/*if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_SUPPORT  ],NKeyMatric[S$KEYMATRIX_SUPPORT  ]))
	{
		gMachineOut.SupportAir_1 = 0;
		gMachineOut.SupportAir_2 = 0;
	
		gMachineOut.SupportAir_TimeVersion = 0;
	}*/

	/* ------------------------- suck air -------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_VACCUM  ],PKeyMatric[S$KEYMATRIX_VACCUM  ]))
		{ 
		gMachineOut.SuckAir_1 = 1;
	}

	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_VACCUM  ],NKeyMatric[S$KEYMATRIX_VACCUM  ]))
		{
		gMachineOut.SuckAir_1 = 0;
	}
	
	/*  ------------------------ sensor zero -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_SENSOR_UP  ],PKeyMatric[S$KEYMATRIX_SENSOR_UP  ]))
		{
		pMold->Sensor.Step = 10300;
	}
	
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_SENSOR_UP  ],NKeyMatric[S$KEYMATRIX_SENSOR_UP  ]))
		{
		pMold->Sensor.Step = 20000;
	}
	
	/*  ------------------------ sensor bottle -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_SENSOR_BOTTLE  ],PKeyMatric[S$KEYMATRIX_SENSOR_BOTTLE  ]))
		{
		pMold->Sensor.Step = 300;
	}
	
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_SENSOR_BOTTLE  ],NKeyMatric[S$KEYMATRIX_SENSOR_BOTTLE  ]))
		{
		pMold->Sensor.Step = 20000;
	}

	/*  ------------------------ sensor waste -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_SENSOR_DN  ],PKeyMatric[S$KEYMATRIX_SENSOR_DN  ]))
		{
		pMold->Sensor.Step = 1300;
	}
	
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_SENSOR_DN  ],NKeyMatric[S$KEYMATRIX_SENSOR_DN  ]))
		{
		pMold->Sensor.Step = 20000;
	}

	/*------------------------Plate Up---------------------------------*/
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_UP  ],PKeyMatric[S$KEYMATRIX_PLATE_UP]))
	{
		if (1 == gMachineOut.OilPump)
		{
			if( 0 == gMachineIn.PlateUpLimit)
			{
				pMold->Plate.Step = 5000;
				gPanelObj.LedMatric[S$KEYMATRIX_PLATE_UP] = 1;
			}
			else
			{
				gImply.PlateUpLimit = gMachineIn.PlateUpLimit;
			}
		}
		else
		{
			gImply.OilPumpNotOn  = !gMachineOut.OilPump;
		}
	}
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_UP  ],NKeyMatric[S$KEYMATRIX_PLATE_UP  ]))
	{
		pMold->Plate.Step = 20000;
		gPanelObj.LedMatric[S$KEYMATRIX_PLATE_UP] = 0;
	}
	
	/*------------------------Plate Down---------------------------------*/
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_DN  ],PKeyMatric[S$KEYMATRIX_PLATE_DN  ]))
	{
		if (1 == gMachineOut.OilPump)
		{
			pMold->Plate.Step = 15000;
			gPanelObj.LedMatric[S$KEYMATRIX_PLATE_DN] = 1;
		}
		else
		{
			gImply.OilPumpNotOn  = !gMachineOut.OilPump;
		}
	}
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_DN  ],NKeyMatric[S$KEYMATRIX_PLATE_DN  ]))
	{
		pMold->Plate.Step = 20000;
		gPanelObj.LedMatric[S$KEYMATRIX_PLATE_DN] = 1;
	}
	/*------------------------Plate Fw---------------------------------*/
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_FW  ],PKeyMatric[S$KEYMATRIX_PLATE_FW]))
	{
		if(1 == gMachineOut.OilPump)
		{
			pMold->Plate.Step = 6100;
			gPanelObj.LedMatric[S$KEYMATRIX_PLATE_FW] = 1;
		}
		else
		{
			gImply.OilPumpNotOn  = !gMachineOut.OilPump;
		}	
	}
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_FW  ],NKeyMatric[S$KEYMATRIX_PLATE_FW  ]))
	{
		pMold->Plate.Step = 20000;
		gPanelObj.LedMatric[S$KEYMATRIX_PLATE_FW] = 0;
	}
	/*------------------------Plate Bw---------------------------------*/
	
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_BW  ],PKeyMatric[S$KEYMATRIX_PLATE_BW]))
	{
		if(1 == gMachineOut.OilPump)
		{
			pMold->Plate.Step = 16100;
			gPanelObj.LedMatric[S$KEYMATRIX_PLATE_BW] = 1;
		}
		else
		{
			gImply.OilPumpNotOn  = !gMachineOut.OilPump;
		}	
	}
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_BW  ],NKeyMatric[S$KEYMATRIX_PLATE_BW  ]))
	{
		pMold->Plate.Step = 20000;
		gPanelObj.LedMatric[S$KEYMATRIX_PLATE_BW] = 0;
	}	
	/*------------------------Blow Pin Down Hold ---------------------------------*/
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_BLOWPINDN_HOLD  ],PKeyMatric[S$KEYMATRIX_BLOWPINDN_HOLD  ]))
	{	
		if(1 == gMachineOut.OilPump)
		{
			if (1==pMold->BlowPin.UpPos)
			{
				//	pMold->BlowPin.Step = 19000;
				pMold->ValveOut.BlowPinDnHold = !pMold->ValveOut.BlowPinDnHold;
			}
			else
			{
				if(pMold == & LMold)
				{
					gImply.LPinUp      = !pMold->BlowPin.UpPos;
				}
				else
				{
					gImply.RPinUp      = !pMold->BlowPin.UpPos;	
				}/* if(pMold == & LMold) */
			}
		}
		else
		{
			gImply.OilPumpNotOn  = !gMachineOut.OilPump;
//			gHardWare.XP0101Led_1[2] =1;
		}				
		
	}
	//	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_BLOWPINDN_HOLD  ],NKeyMatric[S$KEYMATRIX_BLOWPINDN_HOLD  ]))
	//		{
	//		pMold->BlowPin.Step = 20000;
	//		gHardWare.XP0101Led_1[2] =0;
	//	}

}/* end of ManualCtrl( Mold_typ *pMold)  */

void ManualRLCtrl( Mold_typ *pMold)
{
	/*  ------------------------   cutter -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_CUTTER_FW   ],PKeyMatric[S$KEYMATRIX_CUTTER_FW   ]))
		{
		//		gImply.DisableCutterCalibMode = 1;
		
		if (pMold->Option.Cutter)
		{
			if(1 == gMachineIn.SafeGate)
			{
				//			gThickIO.In.StartThick = 1;
				//			RMold.Cutter.Step = 100;
				////			gMachineOut.CutterFw = !gMachineOut.CutterFw;
				if (1 == gMachineOut.OilPump)
				{
					if(1 == gMachineFix.Option.bExtParisonCtrl)
					{
						gMachineInfo.ExtParisonCtrlStart = 1; /*   外部壁厚启动   */
					}
					else
					{
						gThickIO.In.StartThick = 1;      /*   壁厚启动   */
					}
					RMold.SupportAir.cntTime = gThickProf.TimSet.CycleTime;
					RMold.Cutter.Step = 300; //ipis20190131
					//	RMold.SupportAir.Step = 100;	 //ipis0304
				}	
				else
				{
					gImply.OilPumpNotOn  = !gMachineOut.OilPump;
				}
			}
			else
			{	
				gMachineOut.CutterFw = 0;
				gImply.SafeGate = 1;
			}
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}
	
	/*  ------------------------   Extruder up -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_UP   ],PKeyMatric[S$KEYMATRIX_EXTRUDER_UP   ]))
		{
		if(pMold->Option.ExtrLift)
		{
			if(1 == gMachineIn.SafeGate)
			{
				if (1 == gMachineOut.OilPump)
				{
					if( ACTUATOR_HYD  == gMachineFix.ExtrLift.eActuatorType )  // Hydraulic
					{
						if((0 == pMold->TransDIn.ExtruderLiftUpLimit && 0 == pMold->TransDIn.ExtruderLiftDnLimit)
							|| 1 == pMold->TransDIn.ExtruderLiftUpLimit)
						{
							if(pMold->ExtrLift.Step > 0 && pMold->ExtrLift.Step < 3000)
							{
								/*Extruder up*/
							}
							else
							{
								pMold->ExtrLift.Step = 10300;/*Extruder down*/
							}
						}
						else 
						{			
							if(pMold->ExtrLift.Step > 10000 && pMold->ExtrLift.Step < 13000)
							{
								/*Extruder down*/
							}
							else
							{
								pMold->ExtrLift.Step = 300;/*Extruder up*/
							}
						}
					}
					else if(ACTUATOR_ELECTRIC  == gMachineFix.ExtrLift.eActuatorType)	//Electric
					{
						RMold.ExtrLift.Step = 300;/*Extruder up*/
					}
				}
				else
				{
					gImply.OilPumpNotOn   = !gMachineOut.OilPump;
				}	
			}
			else
			{
				gImply.LowerSafeFeed = 1;
			}
		}
		else
		{
			gImply.NoFunction = 1;	
		}
	}
	
	if( ACTUATOR_ELECTRIC  == gMachineFix.ExtrLift.eActuatorType )
	{
		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_UP],NKeyMatric[S$KEYMATRIX_EXTRUDER_UP]))
			{
			RMold.ExtrLift.Step = 20000;
		}
	}

	/*  ------------------------   Extruder down (Electric) -----------------------  */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_DN],PKeyMatric[S$KEYMATRIX_EXTRUDER_DN]))
		{
		if(pMold->Option.ExtrLift)
		{
			if (1 == gMachineOut.OilPump)
			{	
				if(ACTUATOR_ELECTRIC  == gMachineFix.ExtrLift.eActuatorType)
				{
					RMold.ExtrLift.Step = 10300;/*Extruder down*/
				}
			}
			else
			{
				gImply.OilPumpNotOn  = !gMachineOut.OilPump;
			}
		}
		else
		{
			gImply.NoFunction = 1;
		}
	}
	
	if( ACTUATOR_ELECTRIC  == gMachineFix.ExtrLift.eActuatorType )
	{
		if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_DN],NKeyMatric[S$KEYMATRIX_EXTRUDER_DN]))
			{
			RMold.ExtrLift.Step = 20000;
		}
	}
	
	
	/*------------------------Plate Up---------------------------------*/
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_UP  ],PKeyMatric[S$KEYMATRIX_PLATE_UP]))
		{
		if (1 == gMachineOut.OilPump)
		{
			if( 0 == gMachineIn.PlateUpLimit)
			{
				pMold->Plate.Step = 5000;
				gPanelObj.LedMatric[S$KEYMATRIX_PLATE_UP] = 1;
					}
			else
			{
				gImply.PlateUpLimit = gMachineIn.PlateUpLimit;
			}
		}
		else
		{
			gImply.OilPumpNotOn  = !gMachineOut.OilPump;
		}
	}
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_UP  ],NKeyMatric[S$KEYMATRIX_PLATE_UP  ]))
		{
		pMold->Plate.Step = 20000;
		gPanelObj.LedMatric[S$KEYMATRIX_PLATE_UP] = 0;
			}
	
	/*------------------------Plate Down---------------------------------*/
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_DN  ],PKeyMatric[S$KEYMATRIX_PLATE_DN  ]))
		{
		if (1 == gMachineOut.OilPump)
		{
			pMold->Plate.Step = 15000;
			gPanelObj.LedMatric[S$KEYMATRIX_PLATE_DN] = 1;
				}
		else
		{
			gImply.OilPumpNotOn  = !gMachineOut.OilPump;
		}
	}
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_DN  ],NKeyMatric[S$KEYMATRIX_PLATE_DN  ]))
		{
		pMold->Plate.Step = 20000;
		gPanelObj.LedMatric[S$KEYMATRIX_PLATE_DN] = 1;
			}
	/*------------------------Plate Fw---------------------------------*/
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_FW  ],PKeyMatric[S$KEYMATRIX_PLATE_FW]))
		{
		if(1 == gMachineOut.OilPump)
		{
			pMold->Plate.Step = 6100;
			gPanelObj.LedMatric[S$KEYMATRIX_PLATE_FW] = 1;
				}
		else
		{
			gImply.OilPumpNotOn  = !gMachineOut.OilPump;
		}	
	}
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_FW  ],NKeyMatric[S$KEYMATRIX_PLATE_FW  ]))
		{
		pMold->Plate.Step = 20000;
		gPanelObj.LedMatric[S$KEYMATRIX_PLATE_FW] = 0;
			}
	/*------------------------Plate Bw---------------------------------*/
	
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_BW  ],PKeyMatric[S$KEYMATRIX_PLATE_BW]))
		{
		if(1 == gMachineOut.OilPump)
		{
			pMold->Plate.Step = 16100;
			gPanelObj.LedMatric[S$KEYMATRIX_PLATE_BW] = 1;
				}
		else
		{
			gImply.OilPumpNotOn  = !gMachineOut.OilPump;
		}	
	}
	if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_PLATE_BW  ],NKeyMatric[S$KEYMATRIX_PLATE_BW  ]))
		{
		pMold->Plate.Step = 20000;
		gPanelObj.LedMatric[S$KEYMATRIX_PLATE_BW] = 0;
			}
	
}
