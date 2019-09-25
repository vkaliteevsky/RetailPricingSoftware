/*
 * CApp.h
 *
 *  Created on: Jan 19, 2019
 *      Author: kraychik
 */
#pragma once
#ifndef HTTP_CAPP_H_
#define HTTP_CAPP_H_
#include <string>
#include <map>
#include "../processing/CProcessing.h"
#include "CWebServer.h"
#include <string>
#include <fstream>

using namespace std;

class CApp {
public:
	CApp();
	virtual ~CApp();
	void run();
	static CApp * GetInstance();
	map<string, string> SetupParams;
	void loadSettings();
private:
	static CApp instance;
	//std::string mongodb;
};

#endif /* HTTP_CAPP_H_ */
