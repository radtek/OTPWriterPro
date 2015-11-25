// DlgMD5.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "DlgMD5.h"
#include "afxdialogex.h"
#include "HgzMD5.h"
#include "hgz\hgz.h"

// CDlgMD5 �Ի���

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
}


BEGIN_MESSAGE_MAP(CDlgMD5, CDialogEx)
    ON_EN_CHANGE(IDC_EDIT6, &CDlgMD5::OnEnChangeEdit6)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgMD5::OnCbnSelchangeCombo1)
    ON_BN_CLICKED(IDC_BUTTON6, &CDlgMD5::OnBnClickedButtonVerifyMD5File)
    ON_BN_CLICKED(IDC_BUTTON7, &CDlgMD5::OnBnClickedButtonGenerateMD5File)
    ON_BN_CLICKED(IDC_BUTTON8, &CDlgMD5::OnBnClickedButtonOpenMD5File)
    ON_MESSAGE(WM_HgzEditBrowseCtrl_AFTERUPDATE, &CDlgMD5::OnHgzeditbrowsectrlAfterupdate)
END_MESSAGE_MAP()


// CDlgMD5 ��Ϣ�������

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
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString s, sCoding;
    CHgzMD5 md5;

    m_ctrlStringToBeMD5ed.GetWindowText(s);
    m_ctrlStringCoding.GetWindowText(sCoding);
    m_ctrlStringMD5.SetWindowText(md5.md5str(s, NULL, (sCoding.CompareNoCase(_T("Unicode")) == 0)));
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

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_ctrlStringCoding.SelectString(0, _T("ANSI"));
    m_ctrlMD5Progress.SetWindowText(_T(""));
    m_ctrlFileToBeMD5ed.EnableFileBrowseButton();
    m_ctrlFileToBeMD5ed.SetFilter(_T("All Files|*.*|Hex or Bin Files|*.hex;*.bin|"));//(_T("Hex Files (*.hex), Bin Files (*.bin)|*.hex;*.bin|All Files (*.*)|*.*||"));
    m_ctrlMD5File.SetFilter(_T("MD5 Files (*.md5)|*.md5|All Files (*.*)|*.*||"));

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
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
        AfxMessageBox(_T("�ļ������ڣ�"));
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
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    CString s = md5.md5bin(mFile, &m_ctrlMD5Progress);

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


afx_msg LRESULT CDlgMD5::OnHgzeditbrowsectrlAfterupdate(WPARAM wParam, LPARAM lParam)
{
    HWND hwnd = (HWND)lParam;

    if (m_ctrlFileToBeMD5ed.GetSafeHwnd() == hwnd)
    {
        OnEnChangeMfceditbrowse1();
    }

    return 0;
}
