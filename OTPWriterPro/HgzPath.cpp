#include "StdAfx.h"
#include "HgzPath.h"




BOOL CHgzPath::empty_directory( const TCHAR *sPath )
{
    CFileFind finder;
    CString  sWildCard = sPath;
    sWildCard += _T("//*.*");
    BOOL bFound;
    BOOL bWorking = finder.FindFile(sWildCard);
    
    bFound = bWorking;
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
        if (finder.IsDots()) continue;
        if (finder.IsDirectory())
        {
            CString s = finder.GetFilePath();
            empty_directory(s.GetString());
            ::RemoveDirectory(finder.GetFilePath());
            continue;
        }
        _tunlink( finder.GetFilePath() ); // 注意这里用的是_tunlink
    }
    return bFound;
}

BOOL CHgzPath::CreateDirectory()
{
    BOOL res = FALSE;

    int x = SHCreateDirectoryEx(NULL, GetFullDir(), NULL);
    if (x == ERROR_SUCCESS || x == ERROR_FILE_EXISTS || x == ERROR_ALREADY_EXISTS)
        res = TRUE;

    return res;
}

