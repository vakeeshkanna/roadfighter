#ifndef SCREENCLASS_H
#define SCREENCLASS_H

#include <string.h>
#include "imageinfo.h"
#include "basedefs.h"
//#include "gameframe.h"



class EXPORT Screen
{
public:
	Screen();
	Screen(ImageInfo info, LPDIRECTDRAWSURFACE7 externalBuffer, double x, double y, double width, double height);
	~Screen();
	void init();
	void reinit(ImageInfo info);
	void display();
	void setX(double x);
	double getX();
	void setY(double y);
	double getY();
	void setWidth(double width);
	double getWidth();
	void setHeight(double height);
	double getHeight();
	void wait();
	void setSecondsToWait(double sec);
	double getSecondsToWait();
	void setWaitForKeyPress(Logical wait);
	Logical isWaitForkeyPress();
	void setExternalBuffer(LPDIRECTDRAWSURFACE7 extBuffer);
	LPDIRECTDRAWSURFACE7 getExternalBuffer();

private:
	double x,y;
	double width, height;
	double secondsToWait;
	Logical waitForKeyPress;
	ImageInfo info;
	LPDIRECTDRAWSURFACE7 internalBuffer, externalBuffer;
	//GameFrame waitTimer;
};
#endif
