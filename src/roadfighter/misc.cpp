#include "misc.h"

/*LPDIRECTDRAWSURFACE7 bitmap_surface(LPCSTR file_name)
{
	HDC hdc;
	HBITMAP bit;
	LPDIRECTDRAWSURFACE7 surf;
	bit=(HBITMAP)LoadImage(NULL,file_name,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
	if(!bit)
	{
		return NULL;
	}
	BITMAP bitmap;
	GetObject(bit,sizeof(BITMAP),&bitmap);
	int surf_width=bitmap.bmWidth;
	int surf_height=bitmap.bmHeight;
	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth=surf_width;
	ddsd.dwHeight=surf_height;
	ddrval=lpDD->CreateSurface(&ddsd,&surf,NULL);
	if(ddrval!=DD_OK)
	{
		DeleteObject(bit);
		return NULL;
	}
	else
	{
		surf->GetDC(&hdc);
		HDC bit_dc=CreateCompatibleDC(hdc);
		SelectObject(bit_dc,bit);
		BitBlt(hdc,0,0,surf_width,surf_height,bit_dc,0,0,SRCCOPY);
		surf->ReleaseDC(hdc);
		DeleteObject(bit_dc);
	}
	DeleteObject(bit);
	return surf;
}*/
