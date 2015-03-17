
// OTPWriterProDlg.cpp : ʵ���ļ�
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



#ifdef _DEBUG
#define new DEBUG_NEW
#endif




////////////////////////////////////////////////////////////////////////////////
	
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// COTPWriterProDlg �Ի���




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
    DDX_Control(pDX, IDC_CHECK5, m_chkClearBufferAll);
    DDX_Control(pDX, IDC_CHECK_LENGTH_HEX, m_chkDataLen);
    //DDX_Control(pDX, IDC_BUTTON1, m_bnTEST1);
    //DDX_Control(pDX, IDC_BUTTON2, m_bnTEST2);
    DDX_Control(pDX, IDC_BUTTON22, m_bnFind);
    DDX_Control(pDX, IDC_COMBO8, m_ctrlIgnoreMemBegin);
    DDX_Control(pDX, IDC_COMBO9, m_ctrlIgnoreMemEnd);
    DDX_Control(pDX, IDC_CHECK3, m_ctrlIgnoreMem);
    DDX_Control(pDX, IDC_BUTTON6, m_ctrlWrite);
    DDX_Control(pDX, IDC_BUTTON10, m_ctrlEncrypt);
}

BEGIN_MESSAGE_MAP(COTPWriterProDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BUTTON1, &COTPWriterProDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &COTPWriterProDlg::OnBnClickedButton2)
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
	ON_BN_CLICKED(IDC_BUTTON18, &COTPWriterProDlg::OnBnClickedButtonFillInBuffer)
    ON_BN_CLICKED(IDC_BUTTON19, &COTPWriterProDlg::OnBnClickedButtonOption)
    ON_BN_CLICKED(IDC_BUTTON20, &COTPWriterProDlg::OnBnClickedButtonClearBuffer)
    ON_CBN_SELCHANGE(IDC_COMBO1, &COTPWriterProDlg::OnCbnSelchangeComboSelectChipType)
    ON_BN_CLICKED(IDC_BUTTON3, &COTPWriterProDlg::OnBnClickedButtonDetectChipType)
    ON_BN_CLICKED(IDC_BUTTON21, &COTPWriterProDlg::OnBnClickedButtonVersionNum)
    ON_BN_CLICKED(IDC_CHECK_LENGTH_HEX, &COTPWriterProDlg::OnBnClickedCheckLengthHex)
END_MESSAGE_MAP()


// COTPWriterProDlg ��Ϣ�������

BOOL COTPWriterProDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ctrlChipSel.SetCurSel(0);
	m_ctrlMemAddrBegin.SetWindowText(_T("0000"));
	m_ctrlDataLength.SetWindowText(_T("16384"));
    m_ctrlIgnoreMem.SetCheck(TRUE);
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
	m_ctrlListBuffer.SetItemCount(16384/16);
	for (unsigned int i = 0; i < 16384/16; i++) {
		s.Format(_T("%04X %04X"), (unsigned __int16)(i>>12), (unsigned __int16)(i<<4));
		m_ctrlListBuffer.InsertItem(i, s);
	}


    // Option
    m_Option.m_bEnableConsoleOutput = FALSE;
    m_Option.m_bWriteBufSizeReallyUsed = FALSE;
    m_Option.m_nPacketDataLength = 52;
    if (m_Option.m_bEnableConsoleOutput)	
        hgzOpenConsole();

    m_ChipType = HS__CMD__CHIP_TYPE__OTP__HS6206;
    
	// Start a periodic read IRP to timely receive datas from device.
	//SetTimer(1, 1, NULL);
    m_bnFind.EnableWindow(FALSE);
    //m_bnTEST1.EnableWindow(FALSE);
    //m_bnTEST2.EnableWindow(FALSE);
    //m_ctrlWrite.EnableWindow(FALSE);
    //m_ctrlErase.EnableWindow(FALSE);
    //m_ctrlEncrypt.EnableWindow(FALSE);



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COTPWriterProDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COTPWriterProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COTPWriterProDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
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
//	// 1. ���ұ�ϵͳ��HID���GUID��ʶ
//	GUID guidHID;
//	HidD_GetHidGuid( &guidHID );
//	CString s;
//	s.Format( _T("HID ��� GUID ��ʶ: {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}\r\n")
//		, guidHID.Data1, guidHID.Data2, guidHID.Data3
//		, guidHID.Data4[0], guidHID.Data4[1], guidHID.Data4[2], guidHID.Data4[3]
//		, guidHID.Data4[4], guidHID.Data4[5], guidHID.Data4[6], guidHID.Data4[7] );
//
//	// 2. ��ȡ�豸��Ϣ���õ��豸��Ϣ���ľ��
//	s += _T("���ڲ��ҿ��õ� HID �豸...\r\n");
//	HDEVINFO hDevInfo = SetupDiGetClassDevs( &guidHID, NULL/*_T("USB")*/, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE );
//	if( INVALID_HANDLE_VALUE == hDevInfo ) {
//		s += _T("�޷��ҵ��豸��Ϣ���ľ��...\r\n");
//		_tprintf_s(_T("%s"), s);
//		//std::tcout << s;
//		//m_ctrlHid2.SetWindowText(s);
//		ShowError();
//		return;
//	}
//
//	// 3. �����豸��Ϣ���������豸�ӿڣ��õ�����ϸ��Ϣ
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
//			s += _T("�޷��򿪶˿ڡ�");
//			_tprintf_s(_T("%s"), s);
//			std::tcout << s;
//			//m_ctrlHid2.SetWindowText(s);
//			SetupDiDestroyDeviceInfoList( hDevInfo );
//			free( pDetail );
//			return;
//		}
//
//		// ��ѯ�豸��ʶ
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

    CString strExt = _T("*.hex, *.bin|*.hex;*.bin|")
                     _T("Hex Files (*.hex)|*.hex|Bin Files (*.bin)|*.bin|All Files (*.*)|*.*||");
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
            HexRecReadFromFile(g_mem.GetBuf(), g_mem.GetBufFlag(), mFile, hr, addr);
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


    m_ctrlProgress.SetPos(100);
    EditCtrlOutput(-4, _T("100%\r\n"));
    mFile.Close();

    //UpdateBufferShow();
    UpdateBufferDisplay(0, max(nOldSize, g_mem.SizeUsed()));
    m_ctrlMemAddrBegin.SetWindowText(_T("0000"));
    m_ctrlDataLength.SetWindowsTextFormat(_T("%d"), g_mem.SizeUsed());

}

void COTPWriterProDlg::OnBnClickedButtonSaveAs()
{
	m_ctrlProgress.SetPos(0);
	m_ctrlListBuffer.EndEdit(TRUE);
	
	if (g_mem.IsEmpty()) {
		MessageBox(_T("No data to save."));
		return;
	}
	
    CString strExt = _T("*.hex, *.bin|*.hex;*.bin|")
        _T("Hex Files (*.hex)|*.hex|Bin Files (*.bin)|*.bin|All Files (*.*)|*.*||");
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
	else {
		MessageBox(_T("Only Hex/Bin file suported."));
		return;
	}

	m_ctrlProgress.SetPos(100);
	EditCtrlOutput(-4, _T("100%\r\n"));
	mFile.Close();
}


void COTPWriterProDlg::EditCtrlOutput( int pos, const TCHAR *szFormat, ... )
{
    // pos Ϊ��������λ�õ��ַ���

    va_list arglist;
    va_start(arglist, szFormat);
    TCHAR *buf = new TCHAR[_vsctprintf(szFormat, arglist)+1];
    //TCHAR *buf = (TCHAR *)_alloca((_vsctprintf(szFormat, arglist)+1)*sizeof(TCHAR)); // ��̬����ջ�ڴ棬�����ֶ��ͷš�
    _vstprintf(buf, szFormat, arglist);

    int len = m_ctrlEdit.GetWindowTextLength(); 
    m_ctrlEdit.SetSel(len + pos, len); //�������������� 
    m_ctrlEdit.ReplaceSel(buf); 
    //m_ctrlEdit.ScrollWindow(0,0); //����������� (������ʼ���ڵײ���������)
    delete [] buf;
}


void COTPWriterProDlg::OnBnClickedButtonWrite()
{
    PrintCurrentTime();
    EditCtrlOutput(0, _T("ʵ��д���ֽ�����%d\r\n"), m_Option.m_bWriteBufSizeReallyUsed ? g_mem.Write(0, g_mem.SizeUsed()) : g_mem.Write(GetStartAddress(), GetDataLength()));
}


void COTPWriterProDlg::OnBnClickedButtonRead()
{
    PrintCurrentTime();
    
    INT addr = GetStartAddress();
    INT len = GetDataLength();
    EditCtrlOutput(0, _T("ʵ�ʶ�ȡ�ֽ�����%d\r\n"), g_mem.Read(addr, len));
    UpdateBufferDisplay(addr, len);
}


void COTPWriterProDlg::OnBnClickedButtonVerify()
{
    PrintCurrentTime();
    if (m_ctrlIgnoreMem.GetCheck() ? g_mem.VerifyEx(GetStartAddress(), GetDataLength(), GetStartIgnoreAddress(), GetEndIgnoreAddress()) : g_mem.Verify(GetStartAddress(), GetDataLength()))
        EditCtrlOutput(0, _T("У��ɹ���\r\n"));
    else
        EditCtrlOutput(0, _T("У��ʧ�ܣ�\r\n"));
}

void COTPWriterProDlg::OnBnClickedButtonEncrypt()
{
    Cmd1Data0(&CHgzMem::Encryt, _T("����"));
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
            AfxMessageBox(_T("��������Ϊ�գ�"));
            EditCtrlOutput(0, _T("��������Ϊ�գ�\r\n"));
            return;
        }
        CStringArray arr;
        hgzExtractSubStrings1(arr, s, _T("-"));
        if (arr.GetSize() > 2)
        {
            AfxMessageBox(_T("����̫�࣡"));
            EditCtrlOutput(0, _T("����̫�࣡\r\n"));
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
        EditCtrlOutput(0, _T("�ɹ������飺%d - %d\r\n"), startSectorNum, endSectorNum);
    }
    else
    {
        EditCtrlOutput(0, _T("ִ��ʧ�ܣ�\r\n"));
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
    Cmd1Data0(&CHgzMem::IsBlank, _T("���"));
}

void COTPWriterProDlg::OnBnClickedButtonEnterProgramMode()
{
    Cmd1Data0(&CHgzMem::EnterOTPSPIMode, _T("���� OTP SPI ����ģʽ"));
}


void COTPWriterProDlg::OnBnClickedButtonExitProgramMode()
{
    Cmd1Data0(&CHgzMem::ExitOTPSPIMode, _T("�˳� OTP SPI ����ģʽ"));
}


void COTPWriterProDlg::OnBnClickedButtonTestBlankCheck()
{
    Cmd1Data0(&CHgzMem::OTPTest_BlankCheckTest, _T("��ղ���"));
}


void COTPWriterProDlg::OnBnClickedButtonTestDec()
{
    Cmd1Data0(&CHgzMem::OTPTest_WordLineAndBitLineIntegrityTest, _T("��λ�������Բ���"));
}


void COTPWriterProDlg::OnBnClickedButtonTestWR()
{
    Cmd1Data0(&CHgzMem::OTPTest_PreprogramTest, _T("Ԥ��̲���"));
}


void COTPWriterProDlg::OnBnClickedButtonOtpSpiReset()
{
    Cmd1Data0(&CHgzMem::OTPSPIReset, _T("OTP SPI ��λ"));
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
    if (m_Option.DoModal() == IDCANCEL)
    {
        return;
    }

    if (m_Option.m_bEnableConsoleOutput)
        hgzOpenConsole();
    else
        hgzCloseConsole();
}



void COTPWriterProDlg::OnBnClickedButtonClearBuffer()
{
    UINT32 addr;
    UINT64 dataLen;

    if (m_chkClearBufferAll.GetCheck())
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
    else
        m_ChipType = HS__CMD__CHIP_TYPE__NONE;

    HS_CHIP_TYPE_t chipType = m_ChipType;
    CString str(_T("�趨оƬ�ͺ�: "));
    str += s;
    if (g_mem.SetChipType(&chipType) && chipType == m_ChipType)
        str += _T(" -- �ɹ���\r\n");
    else
        str += _T(" -- ʧ�ܣ�\r\n");

    EditCtrlOutput(0, str.GetString());
}


void COTPWriterProDlg::OnBnClickedButtonDetectChipType()
{
    PrintCurrentTime();

    HS_CHIP_TYPE_t chipType = m_ChipType;
    BOOL bRes = g_mem.DetectChipType(&chipType);
    if ( bRes && chipType!=HS__CMD__CHIP_TYPE__NONE )
    {
        EditCtrlOutput(0, _T("���оƬ�ͺųɹ���\r\n"));
    }
    else
    {
        EditCtrlOutput(0, _T("���оƬ�ͺ�ʧ�ܣ�\r\n"));
        return;
    }
    
    switch (chipType)
    {
    case HS__CMD__CHIP_TYPE__OTP__HS6206:
        m_ctrlChipSel.SelectString(0, _T("HS6206"));
        break;
    case HS__CMD__CHIP_TYPE__FLASH__EN25T80:
        m_ctrlChipSel.SelectString(0, _T("EN25T80"));
        break;
    
    case HS__CMD__CHIP_TYPE__NONE:
    default:
        m_ctrlChipSel.SetCurSel(-1);
        break;
    }
}


void COTPWriterProDlg::OnBnClickedButtonVersionNum()
{
    // ��λ���汾��Ϣ
    PrintCurrentTime();
    EditCtrlOutput(0, _T("��λ������汾��Ϣ��%s\r\n"), GetProductVersion(NULL));
    
    // ��λ���汾��Ϣ
    PrintCurrentTime();
    char fmVerInfo[64*2];
    BOOL bRes = g_mem.GetFirmwareVersionInfo((UINT8 *)fmVerInfo);

    if ( bRes )
    {
        CString s(fmVerInfo);
        _tprintf(_T("%s\n"), s);
        EditCtrlOutput(0, _T("��λ���̼��汾��Ϣ��%s\r\n"), s);
    }
    else
    {
        EditCtrlOutput(0, _T("��λ���̼��汾��Ϣ����ȡʧ�ܣ�\r\n"));
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
    if ((g_mem.*CmdFunc)(res) && res)
            str += _T("�ɹ���\r\n");
    else
        str += _T("ʧ�ܣ�\r\n");

    EditCtrlOutput(0, str.GetString());
}
