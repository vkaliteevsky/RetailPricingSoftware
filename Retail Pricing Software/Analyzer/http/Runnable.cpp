/*
 * Runnable.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: kraychik
 */

#include "Runnable.h"

Runnable::Runnable() : mStop(), mThread() {

}

Runnable::~Runnable() {
	try {
		stop();
	}
	catch (...) {

	}
}

void Runnable::start() {
	mThread = std::thread(&Runnable::run, this);
}

void Runnable::stop() {
	mStop = true;
	mThread.join();
}
