#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "cv.h"
#include "highgui.h"
class ImageProcessor
{
	IplImage* img;//
	char * titleName;
public:
	ImageProcessor(const char * filename,bool display = true);

	ImageProcessor(const IplImage* img,const char * title,bool display = true);
	
	~ImageProcessor();

	IplImage* getImg();
		
	void setTitleName(const char * title);
};

//extern ImageProcessor * proc = NULL;
#endif
