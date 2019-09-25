/*
 * CTimer.h
 *
 *  Created on: Jan 20, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_CTIMER_H_
#define PROCESSING_CTIMER_H_

//#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
//#include "CProcessing.h"
class CProcessing;
class CTimer {
public:
	CTimer();
	~CTimer();
	void setInterval(std::function<void()> function, int interval);
	void stop();
private:
	bool clear = false;
};

#endif /* PROCESSING_CTIMER_H_ */
