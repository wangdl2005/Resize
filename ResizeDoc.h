// ResizeDoc.h : interface of the CResizeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZEDOC_H__F5C9ED66_3FF6_411B_B451_1DDBAFDA57BB__INCLUDED_)
#define AFX_RESIZEDOC_H__F5C9ED66_3FF6_411B_B451_1DDBAFDA57BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CResizeDoc : public CDocument
{
protected: // create from serialization only
	CResizeDoc();
	DECLARE_DYNCREATE(CResizeDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResizeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CResizeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZEDOC_H__F5C9ED66_3FF6_411B_B451_1DDBAFDA57BB__INCLUDED_)
