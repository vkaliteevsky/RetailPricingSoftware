/*
 * CTimer.cpp
 *
 *  Created on: Jan 20, 2019
 *      Author: kraychik
 */

#include "CTimer.h"

CTimer::CTimer() {
	// TODO Auto-generated constructor stub

}

CTimer::~CTimer() {
	// TODO Auto-generated destructor stub
}

/*void CTimer::setInterval(std::function<void(CProcessing *)> function, int interval) {
    this->clear = false;
    std::thread t([=]() {
        while(true) {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(this->clear) return;
            function(processing);
        }
    });
    t.detach();
}*/

void CTimer::stop() {
    this->clear = true;
}
