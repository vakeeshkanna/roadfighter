#include "time-class.h"
#include "renderingengine.h"

TimeClass::TimeClass()
{
	cur_time = 0;
	last_time = 0;
	time_elapsed = 0.0;
	next_time = 0.0;
	if(QueryPerformanceFrequency((LARGE_INTEGER*)&perf_cnt))
	{
		perf_flag = yes;
		time_count = (DWORD)perf_cnt / 30;
		QueryPerformanceCounter((LARGE_INTEGER*)&next_time);
		time_scale = 1.0 / perf_cnt;
	}
	else
	{
		next_time = timeGetTime();
		time_scale = 0.001;
		time_count = 33;
	}
	last_time = next_time;
}

TimeClass::~TimeClass()
{

}

BOOL TimeClass::isPerformanceTimerAvailable()
{
	if(perf_flag)
		return yes;
	else
		return no;
}

double TimeClass::getTimeElapsed()
{
	if(isPerformanceTimerAvailable())
		QueryPerformanceCounter((LARGE_INTEGER*)&cur_time);
	else
		cur_time = timeGetTime();
	
	if(cur_time > next_time) 
	{
		time_elapsed = (cur_time - last_time) * time_scale;
		last_time = cur_time;
//		y_pos += move_rate * time_elapsed;
		next_time = cur_time + time_count;
		//RenderingEngine::outputText(100,400,C_WHITE,time_elapsed,10,15);
		return time_elapsed;
	}
	else
		return 1.0;

}


