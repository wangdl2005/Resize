// ResizeView.h : interface of the CResizeView class
//
/////////////////////////////////////////////////////////////////////////////

#include "ImageProcessor.h"
#include "SeamCarveResize.h"

#if !defined(AFX_RESIZEVIEW_H__B72B5493_8B45_46FF_B424_6A37C8636436__INCLUDED_)
#define AFX_RESIZEVIEW_H__B72B5493_8B45_46FF_B424_6A37C8636436__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {};

class CResizeView : public CView
{
protected: // create from serialization only
	CResizeView();
	DECLARE_DYNCREATE(CResizeView)

// Attributes
public:
	CResizeDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResizeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//用于保存图像
	ImageProcessor *proc;
	ImageProcessor *procNew;//保存最后处理的结果
protected:
	void OpenPic();
	void PicResizeInside();
	void SavePic();
	void PicResizeSeamCarve(int EnergyMethod);
	void ShowOnDc(const IplImage*img,int idx = 0,int idy =0,const char * title = "原图");
	//void cairResize(const IplImage* src,const IplImage* dst,int newWidth, int newHeight);
	//void IMG_to_CML( IplImage * Source, CML_color * Dest );
	//void CML_to_IMG( CML_color * Source, IplImage * Dest );

// Generated message map functions
protected:
	//{{AFX_MSG(CResizeView)
	afx_msg void OnFileOpen();
	afx_msg void OnPicResizeInside();
	afx_msg void OnPicSeamcarve();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnPicSeamcarve2();
	afx_msg void OnPicSeamcarveSobel();
	afx_msg void OnPicForTest();
	afx_msg void OnPicSeamcarveLaplace();
	afx_msg void OnPicSeamcarveCanny();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ResizeView.cpp
inline CResizeDoc* CResizeView::GetDocument()
   { return (CResizeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZEVIEW_H__B72B5493_8B45_46FF_B424_6A37C8636436__INCLUDED_)
