#ifndef keyap880_h_
#define keyap880_h_

#define S$KEYMATRIX_MAXCOUNT    400

#define OFFSET_EX2_01            216

/*  0 -- 39 AP880 key */
#define S$KEYMATRIX_MAIN_PUMP   	   0
#define S$KEYMATRIX_PARISON_PUMP 	1
#define S$KEYMATRIX_EXTRUDER_A   	2
#define S$KEYMATRIX_EXTRUDER_B   	3

#define S$KEYMATRIX_MODE_CALIB   	4
#define S$KEYMATRIX_MODE_MANUAL  	5
#define S$KEYMATRIX_MODE_AUTO    	6
#define S$KEYMATRIX_AUTO_RESET   	7
#define S$KEYMATRIX_CUTTER_FW   		8
#define S$KEYMATRIX_EXTRUDER_UP  	9
#define S$KEYMATRIX_LUBRICATION     10
#define S$KEYMATRIX_ALARM_RESET     11

#define S$KEYMATRIX_F1   				12
#define S$KEYMATRIX_F2   				13
#define S$KEYMATRIX_F3   				14
#define S$KEYMATRIX_F4   				15
#define S$KEYMATRIX_F5   				16
#define S$KEYMATRIX_F6   				17
#define S$KEYMATRIX_F7   				18
#define S$KEYMATRIX_F8   				19
#define S$KEYMATRIX_F9   				20
#define S$KEYMATRIX_F10   				21
#define S$KEYMATRIX_F11   				22
#define S$KEYMATRIX_F12   				23
#define S$KEYMATRIX_F13   				24
#define S$KEYMATRIX_F14   				25
#define S$KEYMATRIX_F15   				26
#define S$KEYMATRIX_F16   				27

#define S$KEYMATRIX_CARRIAGE_UP  	28
#define S$KEYMATRIX_CARRIAGE_DN  	29
#define S$KEYMATRIX_CLAMP_OPN   		30
#define S$KEYMATRIX_CLAMP_CLS   		31
#define S$KEYMATRIX_BLOWPIN_UP   	32
#define S$KEYMATRIX_BLOWPIN_DN   	33
#define S$KEYMATRIX_TOP_DEFLASH_FW  34
#define S$KEYMATRIX_BOT_DEFLASH_FW  35
#define S$KEYMATRIX_ROBOT_FW   		36
#define S$KEYMATRIX_ROBOT_BW   		37
#define S$KEYMATRIX_ROBOT_DN   		38
#define S$KEYMATRIX_ROBOT_CLS 		39

/*   5AC800EXT2-01 key   offset 216 */
#define S$KEYMATRIX_NUM7   		 	60
#define S$KEYMATRIX_NUM8   		 	61
#define S$KEYMATRIX_NUM9   		 	62
#define S$KEYMATRIX_NUM4   		 	64
#define S$KEYMATRIX_NUM5   		 	65
#define S$KEYMATRIX_NUM6   		 	66
#define S$KEYMATRIX_NUM1   		 	68	
#define S$KEYMATRIX_NUM2   			69
#define S$KEYMATRIX_NUM3   		 	70
#define S$KEYMATRIX_NUM0   		 	73

#define S$KEYMATRIX_ESC    		 	63
#define S$KEYMATRIX_DEL			 		67
#define S$KEYMATRIX_ENTER  	     	71
#define S$KEYMATRIX_TAB          	77
#define S$KEYMATRIX_UP					75
#define S$KEYMATRIX_DOWN				79
#define S$KEYMATRIX_LEFT				76
#define S$KEYMATRIX_RIGHT				78

/* reserve key  */
#define S$KEYMATRIX_TOP_DEFLASH_BW  101
#define S$KEYMATRIX_ROBOT_UP   		102

#define S$KEYMATRIX_ROBOT_REJECT    103
#define S$KEYMATRIX_ROBOT_RESET 		104
#define S$KEYMATRIX_SUPPORT   		105
#define S$KEYMATRIX_SPEED_UP   		106
#define S$KEYMATRIX_SPEED_DN   	   107
#define S$KEYMATRIX_COOL_DEFLASH    108
#define S$KEYMATRIX_BLOW         	109
#define S$KEYMATRIX_BLOW_LOW        110
#define S$KEYMATRIX_COOL_DN         111
#define S$KEYMATRIX_COOL_BLOW			112
#define S$KEYMATRIX_SERVO_VALVE   	113
#define S$KEYMATRIX_AUTO_LUB   		114
#define S$KEYMATRIX_DIEGAP_UP      	115
#define S$KEYMATRIX_DIEGAP_DN  	   116
#define S$KEYMATRIX_FEED_OPEN       117
#define S$KEYMATRIX_VISU_EXTRUDER   118
#define S$KEYMATRIX_CUT_NECK  	   119

#define S$KEYMATRIX_PLATE_UP        120
#define S$KEYMATRIX_PLATE_DN        121
#define S$KEYMATRIX_SUBMOLD_OPN	   122
#define S$KEYMATRIX_SUBMOLD_CLS		123
#define S$KEYMATRIX_PIN_PART_FW  	124
#define S$KEYMATRIX_PIN_PART_BW	   125
#define S$KEYMATRIX_HANDLE_PUNCHER  126
#define S$KEYMATRIX_SCREW_FW	      131
#define S$KEYMATRIX_SCREW_BW  	   132
#define S$KEYMATRIX_COOL1_DN  	   133
#define S$KEYMATRIX_COOL1_BLOW  	   134
#define S$KEYMATRIX_COOL2_DN	      135
#define S$KEYMATRIX_COOL3_DN		   136
#define S$KEYMATRIX_VACCUM  	      137
//#define S$KEYMATRIX_HEAT_POWER      138
#define S$KEYMATRIX_MINOR_PUMP      139
#define S$KEYMATRIX_PENU_VALVE      140 
#define S$KEYMATRIX_COOLING_PUMP   	141
#define S$KEYMATRIX_SENSOR_UP       142
#define S$KEYMATRIX_SENSOR_DN       143
#define S$KEYMATRIX_SENSOR_BOTTLE   144
#define S$KEYMATRIX_HOPPER_A   	   145
#define S$KEYMATRIX_DRY_CYCLE    	147

#endif


