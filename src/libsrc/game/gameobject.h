#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "string-class.h"
#include "renderingengine.h"
#include "viewport.h"
#include "stage.h"
#include <vector>
#include <math.h>
#include "timer-class.h"
#include "gameframe.h"

#define MAX_FRAMES 25
#define MAX_IMAGES 10

enum gameObjectState
{
	READY,
	ALIVE,
	DEAD
};

using namespace std;

class EXPORT GameObject
{
public:
	GameObject();
	GameObject(char *name);
	virtual ~GameObject();
	virtual void init() = 0;
	virtual void display() = 0;
	virtual void cleanup() = 0;
	void setName(char *);
	const char *getName();
	void setID(int id);
	int getID();
	void setState(gameObjectState state);
	gameObjectState getState();
	void setOnScreen(Logical onScreen);
	Logical getOnScreen();
	void setWidth(double w);
	double getWidth();
	void setHeight(double h);
	double getHeight();
	RECT getDimension();
	void setNumImages(int);
	int getNumImages();
	void setNumFrames(int);
	int getNumFrames();
	void addImage(ImageInfo image);
	void setCurrentFrame(LPDIRECTDRAWSURFACE7 current);
	LPDIRECTDRAWSURFACE7 getCurrentFrame();
	void reInitFrameTimer();
	GameFrame getFrameTimer();

	static int countID;

protected:
	String name;
	gameObjectState state;
	Logical onScreen;
	double width,height;
	int numImages,numFrames;
	int objectID; // useful for keeping track of objects
	vector< ImageInfo > images;
	LPDIRECTDRAWSURFACE7 image;
	LPDIRECTDRAWSURFACE7 frames[MAX_FRAMES];
	LPDIRECTDRAWSURFACE7 currentFrame;
	GameFrame frameTimer;
};
#endif

