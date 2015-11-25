#include "StdAfx.h"
#include "HgzMD5.h"
#include ".\hgz\hgz.h"
#include <time.h>
#include "HgzTimer.h"

CHgzMD5::CHgzMD5(void)
{
    md5init();
}


CHgzMD5::~CHgzMD5(void)
{
}

// md5_buffer
CString CHgzMD5::md5buf( UINT8 * buf, UINT32 byteLen, TCHAR *sDigest /*= NULL*/ )
{
    UINT8 *begin = buf;
    UINT8 *end = begin + byteLen;

    md5init();

    for (; begin+(512/8) <= end; begin+=(512/8))
    {
        md5core(begin);
    }

    md5end(begin, end-begin, byteLen);

    if (sDigest)
        to_string(sDigest);

    return to_CString();
}

// md5_string
CString CHgzMD5::md5str( TCHAR *s, TCHAR *sDigest /*= NULL*/, bool bUnicode /*= false*/ )
{
    CString su;
    
    if (bUnicode)
        su = md5buf((UINT8 *)CStringW(s).GetBuffer(), _tcslen(s)*2, sDigest);
    else
        su = md5buf((UINT8 *)CStringA(s).GetBuffer(), _tcslen(s)*1, sDigest);

    return su;
}

CString CHgzMD5::md5str( CString &s, CString *sDigest /*= NULL*/, bool bUnicode /*= false*/ )
{
    if (sDigest)
    {
        if (bUnicode)
            return *sDigest = md5buf((UINT8*)((CStringW)s).GetBuffer(), s.GetLength()*2);
        else
            return *sDigest = md5buf((UINT8*)((CStringA)s).GetBuffer(), s.GetLength()*1);
    }
    else
    {
        if (bUnicode)
            return md5buf((UINT8*)((CStringW)s).GetBuffer(), s.GetLength()*2);
        else
            return md5buf((UINT8*)((CStringA)s).GetBuffer(), s.GetLength()*1);
    }
}

// md5_bin_file
CString CHgzMD5::md5binseg( CStdioFile &fin, int bytes /*= 0*/, CStatic *progress /*= NULL*/ )
{
    UINT32 byteLen = fin.GetLength() - fin.GetPosition();
    if (bytes < byteLen)
        byteLen = bytes;
    if (bytes == 0)
    {
        fin.SeekToBegin();
        byteLen = fin.GetLength();
    }

    int perOpSize = 1024*2;
    if (byteLen<perOpSize)
        perOpSize = byteLen;

    UINT8 *buf = new UINT8[perOpSize];
    
    if (progress)
        progress->SetWindowText(_T("0%"));

    UINT32 cnt = 0;
    CHgzTimer timer;
    int prog = 0;
    int i = 0;
    md5init();
    while(cnt < byteLen)
    {
        UINT32 byteRead = 0;
        if (perOpSize > byteLen-cnt)
            byteRead = fin.Read(buf, byteLen-cnt);
        else
            byteRead = fin.Read(buf, perOpSize);
        cnt += byteRead;

        UINT8 *begin = buf;
        UINT8 *end = begin + byteRead;

        for (; begin+(512/8) <= end; begin+=(512/8))
        {
            md5core(begin);
        }

        if (cnt == byteLen)
        {
            md5end(begin, end-begin, byteLen);
            break;
        }

        if ((++i%10 == 0) && prog < fin.GetPosition()*100/fin.GetLength())
        {
            prog = fin.GetPosition()*100/fin.GetLength();
            CString sProg;
            sProg.Format(_T("%d%%, %ds"), prog, timer.GetSpan());
            if (progress)
                progress->SetWindowText(sProg);
        }
    }

    if (progress && (fin.GetPosition()==fin.GetLength()))
    {
        CString sProg;
        sProg.Format(_T("100%%, %ds"), timer.GetSpan());
        progress->SetWindowText(sProg);
    }

    delete [] buf;

    return to_CString();

}

CString CHgzMD5::md5bin( CStdioFile &fin, CStatic *progress )
{
    return md5binseg(fin, 0, progress);
}

CString CHgzMD5::md5bin( CStdioFile &fin, CStdioFile *fout, int perBytes /*= 0*/, CStatic *progress /*= NULL*/ )
{
    if (!fout)
        return md5binseg(fin, 0, progress)+_T("\n");

    fin.SeekToBegin();
    fout->SeekToEnd();

    int i = 0;
    while(fin.GetPosition() != fin.GetLength())
    {
        i++;
        fout->WriteString(md5binseg(fin, perBytes, progress)+_T("\n"));
    }

    CString si;
    si.Format(_T("%d"), i);
    return si;
}

CString CHgzMD5::md5bin( CString &fin, CString *fout /*= NULL*/, int perBytes /*= 0*/, CStatic *progress /*= NULL*/ )
{
    CHgzPath inf(fin), outf;
    if (fout)
        outf = CHgzPath(fout);
    else
    {
        outf = inf;
        outf.ChangeFileExt(_T(".md5"));
    }
    outf.CreateDirectory();

    CStdioFile mFile, mFileMD5;
    //CFileException mExcept;

    mFile.Open(fin, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
    mFileMD5.Open(outf.GetFullPath(), CFile::modeCreate /*| CFile::modeNoTruncate*/ | CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeText);
    mFileMD5.SetLength(0);
    CString cnt = md5bin(mFile, &mFileMD5, perBytes, progress);
    mFile.Close();
    mFileMD5.Close();

    return cnt;
}

CString CHgzMD5::md5bin( CString & fin, CString * fout, CStatic * progress )
{
    return md5bin(fin, fout, 0, progress);
}


// md5_txt_file
int CHgzMD5::md5txt( CStdioFile &fin, CStdioFile &fout, bool bUnicode /*= false*/, CStatic *progress /*= NULL*/ )
{
    // As to txt file, md5 each line of its Ascii string and store it line to line.

    if (fin.GetLength() == 0)
        return 0;

    int cnt = 0;
    CString s;

    fin.SeekToBegin();
    fout.SeekToEnd();

    int prog = 0, prog1 = 0;

    __time64_t time1, time2;
    _time64(&time1);
    while (fin.ReadString(s))
    {
        fout.WriteString(md5str(s)+_T("\n"));
        cnt++;

        if ((cnt%1000 == 0) && prog < (prog1 = fin.GetPosition()*100/fin.GetLength()))
        {
            prog = prog1;
            CString sProg;
            //sProg.Format(_T("%s: %d%%"), fin.GetFileName(), prog);
            _time64(&time2);
            sProg.Format(_T("%d%%, %ds"), prog, time2-time1);
            if (progress)
                progress->SetWindowText(sProg);
        }
    }
    if (progress)
    {
        CString sProg;
        //progress->SetWindowText(fin.GetFileName() + _T(": 100%"));

        _time64(&time2);
        //s.Format(_T("%s    Elapsed time is %d seconds."), fin.GetFileName() + _T(": 100%."), time2 - time1);
        sProg.Format(_T("100%%, %ds"), time2 - time1);
        progress->SetWindowText(sProg);
    }

    return cnt;
}

int CHgzMD5::md5txt( CString &fin, CString *fout /*= NULL*/, bool bUnicode /*= false*/, CStatic *progress /*= NULL*/ )
{
    int cnt = 0;

    CHgzPath inf(fin), outf, tmppath;
    if (fout)
        outf = CHgzPath(fout);
    else
    {
        outf = inf;
        outf.ChangeFileExt(_T(".md5"));
    }
    outf.CreateDirectory();

    CStdioFile mFile, mFileMD5;
    //CFileException mExcept;

    mFile.Open(fin, CFile::modeRead | CFile::shareDenyNone | CFile::typeText);
    mFileMD5.Open(outf.GetFullPath(), CFile::modeCreate /*| CFile::modeNoTruncate*/ | CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeText);
    mFileMD5.SetLength(0);
    cnt = md5txt(mFile, mFileMD5, bUnicode, progress);
    mFile.Close();
    mFileMD5.Close();

    return cnt;
}

// md5_all_file
CString CHgzMD5::md5file( CString &fin, CString *fout /*= NULL*/, bool bTxtFile /*= false*/, int bUnicode_perBytes /*= 0*/, CStatic *progress /*= NULL*/ )
{
    if (!bTxtFile)
        return md5bin(fin, fout, bUnicode_perBytes, progress);
    else // As to txt file, md5 each line of its Ascii string and store it line to line.
    {
        CString sLineCount;
        sLineCount.Format(_T("%d"), md5txt(fin, fout, bUnicode_perBytes, progress));
        return sLineCount; // CString format of line count.
    }
}

//
// Core md5 functions, private for internal call.
////////////////////////////////////////////////////
void CHgzMD5::md5init( void )
{
    // 初始化链接变量
    digest[0] = 0x67452301;
    digest[1] = 0xefcdab89;
    digest[2] = 0x98badcfe;
    digest[3] = 0x10325476;

}

void CHgzMD5::md5core( const UINT8 *x )
{
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define RL(x, y) (((x) << (y)) | ((x) >> (32 - (y)))) //x 循环左移 y 位，注意是循环左移，不是左移。

    /*
    假设Mj表示消息的第j个子分组（从0到15），常数ti是4294967296*abs( sin(i) ）的整数部分，i 取值从1到64，单位是弧度。（4294967296=2**32）
    */
#define FF(a, b, c, d, Mj, s, ti) a = b + (RL((a + F(b,c,d) + Mj + ti),s))
#define GG(a, b, c, d, Mj, s, ti) a = b + (RL((a + G(b,c,d) + Mj + ti),s))
#define HH(a, b, c, d, Mj, s, ti) a = b + (RL((a + H(b,c,d) + Mj + ti),s))
#define II(a, b, c, d, Mj, s, ti) a = b + (RL((a + I(b,c,d) + Mj + ti),s))


    /////////////////////////////////////////////
    const UINT32 *X = (const UINT32 *)x;

    // correct byte order
    if(hgzIsBigEndian())
    {
        for (int i = 0; i < 512/8; i++)
        {
            hgzRevertByteOrder((UINT8 *)(_x+i), x+i*4, 4);
        }
        X = _x;
    }

    //MD5核心算法,供64轮

    a = digest[0];
    b = digest[1];
    c = digest[2];
    d = digest[3];

    /**//* Round 1 */
    FF (a, b, c, d, X[ 0],  7, 0xd76aa478); /**//*  1 */
    FF (d, a, b, c, X[ 1], 12, 0xe8c7b756); /**//*  2 */
    FF (c, d, a, b, X[ 2], 17, 0x242070db); /**//*  3 */
    FF (b, c, d, a, X[ 3], 22, 0xc1bdceee); /**//*  4 */
    FF (a, b, c, d, X[ 4],  7, 0xf57c0faf); /**//*  5 */
    FF (d, a, b, c, X[ 5], 12, 0x4787c62a); /**//*  6 */
    FF (c, d, a, b, X[ 6], 17, 0xa8304613); /**//*  7 */
    FF (b, c, d, a, X[ 7], 22, 0xfd469501); /**//*  8 */
    FF (a, b, c, d, X[ 8],  7, 0x698098d8); /**//*  9 */
    FF (d, a, b, c, X[ 9], 12, 0x8b44f7af); /**//* 10 */
    FF (c, d, a, b, X[10], 17, 0xffff5bb1); /**//* 11 */
    FF (b, c, d, a, X[11], 22, 0x895cd7be); /**//* 12 */
    FF (a, b, c, d, X[12],  7, 0x6b901122); /**//* 13 */
    FF (d, a, b, c, X[13], 12, 0xfd987193); /**//* 14 */
    FF (c, d, a, b, X[14], 17, 0xa679438e); /**//* 15 */
    FF (b, c, d, a, X[15], 22, 0x49b40821); /**//* 16 */

    /**//* Round 2 */
    GG (a, b, c, d, X[ 1],  5, 0xf61e2562); /**//* 17 */
    GG (d, a, b, c, X[ 6],  9, 0xc040b340); /**//* 18 */
    GG (c, d, a, b, X[11], 14, 0x265e5a51); /**//* 19 */
    GG (b, c, d, a, X[ 0], 20, 0xe9b6c7aa); /**//* 20 */
    GG (a, b, c, d, X[ 5],  5, 0xd62f105d); /**//* 21 */
    GG (d, a, b, c, X[10],  9, 0x02441453); /**//* 22 */
    GG (c, d, a, b, X[15], 14, 0xd8a1e681); /**//* 23 */
    GG (b, c, d, a, X[ 4], 20, 0xe7d3fbc8); /**//* 24 */
    GG (a, b, c, d, X[ 9],  5, 0x21e1cde6); /**//* 25 */
    GG (d, a, b, c, X[14],  9, 0xc33707d6); /**//* 26 */
    GG (c, d, a, b, X[ 3], 14, 0xf4d50d87); /**//* 27 */
    GG (b, c, d, a, X[ 8], 20, 0x455a14ed); /**//* 28 */
    GG (a, b, c, d, X[13],  5, 0xa9e3e905); /**//* 29 */
    GG (d, a, b, c, X[ 2],  9, 0xfcefa3f8); /**//* 30 */
    GG (c, d, a, b, X[ 7], 14, 0x676f02d9); /**//* 31 */
    GG (b, c, d, a, X[12], 20, 0x8d2a4c8a); /**//* 32 */

    /**//* Round 3 */
    HH (a, b, c, d, X[ 5],  4, 0xfffa3942); /**//* 33 */
    HH (d, a, b, c, X[ 8], 11, 0x8771f681); /**//* 34 */
    HH (c, d, a, b, X[11], 16, 0x6d9d6122); /**//* 35 */
    HH (b, c, d, a, X[14], 23, 0xfde5380c); /**//* 36 */
    HH (a, b, c, d, X[ 1],  4, 0xa4beea44); /**//* 37 */
    HH (d, a, b, c, X[ 4], 11, 0x4bdecfa9); /**//* 38 */
    HH (c, d, a, b, X[ 7], 16, 0xf6bb4b60); /**//* 39 */
    HH (b, c, d, a, X[10], 23, 0xbebfbc70); /**//* 40 */
    HH (a, b, c, d, X[13],  4, 0x289b7ec6); /**//* 41 */
    HH (d, a, b, c, X[ 0], 11, 0xeaa127fa); /**//* 42 */
    HH (c, d, a, b, X[ 3], 16, 0xd4ef3085); /**//* 43 */
    HH (b, c, d, a, X[ 6], 23, 0x04881d05); /**//* 44 */
    HH (a, b, c, d, X[ 9],  4, 0xd9d4d039); /**//* 45 */
    HH (d, a, b, c, X[12], 11, 0xe6db99e5); /**//* 46 */
    HH (c, d, a, b, X[15], 16, 0x1fa27cf8); /**//* 47 */
    HH (b, c, d, a, X[ 2], 23, 0xc4ac5665); /**//* 48 */

    /**//* Round 4 */
    II (a, b, c, d, X[ 0],  6, 0xf4292244); /**//* 49 */
    II (d, a, b, c, X[ 7], 10, 0x432aff97); /**//* 50 */
    II (c, d, a, b, X[14], 15, 0xab9423a7); /**//* 51 */
    II (b, c, d, a, X[ 5], 21, 0xfc93a039); /**//* 52 */
    II (a, b, c, d, X[12],  6, 0x655b59c3); /**//* 53 */
    II (d, a, b, c, X[ 3], 10, 0x8f0ccc92); /**//* 54 */
    II (c, d, a, b, X[10], 15, 0xffeff47d); /**//* 55 */
    II (b, c, d, a, X[ 1], 21, 0x85845dd1); /**//* 56 */
    II (a, b, c, d, X[ 8],  6, 0x6fa87e4f); /**//* 57 */
    II (d, a, b, c, X[15], 10, 0xfe2ce6e0); /**//* 58 */
    II (c, d, a, b, X[ 6], 15, 0xa3014314); /**//* 59 */
    II (b, c, d, a, X[13], 21, 0x4e0811a1); /**//* 60 */
    II (a, b, c, d, X[ 4],  6, 0xf7537e82); /**//* 61 */
    II (d, a, b, c, X[11], 10, 0xbd3af235); /**//* 62 */
    II (c, d, a, b, X[ 2], 15, 0x2ad7d2bb); /**//* 63 */
    II (b, c, d, a, X[ 9], 21, 0xeb86d391); /**//* 64 */

    digest[0] += a;
    digest[1] += b;
    digest[2] += c;
    digest[3] += d;
}

void CHgzMD5::md5end( UINT8 *begin, UINT32 len, UINT32 originalByteLen )
{
    UINT8 x[512/8]; // 64 bytes
    UINT64 originalBitLen = originalByteLen*8;

    memcpy(x, begin, len);

    // Padding： 高位补1，后续补0
    x[len] = 0x80;
    memset(x+len+1, 0, (512/8)-len-1);

    if(len >= ((512-64)/8))
    {
        md5core(x);
        memset(x, 0, (512/8));
    }

    memcpy(x+(512-64)/8, (const UINT8*)&originalBitLen, (64/8));    //文件末尾加入原文件的bit长度
    md5core(x);
}

