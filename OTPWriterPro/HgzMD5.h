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
    CString md5buf(UINT8 * buf, UINT32 byteLen, TCHAR *sDigest = NULL);
    
    // for string
    CString md5str(TCHAR *s, TCHAR *sDigest = NULL, bool bUnicode = false);
    CString md5str(CString &s, CString *sDigest = NULL, bool bUnicode = false);

    // for bin file
    CString md5binseg(CStdioFile &fin,                          int bytes    = 0, CStatic *progress = NULL);
    CString md5bin   (CStdioFile &fin,                                            CStatic *progress = NULL);
    CString md5bin   (CStdioFile &fin, CStdioFile *fout,        int perBytes = 0, CStatic *progress = NULL);
    CString md5bin   (CString    &fin, CString    *fout = NULL, int perBytes = 0, CStatic *progress = NULL);
    CString md5bin   (CString    &fin, CString    *fout,                          CStatic *progress);
    
    // for txt file
    int md5txt(CStdioFile &fin, CStdioFile &fout, bool bUnicode = false, CStatic *progress = NULL);
    int md5txt(CString &fin, CString *fout = NULL, bool bUnicode = false, CStatic *progress = NULL);
    
    // for all file
    CString md5file(CString &fin, CString *fout = NULL, bool bTxtFile = false, int bUnicode_perBytes = 0, CStatic *progress = NULL);

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

private:
    UINT32 digest[4], a, b, c, d;
    UINT32 _x[16];
    //UINT64 originalBitLen;

    void md5init(void);
    void md5core( const UINT8 *x );
    void md5end(UINT8 *begin, UINT32 len, UINT32 originalByteLen);
    
};

