#pragma once


// CHgzEditBrowseCtrl



#define WM_HgzEditBrowseCtrl_AFTERUPDATE (WM_USER+1)

class CHgzEditBrowseCtrl : public CMFCEditBrowseCtrl
{
	//DECLARE_DYNAMIC(CHgzEditBrowseCtrl)

public:
	CHgzEditBrowseCtrl();
	virtual ~CHgzEditBrowseCtrl();

    virtual void OnBrowse();
    virtual void SetFilter(CString strFilter) { m_strFilter = strFilter; }


protected:
	DECLARE_MESSAGE_MAP()

    CString m_strFilter; //_T("All Files (*.*)|*.*||");
};


