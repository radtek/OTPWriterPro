#pragma once


// CDlgTools �Ի���

class CDlgTools : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTools)

public:
	CDlgTools(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTools();

// �Ի�������
	enum { IDD = IDD_DIALOG_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
};
