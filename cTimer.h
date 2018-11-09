#pragma once
#include <chrono>
#include <time.h>


class cTimer
{
public:
	// Call start() to start timing execution of code
	double		start();
	
	// Call stop() to stop timing execution of code
	double		stop();

	// Use reset() to clear the accumlation of elapsed time. 
	//	Note: if start() was called but stop() was not, reset() will effectively start() again
	double		reset();

	// returns the total elapsed time since creation or reset, in different timing units
	double		getElapsed_ns();		// nano-seconds: 1/1,000,000 of a sec
	double		getElapsed_ms();		// milli-seconds: 1/1,000 of a sec
	double		getElapsed_sec();		// seconds

	// returns true if currently accumulating elapsed time, i.e., start() has been called and stop() has not.
	bool		isTiming() { return timerIsTiming; }

	// constructor
	cTimer();

protected:
	bool		hasTurnedOver_{ false };
	bool		timerIsTiming{ false };

	std::chrono::time_point<std::chrono::steady_clock>	reset_;
	std::chrono::time_point<std::chrono::steady_clock>	start_;
	std::chrono::time_point<std::chrono::steady_clock>	stop_;
	std::chrono::duration<double, std::chrono::milliseconds::period> elapsed_ { 0 };
};

