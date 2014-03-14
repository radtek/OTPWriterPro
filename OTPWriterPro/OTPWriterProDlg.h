
// OTPWriterProDlg.h : 头文件
//

#pragma once

#include ".\hgz\hgz.h"
#include "HSOTPWriterProtocol.h"
#include ".\hgz\HgzComboBox.h"
#include ".\hgz\HgzFormatTransformer.h"
#include ".\hgz\HgzListCtrl.h"
#include "Packet.h"
#include "hidapi.h"
#include ".\hgz\HexFile.h"


#define TEST_ON 0

extern "C" {
	// Declare the C libraries used
	#include "setupapi.h"  // Must link in setupapi.lib
	#include "hidsdi.h"   // Must link in hid.lib
}

// COTPWriterProDlg 对话框
#include "afxwin.h"
#include "afxcmn.h"
#include "d:\users\hgz\documents\appdata\visual studio 2010\projects\otpwriterpro\otpwriterpro\hgz\hgzcombobox.h"
class COTPWriterProDlg : public CDialogEx
{
// 构造
public:
	COTPWriterProDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OTPWRITERPRO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();

public:
#define ShowError() ShowError_(_T(__FILE__),__LINE__)

	void EditCtrlOutput(CString &s, int pos = 0);

	void ShowError_(LPCTSTR file, long line)
	{
		LPVOID lpMsgBuf;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS
			, NULL, GetLastError(), 0, (LPTSTR)&lpMsgBuf, 0, NULL );
		_tprintf( _T("---ERROR %s(%ld): %s/n"), file, line, (LPCTSTR)lpMsgBuf );
		LocalFree( lpMsgBuf );
	}
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CButton m_ctrlErase;
	CEdit m_ctrlErasePageNum;
	CButton m_ctrlEraseAll;
	CEdit m_ctrlEdit;
	CHgzComboBox m_ctrlComboEdit;
	CHgzComboBox m_ctrlChipSel;
	CButton m_ctrlCodeSaveAs;
	afx_msg void OnBnClickedButtonRead();

	unsigned int GetStartAddress();

	
	CHgzComboBox m_ctrlMemAddrBegin;
	CHgzComboBox m_ctrlDataLength;
	CButton m_ctrlSaveAppend;

	hid_device * Hid_OpenTopLevelCollection(unsigned short vendorID, unsigned short productID);


    typedef struct {
        BOOL bEnableConsoleOutput;
        BOOL bEnableIncontinuousCell;
        INT nPacketDataLength;
    } OPTION_t;
    OPTION_t m_option;



	typedef struct {
		unsigned char reportID;
		CPacket packet;
	} HIDREPORT_t;


	enum _FLAG_t {
		process_state_idle = 0,
		process_state_read = HS__MEM__READ,
		process_state_write = HS__MEM__WRITE,
		process_state_verify = HS__MEM__READ,
		process_state_encrypt = HS__MEM__WRDIS,
		process_state_erase = HS__MEM__ERASE_PAGE
	} process_state;
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	unsigned char m_buf[0x10000];
    unsigned char m_bufFlag[0x10000];
	long long m_bufDataLength;

	afx_msg void OnBnClickedButtonWrite();

    int SendAndWaitToReceiveReport( HIDREPORT_t &r, hid_device * handle );

	int SendReport( hid_device * handle, HIDREPORT_t &r );

	int ReceiveReport( hid_device * handle, HIDREPORT_t &r );

	int BuildMemWriteReport( HIDREPORT_t &r, unsigned char *buf, unsigned int addr, unsigned int length, unsigned int packetDataLength, unsigned int reportID = 0 );
	BOOL BuildMemReadReport( HIDREPORT_t &r, unsigned int addr, unsigned int length, unsigned int reportID = 0 );

	afx_msg void OnBnClickedButtonOpenFile();

    void SetBuffer(unsigned int addr, long long lenth, unsigned char val, BOOL valid);

	void PrintCurrentTime();

	CProgressCtrl m_ctrlProgress;
	CHgzComboBox m_ctrlPacketDataLength;
	afx_msg void OnBnClickedButtonSaveAs();

	afx_msg void OnBnClickedButtonErase();
	afx_msg void OnBnClickedButtonVerify();
	afx_msg void OnBnClickedButtonEncrypt();

	afx_msg void ExecuteMemCmd( unsigned char memCmd );

	CHgzListCtrl m_ctrlListBuffer;

	unsigned int MemRead( hid_device * handle, HIDREPORT_t &r, unsigned int startAddr, unsigned int length );

protected:
	afx_msg LRESULT OnLvmItemChanged(WPARAM wParam, LPARAM lParam);
	unsigned int GetDataLength();
	int CompareMemData( HIDREPORT_t & r, unsigned char *wbuf );
	BOOL BuildMemCmdReport( HIDREPORT_t &r, unsigned char memCmd, unsigned int reportID=0 );
	void UpdateBufferDisplay( unsigned int addr, unsigned int length );
public:
	afx_msg void OnBnClickedButtonBlankCheck();
	afx_msg void OnBnClickedButtonEnterProgramMode();
	afx_msg void OnBnClickedButtonExitProgramMode();
	afx_msg void OnBnClickedButtonTestBlankCheck();
	afx_msg void OnBnClickedButtonTestDec();
	afx_msg void OnBnClickedButtonTestWR();
	afx_msg void OnBnClickedButtonChipReset();
	CHgzComboBox m_cbDataToFill;
	afx_msg void OnBnClickedButtonInBuffer();
    afx_msg void OnBnClickedButtonOption();
    void ClearBuffer(UINT32 addr, UINT64 length);
    afx_msg void OnBnClickedButtonClearBuffer();
    UINT8 GetDataToFillBuffer();
    CButton m_chkFillBufferAll;
    CButton m_chkClearBufferAll;
};


