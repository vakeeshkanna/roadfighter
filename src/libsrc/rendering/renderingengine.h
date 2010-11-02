#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include "string-class.h"
#include "log-class.h"
#include "imageinfo.h"
#include <ddraw.h>
#include <string>
#define SafeRelease(x) if(x) {x->Release();x = NULL;}

//Color codes
#define C_WHITE 0xFFFFFF
#define C_BLACK 0x000000
#define C_RED 0xFF0000
#define C_GREEN 0x00FF00
#define C_BLUE 0x0000FF
#define C_MAGENTA 0xDECD48


//COLORREF
#define CREF_WHITE RGB(255,255,255)
#define CREF_BLACK RGB(0,0,0)
#define CREF_RED RGB(255,0,0)
#define CREF_GREEN RGB(0,254,0)
#define CREF_BLUE RGB(0,0,255)



bool bExclusive = true;

//Global Handle to RenderingEngine
#define RE RenderingEngine::getInstance()

class EXPORT RenderingEngine
{
	LPDIRECTDRAWSURFACE7 DDrawPrimary;
	LPDIRECTDRAWSURFACE7 DDrawBack;
	LPDIRECTDRAWSURFACE7 DDrawFront;

	//Temporary hold for display
	LPDIRECTDRAWSURFACE7 DDrawTemp2;

	LPDIRECTDRAWCLIPPER DDrawClip;

	friend class Object;
	friend class Stage;
	friend class Viewport;
	friend class Track;
	friend class BackGround;
	friend class RoadFighter;

public:
	LPDIRECTDRAW7 DDraw;
	static RenderingEngine* rEngine;
	int screenWidth,screenHeight;
	int screenOffsetX,screenOffsetY;

	RenderingEngine();
	~RenderingEngine();
	void cleanUp();
	int init(HWND);
	int initWindowed(HWND);
	void setScreenDimensions(int width,int height);
	int getScreenWidth();
	int getScreenHeight();

	LPDIRECTDRAWSURFACE7 createBitmapSurface(String,int *w = NULL,int *h = NULL);
	LPDIRECTDRAWSURFACE7 createEmptySurface(int,int);
	void flipBuffers(HWND hWnd);
	void tempToFront();
	void clearBuffer(LPDIRECTDRAWSURFACE7 buffer);
	void clearAllInternalBuffer();
	void fillBuffer(LPDIRECTDRAWSURFACE7 buffer,int colorCode);
	static RenderingEngine* getInstance();
	LPDIRECTDRAWSURFACE7 getPrimary();
	LPDIRECTDRAWCLIPPER DDrawAttachClipper(LPDIRECTDRAWSURFACE7 lpdds, int numRects, LPRECT clip_list);

	static void outputText(LPDIRECTDRAWSURFACE7 surface, int x,int y,int color,char* str,int fontWidth,int fontHeight, int bgcolor = C_BLACK);
	static void outputText(LPDIRECTDRAWSURFACE7 surface, int x,int y,int color,int value,int fontWidth,int fontHeight, int bgcolor = C_BLACK);
	static void outputText(LPDIRECTDRAWSURFACE7 surface, int x, int y,int color,double value,int fontWidth,int fontHeight, int bgcolor = C_BLACK);
	static void outputText(LPDIRECTDRAWSURFACE7 surface, int x,int y,int color,int value,int fontWidth,int fontHeight,char* strFormat, int bgcolor = C_BLACK);
	static void outputText(LPDIRECTDRAWSURFACE7 surface, int x,int y,int color,string str,int fontWidth,int fontHeight, int bgcolor = C_BLACK);
	static void plotPixel(LPDIRECTDRAWSURFACE7 surface, int x,int y,int color);
	static void drawLine(LPDIRECTDRAWSURFACE7 surface, int x0,int y0, int x1, int y1, int color);

	//Required for Blt function
	DDBLTFX ddbltfx;
	// key used for transparent blitting
	DDCOLORKEY key;
};

#endif
