#include "Log.h"
#include "SeamCarveResize.h"
#include "ImageProcessor.h"
#include <winbase.h>
#include <mmsystem.h>
#include <map>

//tiemGetTime()调用
#pragma comment(lib,"winmm.lib")

using namespace std;

void SeamCarve::neoBrightGradient(const IplImage *src,IplImage *gra){
	for (int h=0;h<src->height-1;h++)
		for (int w=0;w<src->width-1;w++)
			//按照论文应再除以max(HOG(I(X,Y))),这个为常量在判断时可以忽略
			getTiduG(h,w)=(abs(getTiduB(0,0)-getTiduB(0,1))+abs(getTiduB(0,0)-getTiduB(1,0)))/3;
}

//另一种方法：一点的值=8邻域的值之和+自身/9？
void SeamCarve::neoBrightAvgGradient(const IplImage *src,IplImage *gra){
	IplImage *tempGra = cvCreateImage(cvGetSize(gra),gra->depth,gra->nChannels);
	for (int h=0;h<src->height-1;h++)
		for (int w=0;w<src->width-1;w++)
		{
			getTiduEnergy(h,w)=(abs(getTiduB(0,0)-getTiduB(0,1))+abs(getTiduB(0,0)-getTiduB(1,0)))/3;
		}
	for(int i=1;i<gra->height-1;i++)
	{
		for(int j=1;j<gra->width-1;j++)
		{
			getTiduG(i,j) = (getTiduEnergy(i,j) + getTiduEnergy(i-1,j-1) + getTiduEnergy(i-1,j) + getTiduEnergy(i-1,j+1)
				+ getTiduEnergy(i,j-1) +getTiduEnergy(i,j+1) + getTiduEnergy(i+1,j-1) + getTiduEnergy(i+1,j) + getTiduEnergy(i+1,j+1))/9;
		}
	}
	cvReleaseImage(&tempGra);
}

void SeamCarve::neoLaplaceGradient(const IplImage* src,IplImage *pImg8u)
{
	IplImage *pImage = NULL;
	IplImage *pImgLaplace = NULL;//用于灰度图像Sobel转换
	//IplImage *pImg8u = NULL;//用于图像格式转换
	IplImage *pImg8uSmooth = NULL;//存储平滑后的图像
	IplImage *pImgColor = NULL;//用于Sobel变换
	IplImage *pImgSobelColor = NULL;//用于彩色Sobel变换
	
	pImage = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
	
	cvCopy(src,pImage);
	
	pImg8uSmooth = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U,1);
	
	cvCvtColor(pImage,pImg8u,CV_BGR2GRAY);
	
	cvSmooth(pImg8u,pImg8uSmooth,CV_GAUSSIAN,3,0,0);
	
	pImgLaplace = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_16S,1);
	
	cvLaplace(pImg8uSmooth,pImgLaplace,3);
	
	cvConvertScaleAbs(pImgLaplace,pImg8u,1,0);
	
	cvReleaseImage(&pImgSobelColor);
	cvReleaseImage(&pImgColor);
	cvReleaseImage(&pImg8uSmooth);
	cvReleaseImage(&pImgLaplace);
	cvReleaseImage(&pImage);
}

void SeamCarve::neoSobelGradient(const IplImage *src,IplImage *pImg8u)
{
	IplImage *pImage = NULL;
	IplImage *pImgSobelgray = NULL;//用于灰度图像Sobel转换
	//IplImage *pImg8u = NULL;//用于图像格式转换
	IplImage *pImg8uSmooth = NULL;//存储平滑后的图像
	IplImage *pImgColor = NULL;//用于Sobel变换
	IplImage *pImgSobelColor = NULL;//用于彩色Sobel变换

	pImage = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);

	cvCopy(src,pImage);

	pImg8uSmooth = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U,1);

	cvCvtColor(pImage,pImg8u,CV_BGR2GRAY);

	cvSmooth(pImg8u,pImg8uSmooth,CV_GAUSSIAN,3,0,0);

	pImgSobelgray = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_16S,1);

	cvSobel(pImg8uSmooth,pImgSobelgray,0,1,3);

	cvConvertScaleAbs(pImgSobelgray,pImg8u,1,0);
	
	cvReleaseImage(&pImgSobelColor);
	cvReleaseImage(&pImgColor);
	cvReleaseImage(&pImg8uSmooth);
	cvReleaseImage(&pImgSobelgray);
	cvReleaseImage(&pImage);
}

void SeamCarve::neoCannyGradient(const IplImage* src,IplImage *pImg8u)
{
	IplImage *pImage = NULL;
	IplImage *pImgCanny = NULL;//用于灰度图像Canny转换
	//IplImage *pImg8u = NULL;//用于图像格式转换
	IplImage *pImg8uSmooth = NULL;//存储平滑后的图像
	
	pImage = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
	
	cvCopy(src,pImage);
	
	pImg8uSmooth = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U,1);
	
	pImgCanny = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U,1);	
	
	cvCvtColor(pImage,pImg8u,CV_BGR2GRAY);
	
	cvSmooth(pImg8u,pImg8uSmooth,CV_GAUSSIAN,3,0,0);
	
	cvCanny(pImg8uSmooth,pImgCanny,100,200,3);
	
	cvConvertScaleAbs(pImgCanny,pImg8u,1,0);

	for (int i=0;i<pImg8u->width;i++)
	{
		for (int j=0;j<pImg8u->height;j++)
		{
			getPointImg8U(pImg8u,i,j) = abs(255 - getPointImg8U(pImg8u,i,j));
		}
	}
	
	cvReleaseImage(&pImg8uSmooth);

	cvReleaseImage(&pImgCanny);

	cvReleaseImage(&pImage);
}
int SeamCarve::pomin(int x,int y,int z){
	if ((x==maxint)&&(y==maxint)&&(z==maxint))
		return 2;
	else
		if (x<y)
			if (x<z) return -1;
			else return 1;
		else
			if (y<z) return 0;
			else return 1;
}

void SeamCarve::nSeamCarving(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis){

	//路径
	IplImage *seam = cvCreateImage(cvGetSize(img),IPL_DEPTH_8S,1);
	//梯度和
	IplImage *sum = cvCreateImage(cvGetSize(img),IPL_DEPTH_32S,1);
	cvZero(seam);
	cvZero(sum);
	int hh = img->height;
	if (hh<=1) return;
	int i,j,k;
	for(i=0;i<hh;i++)
		getPointImg32S(sum,0,i)=getPointImg8U(gra,0,i);
		
	//算法核心
	for(i=1;i<img->width;i++)
		for(j=0;j<hh;j++)
			if(getPointImg8U(mask,i,j) == 0){
				k = pomin(
					((j==0)||getPointImg8U(mask,i-1,j-1))?
						maxint:getPointImg32S(sum,i-1,j-1),
					getPointImg8U(mask,i-1,j)?
						maxint:getPointImg32S(sum,i-1,j),
					((j==hh-1)||getPointImg8U(mask,i-1,j+1))?
						maxint:getPointImg32S(sum,i-1,j+1)
				);
				if (k==2) getPointImg32S(sum,i,j)=maxint;
				else{
					getPointImg8S(seam,i,j)=k;
					getPointImg32S(sum,i,j)=getPointImg32S(sum,i-1,j+k)+getPointImg8U(gra,i,j);
					if (getPointImg8U(mask2,i,j)) getPointImg32S(sum,i,j)-=valForDelete;
				}
			}
			else getPointImg32S(sum,i,j)=maxint;
	//找到最小的sum
	k=maxint;
	j=-1;
	
	for (i=0;i<hh;i++)
			if (getPointImg32S(sum,img->width-1,i) < k){
				k = getPointImg32S(sum,img->width-1,i);
				j = i;
			}
		
	//按路径压缩图像
	if (k<maxint){
	i = img->width - 1;
	while(i>=0){
		//cvCircle(img,cvPoint(i,j),5,CV_RGB(200,200,200),5);
		//nMove(i,j,img,mask,gra,dis)>>>>>>>>>>>>>>>>
		for (k=j;k<hh-1;k++){
			getPointImg8U(mask,i,k)=getPointImg8U(mask,i,k+1);
			getPointImg8U(mask2,i,k)=getPointImg8U(mask2,i,k+1);
			getPointImg8U(gra,i,k)=getPointImg8U(gra,i,k+1);
			getPointImg8U3C(img,i,k,0)=getPointImg8U3C(img,i,k+1,0);
			getPointImg8U3C(img,i,k,1)=getPointImg8U3C(img,i,k+1,1);
			getPointImg8U3C(img,i,k,2)=getPointImg8U3C(img,i,k+1,2);
			getPointImg8U3C(dis,i,k,0)=getPointImg8U3C(dis,i,k+1,0);
			getPointImg8U3C(dis,i,k,1)=getPointImg8U3C(dis,i,k+1,1);
			getPointImg8U3C(dis,i,k,2)=getPointImg8U3C(dis,i,k+1,2);			
		}
		j += getPointImg8S(seam,i,j);
		i--;
		}
		img->height--;
		mask->height--;
		mask2->height--;
		gra->height--;
		dis->height--;
		//附注：原来IplImage中的imageSize和widthStep都是不用我们修改的：）
	}	
	//释放图像
	cvReleaseImage(&seam);
	cvReleaseImage(&sum);
}
//上面函数的转置!!
void SeamCarve::nSeamCarvingVertical(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis){
	IplImage *seam = cvCreateImage(cvGetSize(img),IPL_DEPTH_8S,1);
	IplImage *sum = cvCreateImage(cvGetSize(img),IPL_DEPTH_32S,1);
	cvZero(seam);
	cvZero(sum);
	int hh = img->width;
	if (hh<=100) return;
	int i,j,k;
	for(i=0;i<hh;i++)
		getPointImg32S(sum,i,0)=getPointImg8U(gra,i,0);
	for(i=1;i<img->height;i++)
		for(j=0;j<hh;j++)
			if(getPointImg8U(mask,j,i) == 0){
				k = pomin(
					((j==0)||getPointImg8U(mask,j-1,i-1))?
						maxint:getPointImg32S(sum,j-1,i-1),
					getPointImg8U(mask,j,i-1)?
						maxint:getPointImg32S(sum,j,i-1),
					((j==hh-1)||getPointImg8U(mask,j+1,i-1))?
						maxint:getPointImg32S(sum,j+1,i-1)
				);
				if (k==2) getPointImg32S(sum,j,i)=maxint;
				else{
					getPointImg8S(seam,j,i)=k;
					getPointImg32S(sum,j,i)=getPointImg32S(sum,j+k,i-1)+getPointImg8U(gra,j,i);
					if (getPointImg8U(mask2,j,i)) getPointImg32S(sum,j,i)-=valForDelete;
				}
			}
			else getPointImg32S(sum,j,i)=maxint;
	k=maxint;
	j=-1;
	for (i=0;i<hh;i++)
		if (getPointImg32S(sum,i,img->height-1) < k){
			k = getPointImg32S(sum,i,img->height-1);
			j = i;
		}
	if (k<maxint){
		i = img->height - 1;
		while(i>=0){
			for (k=j;k<hh-1;k++){
				getPointImg8U(mask,k,i)=getPointImg8U(mask,k+1,i);
				getPointImg8U(mask2,k,i)=getPointImg8U(mask2,k+1,i);
				getPointImg8U(gra,k,i)=getPointImg8U(gra,k+1,i);
				getPointImg8U3C(img,k,i,0)=getPointImg8U3C(img,k+1,i,0);
				getPointImg8U3C(img,k,i,1)=getPointImg8U3C(img,k+1,i,1);
				getPointImg8U3C(img,k,i,2)=getPointImg8U3C(img,k+1,i,2);
				getPointImg8U3C(dis,k,i,0)=getPointImg8U3C(dis,k+1,i,0);
				getPointImg8U3C(dis,k,i,1)=getPointImg8U3C(dis,k+1,i,1);
				getPointImg8U3C(dis,k,i,2)=getPointImg8U3C(dis,k+1,i,2);			
			}
			j += getPointImg8S(seam,j,i);
			i--;
		}
		img->width--;
		mask->width--;
		mask2->width--;
		gra->width--;
		dis->width--;
	}
	cvReleaseImage(&seam);
	cvReleaseImage(&sum);
}

void SeamCarve::nSeamCarvingLarge(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width)
{
	//路径
	IplImage *seam = cvCreateImage(cvSize(width,height),IPL_DEPTH_8S,1);
	//梯度和
	IplImage *sum = cvCreateImage(cvSize(width,height),IPL_DEPTH_32S,1);
	cvZero(seam);
	cvZero(sum);
	int hh = height;
	if (hh<=1) return;
	int i,j,k;
	for(i = hh-1 ;i>=0;i--)
		getPointImg32S(sum,0,i)=getPointImg8U(gra,0,i);
	//算法核心
	for(i=1;i<width;i++)
		for(j=0;j<hh;j++)
			if(getPointImg8U(mask,i,j) == 0){
				k = pomin(
					((j==0)||getPointImg8U(mask,i-1,j-1))?
						maxint:getPointImg32S(sum,i-1,j-1),
					getPointImg8U(mask,i-1,j)?
						maxint:getPointImg32S(sum,i-1,j),
					((j==hh-1)||getPointImg8U(mask,i-1,j+1))?
						maxint:getPointImg32S(sum,i-1,j+1)
				);
				if (k==2) getPointImg32S(sum,i,j)=maxint;
				else{
					getPointImg8S(seam,i,j)=k;
					getPointImg32S(sum,i,j)=getPointImg32S(sum,i-1,j+k)+getPointImg8U(gra,i,j);
					if (getPointImg8U(mask2,i,j)) getPointImg32S(sum,i,j)-=valForDelete;
				}
			}
			else getPointImg32S(sum,i,j)=maxint;
	//找到最小的sum
	k=maxint;int z = 0;
	j=-1;int jmax = -1;
	for (i=0;i<hh;i++)
	{
		if (getPointImg32S(sum,width-1,i) < k){
			k = getPointImg32S(sum,width-1,i);
			j = i;
		}
		if(getPointImg32S(sum,width-1,i) > z)
		{
			z = getPointImg32S(sum,width-1,i);
			jmax = i;
		}
	}
	
	//按路径压缩图像
	if (k<maxint){
		i = width - 1;
		while(i>=0){
			getPointImg8U(gra,i,j) = getPointImg8U(gra,i,jmax);
			for (k=hh;k>j;k--)
			{
				getPointImg8U(mask,i,k)=getPointImg8U(mask,i,k-1);
				getPointImg8U(mask2,i,k)=getPointImg8U(mask2,i,k-1);
				getPointImg8U(gra,i,k)=getPointImg8U(gra,i,k-1);
				getPointImg8U3C(img,i,k,0)=getPointImg8U3C(img,i,k-1,0);
				getPointImg8U3C(img,i,k,1)=getPointImg8U3C(img,i,k-1,1);
				getPointImg8U3C(img,i,k,2)=getPointImg8U3C(img,i,k-1,2);
				getPointImg8U3C(dis,i,k,0)=getPointImg8U3C(dis,i,k-1,0);
				getPointImg8U3C(dis,i,k,1)=getPointImg8U3C(dis,i,k-1,1);
				getPointImg8U3C(dis,i,k,2)=getPointImg8U3C(dis,i,k-1,2);			
			}
			j += getPointImg8S(seam,i,j);
			i--;
		}
	}
	cvReleaseImage(&seam);
	cvReleaseImage(&sum);
}
void SeamCarve::nSeamCarvingLargeVertical(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width)
{
	IplImage *seam = cvCreateImage(cvSize(width,height),IPL_DEPTH_8S,1);
	IplImage *sum = cvCreateImage(cvSize(width,height),IPL_DEPTH_32S,1);
	cvZero(seam);
	cvZero(sum);
	int hh = width;
	if (hh<=100) return;
	int i,j,k;
	for(i=0;i<hh;i++)
		getPointImg32S(sum,i,0)=getPointImg8U(gra,i,0);
	for(i=1;i<height;i++)
		for(j=0;j<hh;j++)
			if(getPointImg8U(mask,j,i) == 0){
				k = pomin(
					((j==0)||getPointImg8U(mask,j-1,i-1))?
						maxint:getPointImg32S(sum,j-1,i-1),
					getPointImg8U(mask,j,i-1)?
						maxint:getPointImg32S(sum,j,i-1),
					((j==hh-1)||getPointImg8U(mask,j+1,i-1))?
						maxint:getPointImg32S(sum,j+1,i-1)
				);
				if (k==2) getPointImg32S(sum,j,i)=maxint;
				else{
					getPointImg8S(seam,j,i)=k;
					getPointImg32S(sum,j,i)=getPointImg32S(sum,j+k,i-1)+getPointImg8U(gra,j,i);
					if (getPointImg8U(mask2,j,i)) getPointImg32S(sum,j,i)-=valForDelete;
				}
			}
			else getPointImg32S(sum,j,i)=maxint;
	k=maxint;int z=0;
	j=-1;int jmax = -1;
	for (i=0;i<hh;i++)
	{
		if (getPointImg32S(sum,i,height-1) < k){
			k = getPointImg32S(sum,i,height-1);
			j = i;
		}
		if(getPointImg32S(sum,i,height-1) > z)
		{
			z = getPointImg32S(sum,i,height-1);
			jmax = i;
		}
	}
	if (k<maxint){
		i = height - 1;
		while(i>=0){
			getPointImg8U(gra,j,i) = getPointImg8U(gra,jmax,i);
			for (k=hh;k>j;k--){
				getPointImg8U(mask,k,i)=getPointImg8U(mask,k-1,i);
				getPointImg8U(mask2,k,i)=getPointImg8U(mask2,k-1,i);
				getPointImg8U(gra,k,i)=getPointImg8U(gra,k-1,i);
				getPointImg8U3C(img,k,i,0)=getPointImg8U3C(img,k-1,i,0);
				getPointImg8U3C(img,k,i,1)=getPointImg8U3C(img,k-1,i,1);
				getPointImg8U3C(img,k,i,2)=getPointImg8U3C(img,k-1,i,2);
				getPointImg8U3C(dis,k,i,0)=getPointImg8U3C(dis,k-1,i,0);
				getPointImg8U3C(dis,k,i,1)=getPointImg8U3C(dis,k-1,i,1);
				getPointImg8U3C(dis,k,i,2)=getPointImg8U3C(dis,k-1,i,2);			
			}
			j += getPointImg8S(seam,j,i);
			i--;
		}
	}
	cvReleaseImage(&seam);
	cvReleaseImage(&sum);
}


void SeamCarve::nSeamCarving(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width)
{	
	//路径
	IplImage *seam = cvCreateImage(cvGetSize(img),IPL_DEPTH_8S,1);
	//梯度和
	IplImage *sum = cvCreateImage(cvGetSize(img),IPL_DEPTH_32S,1);
	cvZero(seam);
	cvZero(sum);
	int hh = img->height;
	if (hh<=1) return;
	int i,j,k;
	for(i=0;i<hh;i++)
		getPointImg32S(sum,0,i)=getPointImg8U(gra,0,i);
		
	//算法核心
	for(i=1;i<img->width;i++)
		for(j=0;j<hh;j++)
			if(getPointImg8U(mask,i,j) == 0){
				k = pomin(
					((j==0)||getPointImg8U(mask,i-1,j-1))?
						maxint:getPointImg32S(sum,i-1,j-1),
					getPointImg8U(mask,i-1,j)?
						maxint:getPointImg32S(sum,i-1,j),
					((j==hh-1)||getPointImg8U(mask,i-1,j+1))?
						maxint:getPointImg32S(sum,i-1,j+1)
				);
				if (k==2) getPointImg32S(sum,i,j)=maxint;
				else{
					getPointImg8S(seam,i,j)=k;
					getPointImg32S(sum,i,j)=getPointImg32S(sum,i-1,j+k)+getPointImg8U(gra,i,j);
					if (getPointImg8U(mask2,i,j)) getPointImg32S(sum,i,j)-=valForDelete;
				}
			}
			else getPointImg32S(sum,i,j)=maxint;
	//找到最小的sum
	k=maxint;
	j=-1;
	multimap<int,int> sumMap;int m =0;
	multimap<int,int> tmpMap;int n =0;
	for (i=0;i<hh;i++)
			if (getPointImg32S(sum,img->width-1,i) < k){
				k = getPointImg32S(sum,img->width-1,i);
				j = i;
			}
	for (i=0;i<hh;i++)
	{
		k = getPointImg32S(sum,img->width-1,i);		
		sumMap.insert(make_pair(k,i));
	}
	/*
	int ij = 0;
		int mn = 0;
		int nT = 0;
		for(multimap<int,int>::const_iterator iter = sumMap.end();(iter!=sumMap.begin()&ij<sumMap.size());--iter)
		{
			if (ij>0)
			{
				
				m = iter->first;
							n = iter->second;
							nT = n;
							for (multimap<int,int>::const_iterator iter2 = sumMap.begin();iter2!=iter;++iter2)
							{
								mn = iter2->second;
								if(mn<nT)
								{
									n--;
								}
							}
				
				tmpMap.insert(make_pair(m,n));
			}
			ij++;
		}*/
		
	int h_to_seam = hh - height;
	for (multimap<int,int>::const_iterator iter3 = sumMap.begin();(iter3!=sumMap.end()&&(h_to_seam>0));++iter3)
	{
		j = iter3->second;
		//按路径压缩图像
		if (k<maxint){
		i = img->width - 1;
		while(i>=0){
			//cvCircle(img,cvPoint(i,j),5,CV_RGB(200,200,200),5);
			//nMove(i,j,img,mask,gra,dis)>>>>>>>>>>>>>>>>
			for (k=j;k<hh-1;k++){
				getPointImg8U(mask,i,k)=getPointImg8U(mask,i,k+1);
				getPointImg8U(mask2,i,k)=getPointImg8U(mask2,i,k+1);
				getPointImg8U(gra,i,k)=getPointImg8U(gra,i,k+1);
				getPointImg8U3C(img,i,k,0)=getPointImg8U3C(img,i,k+1,0);
				getPointImg8U3C(img,i,k,1)=getPointImg8U3C(img,i,k+1,1);
				getPointImg8U3C(img,i,k,2)=getPointImg8U3C(img,i,k+1,2);
				getPointImg8U3C(dis,i,k,0)=getPointImg8U3C(dis,i,k+1,0);
				getPointImg8U3C(dis,i,k,1)=getPointImg8U3C(dis,i,k+1,1);
				getPointImg8U3C(dis,i,k,2)=getPointImg8U3C(dis,i,k+1,2);			
			}
			j += getPointImg8S(seam,i,j);
			i--;
			}
			img->height--;
			mask->height--;
			mask2->height--;
			gra->height--;
			dis->height--;
			//附注：原来IplImage中的imageSize和widthStep都是不用我们修改的：）
		}
		h_to_seam --;
	}
	//释放图像
	cvReleaseImage(&seam);
	cvReleaseImage(&sum);
}