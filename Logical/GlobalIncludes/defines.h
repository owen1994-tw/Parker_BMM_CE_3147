#ifndef __DEFINES_H__
#define __DEFINES_H__


#define MODE_NOTHING              10
#define MODE_MANUAL               20
#define MODE_AUTO                 30
#define MODE_SEMI_AUTO            35
#define MODE_ADJUST               40


/* ------------------------------------------------------------------- */
#define  CONTROL_STAT_ACTIVE    1
/* ------------------------------------------------------------------- */


#define  S$STATUS_AUTORUN           1
#define  S$STATUS_CALIBRATE         2
#define  S$STATUS_MANUAL            3
#define  S$STATUS_NEED_CALIBRATE    4

#define  ABS(X)   (((X) > 0.0) ? (X) : (-1.0*(X))) 



#endif /* #ifndef __DEFINES_H__ */


