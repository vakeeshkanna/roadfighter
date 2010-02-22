/* DirectX Header file for mappydx R11 */
/* (C)2002 Robin Burrows  -  rburrows@bigfoot.com */

#ifndef mpydll_h_included
#define mpydll_h_included

#define RBDIRECTDRAW LPDIRECTDRAW7
#define RBDIRECTDRAWSURFACE LPDIRECTDRAWSURFACE7

/*
 * When building the DLL code, you should define BUILDING_DLL so that
 * the variables/functions are exported correctly. When using the DLL,
 * do NOT define BUILDING_DLL, and then the variables/functions will
 * be imported correctly.
 *
 * You need to be using egcs-1.1.1 or newer.
 *
 * Building the DLL:
 *  - define BUILDING_DLL, which defines DLLIMPORT __attribute__((dllexport))
 * Building the client code:
 *  - DO NOT define BUILDING_DLL, which defines DLLIMPORT to be one
 *    __attribute__((dllimport))
 */

#ifdef __cplusplus
extern "C" {
#endif

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */

#define MER_NONE 0		/* All the horrible things that can go wrong */
#define MER_OUTOFMEM 1
#define MER_MAPLOADERROR 2
#define MER_NOOPEN 3
#define MER_MAPTOONEW 4

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

DLLIMPORT void * MapGetPtr (int);
DLLIMPORT int MapSetPtr (int, void *);
DLLIMPORT int MapGetVal (int);
DLLIMPORT int MapSetVal (int, int);
DLLIMPORT void MapSetParaSize (int, int);
DLLIMPORT void MapSetPalette (RBDIRECTDRAW, RBDIRECTDRAWSURFACE);
DLLIMPORT void MapSetDrawArea (int, int, int, int);
DLLIMPORT void MapFreeMem (void);
DLLIMPORT int MapLoad (char *, RBDIRECTDRAW);
DLLIMPORT int MapDecode (unsigned char *, RBDIRECTDRAW);
DLLIMPORT void MapInitAnims (void);
DLLIMPORT void MapUpdateAnims (void);
DLLIMPORT int MapDrawBG (RBDIRECTDRAWSURFACE, int , int);
DLLIMPORT int MapDrawBGT (RBDIRECTDRAWSURFACE, int, int);
DLLIMPORT int MapDrawFG (RBDIRECTDRAWSURFACE, int, int, int);
DLLIMPORT int MapRestore (void);
DLLIMPORT int MapGenerateYLookup (void);
DLLIMPORT int MapChangeLayer (int);
DLLIMPORT int MapLoadMAR (char *, int);
DLLIMPORT int MapDecodeMAR (unsigned char *, int);
DLLIMPORT int MapGetBlockID (int, int);
DLLIMPORT int MapGetXOffset (int, int);
DLLIMPORT int MapGetYOffset (int, int);
DLLIMPORT BLKSTR * MapGetBlockInPixels (int, int);
DLLIMPORT BLKSTR * MapGetBlock (int, int);
DLLIMPORT void MapSetBlockInPixels (int, int, int);
DLLIMPORT void MapSetBlock (int, int, int);
DLLIMPORT void MapReleaseParallaxSurface (void);
DLLIMPORT void MapRestoreParallaxSurface (void *, int);
DLLIMPORT int MapCreateParallaxSurface (RBDIRECTDRAW);
DLLIMPORT int MapDrawParallax (RBDIRECTDRAWSURFACE, int, int);
DLLIMPORT int MapDrawRow (RBDIRECTDRAWSURFACE, int, int, int,
	void (*cellcall) (int, int, int, int));

#ifdef __cplusplus
}
#endif

#endif
