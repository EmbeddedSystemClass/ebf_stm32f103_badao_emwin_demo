/*
******************************************************************************
* @file    bsp_led.c
* @author  秉火
* @version V1.0
* @date    2015-xx-xx
* @brief   led应用函数接口
******************************************************************************
* @attention
*
* 实验平台:秉火 iSO STM32 开发板 
* 论坛    :http://www.chuxue123.com
* 淘宝    :http://firestm32.taobao.com
*
******************************************************************************
*/
#include "includes.h"
#include  "app.h"

#define BK_COLOR       GUI_BLACK
#define MIDWIN_COLOR   0X303030
#define BOTWIN_COLOR   0X101010
#define ICON_COLOR     GUI_LIGHTCYAN
#define TEXT_COLOR		 GUI_WHITE
/*
*********************************************************************************************************
*                                         宏定义 
*********************************************************************************************************
*/
#define ICONVIEW_Width     48   /* 控件ICONVIEW的宽 */  
#define ICONVIEW_Height    48   /* 控件ICONVIEW的高，包括图标的Y方向间距 */  
#define ICONVIEW_XSpace    10   /* 控件ICONVIEW的X方向间距 */ 
#define ICONVIEW_YSpace    12   /* 控件ICONVIEW的Y方向间距 */ 
#define ICONVIEW_XPos	     3    /* 控件ICONVIEW的X方向调整距离*/
#define VIRTUAL_WIN_NUM	   2    /* icon的页数 */
#define BOTWIN_YSpace	     62   /* 屏幕下方部分窗口Y方向间距*/
#define MIDWIN_yPos		     20   /* 中间窗口Y方向起始位置即顶部文字可用高度*/
#define MIDWIN_xPos		     0 	  /* 中间窗口X方向起始位置*/

/*
*********************************************************************************************************
*                                      变量
*********************************************************************************************************
*/ 
GUI_XBF_DATA XBF_Data;
GUI_FONT     XBF_Font;

uint8_t UserApp_Flag=0;//任务界面标志

uint8_t Flag_ICON000  = 0;   /* ICONVIEW控件按下的标志，0表示未按下，1表示按下 */
uint8_t Flag_ICON001  = 0;

uint8_t Flag_ICON100  = 0;
uint8_t Flag_ICON101  = 0;
uint8_t Flag_ICON102  = 0;
uint8_t Flag_ICON103  = 0;

uint8_t Flag_ICON104  = 0;
uint8_t Flag_ICON105  = 0;
uint8_t Flag_ICON106  = 0;
uint8_t Flag_ICON107  = 0;

uint8_t Flag_ICON108  = 0;
uint8_t Flag_ICON109  = 0;
uint8_t Flag_ICON110  = 0;
uint8_t Flag_ICON111  = 0;

uint8_t Flag_ICON200  = 0;
uint8_t Flag_ICON201  = 0;
uint8_t Flag_ICON202  = 0;
uint8_t Flag_ICON203  = 0;


/* 用于桌面ICONVIEW图标的创建 */
typedef struct {
  const GUI_BITMAP * pBitmap;
  const char       * pText;
} BITMAP_ITEM;

typedef struct WIN_PARA{			//窗口使用到的用户定义参数，方便在回调函数中使用
	int xSizeLCD, ySizeLCD;			//LCD屏幕尺寸
	int xPosWin,  yPosWin;			//窗口的起始位置
	int xSizeWin, ySizeWin;			//窗口尺寸	
	int xSizeBM,  ySizeBM;
	int ySizeBotWin;
	
	WM_HWIN hWinBot;				//控制窗口的句柄（底部）
	WM_HWIN hWinMid;				//主显示窗的句柄（中部）
	
}WIN_PARA;
WIN_PARA WinPara;			//用户使用的窗口额外数据

/* 用于桌面ICONVIEW0图标的创建 */
static const BITMAP_ITEM _aBitmapItem0[] = {
	{&bmPhone,     "Phone" 		},
	{&bmmessages,  "Message"	}
};
/* 用于桌面ICONVIEW1图标的创建 */
static const BITMAP_ITEM _aBitmapItem1[] = {
  {&bmkey,  	 	"KEY"		    },
  {&bmrgbled,	 	"Breathing"	}, 
//  {&bmadc,  	 	"ADC"				},
//	{&bmeeprom,   "EEPROM" 		},
//	
//  {&bmclock,    "Clock" 		},
//  {&bmmusic,    "Music" 		},
//	{&bmrecorder,	"Recorder"	},
//	{&bmusb,  	  "USB"		    },
//	
//	{&bmwifi,			"Wifi"		  },	
//	{&bmnet,   	  "Network" 	},	
//	{&bmWeather,  "Humiture"  }, 
//  {&bmcamera,   "Camera"		}, 
};

/* 用于桌面ICONVIEW2图标的创建 */
static const BITMAP_ITEM _aBitmapItem2[] = {
	{&bmcalculator,"Calculator" },
//	{&bmpicture,   "Picture" 		},
//	{&bmnote,      "Note" 		  },
//	{&bmbrowser,   "Browser"		}, 	
};

void FUN_ICON000Clicked(void)  {printf("FUN_ICON000Clicked\n");}
void FUN_ICON001Clicked(void)  {printf("FUN_ICON001Clicked\n");}

//void FUN_ICON100Clicked(void)  {printf("FUN_ICON100Clicked\n");}
//void FUN_ICON101Clicked(void)  {printf("FUN_ICON101Clicked\n");}
void FUN_ICON102Clicked(void)  {printf("FUN_ICON102Clicked\n");}
void FUN_ICON103Clicked(void)  {printf("FUN_ICON103Clicked\n");}

void FUN_ICON104Clicked(void)  {printf("FUN_ICON104Clicked\n");}
void FUN_ICON105Clicked(void)  {printf("FUN_ICON105Clicked\n");}
void FUN_ICON106Clicked(void)  {printf("FUN_ICON106Clicked\n");}
void FUN_ICON107Clicked(void)  {printf("FUN_ICON107Clicked\n");}

void FUN_ICON108Clicked(void)  {printf("FUN_ICON108Clicked\n");}
void FUN_ICON109Clicked(void)  {printf("FUN_ICON109Clicked\n");}
void FUN_ICON110Clicked(void)  {printf("FUN_ICON110Clicked\n");}
void FUN_ICON111Clicked(void)  {printf("FUN_ICON111Clicked\n");}

void FUN_ICON200Clicked(void)  {printf("FUN_ICON200Clicked\n");}
void FUN_ICON201Clicked(void)  {printf("FUN_ICON201Clicked\n");}
void FUN_ICON202Clicked(void)  {printf("FUN_ICON202Clicked\n");}
void FUN_ICON203Clicked(void)  {printf("FUN_ICON203Clicked\n");}

/*
*********************************************************************************************************
*	函 数 名: _cbBkWindow
*	功能说明: 桌面窗口的回调函数 
*	形    参：WM_MESSAGE * pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
void _cbBkWindow(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		/* 重绘消息*/
		case WM_PAINT:		
				GUI_SetBkColor(BK_COLOR);
				GUI_Clear();
				ICONVIEW_SetSel(WM_GetDialogItem(WinPara.hWinMid, GUI_ID_ICONVIEW2),-1);
				ICONVIEW_SetSel(WM_GetDialogItem(WinPara.hWinMid, GUI_ID_ICONVIEW1),-1);
				ICONVIEW_SetSel(WM_GetDialogItem(WinPara.hWinBot, GUI_ID_ICONVIEW0),-1);			
		break;
			
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: _cbMidWin
*	功能说明: 
*	形    参：WM_MESSAGE * pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
void _cbMidWin(WM_MESSAGE * pMsg) 
{
	int NCode, Id;
	WM_MOTION_INFO * pInfo;
	switch (pMsg->MsgId) 
	{
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
			NCode = pMsg->Data.v;                 /* Notification code */
			switch (Id) 
			{
				case GUI_ID_ICONVIEW1:
					switch (NCode) 
					{
						/* ICON控件点击消息 */
						case WM_NOTIFICATION_CLICKED:
							UserApp_Flag = 1;
							break;
						
						/* ICON控件释放消息 */
						case WM_NOTIFICATION_RELEASED: 
							
							/* 打开相应选项 */
							switch(ICONVIEW_GetSel(pMsg->hWinSrc))
							{
								/* KEY  ******************************************************************/
								case 0:	
									Flag_ICON100 = 1;
									FUN_ICON100Clicked();			
									break;	
								
								/* Breathing ***********************************************************************/
								case 1:
									Flag_ICON101 = 1;
									FUN_ICON101Clicked();
									break;
								
								/* ADC *********************************************************************/
								case 2:
									Flag_ICON102 = 1;
									FUN_ICON102Clicked();	
									break;
								
								/* EEPROM ********************************************************************/
								case 3:
									Flag_ICON103 = 1;
									FUN_ICON103Clicked();
									break;
								
								/* Clock **********************************************************************/
								case 4:
									Flag_ICON104 = 1;
									FUN_ICON104Clicked();
									break;
								
								/* Music ********************************************************************/
								case 5:
									Flag_ICON105 = 1;
									FUN_ICON105Clicked();
									break;
								
								/* Recorder ******************************************************************/
								case 6:					
									Flag_ICON106 = 1;
									FUN_ICON106Clicked();
									break;
								
								/* USB *******************************************************************/
								case 7:
									Flag_ICON107 = 1;
									FUN_ICON107Clicked();
									break;
								
								/* Wifi ******************************************************************/
								case 8:
									Flag_ICON108 = 1;
									FUN_ICON108Clicked();
									break;
								 
								 /* Network ******************************************************************/
								case 9:
									Flag_ICON109 = 1;
									FUN_ICON109Clicked();
									break;
								 
								 /* Humiture ******************************************************************/
								case 10:
									Flag_ICON110 = 1;
									FUN_ICON110Clicked();
									break;
								 
								/* Camera *******************************************************************/
								case 11:
									Flag_ICON111 = 1;
									FUN_ICON111Clicked();
									break;
								default:break;
								}
							 break;
						}
					break;
				case GUI_ID_ICONVIEW2:
					switch (NCode) 
					{
						/* ICON控件点击消息 */
						case WM_NOTIFICATION_CLICKED:
							UserApp_Flag = 1;
							break;
						
						/* ICON控件释放消息 */
						case WM_NOTIFICATION_RELEASED: 
							
							/* 打开相应选项 */
							switch(ICONVIEW_GetSel(pMsg->hWinSrc))
							{			
								
								/* Calculator *******************************************************************/
								case 0:
									Flag_ICON200 = 1;
									FUN_ICON200Clicked();
									break;
								
								/* Picture ***************************************************************/
								case 1:
									Flag_ICON201 = 1;
									FUN_ICON201Clicked();
									break;

								/* Note ***************************************************************/
								case 2:
									Flag_ICON202 = 1;
									FUN_ICON202Clicked();
									break;		
								/* Browser ***************************************************************/
								case 3:
									Flag_ICON203 = 1;
									FUN_ICON203Clicked();
									break;	
								default:break;
							}	
						 break;
					}
				break;
			}
			break;
		
		case WM_MOTION:
			 pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
			 if (pInfo) 
			 {
				//printf("mooton_message=%d\n",pInfo->Cmd);
				 switch (pInfo->Cmd) 
				 {				    
					case WM_MOTION_INIT:
						pInfo->SnapX = 240;		//像素对齐
						pInfo->SnapY = 0;
						//pInfo->Flags = WM_CF_MOTION_X | WM_MOTION_MANAGE_BY_WINDOW;
						pInfo->Period = 1000;
					break;
					
					case WM_MOTION_MOVE:
						WM_DisableWindow(WM_GetDialogItem(pMsg->hWin,GUI_ID_ICONVIEW1));
						WM_DisableWindow(WM_GetDialogItem(pMsg->hWin,GUI_ID_ICONVIEW2));
					break;							
					case WM_MOTION_GETPOS:
						//printf("\n dx = %d",pInfo->dx);
						//printf(" dy = %d",pInfo->dy);
						//printf(" xpos = %d\n",pInfo->xPos);
						//printf(" ypos = %d\n",pInfo->yPos);
					break;								
					default:
						break;						
				  }
					/* 移动结束 */
					if (pInfo->FinalMove || ((pInfo->dx<=2)&&(pInfo->dx>=-2)))
					{
						WM_EnableWindow(WM_GetDialogItem(pMsg->hWin,GUI_ID_ICONVIEW1));
						WM_EnableWindow(WM_GetDialogItem(pMsg->hWin,GUI_ID_ICONVIEW2));
					}
			 }	
			
			break;
			
		/* 重绘消息*/
		case WM_PAINT:
			{
				GUI_SetBkColor(MIDWIN_COLOR);
				GUI_Clear();
			}			
		break;			
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/**
  * @brief  _cbBotWin,控制栏回调函数
  * @param  none
  * @retval none
  */
static void _cbBotWin(WM_MESSAGE * pMsg)
{
	int NCode, Id;
	switch (pMsg->MsgId) 
	{			 		 
		case WM_PAINT:	                             //重绘背景	
			GUI_SetBkColor(BOTWIN_COLOR);
			GUI_Clear();
		break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
			NCode = pMsg->Data.v;                 /* Notification code */
		switch (Id) 
		{
			case GUI_ID_ICONVIEW0:
				switch (NCode) 
				{
					/* ICON控件点击消息 */
					case WM_NOTIFICATION_CLICKED:
						UserApp_Flag = 1;
						break;
					
					/* ICON控件释放消息 */
					case WM_NOTIFICATION_RELEASED: 
						
						/* 打开相应选项 */
						switch(ICONVIEW_GetSel(pMsg->hWinSrc))
						{
							/* Phone *******************************************************************/
							case 0:	
								Flag_ICON000 = 1;
								FUN_ICON000Clicked();			
								break;	
							
							/* Message ***********************************************************************/
							case 1:
								Flag_ICON001 = 1;
								FUN_ICON001Clicked();
								break;
						}	
				 break;
			}
		break;
	}
	break;		
	default:		
		WM_DefaultProc(pMsg);
	break;		 
	}
}
/**
  * @brief  CreateTopWin，创建顶部的窗口
  * @param  none
  * @retval none
  */
static void CreateTopWin(void)
{
	WM_HWIN hText;
	/* 顶部的 "wildfire OS "文本 */
	hText = TEXT_CreateEx(0, 2, 60 , 16, WM_HBKWIN, WM_CF_SHOW, GUI_TA_LEFT|TEXT_CF_VCENTER, GUI_ID_TEXT0, "Wildfire OS");
	TEXT_SetFont(hText, GUI_FONT_10_ASCII);
	TEXT_SetTextColor(hText,TEXT_COLOR);
	
	/* 状态栏的时间显示文本 */
	hText = TEXT_CreateEx(WinPara.xSizeLCD/2-25,2,50,16,WM_HBKWIN,WM_CF_SHOW,GUI_TA_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT1,"11:56:00");
	//TEXT_SetBkColor(hText,GUI_INVALID_COLOR);
	TEXT_SetTextColor(hText,TEXT_COLOR);
	TEXT_SetFont(hText,GUI_FONT_8_ASCII);
}
/**
  * @brief  CreateMidWin，创建中间的窗口
  * @param  none
  * @retval none
  */
static void CreateMidWin(void)
{
	uint8_t i=0;
	WM_HWIN MIDWIN;
	WinPara.hWinMid= WM_CreateWindowAsChild(
											WinPara.xPosWin,											
											WinPara.yPosWin,
											WinPara.xSizeWin,
											WinPara.ySizeWin,	
											WM_HBKWIN, 
											WM_CF_SHOW | WM_CF_MOTION_X, 
											_cbMidWin, 
											sizeof(WIN_PARA *)
											);
/*-------------------------------------------------------------------------------------*/
					/*在指定位置创建指定尺寸的ICONVIEW1 小工具*/
					MIDWIN=ICONVIEW_CreateEx(
											 ICONVIEW_XPos, 					/* 小工具的最左像素（在父坐标中）*/
											 0, 								/* 小工具的最上像素（在父坐标中）*/
											 WinPara.xSizeLCD-ICONVIEW_XPos,	/* 小工具的水平尺寸（单位：像素）*/
											 3*(WinPara.ySizeBM+20), 			/* 小工具的垂直尺寸（单位：像素）*/
											 WinPara.hWinMid, 				    /* 父窗口的句柄。如果为0 ，则新小工具将成为桌面（顶级窗口）的子窗口 */
											 WM_CF_SHOW | WM_CF_HASTRANS,       /* 窗口创建标记。为使小工具立即可见，通常使用 WM_CF_SHOW */ 
											 0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* 默认是0，如果不够现实可设置增减垂直滚动条 */
											 GUI_ID_ICONVIEW1, 			        /* 小工具的窗口ID */
											 ICONVIEW_Width, 				    /* 图标的水平尺寸 */
											 ICONVIEW_Height+10);				/* 图标的垂直尺寸 */
											 
		/* 向ICONVIEW 小工具添加新图标 */
		for (i = 0; i < GUI_COUNTOF(_aBitmapItem1); i++) 
		{
			ICONVIEW_AddBitmapItem(MIDWIN, _aBitmapItem1[i].pBitmap,_aBitmapItem1[i].pText);
			ICONVIEW_SetTextColor(MIDWIN,i,TEXT_COLOR);
		}	
		/* 设置小工具的背景色 32 位颜色值的前8 位可用于alpha混合处理效果*/
		ICONVIEW_SetBkColor(MIDWIN, ICONVIEW_CI_SEL, ICON_COLOR | 0x80000000);
		/* 设置字体 */
		ICONVIEW_SetFont(MIDWIN, &GUI_Font10_ASCII);
		/* 设置初始选择的图标为 -1 (表示尚未选择)*/
		ICONVIEW_SetSel(MIDWIN,-1);
		/* 设置图标在x 或y 方向上的间距。*/
		ICONVIEW_SetSpace(MIDWIN, GUI_COORD_X, ICONVIEW_XSpace);
		ICONVIEW_SetSpace(MIDWIN, GUI_COORD_Y, ICONVIEW_YSpace);
	//	ICONVIEW_SetSpace(hWin, GUI_COORD_Y, ICONVIEW_YSpace);
		/* 设置对齐方式 在5.22版本中最新加入的 */
		ICONVIEW_SetIconAlign(MIDWIN, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
		
/*-------------------------------------------------------------------------------------*/
		/*在指定位置创建指定尺寸的ICONVIEW2小工具*/
		MIDWIN=ICONVIEW_CreateEx(
								 WinPara.xSizeLCD+ICONVIEW_XPos,	/* 小工具的最左像素（在父坐标中）*/
								 0, 								/* 小工具的最上像素（在父坐标中）*/
								 WinPara.xSizeLCD-ICONVIEW_XPos,	/* 小工具的水平尺寸（单位：像素）*/
								 2*(WinPara.ySizeBM+20),			/* 小工具的垂直尺寸（单位：像素）*/
								 WinPara.hWinMid, 				    /* 父窗口的句柄。如果为0 ，则新小工具将成为桌面（顶级窗口）的子窗口 */
								 WM_CF_SHOW | WM_CF_HASTRANS,       /* 窗口创建标记。为使小工具立即可见，通常使用 WM_CF_SHOW */ 
								 0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* 默认是0，如果不够现实可设置增减垂直滚动条 */
								 GUI_ID_ICONVIEW2, 			        /* 小工具的窗口ID */
								 ICONVIEW_Width, 				    /* 图标的水平尺寸 */
								 ICONVIEW_Height+10);				/* 图标的垂直尺寸 */
		/*-------------------------------------------------------------------------------------*/						 
		/* 向ICONVIEW 小工具添加新图标 */
		for (i = 0; i < GUI_COUNTOF(_aBitmapItem2); i++) 
		{
			ICONVIEW_AddBitmapItem(MIDWIN, _aBitmapItem2[i].pBitmap,_aBitmapItem2[i].pText);
			ICONVIEW_SetTextColor(MIDWIN,i,TEXT_COLOR);
		}	
		
		/* 设置小工具的背景色 32 位颜色值的前8 位可用于alpha混合处理效果*/
		ICONVIEW_SetBkColor(MIDWIN, ICONVIEW_CI_SEL, ICON_COLOR | 0x80000000);
		/* 设置字体 */
		ICONVIEW_SetFont(MIDWIN, &GUI_Font10_ASCII);
		/* 设置初始选择的图标为 -1 (表示尚未选择)*/
		ICONVIEW_SetSel(MIDWIN,-1);
		/* 设置图标在x 或y 方向上的间距。*/
		ICONVIEW_SetSpace(MIDWIN, GUI_COORD_X, ICONVIEW_XSpace);
		ICONVIEW_SetSpace(MIDWIN, GUI_COORD_Y, ICONVIEW_YSpace);
	//	ICONVIEW_SetSpace(hWin, GUI_COORD_Y, ICONVIEW_YSpace);
		/* 设置对齐方式 在5.22版本中最新加入的 */
		ICONVIEW_SetIconAlign(MIDWIN, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
}

/**
  * @brief  CreateBotWin，创建底部的窗口
  * @param  none
  * @retval none
  */
static void CreateBotWin(void)
{
	uint8_t i;
	WM_HWIN BOTWIN;
	WinPara.hWinBot= WM_CreateWindowAsChild(
											0,											
											WinPara.ySizeLCD-WinPara.ySizeBotWin,	//底部位置
											WinPara.xSizeLCD,
											WinPara.ySizeBotWin,	//底部剩余宽度
											WM_HBKWIN, 
											WM_CF_SHOW,
											_cbBotWin, 
											sizeof(WIN_PARA *)
											);
					/*在指定位置创建指定尺寸的ICONVIEW 小工具*/
					BOTWIN=ICONVIEW_CreateEx(
											 62,                					      /* 小工具的最左像素（在父坐标中）*/
											 0, 								                /* 小工具的最上像素（在父坐标中）*/
											 116,                               /* 小工具的水平尺寸（单位：像素）*/
											 WinPara.ySizeBotWin,				        /* 小工具的垂直尺寸（单位：像素）*/
											 WinPara.hWinBot, 				          /* 父窗口的句柄。如果为0 ，则新小工具将成为桌面（顶级窗口）的子窗口 */
											 WM_CF_SHOW | WM_CF_HASTRANS,       /* 窗口创建标记。为使小工具立即可见，通常使用 WM_CF_SHOW */ 
											 0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* 默认是0，如果不够现实可设置增减垂直滚动条 */
											 GUI_ID_ICONVIEW0, 			            /* 小工具的窗口ID */
											 ICONVIEW_Width, 				            /* 图标的水平尺寸 */
											 ICONVIEW_Height);					        /* 图标的垂直尺寸 */
	WM_BringToTop(WinPara.hWinBot);
	/* 向ICONVIEW 小工具添加新图标 */
	for (i = 0; i < GUI_COUNTOF(_aBitmapItem0); i++) 
	{
		ICONVIEW_AddBitmapItem(BOTWIN, _aBitmapItem0[i].pBitmap,"");
	}	
	/* 设置小工具的背景色 32 位颜色值的前8 位可用于alpha混合处理效果*/
	ICONVIEW_SetBkColor(BOTWIN, ICONVIEW_CI_SEL, ICON_COLOR | 0x80000000);
	/* 设置字体 */
//	ICONVIEW_SetFont(hWinicon0, &GUI_Font10_ASCII);
	/* 设置初始选择的图标为 -1 (表示尚未选择)*/
	ICONVIEW_SetSel(BOTWIN,-1);
	/* 设置图标在x 或y 方向上的间距。*/
	ICONVIEW_SetSpace(BOTWIN, GUI_COORD_X, ICONVIEW_XSpace);
//	ICONVIEW_SetSpace(hWin, GUI_COORD_Y, ICONVIEW_YSpace);
	/* 设置对齐方式 在5.22版本中最新加入的 */
	ICONVIEW_SetIconAlign(BOTWIN, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
}
/*
*********************************************************************************************************
*
*       _cbGetData
*
* Function description
*   Callback function for getting font data
*
* Parameters:
*   Off      - Position of XBF file to be read
*   NumBytes - Number of requested bytes
*   pVoid    - Application defined pointer
*   pBuffer  - Pointer to buffer to be filled by the function
*
* Return value:
*   0 on success, 1 on error
*********************************************************************************************************
*/
static int _cbGetData(U32 Offset, U16 NumBytes, void * pVoid, void * pBuffer)
{
	SPI_FLASH_BufferRead(pBuffer,60*4096+Offset,NumBytes);
	return 0;
}

static void _CreateXBF(void) 
{
	//
	// Create XBF font
	//
	GUI_XBF_CreateFont(&XBF_Font,       // Pointer to GUI_FONT structure in RAM
					   &XBF_Data,         // Pointer to GUI_XBF_DATA structure in RAM
					   GUI_XBF_TYPE_PROP,//GUI_XBF_TYPE_PROP_EXT, 		// Font type to be created
					   _cbGetData,        // Pointer to callback function
					   0);        // Pointer to be passed to GetData function
}

/*
*********************************************************************************************************
*	函 数 名: UserAPP
*	功能说明: GUI主函数 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void UserAPP(void)
{
	UserApp_Flag=0;
	//准备建立3个窗口，以下是使用到的用户定义参数，方便在回调函数中使用
	WinPara.xSizeLCD = LCD_GetXSize();				//LCD屏幕尺寸
	WinPara.ySizeLCD = LCD_GetYSize();				//LCD屏幕尺寸
	WinPara.xSizeBM  = ICONVIEW_Width;				//图标宽度
	WinPara.ySizeBM  = ICONVIEW_Height;				//图标高度
	WinPara.ySizeBotWin=BOTWIN_YSpace;				//界面下方窗口高度
	WinPara.xSizeWin = WinPara.xSizeLCD*VIRTUAL_WIN_NUM;						//窗口尺寸
	WinPara.ySizeWin = WinPara.ySizeLCD-WinPara.ySizeBotWin-WinPara.yPosWin;	//窗口尺寸
	WinPara.xPosWin	 = MIDWIN_xPos;							//窗口的起始位置
	WinPara.yPosWin  = MIDWIN_yPos;							//窗口的起始位置
	WM_SetSize(WM_HBKWIN,WinPara.xSizeLCD,WinPara.ySizeLCD);//设置背景窗口大小，保证中间窗口移动效果
	
	/***************************设置皮肤色*****************************/
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	/***************************设置默认字体**********************************/
	_CreateXBF();
	GUI_UC_SetEncodeUTF8();
  GUI_SetDefaultFont(&XBF_Font);
	/*************************************************************************/
	
	WM_MOTION_Enable(1);
	WM_MOTION_SetDefaultPeriod(1000);
	WM_SetCallback(WM_HBKWIN, _cbBkWindow);
	
	/* 创建三个窗口 状态栏、控制栏、主窗口*/
	CreateMidWin();
	CreateTopWin();
	CreateBotWin();
	GUI_Delay(10);
	LCD_BK_EN;
	while(1)
	{
		GUI_Delay(5);
	}
}
