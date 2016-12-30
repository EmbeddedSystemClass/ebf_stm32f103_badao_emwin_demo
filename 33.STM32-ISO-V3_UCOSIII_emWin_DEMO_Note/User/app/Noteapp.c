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
#define TXT_NAME_LEN 	30			//文件名长度，如果检测到文件名超过50 则丢弃这个文件 
#define NOTE_NAME_LEN 	20			//LCD显示的文件名的最大长度
#define NOTE_NUM        10
#define _DF1S	0x81
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
static WM_HWIN hname,hsave,hedit,hlist;
static uint8_t  txtfile_num = 0;
static unsigned char  txtlist[NOTE_NUM][TXT_NAME_LEN]={0};
static unsigned char notelist[NOTE_NUM][NOTE_NAME_LEN]={0};
static char txtpath[TXT_NAME_LEN]="0:/Note";	
static char newname[TXT_NAME_LEN]={0};
static uint8_t note_index=0;
static 	uint16_t messagesnote[2];
static uint16_t *buffer=0;
static char *txtBuffer=0;

extern FIL	file;							//文件句柄
extern FRESULT result;						//文件系统API函数返回值
extern UINT bw;			//已读或已写的字节数

/************ KeyInput **************/
extern OS_TCB   AppTaskKeypadTCB;
extern uint8_t  keyflag;
extern WM_HWIN  KEYBOARD;
/************************************/

extern GUI_FONT     XBF_Font;
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateNote[] = {
  { FRAMEWIN_CreateIndirect, "Note", 0, 0, 0, 240, 320, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "name", GUI_ID_EDIT0, 0, 0, 170, 20, 0, 0x1e, 0 },
  { BUTTON_CreateIndirect, "save", GUI_ID_BUTTON0, 180, 0, 40, 20, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "edit", GUI_ID_MULTIEDIT0, 0, 22, 230, 273, 0, 0x0, 0 },
  { LISTBOX_CreateIndirect, "Notelist", GUI_ID_LISTBOX0, 0, 0, 230, 295, 0, 0x0, 0 },
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
static uint8_t  txt2buffer(char * sFilename) 
{
	OS_ERR      	err;
	uint32_t j=0,k=0;
	uint16_t gbkdata=0;
	buffer = (uint16_t *)(0x680c0000);
	txtBuffer = (char *)(0x680c0000+1024*50*2);
	OSSchedLock(&err);
	
	result = f_open(&file, sFilename, FA_READ);
	if (result != FR_OK)
	{
		OSSchedUnlock(&err);
		return 0;
	}
	//printf("filesize=%d\n",file.fsize);
	if(file.fsize>=1024*50*2)
	{
		printf("Note read fail!!!\n");
		OSSchedUnlock(&err);
		return 0;
  	}
	result = f_read(&file, txtBuffer, file.fsize, &bw);
	if (result != FR_OK)
  	{
		OSSchedUnlock(&err);
		return 0;
  	}
	
	for(j=0,k=0;(j<file.fsize)&&txtBuffer[j]!='\0';k++)
	{
		if((uint8_t)txtBuffer[j]>0x80)
		{
			gbkdata=txtBuffer[j+1]+txtBuffer[j]*256;
			buffer[k]=ff_convert(gbkdata,1);
			//printf("%d:%04X\n",j,UCname[k]);
			j+=2;
		}
		else
		{
			buffer[k]=0x00ff&txtBuffer[j];
			//printf("%d:%04X\n",j,UCname[k]);
			j+=1;
		}
		//printf("%04X",buffer[k]);
	}
	buffer[k]='\0';		
	GUI_UC_ConvertUC2UTF8(buffer,2*k+2,txtBuffer,k*3);
	f_close(&file);
	OSSchedUnlock(&err);
	return 1;
	
}
/**
  * @brief  
  * @param  path:初始扫描路径
  * @retval result:文件系统的返回值
  */
static FRESULT scan_txtfiles (char* path) 
{ 
    FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
    FILINFO fno; 
    DIR dir; 
    int i; 
    char *fn; 
	  char file_name[TXT_NAME_LEN];	
	
#if _USE_LFN 
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1]; 	//长文件名支持
    fno.lfname = lfn; 
    fno.lfsize = sizeof(lfn); 
#endif 
		txtfile_num=0;
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
                res = scan_txtfiles(path);											//递归遍历 
                if (res != FR_OK) 
					break; 																		//打开失败，跳出循环
                path[i] = 0; 
            } 
			else 
			{ 
				//printf("%s/%s\r\n", path, fn);								//输出文件名
				if(strstr(fn,".txt")||strstr(fn,".TXT"))
				{
					if (strlen(path)+strlen(fn)<TXT_NAME_LEN)
					{
						sprintf(file_name, "%s/%s", path, fn);						
						memcpy(txtlist[txtfile_num],file_name,strlen(file_name));
						memcpy(notelist[txtfile_num],fn,strlen(fn));
						txtfile_num++;//记录文件个数
						if(txtfile_num>=NOTE_NUM)break;
					}
				}//if mp3||wav
            }//else
        } //for
    } 
    return res; 
}
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialogNote(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	// USER START (Optionally insert additional variables)
	char note_name[TXT_NAME_LEN];
	uint32_t i=0,j=0,k=0;
	uint16_t UCname[TXT_NAME_LEN]={0},gbkdata=0;
	OS_ERR      err;
	// USER END

  switch (pMsg->MsgId) {
	  case WM_DELETE:
		OS_INFO("NOTEapp delete\n");
		Flag_ICON202 = 0;
		WM_DeleteWindow(hname);
		WM_DeleteWindow(hedit);
		WM_DeleteWindow(hsave);
		WM_DeleteWindow(hlist);
		txtBuffer=0;
		note_index=0;
		txtfile_num=0;
		txtBuffer=0;
		UserApp_Flag = 0;
		keyflag=0;
		WM_DeleteWindow(KEYBOARD);
	break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Note'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    FRAMEWIN_SetTitleHeight(hItem, 20);
    //
    // Initialization of 'name'
    //
    hname = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0);
    EDIT_SetFont(hname, &XBF_Font);
		EDIT_EnableBlink(hname,500,1);
		EDIT_SetBkColor(hname,EDIT_CI_ENABLED,GUI_WHITE);
		//EDIT_SetFont(hname, GUI_FONT_16B_ASCII);
		WM_HideWindow(hname);
    //
    // Initialization of 'save'
    //
    hsave = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
    BUTTON_SetFont(hsave, GUI_FONT_13B_ASCII);
		WM_HideWindow(hsave);
    //
    // Initialization of 'edit'
    //
    hedit = WM_GetDialogItem(pMsg->hWin, GUI_ID_MULTIEDIT0);
		MULTIEDIT_SetFont(hedit,&XBF_Font);
		MULTIEDIT_EnableBlink(hedit,500,1);
		MULTIEDIT_SetAutoScrollV(hedit,1);
		MULTIEDIT_SetInsertMode(hedit,1);
		//MULTIEDIT_SetReadOnly(hedit,1);
		MULTIEDIT_SetWrapWord(hedit);
		MULTIEDIT_SetBkColor(hedit,MULTIEDIT_CI_EDIT,APPBKCOLOR);
		WM_HideWindow(hedit);
    //
    // Initialization of 'Notelist'
    //
    hlist = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTBOX0);
    LISTBOX_SetMulti(hlist,0);
    LISTBOX_SetFont(hlist, &XBF_Font);
		LISTBOX_SetAutoScrollV(hlist,1);
		LISTVIEW_SetBkColor(hlist,LISTVIEW_CI_SEL,APPBKCOLOR);
    // USER START (Optionally insert additional code for further widget initialization)
		scan_txtfiles(txtpath);
		if(txtfile_num)
		{		
			for(i=0;i<txtfile_num;i++)	
			{
				for(j=0,k=0;(j<NOTE_NAME_LEN)&&notelist[i][j]!='\0';k++)
				{
					if(notelist[i][j]>0x80)
					{
						gbkdata=notelist[i][j+1]+notelist[i][j]*256;
						UCname[k]=ff_convert(gbkdata,1);
						//printf("%d:%04X\n",j,UCname[k]);
						j+=2;
					}
					else
					{
						UCname[k]=0x00ff&notelist[i][j];
						//printf("%d:%04X\n",j,UCname[k]);
						j+=1;
					}
				}
				UCname[k]='\0';
				//printf("%d.%s\n",i+1,notelist[i]);			
				GUI_UC_ConvertUC2UTF8(UCname,2*k+2,note_name,3*k);
				LISTBOX_AddString(hlist,note_name);
			}
		}
		LISTBOX_AddString(hlist,"new...");
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case GUI_ID_EDIT0: // Notifications sent by 'name'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				messagesnote[0]=WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0);
				messagesnote[1]=GUI_ID_EDIT0;
				if(!keyflag)OSTaskQPost(&AppTaskKeypadTCB,
							(void *)&messagesnote,
							2,
							OS_OPT_POST_FIFO,
							&err);
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case GUI_ID_BUTTON0: // Notifications sent by 'save'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				if(MULTIEDIT_GetTextSize(hedit)>1024*50)break;
				EDIT_GetText(hname,newname,30);
				GUI_UC_ConvertUTF82UC(newname,30,UCname,30);
				i=0;
				j=0;
				while(UCname[i]!='\0'&&j<30)
				{
					if(UCname[i]>0x80)
					{
						gbkdata=ff_convert(UCname[i],0);
						newname[j]=(char)(gbkdata>>8);
						newname[j+1]=(char)gbkdata;
						j+=2;
					}
					else
					{
						newname[j]=(char)UCname[i];
						j+=1;
					}
					i++;
				}
				newname[j]='\0';
				result=f_open(&file,newname,  FA_CREATE_ALWAYS | FA_WRITE  );
				buffer = (uint16_t *)(0x680c0000);
				txtBuffer = (char *)(0x680c0000+1024*50*2);		
				if ( result == FR_OK )
				{
					OSSchedLock(&err);
					MULTIEDIT_GetText(hedit,txtBuffer,MULTIEDIT_GetTextSize(hedit));
					k=MULTIEDIT_GetTextSize(hedit);
					GUI_UC_ConvertUTF82UC(txtBuffer,k,buffer,k);
					i=0;
					j=0;
					while(buffer[i]!='\0'&&j<k)
					{
						if(buffer[i]>0x80)
						{
							gbkdata=ff_convert(buffer[i],0);
							txtBuffer[j]=(char)(gbkdata>>8);
							txtBuffer[j+1]=(char)gbkdata;
							j+=2;
						}
						else
						{
							txtBuffer[j]=(char)buffer[i];
							j+=1;
						}
						i++;
					}
					txtBuffer[j]='\0';
					result = f_write(&file, txtBuffer, k, &bw);
					//printf("filesize=%d,writesize=%d\n",k,bw);
					f_close(&file);  
					OSSchedUnlock(&err);
				}
				buffer =0;
				txtBuffer =0;				
				MULTIEDIT_SetText(hedit,'\0');
				
				WM_HideWindow(hname);
				WM_HideWindow(hedit);
				WM_HideWindow(hsave);
				WM_ShowWindow(hlist);
				//如果是新建文件保存完重新加载列表
				if(txtfile_num==0||note_index==txtfile_num)
				{
					i=txtfile_num+1;
					while(i)
					{
						LISTBOX_DeleteItem(hlist,i-1);
						i--;
					}
					scan_txtfiles(txtpath);
					if(txtfile_num)
					{		
						for(i=0;i<txtfile_num;i++)	
						{
							for(j=0,k=0;(j<NOTE_NAME_LEN)&&notelist[i][j]!='\0';k++)
							{
								if(notelist[i][j]>0x80)
								{
									gbkdata=notelist[i][j+1]+notelist[i][j]*256;
									UCname[k]=ff_convert(gbkdata,1);
									//printf("%d:%04X\n",j,UCname[k]);
									j+=2;
								}
								else
								{
									UCname[k]=0x00ff&notelist[i][j];
									//printf("%d:%04X\n",j,UCname[k]);
									j+=1;
								}
							}
							UCname[k]='\0';
							//printf("%d.%s\n",i+1,notelist[i]);			
							GUI_UC_ConvertUC2UTF8(UCname,2*k+2,note_name,3*k);
							LISTBOX_AddString(hlist,note_name);
						}
					}
					LISTBOX_AddString(hlist,"new...");
				}				
				
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case GUI_ID_MULTIEDIT0: // Notifications sent by 'edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
		messagesnote[0]=WM_GetDialogItem(pMsg->hWin, GUI_ID_MULTIEDIT0);
		messagesnote[1]=GUI_ID_MULTIEDIT0;
		if(!keyflag)OSTaskQPost(&AppTaskKeypadTCB,
					(void *)&messagesnote,
					2,
					OS_OPT_POST_FIFO,
					&err);
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case GUI_ID_LISTBOX0: // Notifications sent by 'Notelist'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				note_index=LISTBOX_GetSel(hlist);
				if(txtfile_num==0||note_index==txtfile_num)
				{
					sprintf(note_name,"%s","0:/Note/new.txt");
					txtBuffer=0;
				}
				else
				{
					//sprintf(note_name,"%s",notelist[note_index]);
					for(j=0,k=0;(j<TXT_NAME_LEN)&&txtlist[note_index][j]!='\0';k++)
					{
						if(txtlist[note_index][j]>0x80)
						{
							gbkdata=txtlist[note_index][j+1]+txtlist[note_index][j]*256;
							UCname[k]=ff_convert(gbkdata,1);
							j+=2;
						}
						else
						{
							UCname[k]=0x00ff&txtlist[note_index][j];
							j+=1;
						}
					}
					UCname[k]='\0';		
					GUI_UC_ConvertUC2UTF8(UCname,2*k+2,note_name,3*k);
					txt2buffer(txtlist[note_index]);
				}
				WM_HideWindow(hlist);
				WM_ShowWindow(hedit);
				WM_ShowWindow(hname);
				WM_ShowWindow(hsave);
				WM_HasFocus(hname);
				EDIT_SetText(hname,note_name);
				WM_HasFocus(hedit);
				MULTIEDIT_AddText(hedit,txtBuffer);
				// USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
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
*       CreateNote
*/
void FUN_ICON202Clicked(void)
{
	DIR recdir;	 					//目录
	OS_INFO("NOTEapp create\n");
	while(result!=FR_OK)
	{
		f_mkdir("0:/Note");				//创建该目录 
		result=f_opendir(&recdir,"0:/Note");
	}
//	f_closedir(&recdir);
	GUI_CreateDialogBox(_aDialogCreateNote, GUI_COUNTOF(_aDialogCreateNote), _cbDialogNote, WM_HBKWIN, 0, 0);
	while(Flag_ICON202)
	{
		GUI_Delay(10); 
	}
}
// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/

