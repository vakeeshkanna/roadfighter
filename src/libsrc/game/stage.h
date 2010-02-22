#ifndef STAGE_H
#define STAGE_H
#include "renderingengine.h"
#include "string-class.h"

class EXPORT Stage
{
public:
	char **cellArray;
	int numCellsX,numCellsY;
	LPDIRECTDRAWSURFACE7 tiledImage;
	LPDIRECTDRAWSURFACE7 buffer;
	int numImages;

	Stage();
	virtual ~Stage();
	Stage(int cellSize);
	virtual void init() = 0;
	virtual void load() = 0;
	virtual void unload() = 0;
	virtual void display() = 0;
	void loadImage(int imageIndex);
	void setWidth(int);
	int getWidth();
	void setHeight(int);
	int getHeight();
	void setNumCellsX(int);
	int getNumCellsX();
	void setNumCellsY(int);
	int getNumCellsY();
	static RECT getRect(int,int,int);
	static RECT getRect(int,int,int,int);
	void setNumCellsImageX(int);
	int getNumCellsImageX();
	void setNumCellsImageY(int);
	int getNumCellsImageY();
	void setCellSize(int);
	int getCellSize();
	void addImage(ImageInfo image);

	LPDIRECTDRAWSURFACE7 getBuffer();

protected:
	int numCellsImageX,numCellsImageY;
	int width,height,cellSize;
	std::vector< ImageInfo > images;
};
#endif


