#ifndef SEAM_CAVE_RESIZE_H
#define SEAM_CAVE_RESIZE_H
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include <stdio.h>
#include <string.h>

#define maxint 2139062143
//Object Removal�Ĳ�����̫���п��������С��Ч������
#define valForDelete 0x400
#define abs(x) ((x)<0?-(x):(x))

//�ͼ����ݶ��йص�һЩ����
#define getTiduS(a,b,c) ((uchar)*(src->imageData+(a)*src->widthStep+(b)*3+(c)))
#define getTiduB(a,b) (getTiduS(h+(a),w+(b),0)+getTiduS(h+(a),w+(b),1)+getTiduS(h+(a),w+(b),2))
#define getTiduG(a,b) (*(gra->imageData+(a)*gra->widthStep+(b)))

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
	IplImage *src,*img,*mask,*mask2,*gra,*dis;

public:
	//���ݶȣ������ȼ��㣩ps��ע��!!!��������������ұ�Եһ�к��±�Եһ�У���ע���ʼ��������
	static void neoBrightGradient(const IplImage *,IplImage *);
	/*���ĺ���nSeamCarving / nSeamCarvingVertical��Ŀ��img(���)������mask�������ã�������mask2��ɾ���ã����ݶ�gra(����Ĳ���)����ʾ������ͼdis
	����ɹ��ҵ�·��������ͼ��ѹ��һ��(һ��)*/
	static void nSeamCarving(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);
	static void nSeamCarvingVertical(IplImage *img,IplImage *mask,IplImage *mask2, IplImage *gra,IplImage *dis);
	//����Сֵ��λ�ã�-1��0��1��2(error)
	static int pomin(int x,int y,int z);
	
	static void nSeamCarvingLarge(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);
	//height,width��ʾ��ǰimg���账��ķ�Χ
	static void nSeamCarvingLarge(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width);
	
	static void nSeamCarvingLargeVertical(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis);

	static void nSeamCarvingLargeVertical(IplImage *img,IplImage *mask,IplImage *mask2,IplImage *gra,IplImage *dis,int height,int width);
};

#endif