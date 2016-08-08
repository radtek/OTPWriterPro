#pragma once
#include "afxeditbrowsectrl.h"
#include "afxwin.h"
#include "hgz\hgz.h"
#include "HgzPath.h"

// CDlgCRC 对话框

class CDlgCRC : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCRC)

public:
	CDlgCRC(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCRC();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CRC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeMfceditbrowse1();
	CMFCEditBrowseCtrl m_ctrlFileToBeCRCed;
	CEdit m_ctrlCRC;

	u8 crc8_of_one_line_of_4bytes(CString s);
	bool generate_crc8_for_each_line_of_a_file_and_store_in_another_file(CString filePath);
	bool generate_crc8_for_each_line_of_each_file_and_store_in_another_file(CString filePath);
};
