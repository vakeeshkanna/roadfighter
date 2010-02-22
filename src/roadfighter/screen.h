/*#ifndef SCREEN_H
#define SCREEN_H
#include "stage.h"
#include "viewport.h"

//friend class RenderingEngine;
class Screen
{
	//double delay;
	//int state;
	int curPixLine;
	int totalPixLine;

public:
	Viewport *vp;

	Screen();
	Screen(Viewport*);
	~Screen();
	void init();
//	void setDelay(double);
//	double getDelay();
	void setCurPixLine(int);
	int getCurPixLine();
	void setTotalPixLine(int);
	int getTotalPixLine();
	void update();
};
#endif

*/
