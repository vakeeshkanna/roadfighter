#ifndef BG_H
#define BG_H

#include <string>
#include "basedefs.h"
#include "mpydll.h"

class EXPORT BackGround
{
public:
	BackGround();
	BackGround(int cellSize);
	~BackGround();
	virtual void init(LPDIRECTDRAW7 DD, LPDIRECTDRAWSURFACE7 BB);
	virtual void load(string mapFile);
	virtual void unload();
	virtual void display();
	void setCellSize(int);
	int getCellSize();
	void setNumCellsX(int);
	int getNumCellsX();
	void setNumCellsY(int);
	int getNumCellsY();
	void setOffsetX(int offsetx);
	int getOffsetX();
	void setOffsetY(int offsety);
	int getOffsetY();
	void setOffsetXY(int offsetx, int offsety);
	Logical isLoaded();

private:
	Logical loaded;
	LPDIRECTDRAW7 DDraw;
	LPDIRECTDRAWSURFACE7 BackBuffer;
	string mapFile;
	int cellSize;
	int numCellsX,numCellsY;
	int offsetX, offsetY;
};
#endif
