// RollnumAndCPConfigDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "RollnumAndCPConfigDialog.h"
#include "afxdialogex.h"
#include "hgz\\HgzString.h"
#include "HgzMD5.h"


// CRollnumAndCPConfigDialog 对话框

const ATL::CPath CRollnumAndCPConfigDialog::s_strConfigFilePath1 = _T("\\");
const ATL::CPath CRollnumAndCPConfigDialog::s_strConfigFilePath2 = _T("\\SiteInfo\\");
const ATL::CPath CRollnumAndCPConfigDialog::s_strHashFilePath = _T("\\Hash\\");
const ATL::CPath CRollnumAndCPConfigDialog::s_strConfigFileName1 = _T("CP_Config_File1.txt");
const ATL::CPath CRollnumAndCPConfigDialog::s_strConfigFileName2 = _T("CP_Config_File2.txt");

const TCHAR CRollnumAndCPConfigDialog::s_strLinePrefix[][50] = {
    _T("Enable OTP writing"),
    _T("Firmware file to write"),
    _T("4_bytes_RF_syncode file"),
    _T("3_bytes_RF_syncode file"),
    _T("RF communication test Frequency 1"),
    _T("RF communication test Frequency 2"),
    _T("RF communication test Frequency 3"),
    _T("OTP address to write rollnum"),
    _T("Rollnum min"),
    _T("Rollnum max"),
    _T("Next rollnum"),
    _T("Previous updated rollnum")
};

const int CRollnumAndCPConfigDialog::s_RF_Freq[8][3] = {
    {2486, 2487, 2488},
    {2491, 2492, 2493},
    {2498, 2499, 2500},
    {2503, 2504, 2505},
    {2508, 2509, 2510},
    {2514, 2515, 2516},
    {2519, 2520, 2521},
    {2524, 2525, 2526}
};

IMPLEMENT_DYNAMIC(CRollnumAndCPConfigDialog, CDialogEx)

CRollnumAndCPConfigDialog::CRollnumAndCPConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRollnumAndCPConfigDialog::IDD, pParent)
{
    m_vTest_OTPWrite.RadixOfNextRollnumInCfgFile = 16;
    m_color_config1 = hgzColor_DarkGreen;
    m_color_config2 = hgzColor_DarkGreen;

    //m_brush.CreateSolidBrush(hgzColor_DarkYellow);
}

CRollnumAndCPConfigDialog::~CRollnumAndCPConfigDialog()
{
    //m_brush.DeleteObject();
}

void CRollnumAndCPConfigDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_CHECK2, m_ctrlEnableOTPWriting);
    DDX_Control(pDX, IDC_EDIT15, m_ctrlEnableOTPWriting);
    DDX_Control(pDX, IDC_EDIT4, m_ctrlFirmwareFileToWrite);
    DDX_Control(pDX, IDC_EDIT5, m_ctrlRFSynccode4BFile);
    DDX_Control(pDX, IDC_EDIT6, m_ctrlRFSynccode3BFile);
    DDX_Control(pDX, IDC_EDIT7, m_ctrlRFCommFreq[0]);
    DDX_Control(pDX, IDC_EDIT8, m_ctrlRFCommFreq[1]);
    DDX_Control(pDX, IDC_EDIT9, m_ctrlRFCommFreq[2]);
    DDX_Control(pDX, IDC_EDIT10, m_ctrlOTPAddrForRollnum);
    DDX_Control(pDX, IDC_EDIT11, m_ctrlRollnumMin);
    DDX_Control(pDX, IDC_EDIT12, m_ctrlRollnumMax);
    DDX_Control(pDX, IDC_EDIT13, m_ctrlNextRollnum);
    DDX_Control(pDX, IDC_EDIT14, m_ctrlPreviousUpdatedRollnum);
    DDX_Control(pDX, IDC_CHECK1, m_chk_En_CP_Config_File1_Hash);
    DDX_Control(pDX, IDC_CHECK8, m_chk_En_CP_Config_File2_Hash);
    DDX_Control(pDX, IDC_CHECK9, m_chk_En_FirmwareToWrite_Hash);
    DDX_Control(pDX, IDC_CHECK10, m_chk_En_RFSynFile4B_Hash);
    DDX_Control(pDX, IDC_CHECK11, m_chk_En_RFSynFile3B_Hash);
    DDX_Control(pDX, IDC_CHECK3, m_chk_VerifyHash);
    DDX_Control(pDX, IDC_CHECK12, m_chk_GenerateHash);
    DDX_Control(pDX, IDC_COMBO1, m_ctrl_RFFreqGroup);
    DDX_Control(pDX, IDC_COMBO2, m_cmb_Enable_OTP_writing);
}


BEGIN_MESSAGE_MAP(CRollnumAndCPConfigDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CRollnumAndCPConfigDialog::OnBnClickedButton_SelectConfigFilePath)
    ON_BN_CLICKED(IDC_BUTTON6, &CRollnumAndCPConfigDialog::OnBnClickedButton_ParseConfigFile)
    ON_BN_CLICKED(IDC_BUTTON7, &CRollnumAndCPConfigDialog::OnBnClickedButton_UpdateConfigFile)
    ON_BN_CLICKED(IDC_BUTTON3, &CRollnumAndCPConfigDialog::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CRollnumAndCPConfigDialog::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &CRollnumAndCPConfigDialog::OnBnClickedButton5)
    ON_BN_CLICKED(IDOK, &CRollnumAndCPConfigDialog::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON8, &CRollnumAndCPConfigDialog::OnBnClickedButton_GenerateHashFiles)
    ON_BN_CLICKED(IDC_BUTTON9, &CRollnumAndCPConfigDialog::OnBnClickedButton_VerifyHashFiles)
    ON_WM_CTLCOLOR()
    ON_CBN_SELCHANGE(IDC_COMBO2, &CRollnumAndCPConfigDialog::OnCbnSelchangeCombo2)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CRollnumAndCPConfigDialog::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CRollnumAndCPConfigDialog 消息处理程序


//void CRollnumAndCPConfigDialog::OnBnClickedButton_SelectConfigFilePath()
//{
//    CFolderPickerDialog dlg;
//    if (dlg.DoModal() != IDOK)
//    {
//        return;
//    }
//
//    m_CPConfigFilePath = dlg.GetPathName();
//    TCHAR stemp[1024];
//    GetCurrentDirectory(1024, stemp);
//    CString s = stemp;
//    u32 n = m_CPConfigFilePath.Find(s + _T('\\'));
//    if (n != -1)
//        AfxGetApp()->WriteProfileString(_T("Settings"), _T("CPConfigFilePath"), m_CPConfigFilePath.Mid(n+s.GetLength()));
//    else
//        AfxGetApp()->WriteProfileString(_T("Settings"), _T("CPConfigFilePath"), m_CPConfigFilePath);
//    
//    SetDlgItemText(IDC_EDIT_CPConfigFilePath, m_CPConfigFilePath);
//}

void CRollnumAndCPConfigDialog::OnBnClickedButton_SelectConfigFilePath()
{
    CFolderPickerDialog dlg;
    if (dlg.DoModal() != IDOK)
    {
        return;
    }

    m_CPConfigFilePath = (ATL::CPath)dlg.GetPathName();
    TCHAR stemp[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, stemp);
    if (m_CPConfigFilePath.IsPrefix(stemp))
    {
        ATL::CPath pathTemp;
        pathTemp.RelativePathTo(stemp, FILE_ATTRIBUTE_DIRECTORY, m_CPConfigFilePath, FILE_ATTRIBUTE_DIRECTORY);
        AfxGetApp()->WriteProfileString(_T("Settings"), _T("CPConfigFilePath"), pathTemp);
    }
    else
        AfxGetApp()->WriteProfileString(_T("Settings"), _T("CPConfigFilePath"), m_CPConfigFilePath);

    SetDlgItemText(IDC_EDIT_CPConfigFilePath, m_CPConfigFilePath);
}

void CRollnumAndCPConfigDialog::OnBnClickedButton_ParseConfigFile()
{
    ConfigFile_Parser();
    UpdateDisplay();
    
    if (m_chk_VerifyHash.GetCheck())
        OnBnClickedButton_VerifyHashFiles();

    UpdateDisplay();
}


void CRollnumAndCPConfigDialog::OnBnClickedButton_UpdateConfigFile()
{
    // Check if all items ready
    UpdateData_To_Value();
    TEST_OTPWRITE_t cfg;
    CONFIG_FILE_CHECK_t check;
    check.CP_Config_File1 = true;
    check.CP_Config_File2 = true;
    Convert_Parameters(cfg);
    CheckParameters(cfg, check);
    

    // open or create config file, write contents to config file, close config file
    CStdioFile CP_Config_File1, CP_Config_File2;
    
    if (CP_Config_File1.Open((CString &)m_CPConfigFilePath+(CString &)s_strConfigFilePath1+(CString &)s_strConfigFileName1, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate))
    {
        _tprintf(_T("\r\nWrite into Chip-Probing Config file: %s\n"), s_strConfigFileName1);
        
        // clear file
        CP_Config_File1.SetLength(0);
        
        // Write into file
        CString s, s1;
        s.Format(_T("%s: %s\n"), s_strLinePrefix[0], (m_ctrlEnableOTPWriting.GetWindowText(s1), s1));
        CP_Config_File1.WriteString(s);

        s.Format(_T("%s: %s\n"), s_strLinePrefix[1], (m_ctrlFirmwareFileToWrite.GetWindowText(s1), s1));
        CP_Config_File1.WriteString(s);
        
        CP_Config_File1.Close();
    }
    else
    {
        check.CP_Config_File1 = false;
        AfxMessageBox(_T("Warning: Failed to open/create file: CP_Config_File1.txt.\n"));
    }

    if (CP_Config_File2.Open((CString &)m_CPConfigFilePath+(CString &)s_strConfigFilePath2+(CString &)s_strConfigFileName2, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate))
    {
        _tprintf(_T("\r\nWrite into Chip-Probing Config file: %s\r\n"), s_strConfigFileName2);

        // clear file
        CP_Config_File2.SetLength(0);

        // Write into file
        CString s, s1;
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[2], (m_ctrlRFSynccode4BFile.GetWindowText(s1), s1)), s));
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[3], (m_ctrlRFSynccode3BFile.GetWindowText(s1), s1)), s));
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[4], (m_ctrlRFCommFreq[0].GetWindowText(s1), s1)), s));
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[5], (m_ctrlRFCommFreq[1].GetWindowText(s1), s1)), s));
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[6], (m_ctrlRFCommFreq[2].GetWindowText(s1), s1)), s));
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[7], (m_ctrlOTPAddrForRollnum.GetWindowText(s1), s1)), s));
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[8], (m_ctrlRollnumMin.GetWindowText(s1), s1)), s));
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[9], (m_ctrlRollnumMax.GetWindowText(s1), s1)), s));
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[10], (m_ctrlNextRollnum.GetWindowText(s1), s1)), s));
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[11], (m_ctrlPreviousUpdatedRollnum.GetWindowText(s1), s1.IsEmpty() ? (m_ctrlNextRollnum.GetWindowText(s1), s1) : s1)), s));

        CP_Config_File2.Close();
    }
    else
    {
        check.CP_Config_File2 = false;
        AfxMessageBox(_T("Warning: Failed to open file: CP_Config_File2.txt.\r\n"));
    }

    if (m_chk_GenerateHash.GetCheck())
        OnBnClickedButton_GenerateHashFiles();


    if (check.CP_Config_File1) {
        m_color_config1 = hgzColor_DarkGreen;
        SetDlgItemText(IDC_CPCONFIGFILE1_CHECK, _T("格式与参数检查：√"));
    }
    else {
        m_color_config1 = hgzColor_Red;
        SetDlgItemText(IDC_CPCONFIGFILE1_CHECK, _T("格式与参数检查：X"));
    }

    if (check.CP_Config_File2) {
        m_color_config2 = hgzColor_DarkGreen;
        SetDlgItemText(IDC_CPCONFIGFILE2_CHECK, _T("格式与参数检查：√"));
    }
    else {
        m_color_config2 = hgzColor_Red;
        SetDlgItemText(IDC_CPCONFIGFILE2_CHECK, _T("格式与参数检查：X"));
    }

    RedrawWindow();

}

void CRollnumAndCPConfigDialog::OnBnClickedButton_GenerateHashFiles()
{
    // Generate MD5 file in subdirectory "Hash"
    CHgzMD5 md5;
    CString s, sfn1, sfn2, sfn3;
    ATL::CPath path1;
    
    sfn1 = (CString &)m_CPConfigFilePath + (CString &)s_strHashFilePath;
    
    // 1. CP_Config_File1.md5
    if (m_chk_En_CP_Config_File1_Hash.GetCheck())
    {
        m_ConfigFileCheck.CP_Config_File1_Hash = true;
        sfn2 = (CString &)m_CPConfigFilePath + (CString &)s_strConfigFilePath1 + (CString &)s_strConfigFileName1;
        path1 = s_strConfigFileName1;
        path1.RenameExtension(_T(".md5"));
        sfn3 = sfn1 + (CString &)path1;
        md5.md5txt(sfn2, &sfn3, false, (CStatic *)&m_chk_En_CP_Config_File1_Hash);
    }

    // 2. Firmware.md5
    if (m_chk_En_FirmwareToWrite_Hash.GetCheck())
    {
        m_ConfigFileCheck.Program_File_Hash = true;
        m_ctrlFirmwareFileToWrite.GetWindowText(s);
        sfn2 = (CString &)m_CPConfigFilePath + (CString &)s_strConfigFilePath1 + s;
        path1 = ATL::CPath(s);
        path1.RenameExtension(_T(".md5"));
        sfn3 = sfn1 + (CString &)path1;
        md5.md5bin(sfn2, &sfn3, 256, (CStatic *)&m_chk_En_FirmwareToWrite_Hash);
    }

    // 3. CP_Config.File2.md5
    if (m_chk_En_CP_Config_File2_Hash.GetCheck())
    {
        m_ConfigFileCheck.CP_Config_File2_Hash = true;
        sfn2 = (CString &)m_CPConfigFilePath + (CString &)s_strConfigFilePath2 + (CString &)s_strConfigFileName2;
        path1 = s_strConfigFileName2;
        path1.RenameExtension(_T(".md5"));
        sfn3 = sfn1 + (CString &)path1;
        md5.md5txt(sfn2, &sfn3, false, (CStatic *)&m_chk_En_CP_Config_File2_Hash);
    }

    // 4. 4_bytes_RF_synccode_file.md5
    if (m_chk_En_RFSynFile4B_Hash.GetCheck())
    {
        m_ConfigFileCheck.RFSynccode4B_Hash = true;
        m_ctrlRFSynccode4BFile.GetWindowText(s);
        sfn2 = (CString &)m_CPConfigFilePath + (CString &)s_strConfigFilePath2 + s;
        path1 = ATL::CPath(s);
        path1.RenameExtension(_T(".md5"));
        sfn3 = sfn1 + (CString &)path1;
        if (!CheckRFSyncFile(sfn2, 4, (CStatic *)&m_chk_En_RFSynFile4B_Hash))
        {
            m_ConfigFileCheck.RFSynccode4B_Hash = false;
        }
        else
            md5.md5txt(sfn2, &sfn3, false, (CStatic *)&m_chk_En_RFSynFile4B_Hash);
    }

    // 5. 3_bytes_RF_synccode_file.md5
    if (m_chk_En_RFSynFile3B_Hash.GetCheck())
    {
        m_ConfigFileCheck.RFSynccode3B_Hash = true;
        m_ctrlRFSynccode3BFile.GetWindowText(s);
        sfn2 = (CString &)m_CPConfigFilePath + (CString &)s_strConfigFilePath2 + s;
        path1 = ATL::CPath(s);
        path1.RenameExtension(_T(".md5"));
        sfn3 = sfn1 + (CString &)path1;
        if (!CheckRFSyncFile(sfn2, 3, (CStatic *)&m_chk_En_RFSynFile3B_Hash))
        {
            m_ConfigFileCheck.RFSynccode3B_Hash = false;
        }
        else
            md5.md5txt(sfn2, &sfn3, false, (CStatic *)&m_chk_En_RFSynFile3B_Hash);
    }

    RedrawWindow();
}

void CRollnumAndCPConfigDialog::OnBnClickedButton_VerifyHashFiles()
{
    // Verify MD5 file in subdirectory "Hash"
    CHgzMD5 md5;
    CString s, sfn1, sfn2, sfn3;
    ATL::CPath path1;

    sfn1 = (CString &)m_CPConfigFilePath + (CString &)s_strHashFilePath;

    // 1. CP_Config_File1.md5
    if (m_chk_En_CP_Config_File1_Hash.GetCheck())
    {
        sfn2 = (CString &)m_CPConfigFilePath + (CString &)s_strConfigFilePath1 + (CString &)s_strConfigFileName1;
        path1 = s_strConfigFileName1;
        path1.RenameExtension(_T(".md5"));
        sfn3 = sfn1 + (CString &)path1;
        if (md5.md5txt(sfn2, &sfn3, false, (CStatic *)&m_chk_En_CP_Config_File1_Hash, true))
            m_ConfigFileCheck.CP_Config_File1_Hash = true;
        else
            m_ConfigFileCheck.CP_Config_File1_Hash = false;
        //m_chk_En_CP_Config_File1_Hash.RedrawWindow();
    }
    
    // 2. Firmware.md5
    if (m_chk_En_FirmwareToWrite_Hash.GetCheck())
    {
        m_ctrlFirmwareFileToWrite.GetWindowText(s);
        sfn2 = (CString &)m_CPConfigFilePath + (CString &)s_strConfigFilePath1 + s;
        path1 = ATL::CPath(s);
        path1.RenameExtension(_T(".md5"));
        sfn3 = sfn1 + (CString &)path1;
        if (md5.md5bin(sfn2, &sfn3, 256, (CStatic *)&m_chk_En_FirmwareToWrite_Hash, true))
            m_ConfigFileCheck.Program_File_Hash = true;
        else
            m_ConfigFileCheck.Program_File_Hash = false;
        //m_chk_En_FirmwareToWrite_Hash.RedrawWindow();
    }

    // 3. CP_Config.File2.md5
    if (m_chk_En_CP_Config_File2_Hash.GetCheck())
    {
        sfn2 = (CString &)m_CPConfigFilePath + (CString &)s_strConfigFilePath2 + (CString &)s_strConfigFileName2;
        path1 = s_strConfigFileName2;
        path1.RenameExtension(_T(".md5"));
        sfn3 = sfn1 + (CString &)path1;
        if (md5.md5txt(sfn2, &sfn3, false, (CStatic *)&m_chk_En_CP_Config_File2_Hash, true))
            m_ConfigFileCheck.CP_Config_File2_Hash = true;
        else
            m_ConfigFileCheck.CP_Config_File2_Hash = false;
        //m_chk_En_CP_Config_File2_Hash.RedrawWindow();
    }

    // 4. 4_bytes_RF_synccode_file.md5
    do {
        if (m_chk_En_RFSynFile4B_Hash.GetCheck())
        {
            m_ctrlRFSynccode4BFile.GetWindowText(s);
            sfn2 = (CString &)m_CPConfigFilePath + (CString &)s_strConfigFilePath2 + s;
            path1 = ATL::CPath(s);
            path1.RenameExtension(_T(".md5"));
            sfn3 = sfn1 + (CString &)path1;
            if (!CheckRFSyncFile(sfn2, 4, (CStatic *)&m_chk_En_RFSynFile4B_Hash))
            {
                m_ConfigFileCheck.RFSynccode4B_Hash = false;
                break;
            }
            if (md5.md5txt(sfn2, &sfn3, false, (CStatic *)&m_chk_En_RFSynFile4B_Hash, true))
                m_ConfigFileCheck.RFSynccode4B_Hash = true;
            else
                m_ConfigFileCheck.RFSynccode4B_Hash = false;
            //m_chk_En_RFSynFile4B_Hash.RedrawWindow();
        }
    } while (0);

    // 5. 3_bytes_RF_synccode_file.md5
    do {
        if (m_chk_En_RFSynFile3B_Hash.GetCheck())
        {
            m_ctrlRFSynccode3BFile.GetWindowText(s);
            sfn2 = (CString &)m_CPConfigFilePath + (CString &)s_strConfigFilePath2 + s;
            path1 = ATL::CPath(s);
            path1.RenameExtension(_T(".md5"));
            sfn3 = sfn1 + (CString &)path1;
            if (!CheckRFSyncFile(sfn2, 3, (CStatic *)&m_chk_En_RFSynFile3B_Hash))
            {
                m_ConfigFileCheck.RFSynccode3B_Hash = false;
                break;
            }
            if (md5.md5txt(sfn2, &sfn3, false, (CStatic *)&m_chk_En_RFSynFile3B_Hash, true))
                m_ConfigFileCheck.RFSynccode3B_Hash = true;
            else
                m_ConfigFileCheck.RFSynccode3B_Hash = false;
            //m_chk_En_RFSynFile3B_Hash.RedrawWindow();
        }
    } while (0);

    RedrawWindow();
}

BOOL CRollnumAndCPConfigDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    /** You can prevent visual styles from being applied to a specified window by specifying an empty string, (T("")), which does not match any section entries.
     *  Don't call this function in OnCtlColor(), or the control's text will flash constantly.
     */
    SetWindowTheme(m_chk_En_CP_Config_File1_Hash.GetSafeHwnd(), _T(""), _T(""));
    SetWindowTheme(m_chk_En_CP_Config_File2_Hash.GetSafeHwnd(), _T(""), _T(""));
    SetWindowTheme(m_chk_En_FirmwareToWrite_Hash.GetSafeHwnd(), _T(""), _T(""));
    SetWindowTheme(m_chk_En_RFSynFile4B_Hash.GetSafeHwnd(), _T(""), _T(""));
    SetWindowTheme(m_chk_En_RFSynFile3B_Hash.GetSafeHwnd(), _T(""), _T(""));
    
    SetWindowTheme(GetDlgItem(IDC_CP_CONFIG_FILE1)->GetSafeHwnd(), _T(""), _T(""));
    SetWindowTheme(GetDlgItem(IDC_CP_CONFIG_FILE2)->GetSafeHwnd(), _T(""), _T(""));
    SetWindowTheme(GetDlgItem(IDC__HASH)->GetSafeHwnd(), _T(""), _T(""));
    
    m_ConfigFileCheck.CP_Config_File1_Hash = true;
    m_ConfigFileCheck.CP_Config_File2_Hash = true;
    m_ConfigFileCheck.Program_File_Hash = true;
    m_ConfigFileCheck.RFSynccode4B_Hash = true;
    m_ConfigFileCheck.RFSynccode3B_Hash = true;




    m_vTest_OTPWrite.enableOTPWriting = false;

    SetDlgItemText(IDC_EDIT_CPConfigFilePath, m_CPConfigFilePath);
    ConfigFile_Parser();
    UpdateDisplay();

    ((CButton *)GetDlgItem(IDC_BUTTON8))->ShowWindow(0);
    ((CButton *)GetDlgItem(IDC_BUTTON9))->ShowWindow(0);
    m_chk_GenerateHash.ShowWindow(0);
    m_chk_VerifyHash.ShowWindow(0);

    m_chk_En_CP_Config_File1_Hash.SetCheck(true);
    m_chk_En_CP_Config_File2_Hash.SetCheck(true);
    m_chk_En_FirmwareToWrite_Hash.SetCheck(true);
    m_chk_En_RFSynFile4B_Hash.SetCheck(false);
    m_chk_En_RFSynFile3B_Hash.SetCheck(false);
    m_chk_GenerateHash.SetCheck(true);
    m_chk_VerifyHash.SetCheck(true);


    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

#define Enable_CP_Config_File_Hash_Check_In_Parser 0
void CRollnumAndCPConfigDialog::ConfigFile_Parser(void)
{
    for (int i = 0; i < CFG_PARAM_NUM; i++)
        _tcscpy(s_strLineValue[i], _T(""));

    
    CStdioFile CP_Config_File;
    CStdioFile CP_Config_File_Hash;
    
    try
    {
        CFileException mExcept;
        if (CP_Config_File.Open((CString &)m_CPConfigFilePath+(CString &)s_strConfigFilePath1+(CString &)s_strConfigFileName1, CFile::modeRead | CFile::shareDenyNone, &mExcept))
        {
#if Enable_CP_Config_File_Hash_Check_In_Parser
            CHgzPath hash_file_name((CString &)m_CPConfigFilePath+(CString &)s_strHashFilePath+(CString &)s_strConfigFileName1);
            hash_file_name.ChangeFileExt(_T(".md5"));
            if (CP_Config_File_Hash.Open(hash_file_name.GetFullPath(), CFile::modeRead | CFile::shareDenyNone, &mExcept))
                m_ConfigFileCheck.CP_Config_File1_Hash = true;
            else
            {
                m_ConfigFileCheck.CP_Config_File1_Hash = false;
                hgzMessageBox(_T("There is no Hash file, Verification ommitted.\n\n") + hash_file_name.GetFullPath());
            }

            int line_cnt = 0;
#endif
            // parse CP_Config_File1:
            m_ConfigFileCheck.CP_Config_File1 = true;
            _tprintf(_T("\r\nRead Chip-Probing Config file: %s\r\n"), s_strConfigFileName1);
            
            CString s;
            while (CP_Config_File.ReadString(s))
            {
#if Enable_CP_Config_File_Hash_Check_In_Parser
                line_cnt++;
                if (m_ConfigFileCheck.CP_Config_File1_Hash)
                {
                    CHgzMD5 md5;
                    if (md5.IsCorrect((CStringA)s.GetString(), &CP_Config_File_Hash, -1, 0, DEBUG_PRINT) == NULL)
                    {
                        hgzMessageBox(_T("Hash verification fails. \n\n %s\n\n line \t= %d\n string \t= %s"), hash_file_name.GetFullPath(), line_cnt, s);
                        m_ConfigFileCheck.CP_Config_File1 = false;
                        CP_Config_File_Hash.Close();
                        CP_Config_File.Close();
                        return;
                    }
                }
#endif
                ConfigFile_LineParser(s, CP_Config_File);
            }
            CP_Config_File.Close();
#if Enable_CP_Config_File_Hash_Check_In_Parser
            if (m_ConfigFileCheck.CP_Config_File1_Hash)
                CP_Config_File_Hash.Close();
#endif
        }
        else
        {
            m_ConfigFileCheck.CP_Config_File1 = false;
            //AfxMessageBox(_T("Warning: Failed to open file: CP_Config_File1.txt.\r\n"));
			print(0, _T("Warning: Failed to open file: CP_Config_File1.txt.\r\n"));
        }

        if (CP_Config_File.Open((CString &)m_CPConfigFilePath+(CString &)s_strConfigFilePath2+(CString &)s_strConfigFileName2, CFile::modeRead | CFile::shareDenyNone, &mExcept))
        {
#if Enable_CP_Config_File_Hash_Check_In_Parser
            CHgzPath hash_file_name((CString &)m_CPConfigFilePath+(CString &)s_strHashFilePath+(CString &)s_strConfigFileName2);
            hash_file_name.ChangeFileExt(_T(".md5"));
            if (CP_Config_File_Hash.Open(hash_file_name.GetFullPath(), CFile::modeRead | CFile::shareDenyNone, &mExcept))
                m_ConfigFileCheck.CP_Config_File2_Hash = true;
            else
            {
                m_ConfigFileCheck.CP_Config_File2_Hash = false;
                hgzMessageBox(_T("There is no Hash file, Verification ommitted.\n\n") + hash_file_name.GetFullPath());
            }

            int line_cnt = 0;
#endif
            // parse CP_Config_File1:
            m_ConfigFileCheck.CP_Config_File2 = true;
            _tprintf(_T("\r\nRead Chip-Probing Config file: %s\r\n"), s_strConfigFileName2);
            
            CString s;
            while (CP_Config_File.ReadString(s))
            {
#if Enable_CP_Config_File_Hash_Check_In_Parser
                line_cnt++;
                if (m_ConfigFileCheck.CP_Config_File2_Hash)
                {
                    CHgzMD5 md5;
                    if (md5.IsCorrect((CStringA)s.GetString(), &CP_Config_File_Hash, -1, 0, DEBUG_PRINT) == NULL)
                    {
                        hgzMessageBox(_T("Hash verification fails. \n\n %s\n\n line \t= %d\n string \t= %s"), hash_file_name.GetFullPath(), line_cnt, s);
                        m_ConfigFileCheck.CP_Config_File2 = false;
                        CP_Config_File_Hash.Close();
                        CP_Config_File.Close();
                        return;
                    }
                }
#endif
                ConfigFile_LineParser(s, CP_Config_File);
            }
            CP_Config_File.Close();
#if Enable_CP_Config_File_Hash_Check_In_Parser
            if (m_ConfigFileCheck.CP_Config_File2_Hash)
                CP_Config_File_Hash.Close();
#endif
        }
        else
        {
            m_ConfigFileCheck.CP_Config_File2 = false;
            //AfxMessageBox(_T("Warning: Failed to open file: CP_Config_File2.txt.\r\n"));
			print(0, _T("Warning: Failed to open file: CP_Config_File2.txt.\r\n"));
        }

    }
    catch (CFileException* e)
    {
        AfxMessageBox(_T("Warning: Failed to open file.\r\n"));
        return;
    }
   
    // Is there any line not existing in config file?
    if (m_ConfigFileCheck.CP_Config_File1 && m_ConfigFileCheck.CP_Config_File2)
    {
        for (int i = 0; i < CFG_PARAM_NUM-1; i++)
        {
            if (_tcscmp(_T(""), s_strLineValue[i]) == 0)
            {
                if (i < 2)
                    m_ConfigFileCheck.CP_Config_File1 = false;
                else
                    m_ConfigFileCheck.CP_Config_File2 = false;

                hgzMessageBox(_T("Warning: ConfigFile_Parser() -- Missing parameter \"%s\" in the configuration file.\r\n"), s_strLinePrefix[i]);
            }
        }
    }
    if (m_ConfigFileCheck.CP_Config_File1 && m_ConfigFileCheck.CP_Config_File2)
    {
        Convert_Parameters(m_vTest_OTPWrite);
        CheckParameters(m_vTest_OTPWrite, m_ConfigFileCheck);
    }

}


void CRollnumAndCPConfigDialog::ConfigFile_LineParser( CString& s, CStdioFile& f )
{
    // printf("String to parse: %s\r\n", s);

    for (int i = 0; i < CFG_PARAM_NUM ; i++)
    {
         int n = s.Find(s_strLinePrefix[i], 0);
        if (n == -1)
            continue;

        n += _tcslen(s_strLinePrefix[i]);
        
        n = s.Find(':', n);
        if (n == -1)
        {
            hgzMessageBox(_T("Warning: Syntex error - Can't find ':' in line %s!\r\n"), s_strLinePrefix[i]);
            break;
        }
        
        while (s.Find(' ', n) != -1)
            n++;

        s.Replace(_T("\n"), _T(""));
        s.Replace(_T("\r"), _T(""));

        _tcscpy(s_strLineValue[i], s.Right(s.GetLength()-n));

        if (i == CFG_PARAM_NUM-3)
            m_vTest_OTPWrite.offsetOfStartRollnumLineInConfigFile = f.GetPosition();

        _tprintf(_T("%s: %s\r\n"), s_strLinePrefix[i], s_strLineValue[i]);

        break;

    }

}

void CRollnumAndCPConfigDialog::UpdateDisplay()
{
    // Update display.
    // format and parameters check
    SetDlgItemText(IDC_CPCONFIGFILE1_CHECK, m_ConfigFileCheck.CP_Config_File1 ? _T("格式与参数检查：√") : _T("格式与参数检查：X"));
    m_color_config1 = m_ConfigFileCheck.CP_Config_File1 ? hgzColor_DarkGreen : hgzColor_Red;
    SetDlgItemText(IDC_CPCONFIGFILE2_CHECK, m_ConfigFileCheck.CP_Config_File2 ? _T("格式与参数检查：√") : _T("格式与参数检查：X"));
    m_color_config2 = m_ConfigFileCheck.CP_Config_File2 ? hgzColor_DarkGreen : hgzColor_Red;


    UpdateData_To_Ctrol();
/*
    m_ctrlEnableOTPWriting.SetWindowText(s_strLineValue[0]);
    m_cmb_Enable_OTP_writing.SetCurSel(m_vTest_OTPWrite.enableOTPWriting);
    m_ctrlFirmwareFileToWrite.SetWindowText(s_strLineValue[1]);
    m_ctrlRFSynccode4BFile.SetWindowText(s_strLineValue[2]);
    m_ctrlRFSynccode3BFile.SetWindowText(s_strLineValue[3]);
    m_ctrlRFCommFreq[0].SetWindowText(s_strLineValue[4]);
    m_ctrlRFCommFreq[1].SetWindowText(s_strLineValue[5]);
    m_ctrlRFCommFreq[2].SetWindowText(s_strLineValue[6]);
    m_ctrl_RFFreqGroup.SetCurSel(Get_RF_Freq_Group(m_vTest_OTPWrite.freq[0], m_vTest_OTPWrite.freq[1], m_vTest_OTPWrite.freq[2]));
    m_ctrlOTPAddrForRollnum.SetWindowText(s_strLineValue[7]);
    m_ctrlRollnumMin.SetWindowText(s_strLineValue[8]);
    m_ctrlRollnumMax.SetWindowText(s_strLineValue[9]);
    m_ctrlNextRollnum.SetWindowText(s_strLineValue[10]);
    m_ctrlPreviousUpdatedRollnum.SetWindowText(s_strLineValue[11]);
*/

    RedrawWindow();
}

void CRollnumAndCPConfigDialog::OnBnClickedButton3()
{
    CString strExt = _T("Bin Files (*.bin)|*.bin|All Files (*.*)|*.*||");
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strExt);

    if (dlg.DoModal() != IDOK)
    {
        return;
    }

    m_ctrlFirmwareFileToWrite.SetWindowText(dlg.GetFileName());
}

void CRollnumAndCPConfigDialog::OnBnClickedButton4()
{
    CFileDialog dlg(TRUE);
    if (dlg.DoModal() != IDOK)
    {
        return;
    }

    m_ctrlRFSynccode4BFile.SetWindowText(dlg.GetFileName());
}


void CRollnumAndCPConfigDialog::OnBnClickedButton5()
{
    CFileDialog dlg(TRUE);
    if (dlg.DoModal() != IDOK)
    {
        return;
    }

    m_ctrlRFSynccode3BFile.SetWindowText(dlg.GetFileName());
}

void CRollnumAndCPConfigDialog::SD_FillRollnumRFSynccode(void)
{
    // Fill next Rollnum_Synccode struct: read synccode from synccode files.
    m_vTest_OTPWrite.rsTable.synccode4b = m_ConfigFileCheck.CP_Config_File2 ? SD_GetRFSynccode((CString &)m_CPConfigFilePath+(CString &)s_strConfigFilePath2+m_vTest_OTPWrite.RFSynccode4B, m_vTest_OTPWrite.rsTable.rollnum) : 0;
    m_vTest_OTPWrite.rsTable.synccode3b = m_ConfigFileCheck.CP_Config_File2 ? SD_GetRFSynccode((CString &)m_CPConfigFilePath+(CString &)s_strConfigFilePath2+m_vTest_OTPWrite.RFSynccode3B, m_vTest_OTPWrite.rsTable.rollnum) : 0;
    m_vTest_OTPWrite.rsTable.reserved = 0x00000000;
}

u32 CRollnumAndCPConfigDialog::SD_GetRFSynccode(CString& fileName, u32 rollnum)
{
    /* Assume one synccode occupies one line, with a '\n' (not "\r\n") as the line terminator.
       There must not be any space charactors in the line. 
       Line number base from 0, for example, Line0 accords to rollnum 0.
    
       If there's any error, return 0xFFFFFFFF (-1, NULL_NUM).
    */
    
    CStdioFile f;
    if (!f.Open(fileName, CFile::modeRead | CFile::shareDenyNone))
    {
//         CString s;
//         s.Format(_T("Cannot open file: %s"), fileName);
        AfxMessageBox(_T("Cannot open file: ")+fileName);
        return 0;
    }

    if (f.GetLength() == 0)
    {
        AfxMessageBox(_T("File length is 0: ")+fileName);
        return 0;
    }

    CHgzString s;
    f.ReadString(s);
    u32 charsPerLine = f.GetPosition();
    u32 syncodeLength = s.GetLength();
    u32 totalLines = f.GetLength() / charsPerLine;

    if (syncodeLength != 6 && syncodeLength != 8)
    {
        AfxMessageBox(_T("RF synccode length is not 3 bytes and 4 bytes: ")+fileName);
        return 0;
    }

    u32 offset = charsPerLine * (rollnum%totalLines);
    
    f.Seek(offset, CFile::begin);
    
    f.ReadString(s);
    u32 x = s.GetLength();
    if (x < syncodeLength)
    {
        AfxMessageBox(_T("RF Synccode is too short!"));
        return 0;
    }

    u32 syncode = s.stoul(0, 16);
    
    return syncode;
}

void CRollnumAndCPConfigDialog::OnBnClickedOk()
{
    OnBnClickedButton_UpdateConfigFile();
    OnBnClickedButton_ParseConfigFile();

    CDialogEx::OnOK();
}

bool CRollnumAndCPConfigDialog::IsRollnumValid( void )
{
    if (m_vTest_OTPWrite.rsTable.rollnum > m_vTest_OTPWrite.rollnumMax || m_vTest_OTPWrite.rsTable.rollnum < m_vTest_OTPWrite.rollnumMin)
        return false;
    else
        return true;

}

bool CRollnumAndCPConfigDialog::IsRFSyncodeValid( void )
{
    if (m_vTest_OTPWrite.rsTable.synccode3b == 0 || m_vTest_OTPWrite.rsTable.synccode4b == 0)
        return false;
    else
        return true;

}

bool CRollnumAndCPConfigDialog::WriteRollnumToConfigFile( void )
{
    printf(m_vTest_OTPWrite.RadixOfNextRollnumInCfgFile==16 ? "Write rollnum to SD: 0x%08X\r\n" : (m_vTest_OTPWrite.RadixOfNextRollnumInCfgFile==10 ? "Write rollnum to SD: %d\r\n" : "Write rollnum to SD: 0%011o\r\n"), m_vTest_OTPWrite.rsTable.rollnum);

    CStdioFile CP_Config_File2;
    if (CP_Config_File2.Open((CString &)m_CPConfigFilePath+(CString &)s_strConfigFilePath2+(CString &)s_strConfigFileName2, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate))
    {
        _tprintf(_T("\r\nWrite into Chip-Probing Config file: %s\r\n"), s_strConfigFileName2);

        CHgzString s1, s2;
        
        CP_Config_File2.Seek(m_vTest_OTPWrite.offsetOfStartRollnumLineInConfigFile, CFile::begin);
        CP_Config_File2.SetLength(CP_Config_File2.GetPosition());
        s1.Format(m_vTest_OTPWrite.RadixOfNextRollnumInCfgFile==16 ? _T("%s: 0x%08X") : (m_vTest_OTPWrite.RadixOfNextRollnumInCfgFile==10 ? _T("%s: %d") : _T("%s: 0%011o")), s_strLinePrefix[CFG_PARAM_NUM-2], m_vTest_OTPWrite.rsTable.rollnum);
        CP_Config_File2.WriteString(s1+_T("\n"));
        s2.Format(m_vTest_OTPWrite.RadixOfNextRollnumInCfgFile==16 ? _T("%s: 0x%08X") : (m_vTest_OTPWrite.RadixOfNextRollnumInCfgFile==10 ? _T("%s: %d") : _T("%s: 0%011o")), s_strLinePrefix[CFG_PARAM_NUM-1], m_vTest_OTPWrite.rsTable.rollnum);
        CP_Config_File2.WriteString(s2+_T("\n"));
        CP_Config_File2.Close();

        // Update hash file
        CHgzPath fn = (CString &)m_CPConfigFilePath+(CString &)s_strHashFilePath+(CString &)s_strConfigFileName2;
        fn.ChangeFileExt(_T(".md5"));
        if (file_exist(fn.GetFullPath())) //if (m_ConfigFileCheck.CP_Config_File2_Hash)
        {
            // CFG_PARAM_NUM-3
            CStdioFile CP_Config_File2_Hash;
            if (CP_Config_File2_Hash.Open(fn.GetFullPath(), CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate | CFile::typeText))
            {
                CString s1md5, s2md5;

                CP_Config_File2_Hash.SeekToBegin();
                CP_Config_File2_Hash.ReadString(s1md5);
                int charsPerLine = CP_Config_File2_Hash.GetPosition();
                CP_Config_File2_Hash.SetLength(charsPerLine*(CFG_PARAM_NUM-2-2));
                CP_Config_File2_Hash.SeekToEnd();

                CHgzMD5 md5;
                s1md5 = md5.md5str(s1);
                s2md5 = md5.md5str(s2);

                CP_Config_File2_Hash.WriteString(s1md5+_T("\n"));
                CP_Config_File2_Hash.WriteString(s2md5+_T("\n"));

                CP_Config_File2_Hash.Close();
            }
            else
            {
                hgzMessageBox(_T("Hash file updating fails: \n%s"), fn.GetFullPath());
                return false;
            }
        }


        return true;
    }
    else
    {
        _tprintf(_T("Warning: Failed to open file: CP_Config_File2.txt.\r\n"));
        AfxMessageBox(_T("Warning: Failed to open file: CP_Config_File2.txt.\r\n"));
        return false;
    }


}





HBRUSH CRollnumAndCPConfigDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  在此更改 DC 的任何特性
    /*if (m_chk_En_CP_Config_File2_Hash.GetDlgCtrlID() == IDC_CHECK8)
    {
        pDC->SetTextColor(hgzColor_Red);
        pDC->SetBkColor(hgzColor_Yellow);
        pDC->SetBkMode(OPAQUE); // OPAQUE, TRANSPARENT
        return (HBRUSH)GetStockObject(NULL_BRUSH);
        //return m_brush;
    }*/

    switch (pWnd->GetDlgCtrlID())
    {
    case IDC__HASH:
        (m_ConfigFileCheck.CP_Config_File1_Hash && m_ConfigFileCheck.CP_Config_File2_Hash && m_ConfigFileCheck.Program_File_Hash && m_ConfigFileCheck.RFSynccode4B_Hash && m_ConfigFileCheck.RFSynccode3B_Hash) ? pDC->SetTextColor(hgzColor_DarkGreen) : pDC->SetTextColor(hgzColor_Red);
        break;
    case IDC_CHECK1:
        m_ConfigFileCheck.CP_Config_File1_Hash ? pDC->SetTextColor(hgzColor_DarkGreen) : pDC->SetTextColor(hgzColor_Red);
        break;
    case IDC_CPCONFIGFILE1_CHECK:
    case IDC_CP_CONFIG_FILE1:
        //m_ConfigFileCheck.CP_Config_File1 ? pDC->SetTextColor(hgzColor_DarkGreen) : pDC->SetTextColor(hgzColor_Red);
        pDC->SetTextColor(m_color_config1);
        break;

    case IDC_CHECK8:
        m_ConfigFileCheck.CP_Config_File2_Hash ? pDC->SetTextColor(hgzColor_DarkGreen) : pDC->SetTextColor(hgzColor_Red);
        break;
    case IDC_CPCONFIGFILE2_CHECK:
    case IDC_CP_CONFIG_FILE2:
        //m_ConfigFileCheck.CP_Config_File2 ? pDC->SetTextColor(hgzColor_DarkGreen) : pDC->SetTextColor(hgzColor_Red);
        pDC->SetTextColor(m_color_config2);
        break;

    case IDC_CHECK9:    m_ConfigFileCheck.Program_File_Hash    ? pDC->SetTextColor(hgzColor_DarkGreen) : pDC->SetTextColor(hgzColor_Red); break;
    case IDC_CHECK10:   m_ConfigFileCheck.RFSynccode4B_Hash    ? pDC->SetTextColor(hgzColor_DarkGreen) : pDC->SetTextColor(hgzColor_Red); break;
    case IDC_CHECK11:   m_ConfigFileCheck.RFSynccode3B_Hash    ? pDC->SetTextColor(hgzColor_DarkGreen) : pDC->SetTextColor(hgzColor_Red); break;
    default: break;
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}




void CRollnumAndCPConfigDialog::OnCbnSelchangeCombo2()
{
    m_ctrlEnableOTPWriting.SetWindowsTextFormat(_T("%d"), m_cmb_Enable_OTP_writing.GetCurSel());
}


void CRollnumAndCPConfigDialog::OnCbnSelchangeCombo1()
{
    int n  = m_ctrl_RFFreqGroup.GetCurSel();
    for (int i = 0; i < 3; i++){
        CString s;
        s.Format(_T("%d"), s_RF_Freq[n][i]);
        m_ctrlRFCommFreq[i].SetWindowText(s);
    }
}

int CRollnumAndCPConfigDialog::Get_RF_Freq_Group( const int f1, const int f2, const int f3 )
{
    for (int i = 0; i < 8; i++) {
        if (f1 == s_RF_Freq[i][0] && f2 == s_RF_Freq[i][1] && f3 == s_RF_Freq[i][2])
            return i;
    }

    return -1;
}

bool CRollnumAndCPConfigDialog::CheckParameters( TEST_OTPWRITE_t &cfg, CONFIG_FILE_CHECK_t &check )
{
    // m_ConfigFileCheck parameters format and range.
    if (cfg.enableOTPWriting > 3)
    {
        check.CP_Config_File1 = false;
        hgzMessageBox(_T("Warning: Value of \"Enable OTP writing\" out of range: %d\r\n"), cfg.enableOTPWriting);
    }

    if (cfg.Program.IsEmpty()) {
        check.CP_Config_File1 = false;
        hgzMessageBox(_T("Warning: Value of \"Firmware file to write\" can not be empty!\r\n"));
    }

    if (cfg.RFSynccode4B.IsEmpty()) {
        check.CP_Config_File2 = false;
        hgzMessageBox(_T("Warning: Value of \"4_bytes_RF_syncode file\" can not be empty!\r\n"));
    }

    if (cfg.RFSynccode3B.IsEmpty()) {
        check.CP_Config_File2 = false;
        hgzMessageBox(_T("Warning: Value of \"3_bytes_RF_syncode file\" can not be empty!\r\n"));
    }

    for (int i = 0; i < 3; i++) {
        if (cfg.freq[i]<2399 || cfg.freq[i]>2529) {
            check.CP_Config_File2 = false;
            hgzMessageBox(_T("Warning: Value of \"RF communication test Frequency %d\" out of range: %d\r\n"), i, cfg.freq[i]);
        }
    }

    // m_ConfigFileCheck rollnum
    if ((_tcsicmp(s_strLineValue[0], _T(""))==0) || 
        (_tcsicmp(s_strLineValue[4], _T(""))==0) || (_tcsicmp(s_strLineValue[5], _T(""))==0) || (_tcsicmp(s_strLineValue[6], _T(""))==0) || 
        (_tcsicmp(s_strLineValue[7], _T(""))==0) || (_tcsicmp(s_strLineValue[8], _T(""))==0) || (_tcsicmp(s_strLineValue[9], _T(""))==0) || (_tcsicmp(s_strLineValue[10], _T(""))==0))
    {
        hgzMessageBox(_T("Warning: Value can not be empty!\r\n"));
    }


    if (cfg.OTPAddrForRollnum >= 0x4000)
    {
        check.CP_Config_File2 = false;
        hgzMessageBox(_T("Warning: Value of \"OTP address to write rollnum\" out of range: %04X\r\n"), cfg.OTPAddrForRollnum);
    }
    if (cfg.rollnumMin > cfg.rollnumMax)
    {
        check.CP_Config_File2 = false;
        hgzMessageBox(_T("Warning: Rollnum range error: [%08X, %08X]\r\n"), cfg.rollnumMin, cfg.rollnumMax);
    }

    if ((cfg.rsTable.rollnum != NULL_NUM) && (cfg.rsTable.rollnum < cfg.rollnumMin || cfg.rollnumMax < cfg.rsTable.rollnum))
    {
        check.CP_Config_File2 = false;
        hgzMessageBox(_T("Warning: Rollnum out of range: %08X is out of [%08X, %08X]\r\n"), cfg.rsTable.rollnum, cfg.rollnumMin, cfg.rollnumMax);
    }

    

    return (check.CP_Config_File1 && check.CP_Config_File2);
}

void CRollnumAndCPConfigDialog::UpdateData_To_Ctrol( void )
{
    TEST_OTPWRITE_t cfg;
    Convert_Parameters( cfg );
    m_ctrlEnableOTPWriting.SetWindowsTextFormat(s_strLineValue[0]);
    m_cmb_Enable_OTP_writing.SetCurSel(cfg.enableOTPWriting);

    m_ctrlFirmwareFileToWrite.SetWindowText(s_strLineValue[1]);
    m_ctrlRFSynccode4BFile.SetWindowText(s_strLineValue[2]);
    m_ctrlRFSynccode3BFile.SetWindowText(s_strLineValue[3]);
    m_ctrlRFCommFreq[0].SetWindowText(s_strLineValue[4]);
    m_ctrlRFCommFreq[1].SetWindowText(s_strLineValue[5]);
    m_ctrlRFCommFreq[2].SetWindowText(s_strLineValue[6]);
    m_ctrl_RFFreqGroup.SetCurSel(Get_RF_Freq_Group(cfg.freq[0], cfg.freq[1], cfg.freq[2]));

    m_ctrlOTPAddrForRollnum.SetWindowText(s_strLineValue[7]);
    m_ctrlRollnumMin.SetWindowText(s_strLineValue[8]);
    m_ctrlRollnumMax.SetWindowText(s_strLineValue[9]);
    m_ctrlNextRollnum.SetWindowText(s_strLineValue[10]);
    m_ctrlPreviousUpdatedRollnum.SetWindowText(s_strLineValue[11]);
}

void CRollnumAndCPConfigDialog::UpdateData_To_Value( void )
{
    CString s;
    m_ctrlEnableOTPWriting.GetWindowText(s);
    _tcscpy(s_strLineValue[0], s);
    m_ctrlFirmwareFileToWrite.GetWindowText(s);
    _tcscpy(s_strLineValue[1], s);
    m_ctrlRFSynccode4BFile.GetWindowText(s);
    _tcscpy(s_strLineValue[2], s);
    m_ctrlRFSynccode3BFile.GetWindowText(s);
    _tcscpy(s_strLineValue[3], s);
    m_ctrlRFCommFreq[0].GetWindowText(s);
    _tcscpy(s_strLineValue[4], s);
    m_ctrlRFCommFreq[1].GetWindowText(s);
    _tcscpy(s_strLineValue[5], s);
    m_ctrlRFCommFreq[2].GetWindowText(s);
    _tcscpy(s_strLineValue[6], s);
    m_ctrlOTPAddrForRollnum.GetWindowText(s);
    _tcscpy(s_strLineValue[7], s);
    m_ctrlRollnumMin.GetWindowText(s);
    _tcscpy(s_strLineValue[8], s);
    m_ctrlRollnumMax.GetWindowText(s);
    _tcscpy(s_strLineValue[9], s);
    m_ctrlNextRollnum.GetWindowText(s);
    _tcscpy(s_strLineValue[10], s);
    m_ctrlPreviousUpdatedRollnum.GetWindowText(s);
    _tcscpy(s_strLineValue[11], s);
}

void CRollnumAndCPConfigDialog::Convert_Parameters( TEST_OTPWRITE_t &cfg )
{
    cfg.enableOTPWriting = _tcsicmp(s_strLineValue[0], _T("")) == 0 ? -1 : _tcstoul(s_strLineValue[0], NULL, 10);
    cfg.Program.Format(_T("%s"), s_strLineValue[1]);

    cfg.RFSynccode4B.Format(_T("%s"), s_strLineValue[2]);
    cfg.RFSynccode3B.Format(_T("%s"), s_strLineValue[3]);

    cfg.freq[0] = _tcstoul(s_strLineValue[4], NULL, 10);
    cfg.freq[1] = _tcstoul(s_strLineValue[5], NULL, 10);
    cfg.freq[2] = _tcstoul(s_strLineValue[6], NULL, 10);

    cfg.OTPAddrForRollnum = _tcstoul(s_strLineValue[7], NULL, 16);
    cfg.rollnumMin = _tcstoul(s_strLineValue[8], NULL, 0);
    cfg.rollnumMax = _tcstoul(s_strLineValue[9], NULL, 0);
    cfg.rsTable.rollnum = _tcstoul(s_strLineValue[10], NULL, 0);
    if (s_strLineValue[10][0] == '0')
    {
        if (s_strLineValue[10][1] == 'x' || s_strLineValue[10][1] == 'X')
            cfg.RadixOfNextRollnumInCfgFile = 16;
        else
            cfg.RadixOfNextRollnumInCfgFile = 8;
    }
    else
        cfg.RadixOfNextRollnumInCfgFile = 10;
    _tprintf(_T("\r\nRadix of \"Next rollnum\" in config file is %d.\r\n\r\n"), cfg.RadixOfNextRollnumInCfgFile);

}

int CRollnumAndCPConfigDialog::CheckRFSyncFile( CString &sfn, int codeBytes, CStatic *progress /*= NULL*/ )
{
    int cnt = 0;
    CStdioFile fin;
    //CFileException mExcept;
    if (!fin.Open(sfn, CFile::modeRead | CFile::shareDenyNone | CFile::typeText) || (fin.GetLength() == 0))
    {
        if (progress)
            progress->SetWindowText(_T("X, 0s"));
        if (fin.GetLength() == 0)
            hgzMessageBox(_T("File is empty: \r\n\r\n%s"), sfn);
        else
            hgzMessageBox(_T("File not found: \r\n\r\n%s"), sfn);
        return 0;
    }

    CHgzString s;
    bool bPass = true;

    fin.SeekToBegin();
    fin.ReadString(s);
    int charsPerLine = s.GetLength();
    fin.SeekToBegin();

    int prog = 0, prog1 = 0;

    __time64_t time1, time2;
    _time64(&time1);
    while (fin.ReadString(s) && bPass)
    {
        cnt++;
        
        if (s.GetLength() != charsPerLine)
        {
            bPass = false;
            hgzMessageBox(_T("Format checking fails: Line lengths are not the same.\n\n%s\n\nLine = %d\nstring = %s"), sfn, cnt, s);
        }
        s.Remove(_T('\n'));
        s.Remove(_T('\r'));
        if (s.GetLength() != codeBytes*2)
        {
            bPass = false;
            hgzMessageBox(_T("Format checking fails: Line lengths are not the same.\n\n%s\n\nLine = %d\nstring = %s"), sfn, cnt, s);
        }
        if (!s.is_xdigit())
        {
            bPass = false;
            hgzMessageBox(_T("Format checking fails: Illegal characters in string.\n\n%s\n\nLine = %d\nstring = %s"), sfn, cnt, s);
        }
        
        if (progress && (cnt%1000 == 0) && prog < (prog1 = fin.GetPosition()*100/fin.GetLength()))
        {
            prog = prog1;
            CString sProg;
            _time64(&time2);
            sProg.Format(_T("%d%%, %ds"), prog, time2-time1);
            progress->SetWindowText(sProg);
        }
    }
    fin.Close();

    if (progress)
    {
        CString sProg;
        _time64(&time2);
        sProg.Format(_T("%s, %ds"), bPass ? _T("√") : _T("X"), time2 - time1); // _T("√") : _T("X")
        progress->SetWindowText(sProg);
    }

    return bPass ? cnt : 0;
}

