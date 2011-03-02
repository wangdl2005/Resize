#ifndef SEAM_CAVE_RESIZE_H
#define SEAM_CAVE_RESIZE_H
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include <stdio.h>
#include <string.h>

#define maxint 2139062143
//Object Removal的参数，太大有可能溢出，小了效果不好
#define valForDelete 0x400
#define abs(x) ((x)<0?-(x):(x))

//和计算梯度有关的一些函数
#define getTiduS(a,b,c) ((uchar)*(src->imageData+(a)*src->widthStep+(b)*3+(c)))
#define getTiduB(a,b) (getTiduS(h+(a),w+(b),0)+getTiduS(h+(a),w+(b),1)+getTiduS(h+(a),w+(b),2))
#define getTiduG(a,b) (*(gra->imageData+(a)*gra->widthStep+(b)))

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
	IplImage *src,*img,*mask,*mask2,*gra,*dis;

public:
	//求梯度（按亮度计算）ps：注意!!!这个函数少算了右边缘一行和下边缘一行，请注意初始化。。。
	static void neoBrightGradient(const IplImage *,IplImage *);
	/*核心函数nSeamCarving / nSeamCarvingVertical：目标img(结果)，遮罩mask（保护用），遮罩mask2（删除用），梯度gra(需给的参数)，显示遮罩用图dis
	如果成功找到路径，所有图被压缩一行(一列)*/
	static void nSeamCarving(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);
	static void nSeamCarvingVertical(IplImage *img,IplImage *mask,IplImage *mask2, IplImage *gra,IplImage *dis);
	//找最小值的位置：-1、0、1、2(error)
	static int pomin(int x,int y,int z);
	
	static void nSeamCarvingLarge(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);
	//height,width表示当前img所需处理的范围
	static void nSeamCarvingLarge(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width);
	
	static void nSeamCarvingLargeVertical(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);

	static void nSeamCarvingLargeVertical(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width);
};

#endif