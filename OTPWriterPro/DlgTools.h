#pragma once


// CDlgTools 对话框

class CDlgTools : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTools)

public:
	CDlgTools(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTools();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1_MD5();
	afx_msg void OnBnClickedButton2_RF_Addr_Gen();
	afx_msg void OnBnClickedButton3_CRC8();
};
