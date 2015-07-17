// RollnumAndCPConfigDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OTPWriterPro.h"
#include "RollnumAndCPConfigDialog.h"
#include "afxdialogex.h"
#include "hgz\\HgzString.h"


// CRollnumAndCPConfigDialog 对话框

const TCHAR CRollnumAndCPConfigDialog::s_strConfigFilePath1[] = _T("\\");
const TCHAR CRollnumAndCPConfigDialog::s_strConfigFilePath2[] = _T("\\SiteInfo\\");
const TCHAR CRollnumAndCPConfigDialog::s_strConfigFileName1[] = _T("CP_Config_File1.txt");
const TCHAR CRollnumAndCPConfigDialog::s_strConfigFileName2[] = _T("CP_Config_File2.txt");

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


IMPLEMENT_DYNAMIC(CRollnumAndCPConfigDialog, CDialogEx)

CRollnumAndCPConfigDialog::CRollnumAndCPConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRollnumAndCPConfigDialog::IDD, pParent)
{

}

CRollnumAndCPConfigDialog::~CRollnumAndCPConfigDialog()
{
}

void CRollnumAndCPConfigDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHECK2, m_ctrlEnableOTPWriting);
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
}


BEGIN_MESSAGE_MAP(CRollnumAndCPConfigDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CRollnumAndCPConfigDialog::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON6, &CRollnumAndCPConfigDialog::OnBnClickedButton_ParseConfigFile)
    ON_BN_CLICKED(IDC_BUTTON7, &CRollnumAndCPConfigDialog::OnBnClickedButton_UpdateConfigFile)
    ON_BN_CLICKED(IDC_BUTTON3, &CRollnumAndCPConfigDialog::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CRollnumAndCPConfigDialog::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &CRollnumAndCPConfigDialog::OnBnClickedButton5)
    ON_BN_CLICKED(IDOK, &CRollnumAndCPConfigDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CRollnumAndCPConfigDialog 消息处理程序


void CRollnumAndCPConfigDialog::OnBnClickedButton1()
{
    CFolderPickerDialog dlg;
    if (dlg.DoModal() != IDOK)
    {
        return;
    }

    m_CPConfigFilePath = dlg.GetPathName();
    TCHAR stemp[1024];
    GetCurrentDirectory(1024, stemp);
    CString s = stemp;
    u32 n = m_CPConfigFilePath.Find(s + _T('\\'));
    if (n != -1)
        AfxGetApp()->WriteProfileString(_T("Settings"), _T("CPConfigFilePath"), m_CPConfigFilePath.Mid(n+s.GetLength()));
    else
        AfxGetApp()->WriteProfileString(_T("Settings"), _T("CPConfigFilePath"), m_CPConfigFilePath);
    
    SetDlgItemText(IDC_EDIT_CPConfigFilePath, m_CPConfigFilePath);
}


void CRollnumAndCPConfigDialog::OnBnClickedButton_ParseConfigFile()
{
    ConfigFile_Parser();
    UpdateDisplay();
}


void CRollnumAndCPConfigDialog::OnBnClickedButton_UpdateConfigFile()
{
    // Check if all items ready


    // open or create config file, write contents to config file, close config file
    CStdioFile CP_Config_File1, CP_Config_File2;

    if (CP_Config_File1.Open(m_CPConfigFilePath+s_strConfigFilePath1+s_strConfigFileName1, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate))
    {
        _tprintf(_T("\r\nWrite into Chip-Probing Config file: %s\n"), s_strConfigFileName1);
        SetDlgItemText(IDC_CPCONFIGFILE1_CHECK, _T("√"));

        // clear file
        CP_Config_File1.SetLength(0);
        
        // Write into file
        CString s, s1;
        s.Format(_T("%s: %d\n"), s_strLinePrefix[0], m_ctrlEnableOTPWriting.GetCheck());
        CP_Config_File1.WriteString(s);

        s.Format(_T("%s: %s\n"), s_strLinePrefix[1], (m_ctrlFirmwareFileToWrite.GetWindowText(s1), s1));
        CP_Config_File1.WriteString(s);
        
        CP_Config_File1.Close();
    }
    else
    {
        SetDlgItemText(IDC_CPCONFIGFILE1_CHECK, _T("×"));
        AfxMessageBox(_T("System fault: Failed to open/create file: CP_Config_File1.txt.\n"));
    }

    if (CP_Config_File2.Open(m_CPConfigFilePath+s_strConfigFilePath2+s_strConfigFileName2, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate))
    {
        _tprintf(_T("\r\nWrite into Chip-Probing Config file: %s\r\n"), s_strConfigFileName2);
        SetDlgItemText(IDC_CPCONFIGFILE2_CHECK, _T("√"));

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
        CP_Config_File2.WriteString((s.Format(_T("%s: %s\n"), s_strLinePrefix[11], (m_ctrlNextRollnum.GetWindowText(s1), s1)), s));

        CP_Config_File2.Close();
    }
    else
    {
        SetDlgItemText(IDC_CPCONFIGFILE2_CHECK, _T("×"));
        AfxMessageBox(_T("System fault: Failed to open file: CP_Config_File2.txt.\r\n"));
    }

    

}


BOOL CRollnumAndCPConfigDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_vTest_OTPWrite.bEnableOTPWriting = false;

    SetDlgItemText(IDC_EDIT_CPConfigFilePath, m_CPConfigFilePath);
    ConfigFile_Parser();
    UpdateDisplay();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CRollnumAndCPConfigDialog::ConfigFile_Parser(void)
{
    for (int i = 0; i < CFG_PARAM_NUM; i++)
        _tcscpy(s_strLineValue[i], _T(""));

    
    CStdioFile CP_Config_File1, CP_Config_File2;
    
    try
    {
        CFileException mExcept;
        if (CP_Config_File1.Open(m_CPConfigFilePath+s_strConfigFilePath1+s_strConfigFileName1, CFile::modeRead | CFile::shareDenyNone, &mExcept))
        {
            // parse CP_Config_File1:
            m_ConfigFileCheck.CP_Config_File1 = true;
            _tprintf(_T("\r\nRead Chip-Probing Config file: %s\r\n"), s_strConfigFileName1);
            
            CString s;
            while (CP_Config_File1.ReadString(s))
                ConfigFile_LineParser(s, CP_Config_File1);

            CP_Config_File1.Close();
        }
        else
        {
            m_ConfigFileCheck.CP_Config_File1 = false;
            AfxMessageBox(_T("System fault: Failed to open file: CP_Config_File1.txt.\r\n"));
        }

        if (CP_Config_File2.Open(m_CPConfigFilePath+s_strConfigFilePath2+s_strConfigFileName2, CFile::modeRead | CFile::shareDenyNone, &mExcept))
        {
            m_ConfigFileCheck.CP_Config_File2 = true;
            _tprintf(_T("\r\nRead Chip-Probing Config file: %s\r\n"), s_strConfigFileName2);
            
            CString s;
            while (CP_Config_File2.ReadString(s))
                ConfigFile_LineParser(s, CP_Config_File2);

            CP_Config_File2.Close();
        }
        else
        {
            m_ConfigFileCheck.CP_Config_File2 = false;
            AfxMessageBox(_T("System fault: Failed to open file: CP_Config_File2.txt.\r\n"));
        }

    }
    catch (CFileException* e)
    {
        AfxMessageBox(_T("System fault: Failed to open file.\r\n"));
        return;
    }
   
    // Is there any line not existing in config file?
    for (int i = 0; i < CFG_PARAM_NUM-1; i++)
    {
        if (_tcscmp(_T(""), s_strLineValue[i]) == 0)
            _tprintf(_T("System fault: ConfigFile_Parser() -- Missing parameter \"%s\" in the configuration file.\r\n"), s_strLinePrefix[i]);
    }


    //---------------------
    m_vTest_OTPWrite.bEnableOTPWriting = _tcstoul(s_strLineValue[0], NULL, 10);
    m_vTest_OTPWrite.Config.Format(_T("%s"), s_strLineValue[1]);

    m_vTest_OTPWrite.RFSynccode4B.Format(_T("%s"), s_strLineValue[2]);
    m_vTest_OTPWrite.RFSynccode3B.Format(_T("%s"), s_strLineValue[3]);
    
    m_vTest_OTPWrite.freq[0] = _tcstoul(s_strLineValue[4], NULL, 10);
    m_vTest_OTPWrite.freq[1] = _tcstoul(s_strLineValue[5], NULL, 10);
    m_vTest_OTPWrite.freq[2] = _tcstoul(s_strLineValue[6], NULL, 10);

    m_vTest_OTPWrite.OTPAddrForRollnum = _tcstoul(s_strLineValue[7], NULL, 16);
    m_vTest_OTPWrite.rollnumMin = _tcstoul(s_strLineValue[8], NULL, 0);
    m_vTest_OTPWrite.rollnumMax = _tcstoul(s_strLineValue[9], NULL, 0);
    m_vTest_OTPWrite.rsTable.rollnum = _tcstoul(s_strLineValue[10], NULL, 0);
    if (s_strLineValue[10][0] == '0')
    {
        if (s_strLineValue[10][1] == 'x' || s_strLineValue[10][1] == 'X')
            m_RadixOfNextRollnumInCfgFile = 16;
        else
            m_RadixOfNextRollnumInCfgFile = 8;
    }
    else
        m_RadixOfNextRollnumInCfgFile = 10;
    _tprintf(_T("\r\nRadix of \"Next rollnum\" in config file is %d.\r\n\r\n"), m_RadixOfNextRollnumInCfgFile);

    if (m_vTest_OTPWrite.rollnumMin > m_vTest_OTPWrite.rollnumMax)
        _tprintf(_T("System fault: Rollnum range error: [%08X, %08X]\r\n"), m_vTest_OTPWrite.rollnumMin, m_vTest_OTPWrite.rollnumMax);

    if ((m_vTest_OTPWrite.rsTable.rollnum != NULL_NUM) && (m_vTest_OTPWrite.rsTable.rollnum < m_vTest_OTPWrite.rollnumMin || m_vTest_OTPWrite.rollnumMax < m_vTest_OTPWrite.rsTable.rollnum))
        _tprintf(_T("System fault: Rollnum out of range: %08X is out of [%08X, %08X]\r\n"), m_vTest_OTPWrite.rsTable.rollnum, m_vTest_OTPWrite.rollnumMin, m_vTest_OTPWrite.rollnumMax);
    //---------------------------------
    
    


    //

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
            CString serr;
            serr.Format(_T("System fault: Syntex error - Can't find ':' in line %s!\r\n"), s_strLinePrefix[i]);
            AfxMessageBox(serr);
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
    // check
    SetDlgItemText(IDC_CPCONFIGFILE1_CHECK, m_ConfigFileCheck.CP_Config_File1 ? _T("√") : _T("×"));
    SetDlgItemText(IDC_CPCONFIGFILE2_CHECK, m_ConfigFileCheck.CP_Config_File2 ? _T("√") : _T("×"));

    //
    m_ctrlEnableOTPWriting.SetCheck(m_vTest_OTPWrite.bEnableOTPWriting);
    m_ctrlFirmwareFileToWrite.SetWindowText(s_strLineValue[1]);
    m_ctrlRFSynccode4BFile.SetWindowText(s_strLineValue[2]);
    m_ctrlRFSynccode3BFile.SetWindowText(s_strLineValue[3]);
    m_ctrlRFCommFreq[0].SetWindowText(s_strLineValue[4]);
    m_ctrlRFCommFreq[1].SetWindowText(s_strLineValue[5]);
    m_ctrlRFCommFreq[2].SetWindowText(s_strLineValue[6]);
    m_ctrlOTPAddrForRollnum.SetWindowText(s_strLineValue[7]);
    m_ctrlRollnumMin.SetWindowText(s_strLineValue[8]);
    m_ctrlRollnumMax.SetWindowText(s_strLineValue[9]);
    m_ctrlNextRollnum.SetWindowText(s_strLineValue[10]);
    m_ctrlPreviousUpdatedRollnum.SetWindowText(s_strLineValue[11]);

    //m_ctrlEnableOTPWriting.FlashWindowEx(FLASHW_ALL, 300, 1000);
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
    m_vTest_OTPWrite.rsTable.synccode4b = m_ConfigFileCheck.CP_Config_File2 ? SD_GetRFSynccode(m_CPConfigFilePath+s_strConfigFilePath2+m_vTest_OTPWrite.RFSynccode4B, m_vTest_OTPWrite.rsTable.rollnum) : 0;
    m_vTest_OTPWrite.rsTable.synccode3b = m_ConfigFileCheck.CP_Config_File2 ? SD_GetRFSynccode(m_CPConfigFilePath+s_strConfigFilePath2+m_vTest_OTPWrite.RFSynccode3B, m_vTest_OTPWrite.rsTable.rollnum) : 0;
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
    printf(m_RadixOfNextRollnumInCfgFile==16 ? "Write rollnum to SD: 0x%08X\r\n" : (m_RadixOfNextRollnumInCfgFile==10 ? "Write rollnum to SD: %d\r\n" : "Write rollnum to SD: 0%011o\r\n"), m_vTest_OTPWrite.rsTable.rollnum);

    CStdioFile CP_Config_File2;
    if (CP_Config_File2.Open(m_CPConfigFilePath+s_strConfigFilePath2+s_strConfigFileName2, CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeNoTruncate))
    {
        _tprintf(_T("\r\nWrite into Chip-Probing Config file: %s\r\n"), s_strConfigFileName2);

        CHgzString s;
        
        CP_Config_File2.Seek(m_vTest_OTPWrite.offsetOfStartRollnumLineInConfigFile, CFile::begin);
        s.Format(m_RadixOfNextRollnumInCfgFile==16 ? _T("%s: 0x%08X\n") : (m_RadixOfNextRollnumInCfgFile==10 ? _T("%s: %d\n") : _T("%s: 0%011o\n")), s_strLinePrefix[CFG_PARAM_NUM-2], m_vTest_OTPWrite.rsTable.rollnum);
        CP_Config_File2.WriteString(s);
        s.Format(m_RadixOfNextRollnumInCfgFile==16 ? _T("%s: 0x%08X\n") : (m_RadixOfNextRollnumInCfgFile==10 ? _T("%s: %d\n") : _T("%s: 0%011o\n")), s_strLinePrefix[CFG_PARAM_NUM-1], m_vTest_OTPWrite.rsTable.rollnum);
        CP_Config_File2.WriteString(s);

        CP_Config_File2.Close();
        return true;
    }
    else
    {
        _tprintf(_T("System fault: Failed to open file: CP_Config_File2.txt.\r\n"));
        AfxMessageBox(_T("System fault: Failed to open file: CP_Config_File2.txt.\r\n"));
        return false;
    }


}
