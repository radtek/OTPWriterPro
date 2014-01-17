
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
	DDX_Control(pDX, IDC_CHECK2, m_ctrlDebugMode);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
	DDX_Control(pDX, IDC_COMBO5, m_ctrlPacketDataLength);
	DDX_Control(pDX, IDC_CHECK3, m_ctrlSaveAppend);
	DDX_Control(pDX, IDC_LIST1, m_ctrlListBuffer);
}

BEGIN_MESSAGE_MAP(COTPWriterProDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &COTPWriterProDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &COTPWriterProDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON7, &COTPWriterProDlg::OnBnClickedButton7)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON6, &COTPWriterProDlg::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON4, &COTPWriterProDlg::OnBnClickedButtonOpenFile)
	ON_BN_CLICKED(IDC_CHECK2, &COTPWriterProDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON5, &COTPWriterProDlg::OnBnClickedButtonSaveAs)
	ON_BN_CLICKED(IDC_BUTTON9, &COTPWriterProDlg::OnBnClickedButtonVerify)
	ON_BN_CLICKED(IDC_BUTTON10, &COTPWriterProDlg::OnBnClickedButtonEncrypt)
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
	m_ctrlDebugMode.SetCheck(TRUE);
	if (m_ctrlDebugMode.GetCheck())	hgzOpenConsole();

	m_ctrlChipSel.SetCurSel(0);
	m_ctrlMemAddrBegin.SetWindowText(_T("0000"));
	m_ctrlDataLength.SetWindowText(_T("16384"));
	m_ctrlPacketDataLength.SetWindowText(_T("16"));

	memset(wbuf, 0, sizeof(wbuf));
	wbuf_num = 0;
	wbuf_data_length = 0;
	memset(rbuf, 0, sizeof(rbuf));
	rbuf_num = 0;
	rbuf_data_length = 0;
	process_state = process_state_idle;

	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetPos(0);


	
	m_ctrlListBuffer.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	// set columns
	CString s;
	m_ctrlListBuffer.InsertColumn(0, _T(""), 0, 86);
	for (int i = 0; i < 16; i++)
	{
		s.Format(_T("%d"), i);
		m_ctrlListBuffer.InsertColumn(i+1, s, 0, 26);
	}
	// lock column width
	m_ctrlListBuffer.GetHeaderCtrl()->EnableWindow(FALSE);

	// set rows (items)
	m_ctrlListBuffer.SetItemCount(16384/16);
	for (unsigned int i = 0; i < 16384/16; i++) {
		s.Format(_T("%04X %04X"), (unsigned __int16)(i>>12), (unsigned __int16)(i<<4));
		m_ctrlListBuffer.InsertItem(i, s);
	}








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
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);


	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	//handle = hid_open(0x4d8, 0x3f, NULL);
	handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
	if (!handle) {
		printf("unable to open device\n");
		return;
	}

	// Read the Manufacturer String
	wstr[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read manufacturer string\n");
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	wstr[0] = 0x0000;
	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read product string\n");
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	wstr[0] = 0x0000;
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read serial number string\n");
	printf("Serial Number String: (%d) %ls", wstr[0], wstr);
	printf("\n");

	// Read Indexed String 1
	wstr[0] = 0x0000;
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read indexed string 1\n");
	printf("Indexed String 1: %ls\n", wstr);

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
		printf("Unable to send a feature report.\n");
	}

	memset(buf,0,sizeof(buf));

	// Read a Feature Report from the device
	buf[0] = 0x2;
	res = hid_get_feature_report(handle, buf, sizeof(buf));
	if (res < 0) {
		printf("Unable to get a feature report.\n");
		printf("%ls", hid_error(handle));
	}
	else {
		// Print out the returned buffer.
		printf("Feature Report\n   ");
		for (i = 0; i < res; i++)
			printf("%02hhx ", buf[i]);
		printf("\n");
	}

	memset(buf,0,sizeof(buf));

	// Toggle LED (cmd 0x80). The first byte is the report number (0x1).
	buf[0] = 0x1;
	buf[1] = 0x80;
	res = hid_write(handle, buf, 17);
	if (res < 0) {
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
	}


	// Request state (cmd 0x81). The first byte is the report number (0x1).
	buf[0] = 0x1;
	buf[1] = 0x81;
	hid_write(handle, buf, 17);
	if (res < 0)
		printf("Unable to write() (2)\n");

	// Read requested state. hid_read() has been set to be
	// non-blocking by the call to hid_set_nonblocking() above.
	// This loop demonstrates the non-blocking nature of hid_read().
	res = 0;
	while (res == 0) {
		res = hid_read(handle, buf, sizeof(buf));
		if (res == 0)
			printf("waiting...\n");
		if (res < 0)
			printf("Unable to read()\n");
#ifdef WIN32
		Sleep(500);
#else
		usleep(500*1000);
#endif
	}

	printf("Data read:\n   ");
	// Print out the returned buffer.
	for (i = 0; i < res; i++)
		printf("%02hhx ", buf[i]);
	printf("\n");

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


void COTPWriterProDlg::OnBnClickedButton7()
{
	HIDREPORT_t r;

	r.report_id = 0;
	r.packet.m_pkt.MemPkt.pl = 11;
	r.packet.m_pkt.MemPkt.pid = HS_PID_MEM;
	r.packet.m_pkt.MemPkt.cmd = HS_MEM_READ;

	CString s;
	m_ctrlMemAddrBegin.GetWindowText(s);
	r.packet.m_pkt.MemPkt.address.whole = s.GetLength() ? stoul((tstring)s.GetString(), 0, 16) : 0;
	hgzRevertByteOrder(r.packet.m_pkt.MemPkt.address.bytearr, 4);

	m_ctrlDataLength.GetWindowText(s);
	r.packet.m_pkt.MemPkt.data_length.whole = s.GetLength() ? stoul((tstring)s.GetString()) : 0;
	hgzRevertByteOrder(r.packet.m_pkt.MemPkt.data_length.bytearr, 4);
	
	r.packet.print();


	// Open the device using the VID, PID, and optionally the Serial number.
	hid_device *handle = handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
	if (!handle) {
		printf("unable to open device\n");
		return;
	}
	
	// send HS_MEM_READ packet.
	int res = hid_write(handle, (unsigned char *)&r, sizeof(r));
	if (res < 0) {
		printf("Unable to send HS_MEM_READ command.\n");
		printf("Error: %ls\n", hid_error(handle));
	}
	
	wbuf_num = 0;
	SetTimer(2, 1, NULL);



	hid_close(handle); /* Free handle objects. */
	hid_exit(); /* Free static HIDAPI objects. */
}

hid_device * COTPWriterProDlg::Hid_OpenTopLevelCollection( unsigned short vendor_id, unsigned short product_id )
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
	handle = hid_open(vendor_id, product_id, NULL);
	if (!handle) {
		printf("unable to open device\n");
		return handle;
	}

	// Read the Manufacturer String
	wstr[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read manufacturer string\n");
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	wstr[0] = 0x0000;
	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read product string\n");
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	wstr[0] = 0x0000;
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read serial number string\n");
	printf("Serial Number String: (%d) %ls", wstr[0], wstr);
	printf("\n");

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
			HIDREPORT_t r = {0};
			hid_device *handle = handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
			if (!handle) {
				printf("unable to open device\n");
				return;
			}

			int res = hid_read(handle, (unsigned char *)&r, sizeof(r));
			if (res < 0) {
				printf("Unable to read().\n");
				printf("Error: %ls\n", hid_error(handle));
			}

			hid_close(handle); /* Free handle objects. */
			hid_exit(); /* Free static HIDAPI objects. */
		}
		break;
	
	case 2:
		{
			HIDREPORT_t r = {0};
			hid_device *handle = handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
			if (!handle) {
				printf("unable to open device\n");
				return;
			}

			int res = hid_read(handle, (unsigned char *)&r, sizeof(r));
			if (res < 0) {
				printf("Unable to read().\n");
				printf("Error: %ls\n", hid_error(handle));
			}

			if (r.packet.m_pkt.MemPkt.data_length.whole == 0) {
				KillTimer(2);
				//SaveMemFile();
			}
			else {
				memcpy(&rbuf[rbuf_num], r.packet.m_pkt.MemPkt.data, r.packet.m_pkt.MemPkt.data_length.whole);
				rbuf_num += r.packet.m_pkt.MemPkt.data_length.whole;
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


void COTPWriterProDlg::OnBnClickedButtonWrite()
{
	if (wbuf_data_length == 0) {
		MessageBox(_T("No data to write."));
		return;
	}
	
	HIDREPORT_t r;

	r.report_id = 0;
	r.packet.m_pkt.MemPkt.pl = 11;
	r.packet.m_pkt.MemPkt.pid = HS_PID_MEM;
	r.packet.m_pkt.MemPkt.cmd = HS_MEM_WRITE;

	CString s;
	m_ctrlMemAddrBegin.GetWindowText(s);
	unsigned int addr = s.GetLength() ? stoul((tstring)s.GetString(), 0, 16) : 0;
	//hgzRevertByteOrder((unsigned char *)&addr, 4);
	//r.packet.m_pkt.MemPkt.address.whole = s.GetLength() ? stoul((tstring)s.GetString(), 0, 16) : 0;
	//hgzRevertByteOrder(r.packet.m_pkt.MemPkt.address.bytearr, 4);

	m_ctrlDataLength.GetWindowText(s);
	unsigned int length = s.GetLength() ? stoul((tstring)s.GetString(), 0, 10) : 0;
	if (length > wbuf_data_length) length = wbuf_data_length;
	//hgzRevertByteOrder((unsigned char *)&length, 4);
	//r.packet.m_pkt.MemPkt.data_length.whole = s.GetLength() ? stoul((tstring)s.GetString()) : 0;
	//hgzRevertByteOrder(r.packet.m_pkt.MemPkt.data_length.bytearr, 4);


#if TEST_ON == 0
	// Open the device using the VID, PID, and optionally the Serial number.
	hid_device *handle = handle = hid_open(HS_VENDOR_ID, HS_PRODUCT_ID_OTPWRITER, NULL);
	if (!handle) {
		printf("unable to open device\n");
		return;
	}
#endif

	// Start Write
	if (process_state == process_state_idle) {
		process_state = process_state_write;

		unsigned int packet_data_length = sizeof(r.packet.m_pkt.MemPkt.data);
		if (m_ctrlPacketDataLength.GetWindowTextLength()) {
			m_ctrlPacketDataLength.GetWindowText(s);
			unsigned int x = stoul((tstring)s.GetString());
			if (x > 0 && x < packet_data_length) packet_data_length = x;
		}
		
		wbuf_num = 0;
		while (wbuf_num != length) {
			// fill data in the packet.
			r.report_id = 0;
			r.packet.m_pkt.MemPkt.pl = 11;
			r.packet.m_pkt.MemPkt.pid = HS_PID_MEM;
			r.packet.m_pkt.MemPkt.cmd = HS_MEM_WRITE;
			r.packet.m_pkt.MemPkt.address.whole = addr + wbuf_num;
			r.packet.m_pkt.MemPkt.data_length.whole = packet_data_length <= (length - wbuf_num) ? packet_data_length : (length - wbuf_num);
			memcpy(r.packet.m_pkt.MemPkt.data, &wbuf[wbuf_num], r.packet.m_pkt.MemPkt.data_length.whole);
			r.packet.m_pkt.MemPkt.pl = 11 + r.packet.m_pkt.MemPkt.data_length.whole;
			wbuf_num += r.packet.m_pkt.MemPkt.data_length.whole;
			
			r.packet.print();

			hgzRevertByteOrder(r.packet.m_pkt.MemPkt.address.bytearr, 4);
			hgzRevertByteOrder(r.packet.m_pkt.MemPkt.data_length.bytearr, 4);
			

#if TEST_ON == 0
			// send HS_MEM_WRITE packet.
			int res = hid_write(handle, (unsigned char *)&r, sizeof(r));
			if (res < 0) {
				printf("Unable to send HS_MEM_READ command.\n");
				printf("Error: %ls\n", hid_error(handle));
			}

			////////////////
			// Set the hid_read() function to be non-blocking.
			hid_set_nonblocking(handle, 1);
			// detect if device is ready for receiving another packet of data
			// Read requested state. hid_read() has been set to be
			// non-blocking by the call to hid_set_nonblocking() above.
			// This loop demonstrates the non-blocking nature of hid_read().
			r.report_id = 0;
			res = 0;
			while (res == 0) {
				res = hid_read(handle, (unsigned char *)&r, sizeof(r));
				if (res == 0)
					printf("waiting...\n");
				if (res < 0)
					printf("Unable to read()\n");
				Sleep(500);
			}
			if (r.packet.m_pkt.MemPkt.pid == HS_PID_MEM 
				&& r.packet.m_pkt.MemPkt.cmd == HS_MEM_WRITE
				&& r.packet.m_pkt.MemPkt.data_length.whole == 0)
			{
				process_state = process_state_idle;
			}
			else {
				tcout << endl << _T("Write error!") << endl;
				break;
			}
#else
			process_state = process_state_idle;
#endif
		
			m_ctrlProgress.SetPos(wbuf_num*100/length);
		}
	}


#if TEST_ON == 0
	hid_close(handle); /* Free handle objects. */
	hid_exit(); /* Free static HIDAPI objects. */
#endif
}


void COTPWriterProDlg::OnBnClickedButtonOpenFile()
{
	m_ctrlProgress.SetPos(0);

	CFileDialog dlg(TRUE);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	CString PathName = dlg.GetPathName();
	CString FileExt = dlg.GetFileExt();

	CStdioFile mFile;
	CFileException mExcept;
	
	wbuf_data_length = 0;
	HEXRECORD_t hr; 

	CString s;
	CTime t;
	s.Format(_T("%s: "), t.Format(_T("::")));
	EditCtrlOutput(s);
	s.Format(_T("%s : Open file: %s ...   0%%"), t.Format(_T("::")), PathName);
	EditCtrlOutput(s);

	if (FileExt.CompareNoCase(_T("HEX")) == 0) {
		mFile.Open(PathName, CFile::modeRead | CFile::typeText, &mExcept);
		unsigned int addr = 0;
		unsigned int filelen = mFile.GetLength();
		unsigned int curpos = 0;
		while (mFile.GetPosition() != mFile.GetLength()) 
		{
			HexRecReadFromFile(wbuf, mFile, hr, addr);
			
			curpos = mFile.GetPosition();
			int percent = 100 * curpos / filelen;
			m_ctrlProgress.SetPos(percent);
			s.Format(_T("%3d%%"), percent);
			EditCtrlOutput(s, -4);
		}
		wbuf_data_length = addr;
	}
	else if (FileExt.CompareNoCase(_T("BIN")) == 0) {
		mFile.Open(PathName, CFile::modeRead | CFile::typeBinary, &mExcept);
		wbuf_data_length = mFile.Read(wbuf, mFile.GetLength());
	}

	m_ctrlProgress.SetPos(100);
	EditCtrlOutput(s = _T("100%\r\n"), -4);
	mFile.Close();
}


void COTPWriterProDlg::OnBnClickedCheck2()
{
	if (m_ctrlDebugMode.GetCheck()) hgzOpenConsole();
	else hgzCloseConsole();
}


void COTPWriterProDlg::OnBnClickedButtonSaveAs()
{
	m_ctrlProgress.SetPos(0);
	
	if (wbuf_data_length == 0) {
		MessageBox(_T("No data to save."));
		return;
	}
	
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	CString PathName = dlg.GetPathName();
	CString FileExt = dlg.GetFileExt();

	CStdioFile mFile;
	CFileException mExcept;

	CString s;
	s.Format(_T("Save file: %s ...   0%%"), PathName);
	EditCtrlOutput(s);

	if (FileExt.CompareNoCase(_T("HEX")) == 0) {
		HEXRECORD_t hr;
		mFile.Open(PathName, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &mExcept);
		
		/*auto checksum = [&] () -> unsigned char {
			unsigned char x = hr.reclen + hr.addrh + hr.addrl + hr.rectype;
			for (unsigned int i = 0; i < hr.reclen; i++){
				x += hr.data[i];
			}
			x = ~x + 1;
			hr.data[hr.reclen] = x;
			return x;
		};*/

		for (unsigned int addr = 0; ; addr += 16)
		{

			if (addr >= wbuf_data_length) {
				HexRec(hr, HEX_REC_EOF, 0, 0, NULL);
				HexRecSaveToFile(hr, mFile);
				break;
			}
			else if (addr && (addr % 0x10000 == 0)) {
				HexRec(hr, HEX_REC_ELA,  2, addr, wbuf);
				HexRecSaveToFile(hr, mFile);
				
				HexRec(hr, HEX_REC_DAT, 16 < (wbuf_data_length-addr) ? 16 : (wbuf_data_length-addr), addr, wbuf);
				HexRecSaveToFile(hr, mFile);
			}
			else {
				HexRec(hr, HEX_REC_DAT, 16 < (wbuf_data_length-addr) ? 16 : (wbuf_data_length-addr), addr, wbuf);
				HexRecSaveToFile(hr, mFile);
			}
			
			// progress
			int percent = 100*addr / wbuf_data_length;
			m_ctrlProgress.SetPos(percent);
			s.Format(_T("%3d%%"), percent);
			EditCtrlOutput(s, -4);
		}
	}
	else if (FileExt.CompareNoCase(_T("BIN")) == 0) {
		mFile.Open(PathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, &mExcept);
		m_ctrlSaveAppend.GetCheck() ? mFile.SeekToEnd() : mFile.SeekToBegin();
		mFile.Write(wbuf, wbuf_data_length);
	}
	else {
		MessageBox(_T("Only Hex/Bin file suported."));
		return;
	}

	m_ctrlProgress.SetPos(100);
	EditCtrlOutput(s = _T("100%\r\n"), -4);
	mFile.Close();
}

void COTPWriterProDlg::HexRec(HEXRECORD_t & hr, HEXRECTYPE_t rectype, unsigned char relen, unsigned int addr, unsigned char *buf)
{
	switch (rectype) {
	case HEX_REC_DAT:
		hr.reclen = relen;
		hr.addrh = (unsigned char)(addr>>8);
		hr.addrl = (unsigned char)addr;
		hr.rectype = (unsigned char)HEX_REC_DAT;
		for (unsigned int i = 0; i < hr.reclen; i++)
			hr.data[i] = wbuf[addr++];
		hr.checksum();
		break;
	case HEX_REC_EOF: // :00000001FF
		hr.reclen = 0x00;
		hr.addrh = 0x00;
		hr.addrl = 0x00;
		hr.rectype = 0x01;
		hr.data[0] = 0xFF;
		break;
	case HEX_REC_ESA:
		hr.reclen = 0x02;
		hr.addrh = 0x00;
		hr.addrl = 0x00;
		hr.rectype = (unsigned char)HEX_REC_ESA;
		hr.data[0] = (unsigned char)(addr>>12);
		hr.data[1] = (unsigned char)(addr>>4);
		hr.checksum();
		break;
	case HEX_REC_SSA:
		break;
	case HEX_REC_ELA:
		hr.reclen = 0x02;
		hr.addrh = 0x00;
		hr.addrl = 0x00;
		hr.rectype = (unsigned char)HEX_REC_ELA;
		hr.data[0] = (unsigned char)(addr>>24);
		hr.data[1] = (unsigned char)(addr>>16);
		hr.checksum();
		break;
	case HEX_REC_SLA:
		break;
	default:
		break;
	}
}

void COTPWriterProDlg::HexRecSaveToFile(HEXRECORD_t &hr, CStdioFile &mFile )
{
	CString s;
	s.Format(_T(":%02X%02X%02X%02X"), hr.reclen, hr.addrh, hr.addrl, hr.rectype);
	for (unsigned int i = 0; i < hr.reclen; i++)
		s.AppendFormat(_T("%02X"), hr.data[i]);
	s.AppendFormat(_T("%02X\n"), hr.data[hr.reclen]);
	mFile.WriteString(s);

#if 0
	_tprintf(_T("%s"), s);
#endif
}

void COTPWriterProDlg::HexRecPrint( HEXRECORD_t &hr )
{
	_tprintf(_T("\nhex record:  %02X | %02X%02X | %02X | "), hr.reclen, hr.addrh, hr.addrl, hr.rectype);
	for (int i = 0; i < hr.reclen; i++)	_tprintf(_T("%02X"), hr.data[i]);
	_tprintf(_T(" | %02X"), hr.data[hr.reclen]);
}

void COTPWriterProDlg::HexRecReadFromFile( unsigned char *buf, CStdioFile &mFile, HEXRECORD_t &hr, unsigned int &cur_addr )
{
	CString s;
	mFile.ReadString(s);
	for (int i = 1; i < s.GetLength(); i+=2) {
		((unsigned char *)&hr)[1+i/2] = stoul((tstring)(s.Mid(i, 2).GetString()), 0, 16);
	}
	//HexRecPrint(hr);

	unsigned int addr;
	unsigned int base_addr = cur_addr & 0xffff0000;

	switch ((HEXRECTYPE_t)hr.rectype) {
	case HEX_REC_DAT: // Data Record
		addr = base_addr + (hr.addrh<<8) + hr.addrl;
		//_tprintf(_T(" | %08X"), addr);
		memset(buf+cur_addr, 0, addr-cur_addr);
		memcpy(buf+addr, hr.data, hr.reclen);
		cur_addr = addr + hr.reclen;
		break;
	case HEX_REC_EOF: // End of File Record
		//wbuf_data_length = cur_addr;
		break;
	case HEX_REC_ESA: // Extended Segment Address Record
		base_addr = (hr.data[0]<<12) + (hr.data[1]<<4);
		memset(buf+cur_addr, 0, base_addr-cur_addr);
		cur_addr = base_addr;
		break;
	case HEX_REC_SSA: // Start Segment Address Record
		MessageBox(_T("Hex item type: 0x03 (Start Segment Address Record), unprocessed."));
		break;
	case HEX_REC_ELA: // Extended Linear Address Record
		base_addr = (hr.data[0]<<24) + (hr.data[1]<<16);
		memset(buf+cur_addr, 0, base_addr-cur_addr);
		cur_addr = base_addr;
		break;
	case HEX_REC_SLA: // Start Linear Address Record
		MessageBox(_T("Hex item type: 0x05 (Start Linear Address Record), unprocessed."));
		break;
	default:
		break;
	}

	
}

void COTPWriterProDlg::EditCtrlOutput( CString &s, int pos )
{
	// pos 为相对于最后位置的字符数
	int len = m_ctrlEdit.GetWindowTextLength(); 
	m_ctrlEdit.SetSel(len + pos,len); //将插入光标放在最后 
	m_ctrlEdit.ReplaceSel(s); 
	//m_ctrlEdit.ScrollWindow(0,0); //滚动到插入点 (滚动条始终在底部，不闪动)
}



void COTPWriterProDlg::OnBnClickedButtonPrint()
{
	// TODO: 在此添加控件通知处理程序代码
}


void COTPWriterProDlg::OnBnClickedButtonErase()
{
	// TODO: 在此添加控件通知处理程序代码
}


void COTPWriterProDlg::OnBnClickedButtonVerify()
{
	// TODO: 在此添加控件通知处理程序代码
}


void COTPWriterProDlg::OnBnClickedButtonEncrypt()
{
	// TODO: 在此添加控件通知处理程序代码
}
