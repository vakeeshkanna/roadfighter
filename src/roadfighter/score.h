#ifndef SCORE_H
#define SCORE_H

class Score
{
	int score;
	int stageNum;
	int time;

public:
	Score();
	~Score();
	void init();
	void setScore(int);
	int getScore();
	void setStageNum(int);
	int getStageNum();
	void setTime(int);
	int getTime();
};
#endif


