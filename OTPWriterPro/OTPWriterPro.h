
// OTPWriterPro.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "Packet.h"
#include "HgzMem.h"

// COTPWriterProApp:
// 有关此类的实现，请参阅 OTPWriterPro.cpp
//

class COTPWriterProApp : public CWinApp
{
public:
	COTPWriterProApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////
// Globals

extern COTPWriterProApp theApp;
void print(int pos, const TCHAR *szFormat, ...);

extern BOOL g_bCancel/* = FALSE*/;
extern CMutex g_mutex;

typedef struct {
    unsigned char reportID;
    CPacket packet;
} HID_REPORT_t;


extern CHgzMem g_mem;
extern CProgressCtrl *g_pctrlProgress;
