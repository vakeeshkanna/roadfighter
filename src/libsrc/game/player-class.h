#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include "string-class.h"
#include "score-class.h"

class Player
{
public:
	Player(String name,Score score);
	Player();
	~Player();
	void setName(String name);
	String& getName();
	void setScore(Score score);
	Score& getScore();
	
private:
	String name;
	Score score;
	
};
#endif
