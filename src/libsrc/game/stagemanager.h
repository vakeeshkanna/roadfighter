#ifndef STAGEMANAGER_H
#define STAGEMANAGER_H

class EXPORT StageManager
{
	int currentID;
	Logical loaded;

public:
	StageManager();
	~StageManager();
	virtual void init() = 0;
	virtual void load(int id) = 0;
	virtual void unload() = 0;
	virtual void cleanup() = 0;
	void setLoaded(Logical yesno);
	Logical isLoaded();
	void setCurrentID(int id);
	int getCurrentID();
};
#endif

