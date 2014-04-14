
// OTPWriterPro.cpp : 定义应用程序的类行为。
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


// COTPWriterProApp 构造

COTPWriterProApp::COTPWriterProApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 COTPWriterProApp 对象

COTPWriterProApp theApp;


// COTPWriterProApp 初始化

BOOL COTPWriterProApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	COTPWriterProDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
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
