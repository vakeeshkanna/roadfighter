#include "imageinfo.h"

ImageInfo::ImageInfo(string n, string fp, string fn,int nf, int csx)
{
	path = fp + fn;
	name = n;
	numFrames = nf;
	cellSizeX = csx;
	cellSizeY = csx;
}

ImageInfo::ImageInfo(string n, string fp, string fn, int nf, int csx, int csy)
{
	path = fp + fn;
	name = n;
	numFrames = nf;
	cellSizeX = csx;
	cellSizeY = csy;
}

ImageInfo::ImageInfo(string n, string fp, string fn)
{
	path = fp + fn;
	name = n;
}

ImageInfo::ImageInfo(string n, string fp, string fn, int nf)
{
	path = fp + fn;
	name = n;
	numFrames = nf;
}

ImageInfo::ImageInfo()
{
}

ImageInfo::~ImageInfo()
{
	
}

void ImageInfo::setCellSizeX(int cs)
{
	cellSizeX = cs;
}

int ImageInfo::getCellSizeX()
{
	return cellSizeX;
}

void ImageInfo::setCellSizeY(int cs)
{
	cellSizeY = cs;
}

int ImageInfo::getCellSizeY()
{
	return cellSizeY;
}

void ImageInfo::setNumFrames(int nf)
{
	numFrames = nf;
}

int ImageInfo::getNumFrames()
{
	return numFrames;
}

void ImageInfo::setName(string n)
{
	name = n;
}

string ImageInfo::getName()
{
	return name;
}


void ImageInfo::setPath(string p)
{
	path = p;
}

string ImageInfo::getPath()
{
	return path;
}
