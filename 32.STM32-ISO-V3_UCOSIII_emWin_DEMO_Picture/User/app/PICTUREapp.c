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

/**************************************************************************************
*
*       Defines
*
***************************************************************************************
*/

// USER START (Optionally insert additional defines)
#define g_ucPicNum    8
#define pathlenth	  40
#define _DF1S	0x81
#define imageWinX  230
#define imageWinY  270
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
static char BMP_Name[g_ucPicNum][pathlenth];
static int8_t BMP_Num=0;
static int8_t BMP_Index=0;

static char JPG_Name[g_ucPicNum][pathlenth];
static int8_t JPG_Num=0;
static int8_t JPG_Index=0;

static char GIF_Name[g_ucPicNum][pathlenth];
static int8_t GIF_Num=0;
static int8_t GIF_Index=0;
static uint8_t g_ucPicType = 0;

static char path[pathlenth*2]="0:/Picture";
extern FIL	file;							//文件句柄
extern FRESULT result;						//文件系统API函数返回值
extern UINT bw;			//已读或已写的字节数

char *_acBuffer;
GUI_GIF_INFO InfoGif1;
GUI_GIF_IMAGE_INFO InfoGif2;
GUI_JPEG_INFO JpegInfo;
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePICTURE[] = {
  { FRAMEWIN_CreateIndirect, "PICTURE", 0, 0, 0, 240, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "PREVIOUS", GUI_ID_BUTTON0, 5, 272, 40, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "NEXT", GUI_ID_BUTTON1, 185, 272, 40, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "TYPE", GUI_ID_BUTTON2, 95, 272, 40, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ZOOM IN", GUI_ID_BUTTON3, 55, 272, 30, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "ZOOM OUT", GUI_ID_BUTTON4, 145, 272, 30, 20, 0, 0x0, 0 },
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
/**
  * @brief  scan_files 递归扫描sd卡内的歌曲文件
  * @param  path:初始扫描路径
  * @retval result:文件系统的返回值
  */
static FRESULT scan_files (char* path) 
{ 
    FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
    FILINFO fno; 
    DIR dir; 
    int i; 
    char *fn; 
	char file_name[pathlenth];	
#if _USE_LFN 
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1]; 	//长文件名支持
    fno.lfname = lfn; 
    fno.lfsize = sizeof(lfn); 
#endif 
	BMP_Num=0;
	JPG_Num=0;
	GIF_Num=0;
	
    res = f_opendir(&dir, path); //打开目录
    if (res == FR_OK) 
	{ 
        i = strlen(path); 
        for (;;) 
		{ 
            res = f_readdir(&dir, &fno); 										//读取目录下的内容
            if (res != FR_OK || fno.fname[0] == 0) break; 	//为空时表示所有项目读取完毕，跳出
#if _USE_LFN 
            fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
            fn = fno.fname; 
#endif 
            if (*fn == '.') continue; 											//点表示当前目录，跳过			
            if (fno.fattrib & AM_DIR) 
			{ 																							//目录，递归读取
                sprintf(&path[i], "/%s", fn); 							//合成完整目录名
                res = scan_files(path);											//递归遍历 
                if (res != FR_OK) 
					break; 																		//打开失败，跳出循环
                path[i] = 0; 
            } 
			else 
			{ 
				//printf("%s/%s\r\n", path, fn);								//输出文件名
				if(strstr(fn,".BMP")||strstr(fn,".bmp"))//判断是否mp3或wav文件
				{
					if (strlen(path)+strlen(fn)<pathlenth)
					{
						sprintf(file_name, "%s/%s", path, fn);						
						memcpy(BMP_Name[BMP_Num],file_name,strlen(file_name));					
						BMP_Num++;//记录文件个数
						if(BMP_Num>=g_ucPicNum)BMP_Num=g_ucPicNum-1;
					}
				}
				if(strstr(fn,".JPG")||strstr(fn,".jpg"))//判断是否mp3或wav文件
				{
					if (strlen(path)+strlen(fn)<pathlenth)
					{
						sprintf(file_name, "%s/%s", path, fn);						
						memcpy(JPG_Name[JPG_Num],file_name,strlen(file_name));					
						JPG_Num++;//记录文件个数
						if(JPG_Num>=g_ucPicNum)JPG_Num=g_ucPicNum-1;
					}
				}
				if(strstr(fn,".GIF")||strstr(fn,".gif"))//判断是否mp3或wav文件
				{
					if (strlen(path)+strlen(fn)<pathlenth)
					{
						sprintf(file_name, "%s/%s", path, fn);						
						memcpy(GIF_Name[GIF_Num],file_name,strlen(file_name));					
						GIF_Num++;//记录文件个数
						if(GIF_Num>=g_ucPicNum)GIF_Num=g_ucPicNum-1;
					}
				}
            }//else
        } //for
    } 
    return res; 
} 
// USER START (Optionally insert additional static code)
static uint8_t  _ShowBMP(const char * sFilename) 
{
	OS_ERR      	err;

	_acBuffer = (char *)(0x680c0000);
	
	OSSchedLock(&err);
	
	result = f_open(&file, sFilename, FA_READ);
	if (result != FR_OK)
	{
		OSSchedUnlock(&err);
		return 0;
	}
	result = f_read(&file, _acBuffer, file.fsize, &bw);
	if (result != FR_OK)
  	{
		OSSchedUnlock(&err);
		return 0;
  	}
    f_close(&file);
	
	OSSchedUnlock(&err);

	return 1;
	
}
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialogPICTURE(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  OS_ERR      	err;
  // USER START (Optionally insert additional variables)
	static int bmp_amred = 100;
    static int jpg_amred = 100;
  // USER END

  switch (pMsg->MsgId) {
	case WM_DELETE:
		OS_INFO("PICTUREapp delete\n");
		Flag_ICON201 = 0;
		BMP_Index=0;
		JPG_Index=0;
		GIF_Index=0;
		UserApp_Flag = 0;
	WM_InvalidateWindow(WM_HBKWIN);
	break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'PICTURE'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
	FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    FRAMEWIN_SetTitleHeight(hItem, 20);
    //
    // Initialization of 'PREVIOUS'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
	BUTTON_SetText(hItem,"<<<");
    //
    // Initialization of 'NEXT'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
	BUTTON_SetText(hItem,">>>");
	//
    // Initialization of 'TYPE'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
	BUTTON_SetText(hItem,"BMP");
	//
    // Initialization of 'ZOOM IN'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
	BUTTON_SetText(hItem,"++");
	//
    // Initialization of 'ZOOM OUT'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON4);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
	BUTTON_SetText(hItem,"--");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case GUI_ID_BUTTON0: // Notifications sent by 'BACK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
		GUI_SetBkColor(GUI_LIGHTGRAY);
		GUI_Clear();
		if(g_ucPicType == 0)
		{
			BMP_Index--;
			if(BMP_Index < 0)BMP_Index=BMP_Num-1;
			_ShowBMP(BMP_Name[BMP_Index]);
			OSSchedLock(&err);			
			GUI_BMP_Draw(_acBuffer,
						 (imageWinX - GUI_BMP_GetXSize(_acBuffer))/2, 
						 (imageWinY - GUI_BMP_GetYSize(_acBuffer))/2);
			OSSchedUnlock(&err);																	
		}
		else if(g_ucPicType == 1)
		{
			JPG_Index--;
			if(JPG_Index < 0)JPG_Index=JPG_Num-1;
			_ShowBMP(JPG_Name[JPG_Index]);
			OSSchedLock(&err);
			GUI_JPEG_GetInfo(_acBuffer, file.fsize, &JpegInfo);
			
			GUI_JPEG_Draw(_acBuffer,
						  file.fsize, 
						  (imageWinX - JpegInfo.XSize)/2, 
						  (imageWinY - JpegInfo.YSize)/2);
			OSSchedUnlock(&err);						
		}
		else
		{
			GIF_Index--;
			if(GIF_Index < 0)GIF_Index=GIF_Num-1;
			_ShowBMP(GIF_Name[GIF_Index]);
			GUI_GIF_GetInfo(_acBuffer, file.fsize, &InfoGif1);
		}
		//IMAGE_SetBMP(WM_GetDialogItem(pMsg->hWin, GUI_ID_IMAGE0),);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case GUI_ID_BUTTON1: // Notifications sent by 'NEXT'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
		GUI_SetBkColor(GUI_LIGHTGRAY);
		GUI_Clear();
		if(g_ucPicType == 0)
		{
			BMP_Index++;
			if(BMP_Index >= BMP_Num)BMP_Index=0;

			_ShowBMP(BMP_Name[BMP_Index]);
			
			OSSchedLock(&err);
			GUI_BMP_Draw(_acBuffer,
						 (imageWinX - GUI_BMP_GetXSize(_acBuffer))/2, 
						 (imageWinY - GUI_BMP_GetYSize(_acBuffer))/2);
			OSSchedUnlock(&err);																	
		}
		else if(g_ucPicType == 1)
		{
			JPG_Index++;
			if(JPG_Index >= JPG_Num)JPG_Index=0;
			_ShowBMP(JPG_Name[JPG_Index]);

			OSSchedLock(&err);
			GUI_JPEG_GetInfo(_acBuffer, file.fsize, &JpegInfo);
			GUI_JPEG_Draw(_acBuffer,
						  file.fsize, 
						  (imageWinX - JpegInfo.XSize)/2, 
						  (imageWinY - JpegInfo.YSize)/2);
			OSSchedUnlock(&err);						
		}
		else
		{
			GIF_Index++;
			if(GIF_Index >= GIF_Num)GIF_Index=0;
			_ShowBMP(GIF_Name[GIF_Index]);
			GUI_GIF_GetInfo(_acBuffer, file.fsize, &InfoGif1);
		}
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
	  case GUI_ID_BUTTON2: // Notifications sent by 'BACK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
		if(g_ucPicType == 0)
		{
			g_ucPicType = 1;
			BUTTON_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2), "JPG");							
		}
		else if(g_ucPicType == 1)
		{
			g_ucPicType = 2;
			GUI_SetBkColor(GUI_LIGHTGRAY);
			GUI_Clear();
			_ShowBMP(GIF_Name[GIF_Index]);
			GUI_GIF_GetInfo(_acBuffer, file.fsize, &InfoGif1);
			BUTTON_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2), "GIF");
		}
		else
		{
			g_ucPicType = 0;
			BUTTON_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2), "BMP");    
		}
		//IMAGE_SetBMP(WM_GetDialogItem(pMsg->hWin, GUI_ID_IMAGE0),);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
	  case GUI_ID_BUTTON3: // Notifications sent by 'ZOOM IN'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
		if(g_ucPicType == 0)
		{
			bmp_amred += 20;
			OSSchedLock(&err);
			GUI_BMP_DrawScaled(_acBuffer,
								(imageWinX - GUI_BMP_GetXSize(_acBuffer)*bmp_amred/100)/2, 
								(imageWinY - GUI_BMP_GetYSize(_acBuffer)*bmp_amred/100)/2,
								bmp_amred, 
								100);
			OSSchedUnlock(&err);																	
		}
		else if(g_ucPicType == 1)
		{
			jpg_amred +=20;
			OSSchedLock(&err);
			GUI_JPEG_DrawScaled(_acBuffer,
								file.fsize, 
								(imageWinX - JpegInfo.XSize*jpg_amred/100)/2, 
								(imageWinY - JpegInfo.YSize*jpg_amred/100)/2,
								jpg_amred,
                                100); 
			OSSchedUnlock(&err);						
		}
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
	  case GUI_ID_BUTTON4: // Notifications sent by 'ZOOM OUT'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
		GUI_SetBkColor(GUI_LIGHTGRAY);
		GUI_Clear();
		if(g_ucPicType == 0)
		{
			bmp_amred -= 10;
            if(bmp_amred < 0) bmp_amred = 10; 
			OSSchedLock(&err);
			GUI_BMP_DrawScaled(_acBuffer,
								(imageWinX - GUI_BMP_GetXSize(_acBuffer)*bmp_amred/100)/2, 
								(imageWinY - GUI_BMP_GetYSize(_acBuffer)*bmp_amred/100)/2,
								bmp_amred, 
								100);
			OSSchedUnlock(&err);																	
		}
		else if(g_ucPicType == 1)
		{
			jpg_amred -= 10;
            if(jpg_amred < 0) jpg_amred = 10;
			OSSchedLock(&err);
			GUI_JPEG_DrawScaled(_acBuffer,
								file.fsize, 
								(imageWinX - JpegInfo.XSize*jpg_amred/100)/2, 
								(imageWinY - JpegInfo.YSize*jpg_amred/100)/2,
								jpg_amred,
                                100); 
			OSSchedUnlock(&err);						
		}
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
  // USER START (Optionally insert additional message handling)
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
*       CreatePICTURE
*/
void FUN_ICON201Clicked(void)
{
	WM_HWIN hWin;
	WM_HWIN himage;
	uint8_t i=0;
	OS_ERR  err;
	DIR recdir;	 					//目录
	g_ucPicType = 0;
	OS_INFO("PICTUREapp create\n");
	result=f_opendir(&recdir,"0:/Picture");
	while(result!=FR_OK)
	{
		f_mkdir("0:/Picture");				//创建该目录 
		result=f_opendir(&recdir,"0:/Picture");
	}
//	f_closedir(&recdir);
	
	hWin=GUI_CreateDialogBox(_aDialogCreatePICTURE, GUI_COUNTOF(_aDialogCreatePICTURE), _cbDialogPICTURE, WM_HBKWIN, 0, 0);
	himage=WM_CreateWindowAsChild(5,20,imageWinX,imageWinY,hWin,WM_CF_SHOW,NULL,0);
	WM_SelectWindow(himage);
	GUI_SetBkColor(GUI_WHITE);//GUI_SetBkColor(GUI_LIGHTGRAY);
	GUI_Clear();
	scan_files(path);
	if(BMP_Num)
	{
		_ShowBMP(BMP_Name[BMP_Index]);		
		OSSchedLock(&err);
		GUI_BMP_Draw(_acBuffer,
					(imageWinX - GUI_BMP_GetXSize(_acBuffer))/2, 
					(imageWinY - GUI_BMP_GetYSize(_acBuffer))/2);
		OSSchedUnlock(&err);
	}
	while(Flag_ICON201)
	{
		if(g_ucPicType == 2)
		{
    		if(i < InfoGif1.NumImages)
    	    {                                    
    			OSSchedLock(&err);
    	        //GUI_GIF_GetImageInfo(_acBuffer2, file.fsize, &InfoGif2, i);
                GUI_GIF_DrawSub(_acBuffer, 
                                file.fsize, 
                                (imageWinX - InfoGif1.xSize)/2, 
								(imageWinY - InfoGif1.ySize)/2, 
                                i++);                             
    	        OSSchedUnlock(&err);
                if(InfoGif2.Delay == 0)
                {
                    GUI_Delay(100);
                }
                else
                {
                    GUI_Delay(InfoGif2.Delay*10);
                }                          						 
    	    }
    	    else
    	    {
    	        i = 0;
    	    }
		}
		else
		{
			GUI_Delay(10); 
		}
	}
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
