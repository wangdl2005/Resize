#ifndef SEAM_CAVE_RESIZE_H
#define SEAM_CAVE_RESIZE_H
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include <string.h>
#include <map>

#define SEAM_CAVE_GRA_BRIGHT 0
#define SEAM_CAVE_GRA_BRIGHT_AVG 1
#define SEAM_CAVE_GRA_SOBEL	2
#define SEAM_CAVE_GRA_LAPLACE 3
#define SEAM_CAVE_GRA_CANNY 4

#define maxint 2139062143
//Object Removal�Ĳ�����̫���п��������С��Ч������
#define valForDelete 0x400
#define abs(x) ((x)<0?-(x):(x))

//�ͼ����ݶ��йص�һЩ����
#define getTiduS(a,b,c) ((uchar)*(src->imageData+(a)*src->widthStep+(b)*3+(c)))
//R+G+B
#define getTiduB(a,b) (getTiduS(h+(a),w+(b),0)+getTiduS(h+(a),w+(b),1)+getTiduS(h+(a),w+(b),2))
//�ض��������ֵ
#define getTiduG(a,b) (*(gra->imageData+(a)*gra->widthStep+(b)))		
#define getTiduEnergy(a,b) (*(tempGra->imageData+(a)*tempGra->widthStep+(b)))

//��ȡ8Uͼ��a�ĵ�
#define getPointImg8U(a,x,y) (((uchar*)((a)->imageData+(a)->widthStep*(y)))[(x)])
//��ȡ32Sͼ��a�ĵ�
#define getPointImg32S(a,x,y) (((int*)((a)->imageData+(a)->widthStep*(y)))[(x)])
//��ȡ8Sͼ��a�ĵ�
#define getPointImg8S(a,x,y) (((char*)((a)->imageData+(a)->widthStep*(y)))[(x)])
//��ȡ8U3Cͼ��a�ĵ�
#define getPointImg8U3C(a,x,y,c) (((uchar*)((a)->imageData+(a)->widthStep*(y)))[(x)*3+(c)])


class SeamCarve
{

	//ԭͼ��Ŀ��ͼ���������֡�ɾ�����֡��ݶ�ͼ����ʾ�ڹ�������ͼ
	//IplImage *src,*img,*mask,*mask2,*gra,*dis;

public:
	//9*9����������������ȡ���������һȦû�м���
	static void neoBrightAvgGradient(const IplImage *src,IplImage *gra);
	//���ݶȣ������ȼ��㣩ps��ע��!!!��������������ұ�Եһ�к��±�Եһ�У���ע���ʼ��������
	static void neoBrightGradient(const IplImage *,IplImage *);
	//Sobel���ӻ�ȡ��Ե
	static void neoSobelGradient(const IplImage *src,IplImage *gra);
	//Laplace���ӻ�ȡ��Ե
	static void neoLaplaceGradient(const IplImage* src,IplImage *gra);
	//Canny���ӻ�ȡ��Ե
	static void neoCannyGradient(const IplImage* src,IplImage *gra);
	/*���ĺ���nSeamCarving / nSeamCarvingVertical��Ŀ��img(���)������mask�������ã�������mask2��ɾ���ã����ݶ�gra(����Ĳ���)����ʾ������ͼdis
	����ɹ��ҵ�·��������ͼ��ѹ��һ��(һ��)*/
	static void nSeamCarving(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);

	static void nSeamCarvingVertical(IplImage *img,IplImage *mask,IplImage *mask2, IplImage *gra,IplImage *dis);
	//height��widthΪĿ��ߡ���
	static void nSeamCarving(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width);
	
	//static void nSeamCarvingVertical(IplImage *img,IplImage *mask,IplImage *mask2, IplImage *gra,IplImage *dis,int height,int width);
	//����Сֵ��λ�ã�-1��0��1��2(error)
	static int pomin(int x,int y,int z);
	
	//static void nSeamCarvingLarge(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);
	//height,width��ʾ��ǰimg���账��ķ�Χ
	static void nSeamCarvingLarge(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width);
	
	//static void nSeamCarvingLargeVertical(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);

	static void nSeamCarvingLargeVertical(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width);

	static void DWT(IplImage *pImage, int nLayer)
	{
		 // ִ������
		 if (pImage)
		 {
		  if (pImage->nChannels == 1 &&
		   pImage->depth == IPL_DEPTH_32F &&
		   ((pImage->width >> nLayer) << nLayer) == pImage->width &&
		   ((pImage->height >> nLayer) << nLayer) == pImage->height)
		  {
		   int     i, x, y, n;
		   float   fValue   = 0;
		   float   fRadius  = sqrt(2.0f);
		   int     nWidth   = pImage->width;
		   int     nHeight  = pImage->height;
		   int     nHalfW   = nWidth / 2;
		   int     nHalfH   = nHeight / 2;
		   float **pData    = new float*[pImage->height];
		   float  *pRow     = new float[pImage->width];
		   float  *pColumn  = new float[pImage->height];
		   for (i = 0; i < pImage->height; i++)
		   {
			pData[i] = (float*) (pImage->imageData + pImage->widthStep * i);
		   }
		   // ���С���任
		   for (n = 0; n < nLayer; n++, nWidth /= 2, nHeight /= 2, nHalfW /= 2, nHalfH /= 2)
		   {
			// ˮƽ�任
			for (y = 0; y < nHeight; y++)
			{
			 // ��ż����
			 memcpy(pRow, pData[y], sizeof(float) * nWidth);
			 for (i = 0; i < nHalfW; i++)
			 {
			  x = i * 2;
			  pData[y][i] = pRow[x];
			  pData[y][nHalfW + i] = pRow[x + 1];
			 }
			 // ����С���任
			 for (i = 0; i < nHalfW - 1; i++)
			 {
			  fValue = (pData[y][i] + pData[y][i + 1]) / 2;
			  pData[y][nHalfW + i] -= fValue;
			 }
			 fValue = (pData[y][nHalfW - 1] + pData[y][nHalfW - 2]) / 2;
			 pData[y][nWidth - 1] -= fValue;
			 fValue = (pData[y][nHalfW] + pData[y][nHalfW + 1]) / 4;
			 pData[y][0] += fValue;
			 for (i = 1; i < nHalfW; i++)
			 {
			  fValue = (pData[y][nHalfW + i] + pData[y][nHalfW + i - 1]) / 4;
			  pData[y][i] += fValue;
			 }
			 // Ƶ��ϵ��
			 for (i = 0; i < nHalfW; i++)
			 {
			  pData[y][i] *= fRadius;
			  pData[y][nHalfW + i] /= fRadius;
			 }
			}
			// ��ֱ�任
			for (x = 0; x < nWidth; x++)
			{
			 // ��ż����
			 for (i = 0; i < nHalfH; i++)
			 {
			  y = i * 2;
			  pColumn[i] = pData[y][x];
			  pColumn[nHalfH + i] = pData[y + 1][x];
			 }
			 for (i = 0; i < nHeight; i++)
			 {
			  pData[i][x] = pColumn[i];
			 }
			 // ����С���任
			 for (i = 0; i < nHalfH - 1; i++)
			 {
			  fValue = (pData[i][x] + pData[i + 1][x]) / 2;
			  pData[nHalfH + i][x] -= fValue;
			 }
			 fValue = (pData[nHalfH - 1][x] + pData[nHalfH - 2][x]) / 2;
			 pData[nHeight - 1][x] -= fValue;
			 fValue = (pData[nHalfH][x] + pData[nHalfH + 1][x]) / 4;
			 pData[0][x] += fValue;
			 for (i = 1; i < nHalfH; i++)
			 {
			  fValue = (pData[nHalfH + i][x] + pData[nHalfH + i - 1][x]) / 4;
			  pData[i][x] += fValue;
			 }
			 // Ƶ��ϵ��
			 for (i = 0; i < nHalfH; i++)
			 {
			  pData[i][x] *= fRadius;
			  pData[nHalfH + i][x] /= fRadius;
			 }
			}
		   }
		   delete[] pData;
		   delete[] pRow;
		   delete[] pColumn;
		  }
		 }
	}
	
	static long getNiuquXishu(IplImage *img)
	{
		// С���任����
		int nLayer = 1;
		// �����ɫͼ��
		IplImage *pSrc = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
		cvCopy(img,pSrc,NULL);
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
		/*
		// ��ʾͼ��pSrc
				cvNamedWindow("dwt",1);
				cvShowImage("dwt",pSrc);
				cvWaitKey(0);
				cvDestroyWindow("dwt");
				*/
		IplImage * pSrcGray  = cvCreateImage(cvGetSize(pSrc),IPL_DEPTH_8U,1);
		cvCvtColor(pSrc,pSrcGray,CV_BGR2GRAY);
		//cvConvertScaleAbs(pSrc,pSrcGray,1,0);
		long sum = 0;
		for (int i = pSrcGray->width/2;i<pSrcGray->width;i++)
		{
			for (int j = pSrcGray->height/2;j<pSrcGray->height;j++)
			{
				sum += getPointImg8U(pSrcGray,i,j);
			}
		}
		
		sum /= (pSrcGray->width * pSrcGray->height/4);
		// ...
		cvReleaseImage(&pSrc);
		cvReleaseImage(&pSrcGray);
		return sum;
	}
};

#endif