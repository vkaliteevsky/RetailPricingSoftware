#include "CBaseTask.h"
#include "../structures/CIntervalSeries.h"


/* CBaseTask::CBaseTask() : projectId(0), calcId(0)
{
} */

CBaseTask::~CBaseTask()
{
}

CBaseTask::CBaseTask(int projectId, int calcId) : projectId(projectId), calcId(calcId) {
}

std::string CBaseTask::ToString() const {
	return "Project: " + std::to_string(projectId);
}

int CBaseTask::TypeId() const {
	return 1;
}
