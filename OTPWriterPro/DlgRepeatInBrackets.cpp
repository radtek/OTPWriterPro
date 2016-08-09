// DlgRepeatInBrackets.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "DlgRepeatInBrackets.h"
#include "afxdialogex.h"
#include "hgz\\HgzString.h"


// CDlgRepeatInBrackets �Ի���

IMPLEMENT_DYNAMIC(CDlgRepeatInBrackets, CDialogEx)

CDlgRepeatInBrackets::CDlgRepeatInBrackets(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_REPEAT_IN_BRACKETS, pParent)
{

}

CDlgRepeatInBrackets::~CDlgRepeatInBrackets()
{
}

void CDlgRepeatInBrackets::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_ctrlFileToBeProcessed);
	DDX_Control(pDX, IDC_EDIT5, m_ctrlResult);
}


BEGIN_MESSAGE_MAP(CDlgRepeatInBrackets, CDialogEx)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CDlgRepeatInBrackets::OnEnChangeMfceditbrowse1)
END_MESSAGE_MAP()


// CDlgRepeatInBrackets ��Ϣ�������


void CDlgRepeatInBrackets::OnEnChangeMfceditbrowse1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString filePath;
	m_ctrlFileToBeProcessed.UpdateWindow();
	m_ctrlFileToBeProcessed.GetWindowText(filePath);

	if (!generate_repeat_in_brackets_for_each_line_of_each_file_and_store_in_another_file(filePath))
		return;
}


bool CDlgRepeatInBrackets::generate_repeat_in_brackets_for_each_line_of_each_file_and_store_in_another_file(CString filePath)
{
	CHgzPath fpath(filePath);
	fpath.ChangeFileName(_T("AddrMode1-150302-2013"));
	for (int i = 0; i <= 271; i++) {
		CHgzString s;
		s.Format(_T("-f%03d.txt"), i);
		CString fs(fpath.GetFullPath() + s);
		if (!generate_repeat_in_brackets_for_each_line_of_a_file_and_store_in_another_file(fs))
			break;
	}
	return true;
}

bool CDlgRepeatInBrackets::generate_repeat_in_brackets_for_each_line_of_a_file_and_store_in_another_file(CString filePath)
{
	CStdioFile mFile;
	if (!mFile.Open(filePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeText))
		return false;

	CHgzPath destPath(filePath);

	CString s = destPath.GetFileTitle();
	CString right = s.Right(5);
	CString left = s.Left(s.GetLength() - 5);
	left += _T("-repeat");
	left += right;

	destPath.ChangeFileTitle(left.GetString());

	CStdioFile mFileDes;
	mFileDes.Open(destPath.GetFullPath(), CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeText | CFile::modeCreate);

	CHgzString src;
	while (mFile.ReadString(src)) {
		CString des(src);
		des.AppendFormat(_T("(%s)\n"), src);
		mFileDes.WriteString(des);

	}

	mFile.Close();
	mFileDes.Close();

	return true;
}