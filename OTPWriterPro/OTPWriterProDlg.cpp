
// OTPWriterProDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "OTPWriterProDlg.h"
#include "afxdialogex.h"
//#include <iostream>
//#include <iomanip>
//#include <fstream>
#include ".\hgz\hgz.h"
#include <string>
#include "hgz/HgzString.h"
#include "HgzMD5.h"
#include "DlgTools.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




////////////////////////////////////////////////////////////////////////////////
	
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COTPWriterProDlg 对话框




COTPWriterProDlg::COTPWriterProDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COTPWriterProDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COTPWriterProDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON8, m_ctrlErase);
    DDX_Control(pDX, IDC_EDIT2, m_ctrlErasePageNum);
    DDX_Control(pDX, IDC_CHECK1, m_ctrlEraseAll);
    DDX_Control(pDX, IDC_EDIT1, m_ctrlEdit);
    DDX_Control(pDX, IDC_COMBO1, m_ctrlChipSel);
    DDX_Control(pDX, IDC_COMBO3, m_ctrlMemAddrBegin);
    DDX_Control(pDX, IDC_COMBO4, m_ctrlDataLength);
    DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
    DDX_Control(pDX, IDC_LIST1, m_ctrlListBuffer);
    DDX_Control(pDX, IDC_COMBO6, m_cbDataToFill);
    DDX_Control(pDX, IDC_CHECK4, m_chkFillBufferAll);
    DDX_Control(pDX, IDC_CHECK_LENGTH_HEX, m_chkDataLen);
    DDX_Control(pDX, IDC_COMBO8, m_ctrlIgnoreMemBegin);
    DDX_Control(pDX, IDC_COMBO9, m_ctrlIgnoreMemEnd);
    DDX_Control(pDX, IDC_CHECK3, m_ctrlIgnoreMem);
    DDX_Control(pDX, IDC_BUTTON6, m_ctrlWrite);
    DDX_Control(pDX, IDC_BUTTON10, m_ctrlEncrypt);
    DDX_Control(pDX, IDC_BUTTON1, m_ctrlAuto);
    DDX_Control(pDX, IDC_COMBO7, m_ctrlBufferSearch);
    DDX_Control(pDX, IDC_SPLIT1, m_ctrlRollnum);
    DDX_Control(pDX, IDC_SPLIT2, m_sbn_Find);
    DDX_Control(pDX, IDC_SPLIT3, m_sbn_Fill);
}

BEGIN_MESSAGE_MAP(COTPWriterProDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON7, &COTPWriterProDlg::OnBnClickedButtonRead)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON6, &COTPWriterProDlg::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON4, &COTPWriterProDlg::OnBnClickedButtonOpenFile)
	ON_BN_CLICKED(IDC_BUTTON5, &COTPWriterProDlg::OnBnClickedButtonSaveAs)
	ON_BN_CLICKED(IDC_BUTTON9, &COTPWriterProDlg::OnBnClickedButtonVerify)
	ON_BN_CLICKED(IDC_BUTTON10, &COTPWriterProDlg::OnBnClickedButtonEncrypt)
	ON_BN_CLICKED(IDC_BUTTON8, &COTPWriterProDlg::OnBnClickedButtonErase)
	ON_MESSAGE(WM_LVM_ITEMCHANGED, &COTPWriterProDlg::OnLvmItemChanged)
	ON_BN_CLICKED(IDC_BUTTON11, &COTPWriterProDlg::OnBnClickedButtonBlankCheck)
	ON_BN_CLICKED(IDC_BUTTON16, &COTPWriterProDlg::OnBnClickedButtonEnterProgramMode)
	ON_BN_CLICKED(IDC_BUTTON17, &COTPWriterProDlg::OnBnClickedButtonExitProgramMode)
	ON_BN_CLICKED(IDC_BUTTON12, &COTPWriterProDlg::OnBnClickedButtonTestBlankCheck)
	ON_BN_CLICKED(IDC_BUTTON13, &COTPWriterProDlg::OnBnClickedButtonTestDec)
	ON_BN_CLICKED(IDC_BUTTON14, &COTPWriterProDlg::OnBnClickedButtonTestWR)
	ON_BN_CLICKED(IDC_BUTTON15, &COTPWriterProDlg::OnBnClickedButtonOtpSpiReset)
//	ON_BN_CLICKED(IDC_BUTTON18, &COTPWriterProDlg::OnBnClickedButtonFillInBuffer1)
    ON_BN_CLICKED(IDC_BUTTON19, &COTPWriterProDlg::OnBnClickedButtonOption)
//    ON_BN_CLICKED(IDC_BUTTON20, &COTPWriterProDlg::OnBnClickedButtonClearBuffer1)
    ON_CBN_SELCHANGE(IDC_COMBO1, &COTPWriterProDlg::OnCbnSelchangeComboSelectChipType)
    ON_BN_CLICKED(IDC_BUTTON3, &COTPWriterProDlg::OnBnClickedButtonDetectChipType)
    ON_BN_CLICKED(IDC_BUTTON21, &COTPWriterProDlg::OnBnClickedButtonVersionNum)
    ON_BN_CLICKED(IDC_CHECK_LENGTH_HEX, &COTPWriterProDlg::OnBnClickedCheckLengthHex)
    ON_BN_CLICKED(IDC_BUTTON1, &COTPWriterProDlg::OnBnClickedButtonAuto)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &COTPWriterProDlg::OnDeltaposSpin1)
    ON_BN_CLICKED(IDC_BUTTON2, &COTPWriterProDlg::OnBnClickedButtonBufferDataProfile)
    ON_BN_CLICKED(IDC_BUTTON23, &COTPWriterProDlg::OnBnClickedButton23)
    ON_COMMAND(ID_32772, &COTPWriterProDlg::OnRollnumAndCPConfig)
    ON_COMMAND(ID_32771, &COTPWriterProDlg::OnRollnumEnable)
    ON_UPDATE_COMMAND_UI(ID_32771, &COTPWriterProDlg::OnUpdateRollnumEnable)
    ON_WM_INITMENUPOPUP()
    ON_BN_CLICKED(IDC_SPLIT1, &COTPWriterProDlg::OnBnClicked_RollnumEnable)
    ON_BN_CLICKED(IDC_SPLIT2, &COTPWriterProDlg::OnBnClickedButtonBufferSearch)
    ON_COMMAND(ID_32774, &COTPWriterProDlg::OnMenu_ClearBufferFind)
    ON_CBN_SELCHANGE(IDC_COMBO7, &COTPWriterProDlg::OnCbnSelchangeCombo7)
//    ON_CBN_CLOSEUP(IDC_COMBO7, &COTPWriterProDlg::OnCbnCloseupCombo7)
//    ON_CBN_EDITCHANGE(IDC_COMBO7, &COTPWriterProDlg::OnCbnEditchangeCombo7)
//ON_CBN_EDITUPDATE(IDC_COMBO7, &COTPWriterProDlg::OnCbnEditupdateCombo7)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_SPLIT3, &COTPWriterProDlg::OnBnClickedButtonFillInBuffer)
ON_COMMAND(ID_32776, &COTPWriterProDlg::OnBnClickedButtonClearBuffer)
ON_CBN_SELCHANGE(IDC_COMBO6, &COTPWriterProDlg::OnCbnSelchangeCombo6)
END_MESSAGE_MAP()


// COTPWriterProDlg 消息处理程序

BOOL COTPWriterProDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CString chipType[] = { _T("None"), _T("HS6206"), _T("HS6210"), _T("EN25T80"), _T("FPGA_HS6206") };
	for (auto &var : chipType)
	{
		m_ctrlChipSel.AddString(var);
	}

	m_ctrlChipSel.SetCurSel(0);
	m_ctrlMemAddrBegin.SetWindowText(_T("0000"));
	m_ctrlDataLength.SetWindowText(_T("16384"));
    m_ctrlIgnoreMem.SetCheck(FALSE);
    m_ctrlIgnoreMemBegin.SetWindowText(_T("1FF0"));
    m_ctrlIgnoreMemEnd.SetWindowText(_T("1FFF"));

	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetPos(0);
    g_pctrlProgress = &m_ctrlProgress;

	
	
	m_ctrlListBuffer.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
    
    // set columns
    CString s;
	m_ctrlListBuffer.InsertColumn(0, _T(""), 0, 76);
	for (int i = 0; i < 16; i++)
	{
		s.Format(_T("%X"), i);
		m_ctrlListBuffer.InsertColumn(i+1, s, 0, 28);
	}
	// lock column width
	m_ctrlListBuffer.GetHeaderCtrl()->EnableWindow(FALSE);

	// set rows (items)
	m_ctrlListBuffer.SetItemCount(16*1024/16);
	for (unsigned int i = 0; i < 16*1024/16; i++) {
		s.Format(_T("%04X %04X"), (unsigned __int16)(i>>12), (unsigned __int16)(i<<4));
		m_ctrlListBuffer.InsertItem(i, s);
	}
    


    // Option
    m_Option.m_bEnableConsoleOutput = FALSE;
    m_Option.m_bWriteBufSizeReallyUsed = FALSE;
    m_Option.m_nPacketDataLength = 52;
    if (m_Option.m_bEnableConsoleOutput)	
        hgzOpenConsole();
    
    m_ChipType = HS__CMD__CHIP_TYPE__NONE;
    
	// Start a periodic read IRP to timely receive datas from device.
	//SetTimer(1, 1, NULL);
    //m_bnFind.EnableWindow(FALSE);
    //m_bnTEST1.EnableWindow(FALSE);
    //m_bnTEST2.EnableWindow(FALSE);
    //m_ctrlWrite.EnableWindow(FALSE);
    //m_ctrlErase.EnableWindow(FALSE);
    //m_ctrlEncrypt.EnableWindow(FALSE);
    
    GetDlgItem(IDC_BUTTON16)->ShowWindow(SW_HIDE); // hide button "Exit OTP SPI Mode"
    GetDlgItem(IDC_BUTTON17)->ShowWindow(SW_HIDE); // hide button "Enter OTP SPI Mode"

    ((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1))->SetRange(0, 100);

    // get rollnum settings from Windows Registry
    TCHAR stemp[1024];
    GetCurrentDirectory(1024, stemp);
    m_RollnumAndCPConfigDialog.m_CPConfigFilePath += stemp;
    s = AfxGetApp()->GetProfileString(_T("Settings"), _T("CPConfigFilePath"));
    if (s.Find(_T(":")) == -1)
        m_RollnumAndCPConfigDialog.m_CPConfigFilePath += s;
    else
        m_RollnumAndCPConfigDialog.m_CPConfigFilePath = s.GetString();

    
    m_sbn_Find.SetDropDownMenu(IDR_MENU1, 3);
    m_ctrlBufferSearch.SetCueBanner(_T("例如：02 E8"));
    m_sbn_Fill.SetDropDownMenu(IDR_MENU1, 4);

    m_ctrlRollnum.SetDropDownMenu(IDR_MENU1, 2);
    m_bRollnumEnable = AfxGetApp()->GetProfileInt(_T("Settings"), _T("EnableRollnum"), 0); // 
    if (m_bRollnumEnable)
        ProcessRollnum();

    
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COTPWriterProDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COTPWriterProDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COTPWriterProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COTPWriterProDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	hgzCloseConsole();

	CDialogEx::PostNcDestroy();
}

//--------------------------
//void COTPWriterProDlg::OnBnClickedButton1()
//{
//	// use HIDAPI
//	int res;
//	unsigned char buf[256];
//	#define MAX_STR 255
//	wchar_t wstr[MAX_STR];
//	hid_device *handle;
//	int i;
//
//	struct hid_device_info *devs, *cur_dev;
//
//	if (hid_init())
//		return;
//
//	devs = hid_enumerate(0x0, 0x0);
//	cur_dev = devs;	
//	while (cur_dev) {
//		_tprintf(_T("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls"), cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
//		_tprintf(_T("\n"));
//		_tprintf(_T("  Manufacturer: %ls\n"), cur_dev->manufacturer_string);
//		_tprintf(_T("  Product:      %ls\n"), cur_dev->product_string);
//		_tprintf(_T("  Release:      %hx\n"), cur_dev->release_number);
//		_tprintf(_T("  Interface:    %d\n"),  cur_dev->interface_number);
//		_tprintf(_T("\n"));
//		cur_dev = cur_dev->next;
//	}
//	hid_free_enumeration(devs);
//
//
//	// Open the device using the VID, PID,
//	// and optionally the Serial number.
//	////handle = hid_open(0x4d8, 0x3f, L"12345");
//	//handle = hid_open(0x4d8, 0x3f, NULL);
//	handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
//	if (!handle) {
//		_tprintf(_T("unable to open device\n"));
//		return;
//	}
//
//	// Read the Manufacturer String
//	wstr[0] = 0x0000;
//	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
//	if (res < 0)
//		_tprintf(_T("Unable to read manufacturer string\n"));
//	_tprintf(_T("Manufacturer String: %ls\n"), wstr);
//
//	// Read the Product String
//	wstr[0] = 0x0000;
//	res = hid_get_product_string(handle, wstr, MAX_STR);
//	if (res < 0)
//		_tprintf(_T("Unable to read product string\n"));
//	_tprintf(_T("Product String: %ls\n"), wstr);
//
//	// Read the Serial Number String
//	wstr[0] = 0x0000;
//	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
//	if (res < 0)
//		_tprintf(_T("Unable to read serial number string\n"));
//	_tprintf(_T("Serial Number String: (%d) %ls"), wstr[0], wstr);
//	_tprintf(_T("\n"));
//
//	// Read Indexed String 1
//	wstr[0] = 0x0000;
//	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
//	if (res < 0)
//		_tprintf(_T("Unable to read indexed string 1\n"));
//	_tprintf(_T("Indexed String 1: %ls\n"), wstr);
//
//	// Set the hid_read() function to be non-blocking.
//	hid_set_nonblocking(handle, 1);
//
//	// Set up the command buffer.
//	memset(buf,0x00,sizeof(buf));
//	buf[0] = 0x01;
//	buf[1] = 0x81;
//
//	// Try to read from the device. There shoud be no
//	// data here, but execution should not block.
//	res = hid_read(handle, buf, 17);
//
//	// Send a Feature Report to the device
//	buf[0] = 0x2;
//	buf[1] = 0xa0;
//	buf[2] = 0x0a;
//	buf[3] = 0x00;
//	buf[4] = 0x00;
//	res = hid_send_feature_report(handle, buf, 17);
//	if (res < 0) {
//		_tprintf(_T("Unable to send a feature report.\n"));
//	}
//
//	memset(buf,0,sizeof(buf));
//
//	// Read a Feature Report from the device
//	buf[0] = 0x2;
//	res = hid_get_feature_report(handle, buf, sizeof(buf));
//	if (res < 0) {
//		_tprintf(_T("Unable to get a feature report.\n"));
//		_tprintf(_T("%ls"), hid_error(handle));
//	}
//	else {
//		// Print out the returned buffer.
//		_tprintf(_T("Feature Report\n   "));
//		for (i = 0; i < res; i++)
//			_tprintf(_T("%02hhx "), buf[i]);
//		_tprintf(_T("\n"));
//	}
//
//	memset(buf,0,sizeof(buf));
//
//	// Toggle LED (cmd 0x80). The first byte is the report number (0x1).
//	buf[0] = 0x1;
//	buf[1] = 0x80;
//	res = hid_write(handle, buf, 17);
//	if (res < 0) {
//		_tprintf(_T("Unable to write()\n"));
//		_tprintf(_T("Error: %ls\n"), hid_error(handle));
//	}
//
//
//	// Request state (cmd 0x81). The first byte is the report number (0x1).
//	buf[0] = 0x1;
//	buf[1] = 0x81;
//	hid_write(handle, buf, 17);
//	if (res < 0)
//		_tprintf(_T("Unable to write() (2)\n"));
//
//	hid_close(handle);
//
//	/* Free static HIDAPI objects. */
//	hid_exit();
//
//}
//
//void COTPWriterProDlg::OnBnClickedButton2()
//{
//	// 1. 查找本系统中HID类的GUID标识
//	GUID guidHID;
//	HidD_GetHidGuid( &guidHID );
//	CString s;
//	s.Format( _T("HID 类的 GUID 标识: {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}\r\n")
//		, guidHID.Data1, guidHID.Data2, guidHID.Data3
//		, guidHID.Data4[0], guidHID.Data4[1], guidHID.Data4[2], guidHID.Data4[3]
//		, guidHID.Data4[4], guidHID.Data4[5], guidHID.Data4[6], guidHID.Data4[7] );
//
//	// 2. 获取设备信息，得到设备信息集的句柄
//	s += _T("正在查找可用的 HID 设备...\r\n");
//	HDEVINFO hDevInfo = SetupDiGetClassDevs( &guidHID, NULL/*_T("USB")*/, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE );
//	if( INVALID_HANDLE_VALUE == hDevInfo ) {
//		s += _T("无法找到设备信息集的句柄...\r\n");
//		_tprintf_s(_T("%s"), s);
//		//std::tcout << s;
//		//m_ctrlHid2.SetWindowText(s);
//		ShowError();
//		return;
//	}
//
//	// 3. 遍历设备信息集中所有设备接口，得到其详细信息
//	SP_DEVICE_INTERFACE_DATA InterfaceData = { sizeof(SP_DEVICE_INTERFACE_DATA) };
//	for( DWORD index = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &guidHID, index, &InterfaceData); ++index )
//	{
//		/*
//		Using function SetupDiGetDeviceInterfaceDetail() to get details about an interface is typically a two-step process:
//		1. Get the required buffer size. 
//		   Call SetupDiGetDeviceInterfaceDetail with a NULLDeviceInterfaceDetailData pointer, a DeviceInterfaceDetailDataSize of zero, and a valid RequiredSize variable. 
//		   In response to such a call, this function returns the required buffer size at RequiredSize and fails with GetLastError returning ERROR_INSUFFICIENT_BUFFER.
//		2. Allocate an appropriately sized buffer and call the function again to get the interface details.
//		*/
//		DWORD dwRequiredSize = 0;
//		SetupDiGetDeviceInterfaceDetail(hDevInfo, &InterfaceData, NULL, 0, &dwRequiredSize, NULL); 
//		PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
//		pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(dwRequiredSize); // create buffer of required size
//		pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
//		if( !SetupDiGetDeviceInterfaceDetail(hDevInfo, &InterfaceData, pDetail, dwRequiredSize, NULL, NULL) ) {
//			ShowError();
//			free(pDetail);
//			continue;
//		}
//
//		s.AppendFormat( _T("[%d] path: %s\r\n"), index, pDetail->DevicePath );
//
//		HANDLE hUsb = CreateFile( pDetail->DevicePath, 0//GENERIC_READ|GENERIC_WRITE
//			, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
//		if( INVALID_HANDLE_VALUE == hUsb ) {
//			s += _T("无法打开端口。");
//			_tprintf_s(_T("%s"), s);
//			std::tcout << s;
//			//m_ctrlHid2.SetWindowText(s);
//			SetupDiDestroyDeviceInfoList( hDevInfo );
//			free( pDetail );
//			return;
//		}
//
//		// 查询设备标识
//		HIDD_ATTRIBUTES strtAttrib = { sizeof(HIDD_ATTRIBUTES) };
//		if( HidD_GetAttributes(hUsb,&strtAttrib) )
//		{
//			s.AppendFormat( _T("VendorID:\t%hX\r\n"), strtAttrib.VendorID );
//			s.AppendFormat( _T("ProductID:\t%hX\r\n"), strtAttrib.ProductID );
//			s.AppendFormat( _T("VerNumber:\t%hX\r\n"), strtAttrib.VersionNumber );
//		}
//		else
//			ShowError();
//
//		CloseHandle( hUsb );
//		free( pDetail );
//	}
//	
//	if( GetLastError() != ERROR_NO_MORE_ITEMS )
//		ShowError();
//
//	_tprintf_s(_T("%s"), s);
//	//std::tcout << s;
//	//m_ctrlHid2.SetWindowText(s);
//	SetupDiDestroyDeviceInfoList( hDevInfo );
//}
////--------------------------


void COTPWriterProDlg::OnBnClickedButtonOpenFile()
{
    m_ctrlProgress.SetPos(0);
    m_ctrlListBuffer.EndEdit(TRUE);

    CString strExt = _T("*.hex, *.bin, *.hexbin|*.hex;*.bin;*.hexbin|")
                     _T("Hex Files (*.hex)|*.hex|Bin Files (*.bin)|*.bin|HexBin Files (*.hexbin)|*.hexbin|All Files (*.*)|*.*||");
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strExt);
    if (dlg.DoModal() != IDOK)
    {
        return;
    }
    CString pathName = dlg.GetPathName();
    CString fileExt = dlg.GetFileExt();

    CStdioFile mFile;
    CFileException mExcept;

    HEXRECORD_t hr; 

    PrintCurrentTime();

    EditCtrlOutput(0, _T("Open file: %s ...   0%%"), pathName);

    int nOldSize = g_mem.SizeUsed();

    if (fileExt.CompareNoCase(_T("HEX")) == 0) 
    {
        mFile.Open(pathName, CFile::modeRead | CFile::typeText, &mExcept);
        unsigned int addr = 0;
        unsigned int max_addr = 0;
        unsigned int curpos = 0;
        unsigned int fileLen = (unsigned int)mFile.GetLength();

        if (fileLen) 
        {
            g_mem.ClearBufAll();
        }
        else 
        {
            AfxMessageBox(_T("File is empty!"));
            mFile.Close();
            return;
        }

        g_mem.ClearBufAll();

        while ((curpos = (unsigned int)mFile.GetPosition()) != fileLen) 
        {
            if (!HexRecReadFromFile(g_mem.GetBuf(), g_mem.GetBufFlag(), mFile, hr, addr))
            {
                EditCtrlOutput(0, _T("  Error!\r\n"));
                return;
            }
            max_addr = max(max_addr, addr);

            int percent = 100 * curpos / fileLen;
            m_ctrlProgress.SetPos(percent);
            EditCtrlOutput(-4, _T("%3d%%"), percent);
        }
        g_mem.SizeUsed(max_addr);
    }
    else if (fileExt.CompareNoCase(_T("BIN")) == 0) 
    {
        mFile.Open(pathName, CFile::modeRead | CFile::typeBinary, &mExcept);
        if (mFile.GetLength()) 
        {
            g_mem.ClearBufAll();
        }
        else 
        {
            AfxMessageBox(_T("File is empty!"));
            mFile.Close();
            return;
        }
        g_mem.ClearBufAll();

        g_mem.SizeUsed(mFile.Read(g_mem.GetBuf(), mFile.GetLength()));
        memset(g_mem.GetBufFlag(), 1, g_mem.SizeUsed());
    }
    else if (fileExt.CompareNoCase(_T("HEXBIN")) == 0) 
    {
        mFile.Open(pathName, CFile::modeRead | CFile::typeText, &mExcept);
        unsigned int addr = 0;
        unsigned int fileLen = (unsigned int)mFile.GetLength();

        if (fileLen) 
        {
            g_mem.ClearBufAll();
        }
        else 
        {
            AfxMessageBox(_T("File is empty!"));
            mFile.Close();
            return;
        }

        g_mem.ClearBufAll();

        CHgzString str;
        while (mFile.ReadString(str)) 
        {
            addr += str.ToHexArr(g_mem.GetBuf()+addr, _T(", *"), 1);
            
            int percent = 100 * mFile.GetPosition() / fileLen;
            m_ctrlProgress.SetPos(percent);
            EditCtrlOutput(-4, _T("%3d%%"), percent);
        }
        g_mem.SizeUsed(addr);
        memset(g_mem.GetBufFlag(), 1, g_mem.SizeUsed());
    }


    m_ctrlProgress.SetPos(100);
    EditCtrlOutput(-4, _T("100%\r\n"));
    mFile.Close();

    mFile.Open(pathName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
    CHgzMD5 md5;
    CString s = md5.md5bin(mFile);
    PrintCurrentTime();
    EditCtrlOutput(0, _T("md5(%s) = %s\r\n"), mFile.GetFileName(), s);
    s = md5.md5buf(g_mem.GetBuf(), g_mem.SizeUsed());
    PrintCurrentTime();
    EditCtrlOutput(0, _T("md5(buffer) = %s\r\n"), s);
    mFile.Close();

    //UpdateBufferShow();
    UpdateBufferDisplay(0, max(nOldSize, g_mem.SizeUsed()));
    m_ctrlMemAddrBegin.SetWindowText(_T("0000"));
    m_ctrlDataLength.SetWindowsTextFormat(m_chkDataLen.GetCheck() ? _T("%X") : _T("%d"), g_mem.SizeUsed());

    // if rollnum enabled, update rollnum region in buffer and display
    if (m_bRollnumEnable)
        ProcessRollnum();

}

void COTPWriterProDlg::OnBnClickedButtonSaveAs()
{
	m_ctrlProgress.SetPos(0);
	m_ctrlListBuffer.EndEdit(TRUE);
	
	if (g_mem.IsEmpty()) {
		MessageBox(_T("No data to save."));
		return;
	}
	
    CString strExt = _T("*.hex, *.bin, *.hexbin|*.hex;*.bin;*.hexbin|")
        _T("Hex Files (*.hex)|*.hex|Bin Files (*.bin)|*.bin|HexBin Files (*.hexbin)|*.hexbin|All Files (*.*)|*.*||");
    CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strExt);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	CString pathName = dlg.GetPathName();
	CString fileExt = dlg.GetFileExt();

	CStdioFile mFile;
	CFileException mExcept;

	PrintCurrentTime();

	EditCtrlOutput(0, _T("Save file: %s ...   0%%"), pathName);

    INT memSize = g_mem.SizeUsed();
    UINT8 *memBuf = g_mem.GetBuf();

    if (fileExt.CompareNoCase(_T("HEX")) == 0) {
		HEXRECORD_t hr;
		mFile.Open(pathName, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &mExcept);
		
		/*auto checksum = [&] () -> unsigned char {
			unsigned char x = hr.reclen + hr.addrh + hr.addrl + hr.rectype;
			for (unsigned int i = 0; i < hr.reclen; i++){
				x += hr.data[i];
			}
			x = ~x + 1;
			hr.data[hr.reclen] = x;
			return x;
		};*/
        for (unsigned int addr = 0, rec_data_len = 0; ; addr += rec_data_len)
		{

			if (addr >= memSize) {
				HexRec(hr, HEX_REC_EOF, 0, 0, NULL);
				HexRecSaveToFile(hr, mFile);
				break;
			}
			else if (addr && (addr % 0x10000 == 0)) {
				HexRec(hr, HEX_REC_ELA,  2, addr, memBuf);
				HexRecSaveToFile(hr, mFile);
			}
			rec_data_len = 16 < (memSize-addr) ? 16 : (memSize-addr);
            HexRec(hr, HEX_REC_DAT, rec_data_len, addr, memBuf);
			HexRecSaveToFile(hr, mFile);

			// progress
			int percent = 100*addr / memSize;
			m_ctrlProgress.SetPos(percent);
			EditCtrlOutput(-4, _T("%3d%%"), percent);
		}
	}
	else if (fileExt.CompareNoCase(_T("BIN")) == 0) {
		mFile.Open(pathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, &mExcept);
		mFile.SeekToBegin();
		mFile.Write(memBuf, memSize);
	}
    else if (fileExt.CompareNoCase(_T("hexbin")) == 0) {
        mFile.Open(pathName, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &mExcept);
        mFile.SeekToBegin();
        CString s;
        for (u32 i = 0; i < memSize; i++)
        {
            if (i == (memSize-1))
                s.AppendFormat(_T("0x%02X\n"), memBuf[i]);
            else if (i%16 == 15)
                s.AppendFormat(_T("0x%02X,\n"), memBuf[i]);
            else
                s.AppendFormat(_T("0x%02X, "), memBuf[i]);
        }
        mFile.WriteString(s);        
    }
	else {
		MessageBox(_T("Only Hex/Bin/HexBin file suported."));
		return;
	}

	m_ctrlProgress.SetPos(100);
	EditCtrlOutput(-4, _T("100%\r\n"));
	mFile.Close();
}


void COTPWriterProDlg::EditCtrlOutput( int pos, const TCHAR *szFormat, ... )
{
    // pos 为相对于最后位置的字符数

    va_list arglist;
    va_start(arglist, szFormat);
    TCHAR *buf = new TCHAR[_vsctprintf(szFormat, arglist)+1];
    //TCHAR *buf = (TCHAR *)_alloca((_vsctprintf(szFormat, arglist)+1)*sizeof(TCHAR)); // 动态分配栈内存，无需手动释放。
    _vstprintf(buf, szFormat, arglist);

    int len = m_ctrlEdit.GetWindowTextLength(); 
    m_ctrlEdit.SetSel(len + pos, len); //将插入光标放在最后 
    m_ctrlEdit.ReplaceSel(buf); 
    //m_ctrlEdit.ScrollWindow(0,0); //滚动到插入点 (滚动条始终在底部，不闪动)
    delete [] buf;
}


void COTPWriterProDlg::OnBnClickedButtonWrite()
{
    PrintCurrentTime();

    // check some status to see if it is not ready for writting.
    // 1. buffer is empty
    // 2. rollnum enabled but is out of range
    // 3. rollnum enabled but RF syncode is not correct
    CString str;
    str.Empty();
    if (g_mem.IsEmpty())
        str.Format(_T("警告！ 缓冲区空，不能烧写。\r\n"));
    else if (m_bRollnumEnable && !m_RollnumAndCPConfigDialog.IsRollnumValid())
        str.Format(_T("警告！ 滚码超限 [%08X, %08X]： %08X\r\n"), m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rollnumMin, m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rollnumMax, m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum);
    else if (m_bRollnumEnable && !m_RollnumAndCPConfigDialog.IsRFSyncodeValid())
        str.Format(_T("警告！ RF 同步码无效 [4B, 3B]： %08X, %06X\r\n"), m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.RFSynccode4B, m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.RFSynccode3B);

    if (!str.IsEmpty())
    {
        EditCtrlOutput(0, str);
        AfxMessageBox(str);
        m_bOK = false;
        return;
    }
    
    //////////////////////////////////////////////////////////////////////////
    u32 rollnumAddr = m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.OTPAddrForRollnum;
    u32 rollnumLen = sizeof(m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable);
    u32 startAddr = 0, numBytesToWrite = 0, numBytesWritten = 0;
    if (m_Option.m_bWriteBufSizeReallyUsed)
    {
        startAddr = 0;
        numBytesToWrite = g_mem.SizeUsed();
    }
    else
    {
        startAddr = GetStartAddress();
        numBytesToWrite = GetDataLength();
    }

    // split OTP writting to 2 steps:
    // step1: write buffer content, which may be splitted into 2 parts by rollnum region
    // step2: write rollnum region

    CHgzMem mem;
    mem.ClearBufAll();
    memcpy(mem.GetBuf(), g_mem.GetBuf(), g_mem.SizeUsed());
    memcpy(mem.GetBufFlag(), g_mem.GetBufFlag(), g_mem.SizeUsed());
    mem.SizeUsed(g_mem.SizeUsed());
    
    if (m_bRollnumEnable) // clear rollnum region before write to OTP
        FillIntersectionOfBuffer(mem.GetBuf()+startAddr, startAddr, numBytesToWrite, rollnumAddr, rollnumLen, 0x00);

    EditCtrlOutput(0, _T("需要写入字节数：%d\r\n"), numBytesToWrite);
    numBytesWritten = mem.Write(startAddr, numBytesToWrite);
    m_bOK = (numBytesWritten == numBytesToWrite);
    PrintCurrentTime();
    EditCtrlOutput(0, _T("实际写入字节数：%d，%s\r\n"), numBytesWritten, m_bOK ? _T("成功！") : _T("失败！"));
    
    if (m_bRollnumEnable && m_bOK)
    {
        mem.WriteBuf(rollnumAddr, (UINT8 *)&m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum, rollnumLen);

        // display rollnumRFSynccode in OUTPUT
        PrintCurrentTime(); EditCtrlOutput(0, _T("需要写入字节数：%d\r\n"), rollnumLen);
        PrintCurrentTime(); EditCtrlOutput(0, _T("滚码=0x%08X, RF同步码4B=0x%08X, RF同步码3B=0x%06X\r\n"),
            m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum,
            m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.synccode4b,
            m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.synccode3b);

        // Write rollnum
        numBytesWritten = mem.Write(rollnumAddr, rollnumLen);
        m_bOK = (numBytesWritten == rollnumLen);
        PrintCurrentTime(); EditCtrlOutput(0, _T("实际写入字节数：%d，%s\r\n"), numBytesWritten, m_bOK ? _T("成功！") : _T("失败！"));

        if (m_bOK)
        {
            // fill written rollnum to buffer's rollnum region.
            g_mem.WriteBuf(rollnumAddr, (UINT8 *)&m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum, rollnumLen);
            UpdateBufferDisplay(rollnumAddr, rollnumLen);

            // rollnum++, and judge if it is still in rollnum range
            m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum++;
            IsRollnumValid();
        
            // write new rollnum to config file
            m_RollnumAndCPConfigDialog.WriteRollnumToConfigFile();
            
            // fill RollnumRegionForWrite with new rollnum and RF synccodes
            ProcessRollnum();
        }
    }

    if (m_Option.m_bWriteBufSizeReallyUsed)
        m_ctrlDataLength.SetWindowsTextFormat(m_chkDataLen.GetCheck() ? _T("%X") : _T("%d"), g_mem.SizeUsed());
}


void COTPWriterProDlg::OnBnClickedButtonRead()
{
    PrintCurrentTime();
    
    INT addr = GetStartAddress();
    INT len = GetDataLength();
    EditCtrlOutput(0, _T("读取 ... "));
    EditCtrlOutput(-7, _T("实际读取字节数：%d\r\n"), g_mem.Read(addr, len));
    UpdateBufferDisplay(addr, len);

    CString s;
    CHgzMD5 md5;
    s = md5.md5buf(g_mem.GetBuf(), g_mem.SizeUsed());
    PrintCurrentTime();
    EditCtrlOutput(0, _T("md5(buffer) = %s\r\n"), s);

    if (m_Option.m_bWriteBufSizeReallyUsed)
        m_ctrlDataLength.SetWindowsTextFormat(m_chkDataLen.GetCheck() ? _T("%X") : _T("%d"), g_mem.SizeUsed());

}


void COTPWriterProDlg::OnBnClickedButtonVerify()
{
    PrintCurrentTime(); EditCtrlOutput(0, _T("校验 ... "));
    m_bOK = m_ctrlIgnoreMem.GetCheck() ? g_mem.VerifyEx(GetStartAddress(), GetDataLength(), GetStartIgnoreAddress(), GetEndIgnoreAddress()) : g_mem.Verify(GetStartAddress(), GetDataLength());
    EditCtrlOutput(-7, m_bOK ? _T("校验成功！\r\n") : _T("校验失败！\r\n"));

    // verify rollnum
    if (m_bOK && m_bRollnumEnable)
    {
        u32 rollnumAddr = m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.OTPAddrForRollnum;
        u32 rollnumLen = sizeof(m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable);

        PrintCurrentTime(); EditCtrlOutput(0, _T("滚码校验 ... "));
        m_bOK = m_ctrlIgnoreMem.GetCheck() ? g_mem.VerifyEx(rollnumAddr, rollnumLen, GetStartIgnoreAddress(), GetEndIgnoreAddress()) : g_mem.Verify(rollnumAddr, rollnumLen);
        EditCtrlOutput(-7, m_bOK ? _T("校验成功！\r\n") : _T("校验失败！\r\n"));
    }
}

void COTPWriterProDlg::OnBnClickedButtonEncrypt()
{
    Cmd1Data0(&CHgzMem::Encryt, _T("加密"));
}

void COTPWriterProDlg::OnBnClickedButtonErase()
{
    PrintCurrentTime();

    UINT32 startSectorNum = 0;
    UINT32 endSectorNum = 0;
    BOOL bRes = FALSE;
    
    if (m_ctrlEraseAll.GetCheck())
        bRes = g_mem.EreaseAll(startSectorNum, endSectorNum);
    else
    {
        CString s;
        m_ctrlErasePageNum.GetWindowText(s);
        s.Trim();
        if (s.IsEmpty()) 
        {
            AfxMessageBox(_T("参数不能为空！"));
            EditCtrlOutput(0, _T("参数不能为空！\r\n"));
            return;
        }
        CStringArray arr;
        hgzExtractSubStrings1(arr, s, _T("-"));
        if (arr.GetSize() > 2)
        {
            AfxMessageBox(_T("参数太多！"));
            EditCtrlOutput(0, _T("参数太多！\r\n"));
            return;
        }
        if (arr.GetSize() == 1)
            arr.SetAtGrow(1, arr[0].GetString());
        arr[0].Trim();
        arr[1].Trim();
        startSectorNum = stoul(arr[0].GetString(), 0, 10);
        endSectorNum = stoul(arr[1].GetString(), 0, 10);
        
        bRes = g_mem.Erease(startSectorNum, endSectorNum);
    }

    if (bRes)
    {
        EditCtrlOutput(0, _T("擦除块：%d - %d\r\n"), startSectorNum, endSectorNum);
        PrintCurrentTime();
        EditCtrlOutput(0, _T("擦除 ... 请等待：容量 X 6.25秒/MByte\r\n"));
    }
    else
    {
        EditCtrlOutput(0, _T("执行失败！\r\n"));
    }
}

void COTPWriterProDlg::PrintCurrentTime()
{
	CTime t = CTime::GetCurrentTime();
	EditCtrlOutput(0, _T("%04d-%02d-%02d %02d:%02d:%02d:  "), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
}


afx_msg LRESULT COTPWriterProDlg::OnLvmItemChanged(WPARAM wParam, LPARAM lParam)
{
    TRACE(_T("Received WM_LVM_ENDEDIT message: %d, %d\n"), wParam, lParam);

    unsigned int addr = wParam*16 + lParam-1;

    CString s;
    s = m_ctrlListBuffer.GetItemText(wParam, lParam);
    s.Trim();
    if (s.GetLength() == 0) {
        g_mem.ClearBuf(addr, 1);
    }
    else {
        try
        {
            UINT8 x = stoul(s.GetString(), 0, 16);
            g_mem.WriteBuf(addr, &x, 1);
            s.Format(_T("%02X"), g_mem.GetBuf()[addr]);
            m_ctrlListBuffer.SetItemText(wParam, lParam, s);
        }
        catch (std::invalid_argument &e)
        {
            EditCtrlOutput(0, _T("Invalid argument!\r\n"));
            if (g_mem.IsUsed(addr))
            {
                s.Format(_T("%02X"), g_mem.GetBuf()[addr]);
                m_ctrlListBuffer.SetItemText(wParam, lParam, s);
            }
            else
            {
                m_ctrlListBuffer.SetItemText(wParam, lParam, _T(""));
            }
        }
    }
    TRACE(_T("SizeUsed = %d, CurCell = %d\n"), g_mem.SizeUsed(), addr);

    if (m_Option.m_bWriteBufSizeReallyUsed)
        m_ctrlDataLength.SetWindowsTextFormat(m_chkDataLen.GetCheck() ? _T("%X") : _T("%d"), g_mem.SizeUsed());

    return 0;
}


unsigned int COTPWriterProDlg::GetStartAddress()
{
	CString s;

	m_ctrlMemAddrBegin.GetWindowText(s);
	unsigned int start_addr = s.GetLength() ? stoul((tstring)s.GetString(), 0, 16) : 0;
	
	return start_addr;
}

unsigned int COTPWriterProDlg::GetDataLength()
{
	CString s;

	m_ctrlDataLength.GetWindowText(s);
	unsigned int length = s.GetLength() ? stoul((tstring)s.GetString(), 0, m_chkDataLen.GetCheck() ? 16 : 10) : 0;
	
	return length;
}

unsigned int COTPWriterProDlg::GetStartIgnoreAddress()
{
    CString s;

    m_ctrlIgnoreMemBegin.GetWindowText(s);
    unsigned int start_addr = s.GetLength() ? stoul((tstring)s.GetString(), 0, 16) : 0;

    return start_addr;
}

unsigned int COTPWriterProDlg::GetEndIgnoreAddress()
{
    CString s;

    m_ctrlIgnoreMemEnd.GetWindowText(s);
    unsigned int start_addr = s.GetLength() ? stoul((tstring)s.GetString(), 0, 16) : 0;

    return start_addr;
}

void COTPWriterProDlg::OnBnClickedButtonBlankCheck()
{
    Cmd1Data0(&CHgzMem::IsBlank, _T("查空"));
}

void COTPWriterProDlg::OnBnClickedButtonEnterProgramMode()
{
    Cmd1Data0(&CHgzMem::EnterOTPSPIMode, _T("进入 OTP SPI 操作模式"));
}


void COTPWriterProDlg::OnBnClickedButtonExitProgramMode()
{
    Cmd1Data0(&CHgzMem::ExitOTPSPIMode, _T("退出 OTP SPI 操作模式"));
}


void COTPWriterProDlg::OnBnClickedButtonTestBlankCheck()
{
    Cmd1Data0(&CHgzMem::OTPTest_BlankCheckTest, _T("查空测试"));
}


void COTPWriterProDlg::OnBnClickedButtonTestDec()
{
    Cmd1Data0(&CHgzMem::OTPTest_WordLineAndBitLineIntegrityTest, _T("字位线完整性测试"));
}


void COTPWriterProDlg::OnBnClickedButtonTestWR()
{
    Cmd1Data0(&CHgzMem::OTPTest_PreprogramTest, _T("预编程测试"));
}


void COTPWriterProDlg::OnBnClickedButtonOtpSpiReset()
{
    Cmd1Data0(&CHgzMem::OTPSPIReset, _T("OTP SPI 复位"));
}


void COTPWriterProDlg::UpdateBufferDisplay( unsigned int addr, unsigned int length )
{
    CString s;
    for (int i = addr; i < addr+length ; i++)
    {
        if (g_mem.GetBufFlag()[i]) 
            s.Format(_T("%02X"), g_mem.GetBuf()[i]);
        else 
            s.Empty();

        m_ctrlListBuffer.SetItemText(i/16, i%16 + 1, s);
    }
}

void COTPWriterProDlg::OnBnClickedButtonOption()
{
    if (m_Option.DoModal() == IDOK)
    {
        if (m_Option.m_bEnableConsoleOutput)
            hgzOpenConsole();
        else
            hgzCloseConsole();

        if (m_Option.m_bWriteBufSizeReallyUsed)
        {
            m_ctrlMemAddrBegin.EnableWindow(FALSE);
            m_ctrlDataLength.EnableWindow(FALSE);

            m_ctrlMemAddrBegin.SetWindowsTextFormat(_T("%04X"), 0);
            m_ctrlDataLength.SetWindowsTextFormat(m_chkDataLen.GetCheck() ? _T("%X") : _T("%d"), g_mem.SizeUsed());
        }
        else
        {
            m_ctrlMemAddrBegin.EnableWindow(TRUE);
            m_ctrlDataLength.EnableWindow(TRUE);
        }
    }
}

UINT8 COTPWriterProDlg::GetDataToFillBuffer()
{
    CString str;
    m_cbDataToFill.GetWindowText(str);
    if (!str.IsEmpty())
    {
        UINT x;
        _stscanf_s(str, _T("%x"), &x);
        return (UINT8)x;
    }
    else
        return 0;;
}

void COTPWriterProDlg::OnCbnSelchangeComboSelectChipType()
{
    PrintCurrentTime();

    CString s;
    m_ctrlChipSel.GetLBText(m_ctrlChipSel.GetCurSel(), s);
    
    if (s.IsEmpty())
        m_ChipType = HS__CMD__CHIP_TYPE__NONE;
    else if (s.Compare(_T("HS6206")) == 0)
        m_ChipType = HS__CMD__CHIP_TYPE__OTP__HS6206;
    else if (s.Compare(_T("EN25T80")) == 0)
        m_ChipType = HS__CMD__CHIP_TYPE__FLASH__EN25T80;
	else if (s.Compare(_T("FPGA_HS6206")) == 0)
		m_ChipType = HS__CMD__CHIP_TYPE__FPGA__HS6206;
	else if (s.Compare(_T("HS6210")) == 0)
	{
		m_ChipType = HS__CMD__CHIP_TYPE__OTP__HS6210;
		m_chkDataLen.SetCheck(FALSE);
		m_ctrlMemAddrBegin.SetWindowText(_T("0000"));
		m_ctrlDataLength.SetWindowText(_T("128"));
	}
	else
        m_ChipType = HS__CMD__CHIP_TYPE__NONE;

    HS_CHIP_TYPE_t chipType = m_ChipType;
    CString str(_T("设定芯片型号: "));
    str += s;
    if (g_mem.SetChipType(&chipType) && chipType == m_ChipType)
        str += _T(" -- 成功！\r\n");
    else
        str += _T(" -- 失败！\r\n");

    EditCtrlOutput(0, str.GetString());
}


void COTPWriterProDlg::OnBnClickedButtonDetectChipType()
{
    PrintCurrentTime();

    HS_CHIP_TYPE_t chipType = m_ChipType;
    BOOL bRes = g_mem.DetectChipType(&chipType);
    if ( bRes && chipType!=HS__CMD__CHIP_TYPE__NONE )
    {
        EditCtrlOutput(0, _T("检测芯片型号成功！\r\n"));
    }
    else
    {
        EditCtrlOutput(0, _T("检测芯片型号失败！\r\n"));
        return;
    }
    
    switch (chipType)
    {
	case HS__CMD__CHIP_TYPE__OTP__HS6206:
		m_ctrlChipSel.SelectString(0, _T("HS6206"));
		break;
	case HS__CMD__CHIP_TYPE__OTP__HS6210:
		m_ctrlChipSel.SelectString(0, _T("HS6210"));
		break;
	case HS__CMD__CHIP_TYPE__FLASH__EN25T80:
        m_ctrlChipSel.SelectString(0, _T("EN25T80"));
        break;
    
    case HS__CMD__CHIP_TYPE__NONE:
    default:
        m_ctrlChipSel.SetCurSel(-1);
        break;
    }

    OnCbnSelchangeComboSelectChipType(); // set downstream machine to enter into correct mode.
}


void COTPWriterProDlg::OnBnClickedButtonVersionNum()
{
    // 上位机版本信息
    PrintCurrentTime();
    EditCtrlOutput(0, _T("上位机软件版本信息：%s\r\n"), GetProductVersion(NULL));
    
    // 下位机版本信息
    PrintCurrentTime();
    char fmVerInfo[64*2];
    BOOL bRes = g_mem.GetFirmwareVersionInfo((UINT8 *)fmVerInfo);

    if ( bRes )
    {
        CString s(fmVerInfo);
        _tprintf(_T("%s\n"), s);
        EditCtrlOutput(0, _T("下位机固件版本信息：%s\r\n"), s);
    }
    else
    {
        EditCtrlOutput(0, _T("下位机固件版本信息：获取失败！\r\n"));
        return;
    }
}


void COTPWriterProDlg::OnBnClickedCheckLengthHex()
{
    BOOL bHex = m_chkDataLen.GetCheck();
    
    CHgzString s;
    m_ctrlDataLength.GetWindowText(s);
    unsigned int length = s.GetLength() ? stoul((tstring)s.GetString(), 0, bHex ? 10 : 16) : 0;

    m_ctrlDataLength.ResetContent();
    m_ctrlDataLength.AddString(_T("0"));
    m_ctrlDataLength.AddString((s.ulltoa(1024, bHex ? 16 : 10), s));
    m_ctrlDataLength.AddString((s.ulltoa(2048, bHex ? 16 : 10), s));
    m_ctrlDataLength.AddString((s.ulltoa(4096, bHex ? 16 : 10), s));
    m_ctrlDataLength.AddString((s.ulltoa(8192, bHex ? 16 : 10), s));
    m_ctrlDataLength.AddString((s.ulltoa(16384, bHex ? 16 : 10), s));

    s.ulltoa(length, bHex ? 16 : 10);
    m_ctrlDataLength.SetWindowText(s);
}

void COTPWriterProDlg::Cmd1Data0( CHgzMem::pMemFunc_Cmd1Data0 CmdFunc, CString s )
{
    PrintCurrentTime();

    CString str = s;
    UINT32 res = 0;
    m_bOK = ((g_mem.*CmdFunc)(res) && res);
    if (m_bOK)
            str += _T("成功！\r\n");
    else
        str += _T("失败！\r\n");

    EditCtrlOutput(0, str.GetString());
}


void COTPWriterProDlg::WriteRollnum(void)
{
    u32 startAddr = 0, numBytesToWrite = 0, numBytesWritten = 0;
    startAddr = m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.OTPAddrForRollnum;
    numBytesToWrite = sizeof(m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable);
    
    CHgzMem mem;
    mem.WriteBuf(startAddr, (UINT8 *)&m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum, numBytesToWrite);
    
    PrintCurrentTime();
    EditCtrlOutput(0, _T("滚码烧录：\r\n"));
    PrintCurrentTime();
    EditCtrlOutput(0, _T("需要写入字节数：%d\r\n"), numBytesToWrite);
    numBytesWritten = mem.Write(startAddr, numBytesToWrite);
    PrintCurrentTime();
    m_bOK = numBytesWritten == numBytesToWrite;
    EditCtrlOutput(0, _T("实际写入字节数：%d，%s\r\n"), numBytesWritten, m_bOK ? _T("成功！") : _T("失败！"));
}


afx_msg void COTPWriterProDlg::OnBnClickedButtonAuto()
{
    EditCtrlOutput(0, _T("\r\n"));
    PrintCurrentTime();
    
    m_bOK = true;
    CString s;
    m_ctrlChipSel.GetLBText(m_ctrlChipSel.GetCurSel(), s);
    
    if (s.Compare(_T("HS6206")) == 0)
    {
        EditCtrlOutput(0, _T("自动 ＝ 查空测试 + 字位线测试 + 预编程测试 + 烧写 + 校验\r\n"));

        if (!m_bOK)
            goto LocalExit;
        OnBnClickedButtonTestBlankCheck();
    
        if (!m_bOK)
            goto LocalExit;
        OnBnClickedButtonTestDec();
    
        if (!m_bOK)
            goto LocalExit;
        OnBnClickedButtonTestWR();
    }
    else if (s.Compare(_T("FPGA_HS6206")) == 0)
    {
        EditCtrlOutput(0, _T("自动 ＝ 查空测试 + 烧写 + 校验\r\n"));
        
        if (!m_bOK)
            goto LocalExit;
        OnBnClickedButtonTestBlankCheck();
    }
    else
    {
        EditCtrlOutput(0, _T("自动 ＝ 烧写 + 校验\r\n"));
    }

    if (!m_bOK)
        goto LocalExit;
    OnBnClickedButtonWrite();
    
    Sleep(200);

    if (!m_bOK)
        goto LocalExit;
    OnBnClickedButtonVerify();
    if (!m_bOK && m_bRollnumEnable) 
    {
        // if rollnum enabled, cancel rollnum increasing.
        PrintCurrentTime();
        EditCtrlOutput(0, _T("滚码自增回撤。\r\n"));

        m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum--;
        IsRollnumValid();

        // write new rollnum to config file
        m_RollnumAndCPConfigDialog.WriteRollnumToConfigFile();

        // fill buffer's rollnum region with new rollnum and RF synccodes
        ProcessRollnum();

    }

LocalExit:
    EditCtrlOutput(0, _T("\r\n"));

}

void COTPWriterProDlg::FillRollnumRegionForWrite(void)
{
    CString s;
    s.Format(_T("%08X"), m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum);
    GetDlgItem(IDC_EDIT3)->SetWindowText(s);
    s.Format(_T("%08X"), m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.synccode4b);
    GetDlgItem(IDC_EDIT10)->SetWindowText(s);
    s.Format(_T("%06X"), m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.synccode3b);
    GetDlgItem(IDC_EDIT11)->SetWindowText(s);
    s.Format(_T("%010X"), m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.reserved);
    GetDlgItem(IDC_EDIT12)->SetWindowText(s);

    if (!m_RollnumAndCPConfigDialog.IsRollnumValid() || !m_RollnumAndCPConfigDialog.IsRFSyncodeValid())
    {
        m_bRollnumEnable = FALSE;
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("EnableRollnum"), m_bRollnumEnable);
        m_bRollnumEnable ? m_ctrlRollnum.SetWindowText(_T(" 滚码烧录 [√]")) : m_ctrlRollnum.SetWindowText(_T(" 滚码烧录 [  ]"));
    }
}



void COTPWriterProDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    AdjustRollnumForWrite(pNMUpDown->iDelta);

    *pResult = 0;
}

bool COTPWriterProDlg::IsRollnumValid()
{
    if (!m_RollnumAndCPConfigDialog.IsRollnumValid())
    {
        PrintCurrentTime();
        CString s;
        s.Format(_T("警告！ 滚码超限 [%08X, %08X]： %08X"), m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rollnumMin, m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rollnumMax, m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum);
        EditCtrlOutput(0, s+_T("\r\n"));
        AfxMessageBox(s);
        return false;
    }
    return true;
}


void COTPWriterProDlg::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    if (GetDlgItem(IDOK) == GetFocus())
    {
        CDialogEx::OnOK();
    }

    if (GetDlgItem(IDC_EDIT3) == GetFocus())
    {
        AdjustRollnumForWrite(0);
    }


    //CDialogEx::OnOK();
}


void COTPWriterProDlg::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类

    CDialogEx::OnCancel();
}

void COTPWriterProDlg::AdjustRollnumForWrite( INT32 iDelta )
{
    CHgzString s;
    GetDlgItem(IDC_EDIT3)->GetWindowText(s);
    m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum = s.stoul(0, 16);
    m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable.rollnum += iDelta;

    if (IsRollnumValid() && m_bRollnumEnable)
        m_RollnumAndCPConfigDialog.WriteRollnumToConfigFile();

    m_RollnumAndCPConfigDialog.SD_FillRollnumRFSynccode();
    FillRollnumRegionForWrite();
}


void COTPWriterProDlg::OnBnClickedButtonBufferDataProfile()
{
    u32 numOfOne = 0;
    u32 numTotal = g_mem.SizeUsed() * 8;

    UINT8 *pbuf = g_mem.GetBuf();

    for (u32 i = 0; i < g_mem.SizeUsed(); i++)
        numOfOne += count_ones(pbuf[i], 8);

    PrintCurrentTime();
    EditCtrlOutput(0, _T("数据概况： num_of_ones = %d, ratio = %.2f%%\r\n"), numOfOne, (FLOAT)numOfOne*100/numTotal);

    //////////////////////////////////////////////////////
    // for MD5 test
    CHgzMD5 md5;
    md5.md5buf(g_mem.GetBuf(), g_mem.SizeUsed());
    /*CString sDigest;
    CString s(_T("jklmn"));*/
    //md5.md5(_T("jklmn"));
    //md5.md5("jklmn");
    //md5.md5("a");


}

void COTPWriterProDlg::OnBnClickedButton23()
{
    CDlgTools dlg;
    dlg.DoModal();
}


void COTPWriterProDlg::OnRollnumAndCPConfig()
{
    if (m_RollnumAndCPConfigDialog.DoModal() == IDCANCEL)
    {
        return;
    }
    

}


void COTPWriterProDlg::OnRollnumEnable()
{
    m_bRollnumEnable = !m_bRollnumEnable;
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("EnableRollnum"), m_bRollnumEnable);
    return ProcessRollnum();
}


void COTPWriterProDlg::OnUpdateRollnumEnable(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bRollnumEnable);
}


void COTPWriterProDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

    // TODO: 在此处添加消息处理程序代码
    if(!bSysMenu && pPopupMenu)  
    {  
        CCmdUI cmdUI;  
        cmdUI.m_pOther = NULL;  
        cmdUI.m_pMenu = pPopupMenu;  
        cmdUI.m_pSubMenu = NULL;  

        UINT count = pPopupMenu->GetMenuItemCount();  
        cmdUI.m_nIndexMax = count;  
        for(UINT i=0; i<count; i++)  
        {  
            UINT nID = pPopupMenu->GetMenuItemID(i);  
            if(-1 == nID || 0 == nID)  
            {  
                continue;  
            }  
            cmdUI.m_nID = nID;  
            cmdUI.m_nIndex = i;  
            cmdUI.DoUpdate(this, FALSE);  
        }  
    }   
}

void COTPWriterProDlg::ProcessRollnum()
{
    m_bRollnumEnable ? m_ctrlRollnum.SetWindowText(_T(" 滚码烧录 [√]")) : m_ctrlRollnum.SetWindowText(_T(" 滚码烧录 [  ]"));

    if (!m_bRollnumEnable)
    {
        if (AfxMessageBox(_T("是否清空缓冲区内的滚码区数据？"), MB_YESNO) == IDYES)
        {
            u32 rollnumAddr = m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.OTPAddrForRollnum;
            u32 rollnumLen = sizeof(m_RollnumAndCPConfigDialog.m_vTest_OTPWrite.rsTable);

            u32 sz = g_mem.SizeUsed();
            g_mem.FillBuf(rollnumAddr, 0, rollnumLen);
            g_mem.SizeUsed(sz); // do not change g_mem's size of used.
            UpdateBufferDisplay(rollnumAddr, rollnumLen);
            PrintCurrentTime();
            EditCtrlOutput(0, _T("将缓冲区内的滚码区数据清空为 0x00。\r\n"));
        }

        return;
    }

    // 1. Parse config file.
    m_RollnumAndCPConfigDialog.ConfigFile_Parser();

    if (!m_RollnumAndCPConfigDialog.m_ConfigFileCheck.CP_Config_File1 && !m_RollnumAndCPConfigDialog.m_ConfigFileCheck.CP_Config_File2)
    {
        PrintCurrentTime();
        EditCtrlOutput(0, _T("滚码与CP测试配置文件路径错误，请重新配置。\r\n"));
    }

    // 2. Fill rollnumRFSynccode struct
    m_RollnumAndCPConfigDialog.SD_FillRollnumRFSynccode();

    // 3. Fill rollnum region for Write
    FillRollnumRegionForWrite();
}


void COTPWriterProDlg::OnBnClicked_RollnumEnable()
{
    OnRollnumEnable();
}

void COTPWriterProDlg::OnBnClickedButtonBufferSearch()
{
    static int pos = -1;
    static CString oldstr(_T(""));

    if (m_ctrlBufferSearch.GetCurSel() == 0)
    {
        pos = -1;
        oldstr.Empty();
        return OnMenu_ClearBufferFind();
    }
    CHgzString newstr;
    m_ctrlBufferSearch.GetWindowText(newstr);

    // check if string is right
    if (newstr.GetLength() == 0)
        return;
    if (!newstr.is_xdigit_or_space())
    {
        hgzMessageBox(_T("请输入十六进制数，以空格分隔。"));
        return;
    }
    
    m_ctrlBufferSearch.AddStringWithoutDuplication(newstr);
    if (newstr.Compare(oldstr) != 0)
    {
        pos = -1;
        oldstr = newstr;
        m_ctrlListBuffer.ClearSubItemColor();
    }

    UINT8 x[1024];
    u32 numByte = m_ctrlBufferSearch.GetWindowHexArray(x);

    // start searching buffer
    pos++;
    int addr = -1;
    for (int i = pos; i < g_mem.SizeUsed(); i++)
    {
        addr = i;
        for (int j = 0; j < numByte; j++) {
            UINT8 y = 0;
            g_mem.ReadBuf(i+j, &y, 1);
            if (y != x[j])
            {
                addr = -1;
                break;
            }
        }
        if (addr == i)
            break;
    }

    pos = addr;

    PrintCurrentTime();
    if (addr != -1) {
        EditCtrlOutput(0, _T("Data found at: %04X.\r\n"), addr);
        m_ctrlListBuffer.EnsureVisible(addr/16, FALSE);
        //m_ctrlListBuffer.SetTopIndex(addr/16);
        m_ctrlListBuffer.SetSubItemColor(addr, addr+numByte-1, RGB(255,0,0), RGB(255,153,0), true);

    }
    else
        EditCtrlOutput(0, _T("Not found.\r\n"));
}


void COTPWriterProDlg::OnMenu_ClearBufferFind()
{
    m_ctrlBufferSearch.SetWindowText(_T(""));
    m_ctrlListBuffer.ClearSubItemColor();
}


void COTPWriterProDlg::OnCbnSelchangeCombo7()
{
    SetTimer(1, 10, NULL);
}

void COTPWriterProDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    switch (nIDEvent)
    {
    case 1:
        KillTimer(1);
        OnBnClickedButtonBufferSearch();
        break;

    case 2:
        KillTimer(2);
        OnBnClickedButtonFillInBuffer();
        break;
    }

    CDialogEx::OnTimer(nIDEvent);
}


void COTPWriterProDlg::OnBnClickedButtonFillInBuffer()
{
    UINT8 data = GetDataToFillBuffer();
    UINT addr;
    UINT dataLen;

    if (m_chkFillBufferAll.GetCheck())
    {
        addr = 0;
        dataLen = g_mem.SizeUsed();
    }
    else
    {
        addr = GetStartAddress();
        dataLen = GetDataLength();
    }

    g_mem.FillBuf(addr, data, dataLen);
    UpdateBufferDisplay(addr, dataLen);

    if (m_Option.m_bWriteBufSizeReallyUsed)
        m_ctrlDataLength.SetWindowsTextFormat(m_chkDataLen.GetCheck() ? _T("%X") : _T("%d"), g_mem.SizeUsed());

}

void COTPWriterProDlg::OnBnClickedButtonClearBuffer()
{
    UINT32 addr;
    UINT64 dataLen;

    if (m_chkFillBufferAll.GetCheck())
    {
        addr = 0;
        dataLen = g_mem.SizeUsed();
    }
    else
    {
        addr = GetStartAddress();
        dataLen = GetDataLength();
    }

    g_mem.ClearBuf(addr, dataLen);
    UpdateBufferDisplay(addr, dataLen);

    if (m_Option.m_bWriteBufSizeReallyUsed)
        m_ctrlDataLength.SetWindowsTextFormat(m_chkDataLen.GetCheck() ? _T("%X") : _T("%d"), g_mem.SizeUsed());

}



void COTPWriterProDlg::OnCbnSelchangeCombo6()
{
    SetTimer(2, 10, NULL);
}
