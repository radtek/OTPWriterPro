#pragma once


// CHgzMFCEditBrowseCtrl



#define WM_USER_UPDATE (WM_USER+1)

class CHgzMFCEditBrowseCtrl : public CMFCEditBrowseCtrl
{
	DECLARE_DYNAMIC(CHgzMFCEditBrowseCtrl)

public:
	CHgzMFCEditBrowseCtrl();
	virtual ~CHgzMFCEditBrowseCtrl();



protected:
	DECLARE_MESSAGE_MAP()
};


