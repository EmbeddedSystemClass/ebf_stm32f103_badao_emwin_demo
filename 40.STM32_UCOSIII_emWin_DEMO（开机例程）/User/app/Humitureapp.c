﻿/**************************************************************************************
* 因为emWin显示只支持UTF-8编码格式的中文，如果希望直接显示在Keil直接输入的中文，      *
*            比如使用：GUI_DispStringHCenterAt("流水灯",110,120);                     *
* 该文件必须以UTF-8编码格式，不然中文无法正常显示。                                   *
*                                                                                     *
* 如果只是个别例程出现中文显示乱码（如果所有例程都无法显示中文可能是字库问题），      *
* 把对应的例程文件(比如LEDapp.c)用电脑的记事本软件打开，然后选择另存为，在弹出对      *
* 话框中“保存(S)"按钮的左边有个"编码(E)"选项，选择"UTF-8",然后同样保存为同名称的      *
* C语言文件(覆盖原来文件)，再编译。                                                   *
*                                                                                     *
* 如果编译工程时出现下面类似错误也是该文件编码格式问题,必须把文件保存为UTF-8格式      *
* 再编译                                                                              *
* ..\..\User\app\LEDapp.c(275): error:  #8: missing closing quote                     *
*        GUI_DispStringHCenterAt("娴?姘?鐏?",110,120);                                *
* ..\..\User\app\LEDapp.c(276): error:  #165: too few arguments in function call      *
*        GUI_DispStringHCenterAt("瑙?鎽?鍋?宸?澶?鎵?闇€瑕?瑙?鎽?鏍?鍑?",110,215);     *
* ..\..\User\app\LEDapp.c(276): error:  #18: expected a ")"                           *
*        GUI_DispStringHCenterAt("瑙?鎽?鍋?宸?澶?鎵?闇€瑕?瑙?鎽?鏍?鍑?",110,215);     *
*                                                                                     *
* 修改文件后编译就出错这是Keil5软件问题(Keil4没这问题)，推荐使用其他程序编辑工具，    *
* 只用Keil5完成编译和下载工作。                                                       *
***************************************************************************************
*                      实验平台: 野火STM32 ISO 开发板                                 *
*                      论    坛: http://www.chuxue123.com                             *
*                      淘    宝: http://firestm32.taobao.com                          *
*                      邮    箱: wildfireteam@163.com                                 *
***************************************************************************************
*/
/**************************************************************************************
*                                                                                     *
*                SEGGER Microcontroller GmbH & Co. KG                                 *
*        Solutions for real time microcontroller applications                         *
*                                                                                     *
***************************************************************************************
*                                                                                     *
* C-file generated by:                                                                *
*                                                                                     *
*        GUI_Builder for emWin version 5.22                                           *
*        Compiled Jul  4 2013, 15:16:01                                               *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG                                *
*                                                                                     *
***************************************************************************************
*                                                                                     *
*        Internet: www.segger.com  Support: support@segger.com                        *
*                                                                                     *
***************************************************************************************
*/
// USER START (Optionally insert additional includes)
#include "includes.h"
#include  "app.h"
// USER END
/**************************************************************************************
*
*       Defines
*
***************************************************************************************
*/
// USER START (Optionally insert additional defines)
#define HIGH  1
#define LOW   0

/*---------------------------------------*/
#define TEMP_HUM_CLK     RCC_APB2Periph_GPIOD
#define TEMP_HUM_PIN     GPIO_Pin_6               
#define TEMP_HUM_PORT		 GPIOD

//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define TEMP_HUM_DATA_OUT(a)	if (a)	\
															GPIO_SetBits(TEMP_HUM_PORT,TEMP_HUM_PIN);\
															else		\
															GPIO_ResetBits(TEMP_HUM_PORT,TEMP_HUM_PIN)
 //读取引脚的电平
#define  TEMP_HUM_DATA_IN()	  GPIO_ReadInputDataBit(TEMP_HUM_PORT,TEMP_HUM_PIN)
/*---------------------------------------*/
typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
}DHT11_Data_TypeDef;

typedef enum
{
	TH_ERR=0,
	TH_DS18B20_OK,
	TH_DHT11_OK
}THRESULT;
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
static DHT11_Data_TypeDef DHT11_Data;
static float temp=0.0;
static THRESULT TH_res=TH_ERR;
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateHumiture[] = {
  { FRAMEWIN_CreateIndirect, "Humiture",0, 0, 0, 240, 320, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Data1",  GUI_ID_TEXT0, 100, 83,  60, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Data2",  GUI_ID_TEXT1, 100, 163, 60, 30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "Data3",  GUI_ID_TEXT2, 100, 193, 60, 30, 0, 0x64, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
// USER START (Optionally insert additional static code)
/*
 * 函数名：TEMP_HUM_GPIO_Config
 * 描述  ：配置TEMP_HUM用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void TEMP_HUM_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启DHT11_PORT的外设时钟*/
	RCC_APB2PeriphClockCmd(TEMP_HUM_CLK, ENABLE); 

	/*选择要控制的DHT11_PORT引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = TEMP_HUM_PIN;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化DHT11_PORT*/
  	GPIO_Init(TEMP_HUM_PORT, &GPIO_InitStructure); 
}

/*
 * 函数名：TEMP_HUM_Mode_IPU
 * 描述  ：使TEMP_HUM-DATA引脚变为上拉输入模式
 * 输入  ：无
 * 输出  ：无
 */
static void TEMP_HUM_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*选择要控制的DHT11_PORT引脚*/	
	  GPIO_InitStructure.GPIO_Pin = TEMP_HUM_PIN;

	   /*设置引脚模式为浮空输入模式*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 

	  /*调用库函数，初始化DHT11_PORT*/
	  GPIO_Init(TEMP_HUM_PORT, &GPIO_InitStructure);	 
}

/*
 * 函数名：TEMP_HUM_Mode_Out_PP
 * 描述  ：使TEMP_HUM-DATA引脚变为推挽输出模式
 * 输入  ：无
 * 输出  ：无
 */
static void TEMP_HUM_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*选择要控制的DHT11_PORT引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = TEMP_HUM_PIN;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*调用库函数，初始化DHT11_PORT*/
  	GPIO_Init(TEMP_HUM_PORT, &GPIO_InitStructure);	 	 
}

/* 
 * 从DHT11读取一个字节，MSB先行
 */
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 
		/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
		while(TEMP_HUM_DATA_IN()==Bit_RESET);

		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
		 */
		bsp_DelayUS(40); //延时x us 这个延时需要大于数据0持续的时间即可	   	  

		if(TEMP_HUM_DATA_IN()==Bit_SET)/* x us后仍为高电平表示数据“1” */
		{
			/* 等待数据1的高电平结束 */
			while(TEMP_HUM_DATA_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
		}
		else	 // x us后为低电平表示数据“0”
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	return temp;
}
/*
 * 一次完整的数据传输为40bit，高位先出
 * 8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
 */
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	/*输出模式*/
	TEMP_HUM_Mode_Out_PP();
	/*主机拉低*/
	TEMP_HUM_DATA_OUT(LOW);
	/*延时18ms*/
	bsp_DelayUS(18000);

	/*总线拉高 主机延时30us*/
	TEMP_HUM_DATA_OUT(HIGH); 

	bsp_DelayUS(30);   //延时30us

	/*主机设为输入 判断从机响应信号*/ 
	TEMP_HUM_Mode_IPU();

	/*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
	if(TEMP_HUM_DATA_IN()==Bit_RESET)     
	{
		/*轮询直到从机发出 的80us 低电平 响应信号结束*/  
		while(TEMP_HUM_DATA_IN()==Bit_RESET);

		/*轮询直到从机发出的 80us 高电平 标置信号结束*/
		while(TEMP_HUM_DATA_IN()==Bit_SET);
		/*开始接收数据*/   
		DHT11_Data->humi_int= Read_Byte();

		DHT11_Data->humi_deci= Read_Byte();

		DHT11_Data->temp_int= Read_Byte();

		DHT11_Data->temp_deci= Read_Byte();

		DHT11_Data->check_sum= Read_Byte();

		/*读取结束，引脚改为输出模式*/
		TEMP_HUM_Mode_Out_PP();
		/*主机拉高*/
		TEMP_HUM_DATA_OUT(HIGH);

		/*检查读取的数据是否正确*/
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return 1;
		else 
			return 0;
	}
	else
	{		
		return 0;
	}   
}

/*********************  DS18B20 **************************/
/*
 *主机给从机发送复位脉冲
 */
static void DS18B20_Rst(void)
{
	/* 主机设置为推挽输出 */
	TEMP_HUM_Mode_Out_PP();
	
	TEMP_HUM_DATA_OUT(LOW);
	
	/* 主机至少产生480us的低电平复位信号 */
	bsp_DelayUS(750);

	/* 主机在产生复位信号后，需将总线拉高 */
	TEMP_HUM_DATA_OUT(HIGH);
	
	/*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
	bsp_DelayUS(15);
}

/*
 * 检测从机给主机返回的存在脉冲
 * 0：成功
 * 1：失败
 */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* 主机设置为上拉输入 */
	TEMP_HUM_Mode_IPU();
	
	/* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
	 * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
	 */
	while( TEMP_HUM_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		bsp_DelayUS(1);
	}	
	/* 经过100us后，存在脉冲都还没有到来*/
	if( pulse_time >=100 )
		return 0;
	else
		pulse_time = 0;
	
	/* 存在脉冲到来，且存在的时间不能超过240us */
	while( !TEMP_HUM_DATA_IN() && pulse_time<240 )
	{
		pulse_time++;
		bsp_DelayUS(1);
	}	
	if( pulse_time >=240 )
		return 0;
	else
		return 1;
}

/*
 * 从DS18B20读取一个bit
 */
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* 读0和读1的时间至少要大于60us */	
	TEMP_HUM_Mode_Out_PP();
	/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	TEMP_HUM_DATA_OUT(LOW);
	bsp_DelayUS(10);
	
	/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	TEMP_HUM_Mode_IPU();
	//Delay_us(2);
	
	if( TEMP_HUM_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* 这个延时参数请参考时序图 */
	bsp_DelayUS(45);
	
	return dat;
}

/*
 * 从DS18B20读一个字节，低位先行
 */
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

/*
 * 写一个字节到DS18B20，低位先行
 */
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	TEMP_HUM_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* 写0和写1的时间至少要大于60us */
		if (testb)
		{			
			TEMP_HUM_DATA_OUT(LOW);
			/* 1us < 这个延时 < 15us */
			bsp_DelayUS(8);
			
			TEMP_HUM_DATA_OUT(HIGH);
			bsp_DelayUS(58);
		}		
		else
		{			
			TEMP_HUM_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			bsp_DelayUS(70);
			
			TEMP_HUM_DATA_OUT(HIGH);			
			/* 1us < Trec(恢复时间) < 无穷大*/
			bsp_DelayUS(2);
		}
	}
}

void DS18B20_Start(void)
{
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);		/* 跳过 ROM */
	DS18B20_Write_Byte(0X44);		/* 开始转换 */
}

uint8_t DS18B20_Init(void)
{
	TEMP_HUM_GPIO_Config();
	DS18B20_Rst();
	
	return DS18B20_Presence();
}
/*
 * 存储的温度是16 位的带符号扩展的二进制补码形式
 * 当工作在12位分辨率时，其中5个符号位，7个整数位，4个小数位
 *
 *         |---------整数----------|-----小数 分辨率 1/(2^4)=0.0625----|
 * 低字节  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----符号位：0->正  1->负-------|-----------整数-----------|
 * 高字节  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * 温度 = 符号位 + 整数 + 小数*0.0625
 */
float DS18B20_Get_Temp(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* 跳过 ROM */
	DS18B20_Write_Byte(0X44);				/* 开始转换 */
	
	DS18B20_Rst();
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* 跳过 ROM */
	DS18B20_Write_Byte(0XBE);				/* 读温度值 */
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* 负温度 */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialogHumiture(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId){
	case WM_DELETE:
		OS_INFO("Humitureapp delete\n");
		Flag_ICON110 = 0;
		UserApp_Flag = 0;
		WM_InvalidateWindow(WM_HBKWIN);
		tpad_flag=0;
		break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Humiture'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
    FRAMEWIN_SetTitleHeight(hItem, 20);
		FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
		FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    //
    // Initialization of 'Data1'
    //
    hItem = WM_GetDialogItem(pMsg->hWin,  GUI_ID_TEXT0);
    TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
    TEXT_SetText(hItem, "");
    TEXT_SetTextColor(hItem,GUI_BLUE);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'Data2'
    //
    hItem = WM_GetDialogItem(pMsg->hWin,  GUI_ID_TEXT1);
    TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
    TEXT_SetText(hItem, "");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem,GUI_BLUE);
		//
    // Initialization of 'Data3'
    //
    hItem = WM_GetDialogItem(pMsg->hWin,  GUI_ID_TEXT2);
    TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
    TEXT_SetText(hItem, "");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem,GUI_BLUE);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
		// USER START (Optionally insert additional message handling)
  case WM_PAINT:	
		GUI_SetBkColor(APPBKCOLOR);
		GUI_SetColor(APPTEXTCOLOR);
		GUI_Clear();
		GUI_DispStringHCenterAt("温湿度读取",115,10);
		GUI_DispStringHCenterAt("请先将传感器插入卡槽上",115,30);
	
		GUI_SetColor(GUI_DARKMAGENTA);
		if(TH_res==TH_DS18B20_OK)
		{
			GUI_DispStringHCenterAt("初始化DS18B20正常",115,60);
			GUI_DispStringAt("温度:       度",60,90);
		}
		else 
			GUI_DispStringHCenterAt("初始化DS18B20异常",115,60);
		
	
	  if(TH_res==TH_DHT11_OK)
		{
			GUI_DispStringHCenterAt("初始化DHT11正常",115,140);
			GUI_DispStringAt("温度:      度",60,170);
			GUI_DispStringAt("湿度:      RH",60,200);
		}
		else 
			GUI_DispStringHCenterAt("初始化DHT11异常",115,140);
	
		break;
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateHumiture
*/
void FUN_ICON110Clicked(void) 
{
	WM_HWIN hWin;
	uint8_t i=0;
	OS_ERR      err;
	char text1[6]={0},text2[6]={0};
	OS_INFO("Humitureapp create\n");
	
	TH_res=TH_ERR;
	
	/*初始化引脚*/
	TEMP_HUM_GPIO_Config();
	
	if(DS18B20_Init()==1) 	
	{
		TH_res=TH_DS18B20_OK;							/*----  检测到DS18B20 -----*/
		temp=DS18B20_Get_Temp();	
	}
	else if(Read_DHT11(&DHT11_Data)==1)				 					
	{
		TH_res=TH_DHT11_OK;									/*----  检测到DHT11 -----*/
	}
	
	hWin = GUI_CreateDialogBox(_aDialogCreateHumiture, GUI_COUNTOF(_aDialogCreateHumiture), _cbDialogHumiture, WM_HBKWIN, 0, 0);
	while(Flag_ICON110)
	{
		if(TH_res!=TH_ERR)i++;
		if(i>=120)
		{
			OSSchedLock(&err);
			/*调用Read_DHT11读取温湿度，若成功则输出该信息*/
			if(TH_res==TH_DHT11_OK)
			{
				Read_DHT11(&DHT11_Data);
				sprintf(text1,"%d.%d",DHT11_Data.temp_int,DHT11_Data.temp_deci);
				TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1),text1);
				sprintf(text2,"%d.%d",DHT11_Data.humi_int,DHT11_Data.humi_deci);
				TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2),text2);
			}
			else if(TH_res==TH_DS18B20_OK)
			{
				temp=DS18B20_Get_Temp();
				sprintf(text1,"%0.2f",temp);
				TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0),text1);
			}
			OSSchedUnlock(&err);
			i=0;
		}
		if(tpad_flag)WM_DeleteWindow(hWin);
		GUI_Delay(10);
	}
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
