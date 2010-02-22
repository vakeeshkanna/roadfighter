#include "viewport.h"
#include "log-class.h"

Viewport::Viewport()
{

}

Viewport::Viewport(int w, int h, int cs)
{
	width = w;
	height = h;
	cellSize = cs;
	numCellsX = (getWidth() / cellSize);
	numCellsY = (getHeight() / cellSize);

	buffer = RE->createEmptySurface(getWidth(), getHeight());

	//Set the offset for viewport
	lprintf("Screen Resolution = %d %d\n",RE->getInstance()->getScreenWidth(),RE->getInstance()->getScreenHeight());
	//offsetX = (RE->getScreenWidth() - VIEWPORT_WIDTH ) / 2;
	//offsetY = (RE->getScreenHeight() - VIEWPORT_HEIGHT) / 2;
	lprintf("Viewport X = %d Y = %d\n",getWidth(),getHeight());
	lprintf("Viewport numCellX = %d numCellsY = %d\n",numCellsX,numCellsY);

	//curRow = (stage->getHeight() / CELL_SIZE) - numCellsY;
}

Viewport::~Viewport()
{
}

Viewport* Viewport::vPort = NULL;

void Viewport::setInstance(Viewport* viewport)
{
	vPort = viewport;
}

Viewport* Viewport::getInstance()
{
	return vPort;
}

void Viewport::init(int numPixLines)
{
	totalPixLine = numPixLines;//background->getNumCellsY() * CELL_SIZE;
	//curPixLine = getCurRow() * CELL_SIZE;
	curPixLine = totalPixLine - getHeight();// actually how many pixel line remaining
	lprintf("totalPixelLine = %d\n",totalPixLine);
}

void  Viewport::setWidth(int w)
{
	width = w;
}

int  Viewport::getWidth()
{
	return width;
}

void  Viewport::setHeight(int h)
{
	height = h;
}

int  Viewport::getHeight()
{
	return height;
}

RECT Viewport::getExtents()
{
	RECT rect;
	rect.left = 0;
	rect.top = getTotalPixLine() - (int)getCurPixLine();
	rect.right = rect.left + getWidth() ;
	rect.bottom = rect.top - getHeight();
	return rect;
}

void Viewport::setNumCellsX(int nx)
{
	numCellsX = nx;
}

int Viewport::getNumCellsX()
{
	return numCellsX;
}

void Viewport::setNumCellsY(int ny)
{
	numCellsY = ny;
}

int Viewport::getNumCellsY()
{
	return numCellsY;
}

void Viewport::setCellSize(int cs)
{
	cellSize = cs;
}

int Viewport::getCellSize()
{
	return cellSize;
}

int Viewport::getCurRow()
{
	return curPixLine / getCellSize();
}

void Viewport::setCurPixLine(double cpl)
{
	if(cpl >= 0 && cpl < getTotalPixLine())
		curPixLine = cpl;
}

double Viewport::getCurPixLine()
{
	return curPixLine;
}

void Viewport::setTotalPixLine(int tpl)
{
	totalPixLine = tpl;
}

int Viewport::getTotalPixLine()
{
	return totalPixLine;
}

int Viewport::getTotalRows()
{
	return totalPixLine / getCellSize();
}

void Viewport::update(double moverate)
{
	if(curPixLine > 0)
		curPixLine -= moverate;
}
