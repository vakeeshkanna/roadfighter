#ifndef SCORE_CLASS_H
#define SCORE_CLASS_H

class Score
{
	int score;
	int highestScore;

public:
	Score();
	Score(int score);
	~Score();
	void setScore(int);
	int getScore();
	void setHighestScore(int);
	int getHighestScore();
};
#endif


