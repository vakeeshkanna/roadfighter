#ifndef BACKGROUNDMANAGER_H
#define BACKGROUNDMANAGER_H

#include "background.h"

class EXPORT BackGroundManager : public StageManager
{
public:
	BackGroundManager();
	~BackGroundManager();
	void init();
	void load(int id);
	void unload();
	void cleanup();

	BackGround *currentBackGround;
};
#endif
