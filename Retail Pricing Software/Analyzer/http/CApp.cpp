/*
 * CApp.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: kraychik
 */

#include "CApp.h"
#include "../processing/CProcessing.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

CApp::CApp() {
	// TODO Auto-generated constructor stub
}

CApp::~CApp() {
	// TODO Auto-generated destructor stub
}

void CApp::run() {
	loadSettings();
	cout << "CApp run() started..." << endl;
	CWebServer *webServer = new CWebServer ();
	CProcessing *processing = new CProcessing();
	//CTaskManager *taskManager = CTaskManager::GetInstance();
	//CResultManager *resultManager = CResultManager::GetInstance();

	while (true) {
		usleep (200);
		std::string str;
		std::cout << "Input command: ";
		std::getline(std::cin, str);
		if (!str.compare("exit")) {
			std::cout << "Quitting application..." << std::endl;
			return;
		}
		else {
			std::cout << "Unknown command" << std::endl;
		}
	}
	delete webServer;
}

CApp* CApp::GetInstance() {
	static CApp instance;
	return &instance;
}

void CApp::loadSettings() {
    std::string line;
    ifstream fileStream("settings");
    if (!fileStream.is_open()) {
    	cout << "Cannot open file with settings" << endl;
    	return;
    }
    while (std::getline(fileStream, line))
	{
		std::istringstream is_line(line);
		std::string key;
		if (std::getline(is_line, key, ':'))
		{
			std::string value;
			if (key[0] == '#')
				continue;

			if (std::getline(is_line, value))
			{
				SetupParams[key] = value;
			}
		}
	}
}
/*void CApp::loadSettings () {
	char buff[FILENAME_MAX];
	getcwd (buff, FILENAME_MAX);
	string currentDir (buff);

	string data = "";
	{
		string fileName = currentDir + "/settings";
		FILE *file = fopen (fileName.c_str(), "r");
		if (!file) {
			cout << "can not open file: " << fileName << endl;
			throw;
		}

		char buffer [1024];
		while (!feof (file)) {
			int ret = fread (buffer, 1, sizeof (buffer), file);
			if (ret <= 0) {
				break;
			}
			buffer [ret] = '\0';

			data += buffer;
		}
		fclose (file);
	}

	string db = "";
	char *str = strdup (data.c_str());
	while (true) {
		char *x = strstr (str, "\n");
		if (!x) {
			break;
		}

		*x = '\0';
		char *s = strstr (str, ": ");
		if (!s) {
			break;
		}

		*s = '\0';
		s += 2;

		SetupParams.insert(std::pair<string, string>(str, s));
	}
	delete [] str;
} */
