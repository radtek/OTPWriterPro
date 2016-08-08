// DlgTools.cpp : 实现文件
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "DlgTools.h"
#include "afxdialogex.h"
#include "DlgMD5.h"
#include "ble_addr_gen.h"
#include "DlgCRC.h"


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
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgTools::OnBnClickedButton1_MD5)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgTools::OnBnClickedButton2_RF_Addr_Gen)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgTools::OnBnClickedButton3_CRC8)
END_MESSAGE_MAP()


// CDlgTools 消息处理程序


void CDlgTools::OnBnClickedButton1_MD5()
{
    CDlgMD5 dlg;
    dlg.DoModal();
}


void CDlgTools::OnBnClickedButton2_RF_Addr_Gen()
{
	uint32_t addr;
	for (int i = 1; i <= 1000; i++) {
		addr = gen_ble_addr(i);

		printf("%lx\n", addr);
	}
}


void CDlgTools::OnBnClickedButton3_CRC8()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgCRC dlg;
	dlg.DoModal();
}


