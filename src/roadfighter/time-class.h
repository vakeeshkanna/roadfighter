#ifndef TIME_CLASS_H
#define TIME_CLASS_H
#include<windows.h>

class TimeClass
{
	LONGLONG last_time,cur_time,next_time,perf_cnt;
	DWORD time_count;
	BOOL perf_flag;
	double time_elapsed;
	double time_scale;
public:
	TimeClass();
	~TimeClass();
	BOOL isPerformanceTimerAvailable();
	void startTimer();
	double getTimeElapsed();
	

	
	//timeToUpdate()
	//{
	//	if(
	

};
#endif