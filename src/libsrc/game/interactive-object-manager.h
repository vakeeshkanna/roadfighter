#ifndef INTERACTIVE_OBJECT_MANAGER_H
#define INTERACTIVE_OBJECT_MANAGER_H

class InteractiveObjectManager
{
public:
	InteractiveObjectManager()
	{
	};
	~InteractiveObjectManager()
	{
	};
	virtual void init() = 0;
	virtual void reinit() = 0;
	virtual void spawn() = 0;
	virtual void display() = 0;
	virtual void update() = 0;
	virtual void cleanup() = 0;
};
#endif
