// Resize.h : main header file for the RESIZE application
//

#if !defined(AFX_RESIZE_H__0CEB1C3E_9F7A_4EB5_BD9E_B8AA2E63D051__INCLUDED_)
#define AFX_RESIZE_H__0CEB1C3E_9F7A_4EB5_BD9E_B8AA2E63D051__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CResizeApp:
// See Resize.cpp for the implementation of this class
//

class CResizeApp : public CWinApp
{
public:
	CResizeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CResizeApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZE_H__0CEB1C3E_9F7A_4EB5_BD9E_B8AA2E63D051__INCLUDED_)
