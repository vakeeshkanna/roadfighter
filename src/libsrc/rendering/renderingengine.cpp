#include "renderingengine.h"
#include "basedefs.h"

RenderingEngine::RenderingEngine()
{
	DDraw = NULL;
	DDrawPrimary = NULL;
	DDrawBack = NULL;
	DDrawFront = NULL;
}

RenderingEngine::~RenderingEngine()
{
	cleanUp();
}

void RenderingEngine::cleanUp()
{
	lprintf("cleaning up\n");
//	if(DDrawPrimary != NULL)
		SafeRelease(DDrawBack);
//	if(DDrawBack != NULL)
		SafeRelease(DDrawPrimary);
//	if(DDrawClip != NULL)
		SafeRelease(DDrawClip);
//	if(DDraw != NULL)
		SafeRelease(DDraw);
}

void RenderingEngine::tempToFront()
{

}

RenderingEngine* RenderingEngine::rEngine = NULL;

RenderingEngine* RenderingEngine::getInstance()
{
	if(rEngine == NULL)
		rEngine = new RenderingEngine();
	return rEngine;
}

void RenderingEngine::setScreenDimensions(int width,int height)
{
	screenWidth = width;
	screenHeight = height;
}
int RenderingEngine::getScreenWidth()
{
	return screenWidth;
}

int RenderingEngine::getScreenHeight()
{
	return screenHeight;
}

void RenderingEngine::clearBuffer(LPDIRECTDRAWSURFACE7 buf)
{
	DDBLTFX fx;
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = 0x000000;
	buf->Blt(NULL, NULL, NULL,DDBLT_COLORFILL, &fx);
}

void RenderingEngine::clearAllInternalBuffer()
{
	DDBLTFX fx;
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = 0x000000;
	DDrawPrimary->Blt(NULL, NULL, NULL,DDBLT_COLORFILL, &fx);
	DDrawBack->Blt(NULL, NULL, NULL,DDBLT_COLORFILL, &fx);
}

void RenderingEngine::fillBuffer(LPDIRECTDRAWSURFACE7 buf,int colorCode)
{
	DDBLTFX fx;
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = colorCode;
	buf->Blt(NULL, NULL, NULL,DDBLT_COLORFILL, &fx);
}

int RenderingEngine::init(HWND hwnd)
{
	HRESULT hRet;
	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;
	DDSCAPS2 ddscaps;
	
	//Erase the backbuffer
	ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;
					
	hRet=DirectDrawCreateEx(NULL,(LPVOID*)&DDraw,IID_IDirectDraw7,NULL);
	if(hRet != DD_OK) {
		return false;
	}

	hRet = DDraw->SetCooperativeLevel(hwnd,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);

	if(hRet != DD_OK)	{
		assert(0);
		//return false;
	}
	


	hRet = DDraw->CreateClipper(NULL,&DDrawClip,NULL);
	if(hRet != DD_OK) {
		return false;
	}

//	ddrval=DDraw->SetDisplayMode(1024,768,32,0,0);
//	if(ddrval != DD_OK) {
//		return false;
//	}


	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;

	ddrval=DDraw->CreateSurface(&ddsd,&DDrawPrimary,NULL);
	if(ddrval != DD_OK) {
		return false;
	}

	ZeroMemory(&ddscaps,sizeof(ddscaps));
	ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
	
	ddrval=DDrawPrimary->GetAttachedSurface(&ddscaps,&DDrawBack);
	if(ddrval != DD_OK) {
		return false;
	}
	
	//Get the display resolution
	DDraw->GetDisplayMode(&ddsd);
	setScreenDimensions(ddsd.dwWidth,ddsd.dwHeight);
	clearBuffer(DDrawBack);
	
    // at the end set the key for transparent blitting
	// Structure used for transparent blitting
	DDPIXELFORMAT ddpf;
	ddpf.dwSize = sizeof(ddpf);
	DDrawPrimary->GetPixelFormat(&ddpf);
	DWORD keycolor = ddpf.dwGBitMask;
	key.dwColorSpaceLowValue = keycolor;
	key.dwColorSpaceHighValue = keycolor;
	return yes;
}

int RenderingEngine::initWindowed(HWND hwnd)
{
	
    DDSURFACEDESC2 ddsd;
    DDSCAPS2 ddscaps;
    HRESULT ddrval;

   /*
    * create the main DirectDraw object
    */
    ddrval = DirectDrawCreateEx(NULL,(LPVOID*)&DDraw,IID_IDirectDraw7,NULL);
    if( ddrval != DD_OK )
    {
        return(false);
    }

    // using DDSCL_NORMAL means we will coexist with GDI
    ddrval = DDraw->SetCooperativeLevel( hwnd, DDSCL_NORMAL );
    if( ddrval != DD_OK )
    {
    	DDraw->Release();
        return(false);
    }

    memset( &ddsd, 0, sizeof(ddsd) );
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    // The primary surface is not a page flipping surface this time
    ddrval = DDraw->CreateSurface( &ddsd, &DDrawPrimary, NULL );
    if( ddrval != DD_OK )
    {
    	DDraw->Release();
        return(false);
    }

    // Create a clipper to ensure that our drawing stays inside our window
    ddrval = DDraw->CreateClipper( 0, &DDrawClip, NULL );
    if( ddrval != DD_OK )
    {
    	DDrawPrimary->Release();
    	DDraw->Release();
        return(false);
    }

    // setting it to our hwnd gives the clipper the coordinates from our window
    ddrval = DDrawClip->SetHWnd( 0, hwnd );
    if( ddrval != DD_OK )
    {
    	DDrawClip-> Release();
        DDrawPrimary->Release();
        DDraw->Release();
        return(false);
    }

    // attach the clipper to the primary surface
    ddrval = DDrawPrimary->SetClipper( DDrawClip );
    if( ddrval != DD_OK )
    {
    	DDrawClip-> Release();
    	DDrawPrimary->Release();
        DDraw->Release();
        return(false);
    }

    memset( &ddsd, 0, sizeof(ddsd) );
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = 640;
    ddsd.dwHeight = 480;

    // create the backbuffer separately
    ddrval = DDraw->CreateSurface( &ddsd, &DDrawBack, NULL );
    if( ddrval != DD_OK )
    {
    	DDrawClip->Release();
    	DDrawPrimary->Release();
        DDraw->Release();
        return(false);
    }
    
    // at the end set the key for transparent blitting
	// Structure used for transparent blitting
	DDPIXELFORMAT ddpf;
	ddpf.dwSize = sizeof(ddpf);
	DDrawPrimary->GetPixelFormat(&ddpf);
	DWORD keycolor = ddpf.dwGBitMask;
	key.dwColorSpaceLowValue = keycolor;
	key.dwColorSpaceHighValue = keycolor;
	
    return(yes);
}

LPDIRECTDRAWSURFACE7 RenderingEngine::createBitmapSurface(String fname, int *pWidth, int *pHeight)
{
	HDC hdc;
	HBITMAP bit;
	LPDIRECTDRAWSURFACE7 surf;
	bit=(HBITMAP)LoadImage(NULL,fname.str(),IMAGE_BITMAP,0,0,LR_DEFAULTSIZE | LR_LOADFROMFILE);
	
	if(!bit) {
		return NULL;
	}

	BITMAP bitmap;
	GetObject(bit,sizeof(BITMAP),&bitmap);

	int surf_width = bitmap.bmWidth;
	int surf_height = bitmap.bmHeight;
	
	//Setting bitmap width and height
	if(pWidth != NULL && pHeight != NULL) {
		*pWidth = bitmap.bmWidth;
		*pHeight = bitmap.bmHeight;
	}
	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;
	
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = surf_width;
	ddsd.dwHeight = surf_height;
	
	ddrval=DDraw->CreateSurface(&ddsd,&surf,NULL);
	if(ddrval != DD_OK) {
		DeleteObject(bit);
		assert(0);
		//return NULL;
	}
	else {
		surf->GetDC(&hdc);
		HDC bit_dc = CreateCompatibleDC(hdc);
		SelectObject(bit_dc,bit);
		BitBlt(hdc,0,0,surf_width,surf_height,bit_dc,0,0,SRCCOPY);
		surf->ReleaseDC(hdc);
		DeleteObject(bit_dc);
	}
	
	// set the transparent blit key here
	surf->SetColorKey(DDCKEY_SRCBLT,&key);

	DeleteObject(bit);
	return surf;
}

LPDIRECTDRAWSURFACE7 RenderingEngine::createEmptySurface(int width,int height)
{
	if(width <= 0 || height <= 0)
		assert(0);

	LPDIRECTDRAWSURFACE7 surf;	
	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;
	
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;
	
	ddrval=DDraw->CreateSurface(&ddsd,&surf,NULL);
	if(ddrval != DD_OK) {
		assert(0);
		//return NULL;
	}
	// set the transparent blit key here
	surf->SetColorKey(DDCKEY_SRCBLT,&key);

	return surf;
}


void RenderingEngine::flipBuffers(HWND hWnd)
{
	//Blast everything to the primary buffer
	//enable if Windowed
	//DDrawPrimary->Blt(NULL, DDrawBack, NULL, DDBLT_WAIT, NULL);
	//enabled if full screen
	HRESULT ddrval;
    RECT rcRectSrc;
    RECT rcRectDest;
    POINT p;

    // if we're windowed do the blit, else just Flip
    if (!bExclusive)
    {
        // first we need to figure out where on the primary surface our window lives
        p.x = 0; p.y = 0;
        ClientToScreen(hWnd, &p);
        GetClientRect(hWnd, &rcRectDest);
        OffsetRect(&rcRectDest, p.x, p.y);
        SetRect(&rcRectSrc, 0, 0, 640, 480);
        ddrval = DDrawPrimary->Blt( &rcRectDest, DDrawBack, &rcRectSrc, DDBLT_WAIT, NULL);
    } else {
        ddrval = DDrawPrimary->Flip( NULL, DDFLIP_WAIT);
    }
    
    if( ddrval == DDERR_SURFACELOST )
    {
    	DDrawPrimary->Restore();
        DDrawBack->Restore();
    }


	//DDrawPrimary->Flip(0,DDFLIP_WAIT);
}

LPDIRECTDRAWSURFACE7 RenderingEngine::getPrimary()
{
	return DDrawPrimary;
}

void RenderingEngine::outputText(LPDIRECTDRAWSURFACE7 surface,int x, int y,int color,char* str,int fontWidth,int fontHeight, int bgcolor)
{
	HDC hdc;
	if(surface == NULL)
		assert(0);

	if(DD_OK == surface->GetDC(&hdc))
	{
		LOGFONT lf;
		memset (&lf, 0, sizeof (lf));
		lf.lfWidth= fontWidth;
		lf.lfHeight = fontHeight;
		HFONT font = CreateFontIndirect (&lf);
		SelectObject (hdc, font);
		SetTextColor(hdc,color);

		if(bgcolor != TRANSPARENT)
		{
			SetBkColor(hdc, bgcolor);
		}
		else
		{
			SetBkMode(hdc, TRANSPARENT);
		}

		TextOut(hdc,x,y,str,strlen(str));
		DeleteObject(font);
		surface->ReleaseDC(hdc);
	}
}

void RenderingEngine::outputText(LPDIRECTDRAWSURFACE7 surface,int x, int y,int color,string str,int fontWidth,int fontHeight, int bgcolor)
{
	HDC hdc;
	if(surface == NULL)
		assert(0);

	if(DD_OK == surface->GetDC(&hdc))
	{
		LOGFONT lf;
		memset (&lf, 0, sizeof (lf));
		lf.lfWidth = fontWidth;
		lf.lfHeight = fontHeight;
		HFONT font = CreateFontIndirect (&lf);
		SelectObject (hdc, font);
		SetTextColor(hdc,color);

		if(bgcolor != TRANSPARENT)
		{
			SetBkColor(hdc, bgcolor);
		}
		else
		{
			SetBkMode(hdc, TRANSPARENT);
		}

		TextOut(hdc,x,y,str.c_str(),strlen(str.c_str()));
		DeleteObject(font);
		surface->ReleaseDC(hdc);
	}
}

void RenderingEngine::outputText(LPDIRECTDRAWSURFACE7 surface,int x, int y,int color,int value,int fontWidth,int fontHeight, int bgcolor)
{
	HDC hdc;
	if(surface == NULL)
		assert(0);
	char temp[100];
	sprintf(temp,"%03d",value);
	if(DD_OK == surface->GetDC(&hdc)) {
		LOGFONT lf;
		memset (&lf, 0, sizeof (lf));
		lf.lfWidth = fontWidth;
		lf.lfHeight = fontHeight;
		HFONT font = CreateFontIndirect (&lf);
		SelectObject (hdc, font);
		SetTextColor(hdc,color);

		if(bgcolor != TRANSPARENT)
		{
			SetBkColor(hdc, bgcolor);
		}
		else
		{
			SetBkMode(hdc, TRANSPARENT);
		}

		TextOut(hdc,x,y,temp,strlen(temp));
		DeleteObject(font);
		surface->ReleaseDC(hdc);
	}
}

void RenderingEngine::outputText(LPDIRECTDRAWSURFACE7 surface,int x, int y,int color,int value,int fontWidth,int fontHeight,char* strFormat, int bgcolor)
{
	HDC hdc;
	char temp[100];
	sprintf(temp,strFormat,value);
	if(surface == NULL)
		assert(0);
	if(DD_OK == surface->GetDC(&hdc)) {
		LOGFONT lf;
		memset (&lf, 0, sizeof (lf));
		lf.lfWidth = fontWidth;
		lf.lfHeight = fontHeight;
		HFONT font = CreateFontIndirect (&lf);
		SelectObject (hdc, font);
		SetTextColor(hdc,color);

		if(bgcolor != TRANSPARENT)
		{
			SetBkColor(hdc, bgcolor);
		}
		else
		{
			SetBkMode(hdc, TRANSPARENT);
		}

		TextOut(hdc,x,y,temp,strlen(temp));
		DeleteObject(font);
		surface->ReleaseDC(hdc);
	}
}

void RenderingEngine::outputText(LPDIRECTDRAWSURFACE7 surface,int x, int y,int color,double value,int fontWidth,int fontHeight, int bgcolor)
{
	HDC hdc;
	if(surface == NULL)
		assert(0);
	char temp[100];
	sprintf(temp,"%03f",value);
	if(DD_OK == surface->GetDC(&hdc)) {
		LOGFONT lf;
		memset (&lf, 0, sizeof (lf));
		lf.lfWidth = fontWidth;
		lf.lfHeight = fontHeight;
		HFONT font = CreateFontIndirect (&lf);
		SelectObject (hdc, font);
		SetTextColor(hdc,color);

		if(bgcolor != TRANSPARENT)
		{
			SetBkColor(hdc, bgcolor);
		}
		else
		{
			SetBkMode(hdc, TRANSPARENT);
		}

		TextOut(hdc,x,y,temp,strlen(temp));
		DeleteObject(font);
		surface->ReleaseDC(hdc);
	}
}

LPDIRECTDRAWCLIPPER RenderingEngine::DDrawAttachClipper(LPDIRECTDRAWSURFACE7 lpdds, int numRects, LPRECT clip_list)
{
	int index;
	HRESULT ddrval;
	LPDIRECTDRAWCLIPPER lpddclipper = NULL;
	LPRGNDATA region_data;
	ddrval = DDraw->CreateClipper(0,&lpddclipper,NULL);
	if(ddrval != DD_OK)
	{
		return NULL;
	}
	region_data = (LPRGNDATA) malloc(sizeof(RGNDATAHEADER) + (numRects * sizeof(RECT)));
	memcpy(region_data->Buffer, clip_list, (sizeof(RECT) * numRects));
	// Set up the header and member fields.
	region_data->rdh.dwSize          = sizeof(RGNDATAHEADER);
	region_data->rdh.iType           = RDH_RECTANGLES;
	region_data->rdh.nCount          = numRects;	
	region_data->rdh.nRgnSize        = numRects * sizeof(RECT);
		
	region_data->rdh.rcBound.left    =  64000;
	region_data->rdh.rcBound.top     =  64000;
	region_data->rdh.rcBound.right   = -64000;
	region_data->rdh.rcBound.bottom  = -64000;
	
	for(index = 0; index < numRects; index++)
	{
		if(clip_list[index].left < region_data->rdh.rcBound.left)
			region_data->rdh.rcBound.left = clip_list[index].left;
		
		if(clip_list[index].right > region_data->rdh.rcBound.right)
			region_data->rdh.rcBound.right = clip_list[index].right;

		if(clip_list[index].top < region_data->rdh.rcBound.top)
			region_data->rdh.rcBound.top = clip_list[index].top;

		if(clip_list[index].bottom > region_data->rdh.rcBound.bottom)
			region_data->rdh.rcBound.bottom = clip_list[index].bottom;
	}
	lprintf("top = %d, left = %d, right = %d, bottom = %d\n",region_data->rdh.rcBound.top,region_data->rdh.rcBound.left,region_data->rdh.rcBound.right,region_data->rdh.rcBound.bottom);
	ddrval = lpddclipper->SetClipList(region_data, 0);
	if(ddrval != DD_OK)
	{
		free(region_data);
		return NULL;
	}

	ddrval = lpdds->SetClipper(lpddclipper);
 	if(ddrval != DD_OK)
	{
		free(region_data);
		return NULL;
	}

	free(region_data);
	return (lpddclipper);
}

void RenderingEngine::plotPixel(LPDIRECTDRAWSURFACE7 surface, int x, int y, int color)
{
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize= sizeof(ddsd);
	surface->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);

	//get video pointer to primary surface
	UCHAR *primaryBuffer = (UCHAR*) ddsd.lpSurface;
	*((DWORD*)(primaryBuffer + x * 4 + y * ddsd.lPitch)) = color;
	surface->Unlock(NULL);
}

void RenderingEngine::drawLine(LPDIRECTDRAWSURFACE7 surface, int x0, int y0, int x1, int y1, int color)
{
	int dx, dy, dx2, dy2, x_inc, y_inc, error, index;
	Logical drawingVerticalLine = no, drawingHorizontalLine = no;
	int x = x0, y = y0;

	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize= sizeof(ddsd);
	surface->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);

	//get video pointer to primary surface
	UCHAR *primaryBuffer = (UCHAR*) ddsd.lpSurface;

	dx = x1 - x0;
	dy = y1 - y0;

	if(dy <= 0)
	{
		drawingHorizontalLine = yes;
	}

	if(dx <= 0)
	{
		drawingVerticalLine = yes;
	}

	if(drawingHorizontalLine)
	{
		//keep y constant and x changing
		for(x = x0; x <= x1; x++)
		{
			*((DWORD*)(primaryBuffer + x * 4 + y * ddsd.lPitch)) = color;
		}

		surface->Unlock(NULL);
		return;
	}
	else if(drawingVerticalLine)
	{
		//keep x constant and y changing
		for(y = y0; y <= y1; y++)
		{
			*((DWORD*)(primaryBuffer + x * 4 + y * ddsd.lPitch)) = color;
		}

		surface->Unlock(NULL);
		return;
	}

	if(dx >= 0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx = -dx;
	}

	if(dy >= 0)
	{
		y_inc = 1;
	}
	else
	{
		y_inc = -1;
		dy = -dy;
	}

	dx2 = dx << 1;
	dy2 = dy << 1;

	if(dx > dy)
	{
		error = dy2 - dx;

		for(index = 0; index <= dx; index++)
		{
			if(error >= 0)
			{
				error -= dx2;
				*((DWORD*)(primaryBuffer + x * 4 + y * ddsd.lPitch)) = color;
				y += y_inc;
			}

			error += dy2;
			x += x_inc;
		}
	}
	else
	{
		error = dx2 - dy;

		for(index = 0; index <= dy; index++)
		{
			if(error >= 0)
			{
				error -= dy2;
				*((DWORD*)(primaryBuffer + x * 4 + y * ddsd.lPitch)) = color;
				x += x_inc;
			}

			error += dx2;
			y += y_inc;
		}
	}
	surface->Unlock(NULL);
}
