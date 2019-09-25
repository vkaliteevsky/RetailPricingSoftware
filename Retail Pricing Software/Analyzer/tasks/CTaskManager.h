#pragma once
#include <queue>
#include <mutex>

#include "CBaseTask.h"
#include "CDynamicsTask.h"
#include "CSupplementTask.h"
#include "CTopAssetsTask.h"
#include "../structures/CFilter.h"
#include "CGetResultTask.h"
#include "../processing/CResultManager.h"
#include "../tasks/CCheckStructureTask.h"
#include "../supply/CTypeConverter.h"
#include "../processing/CCalcIdsManager.h"
#include "tasksFactory/CTasksFactory.h"

#include <cstdlib>
#include <cstring>
#include <vector>
#include <typeinfo>

class CTaskManager
{
public:
	CTaskManager();
	~CTaskManager();
	static CTaskManager *GetInstance();
	static std::shared_ptr<const CBaseTask> CreateTask(const std::string & url);
	static std::shared_ptr<const CBaseTask> CreateTask(const char *url);
	void AddTask(const std::shared_ptr<const CBaseTask> task);
	std::shared_ptr<const CBaseTask> GetTask();
	int Size() const;
	void Clear();
private:
	std::shared_ptr<const CBaseTask> CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params);
	static bool parseUrl(const char *url, std::map<std::string, std::string> & keys, std::string & mode, int & projectId);

	static CTaskManager instance;
	std::queue<std::shared_ptr<const CBaseTask>> tasks;
	CTasksFactory taskFactory;
	mutable std::mutex mutex;
};

