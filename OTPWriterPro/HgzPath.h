#pragma once

#include <stdlib.h>
#include <stdio.h>
//#include <string.h>
//#include "hgz/hgz.h"

//using std::string;
#define CHGZPATH_TEMP_VAR TCHAR _drive[_MAX_DRIVE], _dir[_MAX_DIR], _fname[_MAX_FNAME], _title[_MAX_FNAME], _ext[_MAX_EXT]


#if 0
// version 1:
// A single string is stored as the whole path, no seprate parts are saved. Thus memory occupation is least, but getting each part of the path is slow.
// Core string can be implemented as CString or std::string.
class CHgzPath
{
public:
    CHgzPath(void)  {}
    CHgzPath(TCHAR *fullPath) { _tcscpy_s(_path_buffer, fullPath); split(); }
    ~CHgzPath(void) {}

public:
    CHgzPath& Build();

    CString GetFullPath()  { _tcscat_s(_path_buffer, _drive); _tcscat_s(_path_buffer, _dir); _tcscat_s(_path_buffer, _title); _tcscat_s(_path_buffer, _ext); return _path_buffer; }
    CString GetDrive()     { return _drive; }
    CString GetDir()       { return _dir; }
    CString GetFileName()  { _tcscat_s(_fname, _title); _tcscat_s(_fname, _ext); return _fname; }
    CString GetFileTitle() { return _title; }
    CString GetFileExt()   { return _ext; }

    // Return the resulting full path.
    CString Append(TCHAR *s)          { _tcscat_s(_path_buffer, s); split();     return GetFullPath(); }
    CString ChangePath(TCHAR *s)      { _tcscpy_s(_path_buffer, s); split();     return GetFullPath(); }
    CString ChangeDrive(TCHAR *s)     { _tcscpy_s(_drive, s);                    return GetDrive(); }
    CString ChangeDir(TCHAR *s)       { _tcscpy_s(_dir,   s);                    return GetDir(); }
    CString ChangeFileName(TCHAR *s)  { _tcscpy_s(_fname, s); split_file_name(); return GetFileName(); }
    CString ChangeFileTitle(TCHAR *s) { _tcscpy_s(_title, s); build_file_name(); return GetFileTitle(); }
    CString ChangeFileExt(TCHAR *s)   { _tcscpy_s(_ext,   s); build_file_name(); return GetFileExt(); }


    int  SplitDir(void); // return number of levels of the DIR.
    void MakeDir(void);
    void MakeDir(TCHAR *dir);

    void MakePath(TCHAR *path);

    // Reference to ATL::CPath members below

private:
    TCHAR _path_buffer[MAX_PATH];
    CHGZPATH_TEMP_VAR;


private:
    void split_path()      { _tsplitpath_s(_path_buffer, _drive, _dir, _fname, _ext); }
    void split_file_name() { _tcscpy_s(_title, _tcslen(_fname)-_tcslen(_ext), _fname); }
    void split()           { split_path(); split_file_name(); }
    void build_file_name() { _tcscpy_s(_fname, _title); _tcscat_s(_fname, _ext); }
    void build_path()      {}
};

#else
// version 2: 
// Each part of the path is saved seperately. Thus, getting it is fast.
class CHgzPath
{
public:
    CHgzPath(void)  {}
    CHgzPath(const TCHAR *fullPath)   { create_object(fullPath); }
    CHgzPath(const CString &fullPath) { create_object(fullPath.GetString()); }
    CHgzPath(const CString *fullPath) { create_object(fullPath->GetString()); }
    ~CHgzPath(void) {}

public:
    CHgzPath& Build();
    
    CString GetFullPath()  { _tcscpy_s(_path_buffer, _drive); _tcscat_s(_path_buffer, _dir); _tcscat_s(_path_buffer, _title); _tcscat_s(_path_buffer, _ext); return _path_buffer; }
    CString GetDrive()     { return _drive; }
    CString GetDir()       { return _dir; } // only Dir
    CString GetFullDir()   { _tcscpy_s(_path_buffer, _drive); _tcscat_s(_path_buffer, _dir); return _path_buffer; } // Drive + Dir
    CString GetFileName()  { _tcscpy_s(_fname, _title); _tcscat_s(_fname, _ext); return _fname; }
    CString GetFileTitle() { return _title; }
    CString GetFileExt()   { return _ext; }

    // Return the resulting full path.
    CString Append(const TCHAR *s)          { GetFullPath(); _tcscat_s(_path_buffer, s); split();     return GetFullPath(); }
    CString ChangePath(const TCHAR *s)      { _tcscpy_s(_path_buffer, s); split();     return GetFullPath(); }
    CString ChangeDrive(const TCHAR *s)     { _tcscpy_s(_drive, s);                    return GetDrive(); }
    CString ChangeDir(const TCHAR *s)       { _tcscpy_s(_dir,   s);                    return GetDir(); }
    CString ChangeFullDir(const TCHAR *s)   { split_full_dir(s);                       return GetFullDir(); }
    CString ChangeFileName(const TCHAR *s)  { _tcscpy_s(_fname, s); split_file_name(); return GetFileName(); }
    CString ChangeFileTitle(const TCHAR *s) { _tcscpy_s(_title, s); build_file_name(); return GetFileTitle(); }
    CString ChangeFileExt(const TCHAR *s)   { _tcscpy_s(_ext,   s); build_file_name(); return GetFileExt(); }
    

    int  SplitDir(void); // return number of levels of the DIR.
    void MakeDir(void);
    void MakeDir(TCHAR *dir);

    // Reference to ATL::CPath members below

    // Expansion:
    BOOL CreateDirectory();
    BOOL RemoveDirectory() { BOOL res = FALSE; if (ClearDirectory()) res = ::RemoveDirectory(GetFullDir()); return res; }
    BOOL ClearDirectory() { return empty_directory(GetFullDir().GetString()); }
    bool IsExisted();
    BOOL IsDirExisted() { CFileFind finder; return finder.FindFile(GetFullDir()); }
    bool OpenFile();

private:
    TCHAR _path_buffer[MAX_PATH];
    CHGZPATH_TEMP_VAR;


private:
    void split()           { split_path(); }
    void split_path()      { _tsplitpath_s(_path_buffer, _drive, _dir, _title, _ext); }
    void split_full_dir(const TCHAR *s) { _tsplitpath_s(s, _drive, _MAX_DRIVE, _dir, _MAX_DIR, NULL, 0, NULL, 0); }
    void split_file_name() { _tsplitpath_s(_fname, NULL, 0, NULL, 0, _title, _MAX_FNAME, _ext, _MAX_EXT); }
    void build_file_name() { _tcscpy_s(_fname, _title); _tcscat_s(_fname, _ext); }
    void build_path()      {}
    BOOL empty_directory(const TCHAR *sPath);

    void create_object( const TCHAR * fullPath )
    {
        _tcscpy_s(_path_buffer, fullPath); split();
    }
    
};

#endif


