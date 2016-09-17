#pragma once

#include <string>
#include "hgz\hgz.h"
#include "HgzPath.h"
#include <atlpath.h>
using std::string;
using std::wstring;


class CHgzMD5
{
public:
    CHgzMD5(void);
    ~CHgzMD5(void);

    // for buffer
    CString md5buf(const UINT8 * buf, UINT32 byteLen, TCHAR *sDigest = NULL);
    
    // for string
    CString md5str(const TCHAR *s, TCHAR *sDigest = NULL, bool bUnicode = false);
    CString md5str(CString &s, CString *sDigest = NULL, bool bUnicode = false);

    // for bin file
    /**
        @return CString format of computed md5 when succeed, otherwise empty string.
    */
    CString md5binseg(CStdioFile &fin,                          int bytes    = 0, CStatic *progress = NULL);
    /**
        @return CString format of count of passed lines when succeed, otherwise return -1.
    */
    int md5bin       (CStdioFile &fin, CStdioFile *fout,        int perBytes = 0, CStatic *progress = NULL, bool bVerify = false);
    int md5bin       (CString    &fin, CString    *fout = NULL, int perBytes = 0, CStatic *progress = NULL, bool bVerify = false);
    
    CString md5bin   (CStdioFile &fin,                                            CStatic *progress = NULL);
    int md5bin       (CString    &fin, CString    *fout,                          CStatic *progress);
    
    // for txt file
    /**
        @return Count of passed lines when succeed, otherwise return -1.
    */
    int md5txt(CStdioFile &fin, CStdioFile *fout, bool bUnicode = false, CStatic *progress = NULL, bool bVerify = false);
    int md5txt(CString &fin, CString *fout = NULL, bool bUnicode = false, CStatic *progress = NULL, bool bVerify = false);

    //-----------------------------------------
    /** @prief Get md5 string from md5 file at designated line.
        @param fp is the designated md5 file.
        @param nLine designated line number.
        - -1 : read md5 string from current file position.
        @param chars_per_line characters per line of the md5 file.
        @param md5_str used to strore md5 string for output.
        @return pointer to the wanted md5 string.
    */
    char* file_get_md5(FIL *fp, int nLine, int chars_per_line, char *md5_str);
    
    bool assert_equal(const char *val, bool isprint = false);
    bool assert_equal(string& val, bool isprint = false);
    bool assert_equal(u8 *val, bool isprint = false);
    bool assert_equal(u32 *val, bool isprint = false);
    
    /**
     * @prief Compare md5 value of current CMD5 object with reference md5 string in file at nLine line.
     * @param fp : pointer of reference md5 file.
     * @param nLine: line number (numbering from 0, not 1) of reference md5 string in file @fp.
     * - -1 : read md5 string from current file position.
     * @param chars_per_line: characters per line of fp file.
     * @param isprint: whether print the comparison result or not.
     * @return compare result.
     * - true if equal.
     * - false otherwise.
    */
    bool assert_equal(FIL *fp, int nLine, int chars_per_line, bool isprint = false);
    
    /**
     * @prief Compute md5 based on segmented bytes and compare it with corresponding line string in md5 file. For entire file. 
     * @param fbin  : pointer of binary file to read data to compute md5.
     * @param fhash : pointer of reference md5 file.
     * @param bytes : byte count of data to read out from file @fbin and then to compute md5.
     * - 0 : means byte count equals file length.
     * - positive integer : designated byte count.
     * @param isprint: whether print the comparison result or not.
     * @return compare result.
     * - true if equal.
     * - false otherwise.
    */
    bool assert_equal(FIL *fbin, FIL *fhash, int bytes = 0, bool isprint = false);
    
    /**
     * @prief Compute input string's md5 and compare it with the reference md5 in corresponding md5 file.
     * @param str: string to compute md5 on.
     * @param fp: file of reference md5.
     * @param nLine: line number (numbering from 0, not 1) of reference md5 in file fp.
     * - -1 : read md5 string from current file position.
     * @param chars_per_line: characters per line of fp file.
     * @param isprint: whether print the comparison result or not.
     * @return compare result.
     * - true if equal.
     * - false otherwise.
    */
    bool IsCorrect(const char *str, FIL *fp, int nLine, int chars_per_line, bool isprint = false);
    
    /**
     * @prief Compute input buffer's md5 and compare it with the reference md5 in corresponding md5 file.
     * @param buf: buffer to compute md5 on.
     * @param buf_size: buffer length to compute md5.
     * @param fp: file of reference md5.
     * @param nLine: line number (numbering from 0, not 1) of reference md5 in file fp.
     * - -1 : read md5 string from current file position.
     * @param chars_per_line: characters per line of fp file.
     * @param isprint: whether print the comparison result or not.
     * @return compare result.
     * - true if equal.
     * - false otherwise.
    */
    bool IsCorrect(const u8 *buf, const int buf_size, FIL *fp, int nLine, int chars_per_line, bool isprint = false);
    
    //--------------------------------------------------------------
    
    
    // to strings
    TCHAR* to_string(TCHAR *str)
    {
        for(int i = 0; i < 16; i++)
        {
            _stprintf(str+2*i, _T("%02x"), ((UINT8 *)&digest)[i]);
        }
        str[32] = _T('\0');
        return str;
    }

    char* to_string(char *str)
    {
        for(int i = 0; i < 16; i++)
        {
            sprintf(str+2*i, "%02x", ((UINT8 *)&digest)[i]);
        }
        str[32] = '\0';
        return str;
    }

    tstring& to_string(tstring &s)
    {
        TCHAR x[33];
        to_string(x);
        s = x;
        return s;
    }

	tstring to_string()
	{
		TCHAR x[33];
		to_string(x);
		tstring s = x;
		return s;
	}

    string to_string(const u8 *val, char *str = NULL)
    {
        if (str)
        {
            for(int i = 0; i < 16; i++)
            {
                sprintf(str+2*i, "%02x", val[i]);
            }
            str[32] = 0;
            return string(str, 32);
        }

        char x[33];
        for(int i = 0; i < 16; i++)
        {
            sprintf(x+2*i, "%02x", val[i]);
        }
        x[32] = 0;

        return string(x, 32);
    }

	CString& to_CString(CString &s)
	{
		TCHAR x[33];
		to_string(x);
		s = x;
		return s;
	}
	CString to_CString()
	{
		TCHAR x[33];
		to_string(x);
		CString s = x;
		return s;
	}

    void GetValue(UINT32 *val)
    {
        val[0] = digest[0];
        val[1] = digest[1];
        val[2] = digest[2];
        val[3] = digest[3];
    }

    void print(void)
    {
        tstring s = to_string();
        printf("md5 = %s", s.c_str());
    }

    void println(void) { print(); printf("\r\n"); }

private:
    UINT32 digest[4], a, b, c, d;
    UINT32 _x[16];
    //UINT64 originalBitLen;

    void md5init(void);
    void md5core( const UINT8 *x );
    void md5end(UINT8 *begin, UINT32 len, UINT32 originalByteLen);
    
};

