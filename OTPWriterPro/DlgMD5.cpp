// DlgMD5.cpp : 实现文件
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "DlgMD5.h"
#include "afxdialogex.h"
#include "HgzMD5.h"
#include "hgz\hgz.h"

// CDlgMD5 对话框

IMPLEMENT_DYNAMIC(CDlgMD5, CDialogEx)

CDlgMD5::CDlgMD5(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMD5::IDD, pParent)
{
    
}

CDlgMD5::~CDlgMD5()
{
}

void CDlgMD5::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_ctrlFileToBeMD5ed);
    DDX_Control(pDX, IDC_EDIT5, m_ctrlFileMD5);
    DDX_Control(pDX, IDC_MFCEDITBROWSE3, m_ctrlMD5File);
    DDX_Control(pDX, IDC_EDIT6, m_ctrlStringToBeMD5ed);
    DDX_Control(pDX, IDC_EDIT7, m_ctrlStringMD5);
    DDX_Control(pDX, IDC_COMBO1, m_ctrlStringCoding);
    DDX_Control(pDX, IDC_MD5_COMPUTE_PROGRESS, m_ctrlMD5Progress);
    DDX_Control(pDX, IDC_MFCSHELLTREE1, m_ctrlShellTree);
}


BEGIN_MESSAGE_MAP(CDlgMD5, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT6, &CDlgMD5::OnEnChangeEdit6)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgMD5::OnCbnSelchangeCombo1)
    ON_BN_CLICKED(IDC_BUTTON6, &CDlgMD5::OnBnClickedButtonVerifyMD5File)
    ON_BN_CLICKED(IDC_BUTTON7, &CDlgMD5::OnBnClickedButtonGenerateMD5File)
    ON_BN_CLICKED(IDC_BUTTON8, &CDlgMD5::OnBnClickedButtonOpenMD5File)
END_MESSAGE_MAP()


// CDlgMD5 消息处理程序


void CDlgMD5::OnBnClickedButton5()
{
    CString strExt = _T("*.md5|*.md5|All Files (*.*)|*.*||");
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strExt);

    if (dlg.DoModal() != IDOK)
    {
        return;
    }

    m_ctrlMD5File.SetWindowText(dlg.GetPathName());
}


void CDlgMD5::OnEnChangeEdit6()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    CString s, sDigest;
    CStringA sa;
    char saDigest[128/8*2+1];
    saDigest[128/8*2] = '\0';
    
    m_ctrlStringToBeMD5ed.GetWindowText(s);

    CString sCoding;
    m_ctrlStringCoding.GetWindowText(sCoding);
    CHgzMD5 md5;
    if (sCoding.CompareNoCase(_T("ANSI")) == 0)
    {
        sa = s; // TCHAR -> char

        md5.md5(sa.GetBuffer(), saDigest);
        sDigest = saDigest;
    }
    else if (sCoding.CompareNoCase(_T("Unicode")) == 0)
    {
        md5.md5(s, sDigest);
    }
    
    m_ctrlStringMD5.SetWindowText(sDigest);
}


void CDlgMD5::OnCbnSelchangeCombo1()
{
    CString s;
    m_ctrlStringCoding.GetWindowText(s);

    OnEnChangeEdit6();
}


BOOL CDlgMD5::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_ctrlStringCoding.SelectString(0, _T("ANSI"));
    m_ctrlMD5Progress.SetWindowText(_T(""));
    m_ctrlFileToBeMD5ed.EnableFileBrowseButton();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CDlgMD5::OnBnClickedButtonVerifyMD5File()
{
    CString sMD5, sMD5File, s;
    m_ctrlFileMD5.GetWindowText(sMD5);
    m_ctrlMD5File.GetWindowText(sMD5File);

    CStdioFile mFile;
    CFileException mExcept;
    if (!mFile.Open(sMD5File, CFile::modeRead | CFile::shareDenyNone | CFile::typeText))
    {
        AfxMessageBox(_T("文件不存在！"));
        return;
    }
    
    mFile.ReadString(s);
    int i = s.Find(_T(" = "));
    if (i != -1)
    {
        s = s.Right(s.GetLength()-i-3);
        if (s.CompareNoCase(sMD5) == 0)
            AfxMessageBox(_T("Correct!"));
        else
            AfxMessageBox(_T("Fail!"));
    }
    
    mFile.Close();

}


void CDlgMD5::OnBnClickedButtonGenerateMD5File()
{
    CString sFileToBeMD5ed, sMD5, sMD5File, s;
    m_ctrlFileToBeMD5ed.GetWindowText(sFileToBeMD5ed);
    m_ctrlFileMD5.GetWindowText(sMD5);
    m_ctrlMD5File.GetWindowText(sMD5File);

    CStdioFile mFile;
    CFileException mExcept;
    mFile.Open(sMD5File, CFile::modeCreate /*| CFile::modeNoTruncate*/ | CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeText);
    s.Format(_T("md5(%s) = %s"), sFileToBeMD5ed.Right(sFileToBeMD5ed.GetLength() - 1 - sFileToBeMD5ed.ReverseFind(_T('\\'))), sMD5);
    mFile.WriteString(s);
    mFile.Close();
}


void CDlgMD5::OnBnClickedButtonOpenMD5File()
{
    CString s;
    m_ctrlMD5File.GetWindowText(s);

    ShellExecuteA(this->m_hWnd, "open", "notepad.exe", CStringA(s), "", SW_SHOW);
}

void CDlgMD5::OnEnChangeMfceditbrowse1()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    //CString strExt = _T("All Files (*.*)|*.*||*.hex, *.bin|*.hex;*.bin|");
    //CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strExt);

    //if (dlg.DoModal() != IDOK)
    //{
    //    return;
    //}

    
    CString filePath;
    m_ctrlFileToBeMD5ed.UpdateWindow();
    m_ctrlFileToBeMD5ed.GetWindowText(filePath);


    m_ctrlFileMD5.SetWindowText(_T(""));
    m_ctrlFileMD5.UpdateWindow();
    m_ctrlMD5File.SetWindowText(filePath + _T(".md5"));
    m_ctrlMD5File.UpdateWindow();
    
    // compute MD5 for file
    CStdioFile mFile;
    CFileException mExcept;
    mFile.Open(filePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);

    CHgzMD5 md5;
    CString s = md5.md5(mFile, &m_ctrlMD5Progress);

    mFile.Close();

    m_ctrlFileMD5.SetWindowText(s);

}




void CDlgMD5::OnOK()
{
    if (GetDlgItem(IDOK) == GetFocus())
    {
        CDialogEx::OnOK();
    }

    if (GetDlgItem(IDC_MFCEDITBROWSE1) == GetFocus())
    {
        OnEnChangeMfceditbrowse1();
    }
}
