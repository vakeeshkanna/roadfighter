/*#include "screen.h"
#include "misc.h"


Screen::Screen()
{

}

Screen::Screen(Viewport *v)
{
	//int i;
	vp = v;
	totalPixLine = vp->stage->getNumCellsY() * CELL_SIZE;
	curPixLine = vp->getCurRow() * CELL_SIZE;
}

Screen::~Screen()
{

}

void Screen::init()
{

}


void Screen::update()
{
	if(curPixLine > 0)
		curPixLine -= 1;
	
	if(curPixLine % CELL_SIZE == 0) {
		vp->setCurRow(vp->getCurRow() - 1);
		curPixLine += 7;
		vp->update();
	}
}

void Screen::setCurPixLine(int cpl)
{ 
	if(cpl >= 0 && cpl < getTotalPixLine())
		curPixLine = cpl;
}

int Screen::getCurPixLine()
{ 
	return curPixLine;
}

void Screen::setTotalPixLine(int tpl)
{ 
	totalPixLine = tpl;
}

int Screen::getTotalPixLine()
{ 
	return totalPixLine;
}
*/


