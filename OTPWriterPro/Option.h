#pragma once


// COption 对话框

class COption : public CDialogEx
{
	DECLARE_DYNAMIC(COption)

public:
	COption(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COption();

// 对话框数据
	enum { IDD = IDD_DIALOG_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    BOOL m_bEnableIncontinuousCell;
    BOOL m_bEnableConsoleOutput;
    int m_nPacketDataLength;
};
