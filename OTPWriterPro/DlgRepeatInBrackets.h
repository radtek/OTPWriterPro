#pragma once
#include "hgz\hgz.h"
#include "HgzPath.h"
#include "afxeditbrowsectrl.h"
#include "afxwin.h"

// CDlgRepeatInBrackets �Ի���

class CDlgRepeatInBrackets : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRepeatInBrackets)

public:
	CDlgRepeatInBrackets(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRepeatInBrackets();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REPEAT_IN_BRACKETS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeMfceditbrowse1();
	bool generate_repeat_in_brackets_for_each_line_of_each_file_and_store_in_another_file(CString filePath);
	bool generate_repeat_in_brackets_for_each_line_of_a_file_and_store_in_another_file(CString filePath);
	CMFCEditBrowseCtrl m_ctrlFileToBeProcessed;
	CEdit m_ctrlResult;
};
