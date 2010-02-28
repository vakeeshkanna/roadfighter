#ifndef IMAGEINFO_H_
#define IMAGEINFO_H_

#include <string>
#include <vector>

using namespace std;

class EXPORT ImageInfo
{
public:
	ImageInfo(string name, string filePath, string fileName, int numFrames, int cellSize);
	ImageInfo(string name, string filePath, string fileName, int numFrames, int cellSizex, int cellsizey);
	ImageInfo(string name, string filePath, string fileName);
	ImageInfo(string name, string filePath, string fileName, int numFrames);
	ImageInfo();
	~ImageInfo();
	void setCellSizeX(int cellsize);
	int getCellSizeX();
	void setCellSizeY(int cellsize);
	int getCellSizeY();
	void setNumFrames(int nf);
	int getNumFrames();
	void setName(string name);
	string getName();
	void setPath(string path);
	string getPath();

private:
	int numFrames, cellSizeX, cellSizeY;
	string name;
	string path;
};

#endif /*IMAGEINFO_H_*/
