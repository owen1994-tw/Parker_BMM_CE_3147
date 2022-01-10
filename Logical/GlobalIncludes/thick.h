
#ifndef _THICK_H_
#define _THICK_H_

#define 	STEP_THICK_READY    	     0
#define 	STEP_THICK_AUTO_RUN     100
#define 	STEP_THICK_CALIB_UP   	5000
#define 	STEP_THICK_CALIB_DOWN 	15000
#define  STEP_THICK_STOP         20000

#define	THICK_NUM				   100
/*number of thickness points            壁厚控制点数*/
#define  TIME_OF_CYCLE             2
/*cyclic time of timer1                 高速任务循环时间*/
#define  NOFOCUS                   0
#define	THICK_RANGE					300

/************** thickness control function ********************/
void TimeCurveData(Thick_Data_typ *);
void AccuCurveData(Thick_Data_typ *);
void PIDCtrl(Thick_PID_typ * pPID);
int  ISqrt(long n);                             /* 整型开平方 */
/****************************************************/
#endif /* _THICK_H_ end */


