
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
#include "HidReport.h"
#include "HgzMem.h"
#include "OTPWriterPro.h"
#include "Option.h"


extern "C" {
	// Declare the C libraries used
	#include "setupapi.h"  // Must link in setupapi.lib
	#include "hidsdi.h"   // Must link in hid.lib
}

// COTPWriterProDlg 对话框
#include "afxwin.h"
#include "afxcmn.h"
#include "d:\users\hgz\documents\appdata\visual studio 2010\projects\otpwriterpro\otpwriterpro\hgz\hgzcombobox.h"



///////////////////////////////////////////////////



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
    //--------------------------------
    #define ShowError() ShowError_(_T(__FILE__),__LINE__)
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
    //--------------------------------
    
    CButton m_ctrlErase;
	CEdit m_ctrlErasePageNum;
	CButton m_ctrlEraseAll;
	CEdit m_ctrlEdit;
	CHgzComboBox m_ctrlComboEdit;
	CHgzComboBox m_ctrlChipSel;
    CHgzComboBox m_ctrlMemAddrBegin;
    CHgzComboBox m_ctrlDataLength;
    CProgressCtrl m_ctrlProgress;
    CHgzComboBox m_ctrlPacketDataLength;
    CHgzListCtrl m_ctrlListBuffer;
    CHgzComboBox m_cbDataToFill;
    CButton m_chkFillBufferAll;
    CButton m_chkClearBufferAll;
    CButton m_chkDataLen;

    HS_CHIP_TYPE_t m_ChipType;

    COption m_Option;

    afx_msg void OnBnClickedButtonOpenFile();
    afx_msg void OnBnClickedButtonSaveAs();
    afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonErase();
	afx_msg void OnBnClickedButtonVerify();
	afx_msg void OnBnClickedButtonEncrypt();
    afx_msg LRESULT OnLvmItemChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonBlankCheck();
	afx_msg void OnBnClickedButtonEnterProgramMode();
    afx_msg void OnBnClickedButtonExitProgramMode();
	afx_msg void OnBnClickedButtonTestBlankCheck();
	afx_msg void OnBnClickedButtonTestDec();
	afx_msg void OnBnClickedButtonTestWR();
	afx_msg void OnBnClickedButtonOtpSpiReset();
	afx_msg void OnBnClickedButtonFillInBuffer();
    afx_msg void OnBnClickedButtonOption();
    afx_msg void OnBnClickedButtonClearBuffer();
    afx_msg void OnCbnSelchangeComboSelectChipType();
    afx_msg void OnBnClickedButtonDetectChipType();
    afx_msg void OnBnClickedButtonVersionNum();
    afx_msg void OnBnClickedCheckLengthHex();

    void PrintCurrentTime();
    void EditCtrlOutput(int pos, const TCHAR *szFormat, ...);
    /*// Macro implimentation:
    #define EditCtrlOutput(pos, szFormat, ...) \
    do {\
        CString s; \
        s.Format(szFormat, ##__VA_ARGS__); \
        int len = m_ctrlEdit.GetWindowTextLength(); \
        m_ctrlEdit.SetSel(len + pos, len); \
        m_ctrlEdit.ReplaceSel(s); \
        len; \
    } while (0)*/
    unsigned int GetStartAddress();
    unsigned int GetDataLength();
    UINT8 GetDataToFillBuffer();
    void UpdateBufferDisplay( unsigned int addr, unsigned int length );
    void Cmd1Data0( CHgzMem::pMemFunc_Cmd1Data0 CmdFunc, CString s );


    CButton m_bnTEST1;
    CButton m_bnTEST2;
    CButton m_bnFind;
};


