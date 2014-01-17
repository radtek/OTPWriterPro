
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
	afx_msg void OnBnClickedButton7();
	CHgzComboBox m_ctrlMemAddrBegin;
	CHgzComboBox m_ctrlDataLength;
	CButton m_ctrlSaveAppend;

	hid_device * Hid_OpenTopLevelCollection(unsigned short vendor_id, unsigned short product_id);



	typedef struct {
		unsigned char report_id;
		CPacket packet;
	} HIDREPORT_t;


	typedef enum {
		HEX_REC_DAT = 0, // Data Record
		HEX_REC_EOF = 1, // End of File Record
		HEX_REC_ESA = 2, // Extended Segment Address Record
		HEX_REC_SSA = 3, // Start Segment Address Record
		HEX_REC_ELA = 4, // Extended Linear Address Record
		HEX_REC_SLA = 5  // Start Linear Address Record
	} HEXRECTYPE_t;

	typedef struct {
		unsigned char recmark;
		unsigned char reclen;
		unsigned char addrh;
		unsigned char addrl;
		unsigned char rectype;
		unsigned char data[256];

		unsigned char checksum() {
			data[reclen] = reclen + addrh + addrl + rectype;
			for (unsigned int i = 0; i < reclen; i++){
				data[reclen] += data[i];
			}
			data[reclen] = ~data[reclen] + 1;
			return data[reclen];
		};
	} HEXRECORD_t; 

	/*typedef struct {
		CString recmark;
		CString reclen;
		CString address;
		CString rectype;
		CString data;
	} HEXRECORDSTRING_t;*/

	enum _FLAG_t {
		process_state_idle = 0,
		process_state_read,
		process_state_write
	} process_state;
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	unsigned char wbuf[0x10000];
	long long wbuf_num;
	long long wbuf_data_length;
	unsigned char rbuf[0x10000];
	long long rbuf_num;
	long long rbuf_data_length;

	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonOpenFile();

	void HexRecReadFromFile( unsigned char *buf, CStdioFile &mFile, HEXRECORD_t &hr, unsigned int &cur_addr );

	void HexRecPrint( HEXRECORD_t &hr );

	CButton m_ctrlDebugMode;
	afx_msg void OnBnClickedCheck2();
	CProgressCtrl m_ctrlProgress;
	CHgzComboBox m_ctrlPacketDataLength;
	afx_msg void OnBnClickedButtonSaveAs();

	void HexRec(HEXRECORD_t & hr, HEXRECTYPE_t rectype, unsigned char relen, unsigned int addr, unsigned char *buf);
	void HexRecSaveToFile( HEXRECORD_t &hr, CStdioFile &mFile );

	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonErase();
	afx_msg void OnBnClickedButtonVerify();
	afx_msg void OnBnClickedButtonEncrypt();
	CHgzListCtrl m_ctrlListBuffer;
};


