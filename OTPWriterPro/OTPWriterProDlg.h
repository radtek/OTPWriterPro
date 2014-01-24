
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


#define TEST_ON 1

extern "C" {
	// Declare the C libraries used
	#include "setupapi.h"  // Must link in setupapi.lib
	#include "hidsdi.h"   // Must link in hid.lib
}

// COTPWriterProDlg 对话框
#include "afxwin.h"
#include "afxcmn.h"
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

	hid_device * Hid_OpenTopLevelCollection(unsigned short vendor_id, unsigned short product_id);



	typedef struct {
		unsigned char report_id;
		CPacket packet;
	} HIDREPORT_t;


	enum _FLAG_t {
		process_state_idle = 0,
		process_state_read = HS_MEM_READ,
		process_state_write = HS_MEM_WRITE,
		process_state_verify = HS_MEM_READ,
		process_state_encrypt = HS_MEM_WRDIS,
		process_state_erase = HS_MEM_ERASE_PAGE
	} process_state;
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	unsigned char m_buf[0x10000];
	long long m_buf_num;
	long long m_buf_data_length;
	unsigned char rbuf[0x10000];


	afx_msg void OnBnClickedButtonWrite();

	unsigned int GetPacketDataLength();

	int SendPacket( hid_device * handle, HIDREPORT_t &r );

	int ReceivePacket( hid_device * handle, HIDREPORT_t &r );

	int BuildMemWritePacket( HIDREPORT_t &r, unsigned char *buf, unsigned int addr, unsigned int length, unsigned int packet_data_length, unsigned int report_id = 0 );
	BOOL BuildMemReadPacket( HIDREPORT_t &r, unsigned int addr, unsigned int length, unsigned int report_id = 0 );

	afx_msg void OnBnClickedButtonOpenFile();

	void PrintCurrentTime();

	CButton m_ctrlDebugMode;
	afx_msg void OnBnClickedCheckConsoleEnable();
	CProgressCtrl m_ctrlProgress;
	CHgzComboBox m_ctrlPacketDataLength;
	afx_msg void OnBnClickedButtonSaveAs();

	afx_msg void OnBnClickedButtonErase();
	afx_msg void OnBnClickedButtonVerify();
	afx_msg void OnBnClickedButtonEncrypt();

	afx_msg void ExecuteMemCmd( unsigned char MemCmd );

	CHgzListCtrl m_ctrlListBuffer;

	int MemRead( hid_device * handle, HIDREPORT_t &r, unsigned int start_addr, unsigned int length );

protected:
	afx_msg LRESULT OnLvmItemChanged(WPARAM wParam, LPARAM lParam);
	unsigned int GetDataLength();
	int CompareMemData( HIDREPORT_t & r, unsigned char *wbuf );
	BOOL BuildMemCmdPacket( HIDREPORT_t &r, unsigned char MemCmd, unsigned int report_id=0 );
	void UpdateBufferDisplay( unsigned int addr, unsigned int length );
public:
	afx_msg void OnBnClickedButtonBlankCheck();
	afx_msg void OnBnClickedButtonEnterProgramMode();
	afx_msg void OnBnClickedButtonExitProgramMode();
	afx_msg void OnBnClickedButtonTestBlankCheck();
	afx_msg void OnBnClickedButtonTestDec();
	afx_msg void OnBnClickedButtonTestWR();
	afx_msg void OnBnClickedButtonChipReset();
};


