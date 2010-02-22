#ifndef VIEWPORT_H
#define VIEWPORT_H

//Global Handle to Viewport
#define VP Viewport::getInstance()

class EXPORT Viewport
{
	int numCellsX,numCellsY;
	int width,height;
	double curPixLine;
	int totalPixLine;
	int cellSize;
	
public:
	static Viewport* vPort;
	LPDIRECTDRAWSURFACE7 buffer;
	
	Viewport();
	Viewport(int width, int height, int cellSize);
	~Viewport();
	static void setInstance(Viewport *viewport);
	static Viewport* getInstance();
	void init(int numPixLines);
	virtual void update(double moverate);
	void setWidth(int width);
	int getWidth();
	void setHeight(int height);
	int getHeight();
	RECT getExtents();
	void setNumCellsX(int);
	int getNumCellsX();
	void setNumCellsY(int);
	void setCellSize(int cellsize);
	int getCellSize();
	int getNumCellsY();
	int getCurRow();
	void setCurPixLine(double);
	double getCurPixLine();
	void setTotalPixLine(int);
	int getTotalPixLine();
	int getTotalRows();
};
#endif


