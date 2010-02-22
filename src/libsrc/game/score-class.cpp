#include "score-class.h"

Score::Score()
{
	
}

Score::Score(int sc)
{
	score = sc;
}

Score::~Score()
{
	
}

void Score::setScore(int sc)
{
	score = sc;
}

int Score::getScore()
{
	return score;
}

void Score::setHighestScore(int hs)
{
	highestScore = hs;
}

int Score::getHighestScore()
{
	return highestScore;
}
