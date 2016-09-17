
// OTPWriterPro.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "OTPWriterProDlg.h"
#include "hgz/hgz.h"

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


// 
void print(int pos, const TCHAR *szFormat, ...)
{
	// pos Ϊ��������λ�õ��ַ���

	va_list arglist;
	va_start(arglist, szFormat);
	TCHAR *buf = new TCHAR[_vsctprintf(szFormat, arglist) + 1];
	//TCHAR *buf = (TCHAR *)_alloca((_vsctprintf(szFormat, arglist)+1)*sizeof(TCHAR)); // ��̬����ջ�ڴ棬�����ֶ��ͷš�
	_vstprintf(buf, szFormat, arglist);

	CEdit &edit = (reinterpret_cast<COTPWriterProDlg*>(AfxGetApp()->GetMainWnd()))->m_ctrlEdit;
	int len = edit.GetWindowTextLength();
	edit.SetSel(len + pos, len); //�������������� 
	edit.ReplaceSel(buf);
	//m_ctrlEdit.ScrollWindow(0,0); //����������� (������ʼ���ڵײ���������)
	delete[] buf;
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
	SetRegistryKey(_T("HunterSun\\OTPWriterPro"));

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
void GlobalInit()
{
	g_ChipType = HS__CMD__CHIP_TYPE__NONE;

	g_hs6210_rollnum_full_path = GetModuleDir() + _T("\\ChipType\\HS6210\\rollnum.txt");
	g_hs6210_rf_syncode_full_path = GetModuleDir() + _T("\\ChipType\\HS6210\\common\\RFAddr\\AddrMode1-150302-2013-repeat-f000.txt");
	
}

bool IsChipType_HS6210() {
	return g_ChipType == HS__CMD__CHIP_TYPE__OTP__HS6210;
}
//////////////////////////////////////////////////////////////////////////////
// Globals
BOOL g_bCancel = FALSE;
CMutex g_mutex;
HS_CHIP_TYPE_t g_ChipType;
CString g_hs6210_rollnum_full_path = _T("");
CString g_hs6210_rf_syncode_full_path = _T("");

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
