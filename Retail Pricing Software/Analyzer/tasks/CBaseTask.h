#pragma once
#include <map>
#include <string>
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <list>
#include "../structures/CIntervalSeries.h"
#include "../supply/CSmartException.h"

class CBaseTask
{
public:
	//CBaseTask();
	CBaseTask(int projectId, int calcId);
	virtual ~CBaseTask();
	virtual std::string ToString() const;
	virtual int TypeId() const;

	int projectId = 0;
	int calcId = 0;
};
