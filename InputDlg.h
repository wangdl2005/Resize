#if !defined(AFX_INPUTDLG_H__6FAE1B85_CFA2_4480_9214_208A735AB89C__INCLUDED_)
#define AFX_INPUTDLG_H__6FAE1B85_CFA2_4480_9214_208A735AB89C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog

class CInputDlg : public CDialog
{
// Construction
public:
	CInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDlg)
	enum { IDD = IDD_SIZEDLG };
	CString	m_wid_origin;
	CString	m_h_origin;
	int		m_h_to;
	int		m_wid_to;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG_H__6FAE1B85_CFA2_4480_9214_208A735AB89C__INCLUDED_)
