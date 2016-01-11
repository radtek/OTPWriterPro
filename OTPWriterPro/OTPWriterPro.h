
// OTPWriterPro.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "Packet.h"
#include "HgzMem.h"

// COTPWriterProApp:
// �йش����ʵ�֣������ OTPWriterPro.cpp
//

class COTPWriterProApp : public CWinApp
{
public:
	COTPWriterProApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

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
