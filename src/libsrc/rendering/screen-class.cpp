#include "screen-class.h"

Screen::Screen()
{
	internalBuffer = NULL;
}

Screen::~Screen()
{
	if(internalBuffer != NULL)
	{
		delete internalBuffer;
		internalBuffer = NULL;
	}
}

Screen::Screen(ImageInfo inf, LPDIRECTDRAWSURFACE7 extBuffer, double x_, double y_, double w, double h)
{
	info = inf;
	externalBuffer = extBuffer;
	internalBuffer = NULL;
	x = x_;
	y = y_;
	width = w;
	height = h;
	secondsToWait = 0.0;
	waitForKeyPress = no;
	init();
}

void Screen::init()
{
	int bmpWidth,bmpHeight;
	internalBuffer = RE->createBitmapSurface(info.getPath().c_str(), &bmpWidth, &bmpHeight);
}

void Screen::reinit(ImageInfo inf)
{
	info = inf;
	if(internalBuffer != NULL)
	{
		DeleteObject(internalBuffer);
		internalBuffer = NULL;
	}
	init();
}

void Screen::setX(double x_)
{
	x = x_;
}

double Screen::getX()
{
	return x;
}

void Screen::setY(double y_)
{
	y = y_;
}

double Screen::getY()
{
	return y;
}

void Screen::setWidth(double w)
{
	width = w;
}

double Screen::getWidth()
{
	return width;
}

void Screen::setHeight(double h)
{
	height = h;
}

double Screen::getHeight()
{
	return height;
}

void Screen::setExternalBuffer(LPDIRECTDRAWSURFACE7 extBuffer)
{
	externalBuffer = extBuffer;
}

LPDIRECTDRAWSURFACE7 Screen::getExternalBuffer()
{
	return externalBuffer;
}

void Screen::wait()
{
	if(isWaitForkeyPress())
	{
		Logical looping = yes;
		while(looping)
		{
			if(((GetAsyncKeyState(VK_RETURN)&0x8000)?1:0))
			{
				looping = no;
			}
		}
	}
	else
	{
		Sleep(secondsToWait * 1000);
	}
}

void Screen::setSecondsToWait(double secs)
{
	secondsToWait = secs;
}

double Screen::getSecondsToWait()
{
	return secondsToWait;
}

void Screen::setWaitForKeyPress(Logical wait)
{
	waitForKeyPress = wait;
}

Logical Screen::isWaitForkeyPress()
{
	return waitForKeyPress;
}

void Screen::display()
{
	if(internalBuffer != NULL && externalBuffer != NULL)
	{
		RECT dstRct;

		dstRct.top = y;
		dstRct.left = x;
		dstRct.right = dstRct.left + getWidth();
		dstRct.bottom = dstRct.top + getHeight();

		//render from internal buffer to external buffer
		externalBuffer->Blt(&dstRct, internalBuffer, NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, &RE->ddbltfx);
	}
}
