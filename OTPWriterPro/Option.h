#pragma once


// COption �Ի���

class COption : public CDialogEx
{
	DECLARE_DYNAMIC(COption)

public:
	COption(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COption();

// �Ի�������
	enum { IDD = IDD_DIALOG_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    BOOL m_bEnableIncontinuousCell;
    BOOL m_bEnableConsoleOutput;
    int m_nPacketDataLength;
};
