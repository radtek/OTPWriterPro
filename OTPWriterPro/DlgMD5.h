#pragma once
#include "afxwin.h"
#include "afxshelltreectrl.h"
#include "afxeditbrowsectrl.h"


// CDlgMD5 对话框

class CDlgMD5 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMD5)

public:
	CDlgMD5(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMD5();

// 对话框数据
	enum { IDD = IDD_DIALOG_MD5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton3();
    CMFCEditBrowseCtrl m_ctrlFileToBeMD5ed;
    CEdit m_ctrlFileMD5;
    CMFCEditBrowseCtrl m_ctrlMD5File;
    CEdit m_ctrlStringToBeMD5ed;
    CEdit m_ctrlStringMD5;
    afx_msg void OnBnClickedButton5();
    afx_msg void OnEnChangeEdit6();
    CComboBox m_ctrlStringCoding;
    afx_msg void OnCbnSelchangeCombo1();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonVerifyMD5File();
    afx_msg void OnBnClickedButtonGenerateMD5File();
    afx_msg void OnBnClickedButtonOpenMD5File();
    CStatic m_ctrlMD5Progress;
    CMFCShellTreeCtrl m_ctrlShellTree;
    afx_msg void OnEnChangeMfceditbrowse1();
    virtual void OnOK();
};
