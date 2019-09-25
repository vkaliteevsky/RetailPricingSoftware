/*
 * Runnable.h
 *
 *  Created on: Jan 19, 2019
 *      Author: kraychik
 */

#ifndef HTTP_RUNNABLE_H_
#define HTTP_RUNNABLE_H_
#include <thread>
#include <atomic>

class Runnable {
public:
	Runnable();
	virtual ~Runnable();
	Runnable (Runnable const&) = delete;
	Runnable & operator =(Runnable const&) = delete;
	void start();
	void stop();
protected:
    virtual void run() = 0;
    std::atomic<bool> mStop;
private:
    std::thread mThread;
};

#endif /* HTTP_RUNNABLE_H_ */
