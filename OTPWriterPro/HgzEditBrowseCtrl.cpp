// HgzMFCEditBrowseCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "HgzEditBrowseCtrl.h"


// CHgzEditBrowseCtrl

//IMPLEMENT_DYNAMIC(CHgzEditBrowseCtrl, CMFCEditBrowseCtrl)

CHgzEditBrowseCtrl::CHgzEditBrowseCtrl()
    : m_strFilter(_T("All Files (*.*)|*.*||"))
{

}

CHgzEditBrowseCtrl::~CHgzEditBrowseCtrl()
{
}


BEGIN_MESSAGE_MAP(CHgzEditBrowseCtrl, CMFCEditBrowseCtrl)
END_MESSAGE_MAP()


void CHgzEditBrowseCtrl::OnBrowse()
{
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, m_strFilter);
    
    if (dlg.DoModal() == IDCANCEL)
       return;

    SetWindowText(dlg.GetPathName());
    GetParent()->PostMessage(WM_HgzEditBrowseCtrl_AFTERUPDATE, 0, (LPARAM)GetSafeHwnd());

}


// CHgzEditBrowseCtrl 消息处理程序


