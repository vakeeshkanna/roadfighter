#include "gameobject.h"

int GameObject::countID = 0;

GameObject::GameObject()
{
}

GameObject::GameObject(char *newName)
{
	name = newName;
	numImages = 0;
	numFrames = 0;
	state = DEAD;
	onScreen = false;
	countID++;
	objectID = countID;
}

GameObject::~GameObject()
{

}

void GameObject::setName(char* n)
{
	name = n;
}

const char *GameObject::getName()
{
	return name.str();
}

void GameObject::setID(int id)
{
	objectID = id;
}

int GameObject::getID()
{
	return objectID;
}

void GameObject::setState(gameObjectState s)
{
	state = s;
}

gameObjectState GameObject::getState()
{
	return state;
}

void GameObject::setOnScreen(Logical screen)
{
	onScreen = screen;
}

Logical GameObject::getOnScreen()
{
	return onScreen;
}

void GameObject::setWidth(double w)
{
	width = w;
}

double GameObject::getWidth()
{
	return width;
}

void GameObject::setHeight(double h)
{
	height = h;
}

double GameObject::getHeight()
{
	return height;
}

RECT GameObject::getDimension()
{
	RECT rct;
	rct.left = 1;
	rct.top = 1;
	rct.right = getWidth();
	rct.bottom = getHeight();
	return rct;
}

void GameObject::setNumImages(int nf)
{
	numImages = nf;
}

int GameObject::getNumImages()
{
	return numImages;
}

void GameObject::addImage(ImageInfo image)
{
	images.push_back(image);
	numImages++;
}

void GameObject::setNumFrames(int nf)
{
	numFrames = nf;
}

int GameObject::getNumFrames()
{
	return numFrames;
}

void GameObject::setCurrentFrame(LPDIRECTDRAWSURFACE7 current)
{
	currentFrame = current;
}

LPDIRECTDRAWSURFACE7 GameObject::getCurrentFrame()
{
	return currentFrame;
}

void GameObject::reInitFrameTimer()
{
	frameTimer.init();
}

GameFrame GameObject::getFrameTimer()
{
	return frameTimer;
}
