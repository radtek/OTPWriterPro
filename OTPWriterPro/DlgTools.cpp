// DlgTools.cpp : 实现文件
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "DlgTools.h"
#include "afxdialogex.h"
#include "DlgMD5.h"


// CDlgTools 对话框

IMPLEMENT_DYNAMIC(CDlgTools, CDialogEx)

CDlgTools::CDlgTools(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTools::IDD, pParent)
{

}

CDlgTools::~CDlgTools()
{
}

void CDlgTools::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTools, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgTools::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgTools 消息处理程序


void CDlgTools::OnBnClickedButton1()
{
    CDlgMD5 dlg;
    dlg.DoModal();
}
