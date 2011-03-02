#include "SeamCarveResize.h"
void SeamCarve::neoBrightGradient(const IplImage *src,IplImage *gra){
	for (int h=0;h<src->height-1;h++)
		for (int w=0;w<src->width-1;w++)
			getTiduG(h,w)=(abs(getTiduB(0,0)-getTiduB(0,1))+abs(getTiduB(0,0)-getTiduB(1,0)))/3;
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
	j=-1;int jmax = 0;
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
		i = img->width - 1;
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
	j=-1;int jmax = 0;
	for (i=0;i<hh;i++)
	{
		if (getPointImg32S(sum,i,img->height-1) < k){
			k = getPointImg32S(sum,i,height-1);
			j = i;
		}
		if(getPointImg32S(sum,i,img->height-1) > z)
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
