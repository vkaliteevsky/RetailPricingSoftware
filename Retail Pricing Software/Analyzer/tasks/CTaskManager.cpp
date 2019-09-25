#include "CTaskManager.h"
#include <type_traits>

CTaskManager::CTaskManager()
{
}

void CTaskManager::AddTask(const std::shared_ptr<const CBaseTask> task)
{
	mutex.lock();
	tasks.push(task);
	auto calcId = task->calcId;

	CCalcIdsManager *calcIdsManager = CCalcIdsManager::GetInstance();
	calcIdsManager->SetStatus(calcId, CCalcIdsManager::IdStatus::NotExecutedYet);
	//resultManager->InformAboutTaskExistance(calcId);
	mutex.unlock();
}

std::shared_ptr<const CBaseTask> CTaskManager::GetTask()
{
	std::shared_ptr<const CBaseTask> task = nullptr;
	mutex.lock();
	if (tasks.empty()) {
		mutex.unlock();
		return task;
	}
	task = tasks.front();
	tasks.pop();
	mutex.unlock();
	return task;
}

int CTaskManager::Size() const
{
	mutex.lock();
	int size = tasks.size();
	mutex.unlock();
	return size;
}

CTaskManager *CTaskManager::GetInstance() {
	static CTaskManager instance;
	return &instance;
}

std::shared_ptr<const CBaseTask> CTaskManager::CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params) {
	std::shared_ptr<const CBaseTask> newTask = taskFactory.CreateTask(modeRequest, projectId, params);
	auto calcIdsManager = CCalcIdsManager::GetInstance();
	if (newTask->TypeId() == 2) {
		// GetResultTask
		return newTask;
	}
	calcIdsManager->AddCalcId(newTask->calcId, CCalcIdsManager::IdStatus::NotExecutedYet);
	GetInstance()->AddTask(newTask);
	return newTask;
}

CTaskManager::~CTaskManager()
{
}
std::shared_ptr<const CBaseTask> CTaskManager::CreateTask(const std::string & url) {
	return CreateTask(url.c_str());
}
std::shared_ptr<const CBaseTask> CTaskManager::CreateTask(const char* url) {
	std::string modeReport;
	std::map<std::string, std::string> keys;
	char *copyUrl = strdup(url);
	const char *cpStr = copyUrl;
	while (*copyUrl++) {
		*copyUrl = tolower(*copyUrl);
	}
	int projectId;
	bool isOk = parseUrl(cpStr, keys, modeReport, projectId);
	if (!isOk) {
		return nullptr;
	}
	delete [] cpStr;
	return GetInstance()->CreateTask(modeReport, projectId, keys);
}

bool CTaskManager::parseUrl(const char* url, std::map<std::string, std::string>& keys, std::string& mode, int& projectId) {
	std::string str(url);
	int startNum = 1, endNum = 1;
	if (*url != '/') return false;	// url должен начинаться с симовла '
	url++;
	// далее накапливаем строку до первого символа '/' - это modeReport
	while ((*url != '/') && (*url)) {
		endNum++;
		url++;
	}
	if (!(*url)) return false;
	mode = str.substr(startNum, endNum - startNum);
	endNum++;
	startNum = endNum;
	url++;
	// далее накапливаем projectId до первого символа '/'
	while ((*url != '/') && (*url)) {
			endNum++;
			url++;
	}
	if (!url) return false;
	std::string projectIdStr = str.substr(startNum, endNum - startNum);
	projectId = std::atoi(projectIdStr.c_str());	// TODO. Might work not stable
	endNum++;
	startNum = endNum;
	url++;
	if (*url != '?') return false;
	url++; endNum++; startNum = endNum;
	std::string key, value;
	while (*url) {
		if (*url == '=') {
			key = str.substr(startNum, endNum-startNum);
			endNum++;
			startNum = endNum;
		} else if (*url == '&') {
			value = str.substr(startNum, endNum-startNum);
			endNum++; startNum = endNum;
			keys.insert(std::pair<std::string, std::string>(key, value));
			key.clear(); value.clear();
		}
		else {
			endNum++;
		}
		url++;
	}
	value = str.substr(startNum, endNum-startNum);
	keys.insert(std::pair<std::string, std::string>(key, value));
	return true;
}

void CTaskManager::Clear() {
	while (tasks.size() > 0) tasks.pop();
}
