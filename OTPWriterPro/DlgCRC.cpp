// DlgCRC.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "DlgCRC.h"
#include "afxdialogex.h"
#include "crc.h"
#include "hgz\\HgzString.h"


// CDlgCRC �Ի���

IMPLEMENT_DYNAMIC(CDlgCRC, CDialogEx)

CDlgCRC::CDlgCRC(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CRC, pParent)
{

}

CDlgCRC::~CDlgCRC()
{
}

void CDlgCRC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_ctrlFileToBeCRCed);
	DDX_Control(pDX, IDC_EDIT5, m_ctrlCRC);
}


BEGIN_MESSAGE_MAP(CDlgCRC, CDialogEx)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CDlgCRC::OnEnChangeMfceditbrowse1)
END_MESSAGE_MAP()


// CDlgCRC ��Ϣ�������


void CDlgCRC::OnEnChangeMfceditbrowse1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	u8 crc;

	CString filePath;
	m_ctrlFileToBeCRCed.UpdateWindow();
	m_ctrlFileToBeCRCed.GetWindowText(filePath);



	if (!generate_crc8_for_each_line_of_each_file_and_store_in_another_file(filePath))
		return;

	

}

u8 CDlgCRC::crc8_of_one_line_of_4bytes(CString s)
{
	u32 x = ((CHgzString)s).stoll(0, 16);
	return crc8((u8*)&x, 4);
}

bool CDlgCRC::generate_crc8_for_each_line_of_a_file_and_store_in_another_file(CString filePath)
{
	CStdioFile mFile;
	if (!mFile.Open(filePath, CFile::modeRead | CFile::shareDenyNone | CFile::typeText))
		return false;

	CHgzPath destPath(filePath);
	CString s = destPath.GetFileTitle();
	CString right = s.Right(5);
	CString left = s.Left(s.GetLength() - 5);
	left += _T("-crc8");
	left += right;

	destPath.ChangeFileTitle((TCHAR*)left.GetString());
	CStdioFile mFileCRC;
	mFileCRC.Open(destPath.GetFullPath(), CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeText | CFile::modeCreate);
	
	CHgzString src;
	while (mFile.ReadString(src)) {
		u8 crc = crc8_of_one_line_of_4bytes(src);
		src.AppendFormat(_T(":%02X\n"), crc);
		mFileCRC.WriteString(src);

	}

	mFile.Close();
	mFileCRC.Close();
	
	return true;
}

bool CDlgCRC::generate_crc8_for_each_line_of_each_file_and_store_in_another_file(CString filePath)
{
	CHgzPath fpath(filePath);
	fpath.ChangeFileName(_T("AddrMode1-150302-2013"));
	for (int i = 0; i <= 271; i++) {
		CHgzString s;
		s.Format(_T("-f%03d.txt"), i);
		CString fs(fpath.GetFullPath()+s);
		if (!generate_crc8_for_each_line_of_a_file_and_store_in_another_file(fs))
			break;
	}
	return true;
}
