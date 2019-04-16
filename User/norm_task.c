#include "norm_task.h"
#include <stdint.h>
#include "stm32f1xx_hal.h"

#define	C_THREADNUM 	22

ThreadTaskTimeTypedef gaT_thread[C_THREADNUM]; 		//支持25个线程

ThreadTaskTimeTypedef* 	gpT_threadpids 	 = NULL; 	//线程PID数组指针
ThreadTaskTimeTypedef* 	gpT_threadpidrun = NULL; 	//当前运行线程

SystemStatusTypedef gT_carstatus = {0}; 			//小车所有参数，状态及设备的控制结构体

SysRunTimeP_T   gT_sysnowruntime,gT_systatolruntime;
SysRunTimeM_T   gT_sysrun_mtime; 

uint32_t gui_threadstarttime = 0; 					//线程启动时间
uint8_t guch_threadnum = 0;							//记录线程数量

/**************************************************************************************
* 作者/公司  :胡鹏/高仙
* 函数介绍   :创建线程,控制时间初始化,
* 输入参数   :无
* 输出参数   :无
* 返回值     :无
****************************************************************************************/
void vFN_SysThradTaskInit(void)
{
	uint8_t i = 0;

	gpT_threadpids = &gaT_thread[0];			//线程
	i = 0;
//	vFN_CreateThread(&gpT_threadPIDs[i++],SystemLogSend,30);		//发送系统日志       0
//	vFN_CreateThread(&gpT_threadPIDs[i++],SystemDebug,50);  		//系统调试信息输出   1
//	vFN_CreateThread(&gpT_threadPIDs[i++],Read_Mode_Key,0);  		//模式切换开关按键   2
//	vFN_CreateThread(&gpT_threadPIDs[i++],Read_Emergency_Key,0); 	//急停按键判断       3	
//	vFN_CreateThread(&gpT_threadPIDs[i++],ks136_op.read,0);  		//超声波数据采集     4
//	
//	vFN_CreateThread(&gpT_threadPIDs[i++],Thread_Run_Nanopb,0); 	//上位机通信线程     5
//	vFN_CreateThread(&gpT_threadPIDs[i++],do_http,0);  				//网页配置 			 6
//	vFN_CreateThread(&gpT_threadPIDs[i++],Power_Key_Read,3);  		//钥匙开关状态检测   7  	
//	vFN_CreateThread(&gpT_threadPIDs[i++],ThreadUart3Send,5); 		//串口三发送函数     8
//	vFN_CreateThread(&gpT_threadPIDs[i++],MotorRun,10);  			//电机数据读取与控制 9
//	
//	vFN_CreateThread(&gpT_threadPIDs[i++],EncoderRead,10);  		//编码器数据读取     10
//	vFN_CreateThread(&gpT_threadPIDs[i++],imu_usart_op.read,20); 	//读取IMU            11
//	vFN_CreateThread(&gpT_threadPIDs[i++],Infrared_Read,20); 		//读取防跌落传感器   12
//	vFN_CreateThread(&gpT_threadPIDs[i++],Read_Water,35); 			//水位读取           13
//	vFN_CreateThread(&gpT_threadPIDs[i++],Thread_Run_LED,35); 		//LED控制            14 
//	
//	vFN_CreateThread(&gpT_threadPIDs[i++],DN_Motor_Read_D,50);  	//编码器数据计算     15
//	vFN_CreateThread(&gpT_threadPIDs[i++],Thread_Run_Buzzer,50); 	//蜂鸣器运行线程	 16
//	vFN_CreateThread(&gpT_threadPIDs[i++],Device_Data_Processing,100);	//电机报警信息处理  17
//	vFN_CreateThread(&gpT_threadPIDs[i++],LoraRun,100);  			//充电桩控制         18
//	vFN_CreateThread(&gpT_threadPIDs[i++],Battery_Read_Start,500); 	//读取电池数据	   	 19
//	
//	vFN_CreateThread(&gpT_threadPIDs[i++],AirDetection_Read_Start,2000); 	//读取空气质量传感器数据       20
//	vFN_CreateThread(&gpT_threadPIDs[i++],Thread_PCSystem_Init_Reset,1000); //开机检测上位机是否正常启动   21
	
	guch_threadnum = i;	//记录线程数量
	
}

/**************************************************************************************
* 作者/公司  :胡鹏/高仙
* 函数介绍   :创建线程
* 输入参数   :	
  ThreadTaskTimeTypedef *mT_pid -- 线程配置地址
  void (*mvFN_Thread)(void)     -- 函数指针，指向线程函数的指针
  uint32_t mui_targetTime		-- 函数执行的时间点

* 输出参数   :无
* 返回值     :无
****************************************************************************************/
void vFN_SysCreateThread(ThreadTaskTimeTypedef *mT_pid, void (*mvFN_Thread)(void), uint32_t mui_targetTime)
{
	mT_pid->b1_enable 	  = 1;
	mT_pid->vFN_Thread 	  = mvFN_Thread;
	mT_pid->ui_targettime = mui_targetTime;
}


/**************************************************************************************
* 作者/公司  :胡鹏/高仙
* 函数介绍   :线程时间判断与处理，(在中断定时器 1ms运行一次) 
* 输入参数   :
* 输出参数   :
  gpT_threadPIDs[i].ui_timeCount -- 线程时间计数

* 返回值     :
****************************************************************************************/
void vFN_SysTheadTaskRemarks(void) //定时器中断函数 执行时间是
{
	uint8_t i;
	uint8_t len = C_THREADNUM;	   //线程数量
	
    for (i = 0; i < len; i++) 
	{
		if(gpT_threadpids[i].b1_enable) //线程开启
		{
			if(gpT_threadpids[i].ui_timecount < gpT_threadpids[i].ui_targettime)
			{
				gpT_threadpids[i].ui_timecount++;
			}
			else
			{
				gpT_threadpids[i].ui_timecount = gpT_threadpids[i].ui_targettime;
			}
		}
    }
}

/**************************************************************************************
* 作者/公司  :胡鹏/高仙
* 函数介绍   :线程运行与处理
* 输入参数   :
* 输出参数   :
* 返回值     :
****************************************************************************************/
void vFN_SysThreadsRun(void)
{
	int i = 0;
	uint8_t len = guch_threadnum;			        //线程数量
	gT_sysrun_mtime.ui_us_loop_first_count = vFN_SystemReadTimeNow();//循环起始时间记录,单位是us
	for (i = 0; i < len; i++) 
	{		
		if(gpT_threadpids[i].b1_enable)	            //确认线程是开启的
		{
			if(gpT_threadpids[i].vFN_Thread != NULL)//确认线程非空
			{
				gT_sysrun_mtime.ui_us_thread_first_count =  vFN_SystemReadTimeNow();	//线程启动时间记录,单位是us
				if(gpT_threadpids[i].ui_targettime != 0)			//大于0,非实时线程 
				{ 
					if(gpT_threadpids[i].ui_timecount >= gpT_threadpids[i].ui_targettime)//判断线程计数时间是否到 设定触发时间
					{
						gpT_threadpidrun = &gpT_threadpids[i];	    //记录当前运行线程地址
						gpT_threadpids[i].vFN_Thread();				//执行线程函数
						gpT_threadpids[i].ui_timecount = 0;			//清0线程时间计数器
					}
				}
				else	
				{
					gpT_threadpidrun = &gpT_threadpids[i];		    //记录当前运行线程地址
					gpT_threadpids[i].vFN_Thread();				    //执行优先级最高的任务，不用判断计数，此种线程尽量少
				}
				gpT_threadpids[i].ui_threadruntime = vFN_SystemReadTimeNow() - gT_sysrun_mtime.ui_us_thread_first_count; //计算当前线程运行时间,单位是us
			}
		}
	}
    //这里添加记录线程循环时间
}

/**************************************************************************************
* 作者/公司  :胡鹏/高仙
* 函数介绍   :计数器重置，系统运行中禁止使用延时函数
              SysTick->VAL，系统滴答计数器值，72000 --> 0,  
* 输入参数   :
* 输出参数   :
* 返回值     :uch_uscount 当前us时间，最大4294S=1.1小时
****************************************************************************************/
uint32_t vFN_SystemReadTimeNow(void)
{
    uint32_t mui_uscount=0,mui_mscount=0;
    mui_mscount = HAL_GetTick();
    mui_uscount = (72000 - SysTick->VAL)/72;        //ms+us 时间
    mui_mscount = 1000*mui_mscount;
    return  mui_uscount; 
}

