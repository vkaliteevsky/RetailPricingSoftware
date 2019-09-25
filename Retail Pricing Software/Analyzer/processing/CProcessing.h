/*
 * CProcessing.h
 *
 *  Created on: Jan 19, 2019
 *      Author: kraychik
 */

#ifndef HTTP_CPROCESSING_H_
#define HTTP_CPROCESSING_H_

#include <memory>
#include "../tasks/CTaskManager.h"
#include "../structures/CSalesHistory.h"
#include <map>
#include "../supply/CDataBase.h"
#include "../structures/CSalesHistory.h"
#include <type_traits>
#include <thread>
#include <chrono>
#include <functional>

#include "../tasks/CBaseTask.h"
#include "../tasks/CSupplementTask.h"
#include "CResultManager.h"
#include "../http/CJSONCreator.h"

#include "executors/IExecutor.h"

class IExecutor;
class CProcessing {
public:
	CProcessing();
	virtual ~CProcessing();
	void WakeUp();
	void SetUpExecutor(int taskTypeId, std::shared_ptr<IExecutor> executor);
	void ExecuteTask(std::shared_ptr<const CBaseTask> task);
	static void WakeUpTimer(CProcessing *processing);
private:
	std::map<int, std::shared_ptr<IExecutor>> mapExecutor;
	void SetTimer();
	const int delay = 200;
};

#endif /* HTTP_CPROCESSING_H_ */
