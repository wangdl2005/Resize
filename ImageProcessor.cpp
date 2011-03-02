#include "ImageProcessor.h"
ImageProcessor::ImageProcessor(const char * filename,bool display /* = true */)
{
	img = cvLoadImage(filename);//

	titleName = "ԭʼͼƬ";	

	if(display)
	{
		//��������
		cvNamedWindow(ImageProcessor::titleName,CV_WINDOW_AUTOSIZE);
		//��ʾͼƬ
		cvShowImage(ImageProcessor::titleName,img);
	}
}
ImageProcessor::ImageProcessor(const IplImage* image,const char * title,bool display /* = true */)
{
	img = cvCreateImage(cvSize(image->width,image->height),image->depth,image->nChannels);

	cvCopy(image,img);

	titleName = (char *) title;

	if(display)
	{
		//��������
		cvNamedWindow(ImageProcessor::titleName,CV_WINDOW_AUTOSIZE);
		//��ʾͼƬ
		cvShowImage(ImageProcessor::titleName,img);
	}
}
ImageProcessor::~ImageProcessor()
{
	cvReleaseImage(&img);	
	//delete titleName;
}
IplImage* ImageProcessor::getImg()
{
	return ImageProcessor::img;
}
void ImageProcessor::setTitleName(const char * title)
{
	ImageProcessor::titleName = (char *)title;
}
