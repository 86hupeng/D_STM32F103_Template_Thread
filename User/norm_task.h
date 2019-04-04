
/* 
    编程规范
    变量名 = 属性+类型+_对象描述
 
    重要变量名 		全部小写，用_分割关键字
    函数内临时变量	全部字母小写，
    函数名 			首字母大写，前缀vFN_、uiFN_、等
    结构体类型名	首字母大写，后缀_T
    
    函数类型:       _Sys                系统任务函数
                    _Mod
                    _Pro                数据处理函数，算法，数据处理等等
                    _Get，_Write        数据控制流，获取，读取
                    _Set，_Read         数据控制流，设置，输出
                    _IO，               IO控制流，输入，输出
                    
    全局变量		g
    静态变量		s
    成员变量		m
 
    常量			c
    元素常量		e
  
    结构体变量		T_
  	数组变量		a_
  	函数			Fn_
 
    char类型        ch、uch
 	short int类型	n、un
 	int类型			i、ui
  	long类型		l、ul、
  	float类型		f、uf
  	double类型		d、ud
 
    bool类型		b
    vold类型		v
  
    线程运行控制相关-结构体
  	typedef struct {
  	char b1_enable:1; 				//线程使能标志
	unsigned int un_count; 			//线程计数器
  	unsigned int un_target_time; 	//线程激活时间，ms
  	void (*vFn_Thread)(void);		//线程运行函数
  	uint32_t un_thread_run_time; 	//每个线程运行时间,单位：ns
  	} TaskTimeTypedef;
 
    结构体类型	Typedef
 
 	函数定义示列：
 	void     	vFn_CreateThread(TaskTimeTypedef *pid, void (*fun)(void), unsigned int time);
    uint32_t 	unFn_SystemRunTime(void);
 
 
 */

#ifndef NORMTASK_H
#define NORMTASK_H

#include <stdint.h>
#include <stdio.h>

// 线程控制相关-结构体
typedef struct {
	uint8_t  b1_enable:1; 				//线程使能标志
	uint32_t ui_timecount;				//线程当前计数器,ms
	uint32_t ui_targettime;				//线程激活目标时间，ms
	uint32_t ui_threadruntime; 			//每个线程运行时间,单位：us
	void (*vFN_Thread)(void);			//线程运行函数
} ThreadTaskTimeTypedef;
	
// 时间相关-结构体-(总计数/当前计数，给人看)
typedef struct {
	uint8_t uch_yearcount; 				//运行年
	uint8_t uch_monthcount;				//运行月
	uint8_t uch_daycount;				//运行天
	uint8_t uch_hourcount; 			    //运行小时
 	uint8_t uch_minutecount; 			//运行分钟
  	uint8_t uch_secondcount; 			//运行秒
   	uint16_t un_mscount; 			    //运行ms
   	uint16_t un_uscount; 			    //运行us
    
} SysRunTimeHumanTypedef;

// 时间相关-结构体-(总计数/当前计数，)
typedef struct {
   	uint32_t ui_mscount; 			    //运行ms
   	uint32_t ui_uscount; 			    //运行us
    
    uint32_t ui_usloopfirstcount; 	    //运行第一次us
    uint32_t ui_usthreadfirstcount; 	//运行第一次us
   
    uint32_t ui_usthreadloopcount;      //记录一次循环循环线程时间
} SysRunTimeMachineTypedef;

//// 系统运行状态相关-结构体,需要放到EEPROM中的数据
//typedef struct {
//	uint32_t ; 						//系统上电次数
//	uint32_t ;						//系统当前运行时间,ms，最大57天
//	uint32_t ;						//系统当前运行时间,S，最大138年
//	uint32_t 						//累计运行时间，S ,最大138年
//	uint32_t						//累计运行时间，S ,最大138年
//	
//} SystemRunStatusTypedef;
//	
//// 系统异常状态相关-结构体
//typedef struct {
//	uint32_t ; 						//累计报警
//	uint32_t ;						//XX异常
//	uint32_t ;						//XX异常
//	uint32_t 						//XX异常
//	uint32_t						//XX异常
//	
//} SystemRunStatusTypedef;	

//// 异常状态相关-结构体
//typedef struct {
//	uint32_t ; 							//异常设备
//	uint32_t ;							//异常发生时间
//	uint32_t ;							//异常持续时间
//	uint32_t 							//异常发生地点
//	
//} SystemRunStatusTypedef;	
	
//小车所有参数，状态及设备的控制
typedef struct
{
    uint8_t b1_emergency:1;           	//急停按键状态
    uint8_t b1_carMode:1;             	//0 手动模式 1 自动模式
    uint8_t b1_expectBushLift:1;      	//刷盘期望位置 0 上 1下
    uint8_t b1_expectSqueegeeLift:1;  	//吸水条期望位置 0 上 1下
    uint8_t b2_runBushLift:2;         	//刷盘上下的运行状态 0 上 1下 2 运行中
    uint8_t b2_runSqueegeeLift:2;     	//吸水上下的运行状态 0 上 1下 2运行中
	
	uint8_t b1_tfMiniFlag:1;			//北醒传感器距离触发标志
	uint8_t b1_relayCmd:1; 				//下发的继电器控制命令	
    uint8_t b1_bushLiftMode:1;        	//刷盘升降电机0正常模式 1 电流模式
    uint8_t b1_keyWitch:1; 				//钥匙开关状态
	uint8_t b1_brokenLineFlag:1;		
	uint8_t b1_brakerDownFlag:1; 		//刹车
	
	uint8_t ch_brushPressureLevel; 		//刷盘电机压力等级
    uint8_t ch_cleanWaterLevel;			//清水水量百分百
    uint8_t ch_sewageLevel;				//污水水量百分百
    uint8_t ch_waterMotorValve; 		//喷水电机阀门状态
    uint8_t ch_runTaskStatus;			//当前车的任务状态
	uint8_t  uch_batterSof;				//电池电量百分比
	uint8_t  uch_systemStartFlag; 		//完成开机标志 0 开机中 1开机完成 2 关机过程中 3 关机完成
	uint8_t  uch_waterLevelSensorStatus;	//水位浮子开关标志，过滤电机开机，判断条件
	uint16_t un_squeegeeDownFullTime; 	//吸水条电机升降时间
	uint16_t un_outputState; 			//输出状态
	uint16_t un_intputState; 			//输入状态
	
	int16_t  na_distance[3]; 			//红外测距，单位mm
	uint16_t una_tfMini[2];				//北醒传感器距离数据	
	uint16_t una_ks136Data[16];			//超声波数据
	uint32_t ula_mcuErrorCode[20]; 		//报警信息

	uint8_t ch_relayData; 				//上传的继电器控制命令
	uint8_t ch_checkCharger; 			//充电桩查询命令
	uint8_t ch_chargerStatus; 			//充电桩状态

//    TbBatteryTypedef 	T_battery;  	//拓邦电池相关信息
//    CarErnCoderTypedef 	T_ernCode;		//编码器相关信息
//	AvrDataTypedef 		T_avrData;		//avr相关数据
//    LEDTypedef 			T_led;			//LED灯的相关数据	
//	ADS1115Typedef 		T_ads1115; 		//IIC3所接的ads1115
//	SnlaircDataTypedef 	T_snlairc; 		//空气质量传感器数据
	
}SystemStatusTypedef;

extern SystemStatusTypedef CarStatus;	//小车所有参数，状态及设备的控制结构体

void vFN_ThradTaskRemarks(void); 		//任务时间分配控制
void vFN_ThradTaskProcess(void);		//任务处理函数
void vFN_ThradTaskInit(void);			//线程任务初始化

void vFN_SystemParameterInit(void);		//系统参数初始化
void vFN_SystemPeripheralsInit(void);	//系统外设初始化

uint32_t vFN_SystemReadTimeNow(void);   //读系统现在时间-us

void vFN_CreateThread(ThreadTaskTimeTypedef *mT_pid, void (*mvFN_Thread)(void), uint32_t mui_targetTime);//创建线程

uint32_t uiFN_ThreadSystemRunTime(void);		//计算线程运行时间
uint32_t uiFN_ThreadRunTime(void);		//当前系统运行时间

void vFN_ThreadPCSystemInitReset(void);	//开机检测上位机是否启动，超时未启动则进行重启


#endif
