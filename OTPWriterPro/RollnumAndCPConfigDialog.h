#pragma once
#include "afxwin.h"
#include "hgz\HgzComboBox.h"
#include "HgzMD5.h"
#include "afxcmn.h"


// CRollnumAndCPConfigDialog 对话框

class CRollnumAndCPConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CRollnumAndCPConfigDialog)

public:
	CRollnumAndCPConfigDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRollnumAndCPConfigDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_ROLLNUM_AND_CPCONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    ATL::CPath m_CPConfigFilePath;

    #define u8 UINT8
    #define u16 UINT16
    #define u32 UINT32
    #define FIL CStdioFile

    static const u32 NULL_NUM = 0xFFFFFFFFUL;
    enum {
        CFG_PARAM_NUM             = 12,
    };

    typedef struct
    {
        u32 rollnum;
        u32 synccode4b;
        u32 synccode3b;
        u32 reserved;
    } ROLLNUM_RFSynccode_TABLE_t;

    u32 m_RadixOfNextRollnumInCfgFile;

    typedef struct 
    {
        u32 enableOTPWriting;	

        CString Config;
        CString Program;
        CString RFSynccode4B;
        CString RFSynccode3B;

        u32 freq[3];

        u32 offsetOfStartRollnumLineInConfigFile;    
        u16 OTPAddrForRollnum;
        u32 rollnumMin;
        u32 rollnumMax;
        ROLLNUM_RFSynccode_TABLE_t rsTable;
        u32 nextSFAddrForRollnum;
        bool SDRollnumUpdated;
    } TEST_OTPWRITE_t;
    TEST_OTPWRITE_t m_vTest_OTPWrite;

    typedef struct  
    {
        bool CP_Config_File1;
        bool EnableOTPWriting;
        bool ProgramFileToWrite;

        bool CP_Config_File2;
        bool RFSynccode4B;
        bool RFSynccode3B;
        bool freq[3];
        bool OTPAddrToWriteRollnum;
        bool rollnumMin;
        bool rollnumMax;
        bool NextRollnum;
        bool PreviousUpdatedRollnum;

    } CONFIG_FILE_CHECK_t;
    CONFIG_FILE_CHECK_t m_ConfigFileCheck;

    /*static const TCHAR s_strConfigFilePath1[];
    static const TCHAR s_strConfigFilePath2[];
    static const TCHAR s_strConfigFileName1[];
    static const TCHAR s_strConfigFileName2[];*/
    static const ATL::CPath s_strConfigFilePath1;
    static const ATL::CPath s_strConfigFilePath2;
    static const ATL::CPath s_strConfigFileName1;
    static const ATL::CPath s_strConfigFileName2;
    static const TCHAR s_strLinePrefix[][50];
    TCHAR s_strLineValue[CFG_PARAM_NUM][50];

public:
    afx_msg void OnBnClickedButton_SelectConfigFilePath();
    afx_msg void OnBnClickedButton_ParseConfigFile();
    afx_msg void OnBnClickedButton_UpdateConfigFile();

    virtual BOOL OnInitDialog();

    void UpdateDisplay();

    CHgzComboBox m_ctrlEnableOTPWriting;
    CEdit m_ctrlFirmwareFileToWrite;
    afx_msg void OnBnClickedButton3();
    void ConfigFile_Parser(void);
    void ConfigFile_LineParser(CString& s, CStdioFile& f);
    CEdit m_ctrlRFSynccode4BFile;
    CEdit m_ctrlRFSynccode3BFile;
    CEdit m_ctrlRFCommFreq[3];
    CEdit m_ctrlOTPAddrForRollnum;
    CEdit m_ctrlRollnumMin;
    CEdit m_ctrlRollnumMax;
    CEdit m_ctrlNextRollnum;
    CEdit m_ctrlPreviousUpdatedRollnum;
    afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedButton5();
    void SD_FillRollnumRFSynccode(void);
    u32 SD_GetRFSynccode(CString& fileName, u32 rollnum);
    afx_msg void OnBnClickedOk();
    bool IsRollnumValid(void);
    bool IsRFSyncodeValid(void);
    bool WriteRollnumToConfigFile(void);
    afx_msg void OnBnClickedButton_GenerateVerificationFiles();
    CButton m_ctrl_EnMD5Gen_CP_Config_File1;
    CButton m_ctrl_EnMD5Gen_CP_Config_File2;
    CButton m_ctrl_EnMD5Gen_FirmwareToWrite;
    CButton m_ctrl_EnMD5Gen_RFSynFile4B;
    CButton m_ctrl_EnMD5Gen_RFSynFile3B;
};
