/**************************************************************************************
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
*                      实验平台: 秉火STM32 ISO 开发板                                 *
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
/*
**************************************************************************************
*                                         宏定义 
**************************************************************************************
*/ 
// USER START (Optionally insert additional defines)
#define ID_BUTTON              (GUI_ID_USER + 0)

#define BUTTON_WHITH   70
#define BUTTON_HIGHT   40
#define BUTTON_SpaceX  (5+BUTTON_WHITH) 
#define BUTTON_SpaceY  (5+BUTTON_HIGHT)
#define TEXTLENGTH   	25
// USER END
/*
**************************************************************************************
*                                      变量
**************************************************************************************
*/ 
extern GUI_CONST_STORAGE GUI_BITMAP bmhangon;
extern GUI_CONST_STORAGE GUI_BITMAP bmhangoff;
extern GUI_CONST_STORAGE GUI_BITMAP bmdelete;
extern GUI_FONT     XBF_Font;

static WM_HWIN hKey,hBack,hText;

static uint8_t 		s_Key;
static char 		text[TEXTLENGTH]={"112"};
static uint8_t 		IsCall=0;//1:calling  0:waiting
static uint8_t		textoff=0;

typedef struct {
  int          xPos;
  int          yPos;
  int          xSize;
  int          ySize;
  const char * acLabel;   /* 按钮对应的字符 */
} BUTTON;

static const BUTTON ButtonData[] = 
{

	/* 第1排按钮 */
	{ 5,                   3, 				 	BUTTON_WHITH, 		BUTTON_HIGHT, "1"  },
	{ 5+BUTTON_SpaceX,     3,  					BUTTON_WHITH, 		BUTTON_HIGHT, "2"  },
	{ 5+BUTTON_SpaceX*2,   3,  					BUTTON_WHITH, 		BUTTON_HIGHT, "3"  },
	/* 第2排按钮 */
	{ 5,   				   3+BUTTON_SpaceY,  	BUTTON_WHITH, 		BUTTON_HIGHT, "4"  },	
	{ 5+BUTTON_SpaceX,     3+BUTTON_SpaceY,  	BUTTON_WHITH, 		BUTTON_HIGHT, "5"  },
	{ 5+BUTTON_SpaceX*2,   3+BUTTON_SpaceY,  	BUTTON_WHITH, 		BUTTON_HIGHT, "6"  },
	/* 第3排按钮 */
	{ 5,   				   3+BUTTON_SpaceY*2,  	BUTTON_WHITH, 		BUTTON_HIGHT, "7"  },
	{ 5+BUTTON_SpaceX,     3+BUTTON_SpaceY*2,  	BUTTON_WHITH, 		BUTTON_HIGHT, "8"  },	
	{ 5+BUTTON_SpaceX*2,   3+BUTTON_SpaceY*2,  	BUTTON_WHITH, 		BUTTON_HIGHT, "9"  },
	/* 第4排按钮 */
	{ 5,     			   3+BUTTON_SpaceY*3,  	BUTTON_WHITH, 		BUTTON_HIGHT, "*"  },
	{ 5+BUTTON_SpaceX,     3+BUTTON_SpaceY*3,  	BUTTON_WHITH, 		BUTTON_HIGHT, "0"  },
	{ 5+BUTTON_SpaceX*2,   3+BUTTON_SpaceY*3,  	BUTTON_WHITH, 		BUTTON_HIGHT, "#"  }
};

/***********************************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePhone[] = {
	{ FRAMEWIN_CreateIndirect, "Phone",0, 0, 0, 240, 320, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "number", GUI_ID_TEXT0, 0, 0, 220, 40, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "calling", GUI_ID_TEXT1, 65, 42, 100, 20, 0, 0x64, 0 },
	{ BUTTON_CreateIndirect, "back", GUI_ID_BUTTON0, 200, 8, 24, 24, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "hangon", GUI_ID_BUTTON1, 50,232, 48, 48, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "hangoff", GUI_ID_BUTTON2, 130,232, 48, 48, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "call", GUI_ID_BUTTON3, 90,232, 48, 48, 0, 0x0, 0 },
	// USER START (Optionally insert additional widgets)
	// USER END
};
/*
*************************************************************************************
*	函 数 名: _cbKeyPad
*	功能说明: 回调函数
*	形    参：pMsg  指针参数
*	返 回 值: 无
*************************************************************************************
*/
static void _cbDialogPhone(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int        Id;
	int        NCode;
	switch (pMsg->MsgId) 
	{
		case WM_DELETE:
		OS_INFO("Phonerapp delete\n");
		Flag_ICON000 = 0;
		textoff=0;
		text[0]='\0';
		IsCall=0;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);	
		USART_Cmd(USART2, DISABLE);
		UserApp_Flag = 0;
		break;
		case WM_INIT_DIALOG:
			//
			// Initialization of 'Phone'
			//
			hItem = pMsg->hWin;
			FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
			FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
			FRAMEWIN_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
			FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
			FRAMEWIN_SetTitleHeight(hItem, 20);
			//
			// Initialization of 'number'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);
			hText=hItem;
			TEXT_SetTextColor(hItem, GUI_BLACK);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetFont(hItem, GUI_FONT_32B_ASCII);
			TEXT_SetText(hItem, "112");
			//
			// Initialization of 'calling'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1);
			TEXT_SetTextColor(hItem, GUI_BLACK);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetFont(hItem, &XBF_Font);
			TEXT_SetText(hItem, "正在呼叫...");
			WM_HideWindow(hItem);
			//
			// Initialization of 'back'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
			hBack=hItem;
			BUTTON_SetBitmap(hItem,BUTTON_BI_UNPRESSED,&bmdelete);			
			BUTTON_SetText(hItem,"");
			//WM_HideWindow(hBack);
			//
			// Initialization of 'hangon'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
			BUTTON_SetBitmap(hItem,BUTTON_BI_UNPRESSED,&bmhangon);
			BUTTON_SetText(hItem,"");
			WM_BringToTop(hItem);
			WM_HideWindow(hItem);
			//
			// Initialization of 'hangoff'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
			BUTTON_SetBitmap(hItem,BUTTON_BI_UNPRESSED,&bmhangoff);
			BUTTON_SetText(hItem,"");
			WM_BringToTop(hItem);
			WM_HideWindow(hItem);
			//
			// Initialization of 'call'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3);
			BUTTON_SetBitmap(hItem,BUTTON_BI_UNPRESSED,&bmhangon);
			BUTTON_SetText(hItem,"");
			WM_BringToTop(hItem);
			// USER START (Optionally insert additional code for further widget initialization)
			// USER END
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(Id) {
				case GUI_ID_BUTTON0: // Notifications sent by 'back'
				  switch(NCode) {
					  case WM_NOTIFICATION_CLICKED:
						// USER START (Optionally insert code for reacting on notification message)
						textoff--;
						if(textoff==255)
						{
							textoff=0;
						}
						// USER END
						break;
					  case WM_NOTIFICATION_RELEASED:
						// USER START (Optionally insert code for reacting on notification message)
						text[textoff]='\0';
						TEXT_SetText(hText,text);
						//printf("text:%s\n",text);
						if(textoff==0)WM_HideWindow(hBack);
						// USER END
						break;
					  // USER START (Optionally insert additional code for further notification handling)
					  // USER END
				  }
				  break;
				case GUI_ID_BUTTON1: // Notifications sent by 'hangon'
				  switch(NCode) {
					  case WM_NOTIFICATION_CLICKED:
						// USER START (Optionally insert code for reacting on notification message)							
						// USER END
						break;
					  case WM_NOTIFICATION_RELEASED:
						// USER START (Optionally insert code for reacting on notification message)
						IsCall=1;							
						SIM900A_HANGON();
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), "通话中...");
						SIM900A_CLEAN_RX();//清除接收缓存
						// USER END
						break;
					  // USER START (Optionally insert additional code for further notification handling)
					  // USER END
				  }
				  break;
				case GUI_ID_BUTTON2: // Notifications sent by 'hangoff'
				  switch(NCode) {
					  case WM_NOTIFICATION_CLICKED:
						// USER START (Optionally insert code for reacting on notification message)										
						// USER END
						break;
					  case WM_NOTIFICATION_RELEASED:
						// USER START (Optionally insert code for reacting on notification message)
						/*挂电话*/
						SIM900A_HANGOFF();	
						IsCall=0;
						
						WM_ShowWindow(hKey);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), "正在呼叫...");
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1));
						textoff=0;
						text[0]='\0';
						TEXT_SetText(hText,text);						
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2));
						WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3));
						SIM900A_CLEAN_RX();//清除接收缓存
						// USER END
						break;
					  // USER START (Optionally insert additional code for further notification handling)
					  // USER END
				  }
				  break;
				case GUI_ID_BUTTON3: // Notifications sent by 'call'
				  switch(NCode) {
					  case WM_NOTIFICATION_CLICKED:
						// USER START (Optionally insert code for reacting on notification message)							
						// USER END
						break;
					  case WM_NOTIFICATION_RELEASED:
						// USER START (Optionally insert code for reacting on notification message)
						if(IsCall==0)
						{
							//拨打电话号码
							sim900a_call(text);
							BUTTON_SetBitmap(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3),BUTTON_BI_UNPRESSED,&bmhangoff);
							WM_HideWindow(hKey);
							WM_HideWindow(hBack);
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1));			
							IsCall=1;
						}else	if(IsCall==1)
						{
							/*挂电话*/
							SIM900A_HANGOFF();	
							BUTTON_SetBitmap(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3),BUTTON_BI_UNPRESSED,&bmhangon);
							WM_ShowWindow(hKey);
							WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1));
							textoff=0;
							text[0]='\0';
							TEXT_SetText(hText,text);
							IsCall=0;
						}
						SIM900A_CLEAN_RX();//清除接收缓存
						// USER END
						break;
					  // USER START (Optionally insert additional code for further notification handling)
					  // USER END
				  }
				  break;
				  // USER START (Optionally insert additional code for further Ids)
				// USER END
			}
			break;
		/* 绘制背景 */	
		case WM_PAINT:	
			GUI_SetBkColor(GUI_WHITE);
			GUI_Clear();
			break;
		default:
		WM_DefaultProc(pMsg);
		break;
	}
}

static void _cbKey(WM_MESSAGE * pMsg)
{
	WM_HWIN hButton;
	uint8_t i;
	int        Id;
	int        NCode;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			// USER START (Optionally insert additional code for further widget initialization)
			/* 创建所需的按钮 */
			for (i = 0; i < GUI_COUNTOF(ButtonData); i++) 
			{
				hButton = BUTTON_CreateEx(ButtonData[i].xPos, ButtonData[i].yPos, ButtonData[i].xSize, ButtonData[i].ySize, 
				WM_GetClientWindow(pMsg->hWin), WM_CF_SHOW, 0, ID_BUTTON + i);
				BUTTON_SetFont(hButton, &GUI_Font32B_ASCII);
				BUTTON_SetText(hButton, ButtonData[i].acLabel);
				BUTTON_SetTextAlign(hButton,GUI_TA_HCENTER|GUI_TA_VCENTER);
				BUTTON_SetFocussable(hButton, 0);
			}
			// USER END
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch (NCode) 
			{
				case WM_NOTIFICATION_CLICKED:
					// USER START (Optionally insert code for reacting on notification message)
					LED1_ON;
				#ifdef USERBEEP
					macBEEP_ON();
				#endif
					WM_ShowWindow(hBack);
					// USER END
				break;
				case WM_NOTIFICATION_RELEASED:					
					// USER START (Optionally insert code for reacting on notification message)
					LED1_OFF;
				#ifdef USERBEEP
					macBEEP_OFF();
				#endif
					s_Key = ButtonData[Id - ID_BUTTON].acLabel[0];
					text[textoff]=s_Key;
					text[textoff+1]='\0';
					TEXT_SetText(hText,text);
					//printf("text:%s\n",text);
					textoff++;
					// USER END
				break;
			    // USER START (Optionally insert additional code for further notification handling)
			    // USER END
			}
			break;
		/* 绘制背景 */	
		case WM_PAINT:	
			GUI_SetBkColor(GUI_WHITE);
			GUI_Clear();
			break;
		default:
		WM_DefaultProc(pMsg);
		break;
	}
}
/*
*********************************************************************************************************
*	函 数 名: MainAPP
*	功能说明: GUI主函数 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void FUN_ICON000Clicked(void)
{
	WM_HWIN hWin;
	char num[20]={0};
	uint8_t timecount=0;
	IsCall=0;
	text[0]='1';text[1]='1';text[2]='2';text[3]='\0';
	OS_INFO("Phoneapp create\n");
	hWin=GUI_CreateDialogBox(_aDialogCreatePhone, GUI_COUNTOF(_aDialogCreatePhone), _cbDialogPhone, WM_HBKWIN, 0, 0);
	hKey=WM_CreateWindowAsChild(5,65,230,190,hWin,WM_CF_SHOW,_cbKey,0);
	WM_BringToTop(hKey);
	
	/* 初始化并检测模块 */
	if(sim900a_init()!= SIM900A_TRUE)
	{
		GUI_MessageBox("\r\n No detected SIM900A module! \r\n","error",GUI_MESSAGEBOX_CF_MOVEABLE);
	}
	SIM900A_CLEAN_RX();//清除接收缓存

	SIM900A_TX("AT+CLIP=1\r");
	timecount=0;
	while(Flag_ICON000)
	{
		if(timecount>100)
		{
			if(IsRing(num)== SIM900A_TRUE)
			{
				macBEEP_ON();
				TEXT_SetText(hText,num);
				WM_HideWindow(hKey);
				WM_HideWindow(hBack);
				TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1),"新来电...");
				WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_TEXT1));
				WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON1));
				WM_ShowWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON2));
				WM_HideWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON3));				
			}
			SIM900A_CLEAN_RX();//清除接收缓存
			macBEEP_OFF();
			timecount=0;
		}
		timecount++;
		GUI_Delay(10);
	}
}
