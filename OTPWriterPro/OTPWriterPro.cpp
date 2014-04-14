
// OTPWriterPro.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "OTPWriterProDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COTPWriterProApp

BEGIN_MESSAGE_MAP(COTPWriterProApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// COTPWriterProApp ����

COTPWriterProApp::COTPWriterProApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� COTPWriterProApp ����

COTPWriterProApp theApp;


// COTPWriterProApp ��ʼ��

BOOL COTPWriterProApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	COTPWriterProDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
// Globals
BOOL g_bCancel = FALSE;
CMutex g_mutex;

CHgzMem g_mem;
CProgressCtrl *g_pctrlProgress;

//UINT ThreadFunc_Read(LPVOID lpParam)
//{
//    g_mutex.Lock();
//    g_bCancel = FALSE;
//
//    // Open the device using the VID, PID, and optionally the Serial number.
//    hid_device *handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
//    if (!handle) {
//        _tprintf(_T("Unable to open device\n"));
//        g_mutex.Unlock();
//        return;
//    }
//
//    HID_REPORT_t r;
//    MemRead(handle, r, GetStartAddress(), GetDataLength());
//
//    hid_close(handle); /* Free handle objects. */
//    hid_exit(); /* Free static HIDAPI objects. */
//
//    UpdateBufferDisplay(GetStartAddress(), GetDataLength());
//
//    g_mutex.Unlock();
//}
