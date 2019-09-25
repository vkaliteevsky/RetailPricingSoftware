#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <memory>
#include <ctime>

#include "structures/CPriceItem.h"
#include "structures/CCheck.h"
#include "structures/CSalesHistory.h"
#include "structures/CHistoryItem.h"

#include "http/Runnable.h"
#include "http/CApp.h"
#include "http/CWebServer.h"

#include "tasks/CBaseTask.h"
#include "tasks/CTaskManager.h"
#include "analizer/COptimizer.h"

#include "processing/CResultManager.h"
#include "supply/CDataBase.h"

#include <pqxx/pqxx>

#include <iostream>


#include "supply/CCSVReader.h"
int main() {
	setenv("TZ", "UTC", 1);
	srand(time(NULL));
	std::cout << time(NULL) << std::endl;
	std::cout << "Main Works again" << std::endl;
	CApp *app = new CApp();
	app->run();
	std::cout << "Quit" << std::endl;
	return EXIT_SUCCESS;
}
