/********************************************************************
 * COPYRIGHT --  B&R Automation China
 ********************************************************************
 * Program: exchange
 * File: exchange.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program exchangel --- data exchange control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#define TIME_OF_CYCLE               2

/************************************************************************************
****	        		I N I T U P  O F   T H E   T A S K 		 				 ****
************************************************************************************/

void _INIT Exchange_init(void)
{
	IsAccuReCal  = 0;
	OldAccuStart = 0;
	OldAccuStop  = 0;

	gThickProf.ShareSet.OpenGapTime  = 5;
	gThickProf.ShareSet.SealGapTime  = 5;
   
   GapMaxLimitChange = 0;
}

/************************************************************************************
****				C Y C L I C   P A R T   O F   T H E   T A S K  				 ****
************************************************************************************/

void _CYCLIC Exchange_cyclic (void)
{
	int i;

	/* 非储料缸吹瓶机 */
	gThickProf.FunOption.IsAccu = 0;

	/* 储料缸电子尺显示 */
	if((gAccuLinear.MaxPos > gAccuLinear.ZeroPos + 1000)&&(gAccuLinear.MaxPos != 32767)&&(gAccuLinear.ZeroPos != 0))
	{
		gAccuLinear.IsReady = 1;
	}
	else
	{
		gAccuLinear.IsReady = 0;
	}

	gAccuLinear.Status      = gAccuLinear.IsReady;
	gAccuLinear.MoveLength  = (gAccuLinear.MaxPos - gAccuLinear.ZeroPos)* gAccuLinear.Length/32767;
	gAccuLinear.Unit        = gAccuLinear.Length ;
	gAccuLinear.MoveLengthV = gAccuLinear.MaxPos - gAccuLinear.ZeroPos;
	
	/*Albert make connect*/
	gThickData.IsZeroDn = gThickProf.FunOption.IsZeroDn;
   
	/* 伺服电子尺显示 */
	gServoLinear.MoveLength = (gServoLinear.MaxPos - gServoLinear.ZeroPos)* gServoLinear.Length/32767;
	gServoLinear.Unit = gServoLinear.Length;
	gServoLinear.MoveLengthV = gServoLinear.MaxPos - gServoLinear.ZeroPos;

	/* 壁厚图形改变 */
   if(gServoLinear.GapMaxLimit < 10)gServoLinear.GapMaxLimit  = 10;
   if(gServoLinear.GapMaxLimit > 100)gServoLinear.GapMaxLimit = 100;
	
	if(1 == gRecipeOperate.OnceOk && 1 == gRecipeOperate.CurveOk)
	{
		if(gServoLinear.GapMaxLimit != GapMaxLimitOld)
		{
			GapMaxLimitChange = 1;
		}
	}
   GapMaxLimitOld = gServoLinear.GapMaxLimit;
   
	//   gThickData.ServoMin = gServoLinear.ZeroPos;
	//   gThickData.ServoMax = gServoLinear.ZeroPos + (gServoLinear.MaxPos - gServoLinear.ZeroPos) * gServoLinear.GapMaxLimit /100;	

	if(0 == gThickProf.FunOption.IsZeroDn)
	{
		gThickData.ServoMin = gServoLinear.ZeroPos;
		gThickData.ServoMax = gServoLinear.ZeroPos + (gServoLinear.MaxPos - gServoLinear.ZeroPos) * gServoLinear.GapMaxLimit /100;	
	}
	else
	{
		gThickData.ServoMin = gServoLinear.MaxPos - (gServoLinear.MaxPos - gServoLinear.ZeroPos) * gServoLinear.GapMaxLimit /100;
		gThickData.ServoMax = gServoLinear.MaxPos;
	}	

	gThickData.ServoLength = gThickData.ServoMax - gThickData.ServoMin;
   
	/* 封口位置 */
	if(1 == gThickProf.FunOption.SealMode)
	{
	//	if(gThickProf.ShareSet.SealGap <= 100)gThickProf.ShareSet.SealGap = 100; 
		if(gThickProf.ShareSet.SealGap <= 0)gThickProf.ShareSet.SealGap = 0;   //ipis0718
	}

//	if(gThickProf.TimSet.PurgeGap < 100)gThickProf.TimSet.PurgeGap = 100;		// 2018.07.12 Jack: Remove protection

	//	gThickData.SealPos  = gThickData.ServoMin + (gThickData.ServoMax - gThickData.ServoMin)* gThickProf.ShareSet.SealGap/1000;
	//	gThickData.PurgePos = gThickData.ServoMin + (gThickData.ServoMax - gThickData.ServoMin)* gThickProf.TimSet.PurgeGap/1000;
	if(0 == gThickProf.FunOption.IsZeroDn)
	{
		gThickData.SealPos  = gThickData.ServoMin + (gThickData.ServoMax - gThickData.ServoMin)* gThickProf.ShareSet.SealGap/1000;
		gThickData.PurgePos = gThickData.ServoMin + (gThickData.ServoMax - gThickData.ServoMin)* gThickProf.TimSet.PurgeGap/1000;
	}
	else
	{
		gThickData.SealPos  = gThickData.ServoMax - (gThickData.ServoMax - gThickData.ServoMin)* gThickProf.ShareSet.SealGap/1000;
		gThickData.PurgePos = gThickData.ServoMax - (gThickData.ServoMax - gThickData.ServoMin)* gThickProf.TimSet.PurgeGap/1000;
	}

	if(gThickProf.AccuSet.StartSize != OldAccuStart)
	{
		OldAccuStart = gThickProf.AccuSet.StartSize;
		IsAccuReCal  = 1;
	}

	if(gThickProf.AccuSet.StopBuffer != OldAccuStop)
	{
		OldAccuStart = gThickProf.AccuSet.StopBuffer;
		IsAccuReCal  = 1;
	}

	/* 储料缸每点位置计算 */
	if(1 == gAccuLinear.IsReady)
	{
		if( 1 == IsAccuReCal)
		{
			gThickData.PosData[0]  = (short)(gAccuLinear.ZeroPos + (gAccuLinear.MaxPos - gAccuLinear.ZeroPos)
									        * gThickProf.AccuSet.StartSize/1000);
			gThickData.PosData[100]= (short)(gAccuLinear.ZeroPos + (gAccuLinear.MaxPos - gAccuLinear.ZeroPos)
									        * gThickProf.AccuSet.StopBuffer/1000);
			gThickData.PointLength = (gThickData.PosData[0]- gThickData.PosData[100])/100;

			for(i=1;i<100;i++)
			{
				gThickData.PosData[i] = gThickData.PosData[100] + (100 - i)*gThickData.PointLength;
			}
			IsAccuReCal = 0;
		}
		if(gThickProf.AccuSet.StartDly<= 0)gThickProf.AccuSet.StartDly = 0;
		if(gAccuLinear.Current > gThickData.PosData[0]+ gThickData.PointLength * gThickProf.AccuSet.StartDly/10)
		{
			gThickIO.Out.FullPos  = 1;
			gThickIO.Out.EmptyPos = 0;
		}
		else if(gAccuLinear.Current < gThickData.PosData[100])
		{
			gThickIO.Out.FullPos  = 0;
			gThickIO.Out.EmptyPos = 1;
		}
		else
		{
			gThickIO.Out.FullPos  = 0;
			gThickIO.Out.EmptyPos = 0;
		}

		gControlInfo.CurrentAccuPercent = (gAccuLinear.Current - gAccuLinear.ZeroPos)* 100.0 /
										          (gAccuLinear.MaxPos - gAccuLinear.ZeroPos);
	}

	/* 储料缸射出位置限制 */
	if(gThickProf.AccuSet.StartSize < 50)gThickProf.AccuSet.StartSize = 50;

	if(gThickProf.AccuSet.StartSize < gThickProf.AccuSet.StopBuffer)gThickProf.AccuSet.StopBuffer = 0;
	/* 储料当前位置显示 */
	if((1 == gAccuLinear.IsReady) && (gThickProf.AccuSet.StartSize > gThickProf.AccuSet.StopBuffer))
	{
		if(gAccuLinear.Current <= gThickData.PosData[100])
		{
			gControlInfo.CurrentAccu = 0;
		}
		else
		{
			if(gThickData.PointLength != 0)
			{
				gControlInfo.CurrentAccu = (gAccuLinear.Current - gThickData.PosData[100])* 10/gThickData.PointLength;
			}
		    if(gControlInfo.CurrentAccu > 1000 )gControlInfo.CurrentAccu = 1000;
		}
	}
	BarAccuDis = gControlInfo.CurrentAccu/10;

	/* 伺服参数传递 */
	gThickData.CycleTimes = gThickProf.TimSet.CycleTime * 5;
	if(gThickData.CycleTimes < 500)gThickData.CycleTimes = 500;
	
	if(gThickProf.ShareSet.OpenGapTime == 0)gThickProf.ShareSet.OpenGapTime = 5;
	if(gThickProf.ShareSet.SealGapTime == 0)gThickProf.ShareSet.SealGapTime = 5;
	
	gThickProf.TimSet.StartDly =0; //ipis0801
	if(0 == gThickProf.FunOption.IsAccu)
	{
		gThickData.TimeOpenDly = gThickProf.TimSet.StartDly * gThickProf.TimSet.CycleTime / 200;
	}
	else
	{
		gThickData.AccuOpenDly = gThickProf.ShareSet.OpenGapTime * 100/TIME_OF_CYCLE;
	}

	gThickData.TimePerDot = gThickData.CycleTimes/100;

	if(gThickProf.ShareSet.SealGapTime != 0)
	{
		gThickData.SealGapDly = gThickProf.ShareSet.SealGapTime * 100/TIME_OF_CYCLE;
	}
	else
	{
		gThickProf.ShareSet.SealGapTime = 5;
	}

	if(gThickProf.ShareSet.Gain >10)gThickProf.ShareSet.Gain =10;

	TimePid.KpGain = 10 + gThickProf.ShareSet.Gain * 10;

	AccuPid.KpGain = TimePid.KpGain;
	AccuPid.KdGain = TimePid.KdGain;

	if(TimePid.Wave == 0)TimePid.Wave = 10;
	AccuPid.Wave = TimePid.Wave;

	AccuPid.Change = TimePid.Change;

	if(TimePid.Slew < 200)TimePid.Slew = 200;
	AccuPid.Slew = TimePid.Slew ;

	AccuPid.RepairOut = TimePid.RepairOut;

	if(gThickProf.ShareSet.MarkerTime < 1)gThickProf.ShareSet.MarkerTime = 5;

	if(gThickProf.ShareSet.ThickFlow < 200)gThickProf.ShareSet.ThickFlow = 800;
	if(gThickProf.ShareSet.StartFlow < 200)gThickProf.ShareSet.StartFlow = 700;
	if(gThickProf.ShareSet.SealFlow < 200 )gThickProf.ShareSet.SealFlow  = 700;

	/*gImply.ParisonPumpNotOn = !gServoLinear.IsReady;*/
}/* cyclic part end  */


