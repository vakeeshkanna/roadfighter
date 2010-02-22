/* (C)2002 Robin Burrows - rburrows@bigfoot.com
 * This is Release 11b, 7/5/2002 - for Mappy FMP maps with DirectX SDK
 * Please read the mappydx.htm file and look at the example
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef _WIN32
#define _WIN32
#endif
#define  NO_STRICT
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#define RBDIRECTDRAW LPDIRECTDRAW7
#define RBDIRECTDRAWSURFACE LPDIRECTDRAWSURFACE7
#define RBSURFACEDESC DDSURFACEDESC2
/* Settings for later versions of DirectX, uncomment these and comment out above */
/* You should use defines that correspond to the interface you are using */
/* Don't forget to change mpydll.h too! */
# define DLLIMPORT __declspec (dllexport)
//#define RBDIRECTDRAW LPDIRECTDRAW4
//#define RBDIRECTDRAWSURFACE LPDIRECTDRAWSURFACE4
//#define RBSURFACEDESC DDSURFACEDESC2

//#define RBUSEBLTONLY 1
//#define RBSCAPS_TYPE DDSCAPS_OFFSCREENPLAIN
//#define RBSCAPS_TYPE DDSCAPS_SYSTEMMEMORY

/* These define how far from the left and top of the surface the drawn area is */
#define MMOX mapmmox
#define MMOY mapmmoy
/* These define the width and height of the drawn area... */
#define MSCRW mapmscrw
#define MSCRH mapmscrh

/* Size of Parallax tile, ignore if not using Parallax */
#define PARAW mapparaw			/* Width of parallax block, in pixels, MUST be multiple of MBSW */
#define PARAH mapparah			/* Height of parallax block, in pixels, MUST be multiple of MBSH */

//#define MOVERIDETRANSP 1	/* Uncomment to use values below instead of in FMP */
#define MTRANSP8 0		/* Transparency index for paletted screen (0 to 255)*/
#define MTRANSPR 0xFF	/* 8bit RED transparency value for high/true colour */
#define MTRANSPG 0x00	/* 8bit GREEN transparency value for high/true colour */
#define MTRANSPB 0xFF	/* 8bit BLUE transparency value for high/true colour */

/* Comment out next line to disable index0 to truecolour pink conversion */
//#define RB8BITTOPINK 1
/* END OF USER SETTINGS */

#define MPY_MAPWIDTH 1		/* values for MapGetVal() */
#define MPY_MAPHEIGHT 2
#define MPY_MAPDEPTH 3
#define MPY_BLOCKWIDTH 4
#define MPY_BLOCKHEIGHT 5
#define MPY_NUMBLOCKSTR 6
#define MPY_NUMBLOCKGFX 7
#define MPY_BLOCKGAPX 8
#define MPY_BLOCKGAPY 9
#define MPY_BLOCKSTAGGERX 10
#define MPY_BLOCKSTAGGERY 11
#define MPY_CLICKMASK 12
#define MPY_MAPTRANS8 13
#define MPY_MAPTRANSRED 14
#define MPY_MAPTRANSGREEN 15
#define MPY_MAPTRANSBLUE 16
#define MPY_MAPERROR 17
#define MPY_BLTTYPE 18
#define MPY_MEMTYPE 19
#define MPY_8BITTOPINK 20
#define MPY_PALETTE 1000

#define MER_NONE 0		/* All the horrible things that can go wrong */
#define MER_OUTOFMEM 1
#define MER_MAPLOADERROR 2
#define MER_NOOPEN 3
#define MER_MAPTOONEW 4

#define AN_END -1			/* Animation types, AN_END = end of anims */
#define AN_NONE 0			/* No anim defined */
#define AN_LOOPF 1		/* Loops from start to end, then jumps to start etc */
#define AN_LOOPR 2		/* As above, but from end to start */
#define AN_ONCE 3			/* Only plays once */
#define AN_ONCEH 4		/* Only plays once, but holds end frame */
#define AN_PPFF 5			/* Ping Pong start-end-start-end-start etc */
#define AN_PPRR 6			/* Ping Pong end-start-end-start-end etc */
#define AN_PPRF 7			/* Used internally by playback */
#define AN_PPFR 8			/* Used internally by playback */
#define AN_ONCES 9		/* Used internally by playback */

typedef struct {				/* Structure for data blocks */
RBDIRECTDRAWSURFACE bgoff, fgoff;			/* offsets from start of graphic blocks */
RBDIRECTDRAWSURFACE fgoff2, fgoff3; 		/* more overlay blocks */
unsigned long int user1, user2;	/* user long data */
unsigned short int user3, user4;	/* user short data */
unsigned char user5, user6, user7;	/* user byte data */
unsigned char tl : 1;				/* bits for collision detection */
unsigned char tr : 1;
unsigned char bl : 1;
unsigned char br : 1;
unsigned char trigger : 1;			/* bit to trigger an event */
unsigned char unused1 : 1;
unsigned char unused2 : 1;
unsigned char unused3 : 1;
} BLKSTR;

typedef struct {		/* Animation control structure */
signed char antype;	/* Type of anim, AN_? */
signed char andelay;	/* Frames to go before next frame */
signed char ancount;	/* Counter, decs each frame, till 0, then resets to andelay */
signed char anuser;	/* User info */
long int ancuroff;	/* Points to current offset in list */
long int anstartoff;	/* Points to start of blkstr offsets list, AFTER ref. blkstr offset */
long int anendoff;	/* Points to end of blkstr offsets list */
} ANISTR;

/* Prototypes */
extern "C" DLLIMPORT void MapReleaseParallaxSurface (void);

/* All global variables used by Mappy playback are here */
static int rbcapstype = DDSCAPS_OFFSCREENPLAIN, rbblttype = 1, rb8bittopink = 1;
static int mapparaw = 128, mapparah = 128;
static int mapmmox = 0, mapmmoy = 0, mapmscrw = 640, mapmscrh = 480;
static int maperror = 0;	/* Set to a MER_ error if something wrong happens */
static short int mapwidth, mapheight, mapblockwidth, mapblockheight, mapdepth;
static short int mapblockstrsize, mapnumblockstr, mapnumblockgfx;
static FILE * mapfilept;
static short int * mappt = NULL;
static short int ** maparraypt = NULL;
static unsigned char * mapcmappt = NULL;
static char * mapblockgfxpt = NULL;
static char * mapblockstrpt = NULL;
static ANISTR * mapanimstrpt = NULL;
static ANISTR * mapanimstrendpt;
static PALETTEENTRY mappept[256];
static LPDIRECTDRAWPALETTE	lpddpal = NULL;	// a pointer to the created dd palette
static RBDIRECTDRAWSURFACE * maplpDDSTiles = NULL;	// Tiles Direct Draw Surfaces
static RBDIRECTDRAWSURFACE lpDDSParallax = NULL;
static short int * mapmappt[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
static short int ** mapmaparraypt[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
/* Added in release 7 */
static char mapnovctext[80];
static int * mapanimseqpt = NULL;
static int mapblockgapx, mapblockgapy, mapblockstaggerx, mapblockstaggery, mapclickmask;
static int maptype, mapislsb, mapaltdepth;
static char * mapaltblockgfxpt = NULL;
static int maptrans8, maptransred, maptransgreen, maptransblue;
/* End of Mappy globals */

extern "C" BOOL APIENTRY
DllMain (
	 HINSTANCE hInst /* Library instance handle. */ ,
	 DWORD reason /* Reason this function is being called. */ ,
	 LPVOID reserved /* Not used. */ )
{

  switch (reason)
    {
    case DLL_PROCESS_ATTACH:
      break;

    case DLL_PROCESS_DETACH:
      break;

    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;
    }
  return TRUE;
}

extern "C" DLLIMPORT void * MapGetPtr (int mgval)
{
	return NULL;
}

extern "C" DLLIMPORT int MapSetPtr (int mgval, void * ptr)
{
	return 0;
}

extern "C" DLLIMPORT int MapGetVal (int mgval)
{
	if (mgval >= MPY_PALETTE && mgval < (MPY_PALETTE+(256*3))) {
		if (mapcmappt == NULL) return 0;
		return ((int) mapcmappt[mgval-MPY_PALETTE]);
	}
	switch (mgval) {
		case 0: return 0;
		case MPY_MAPWIDTH: return ((int) mapwidth);
		case MPY_MAPHEIGHT: return ((int) mapheight);
		case MPY_MAPDEPTH: return ((int) mapdepth);
		case MPY_BLOCKWIDTH: return ((int) mapblockwidth);
		case MPY_BLOCKHEIGHT: return ((int) mapblockheight);
		case MPY_NUMBLOCKSTR: return ((int) mapnumblockstr);
		case MPY_NUMBLOCKGFX: return ((int) mapnumblockgfx);
		case MPY_BLOCKGAPX: return ((int) mapblockgapx);
		case MPY_BLOCKGAPY: return ((int) mapblockgapy);
		case MPY_BLOCKSTAGGERX: return ((int) mapblockstaggerx);
		case MPY_BLOCKSTAGGERY: return ((int) mapblockstaggery);
		case MPY_CLICKMASK: return ((int) mapclickmask);
		case MPY_MAPTRANS8: return ((int) maptrans8);
		case MPY_MAPTRANSRED: return ((int) maptransred);
		case MPY_MAPTRANSGREEN: return ((int) maptransgreen);
		case MPY_MAPTRANSBLUE: return ((int) maptransblue);
		case MPY_MAPERROR: return ((int) maperror);
	}
	return 0;
}

extern "C" DLLIMPORT int MapSetVal (int mgval, int tsval)
{
	switch (mgval) {
		case 0: return 0;
		case MPY_BLTTYPE: { rbblttype = tsval; return 0; }
		case MPY_MEMTYPE: { if (tsval == 0) rbcapstype = DDSCAPS_OFFSCREENPLAIN; else rbcapstype = DDSCAPS_SYSTEMMEMORY; return 0; }
		case MPY_8BITTOPINK: { if (tsval == 0) rb8bittopink = 0; else rb8bittopink = 1; return 0; }
	}
	return 0;
}

extern "C" DLLIMPORT void MapSetDrawArea (int mx, int my, int mw, int mh)
{
	mapmmox = mx; mapmmoy = my; mapmscrw = mw; mapmscrh = mh;
}

extern "C" DLLIMPORT void MapSetParaSize (int mpw, int mph)
{
	mapparaw = mpw; mapparah = mph;
}
extern "C" DLLIMPORT void MapFreeMem (void)
{
int i;

	MapReleaseParallaxSurface ();
	for (i=0;i<8;i++) { if (mapmappt[i]!=NULL) { free (mapmappt[i]); mapmappt[i] = NULL; } }
	mappt = NULL;
	for (i=0;i<8;i++) { if (mapmaparraypt[i]!=NULL) { free (mapmaparraypt[i]); mapmaparraypt[i] = NULL; } }
	maparraypt = NULL;
	if (mapcmappt!=NULL) { free (mapcmappt); mapcmappt = NULL; }
	if (mapblockgfxpt!=NULL) { free (mapblockgfxpt); mapblockgfxpt = NULL; }
	if (mapaltblockgfxpt!=NULL) { free (mapaltblockgfxpt); mapaltblockgfxpt = NULL; }
	if (mapblockstrpt!=NULL) { free (mapblockstrpt); mapblockstrpt = NULL; }
	if (mapanimstrpt!=NULL) { free (mapanimstrpt); mapanimstrpt = NULL; }
	if (mapanimseqpt!=NULL) { free (mapanimseqpt); mapanimseqpt = NULL; }

	if (maplpDDSTiles != NULL) {
		for (i=0;(maplpDDSTiles[i]!=NULL);i++)
		{ maplpDDSTiles[i]->Release(); maplpDDSTiles[i] = NULL; }
		free (maplpDDSTiles); maplpDDSTiles = NULL;
	}
	mapnovctext[0] = 0;
//	mapmmox = 0; mapmmoy = 0; mapmscrw = 640; mapmscrh = 480;
//	mapparaw = 128; mapparah = 128;
}

extern "C" DLLIMPORT int MapGenerateYLookup (void)
{
int i, j;

	for (i=0;i<8;i++) {
		if (mapmaparraypt[i]!=NULL) { free (mapmaparraypt[i]); mapmaparraypt[i] = NULL; }
		if (mapmappt[i]!=NULL) {
			mapmaparraypt[i] = (short int **) malloc (mapheight*sizeof(short int *));
			if (mapmaparraypt[i] == NULL) return -1;
			for (j=0;j<mapheight;j++) mapmaparraypt[i][j] = (mapmappt[i]+(j*mapwidth));
			if (mapmappt[i] == mappt) maparraypt = mapmaparraypt[i];
		}
	}
	return 0;
}

static int MEClickmask (int x, int y, int xory)
{
unsigned char * cmgfxpt;

	if (mapblockgfxpt == NULL) return 0;

	cmgfxpt = (unsigned char *) mapblockgfxpt;
	x %= mapblockgapx; y %= mapblockgapy;

	if (x >= mapblockwidth && xory == 0) return 0;
	if (x >= mapblockwidth && xory == 1) {
		if (y < mapblockstaggery) return -1;
		else return 1;
	}
	if (y >= mapblockheight && xory == 1) return 1;
	if (y >= mapblockheight && xory == 0) {
		if (x < mapblockstaggerx) return -1;
		else return 0;
	}

	cmgfxpt += (mapblockwidth*mapblockheight*((mapdepth+1)/8)*mapclickmask);
	switch (mapdepth) {
		case 8:
			if (cmgfxpt[(y*mapblockwidth)+x] == maptrans8) {
				if (x < (mapblockwidth/2) && xory == 0) return -1;
				if (x >= (mapblockwidth/2) && xory == 0) return 0;
				if (y < (mapblockheight/2) && xory == 1) return -1;
				if (y >= (mapblockheight/2) && xory == 1) return 1;
			}
			return 0;
		case 15:
			if (cmgfxpt[((y*mapblockwidth)+x)*2] == (((maptransred>>1)&0x7C)|((maptransgreen>>6)&0x03)) &&
				cmgfxpt[((y*mapblockwidth)+x)*2+1] == (((maptransgreen<<2)&0xE0)|((maptransblue>>3)&0x1F))) {
				if (x < (mapblockwidth/2) && xory == 0) return -1;
				if (x >= (mapblockwidth/2) && xory == 0) return 0;
				if (y < (mapblockheight/2) && xory == 1) return -1;
				if (y >= (mapblockheight/2) && xory == 1) return 1;
			}
			return 0;
		case 16:
			if (cmgfxpt[((y*mapblockwidth)+x)*2] == (((maptransred)&0xF8)|((maptransgreen>>5)&0x07)) &&
				cmgfxpt[((y*mapblockwidth)+x)*2+1] == (((maptransgreen<<3)&0xE0)|((maptransblue>>3)&0x1F))) {
				if (x < (mapblockwidth/2) && xory == 0) return -1;
				if (x >= (mapblockwidth/2) && xory == 0) return 0;
				if (y < (mapblockheight/2) && xory == 1) return -1;
				if (y >= (mapblockheight/2) && xory == 1) return 1;
			}
			return 0;
		case 24:
			if (cmgfxpt[((y*mapblockwidth)+x)*3] == maptransred &&
				cmgfxpt[((y*mapblockwidth)+x)*3+1] == maptransgreen &&
				cmgfxpt[((y*mapblockwidth)+x)*3+2] == maptransblue) {
				if (x < (mapblockwidth/2) && xory == 0) return -1;
				if (x >= (mapblockwidth/2) && xory == 0) return 0;
				if (y < (mapblockheight/2) && xory == 1) return -1;
				if (y >= (mapblockheight/2) && xory == 1) return 1;
			}
			return 0;
		case 32:
			if (cmgfxpt[((y*mapblockwidth)+x)*4+1] == maptransred &&
				cmgfxpt[((y*mapblockwidth)+x)*4+2] == maptransgreen &&
				cmgfxpt[((y*mapblockwidth)+x)*4+3] == maptransblue) {
				if (x < (mapblockwidth/2) && xory == 0) return -1;
				if (x >= (mapblockwidth/2) && xory == 0) return 0;
				if (y < (mapblockheight/2) && xory == 1) return -1;
				if (y >= (mapblockheight/2) && xory == 1) return 1;
			}
			return 0;
	}
	return 0;
}

extern "C" DLLIMPORT int MapGetXOffset (int xpix, int ypix)
{
int xb;

	if (mapblockstaggerx || mapblockstaggery) {
		xpix += (mapblockstaggerx);
		ypix += (mapblockstaggery);
	}
	xb = xpix/mapblockgapx;

	if ((mapblockstaggerx || mapblockstaggery) && mapclickmask) xb += MEClickmask (xpix, ypix, 0);

	if (xb < 0) xb = 0;
	if (xb >= mapwidth) xb = mapwidth-1;
	return xb;
}

extern "C" DLLIMPORT int MapGetYOffset (int xpix, int ypix)
{
int yb;

	if (mapblockstaggerx || mapblockstaggery) {
		xpix += (mapblockstaggerx);
		ypix += (mapblockstaggery);
	}
	yb = ypix/mapblockgapy;

	if ((mapblockstaggerx || mapblockstaggery) && mapclickmask) {
		yb *= 2;
		yb += MEClickmask (xpix, ypix, 1);
	}

	if (yb < 0) yb = 0;
	if (yb >= mapheight) yb = mapheight-1;
	return yb;
}

extern "C" DLLIMPORT BLKSTR * MapGetBlockInPixels (int x, int y)
{
int xp, yp;
short int * mymappt;
ANISTR * myanpt;

	if (x < 0 || y < 0 || x >= (mapwidth*mapblockwidth) || y >= (mapheight*mapblockheight)) return NULL;

	xp = x; yp = y;
	x = MapGetXOffset (xp, yp);
	y = MapGetYOffset (xp, yp);

	if (maparraypt!= NULL) {
		mymappt = maparraypt[y]+x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y*mapwidth;
	}
	if (*mymappt>=0) return ((BLKSTR*) mapblockstrpt) + *mymappt;
	else { myanpt = mapanimstrendpt + *mymappt;
		return ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
}

extern "C" DLLIMPORT BLKSTR * MapGetBlock (int x, int y)
{
short int * mymappt;
ANISTR * myanpt;

	if (maparraypt!= NULL) {
		mymappt = maparraypt[y]+x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y*mapwidth;
	}
	if (*mymappt>=0) return ((BLKSTR*) mapblockstrpt) + *mymappt;
	else { myanpt = mapanimstrendpt + *mymappt;
		return ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
}

extern "C" DLLIMPORT void MapSetBlockInPixels (int x, int y, int strvalue)
{
int xp, yp;
short int * mymappt;

	if (x < 0 || y < 0 || x >= (mapwidth*mapblockwidth) || y >= (mapheight*mapblockheight)) return;
	xp = x; yp = y;
	x = MapGetXOffset (xp, yp);
	y = MapGetYOffset (xp, yp);

	if (maparraypt!= NULL) {
		mymappt = maparraypt[y]+x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y*mapwidth;
	}
	*mymappt = strvalue;
}

extern "C" DLLIMPORT void MapSetBlock (int x, int y, int strvalue)
{
short int * mymappt;

	if (maparraypt!= NULL) {
		mymappt = maparraypt[y]+x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y*mapwidth;
	}
	*mymappt = strvalue;
}

extern "C" DLLIMPORT int MapChangeLayer (int newlyr)
{
	if (newlyr<0 || newlyr>7 || mapmappt[newlyr] == NULL) return -1;
	mappt = mapmappt[newlyr]; maparraypt = mapmaparraypt[newlyr];
	return newlyr;
}

extern "C" DLLIMPORT int MapGetBlockID (int blid, int usernum)
{
int i;
BLKSTR * myblkpt;

	myblkpt = (BLKSTR *) mapblockstrpt;
	if (myblkpt == NULL) return -1;

	for (i=0;i<mapnumblockstr;i++) {
		switch (usernum) {
			case 1:
				if (myblkpt[i].user1 == (unsigned int) blid) return i;
				break;
			case 2:
				if (myblkpt[i].user2 == (unsigned int) blid) return i;
				break;
			case 3:
				if (myblkpt[i].user3 == blid) return i;
				break;
			case 4:
				if (myblkpt[i].user4 == blid) return i;
				break;
			case 5:
				if (myblkpt[i].user5 == blid) return i;
				break;
			case 6:
				if (myblkpt[i].user6 == blid) return i;
				break;
			case 7:
				if (myblkpt[i].user7 == blid) return i;
				break;
		}
	}

	return -1;
}

extern "C" DLLIMPORT int MapDecodeMAR (unsigned char * mrpt, int marlyr)
{
int i, j;
short int * mymarpt;

	if (marlyr < 0 || marlyr > 7) return -1;

	if (mapmappt[marlyr] == NULL)
		mapmappt[marlyr] = (short int *) malloc (mapwidth*mapheight*sizeof(short int));

	memcpy (mapmappt[marlyr], mrpt, (mapwidth*mapheight*sizeof(short int)));

	mymarpt = mapmappt[marlyr];
	j = 0; for (i=0;i<(mapwidth*mapheight);i++) { if (mymarpt[i]&0xF) j = 1; }
	if (j == 0) {
		for (i=0;i<(mapwidth*mapheight);i++) {
			if (mymarpt[i] >= 0) mymarpt[i] /= 32;
			else mymarpt[i] /= 16;
		}
	}

	return 0;
}

extern "C" DLLIMPORT int MapLoadMAR (char * mname, int marlyr)
{
int i, j;
short int * mymarpt;
FILE * marfpt;

	if (marlyr < 0 || marlyr > 7) return -1;

	marfpt = fopen (mname, "rb");
	if (marfpt==NULL) { return -1; }

	if (mapmappt[marlyr] == NULL)
		mapmappt[marlyr] = (short int *) malloc (mapwidth*mapheight*sizeof(short int));

	if (fread (mapmappt[marlyr], 1, (mapwidth*mapheight*sizeof(short int)), marfpt) !=
		(mapwidth*mapheight*sizeof(short int))) { fclose (marfpt); return -1; }

	fclose (marfpt);

	mymarpt = mapmappt[marlyr];
	j = 0; for (i=0;i<(mapwidth*mapheight);i++) { if (mymarpt[i]&0xF) j = 1; }
	if (j == 0) {
		for (i=0;i<(mapwidth*mapheight);i++) {
			if (mymarpt[i] >= 0) mymarpt[i] /= 32;
			else mymarpt[i] /= 16;
		}
	}

	return 0;
}

extern "C" DLLIMPORT void MapInitAnims (void)
{
ANISTR * myanpt;
	if (mapanimstrpt==NULL) return;
	myanpt = (ANISTR *) mapanimstrendpt; myanpt--;
	while (myanpt->antype!=-1)
	{
		if (myanpt->antype==AN_PPFR) myanpt->antype = AN_PPFF;
		if (myanpt->antype==AN_PPRF) myanpt->antype = AN_PPRR;
		if (myanpt->antype==AN_ONCES) myanpt->antype = AN_ONCE;
		if ((myanpt->antype==AN_LOOPR) || (myanpt->antype==AN_PPRR))
		{
		myanpt->ancuroff = myanpt->anstartoff;
		if ((myanpt->anstartoff)!=(myanpt->anendoff)) myanpt->ancuroff=(myanpt->anendoff)-1;
		} else {
		myanpt->ancuroff = myanpt->anstartoff;
		}
		myanpt->ancount = myanpt->andelay;
		myanpt--;
	}
}

extern "C" DLLIMPORT void MapUpdateAnims (void)
{
ANISTR * myanpt;

	if (mapanimstrpt==NULL) return;
	myanpt = (ANISTR *) mapanimstrendpt; myanpt--;
	while (myanpt->antype!=-1)
	{
		if (myanpt->antype!=AN_NONE) { myanpt->ancount--; if (myanpt->ancount<0) {
		myanpt->ancount = myanpt->andelay;
		if (myanpt->antype==AN_LOOPF)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff++;
			if (myanpt->ancuroff==myanpt->anendoff) myanpt->ancuroff = myanpt->anstartoff;
		} }
		if (myanpt->antype==AN_LOOPR)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff--;
			if (myanpt->ancuroff==((myanpt->anstartoff)-1))
				myanpt->ancuroff = (myanpt->anendoff)-1;
		} }
		if (myanpt->antype==AN_ONCE)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff++;
			if (myanpt->ancuroff==myanpt->anendoff) { myanpt->antype = AN_ONCES;
				myanpt->ancuroff = myanpt->anstartoff; }
		} }
		if (myanpt->antype==AN_ONCEH)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) {
			if (myanpt->ancuroff!=((myanpt->anendoff)-1)) myanpt->ancuroff++;
		} }
		if (myanpt->antype==AN_PPFF)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff++;
			if (myanpt->ancuroff==myanpt->anendoff) { myanpt->ancuroff -= 2;
			myanpt->antype = AN_PPFR;
			if (myanpt->ancuroff<myanpt->anstartoff) myanpt->ancuroff++; }
		} } else {
		if (myanpt->antype==AN_PPFR)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff--;
			if (myanpt->ancuroff==((myanpt->anstartoff)-1)) { myanpt->ancuroff += 2;
			myanpt->antype = AN_PPFF;
			if (myanpt->ancuroff>myanpt->anendoff) myanpt->ancuroff--; }
		} } }
		if (myanpt->antype==AN_PPRR)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff--;
			if (myanpt->ancuroff==((myanpt->anstartoff)-1)) { myanpt->ancuroff += 2;
			myanpt->antype = AN_PPRF;
			if (myanpt->ancuroff>myanpt->anendoff) myanpt->ancuroff--; }
		} } else {
		if (myanpt->antype==AN_PPRF)
		{
			if (myanpt->anstartoff!=myanpt->anendoff) { myanpt->ancuroff++;
			if (myanpt->ancuroff==myanpt->anendoff) { myanpt->ancuroff -= 2;
			myanpt->antype = AN_PPRR;
			if (myanpt->ancuroff<myanpt->anstartoff) myanpt->ancuroff++; }
		} } }
	} } myanpt--; }
}

extern "C" DLLIMPORT void MapSetPalette (RBDIRECTDRAW lpDD, RBDIRECTDRAWSURFACE lpDDS)
{
	lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, mappept, &lpddpal, NULL);
	lpDDS->SetPalette(lpddpal);
}

static void MapCMAPtoPE (unsigned char * mycmappt, PALETTEENTRY * mypept)
{
int i;
	for (i=0;i<256;i++)
	{
		mypept[i].peRed = (BYTE) *mycmappt; mycmappt++;
		mypept[i].peGreen = (BYTE) *mycmappt; mycmappt++;
		mypept[i].peBlue = (BYTE) *mycmappt; mycmappt++;
		mypept[i].peFlags = 0;
	}
}

static int MapHighTo8 (void)
{
int i, j;
char * tempblockgfxpt;
unsigned char * temp24bitbuffer, * blkgfxpt;
HDC temphdc, myhdc;
HBITMAP htempbm, holdbm;
BITMAPINFOHEADER bih;
RGBQUAD rgbq;

/* This isn't needed, but left for reference
	if (mapdepth == 8)
	{
		memset (&tempbm, 0, sizeof(BITMAP));
		tempbm.bmWidth = mapblockwidth;
		tempbm.bmHeight = mapblockheight;
		tempbm.bmWidthBytes = mapblockwidth;
		tempbm.bmPlanes = 1;
		tempbm.bmBitsPixel = mapdepth;
	} else {
*/
	memset (&bih, 0, sizeof(BITMAPINFOHEADER));
	memset (&rgbq, 0, sizeof(RGBQUAD));
	bih.biSize = sizeof (BITMAPINFOHEADER);
	bih.biWidth = mapblockwidth;
	bih.biHeight = -mapblockheight;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;

	htempbm = NULL;
	temp24bitbuffer = (unsigned char *) malloc (mapblockwidth*mapblockheight*3);
	if (temp24bitbuffer == NULL) return -1;
/* This isn't needed, but left for reference
	}
*/
	tempblockgfxpt = mapblockgfxpt;
	temphdc = CreateCompatibleDC (NULL);
	for (i=0;i<mapnumblockgfx;i++)
	{
		if ((maplpDDSTiles[i]->GetDC (&myhdc)) != DD_OK) return -1;
		switch (mapdepth)
		{
/* This isn't needed, but left for reference
		case 8:
			tempbm.bmBits = tempblockgfxpt;
			htempbm = CreateBitmapIndirect (&tempbm);
			if (htempbm == NULL) return -1;
			break;
*/
		case 15:
			blkgfxpt = (unsigned char *) tempblockgfxpt;
			for (j=0;j<(mapblockwidth*mapblockheight*3);j+=3)
			{
				temp24bitbuffer[0+j] = (unsigned char) ((*(blkgfxpt+1)&0x1F)<<3);
				temp24bitbuffer[1+j] = (unsigned char) (((((*blkgfxpt)&0x3)<<3)|((*(blkgfxpt+1))>>5))<<3);
				temp24bitbuffer[2+j] = (unsigned char) (((*blkgfxpt)&0x7C)<<1);
				blkgfxpt += 2;
			}
			htempbm = CreateDIBitmap (myhdc, &bih, CBM_INIT,
				 temp24bitbuffer, (BITMAPINFO *) &bih, DIB_RGB_COLORS);
			if (htempbm == NULL) return -1;
			break;
		case 16:
			blkgfxpt = (unsigned char *) tempblockgfxpt;
			for (j=0;j<(mapblockwidth*mapblockheight*3);j+=3)
			{
				temp24bitbuffer[0+j] = (unsigned char) ((*(blkgfxpt+1)&0x1F)<<3);
				temp24bitbuffer[1+j] = (unsigned char) (((((*blkgfxpt)&0x7)<<3)|((*(blkgfxpt+1))>>5))<<2);
				temp24bitbuffer[2+j] = (unsigned char) ((*blkgfxpt)&0xF8);
				blkgfxpt += 2;
			}
			htempbm = CreateDIBitmap (myhdc, &bih, CBM_INIT,
				 temp24bitbuffer, (BITMAPINFO *) &bih, DIB_RGB_COLORS);
			if (htempbm == NULL) return -1;
			break;
		case 24:
			blkgfxpt = (unsigned char *) tempblockgfxpt;
			for (j=0;j<(mapblockwidth*mapblockheight*3);j+=3)
			{
				temp24bitbuffer[2+j] = *blkgfxpt; blkgfxpt++;
				temp24bitbuffer[1+j] = *blkgfxpt; blkgfxpt++;
				temp24bitbuffer[0+j] = *blkgfxpt; blkgfxpt++;
			}
			htempbm = CreateDIBitmap (myhdc, &bih, CBM_INIT,
				 temp24bitbuffer, (BITMAPINFO *) &bih, DIB_RGB_COLORS);
			if (htempbm == NULL) return -1;
			break;
		case 32:
			blkgfxpt = (unsigned char *) tempblockgfxpt;
			for (j=0;j<(mapblockwidth*mapblockheight*3);j+=3)
			{
				blkgfxpt ++;
				temp24bitbuffer[2+j] = *blkgfxpt; blkgfxpt++;
				temp24bitbuffer[1+j] = *blkgfxpt; blkgfxpt++;
				temp24bitbuffer[0+j] = *blkgfxpt; blkgfxpt++;
			}
			htempbm = CreateDIBitmap (myhdc, &bih, CBM_INIT,
				 temp24bitbuffer, (BITMAPINFO *) &bih, DIB_RGB_COLORS);
			if (htempbm == NULL) return -1;
			break;
		}
		holdbm = (HBITMAP) SelectObject (temphdc, htempbm);
		BitBlt (myhdc, 0, 0, mapblockwidth, mapblockheight, temphdc, 0, 0, SRCCOPY);
		SelectObject (temphdc, holdbm);
		DeleteObject (htempbm);
		maplpDDSTiles[i]->ReleaseDC(myhdc);
		tempblockgfxpt += mapblockwidth*mapblockheight*((mapdepth+1)/8);
	}
	DeleteDC (temphdc);
/* This line was replaced by the one below
	if (mapdepth != 8) free (temp24bitbuffer);
*/
	free (temp24bitbuffer);
	return 0;
}

static int MapCopyBlocksToSurfaces (void)
{
unsigned int i, j, k, rshift, gshift, bshift, rbits, gbits, bbits, rval, gval, bval;
unsigned char * tempblockgfxpt, * tempsurfacept;
RBSURFACEDESC mddsd;
DDPIXELFORMAT mddpf;

	memset (&mddpf, 0, sizeof(DDPIXELFORMAT));
	mddpf.dwSize = sizeof(DDPIXELFORMAT);
	if (maplpDDSTiles[0]->GetPixelFormat(&mddpf)!=DD_OK) return -1;
	if (mddpf.dwRGBBitCount>8) {
		j = (int) mddpf.dwRBitMask; rshift = 0;
		i = 1; while (!(i&j)) { rshift++; i<<=1; }
		rbits = 0; while (i&j) { rbits++; i<<=1; }
		j = (int) mddpf.dwGBitMask; gshift = 0;
		i = 1; while (!(i&j)) { gshift++; i<<=1; }
		gbits = 0; while (i&j) { gbits++; i<<=1; }
		j = (int) mddpf.dwBBitMask; bshift = 0;
		i = 1; while (!(i&j)) { bshift++; i<<=1; }
		bbits = 0; while (i&j) { bbits++; i<<=1; }
	} else {
		if (mapdepth > 8 && mapaltdepth == 8) {
			free (mapblockgfxpt);
			mapblockgfxpt = mapaltblockgfxpt;
			mapaltblockgfxpt = NULL;
			mapdepth = 8;
		}
		rbits = gbits = bbits = 0;
		rshift = gshift = bshift = 0;
	}

	tempblockgfxpt = (unsigned char *) mapblockgfxpt;
	if (mapdepth!=8 && mddpf.dwRGBBitCount==8) MapHighTo8 ();
	if (mapdepth==8 && mddpf.dwRGBBitCount==8) {
		for (i=0;i<((unsigned int)mapnumblockgfx);i++)
		{
			memset (&mddsd, 0, sizeof(RBSURFACEDESC));
			mddsd.dwSize = sizeof(RBSURFACEDESC);
			if (maplpDDSTiles[i]->Lock (NULL, &mddsd, DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_SURFACEMEMORYPTR, NULL)!=DD_OK) return -1;
			tempsurfacept = (unsigned char *) mddsd.lpSurface;
			for (j=mapblockheight;j>0;j--)
			{
				memcpy (tempsurfacept, tempblockgfxpt, mapblockwidth);
				tempsurfacept += mddsd.lPitch;
				tempblockgfxpt += mapblockwidth;
			}
			maplpDDSTiles[i]->Unlock (NULL);
		}
	} else {

	for (i=0;i<((unsigned int)mapnumblockgfx);i++)
	{
		memset (&mddsd, 0, sizeof(RBSURFACEDESC));
		mddsd.dwSize = sizeof(RBSURFACEDESC);
		if (maplpDDSTiles[i]->Lock (NULL, &mddsd, DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_SURFACEMEMORYPTR, NULL)!=DD_OK) return -1;
		for (j=0;j<((unsigned int)mapblockheight);j++) {
		tempsurfacept = (unsigned char *) mddsd.lpSurface;
		tempsurfacept += j*mddsd.lPitch;
		for (k=mapblockwidth;k>0;k--) {
		switch (mapdepth) {
			case 8:
				if (rb8bittopink == 0 || tempblockgfxpt[0] != maptrans8) {
					rval = mapcmappt[tempblockgfxpt[0]*3];
					rval >>= (8-rbits);
					gval = mapcmappt[tempblockgfxpt[0]*3+1];
					gval >>= (8-gbits);
					bval = mapcmappt[tempblockgfxpt[0]*3+2];
					bval >>= (8-bbits);
				} else {
					rval = (unsigned int) maptransred;
					rval >>= (8-rbits);
					gval = (unsigned int) maptransgreen;
					gval >>= (8-gbits);
					bval = (unsigned int) maptransblue;
					bval >>= (8-bbits);
				}
				tempblockgfxpt++;
				break;
			case 15:
				rval = ((((unsigned long int)tempblockgfxpt[0])<<8)|((unsigned long int)tempblockgfxpt[1]))&0x7C00;
				rval >>= (7+(8-rbits));
				gval = ((((unsigned long int)tempblockgfxpt[0])<<8)|((unsigned long int)tempblockgfxpt[1]))&0x03E0;
				gval >>= (2+(8-gbits));
				bval = ((((unsigned long int)tempblockgfxpt[0])<<8)|((unsigned long int)tempblockgfxpt[1]))&0x001F;
				bval <<= 3;
				bval >>= (8-bbits);
				tempblockgfxpt += 2;
				break;
			case 16:
				rval = ((((unsigned long int)tempblockgfxpt[0])<<8)|((unsigned long int)tempblockgfxpt[1]))&0xF800;
				rval >>= (8+(8-rbits));
				gval = ((((unsigned long int)tempblockgfxpt[0])<<8)|((unsigned long int)tempblockgfxpt[1]))&0x07E0;
				gval >>= (3+(8-gbits));
				bval = ((((unsigned long int)tempblockgfxpt[0])<<8)|((unsigned long int)tempblockgfxpt[1]))&0x001F;
				bval <<= 3;
				bval >>= (8-bbits);
				tempblockgfxpt += 2;
				break;
			case 24:
				rval = (unsigned long int) tempblockgfxpt[0];
				rval >>= (8-rbits);
				gval = (unsigned long int) tempblockgfxpt[1];
				gval >>= (8-gbits);
				bval = (unsigned long int) tempblockgfxpt[2];
				bval >>= (8-bbits);
				tempblockgfxpt += 3;
				break;
			case 32:
				rval = (unsigned long int) tempblockgfxpt[1];
				rval >>= (8-rbits);
				gval = (unsigned long int) tempblockgfxpt[2];
				gval >>= (8-gbits);
				bval = (unsigned long int) tempblockgfxpt[3];
				bval >>= (8-bbits);
				tempblockgfxpt += 4;
				break;
			default:
				return -1;
		}
		if (rbits < 8) rval |= (rval>>rbits);
		if (gbits < 8) gval |= (gval>>gbits);
		if (bbits < 8) bval |= (bval>>bbits);
		rval <<= rshift; rval &= mddpf.dwRBitMask;
		gval <<= gshift; gval &= mddpf.dwGBitMask;
		bval <<= bshift; bval &= mddpf.dwBBitMask;
		switch (mddpf.dwRGBBitCount) {
/* Apparently 16bits are reported even in 555 (15bit) mode */
			case 16:
				*((unsigned short int *)tempsurfacept) = (unsigned short int) (rval|gval|bval);
				tempsurfacept += 2;
				break;
			case 24:
				tempsurfacept[2] = (unsigned char) (((rval|gval|bval)>>16)&0xFF);
				tempsurfacept[1] = (unsigned char) (((rval|gval|bval)>>8)&0xFF);
				tempsurfacept[0] = (unsigned char) ((rval|gval|bval)&0xFF);
				tempsurfacept += 3;
				break;
			case 32:
				tempsurfacept[3] = (unsigned char) (((rval|gval|bval)>>24)&0xFF);
				tempsurfacept[2] = (unsigned char) (((rval|gval|bval)>>16)&0xFF);
				tempsurfacept[1] = (unsigned char) (((rval|gval|bval)>>8)&0xFF);
				tempsurfacept[0] = (unsigned char) ((rval|gval|bval)&0xFF);
				tempsurfacept += 4;
				break;
		}
	} }
		maplpDDSTiles[i]->Unlock (NULL);
	}
	}
	return 0;
}

extern "C" DLLIMPORT int MapRestore (void)
{
int i;

	i = 0;
	while (maplpDDSTiles[i] != NULL)
	{
		if (maplpDDSTiles[i]->Restore() != DD_OK) return -1;
		i++;
	}
	return MapCopyBlocksToSurfaces ();
}

static DWORD MapTranspToPixel (LPDDPIXELFORMAT lpDDpf)
{
DWORD transpresult;
unsigned int i, j, rshift, gshift, bshift, rbits, gbits, bbits;

		transpresult = 0;
		if (lpDDpf->dwRGBBitCount>8) {
			j = (int) lpDDpf->dwRBitMask; rshift = 0;
			i = 1; while (!(i&j)) { rshift++; i<<=1; }
			rbits = 0; while (i&j) { rbits++; i<<=1; }
			j = (int) lpDDpf->dwGBitMask; gshift = 0;
			i = 1; while (!(i&j)) { gshift++; i<<=1; }
			gbits = 0; while (i&j) { gbits++; i<<=1; }
			j = (int) lpDDpf->dwBBitMask; bshift = 0;
			i = 1; while (!(i&j)) { bshift++; i<<=1; }
			bbits = 0; while (i&j) { bbits++; i<<=1; }

			transpresult = (((maptransred<<rshift)>>(8-rbits))&lpDDpf->dwRBitMask) |
								(((maptransgreen<<gshift)>>(8-gbits))&lpDDpf->dwGBitMask) |
								(((maptransblue<<bshift)>>(8-bbits))&lpDDpf->dwBBitMask);
		}
		return transpresult;
}

static int MapRelocate (RBDIRECTDRAW lpDD)
{
int i, j, k;
BLKSTR * myblkstrpt;
RBSURFACEDESC mddsd;
DDPIXELFORMAT mddpf;
DDCOLORKEY ddck;
DWORD transpcol;
char * novcarray;
char ascnum[64];

	memset (&mddsd, 0, sizeof (mddsd));
	memset (&mddpf, 0, sizeof (mddpf));
	memset (&ddck, 0, sizeof (ddck));

	mddpf.dwSize = sizeof (mddpf);

	transpcol = 0;

	novcarray = (char *) malloc (mapnumblockgfx);
	memset (novcarray, 0, mapnumblockgfx);
	i = 0; while (mapnovctext[i] != 0) {
		j = 0; while (mapnovctext[i] >= '0' && mapnovctext[i] <= '9') {
			ascnum[j] = mapnovctext[i];
			i++; j++;
		}
		ascnum[j] = 0;
		k = atoi(ascnum);
		if (k < 0 || k >= mapnumblockgfx) break;
		if (mapnovctext[i] == '-') {
			i++;
			j = 0; while (mapnovctext[i] >= '0' && mapnovctext[i] <= '9') {
				ascnum[j] = mapnovctext[i];
				i++; j++;
			}
			ascnum[j] = 0;
			j = atoi(ascnum);
			if (j < k || j >= mapnumblockgfx) break;
			while (k <= j) {
				novcarray[k] = 1; k++;
			}
		} else {
			novcarray[k] = 1;
		}
		if (mapnovctext[i] == ',') i++;
	}

	maplpDDSTiles = (RBDIRECTDRAWSURFACE *) malloc ((mapnumblockgfx+2)*sizeof(RBDIRECTDRAWSURFACE));
	for (i=0;i<mapnumblockgfx;i++)
	{
		mddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		if (novcarray[i] == 0)
			mddsd.ddsCaps.dwCaps = rbcapstype;
		else
			mddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;
		mddsd.dwHeight = mapblockheight;
		mddsd.dwWidth = mapblockwidth;
		mddsd.dwSize = sizeof (mddsd);
		if (lpDD->CreateSurface (&mddsd, &maplpDDSTiles[i], NULL)!= DD_OK)
			{ MapFreeMem (); return -1; }

		if (!i) {
			if (maplpDDSTiles[i]->GetPixelFormat(&mddpf)!=DD_OK) { MapFreeMem (); return -1; }
			if (mddpf.dwRGBBitCount!=8) { transpcol = MapTranspToPixel (&mddpf); }
			else transpcol = (DWORD) maptrans8;
		}
		ddck.dwColorSpaceLowValue = transpcol;
		ddck.dwColorSpaceHighValue = transpcol;
		maplpDDSTiles[i]->SetColorKey( DDCKEY_SRCBLT, &ddck );
	}
	maplpDDSTiles[i] = NULL;
	free (novcarray);
	if (MapCopyBlocksToSurfaces ()) { MapFreeMem (); return -1; }

	i = mapnumblockstr;
	myblkstrpt = (BLKSTR *) mapblockstrpt;
	while (i)
	{
		myblkstrpt->bgoff = maplpDDSTiles[(((long int) myblkstrpt->bgoff))];
		if (myblkstrpt->fgoff!=0) myblkstrpt->fgoff = maplpDDSTiles[(((long int) myblkstrpt->fgoff))];
		if (myblkstrpt->fgoff2!=0) myblkstrpt->fgoff2 = maplpDDSTiles[(((long int) myblkstrpt->fgoff2))];
		if (myblkstrpt->fgoff3!=0) myblkstrpt->fgoff3 = maplpDDSTiles[(((long int) myblkstrpt->fgoff3))];
		myblkstrpt++; i--;
	}

	return 0;
}

static int MapGetchksz (unsigned char * locpt)
{
	return ((((int) (locpt[0]))<<24)|(((int) (locpt[1]))<<16)|
		(((int) (locpt[2]))<<8)|((int) (locpt[3])));
}

static int MapGetshort (unsigned char * locpt)
{
int rval;

	if (mapislsb)
	rval = ((((int) (locpt[1]))<<8)|((int) (locpt[0])));
	else
	rval = ((((int) (locpt[0]))<<8)|((int) (locpt[1])));
	if (rval & 0x8000) rval -= 0x10000;
	return rval;
}

static int MapGetlong (unsigned char * locpt)
{
	if (mapislsb)
	return ((((int) (locpt[3]))<<24)|(((int) (locpt[2]))<<16)|
		(((int) (locpt[1]))<<8)|((int) (locpt[0])));
	else
	return ((((int) (locpt[0]))<<24)|(((int) (locpt[1]))<<16)|
		(((int) (locpt[2]))<<8)|((int) (locpt[3])));
}

static int MapDecodeMPHD (unsigned char * mdatpt)
{
	mdatpt += 8;
	if (mdatpt[0] > 1) { maperror = MER_MAPTOONEW; return -1; }
	if (mdatpt[2] == 1) mapislsb = 1; else mapislsb = 0;

	maptype = (int) mdatpt[3];
	if (maptype > 3) { maperror = MER_MAPTOONEW; return -1; }
	mapwidth = (short) MapGetshort (mdatpt+4);
	mapheight = (short) MapGetshort (mdatpt+6);
	mapblockwidth = (short) MapGetshort (mdatpt+12);
	mapblockheight = (short) MapGetshort (mdatpt+14);
	mapdepth = (short) MapGetshort (mdatpt+16);
	mapaltdepth = mapdepth;
	mapblockstrsize = (short) MapGetshort (mdatpt+18);
	mapnumblockstr = (short) MapGetshort (mdatpt+20);
	mapnumblockgfx = (short) MapGetshort (mdatpt+22);

#ifndef MOVERIDETRANSP
	if (MapGetchksz (mdatpt-4) > 24) {
		maptrans8 = (int) mdatpt[24];
		maptransred = (int) mdatpt[25];
		maptransgreen = (int) mdatpt[26];
		maptransblue = (int) mdatpt[27];
	} else {
#endif
		maptrans8 = MTRANSP8;
		maptransred = MTRANSPR;
		maptransgreen = MTRANSPG;
		maptransblue = MTRANSPB;
#ifndef MOVERIDETRANSP
	}
#endif

	if (MapGetchksz (mdatpt-4) > 28) {
		mapblockgapx = (int) MapGetshort (mdatpt+28);
		mapblockgapy = (int) MapGetshort (mdatpt+30);
		mapblockstaggerx = (int) MapGetshort (mdatpt+32);
		mapblockstaggery = (int) MapGetshort (mdatpt+34);
	} else {
		mapblockgapx = (int) mapblockwidth;
		mapblockgapy = (int) mapblockheight;
		mapblockstaggerx = 0;
		mapblockstaggery = 0;
	}
	if (MapGetchksz (mdatpt-4) > 36) mapclickmask = (short) MapGetshort (mdatpt+36);
	else mapclickmask = 0;

	return 0;
}

/*
int DecodeEDHD (unsigned char * mdatpt)
{
int i, j;
short int * mybrshpt;
char * mynamept;
short int * mybrsh2pt;

	mdatpt += 8;
	xmapoffset = (short) MapGetshort (mdatpt);
	ymapoffset = (short) MapGetshort (mdatpt+2);
	fgcolour = (int) MapGetlong (mdatpt+4);
	bgcolour = (int) MapGetlong (mdatpt+8);
	swidth = (short) MapGetshort (mdatpt+12);
	sheight = (short) MapGetshort (mdatpt+14);
	strtstr = (short) MapGetshort (mdatpt+16);
	strtblk = (short) MapGetshort (mdatpt+18);
	curstr = (short) MapGetshort (mdatpt+20);
	curanim = (short) MapGetshort (mdatpt+22); curanim = -1;
	animspd = (short) MapGetshort (mdatpt+24);
	span = (short) MapGetshort (mdatpt+26);
	numbrushes = (short) MapGetshort (mdatpt+28);
	if (clickmask == 0) {
		clickmask = (int) MapGetshort (mdatpt+30);
		if (clickmask<0 || clickmask >= numblockgfx) clickmask = 0;
	}

	if (numbrushes>0)
	{
	mybrshpt =(short int *) (mdatpt+32);
		for (i=0;i<8;i++)
		{
			j = *mybrshpt; j *= *(mybrshpt+1); j *= 2; j += 4;
			brshpt[i] = malloc (j); j /= 2; mybrsh2pt = brshpt[i];
			*mybrsh2pt = *mybrshpt;
			mybrsh2pt++; mybrshpt++;
			*mybrsh2pt = *mybrshpt;
			mybrsh2pt++; mybrshpt++;
			j -= 2;
			if (maptype == 0) {
				while (j) {
					j--;
					*mybrsh2pt = *mybrshpt;
					if (*mybrsh2pt >= 0) *mybrsh2pt /= BLOCKSTRSIZE;
					mybrsh2pt++; mybrshpt++;
				}
			} else {
				while (j) {
					j--;
					*mybrsh2pt = *mybrshpt;
					if (*mybrsh2pt < 0) *mybrsh2pt *= sizeof(ANISTR);
					mybrsh2pt++; mybrshpt++;
				}
			}
			numbrushes--; if (!numbrushes) i=8;
		}
	mynamept = (char *) mybrshpt;
	if ((MapGetchksz (mdatpt-4)) > (mynamept-((char *) mdatpt))) {
		for (i=0;i<8;i++) {
			if (brshpt[i] != NULL) {
				strcpy (brshname[i], mynamept);
				mynamept += strlen (mynamept);
				mynamept++;
			}
		}
	}
	}

	return 0;
}

int DecodeATHR (unsigned char * mdatpt)
{
int i, j;

	mdatpt += 8;
	i = 0; while (i < MapGetchksz (mdatpt-4))
	{
		authorname[i]=mdatpt[i];
		if (mdatpt[i]==0) break;
		i++;
	}
	i++; j = 0;
	while (i < MapGetchksz (mdatpt-4))
	{
		authorinfo1[j]=mdatpt[i];
		if (mdatpt[i]==0) break;
		i++; j++;
	}
	i++; j = 0;
	while (i < MapGetchksz (mdatpt-4))
	{
		authorinfo2[j]=mdatpt[i];
		if (mdatpt[i]==0) break;
		i++; j++;
	}
	i++; j = 0;
	while (i < MapGetchksz (mdatpt-4))
	{
		authorinfo3[j]=mdatpt[i];
		if (mdatpt[i]==0) break;
		i++; j++;
	}
	return 0;
}
*/

static int MapDecodeCMAP (unsigned char * mdatpt)
{
	mdatpt += 8;
	mapcmappt = (unsigned char *) malloc (MapGetchksz (mdatpt-4));
	if (mapcmappt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	memcpy (mapcmappt, mdatpt, MapGetchksz (mdatpt-4));
	MapCMAPtoPE	(mapcmappt, mappept);
	return 0;
}

static int MapDecodeBKDT (unsigned char * mdatpt)
{
int i, j;
BLKSTR * myblkpt;

	mdatpt += 8;
	mapblockstrpt = (char *) malloc (mapnumblockstr*sizeof(BLKSTR));
	if (mapblockstrpt==NULL) { maperror = MER_OUTOFMEM; return -1; }

	myblkpt = (BLKSTR *) mapblockstrpt;
	j = MapGetchksz (mdatpt-4);
	i = 0; while (i < (mapnumblockstr*mapblockstrsize)) {
		myblkpt->bgoff = (RBDIRECTDRAWSURFACE) MapGetlong (mdatpt+i);
		myblkpt->fgoff = (RBDIRECTDRAWSURFACE) MapGetlong (mdatpt+i+4);
		myblkpt->fgoff2 = (RBDIRECTDRAWSURFACE) MapGetlong (mdatpt+i+8);
		myblkpt->fgoff3 = (RBDIRECTDRAWSURFACE) MapGetlong (mdatpt+i+12);
		if (maptype == 0) {
			myblkpt->bgoff = (RBDIRECTDRAWSURFACE) (((int) myblkpt->bgoff) / (mapblockwidth*mapblockheight*((mapdepth+1)/8)));
			myblkpt->fgoff = (RBDIRECTDRAWSURFACE) (((int) myblkpt->fgoff) / (mapblockwidth*mapblockheight*((mapdepth+1)/8)));
			myblkpt->fgoff2 = (RBDIRECTDRAWSURFACE) (((int) myblkpt->fgoff2) / (mapblockwidth*mapblockheight*((mapdepth+1)/8)));
			myblkpt->fgoff3 = (RBDIRECTDRAWSURFACE) (((int) myblkpt->fgoff3) / (mapblockwidth*mapblockheight*((mapdepth+1)/8)));
		}
		myblkpt->user1 = (unsigned int) MapGetlong (mdatpt+i+16);
		myblkpt->user2 = (unsigned int) MapGetlong (mdatpt+i+20);
		myblkpt->user3 = (unsigned short int) MapGetshort (mdatpt+i+24);
		myblkpt->user4 = (unsigned short int) MapGetshort (mdatpt+i+26);
		myblkpt->user5 = mdatpt[i+28];
		myblkpt->user6 = mdatpt[i+29];
		myblkpt->user7 = mdatpt[i+30];
		if (mdatpt[i+31]&0x80) myblkpt->unused3 = 1; else myblkpt->unused3 = 0;
		if (mdatpt[i+31]&0x40) myblkpt->unused2 = 1; else myblkpt->unused2 = 0;
		if (mdatpt[i+31]&0x20) myblkpt->unused1 = 1; else myblkpt->unused1 = 0;
		if (mdatpt[i+31]&0x10) myblkpt->trigger = 1; else myblkpt->trigger = 0;
		if (mdatpt[i+31]&0x08) myblkpt->br = 1; else myblkpt->br = 0;
		if (mdatpt[i+31]&0x04) myblkpt->bl = 1; else myblkpt->bl = 0;
		if (mdatpt[i+31]&0x02) myblkpt->tr = 1; else myblkpt->tr = 0;
		if (mdatpt[i+31]&0x01) myblkpt->tl = 1; else myblkpt->tl = 0;
		i += mapblockstrsize;
		myblkpt++;
	}
	return 0;
}

static int MapDecodeANDT (unsigned char * mdatpt)
{
int numani, i, ancksz;
unsigned char * mdatendpt;

	mdatpt += 8;
	ancksz = MapGetchksz(mdatpt-4);
	mdatendpt = mdatpt+ancksz;

	numani = 0; while (1) {
		mdatendpt -= 16;
		numani++;
		if (*mdatendpt == 255) break;
	}

	mapanimseqpt = (int *) malloc (((mdatendpt-mdatpt)/4)*sizeof(int));
	if (mapanimseqpt == NULL) { maperror = MER_OUTOFMEM; return -1; }
	i = 0; while (mdatpt != mdatendpt) {
		mapanimseqpt[i] = MapGetlong (mdatpt);
		if (maptype == 0) mapanimseqpt[i] /= mapblockstrsize;
		mdatpt += 4; i++;
	}

	mapanimstrpt = (ANISTR *) malloc (numani*sizeof(ANISTR));
	if (mapanimstrpt == NULL) { maperror = MER_OUTOFMEM; return -1; }
	mapanimstrendpt = mapanimstrpt;
	mapanimstrendpt += numani;

	i = 0; while (i<numani) {
		mapanimstrpt[i].antype = mdatendpt[0];
		mapanimstrpt[i].andelay = mdatendpt[1];
		mapanimstrpt[i].ancount = mdatendpt[2];
		mapanimstrpt[i].anuser = mdatendpt[3];
		if (maptype == 0) {
			mapanimstrpt[i].ancuroff = (int) ((MapGetlong (mdatendpt+4)+ancksz)/4);
			mapanimstrpt[i].anstartoff = (int) ((MapGetlong (mdatendpt+8)+ancksz)/4);
			mapanimstrpt[i].anendoff = (int) ((MapGetlong (mdatendpt+12)+ancksz)/4);
		} else {
			mapanimstrpt[i].ancuroff = (int) MapGetlong (mdatendpt+4);
			mapanimstrpt[i].anstartoff = (int) MapGetlong (mdatendpt+8);
			mapanimstrpt[i].anendoff = (int) MapGetlong (mdatendpt+12);
		}
		mdatendpt += 16; i++;
	}

	MapInitAnims ();
	return 0;
}

static int MapDecodeAGFX (unsigned char * mdatpt)
{
//	if (bitmap_color_depth (screen) > 8) return 0;
//	if (mapblockgfxpt != NULL) free (mapblockgfxpt);
	mapaltblockgfxpt = (char *) malloc (MapGetchksz (mdatpt+4));
	if (mapaltblockgfxpt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	memcpy (mapaltblockgfxpt, mdatpt+8, MapGetchksz(mdatpt+4));
	mapaltdepth = 8;
	return 0;
}

static int MapDecodeBGFX (unsigned char * mdatpt)
{
//	if (mapblockgfxpt != NULL) return 0;
	mapblockgfxpt = (char *) malloc (MapGetchksz (mdatpt+4));
	if (mapblockgfxpt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	memcpy (mapblockgfxpt, mdatpt+8, MapGetchksz(mdatpt+4));
	return 0;
}

static int MapDecodeNOVC (unsigned char * mdatpt)
{
	memset (mapnovctext, 0, 70);
	if (MapGetchksz ((unsigned char *) mdatpt+4) < 70) strcpy (mapnovctext, (char *) mdatpt+8);
	return 0;
}

static int MapDecodeLayer (unsigned char * mdatpt, int lnum)
{
int i, j, k, l;
short int * mymappt, * mymap2pt;

	mapmappt[lnum] = (short int *) malloc (mapwidth*mapheight*sizeof(short int));
	if (mapmappt[lnum] == NULL) { maperror = MER_OUTOFMEM; return -1; }

	mdatpt += 8;
	mymappt = mapmappt[lnum];
	if (maptype == 0) {
	for (j=0;j<mapheight;j++) {
		for (i=0;i<mapwidth;i++) {
			*mymappt = (short int) MapGetshort (mdatpt);
			if (*mymappt >= 0) { *mymappt /= mapblockstrsize; }
			else { *mymappt /= 16; }
			mdatpt+=2; mymappt++;
		}
	}
	} else {
	if (maptype == 1) {
	for (j=0;j<mapheight;j++) {
		for (i=0;i<mapwidth;i++) {
			*mymappt = (short int) MapGetshort (mdatpt);
			mdatpt+=2; mymappt++;
		}
	}
	} else {
	if (maptype == 2) {
	for (j=0;j<mapheight;j++) {
		for (i=0;i<mapwidth;) {
			k = (int) MapGetshort (mdatpt);
			mdatpt += 2;
			if (k > 0) {
				while (k) {
					*mymappt = (short int) MapGetshort (mdatpt);
					mymappt++; mdatpt += 2;
					i++; k--;
				}
			} else {
			if (k < 0) {
				l = (int) MapGetshort (mdatpt); mdatpt += 2;
				while (k) {
					*mymappt = (short int) l;
					mymappt++;
					i++; k++;
				}
			} else {
			} }
		}
	}
	} else {
	if (maptype == 3) {
	for (j=0;j<mapheight;j++) {
		for (i=0;i<mapwidth;) {
			k = (int) MapGetshort (mdatpt);
			mdatpt += 2;
			if (k > 0) {
				while (k) {
					*mymappt = (short int) MapGetshort (mdatpt);
					mymappt++; mdatpt += 2;
					i++; k--;
				}
			} else {
			if (k < 0) {
				mymap2pt = mymappt + (int) MapGetshort (mdatpt); mdatpt += 2;
				while (k) {
					*mymappt = *mymap2pt;
					mymappt++; mymap2pt++;
					i++; k++;
				}
			} else {
			} }
		}
	}
	} } } }

	if (lnum == 0) { mappt = mapmappt[lnum]; }
	return 0;
}

static int MapDecodeNULL (unsigned char * mdatpt)
{
	return 0;
}

static int MapRealDecode (FILE * mfpt, unsigned char * mmpt, long int mpfilesize, RBDIRECTDRAW lpDD)
{
int chkdn;
unsigned char * fmappospt;
char mphdr[8];

	mpfilesize -= 12;

	while (mpfilesize > 0) {

		if (mfpt != NULL) {
			if (fread (mphdr, 1, 8, mfpt) != 8) {
				maperror = MER_MAPLOADERROR;
				MapFreeMem ();
				return -1;
			}
			fmappospt = (unsigned char *) malloc (MapGetchksz((unsigned char *) mphdr+4)+8);
			if (fmappospt == NULL) {
				maperror = MER_OUTOFMEM;
				MapFreeMem ();
				return -1;
			}
			memcpy (fmappospt, mphdr, 8);
			if (fread (fmappospt+8, 1, MapGetchksz((unsigned char *) mphdr+4),
				mfpt) != (unsigned int) MapGetchksz((unsigned char *) mphdr+4)) {
				maperror = MER_MAPLOADERROR;
				MapFreeMem ();
				return -1;
			}
		} else {
			fmappospt = mmpt;
			mmpt += MapGetchksz(mmpt+4);
			mmpt += 8;
		}

		chkdn = 0;
		if (!strncmp ((char *) fmappospt, "MPHD", 4)) { chkdn = 1; MapDecodeMPHD (fmappospt); }
/*		if (!strncmp ((char *) fmappospt, "ATHR", 4)) { chkdn = 1; MapDecodeATHR (fmappospt); }
		if (!strncmp ((char *) fmappospt, "EDHD", 4)) { chkdn = 1; MapDecodeEDHD (fmappospt); }
*/
		if (!strncmp ((char *) fmappospt, "CMAP", 4)) { chkdn = 1; MapDecodeCMAP (fmappospt); }
		if (!strncmp ((char *) fmappospt, "BKDT", 4)) { chkdn = 1; MapDecodeBKDT (fmappospt); }
		if (!strncmp ((char *) fmappospt, "ANDT", 4)) { chkdn = 1; MapDecodeANDT (fmappospt); }
		if (!strncmp ((char *) fmappospt, "AGFX", 4)) { chkdn = 1; MapDecodeAGFX (fmappospt); }
		if (!strncmp ((char *) fmappospt, "BGFX", 4)) { chkdn = 1; MapDecodeBGFX (fmappospt); }
		if (!strncmp ((char *) fmappospt, "NOVC", 4)) { chkdn = 1; MapDecodeNOVC (fmappospt); }
		if (!strncmp ((char *) fmappospt, "BODY", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 0); }
		if (!strncmp ((char *) fmappospt, "LYR1", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 1); }
		if (!strncmp ((char *) fmappospt, "LYR2", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 2); }
		if (!strncmp ((char *) fmappospt, "LYR3", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 3); }
		if (!strncmp ((char *) fmappospt, "LYR4", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 4); }
		if (!strncmp ((char *) fmappospt, "LYR5", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 5); }
		if (!strncmp ((char *) fmappospt, "LYR6", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 6); }
		if (!strncmp ((char *) fmappospt, "LYR7", 4)) { chkdn = 1; MapDecodeLayer (fmappospt, 7); }
		if (!chkdn) MapDecodeNULL (fmappospt);

		mpfilesize -= 8;
		mpfilesize -= MapGetchksz (fmappospt+4);
		if (mfpt != NULL) free (fmappospt);

		if (maperror != MER_NONE) { MapFreeMem (); return -1; }
	}

	return MapRelocate (lpDD);
}

extern "C" DLLIMPORT int MapLoad (char * mapname, RBDIRECTDRAW lpDD)
{
int mretval;
char idtag[4];
unsigned char tempc;
int mapfilesize = 0;

	MapFreeMem ();
	maperror = MER_NONE;

	mapfilept = fopen (mapname, "rb");
	if (mapfilept==NULL) { maperror = MER_NOOPEN; return -1; }

	maperror = MER_MAPLOADERROR;
	if (fread (&idtag[0], 1, 1, mapfilept) == 1) {
	if (fread (&idtag[1], 1, 1, mapfilept) == 1) {
	if (fread (&idtag[2], 1, 1, mapfilept) == 1) {
	if (fread (&idtag[3], 1, 1, mapfilept) == 1) {
	if (fread (&tempc, 1, 1, mapfilept) == 1) {
	mapfilesize = (((int) tempc)<<24);
	if (fread (&tempc, 1, 1, mapfilept) == 1) {
	mapfilesize |= (((int) tempc)<<16);
	if (fread (&tempc, 1, 1, mapfilept) == 1) {
	mapfilesize |= (((int) tempc)<<8);
	if (fread (&tempc, 1, 1, mapfilept) == 1) {
	mapfilesize |= (((int) tempc));
	mapfilesize += 8;
	if (!strncmp (idtag, "FORM", 4)) {
	if (fread (&idtag[0], 1, 1, mapfilept) == 1) {
	if (fread (&idtag[1], 1, 1, mapfilept) == 1) {
	if (fread (&idtag[2], 1, 1, mapfilept) == 1) {
	if (fread (&idtag[3], 1, 1, mapfilept) == 1) {
	if (!strncmp (idtag, "FMAP", 4)) maperror = MER_NONE;
	} } } } }
	} } } } } } } }

	if (maperror != MER_NONE) { fclose (mapfilept); return -1; }

	mretval = MapRealDecode (mapfilept, NULL, mapfilesize, lpDD);
	fclose (mapfilept);

	return mretval;
}

extern "C" DLLIMPORT int MapDecode (unsigned char * mapmempt, RBDIRECTDRAW lpDD)
{
long int maplength;

	MapFreeMem ();
	maperror = MER_NONE;

	if (*mapmempt!='F') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+1)!='O') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+2)!='R') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+3)!='M') maperror = MER_MAPLOADERROR;
	mapmempt += 4;
	maplength = (MapGetchksz (mapmempt))+8;

	if (maperror) return -1;
	mapmempt += 4;

	if (*mapmempt!='F') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+1)!='M') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+2)!='A') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+3)!='P') maperror = MER_MAPLOADERROR;
	mapmempt+=4;

	if (maperror) return -1;
	return MapRealDecode (NULL, mapmempt, maplength, lpDD);
}

extern "C" DLLIMPORT void MapReleaseParallaxSurface (void)
{
	if (lpDDSParallax!=NULL) { lpDDSParallax->Release (); lpDDSParallax = NULL; }
}

extern "C" DLLIMPORT void MapRestoreParallaxSurface (void* srcpt, int pitch)
{
int i;
unsigned char * destpt;
RBSURFACEDESC mddsd;
RECT cliprect;
RECT dstrect;

	if (lpDDSParallax==NULL) return;
	if (lpDDSParallax->IsLost ()==DDERR_SURFACELOST) {
		if (lpDDSParallax->Restore ()!=DD_OK) return;
	}

/* Must restore surface contents here! */
	memset (&mddsd, 0, sizeof(RBSURFACEDESC));
	mddsd.dwSize = sizeof(RBSURFACEDESC);
	if (srcpt != NULL) {
	if (lpDDSParallax->Lock (NULL, &mddsd, DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_SURFACEMEMORYPTR, NULL)!=DD_OK) return;
	destpt = (unsigned char *) mddsd.lpSurface;
	for (i=0;i<PARAH;i++) {
		memcpy (destpt+(i*mddsd.lPitch), ((char *) srcpt)+(i*pitch), PARAW*((mapdepth+1)/8));
	}
	lpDDSParallax->Unlock (NULL);
   }

/* OK, now the important part, must duplicate the top row and left column
 * in the extra space allocated
 */

	cliprect.left = 0; cliprect.right = PARAW;
	cliprect.top = 0; cliprect.bottom = mapblockheight;
	if (rbblttype == 1) {
	dstrect.left = 0;
	dstrect.top = PARAH;
	dstrect.right = PARAW;
	dstrect.bottom = PARAH+mapblockheight;
	lpDDSParallax->Blt (&dstrect, lpDDSParallax, &cliprect, DDBLT_WAIT, NULL);
	} else {
	lpDDSParallax->BltFast (0, PARAH, lpDDSParallax, &cliprect, DDBLTFAST_WAIT );
	}
	cliprect.left = 0; cliprect.right = mapblockwidth;
	cliprect.top = 0; cliprect.bottom = PARAH+mapblockheight;
	if (rbblttype == 1) {
	dstrect.left = PARAW;
	dstrect.top = 0;
	dstrect.right = PARAW+mapblockwidth;
	dstrect.bottom = PARAH+mapblockheight;
	lpDDSParallax->Blt (&dstrect, lpDDSParallax, &cliprect, DDBLT_WAIT, NULL);
	} else {
	lpDDSParallax->BltFast (PARAW, 0, lpDDSParallax, &cliprect, DDBLTFAST_WAIT );
	}

	return;
}

extern "C" DLLIMPORT int MapCreateParallaxSurface (RBDIRECTDRAW lpDD)
/* BITMAP width must be a multiple of mapblockwidth
 * BITMAP height must be a multiple of mapblockheight
 */
{
RBSURFACEDESC mddsd;
DDCOLORKEY ddck;

	if (lpDDSParallax!=NULL) return -1;

	memset (&mddsd, 0, sizeof (mddsd));
	memset (&ddck, 0, sizeof (ddck));
	mddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	mddsd.ddsCaps.dwCaps = rbcapstype;
	mddsd.dwHeight = PARAW+mapblockwidth;	/* Width is desired width + blockwidth */
	mddsd.dwWidth = PARAH+mapblockheight;	/* Height is desired height + block height */
	mddsd.dwSize = sizeof (mddsd);
	lpDD->CreateSurface (&mddsd, &lpDDSParallax, NULL);
	if (lpDDSParallax==NULL) return -1;
	lpDDSParallax->SetColorKey( DDCKEY_SRCBLT, &ddck );	/* Just setting to 0 for this test */

	return 0;
}

extern "C" DLLIMPORT int MapDrawParallax (RBDIRECTDRAWSURFACE lpDDSDest, int mapxo, int mapyo)
/* lpDDSDest = lpdirectdrawsurface.
 * mapxo = offset, in pixels, from the left edge of the map.
 * mapyo = offset, in pixels, from the top edge of the map.
 */
{
RECT cliprect;
RECT dstrect;
int i, i2, j, xedge, yedge, indentx, indentx2, indenty;
short int *mymappt;
short int *mymappt2;
BLKSTR *blkdatapt;
ANISTR *myanpt;

	if (mapblockstaggerx || mapblockstaggery) return 0;
	if (mapblockwidth != mapblockgapx || mapblockheight != mapblockgapy) return 0;

	mymappt = (short int *) mappt;
	mymappt += (mapxo/mapblockwidth)+((mapyo/mapblockheight)*mapwidth);

	indentx = ((mapxo-(mapxo%mapblockwidth))%(PARAW))-((mapxo/2)%(PARAW));
	indenty = ((mapyo-(mapyo%mapblockheight))%(PARAH))-((mapyo/2)%(PARAH));
	while (indentx < 0) indentx += PARAW;
	while (indenty < 0) indenty += PARAH;

	i = MMOX-(mapxo%mapblockwidth);
	j = MMOY-(mapyo%mapblockheight);

	i2 = i; indentx2 = indentx; mymappt2 = mymappt;
	while (j < (MMOY+MSCRH)) {
		while (i < (MMOX+MSCRW)) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
			if (blkdatapt->trigger) {
				cliprect.left = indentx;
				cliprect.top = indenty;
				cliprect.right = indentx;
				cliprect.bottom = indenty;
				if (i < MMOX) { xedge = MMOX; cliprect.left += (MMOX-i); } else { xedge = i; }
				if (j < MMOY) { yedge = MMOY; cliprect.top += (MMOY-j); } else { yedge = j; }
				if (i > ((MMOX+MSCRW)-mapblockwidth)) cliprect.right += (MMOX+MSCRW)-i; else cliprect.right += mapblockwidth;
				if (j > ((MMOY+MSCRH)-mapblockheight)) cliprect.bottom += (MMOY+MSCRH)-j; else cliprect.bottom += mapblockheight;

			if (rbblttype == 1) {
				dstrect.left = xedge;
				dstrect.top = yedge;
				dstrect.right = i+mapblockwidth;
				dstrect.bottom = j+mapblockheight;
				if (dstrect.right > (MMOX+MSCRW)) dstrect.right = MMOX+MSCRW;
				if (dstrect.bottom > (MMOY+MSCRH)) dstrect.bottom = MMOY+MSCRH;
				if (lpDDSDest->Blt (&dstrect, lpDDSParallax, &cliprect, DDBLT_WAIT, NULL) != DD_OK)
					return -1;
			} else {
				if (lpDDSDest->BltFast (xedge, yedge, lpDDSParallax, &cliprect, DDBLTFAST_WAIT)!=DD_OK)
					return -1;
			}
			}
			indentx += mapblockwidth;
			if (indentx >= (PARAW)) indentx -= (PARAW);
			i += mapblockwidth; mymappt++;
		}
		indenty += mapblockheight;
		if (indenty >= (PARAH)) indenty -= (PARAH);
		i = i2; indentx = indentx2; mymappt2 += mapwidth; mymappt = mymappt2;
		j += mapblockheight;
	}

	return 0;
}

static int FGBlit (RBDIRECTDRAWSURFACE lpDDSDest, int x, int y, RBDIRECTDRAWSURFACE mapgfxpt)
{
RECT cliprect;
RECT dstrect;
int xedge, yedge;

	if (x <= (-mapblockwidth) || y <= (-mapblockheight)) return 0;
	if (x >= (MSCRW) || y >= (MSCRH)) return 0;
	if (x < 0) cliprect.left = -x; else cliprect.left = 0;
	if (y < 0) cliprect.top = -y; else cliprect.top = 0;
	if (x > ((MSCRW)-mapblockwidth)) cliprect.right = (MSCRW)-x; else cliprect.right = mapblockwidth;
	if (y > ((MSCRH)-mapblockheight)) cliprect.bottom = (MSCRH)-y; else cliprect.bottom = mapblockheight;
	if (x < 0) xedge = 0; else xedge = x;
	if (y < 0) yedge = 0; else yedge = y;
	if (rbblttype == 1) {
	dstrect.left = xedge+MMOX;
	dstrect.top = yedge+MMOY;
	dstrect.right = x+MMOX+mapblockwidth;
	dstrect.bottom = y+MMOY+mapblockheight;
	if (dstrect.right > (MMOX+MSCRW)) dstrect.right = MMOX+MSCRW;
	if (dstrect.bottom > (MMOY+MSCRH)) dstrect.bottom = MMOY+MSCRH;
	return lpDDSDest->Blt (&dstrect, mapgfxpt, &cliprect, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
	} else {
	return lpDDSDest->BltFast (xedge+MMOX, yedge+MMOY, mapgfxpt, &cliprect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}
}

static int FGBlitleft (RBDIRECTDRAWSURFACE lpDDSDest, int x, int y, RBDIRECTDRAWSURFACE mapgfxpt)
{
RECT cliprect;
#ifdef RBUSEBLTONLY
RECT dstrect;
#endif
int xedge, yedge;

	if (x <= (-(mapblockwidth/2)) || y <= (-mapblockheight)) return 0;
	if (x >= (MSCRW) || y >= (MSCRH)) return 0;
	if (x < 0) cliprect.left = -x; else cliprect.left = 0;
	if (y < 0) cliprect.top = -y; else cliprect.top = 0;
	if (x > ((MSCRW)-(mapblockwidth/2))) cliprect.right = (MSCRW)-x; else cliprect.right = mapblockwidth/2;
	if (y > ((MSCRH)-mapblockheight)) cliprect.bottom = (MSCRH)-y; else cliprect.bottom = mapblockheight;
	if (x < 0) xedge = 0; else xedge = x;
	if (y < 0) yedge = 0; else yedge = y;
#ifdef RBUSEBLTONLY
	dstrect.left = xedge+MMOX;
	dstrect.top = yedge+MMOY;
	dstrect.right = x+MMOX+(mapblockwidth/2);
	dstrect.bottom = y+MMOY+mapblockheight;
	if (dstrect.right > (MMOX+MSCRW)) dstrect.right = MMOX+MSCRW;
	if (dstrect.bottom > (MMOY+MSCRH)) dstrect.bottom = MMOY+MSCRH;
	return lpDDSDest->Blt (&dstrect, mapgfxpt, &cliprect, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
#else
	return lpDDSDest->BltFast (xedge+MMOX, yedge+MMOY, mapgfxpt, &cliprect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
#endif
}

static int FGBlitright (RBDIRECTDRAWSURFACE lpDDSDest, int x, int y, RBDIRECTDRAWSURFACE mapgfxpt)
{
RECT cliprect;
#ifdef RBUSEBLTONLY
RECT dstrect;
#endif
int xedge, yedge;

	x += (mapblockwidth/2);
	if (x <= (-(mapblockwidth/2)) || y <= (-mapblockheight)) return 0;
	if (x >= (MSCRW) || y >= (MSCRH)) return 0;
	if (x < 0) cliprect.left = (mapblockwidth/2)-x; else cliprect.left = (mapblockwidth/2);
	if (y < 0) cliprect.top = -y; else cliprect.top = 0;
	if (x > ((MSCRW)-(mapblockwidth/2))) cliprect.right = ((MSCRW)-x)+(mapblockwidth/2); else cliprect.right = mapblockwidth;
	if (y > ((MSCRH)-mapblockheight)) cliprect.bottom = (MSCRH)-y; else cliprect.bottom = mapblockheight;
	if (x < 0) xedge = 0; else xedge = x;
	if (y < 0) yedge = 0; else yedge = y;
#ifdef RBUSEBLTONLY
	dstrect.left = xedge+MMOX;
	dstrect.top = yedge+MMOY;
	dstrect.right = x+MMOX+(mapblockwidth/2);
	dstrect.bottom = y+MMOY+mapblockheight;
	if (dstrect.right > (MMOX+MSCRW)) dstrect.right = MMOX+MSCRW;
	if (dstrect.bottom > (MMOY+MSCRH)) dstrect.bottom = MMOY+MSCRH;
	return lpDDSDest->Blt (&dstrect, mapgfxpt, &cliprect, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
#else
	return lpDDSDest->BltFast (xedge+MMOX, yedge+MMOY, mapgfxpt, &cliprect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
#endif
}

static int BGBlit (RBDIRECTDRAWSURFACE lpDDSDest, int x, int y, RBDIRECTDRAWSURFACE mapgfxpt)
{
RECT cliprect;
RECT dstrect;
int xedge, yedge;

	if (x <= (-mapblockwidth) || y <= (-mapblockheight)) return 0;
	if (x >= (MSCRW) || y >= (MSCRH)) return 0;
	if (x < 0) cliprect.left = -x; else cliprect.left = 0;
	if (y < 0) cliprect.top = -y; else cliprect.top = 0;
	if (x > ((MSCRW)-mapblockwidth)) cliprect.right = (MSCRW)-x; else cliprect.right = mapblockwidth;
	if (y > ((MSCRH)-mapblockheight)) cliprect.bottom = (MSCRH)-y; else cliprect.bottom = mapblockheight;
	if (x < 0) xedge = 0; else xedge = x;
	if (y < 0) yedge = 0; else yedge = y;
	if (rbblttype == 1) {
	dstrect.left = xedge+MMOX;
	dstrect.top = yedge+MMOY;
	dstrect.right = x+MMOX+mapblockwidth;
	dstrect.bottom = y+MMOY+mapblockheight;
	if (dstrect.right > (MMOX+MSCRW)) dstrect.right = MMOX+MSCRW;
	if (dstrect.bottom > (MMOY+MSCRH)) dstrect.bottom = MMOY+MSCRH;
	return lpDDSDest->Blt (&dstrect, mapgfxpt, &cliprect, DDBLT_WAIT, NULL);
	} else {
	return lpDDSDest->BltFast (xedge+MMOX, yedge+MMOY, mapgfxpt, &cliprect, DDBLTFAST_WAIT);
	}
}

extern "C" DLLIMPORT int MapDrawBG (RBDIRECTDRAWSURFACE lpDDSDest, int mapxo, int mapyo)
/* lpDDSDest = lpdirectdrawsurface.
 * mapxo = offset, in pixels, from the left edge of the map.
 * mapyo = offset, in pixels, from the top edge of the map.
 */
{
int i, j, mapvclip, maphclip;
short int *mymappt;
short int *mymap2pt;
BLKSTR *blkdatapt;
ANISTR *myanpt;

	mapxo -= mapblockstaggerx; mapyo -= mapblockstaggery;
	maphclip = mapxo%mapblockgapx;	/* Number of pixels to clip from left */
	mapvclip = mapyo%mapblockgapy;	/* Number of pixels to clip from top */

	mymappt = (short int *) mappt;
	if (mapblockstaggerx || mapblockstaggery) {
		mymappt += (mapxo/mapblockgapx)+((mapyo/mapblockgapy)*mapwidth*2);
	} else {
		mymappt += (mapxo/mapblockgapx)+((mapyo/mapblockgapy)*mapwidth);
	}
	mymap2pt = NULL;

/* Draw blocks */
	i = (-mapvclip);
	if (mapblockstaggerx || mapblockstaggery) i -= mapblockgapy;
	while (i<(MSCRH)) {
		mymap2pt = mymappt;
		mymap2pt += mapwidth;
		j = (-maphclip);
		if (mapblockstaggerx || mapblockstaggery) j -= mapblockgapx;
		while (j<(MSCRW)) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }

			if (mapblockstaggerx || mapblockstaggery) {
				if (FGBlit (lpDDSDest, j, i, blkdatapt->bgoff) != DD_OK)	return -1;
			} else {
				if (BGBlit (lpDDSDest, j, i, blkdatapt->bgoff) != DD_OK)	return -1;
			}
			mymappt++; j += mapblockgapx;
		}

		if (mapblockstaggerx || mapblockstaggery) {
			mymappt = mymap2pt;
			mymap2pt += mapwidth;
			j = (-maphclip)-mapblockgapx; while (j<(MSCRW)) {
				if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
				else { myanpt = mapanimstrendpt + *mymappt;
					blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }

				if (*mymappt) {
					if (FGBlit (lpDDSDest, j+mapblockstaggerx, i+mapblockstaggery, blkdatapt->bgoff) != DD_OK)	return -1;
				}
				mymappt++; j += mapblockgapx;
			}
		}
		mymappt = mymap2pt;
		i += mapblockgapy;

	}

	return 0;
}

extern "C" DLLIMPORT int MapDrawBGT (RBDIRECTDRAWSURFACE lpDDSDest, int mapxo, int mapyo)
/* lpDDSDest = lpdirectdrawsurface.
 * mapxo = offset, in pixels, from the left edge of the map.
 * mapyo = offset, in pixels, from the top edge of the map.
 */
{
int i, j, mapvclip, maphclip;
short int *mymappt;
short int *mymap2pt;
BLKSTR *blkdatapt;
ANISTR *myanpt;

	if (mapblockstaggerx || mapblockstaggery) return (MapDrawBG (lpDDSDest, mapxo, mapyo));

	maphclip = mapxo%mapblockgapx;	/* Number of pixels to clip from left */
	mapvclip = mapyo%mapblockgapy;	/* Number of pixels to clip from top */

	mymappt = (short int *) mappt;
	mymappt += (mapxo/mapblockgapx)+((mapyo/mapblockgapy)*mapwidth);

/* Draw blocks */
	i = -mapvclip; while (i<(MSCRH)) {
		mymap2pt = mymappt;
		mymap2pt += mapwidth;
		j = -maphclip; while (j<(MSCRW)) {
			if (*mymappt) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }

				if (blkdatapt->trigger) {
					if (FGBlit (lpDDSDest, j, i, blkdatapt->bgoff) != DD_OK)	return -1;
				} else {
					if (BGBlit (lpDDSDest, j, i, blkdatapt->bgoff) != DD_OK)	return -1;
				}
			}
			mymappt++; j += mapblockgapx;
		}
		mymappt = mymap2pt;
		i += mapblockgapy;
	}
	return 0;
}

extern "C" DLLIMPORT int MapDrawFG (RBDIRECTDRAWSURFACE lpDDSDest, int mapxo, int mapyo, int mapfg)
/* lpDDSDest = lpdirectdrawsurface.
 * mapxo = offset, in pixels, from the left edge of the map.
 * mapyo = offset, in pixels, from the top edge of the map.
 */
{
int i, j, mapvclip, maphclip;
short int *mymappt;
short int *mymap2pt;
BLKSTR *blkdatapt;
ANISTR *myanpt;
RBDIRECTDRAWSURFACE mapgfxpt;

	mapxo -= mapblockstaggerx; mapyo -= mapblockstaggery;
	maphclip = mapxo%mapblockgapx;	/* Number of pixels to clip from left */
	mapvclip = mapyo%mapblockgapy;	/* Number of pixels to clip from top */

	mymappt = (short int *) mappt;
	if (mapblockstaggerx || mapblockstaggery) {
		mymappt += (mapxo/mapblockgapx)+((mapyo/mapblockgapy)*mapwidth*2);
	} else {
		mymappt += (mapxo/mapblockgapx)+((mapyo/mapblockgapy)*mapwidth);
	}

/* Draw blocks */
	i = (-mapvclip);
	if (mapblockstaggerx || mapblockstaggery) i -= mapblockgapy;
	while (i<(MSCRH)) {
		mymap2pt = mymappt;
		mymap2pt += mapwidth;
		j = (-maphclip);
		if (mapblockstaggerx || mapblockstaggery) j -= mapblockgapx;
		while (j<(MSCRW)) {
			if (*mymappt) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
			if (!mapfg) mapgfxpt = blkdatapt->fgoff;
			else if (mapfg == 1) mapgfxpt = blkdatapt->fgoff2;
			else mapgfxpt = blkdatapt->fgoff3;

			if (((int)mapgfxpt) != 0) {
				if (FGBlit (lpDDSDest, j, i, mapgfxpt) != DD_OK) return -1;
			} }
			mymappt++; j += mapblockgapx;
		}

	if (mapblockstaggerx || mapblockstaggery) {
		mymappt = mymap2pt;
		mymap2pt += mapwidth;
		j = (-maphclip)-mapblockgapx; while (j<(MSCRW)) {
			if (*mymappt) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }
			if (!mapfg) mapgfxpt = blkdatapt->fgoff;
			else if (mapfg == 1) mapgfxpt = blkdatapt->fgoff2;
			else mapgfxpt = blkdatapt->fgoff3;

			if (((int)mapgfxpt) != 0) {
				if (FGBlit (lpDDSDest, j+mapblockstaggerx, i+mapblockstaggery, mapgfxpt) != DD_OK)	return -1;
			}
			}
			mymappt++; j += mapblockgapx;
		}
	}
		mymappt = mymap2pt;
		i += mapblockgapy;

	}
	return 0;
}

extern "C" DLLIMPORT int MapDrawRow (RBDIRECTDRAWSURFACE lpDDSDest, int mapxo, int mapyo, int maprw,
	void (*cellcall) (int cx, int cy, int dx, int dy))
{
int i, j, mapvclip, maphclip;
int cx, cy, bfield, bysub;
short int *mymappt;
BLKSTR *blkdatapt;
ANISTR *myanpt;
RBDIRECTDRAWSURFACE mapgfxpt;

	if (((mapyo/mapblockgapy)+maprw) >= mapheight) return 0;

	if (mapblockstaggerx || mapblockstaggery) {
		mapxo -= mapblockstaggerx;
		mapyo -= mapblockstaggery;
		maphclip = mapxo%mapblockgapx;	/* Number of pixels to clip from left */
		mapvclip = mapyo%mapblockgapy;	/* Number of pixels to clip from top */
		i = (-mapvclip); j = (-maphclip);
		i -= mapblockgapy; j -= mapblockgapx;
		if ((((mapyo/mapblockgapy)*2)+maprw) >= (mapheight-1)) return 0;
		cx = mapxo/mapblockgapx;
		cy = (((mapyo/mapblockgapy)*2)+maprw);
		mymappt = (short int *) mappt;
		mymappt += (cx+(cy*mapwidth));
		i += ((maprw/2)*mapblockgapy);
		if (maprw&1) { i += mapblockstaggery; j += mapblockstaggerx; }
	} else {
		maphclip = mapxo%mapblockgapx;	/* Number of pixels to clip from left */
		mapvclip = mapyo%mapblockgapy;	/* Number of pixels to clip from top */
		i = (-mapvclip); j = (-maphclip);
		cx = mapxo/mapblockgapx;
		cy = (((mapyo/mapblockgapy))+maprw);
		mymappt = (short int *) mappt;
		mymappt += (cx+(cy*mapwidth));
		i += (maprw*mapblockgapy);
	}

/* Draw blocks */
		while (j<(MSCRW)) {
			if (cellcall != NULL) cellcall (cx, cy, j+MMOX, i+MMOY);
			if (*mymappt) {
			if (*mymappt>=0) blkdatapt = ((BLKSTR*) mapblockstrpt) + *mymappt;
			else { myanpt = mapanimstrendpt + *mymappt;
				blkdatapt = ((BLKSTR *) mapblockstrpt) + mapanimseqpt[myanpt->ancuroff]; }

			bfield = 1; bysub = 0;
			do {
			if (!bfield) blkdatapt++;
			for (;bfield<4;bfield++) {
			switch (bfield) {
				case 0: mapgfxpt = blkdatapt->bgoff; break;
				case 1: mapgfxpt = blkdatapt->fgoff; break;
				case 2: mapgfxpt = blkdatapt->fgoff2; break;
				default:
				case 3: mapgfxpt = blkdatapt->fgoff3; break;
			}
			if (((int)mapgfxpt) != 0) {

				if (blkdatapt->unused2 && !blkdatapt->unused3) {
					if (FGBlitleft (lpDDSDest, j, i-bysub, mapgfxpt) != DD_OK) return -1;
				} else {
					if (!blkdatapt->unused2 && blkdatapt->unused3) {
						if (FGBlitright (lpDDSDest, j, i-bysub, mapgfxpt) != DD_OK) return -1;
					} else {
						if (FGBlit (lpDDSDest, j, i-bysub, mapgfxpt) != DD_OK) return -1;
					}
				}
			}
			bysub += mapblockheight;
			}
			bfield = 0;
			} while (blkdatapt->unused1);

			mymappt++; j += mapblockgapx; cx++;
			}
		}

	return 0;
}
