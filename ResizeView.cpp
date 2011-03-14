// ResizeView.cpp : implementation of the CResizeView class
//

#include "stdafx.h"
#include "Resize.h"

#include "ResizeDoc.h"
#include "ResizeView.h"
#include <afxwin.h>
#include "InputDlg.h"
#include <cxcore.h>
#include "SeamCarveResize.h"
#include <time.h>
#include <mmsystem.h>
#include "Log.h"
//tiemGetTime()����
#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizeView

IMPLEMENT_DYNCREATE(CResizeView, CView)

BEGIN_MESSAGE_MAP(CResizeView, CView)
	//{{AFX_MSG_MAP(CResizeView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_PIC_RESIZE_INSIDE, OnPicResizeInside)
	ON_COMMAND(ID_PIC_SEAMCARVE, OnPicSeamcarve)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_PIC_SEAMCARVE2, OnPicSeamcarve2)
	ON_COMMAND(ID_PIC_SEAMCARVE_SOBEL, OnPicSeamcarveSobel)
	ON_COMMAND(ID_PIC_FOR_TEST, OnPicForTest)
	ON_COMMAND(ID_PIC_SEAMCARVE_LAPLACE, OnPicSeamcarveLaplace)
	ON_COMMAND(ID_PIC_SEAMCARVE_CANNY, OnPicSeamcarveCanny)
	ON_COMMAND(ID_PIC_SEAMCARVE_WEIGHT, OnPicSeamcarveWeight)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizeView construction/destruction

CResizeView::CResizeView()
{
	// TODO: add construction code here
	proc = 0;
	procNew = 0;
}

CResizeView::~CResizeView()
{
	delete proc;
	delete procNew;
}

BOOL CResizeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CResizeView drawing

void CResizeView::OnDraw(CDC* pDC)
{
	CResizeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CResizeView printing

BOOL CResizeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CResizeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CResizeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CResizeView diagnostics

#ifdef _DEBUG
void CResizeView::AssertValid() const
{
	CView::AssertValid();
}

void CResizeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CResizeDoc* CResizeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResizeDoc)));
	return (CResizeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CResizeView message handlers
void CResizeView::ShowOnDc(const IplImage*img,int idx /* = 0 */,int idy /* =0 */,const char * title /* = */ )
{
	CClientDC dc(this);	
	OnPrepareDC(&dc);
	CImage image;
	image.CopyOf((IplImage *)img);
	RECT * tmpRect = new RECT();
	tmpRect->right = image.Width() + idx;
	tmpRect->left = 0 + idx;
	tmpRect->top = 0 + idy;
	tmpRect->bottom = image.Height() + idy;
	image.DrawToHDC(dc,tmpRect);
	dc.TextOut(idx,image.Height() + idy,title);
	OnDraw(&dc);
}

void CResizeView::OpenPic()
{
	CFileDialog dlg(TRUE,_T("*.bmp"),"",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY
		,"image files(*.bmp;*.jpg;*.png;*.gif)|*.bmp;*.jpg;*.png;*.gif|ALLFiles(*.*)||"
		,NULL);
	char title[] = 
	{
		"Open Image"
	};
	dlg.m_ofn.lpstrTitle = title;
	if(IDOK == dlg.DoModal())
	{
		CString path = dlg.GetPathName();
		if(proc != 0)
		{
			delete proc;		
		}
		proc = new ImageProcessor (path,true);
		CClientDC dc(this);
		Invalidate(TRUE);
		UpdateWindow();
		//ShowOnDc(proc->getImg());	
	}
}
void CResizeView::PicResizeInside()
{
	if(proc != 0)
	{
		CInputDlg dlg;
		char h_origin[5];
		char wid_origin[5];
		int h_to = 0;
		int wid_to = 0;
		itoa(proc->getImg()->height,h_origin,10);
		itoa(proc->getImg()->width,wid_origin,10);
		dlg.m_h_origin = (CString)h_origin;
		dlg.m_wid_origin = (CString)wid_origin;
		if(IDOK == dlg.DoModal())
		{
			h_to = dlg.m_h_to;
			wid_to = dlg.m_wid_to;
			
				if(procNew != 0)
				{
					delete procNew;		
				}
				time_t tt_start;
				//ctime(&tt_start);//������s����׼ȷ
				//���ص�ǰϵͳ������ʱ��ms��
				tt_start = (time_t) timeGetTime();
				IplImage* desc = cvCreateImage(cvSize(wid_to,h_to),proc->getImg()->depth,proc->getImg()->nChannels);
				cvResize(proc->getImg(),desc,CV_INTER_CUBIC);
				/*
				��ֵ����: 
				CV_INTER_NN - ����ڲ�ֵ,
				CV_INTER_LINEAR - ˫���Բ�ֵ (ȱʡʹ��)
				CV_INTER_AREA - ʹ�����ع�ϵ�ز�������ͼ����Сʱ�򣬸÷������Ա��Ⲩ�Ƴ��֡���ͼ��Ŵ�ʱ�������� CV_INTER_NN ����..
				CV_INTER_CUBIC - ������ֵ.
				*/
				procNew = new ImageProcessor(desc,"ת�����ͼƬ(���÷���)",true);		
				//ShowOnDc(procNew->getImg(),proc->getImg()->width + 10, 0,"ת�����ͼƬ(���÷���)");				
				cvReleaseImage(&desc);
				time_t tt_end;
				tt_end = (time_t) timeGetTime();
				char tempString[20];
				sprintf(tempString,"����ʱ��Ϊ%d����",(tt_end-tt_start));
		}		
	}
	else
	{
		MessageBox("���ȴ�ͼƬ");
	}
}

void CResizeView::PicResizeSeamCarve(int energyMethod)
{
	if(proc != 0)
	{
		CInputDlg dlg;
		char h_origin[5];
		char wid_origin[5];
		int h_to = 0;
		int wid_to = 0;
		int h_from = 0;
		int wid_from = 0;
		h_from = proc->getImg()->height;
		wid_from = proc->getImg()->width;
		itoa(h_from,h_origin,10);
		itoa(wid_from,wid_origin,10);
		dlg.m_h_origin = (CString)h_origin;
		dlg.m_wid_origin = (CString)wid_origin;
		if(IDOK == dlg.DoModal())
		{
			h_to = dlg.m_h_to;
			wid_to = dlg.m_wid_to;			
			if(procNew!=0)
			{
				delete procNew;
			}			
			time_t tt_start;
			//ctime(&tt_start);//������s����׼ȷ
			//���ص�ǰϵͳ������ʱ��ms��
			tt_start = (time_t) timeGetTime();	
			IplImage *img = NULL;
			IplImage *mask = NULL;
			IplImage *mask2 = NULL;
			IplImage *gra = NULL;
			IplImage *dis = NULL;
			int wid_toSet = 0;
			int h_toSet = 0;
			int depth_toSet = 0;
			int nChan_toSet = 0;
			long uNew = 0;
			long uOld = 0;
			wid_toSet = wid_from < wid_to ? wid_to : proc->getImg()->width;
			h_toSet = h_from < h_to ? h_to : proc->getImg()->height;
			depth_toSet = proc->getImg()->depth;
			nChan_toSet = proc->getImg()->nChannels;
			//Ԥ��ͼƬ�ռ䣬��ֹ�㷨���ظ�����ռ�
			img = cvCreateImage(cvSize(wid_toSet,h_toSet),depth_toSet,nChan_toSet);
			mask = cvCreateImage(cvSize(wid_toSet,h_toSet),IPL_DEPTH_8U,1);
			mask2 = cvCreateImage(cvSize(wid_toSet,h_toSet),IPL_DEPTH_8U,1);
			gra = cvCreateImage(cvSize(wid_toSet,h_toSet),IPL_DEPTH_8U,1);
			dis = cvCreateImage(cvSize(wid_toSet,h_toSet),depth_toSet,nChan_toSet);
			cvZero(img);			
			cvZero(dis);
			for(int i=0;i<wid_from;i++)
				for(int j=0;j<h_from;j++)
				{
					getPointImg8U3C(img,i,j,0)=getPointImg8U3C(proc->getImg(),i,j,0);
					getPointImg8U3C(img,i,j,1)=getPointImg8U3C(proc->getImg(),i,j,1);
					getPointImg8U3C(img,i,j,2)=getPointImg8U3C(proc->getImg(),i,j,2);
					getPointImg8U3C(dis,i,j,0)=getPointImg8U3C(proc->getImg(),i,j,0);
					getPointImg8U3C(dis,i,j,1)=getPointImg8U3C(proc->getImg(),i,j,1);
					getPointImg8U3C(dis,i,j,2)=getPointImg8U3C(proc->getImg(),i,j,2);
				}
			cvZero(mask);			
			cvZero(mask2);			
			cvZero(gra);
			//SeamCarve::neoBrightGradient(img,gra);
			switch (energyMethod)
			{
			case SEAM_CAVE_GRA_BRIGHT ://BrightGra	
			   {
					SeamCarve::neoBrightGradient(img,gra);
					break;
			   }
			case SEAM_CAVE_GRA_BRIGHT_AVG ://BrightGraAvg
			   {
					SeamCarve::neoBrightAvgGradient(img,gra);
					break;
			   }
			case SEAM_CAVE_GRA_SOBEL:
				{
					SeamCarve::neoSobelGradient(img,gra);
					break;
				}
			case SEAM_CAVE_GRA_LAPLACE:
				{
					SeamCarve::neoLaplaceGradient(img,gra);
					break;
				}
			case SEAM_CAVE_GRA_CANNY:
				{
					SeamCarve::neoCannyGradient(img,gra);
					break;
				}
			case SEAM_CAVE_GRA_WEIGHT:
				{
					SeamCarve::neoWeightGradient(img,gra);
					break;
				}
			default : 
				{
					MessageBox("Wrong Energy Method");
					return;
				}
			}
			uOld = SeamCarve::getNiuquXishu(img);
			while(h_from!=h_to)
			{
				uNew = SeamCarve::getNiuquXishu(img);
				if(abs(uNew - uOld) < 2)
				{
					if(h_from>h_to)
					{
						SeamCarve::nSeamCarving(img,mask,mask2,gra,dis);
						h_from = img->height ;
					}
					else if(h_from<h_to)
					{
						SeamCarve::nSeamCarvingLarge(img,mask,mask2,gra,dis,h_from,wid_from);
						h_from ++;
					}
				}
				else
				{
					IplImage *pImgTmp;
					if(h_from>h_to)
					/*
					{
											pImgTmp = cvCreateImage(cvSize(wid_from,(--h_from)),img->depth,img->nChannels);
										}
										else if(h_from<h_to)
										{
											pImgTmp = cvCreateImage(cvSize(wid_from,(++h_from)),img->depth,img->nChannels);
										
										}*/
					pImgTmp = cvCreateImage(cvSize(wid_from,h_to),img->depth,img->nChannels);
					cvResize(img,pImgTmp,CV_INTER_CUBIC);
					img = cvCreateImage(cvGetSize(pImgTmp),pImgTmp->depth,pImgTmp->nChannels);
					cvCopy(pImgTmp,img);
					cvReleaseImage(&pImgTmp);
					break;
				}
				uOld = uNew;
			}
			switch (energyMethod)
			{
			case SEAM_CAVE_GRA_BRIGHT ://BrightGra	
				{
					SeamCarve::neoBrightGradient(img,gra);
					break;
				}
			case SEAM_CAVE_GRA_BRIGHT_AVG ://BrightGraAvg
				{
					SeamCarve::neoBrightAvgGradient(img,gra);
					break;
				}
			case SEAM_CAVE_GRA_SOBEL:
				{
					SeamCarve::neoSobelGradient(img,gra);
					break;
				}
			case SEAM_CAVE_GRA_LAPLACE:
				{
					SeamCarve::neoLaplaceGradient(img,gra);
					break;
				}
			case SEAM_CAVE_GRA_CANNY:
				{
					SeamCarve::neoCannyGradient(img,gra);
					break;
				}
			case SEAM_CAVE_GRA_WEIGHT:
				{
					SeamCarve::neoWeightGradient(img,gra);
					break;
				}
			default : 
				{
					MessageBox("Wrong Energy Method");
					return;
				}
			}
			uOld = SeamCarve::getNiuquXishu(img);
			while(wid_from!=wid_to)
			{
				uNew = SeamCarve::getNiuquXishu(img);
				if(abs(uNew - uOld) < 2)
				{
					if(wid_from>wid_to)
					{
						SeamCarve::nSeamCarvingVertical(img,mask,mask2,gra,dis);
						wid_from = img->width;
					}
					else if(wid_from<wid_to)
					{
						SeamCarve::nSeamCarvingLargeVertical(img,mask,mask2,gra,dis,h_from,wid_from);
						wid_from ++;
					}
				}
				else
				{
					IplImage *pImgTmp;
					/*
					if(wid_from>wid_to)
										{
											pImgTmp = cvCreateImage(cvSize((--wid_from),h_to),img->depth,img->nChannels);
										}
										else if(wid_from<wid_to)
										{
											pImgTmp = cvCreateImage(cvSize((++wid_from),h_to),img->depth,img->nChannels);
											
										}*/
					pImgTmp = cvCreateImage(cvSize(wid_to,h_to),img->depth,img->nChannels);
					cvResize(img,pImgTmp,CV_INTER_CUBIC);
					img = cvCreateImage(cvGetSize(pImgTmp),pImgTmp->depth,img->nChannels);
					cvCopy(pImgTmp,img);
					cvReleaseImage(&pImgTmp);
					break;
				}
				uOld = uNew;
			}
			procNew = new ImageProcessor(img,"ת�����ͼƬ(SeamCarv)",true);
			ImageProcessor * procTemp = new ImageProcessor(gra,"gra");
			cvReleaseImage(&img);
			cvReleaseImage(&mask);
			cvReleaseImage(&mask2);
			cvReleaseImage(&dis);
			cvReleaseImage(&gra);				
			time_t tt_end;
			tt_end = (time_t) timeGetTime();
			char tempString[20];
			sprintf(tempString,"����ʱ��Ϊ%d����",(tt_end-tt_start));
			Log::toLog(tempString);
			MessageBox(tempString);
		}		
	}
	else
	{
		MessageBox("���ȴ�ͼƬ");
	}
}

void CResizeView::SavePic()
{
	if(procNew != 0)
	{
		CFileDialog dlgSave(FALSE,_T("*.bmp"),"",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY
			,"image files(*.bmp;*.jpg;*.png;*.gif)|*.bmp;*.jpg;*.png;*.gif|ALLFiles(*.*)||"
			,NULL);
		char title[] = 
		{
			"Save Image"
		};
		dlgSave.m_ofn.lpstrTitle = title;
		if(IDOK == dlgSave.DoModal())
		{
			CString path = dlgSave.GetPathName();
			cvSaveImage(path,procNew->getImg());
		}
		
	}
	else
	{
		MessageBox("���ȴ���ͼƬ");
	}	
}
 
void CResizeView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	OpenPic();
}

void CResizeView::OnPicResizeInside() 
{
	// TODO: Add your command handler code here
	PicResizeInside();
}

void CResizeView::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	SavePic();
}

void CResizeView::OnPicSeamcarve() 
{
	// TODO: Add your command handler code here
	PicResizeSeamCarve(SEAM_CAVE_GRA_BRIGHT);
}

void CResizeView::OnFileSave() 
{
	// TODO: Add your command handler code here
	SavePic();
}

void CResizeView::OnPicSeamcarve2() 
{
	// TODO: Add your command handler code here
	PicResizeSeamCarve(SEAM_CAVE_GRA_BRIGHT_AVG);
	
}

void CResizeView::OnPicSeamcarveSobel() 
{
	// TODO: Add your command handler code here
	PicResizeSeamCarve(SEAM_CAVE_GRA_SOBEL);
}


void CResizeView::OnPicSeamcarveLaplace() 
{
	// TODO: Add your command handler code here
	PicResizeSeamCarve(SEAM_CAVE_GRA_LAPLACE);
}

void CResizeView::OnPicSeamcarveCanny() 
{
	// TODO: Add your command handler code here
	PicResizeSeamCarve(SEAM_CAVE_GRA_CANNY);
}


void CResizeView::OnPicForTest() 
{
	// TODO: Add your command handler code here
	
	
		IplImage *src = cvCreateImage(cvGetSize(proc->getImg()),proc->getImg()->depth,proc->getImg()->nChannels);
		cvCopy(proc->getImg(),src,NULL);
		IplImage *gra = cvCreateImage(cvGetSize(proc->getImg()),IPL_DEPTH_8U,1);
		cvZero(gra);
		SeamCarve::neoWeightGradient(src,gra);
		procNew = new ImageProcessor(gra,"gra");
		cvReleaseImage(&src);
		cvReleaseImage(&gra);
		/*
			// С���任����
					int nLayer = 1;
					// �����ɫͼ��
					IplImage *pSrc = cvCreateImage(cvGetSize(proc->getImg()),proc->getImg()->depth,proc->getImg()->nChannels);
					cvCopy(proc->getImg(),pSrc,NULL);
					 // ����С��ͼ���С
					 CvSize size = cvGetSize(pSrc);
					 if ((pSrc->width >> nLayer) << nLayer != pSrc->width)
					 {
						 size.width = ((pSrc->width >> nLayer) + 1) << nLayer;
					 }
					 if ((pSrc->height >> nLayer) << nLayer != pSrc->height)
					 {
						 size.height = ((pSrc->height >> nLayer) + 1) << nLayer;
					 }
					 // ����С��ͼ��
					 IplImage *pWavelet = cvCreateImage(size, IPL_DEPTH_32F, pSrc->nChannels);
					 if (pWavelet)
					 {
						 // С��ͼ��ֵ
						 cvSetImageROI(pWavelet, cvRect(0, 0, pSrc->width, pSrc->height));
						 cvConvertScale(pSrc, pWavelet, 1, -128);
						 cvResetImageROI(pWavelet);
						 // ��ɫͼ��С���任
						 IplImage *pImage = cvCreateImage(cvGetSize(pWavelet), IPL_DEPTH_32F, 1);
						 if (pImage)
						 {
							 for (int i = 1; i <= pWavelet->nChannels; i++)
							 {
								 cvSetImageCOI(pWavelet, i);
								 cvCopy(pWavelet, pImage, NULL);
								 // ��ά��ɢС���任
								 SeamCarve::DWT(pImage, nLayer);
								 // ��ά��ɢС���ָ�
								 // IDWT(pImage, nLayer);
								 cvCopy(pImage, pWavelet, NULL);
							 }
							 cvSetImageCOI(pWavelet, 0);
							 cvReleaseImage(&pImage);
						 }
						 // С���任ͼ��
						 cvSetImageROI(pWavelet, cvRect(0, 0, pSrc->width, pSrc->height));
						 cvConvertScale(pWavelet, pSrc, 1, 128);
						 cvResetImageROI(pWavelet); // ���д����е���࣬���������������õı��ϰ��
						 cvReleaseImage(&pWavelet);
					 }
					 // ��ʾͼ��pSrc
					 cvNamedWindow("dwt",1);
					 cvShowImage("dwt",pSrc);	 
					 // ...
					 cvReleaseImage(&pSrc);*/
			
}

void CResizeView::OnPicSeamcarveWeight() 
{
	// TODO: Add your command handler code here
	PicResizeSeamCarve(SEAM_CAVE_GRA_WEIGHT);
}
