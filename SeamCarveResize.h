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
//Object Removal的参数，太大有可能溢出，小了效果不好
#define valForDelete 0x400
#define abs(x) ((x)<0?-(x):(x))

//和计算梯度有关的一些函数
#define getTiduS(a,b,c) ((uchar)*(src->imageData+(a)*src->widthStep+(b)*3+(c)))
//R+G+B
#define getTiduB(a,b) (getTiduS(h+(a),w+(b),0)+getTiduS(h+(a),w+(b),1)+getTiduS(h+(a),w+(b),2))
//特定点的像素值
#define getTiduG(a,b) (*(gra->imageData+(a)*gra->widthStep+(b)))		
#define getTiduEnergy(a,b) (*(tempGra->imageData+(a)*tempGra->widthStep+(b)))

//提取8U图像a的点
#define getPointImg8U(a,x,y) (((uchar*)((a)->imageData+(a)->widthStep*(y)))[(x)])
//提取32S图像a的点
#define getPointImg32S(a,x,y) (((int*)((a)->imageData+(a)->widthStep*(y)))[(x)])
//提取8S图像a的点
#define getPointImg8S(a,x,y) (((char*)((a)->imageData+(a)->widthStep*(y)))[(x)])
//提取8U3C图像a的点
#define getPointImg8U3C(a,x,y,c) (((uchar*)((a)->imageData+(a)->widthStep*(y)))[(x)*3+(c)])


class SeamCarve
{

	//原图、目标图、保护遮罩、删除遮罩、梯度图、显示在工作区的图
	//IplImage *src,*img,*mask,*mask2,*gra,*dis;

public:
	//9*9区域内亮度特征提取，最外面的一圈没有计算
	static void neoBrightAvgGradient(const IplImage *src,IplImage *gra);
	//求梯度（按亮度计算）ps：注意!!!这个函数少算了右边缘一行和下边缘一行，请注意初始化。。。
	static void neoBrightGradient(const IplImage *,IplImage *);
	//Sobel算子获取边缘
	static void neoSobelGradient(const IplImage *src,IplImage *gra);
	//Laplace算子获取边缘
	static void neoLaplaceGradient(const IplImage* src,IplImage *gra);
	//Canny算子获取边缘
	static void neoCannyGradient(const IplImage* src,IplImage *gra);
	/*核心函数nSeamCarving / nSeamCarvingVertical：目标img(结果)，遮罩mask（保护用），遮罩mask2（删除用），梯度gra(需给的参数)，显示遮罩用图dis
	如果成功找到路径，所有图被压缩一行(一列)*/
	static void nSeamCarving(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);

	static void nSeamCarvingVertical(IplImage *img,IplImage *mask,IplImage *mask2, IplImage *gra,IplImage *dis);
	//height，width为目标高、宽
	static void nSeamCarving(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width);
	
	//static void nSeamCarvingVertical(IplImage *img,IplImage *mask,IplImage *mask2, IplImage *gra,IplImage *dis,int height,int width);
	//找最小值的位置：-1、0、1、2(error)
	static int pomin(int x,int y,int z);
	
	//static void nSeamCarvingLarge(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);
	//height,width表示当前img所需处理的范围
	static void nSeamCarvingLarge(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width);
	
	//static void nSeamCarvingLargeVertical(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);

	static void nSeamCarvingLargeVertical(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width);

	static void DWT(IplImage *pImage, int nLayer)
	{
		 // 执行条件
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
		   // 多层小波变换
		   for (n = 0; n < nLayer; n++, nWidth /= 2, nHeight /= 2, nHalfW /= 2, nHalfH /= 2)
		   {
			// 水平变换
			for (y = 0; y < nHeight; y++)
			{
			 // 奇偶分离
			 memcpy(pRow, pData[y], sizeof(float) * nWidth);
			 for (i = 0; i < nHalfW; i++)
			 {
			  x = i * 2;
			  pData[y][i] = pRow[x];
			  pData[y][nHalfW + i] = pRow[x + 1];
			 }
			 // 提升小波变换
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
			 // 频带系数
			 for (i = 0; i < nHalfW; i++)
			 {
			  pData[y][i] *= fRadius;
			  pData[y][nHalfW + i] /= fRadius;
			 }
			}
			// 垂直变换
			for (x = 0; x < nWidth; x++)
			{
			 // 奇偶分离
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
			 // 提升小波变换
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
			 // 频带系数
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
		// 小波变换层数
		int nLayer = 1;
		// 输入彩色图像
		IplImage *pSrc = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
		cvCopy(img,pSrc,NULL);
		// 计算小波图象大小
		CvSize size = cvGetSize(pSrc);
		if ((pSrc->width >> nLayer) << nLayer != pSrc->width)
		{
			size.width = ((pSrc->width >> nLayer) + 1) << nLayer;
		}
		if ((pSrc->height >> nLayer) << nLayer != pSrc->height)
		{
			size.height = ((pSrc->height >> nLayer) + 1) << nLayer;
		}
		// 创建小波图象
		IplImage *pWavelet = cvCreateImage(size, IPL_DEPTH_32F, pSrc->nChannels);
		if (pWavelet)
		{
			// 小波图象赋值
			cvSetImageROI(pWavelet, cvRect(0, 0, pSrc->width, pSrc->height));
			cvConvertScale(pSrc, pWavelet, 1, -128);
			cvResetImageROI(pWavelet);
			// 彩色图像小波变换
			IplImage *pImage = cvCreateImage(cvGetSize(pWavelet), IPL_DEPTH_32F, 1);
			if (pImage)
			{
				for (int i = 1; i <= pWavelet->nChannels; i++)
				{
					cvSetImageCOI(pWavelet, i);
					cvCopy(pWavelet, pImage, NULL);
					// 二维离散小波变换
					SeamCarve::DWT(pImage, nLayer);
					// 二维离散小波恢复
					// IDWT(pImage, nLayer);
					cvCopy(pImage, pWavelet, NULL);
				}
				cvSetImageCOI(pWavelet, 0);
				cvReleaseImage(&pImage);
			}
			// 小波变换图象
			cvSetImageROI(pWavelet, cvRect(0, 0, pSrc->width, pSrc->height));
			cvConvertScale(pWavelet, pSrc, 1, 128);
			cvResetImageROI(pWavelet); // 本行代码有点多余，但有利用养成良好的编程习惯
			cvReleaseImage(&pWavelet);
		}
		/*
		// 显示图像pSrc
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