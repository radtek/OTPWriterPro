// Option.cpp : 实现文件
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "Option.h"
#include "afxdialogex.h"


// COption 对话框

IMPLEMENT_DYNAMIC(COption, CDialogEx)

COption::COption(CWnd* pParent /*=NULL*/)
	: CDialogEx(COption::IDD, pParent)
    , m_bWriteBufSizeReallyUsed(FALSE)
    , m_bEnableConsoleOutput(FALSE)
    , m_nPacketDataLength(52)
{

}

COption::~COption()
{
}

void COption::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK_ENABLE_INCONTINUOUS_ADDRESS, m_bWriteBufSizeReallyUsed);
    DDX_Check(pDX, IDC_CHECK2, m_bEnableConsoleOutput);
    DDX_Text(pDX, IDC_COMBO5, m_nPacketDataLength);
	DDV_MinMaxInt(pDX, m_nPacketDataLength, 1, 52);
}


BEGIN_MESSAGE_MAP(COption, CDialogEx)
END_MESSAGE_MAP()


// COption 消息处理程序


