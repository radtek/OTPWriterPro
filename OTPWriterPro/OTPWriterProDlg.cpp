
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
#include "Option.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


	
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
    DDX_Control(pDX, IDC_BUTTON5, m_ctrlCodeSaveAs);
    DDX_Control(pDX, IDC_COMBO3, m_ctrlMemAddrBegin);
    DDX_Control(pDX, IDC_COMBO4, m_ctrlDataLength);
    DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
    DDX_Control(pDX, IDC_CHECK3, m_ctrlSaveAppend);
    DDX_Control(pDX, IDC_LIST1, m_ctrlListBuffer);
    DDX_Control(pDX, IDC_COMBO6, m_cbDataToFill);
    DDX_Control(pDX, IDC_CHECK4, m_chkFillBufferAll);
    DDX_Control(pDX, IDC_CHECK5, m_chkClearBufferAll);
}

BEGIN_MESSAGE_MAP(COTPWriterProDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &COTPWriterProDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COTPWriterProDlg::OnBnClickedButton2)
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
	ON_BN_CLICKED(IDC_BUTTON15, &COTPWriterProDlg::OnBnClickedButtonChipReset)
	ON_BN_CLICKED(IDC_BUTTON18, &COTPWriterProDlg::OnBnClickedButtonInBuffer)
    ON_BN_CLICKED(IDC_BUTTON19, &COTPWriterProDlg::OnBnClickedButtonOption)
    ON_BN_CLICKED(IDC_BUTTON20, &COTPWriterProDlg::OnBnClickedButtonClearBuffer)
    ON_CBN_SELCHANGE(IDC_COMBO1, &COTPWriterProDlg::OnCbnSelchangeComboSelectChipType)
    ON_BN_CLICKED(IDC_BUTTON3, &COTPWriterProDlg::OnBnClickedButtonDetectChipType)
    ON_BN_CLICKED(IDC_BUTTON21, &COTPWriterProDlg::OnBnClickedButtonVersionNum)
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
	m_ctrlChipSel.SetCurSel(0);
	m_ctrlMemAddrBegin.SetWindowText(_T("0000"));
	m_ctrlDataLength.SetWindowText(_T("16384"));

	memset(m_buf, 0, sizeof(m_buf));
    memset(m_bufFlag, 0, sizeof(m_bufFlag));
	m_bufDataLength = 0;
	process_state = process_state_idle;

	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetPos(0);


	
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
	m_ctrlListBuffer.SetItemCount(16384/16);
	for (unsigned int i = 0; i < 16384/16; i++) {
		s.Format(_T("%04X %04X"), (unsigned __int16)(i>>12), (unsigned __int16)(i<<4));
		m_ctrlListBuffer.InsertItem(i, s);
	}


    // Option
    m_option.bEnableConsoleOutput = FALSE;
    m_option.bEnableIncontinuousCell = FALSE;
    m_option.nPacketDataLength = 52;
    if (m_option.bEnableConsoleOutput)	
        hgzOpenConsole();

    m_ChipType = HS__CMD__CHIP_TYPE__OTP__HS6206;


	// Start a periodic read IRP to timely receive datas from device.
	//SetTimer(1, 1, NULL);


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


void COTPWriterProDlg::OnBnClickedButton1()
{
	// use HIDAPI
	int res;
	unsigned char buf[256];
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	struct hid_device_info *devs, *cur_dev;

	if (hid_init())
		return;

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	while (cur_dev) {
		_tprintf(_T("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls"), cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		_tprintf(_T("\n"));
		_tprintf(_T("  Manufacturer: %ls\n"), cur_dev->manufacturer_string);
		_tprintf(_T("  Product:      %ls\n"), cur_dev->product_string);
		_tprintf(_T("  Release:      %hx\n"), cur_dev->release_number);
		_tprintf(_T("  Interface:    %d\n"),  cur_dev->interface_number);
		_tprintf(_T("\n"));
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);


	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	//handle = hid_open(0x4d8, 0x3f, NULL);
	handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
	if (!handle) {
		_tprintf(_T("unable to open device\n"));
		return;
	}

	// Read the Manufacturer String
	wstr[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		_tprintf(_T("Unable to read manufacturer string\n"));
	_tprintf(_T("Manufacturer String: %ls\n"), wstr);

	// Read the Product String
	wstr[0] = 0x0000;
	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		_tprintf(_T("Unable to read product string\n"));
	_tprintf(_T("Product String: %ls\n"), wstr);

	// Read the Serial Number String
	wstr[0] = 0x0000;
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (res < 0)
		_tprintf(_T("Unable to read serial number string\n"));
	_tprintf(_T("Serial Number String: (%d) %ls"), wstr[0], wstr);
	_tprintf(_T("\n"));

	// Read Indexed String 1
	wstr[0] = 0x0000;
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	if (res < 0)
		_tprintf(_T("Unable to read indexed string 1\n"));
	_tprintf(_T("Indexed String 1: %ls\n"), wstr);

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);

	// Set up the command buffer.
	memset(buf,0x00,sizeof(buf));
	buf[0] = 0x01;
	buf[1] = 0x81;

	// Try to read from the device. There shoud be no
	// data here, but execution should not block.
	res = hid_read(handle, buf, 17);

	// Send a Feature Report to the device
	buf[0] = 0x2;
	buf[1] = 0xa0;
	buf[2] = 0x0a;
	buf[3] = 0x00;
	buf[4] = 0x00;
	res = hid_send_feature_report(handle, buf, 17);
	if (res < 0) {
		_tprintf(_T("Unable to send a feature report.\n"));
	}

	memset(buf,0,sizeof(buf));

	// Read a Feature Report from the device
	buf[0] = 0x2;
	res = hid_get_feature_report(handle, buf, sizeof(buf));
	if (res < 0) {
		_tprintf(_T("Unable to get a feature report.\n"));
		_tprintf(_T("%ls"), hid_error(handle));
	}
	else {
		// Print out the returned buffer.
		_tprintf(_T("Feature Report\n   "));
		for (i = 0; i < res; i++)
			_tprintf(_T("%02hhx "), buf[i]);
		_tprintf(_T("\n"));
	}

	memset(buf,0,sizeof(buf));

	// Toggle LED (cmd 0x80). The first byte is the report number (0x1).
	buf[0] = 0x1;
	buf[1] = 0x80;
	res = hid_write(handle, buf, 17);
	if (res < 0) {
		_tprintf(_T("Unable to write()\n"));
		_tprintf(_T("Error: %ls\n"), hid_error(handle));
	}


	// Request state (cmd 0x81). The first byte is the report number (0x1).
	buf[0] = 0x1;
	buf[1] = 0x81;
	hid_write(handle, buf, 17);
	if (res < 0)
		_tprintf(_T("Unable to write() (2)\n"));

	hid_close(handle);

	/* Free static HIDAPI objects. */
	hid_exit();

}


void COTPWriterProDlg::OnBnClickedButton2()
{
	// 1. 查找本系统中HID类的GUID标识
	GUID guidHID;
	HidD_GetHidGuid( &guidHID );
	CString s;
	s.Format( _T("HID 类的 GUID 标识: {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}\r\n")
		, guidHID.Data1, guidHID.Data2, guidHID.Data3
		, guidHID.Data4[0], guidHID.Data4[1], guidHID.Data4[2], guidHID.Data4[3]
		, guidHID.Data4[4], guidHID.Data4[5], guidHID.Data4[6], guidHID.Data4[7] );

	// 2. 获取设备信息，得到设备信息集的句柄
	s += _T("正在查找可用的 HID 设备...\r\n");
	HDEVINFO hDevInfo = SetupDiGetClassDevs( &guidHID, NULL/*_T("USB")*/, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE );
	if( INVALID_HANDLE_VALUE == hDevInfo ) {
		s += _T("无法找到设备信息集的句柄...\r\n");
		_tprintf_s(_T("%s"), s);
		//std::tcout << s;
		//m_ctrlHid2.SetWindowText(s);
		ShowError();
		return;
	}

	// 3. 遍历设备信息集中所有设备接口，得到其详细信息
	SP_DEVICE_INTERFACE_DATA InterfaceData = { sizeof(SP_DEVICE_INTERFACE_DATA) };
	for( DWORD index = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &guidHID, index, &InterfaceData); ++index )
	{
		/*
		Using function SetupDiGetDeviceInterfaceDetail() to get details about an interface is typically a two-step process:
		1. Get the required buffer size. 
		   Call SetupDiGetDeviceInterfaceDetail with a NULLDeviceInterfaceDetailData pointer, a DeviceInterfaceDetailDataSize of zero, and a valid RequiredSize variable. 
		   In response to such a call, this function returns the required buffer size at RequiredSize and fails with GetLastError returning ERROR_INSUFFICIENT_BUFFER.
		2. Allocate an appropriately sized buffer and call the function again to get the interface details.
		*/
		DWORD dwRequiredSize = 0;
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &InterfaceData, NULL, 0, &dwRequiredSize, NULL); 
		PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
		pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(dwRequiredSize); // create buffer of required size
		pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if( !SetupDiGetDeviceInterfaceDetail(hDevInfo, &InterfaceData, pDetail, dwRequiredSize, NULL, NULL) ) {
			ShowError();
			free(pDetail);
			continue;
		}

		s.AppendFormat( _T("[%d] path: %s\r\n"), index, pDetail->DevicePath );

		HANDLE hUsb = CreateFile( pDetail->DevicePath, 0//GENERIC_READ|GENERIC_WRITE
			, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if( INVALID_HANDLE_VALUE == hUsb ) {
			s += _T("无法打开端口。");
			_tprintf_s(_T("%s"), s);
			std::tcout << s;
			//m_ctrlHid2.SetWindowText(s);
			SetupDiDestroyDeviceInfoList( hDevInfo );
			free( pDetail );
			return;
		}

		// 查询设备标识
		HIDD_ATTRIBUTES strtAttrib = { sizeof(HIDD_ATTRIBUTES) };
		if( HidD_GetAttributes(hUsb,&strtAttrib) )
		{
			s.AppendFormat( _T("VendorID:\t%hX\r\n"), strtAttrib.VendorID );
			s.AppendFormat( _T("ProductID:\t%hX\r\n"), strtAttrib.ProductID );
			s.AppendFormat( _T("VerNumber:\t%hX\r\n"), strtAttrib.VersionNumber );
		}
		else
			ShowError();

		CloseHandle( hUsb );
		free( pDetail );
	}
	
	if( GetLastError() != ERROR_NO_MORE_ITEMS )
		ShowError();

	_tprintf_s(_T("%s"), s);
	//std::tcout << s;
	//m_ctrlHid2.SetWindowText(s);
	SetupDiDestroyDeviceInfoList( hDevInfo );
}

hid_device * COTPWriterProDlg::Hid_OpenTopLevelCollection( unsigned short vendorID, unsigned short productID )
{
	// use HIDAPI
	int res;
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	hid_device *handle;


	if (hid_init())
		return 0;

	/*struct hid_device_info *devs, *cur_dev;
	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	hid_free_enumeration(devs);*/

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	//handle = hid_open(0x4d8, 0x3f, NULL);
	handle = hid_open(vendorID, productID, NULL);
	if (!handle) {
		_tprintf(_T("unable to open device\n"));
		return handle;
	}

	// Read the Manufacturer String
	wstr[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		_tprintf(_T("Unable to read manufacturer string\n"));
	_tprintf(_T("Manufacturer String: %ls\n"), wstr);

	// Read the Product String
	wstr[0] = 0x0000;
	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		_tprintf(_T("Unable to read product string\n"));
	_tprintf(_T("Product String: %ls\n"), wstr);

	// Read the Serial Number String
	wstr[0] = 0x0000;
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (res < 0)
		_tprintf(_T("Unable to read serial number string\n"));
	_tprintf(_T("Serial Number String: (%d) %ls"), wstr[0], wstr);
	_tprintf(_T("\n"));

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);
	
	return handle;
}



void COTPWriterProDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			HID_REPORT_t r = {0};
			hid_device *handle = handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
			if (!handle) {
				_tprintf(_T("unable to open device\n"));
				return;
			}

			int res = hid_read(handle, (unsigned char *)&r.packet, sizeof(r.packet));
			if (res < 0) {
				_tprintf(_T("Unable to read().\n"));
				_tprintf(_T("Error: %ls\n"), hid_error(handle));
			}

			hid_close(handle); /* Free handle objects. */
			hid_exit(); /* Free static HIDAPI objects. */
		}
		break;
	

	
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void COTPWriterProDlg::OnBnClickedButtonOpenFile()
{
	m_ctrlProgress.SetPos(0);
	m_ctrlListBuffer.EndEdit(TRUE);

	

	CFileDialog dlg(TRUE);
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

	CString s;
	s.Format(_T("Open file: %s ...   0%%"), pathName);
	EditCtrlOutput(s);

	if (fileExt.CompareNoCase(_T("HEX")) == 0) 
    {
		mFile.Open(pathName, CFile::modeRead | CFile::typeText, &mExcept);
		unsigned int addr = 0;
		unsigned int curpos = 0;
        unsigned int fileLen = (unsigned int)mFile.GetLength();
        
        if (fileLen) 
        {
            ClearBuffer(0, m_bufDataLength);
        }
        else 
        {
            AfxMessageBox(_T("File is empty!"));
            mFile.Close();
            return;
        }
		
		while ((curpos = (unsigned int)mFile.GetPosition()) != fileLen) 
		{
			HexRecReadFromFile(m_buf, m_bufFlag, mFile, hr, addr);
			
			int percent = 100 * curpos / fileLen;
			m_ctrlProgress.SetPos(percent);
			s.Format(_T("%3d%%"), percent);
			EditCtrlOutput(s, -4);
		}
		m_bufDataLength = addr;
	}
	else if (fileExt.CompareNoCase(_T("BIN")) == 0) 
    {
		mFile.Open(pathName, CFile::modeRead | CFile::typeBinary, &mExcept);
		m_bufDataLength = mFile.Read(m_buf, mFile.GetLength());
        memset(m_bufFlag, 1, m_bufDataLength);
	}


	m_ctrlProgress.SetPos(100);
	EditCtrlOutput(s = _T("100%\r\n"), -4);
	mFile.Close();

    //UpdateBufferShow();
    UpdateBufferDisplay(0, m_bufDataLength);
}

void COTPWriterProDlg::OnBnClickedButtonSaveAs()
{
	m_ctrlProgress.SetPos(0);
	m_ctrlListBuffer.EndEdit(TRUE);
	
	if (m_bufDataLength == 0) {
		MessageBox(_T("No data to save."));
		return;
	}
	
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	CString pathName = dlg.GetPathName();
	CString fileExt = dlg.GetFileExt();

	CStdioFile mFile;
	CFileException mExcept;

	PrintCurrentTime();

	CString s;
	s.Format(_T("Save file: %s ...   0%%"), pathName);
	EditCtrlOutput(s);

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

			if (addr >= m_bufDataLength) {
				HexRec(hr, HEX_REC_EOF, 0, 0, NULL);
				HexRecSaveToFile(hr, mFile);
				break;
			}
			else if (addr && (addr % 0x10000 == 0)) {
				HexRec(hr, HEX_REC_ELA,  2, addr, m_buf);
				HexRecSaveToFile(hr, mFile);
			}
			rec_data_len = 16 < (m_bufDataLength-addr) ? 16 : (m_bufDataLength-addr);
            HexRec(hr, HEX_REC_DAT, rec_data_len, addr, m_buf);
			HexRecSaveToFile(hr, mFile);

			// progress
			int percent = 100*addr / m_bufDataLength;
			m_ctrlProgress.SetPos(percent);
			s.Format(_T("%3d%%"), percent);
			EditCtrlOutput(s, -4);
		}
	}
	else if (fileExt.CompareNoCase(_T("BIN")) == 0) {
		mFile.Open(pathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, &mExcept);
		m_ctrlSaveAppend.GetCheck() ? mFile.SeekToEnd() : mFile.SeekToBegin();
		mFile.Write(m_buf, m_bufDataLength);
	}
	else {
		MessageBox(_T("Only Hex/Bin file suported."));
		return;
	}

	m_ctrlProgress.SetPos(100);
	EditCtrlOutput(s = _T("100%\r\n"), -4);
	mFile.Close();
}


void COTPWriterProDlg::EditCtrlOutput( CString &s, int pos )
{
	// pos 为相对于最后位置的字符数
	int len = m_ctrlEdit.GetWindowTextLength(); 
	m_ctrlEdit.SetSel(len + pos,len); //将插入光标放在最后 
	m_ctrlEdit.ReplaceSel(s); 
	//m_ctrlEdit.ScrollWindow(0,0); //滚动到插入点 (滚动条始终在底部，不闪动)
}


void COTPWriterProDlg::OnBnClickedButtonWrite()
{
	if (m_bufDataLength == 0) {
		MessageBox(_T("No data to write."));
		return;
	}

	if (process_state != process_state_idle) return;
	process_state = process_state_write;

	// Open the device using the VID, PID, and optionally the Serial number.
	hid_device *handle = handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
	if (!handle) {
        process_state = process_state_idle;
        _tprintf(_T("unable to open device\n"));
		return;
	}

	// Start Write
	HID_REPORT_t r;
	unsigned int m_buf_num = 0;
	unsigned int start_addr = GetStartAddress();
	unsigned int length = min(GetDataLength(), m_bufDataLength);
    if (length == 0)
    {
        AfxMessageBox(_T("The lenth of data to write can't be 0!"));
        goto OnBnClickedButtonWrite_Label;
    }
	unsigned int packet_data_length = m_option.nPacketDataLength;
    if (packet_data_length == 0)
    {
        AfxMessageBox(_T("packet data length can't be 0!"));
        goto OnBnClickedButtonWrite_Label;
    }

	while (m_buf_num != length) 
	{
		// build the MemWrite packet.
		m_buf_num += BuildMemWriteReport(r, m_buf, start_addr+m_buf_num, length-m_buf_num, packet_data_length);

        if (SendAndWaitToReceiveReport(r, handle) < 0) 
            break;
		m_ctrlProgress.SetPos(m_buf_num*100/length);
	}
    // Send the terminating data-empty packet.
    BuildMemWriteReport(r, m_buf, start_addr+m_buf_num, length-m_buf_num, packet_data_length);
    SendAndWaitToReceiveReport(r, handle);

OnBnClickedButtonWrite_Label:
	hid_close(handle); /* Free handle objects. */
	hid_exit(); /* Free static HIDAPI objects. */

	process_state = process_state_idle;
}

void COTPWriterProDlg::OnBnClickedButtonRead()
{
	if (process_state != process_state_idle) return;
	process_state = process_state_read;

	// Open the device using the VID, PID, and optionally the Serial number.
	hid_device *handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
	if (!handle) {
        process_state = process_state_idle;
        _tprintf(_T("Unable to open device\n"));
		return;
	}

	HID_REPORT_t r;
	MemRead(handle, r, GetStartAddress(), GetDataLength());

	hid_close(handle); /* Free handle objects. */
	hid_exit(); /* Free static HIDAPI objects. */

	UpdateBufferDisplay(GetStartAddress(), GetDataLength());
	process_state = process_state_idle;
}

void COTPWriterProDlg::OnBnClickedButtonVerify()
{
	// if (isEmpty(buf)) { messagebox(error); return;}
	if (m_bufDataLength == 0) {
		MessageBox(_T("There's no data in the buffer to verify."));
		return;
	}

	if (process_state != process_state_idle) return;
	process_state = process_state_verify;


	// open HID device
	// Open the device using the VID, PID, and optionally the Serial number.
	hid_device *handle = handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
	if (!handle) {
        process_state = process_state_idle;
        _tprintf(_T("unable to open device\n"));
		return;
	}

    unsigned int start_addr = GetStartAddress();
	unsigned int total_length = GetDataLength();
	total_length = total_length <= m_bufDataLength ? total_length : m_bufDataLength;
	unsigned int addr = start_addr;
	unsigned int length = 16;
	HID_REPORT_t r;
	int res = 0;
	// loop
	for (int i = 0; i < m_bufDataLength; i++, addr += length)
	{
		// read N bytes to a temp buffer
		if (MemRead(handle, r, addr, length) == 0) {
			_tprintf(_T("Verification error (addr | mem_data | buffer_data): %08X | %02X | %02X\r\n"), addr+res, r.packet.m_pkt.memPkt.data[i], m_buf[i]);
			goto OnBnClickedButtonVerify_local_exit;
		}
		
		// compare the temp buffer with the buf
		res = CompareMemData(r, m_buf);

		// if error, message(error), return; else continue.
		if (res != -1) {
			_tprintf(_T("Verification error (addr | mem_data | buffer_data): %08X | %02X | %02X\r\n"), addr+res, r.packet.m_pkt.memPkt.data[i], m_buf[i]);
			goto OnBnClickedButtonVerify_local_exit;
		}

	// end loop
	}

	// ok, output successful.
	_tprintf(_T("Verification is successful."));

OnBnClickedButtonVerify_local_exit:
	hid_close(handle); /* Free handle objects. */
	hid_exit(); /* Free static HIDAPI objects. */

	process_state = process_state_idle;
}

void COTPWriterProDlg::OnBnClickedButtonEncrypt()
{
	return ExecuteMemCmd(HS__MEM__WRDIS);

}

void COTPWriterProDlg::OnBnClickedButtonErase()
{
	return m_ctrlEraseAll.GetCheck() ? 
          ExecuteMemCmd(HS__MEM__ERASE_ALL) 
        : ExecuteMemCmd(HS__MEM__ERASE_PAGE);
}

void COTPWriterProDlg::PrintCurrentTime()
{
	CString s;
	CTime t = CTime::GetCurrentTime();
	s.Format(_T("%04d-%02d-%02d %02d:%02d:%02d:  "), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	EditCtrlOutput(s);
}

int COTPWriterProDlg::BuildMemWriteReport( HID_REPORT_t &r, unsigned char *buf, unsigned int addr, unsigned int length, unsigned int packetDataLength, unsigned int reportID /*= 0 */ )
{
	int datalen = length <= packetDataLength ? length : packetDataLength;
	
	r.reportID = reportID;
	r.packet.m_pkt.memPkt.len = 12 + datalen;
    r.packet.m_pkt.memPkt.csb = 0;
	r.packet.m_pkt.memPkt.cmdL1 = HS__MEM;
	r.packet.m_pkt.memPkt.cmdL2 = HS__MEM__WRITE;
	r.packet.m_pkt.memPkt.addr = addr;
	r.packet.m_pkt.memPkt.dataLen = datalen;
	memcpy(r.packet.m_pkt.memPkt.data, buf+addr, datalen);

	hgzRevertByteOrder((unsigned char *)&r.packet.m_pkt.memPkt.addr, 4);
	hgzRevertByteOrder((unsigned char *)&r.packet.m_pkt.memPkt.dataLen, 4);

	return datalen;
}

BOOL COTPWriterProDlg::BuildMemReadReport( HID_REPORT_t &r, unsigned int addr, unsigned int length, unsigned int reportID /*= 0 */ )
{
	r.reportID = reportID;
	r.packet.m_pkt.memPkt.len = 12;
    r.packet.m_pkt.memPkt.csb = 0;
	r.packet.m_pkt.memPkt.cmdL1 = HS__MEM;
	r.packet.m_pkt.memPkt.cmdL2 = HS__MEM__READ;
	r.packet.m_pkt.memPkt.addr = addr;
	r.packet.m_pkt.memPkt.dataLen = length;

	hgzRevertByteOrder((unsigned char *)&r.packet.m_pkt.memPkt.addr, 4);
	hgzRevertByteOrder((unsigned char *)&r.packet.m_pkt.memPkt.dataLen, 4);

	return TRUE;
}

BOOL COTPWriterProDlg::BuildMemCmdReport( HID_REPORT_t &r, unsigned char memCmd, unsigned int reportID/*=0 */ )
{
	r.reportID = reportID;
	r.packet.m_pkt.cmdPkt.len = sizeof(CPacket::HS_CMD_PACKET_t);
    r.packet.m_pkt.cmdPkt.csb = 0;
	r.packet.m_pkt.cmdPkt.cmdL1 = HS__MEM;
	r.packet.m_pkt.cmdPkt.cmdL2 = memCmd;
	
	switch (memCmd)
	{
	case HS__MEM__ENTER:
	case HS__MEM__EXIT:
	case HS__MEM__WREN:
	case HS__MEM__WRDIS:
	case HS__MEM__ACCESS_DISABLE:
	case HS__MEM__READ_DISABLE:
	case HS__MEM__WRITE_DISABLE:
	case HS__MEM__BLKCHK:
	case HS__MEM__RESET:
	case HS__MEM__TEST_BLKCHK:
	case HS__MEM__TEST_DEC:
	case HS__MEM__TEST_WR:
	case HS__MEM__ERASE_ALL:
	case HS__MEM__RDFPCR:
	case HS__MEM__RDSR:
		break;

	case HS__MEM__ERASE_PAGE:
		{
			r.packet.m_pkt.cmdPkt.len += 8;
			CString s;
			m_ctrlErasePageNum.GetWindowText(s);
			s.Trim();
			if (s.IsEmpty()) 
                return FALSE;

            CStringArray arr;
            hgzExtractSubStrings1(arr, s, _T("-"));
            if (arr.GetSize() > 2)
                return FALSE;
            if (arr.GetSize() == 1)
                arr.SetAtGrow(1, arr[0].GetString());
            arr[0].Trim();
            arr[1].Trim();

			r.packet.m_pkt.memPkt.addr = stoul(arr[0].GetString(), 0, 10);
			r.packet.m_pkt.memPkt.dataLen = stoul(arr[1].GetString(), 0, 10);
			
			hgzRevertByteOrder((unsigned char *)&r.packet.m_pkt.memPkt.addr, 4);
			hgzRevertByteOrder((unsigned char *)&r.packet.m_pkt.memPkt.dataLen, 4);			

			return TRUE;
		}
	case HS__MEM__WRSR:
		{
			r.packet.m_pkt.cmdPkt.len += 1;
			r.packet.m_pkt.valPkt.val = 0x00; // value to set

			return TRUE;
		}

	default:
		break;
	}

	return TRUE;
}

BOOL COTPWriterProDlg::BuildCmdReport( HID_REPORT_t &r, unsigned char cmd, unsigned int reportID/*=0 */ )
{
    r.reportID = reportID;
    r.packet.m_pkt.cmdPkt.len = sizeof(CPacket::HS_CMD_PACKET_t);
    r.packet.m_pkt.cmdPkt.csb = 0;
    r.packet.m_pkt.cmdPkt.cmdL1 = HS__CMD;
    r.packet.m_pkt.cmdPkt.cmdL2 = cmd;

    switch (cmd)
    {
    case HS__CMD__SET__CHIP_TYPE:
        {
            if (m_ChipType == 0) 
                return FALSE;

            r.packet.m_pkt.valPkt.val = m_ChipType;
            r.packet.m_pkt.cmdPkt.len += 1;
            
            return TRUE;
        }

    case HS__CMD__GET__CHIP_TYPE:
    case HS__CMD__GET__FIRMWARE_VERSION:
            return TRUE;

    default:
        break;
    }

    return TRUE;
}

int COTPWriterProDlg::ReceiveReport( hid_device * handle, HID_REPORT_t &r )
{
	HID_REPORT_t r1;
	r1.reportID = r.reportID;
	r1.packet.m_pkt.memPkt.cmdL1 = r.packet.m_pkt.memPkt.cmdL1;
	r1.packet.m_pkt.memPkt.cmdL2 = r.packet.m_pkt.memPkt.cmdL2;

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);
	// detect if device is ready for receiving another packet of data
	// Read requested state. hid_read() has been set to be
	// non-blocking by the call to hid_set_nonblocking() above.
	// This loop demonstrates the non-blocking nature of hid_read().
	int res = 0;
	_tprintf(_T("Receiving Report ...\n"));
	while (res == 0) {
		res = hid_read(handle, (unsigned char *)&r.packet, sizeof(r.packet));
// 		if (res == 0)
// 			_tprintf(_T("."));
		if (res < 0)
			_tprintf(_T("Unable to read()\n"));
		//Sleep(500);
	}
	_tprintf(_T("Received data: "));
    r.packet.print(FALSE);
	
    /*if (r.packet.m_pkt.memPkt.cmdL1 == HS__MEM 
		&& r.packet.m_pkt.memPkt.cmdL2 == HS__MEM__WRITE
		&& r.packet.m_pkt.memPkt.dataLen == 0)*/
	if (r.packet.m_pkt.memPkt.cmdL1 == r1.packet.m_pkt.memPkt.cmdL1 && 
		r.packet.m_pkt.memPkt.cmdL2 == r1.packet.m_pkt.memPkt.cmdL2)
	{
		res = 1;
	}
	else {
		tcout << endl << _T("Read error!") << endl;
		res = -1;
	}	
	
	return res;
}

int COTPWriterProDlg::SendReport( hid_device * handle, HID_REPORT_t &r )
{
	r.packet.print(TRUE);
    // Set the hid_write() function to be blocking.
	hid_set_nonblocking(handle, 1);
	// send HS__MEM__WRITE packet.
	int res = hid_write(handle, (unsigned char *)&r, sizeof(r));
	
	CString s(_T(":)"));
	/*switch (r.packet.m_pkt.memPkt.cmdL2)
	{
	case HS__MEM__WRITE:
		s = "HS__MEM__WRITE";
		break;

	case HS__MEM__READ:
		s = "HS__MEM__READ";
		break;

	default:
		break;
	}*/

	if (res < 0) {
		_tprintf(_T("Unable to send command: %s\n"), s);
		_tprintf(_T("Error: %ls\n"), hid_error(handle));
		return -1;
	}

	return 1;
}



afx_msg LRESULT COTPWriterProDlg::OnLvmItemChanged(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("Received WM_LVM_ENDEDIT message: %d, %d\n"), wParam, lParam);

    unsigned int addr = wParam*16 + lParam-1;

	CString s;
	s = m_ctrlListBuffer.GetItemText(wParam, lParam);
	s.Trim();
	if (s.GetLength() == 0) {
		m_buf[addr] = 0;
        m_bufFlag[addr] = 0;
	}
	else {
		m_buf[addr] = stoul(s.GetString(), 0, 16);
		s.Format(_T("%02X"), m_buf[addr]);
		m_ctrlListBuffer.SetItemText(wParam, lParam, s);
        m_bufFlag[addr] = 1;
        m_bufDataLength = max(addr+1, m_bufDataLength);
	}

	return 0;
}

unsigned int COTPWriterProDlg::MemRead( hid_device * handle, HID_REPORT_t &r, unsigned int startAddr, unsigned int length )
{
	BuildMemReadReport(r, startAddr, length);

    long long receivedNum = 0;
	// Start Write
		int res = 0;
		res = SendReport(handle, r);
		if (res < 0) {
			process_state = process_state_idle;
			return 0;
		}

        ClearBuffer(0, m_bufDataLength);

		while (process_state == process_state_read) 
		{
			res = ReceiveReport(handle, r);
			if (res < 0) break;

			unsigned int dataLen = hgzRevertByteOrder32(r.packet.m_pkt.memPkt.dataLen);
            unsigned int addr = hgzRevertByteOrder32(r.packet.m_pkt.memPkt.addr);
			if (dataLen == 0) {
				m_ctrlProgress.SetPos(100);
				break;
			}
			else {
				receivedNum += dataLen;
				memcpy(m_buf+addr, r.packet.m_pkt.memPkt.data, dataLen);
                memset(m_bufFlag+addr, 1, dataLen);
                if (m_bufDataLength < (addr + dataLen))
                    m_bufDataLength = addr + dataLen;
			}
			m_ctrlProgress.SetPos(receivedNum*100/length);
		}
        return (unsigned int)receivedNum;
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
	unsigned int length = s.GetLength() ? stoul((tstring)s.GetString()) : 0;
	
	return length;
}

int COTPWriterProDlg::CompareMemData( HID_REPORT_t & r, unsigned char *buf )
{
	for (int i = 0; i < r.packet.m_pkt.memPkt.dataLen ; i++)
	{
		if (r.packet.m_pkt.memPkt.data[i] != buf[r.packet.m_pkt.memPkt.addr + i]) {
			return i; // index of the first error byte.
		}
	}

	return -1; // successful
}

afx_msg void COTPWriterProDlg::ExecuteMemCmd( unsigned char memCmd )
{
	if (process_state != process_state_idle) return;
	process_state = (enum _FLAG_t)memCmd;

	HID_REPORT_t r;
	
	if (BuildMemCmdReport(r, memCmd) == FALSE) {
		process_state = process_state_idle;
		return;
	}

	// Open the device using the VID, PID, and optionally the Serial number.
	hid_device *handle = handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
	if (!handle) {
        process_state = process_state_idle;
        _tprintf(_T("Unable to open device\n"));
		return;
	}

	int res = 0;
	res = SendReport(handle, r);
	if (res < 0) {

	}
	res = ReceiveReport(handle, r);
	if (res < 0) {

	}

	hid_close(handle); /* Free handle objects. */
	hid_exit(); /* Free static HIDAPI objects. */

	process_state = process_state_idle;
}

afx_msg void COTPWriterProDlg::ExecuteCmd( unsigned char cmd )
{
    if (process_state != process_state_idle) return;
    process_state = (enum _FLAG_t)cmd;

    HID_REPORT_t r;

    if (BuildCmdReport(r, cmd) == FALSE) {
        process_state = process_state_idle;
        return;
    }

    // Open the device using the VID, PID, and optionally the Serial number.
    hid_device *handle = handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
    if (!handle) {
        process_state = process_state_idle;
        _tprintf(_T("Unable to open device\n"));
        return;
    }

    int res = 0;
    res = SendReport(handle, r);
    if (res < 0) {

    }
    res = ReceiveReport(handle, r);
    if (res < 0) {

    }

    hid_close(handle); /* Free handle objects. */
    hid_exit(); /* Free static HIDAPI objects. */

    process_state = process_state_idle;
}

void COTPWriterProDlg::OnBnClickedButtonBlankCheck()
{
	ExecuteMemCmd(HS__MEM__BLKCHK);
}


void COTPWriterProDlg::OnBnClickedButtonEnterProgramMode()
{
	ExecuteMemCmd(HS__MEM__ENTER);
}


void COTPWriterProDlg::OnBnClickedButtonExitProgramMode()
{
	ExecuteMemCmd(HS__MEM__EXIT);
}


void COTPWriterProDlg::OnBnClickedButtonTestBlankCheck()
{
	ExecuteMemCmd(HS__MEM__TEST_BLKCHK);
}


void COTPWriterProDlg::OnBnClickedButtonTestDec()
{
	ExecuteMemCmd(HS__MEM__TEST_DEC);
}


void COTPWriterProDlg::OnBnClickedButtonTestWR()
{
	ExecuteMemCmd(HS__MEM__TEST_WR);
}


void COTPWriterProDlg::OnBnClickedButtonChipReset()
{
	ExecuteMemCmd(HS__MEM__RESET);
}


void COTPWriterProDlg::OnBnClickedButtonInBuffer()
{
	UINT8 data = GetDataToFillBuffer();
    UINT addr;
    UINT dataLen;

    if (m_chkFillBufferAll.GetCheck())
    {
        addr = 0;
        dataLen = m_bufDataLength;
    }
    else
    {
        addr = GetStartAddress();
        dataLen = GetDataLength();
    }

    memset(m_buf+addr, data, dataLen);
    memset(m_bufFlag+addr, 1, dataLen);

    if (m_bufDataLength < addr+dataLen)
        m_bufDataLength = addr + dataLen;

    UpdateBufferDisplay(addr, dataLen);
}

int COTPWriterProDlg::SendAndWaitToReceiveReport( HID_REPORT_t &r, hid_device * handle )
{
    int res = 0;
    res = SendReport(handle, r);
    if (res < 0) 
        return res;
    
    res = ReceiveReport(handle, r);
    if (res < 0) 
        return res;

    return res;
}

void COTPWriterProDlg::UpdateBufferDisplay( unsigned int addr, unsigned int length )
{
    CString s;
    for (int i = addr; i < addr+length ; i++)
    {
        if (m_bufFlag[i]) 
            s.Format(_T("%02X"), m_buf[i]);
        else 
            s.Empty();

        m_ctrlListBuffer.SetItemText(i/16, i%16 + 1, s);
    }
}


void COTPWriterProDlg::OnBnClickedButtonOption()
{
    COption o;
    o.m_bEnableConsoleOutput = m_option.bEnableConsoleOutput;
    o.m_bEnableIncontinuousCell = m_option.bEnableIncontinuousCell;
    o.m_nPacketDataLength = m_option.nPacketDataLength;

    if (o.DoModal() == IDCANCEL)
    {
        return;
    }

    m_option.bEnableConsoleOutput = o.m_bEnableConsoleOutput;
    m_option.bEnableIncontinuousCell = o.m_bEnableIncontinuousCell;
    m_option.nPacketDataLength = o.m_nPacketDataLength;

    if (m_option.bEnableConsoleOutput)
        hgzOpenConsole();
    else
        hgzCloseConsole();
}

void COTPWriterProDlg::SetBuffer( unsigned int addr, long long lenth, unsigned char val, BOOL valid )
{
    memset(m_buf+addr, val, lenth);
    
    if (valid)
        memset(m_bufFlag+addr, 1, lenth);
    else
        memset(m_bufFlag+addr, 0, lenth);
}

void COTPWriterProDlg::ClearBuffer( UINT32 addr, UINT64 length )
{
    UINT64 nl;
    
    if (m_bufDataLength <= addr)
        return;
    else if (m_bufDataLength <= addr+length)
    {
        nl = m_bufDataLength - addr;
        m_bufDataLength = addr;
    }
    else
    {
        nl = length;
    }
    
    memset(m_buf+addr, 0, nl);
    memset(m_bufFlag+addr, 0, nl);
}

void COTPWriterProDlg::OnBnClickedButtonClearBuffer()
{
    UINT32 addr;
    UINT64 dataLen;

    if (m_chkClearBufferAll.GetCheck())
    {
        addr = 0;
        dataLen = m_bufDataLength;
    }
    else
    {
        addr = GetStartAddress();
        dataLen = GetDataLength();
    }

    ClearBuffer(addr, dataLen);
    UpdateBufferDisplay(addr, dataLen);
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
    CString s;
    m_ctrlChipSel.GetLBText(m_ctrlChipSel.GetCurSel(), s);

    if (s.IsEmpty())
        m_ChipType = HS__CMD__CHIP_TYPE__NONE;
    else if (s.Compare(_T("HS6206")) == 0)
        m_ChipType = HS__CMD__CHIP_TYPE__OTP__HS6206;
    else if (s.Compare(_T("EN25T80")) == 0)
        m_ChipType = HS__CMD__CHIP_TYPE__FLASH__EN25T80;
    else
        m_ChipType = HS__CMD__CHIP_TYPE__NONE;


    ExecuteCmd(HS__CMD__SET__CHIP_TYPE);
}


void COTPWriterProDlg::OnBnClickedButtonDetectChipType()
{
    return ExecuteCmd(HS__CMD__GET__CHIP_TYPE);
}


void COTPWriterProDlg::OnBnClickedButtonVersionNum()
{
    return ExecuteCmd(HS__CMD__GET__FIRMWARE_VERSION);
}
