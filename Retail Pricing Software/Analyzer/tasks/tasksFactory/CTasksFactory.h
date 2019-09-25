/*
 * CTasksFactory.h
 *
 *  Created on: Feb 26, 2019
 *      Author: kraychik
 */

#ifndef TASKS_ITASKSFACTORY_H_
#define TASKS_ITASKSFACTORY_H_

#include "../../processing/CCalcIdsManager.h"
#include "../CBaseTask.h"
#include "../CSupplementTask.h"
#include "../CDynamicsTask.h"
#include "../CReportTask.h"
#include "../CTopAssetsTask.h"
#include "../CGetResultTask.h"
#include "../CCheckStructureTask.h"
#include "../CPriceOptimizationTask.h"

class CTasksFactory {
public:
	CTasksFactory();
	virtual std::shared_ptr<const CBaseTask> CreateTask(const std::string& modeRequest, int projectId, const std::map<std::string, std::string>& params);
	virtual ~CTasksFactory();
protected:
	void ParseParam(const std::map<std::string, std::string> & params, const char *paramName, int & paramValue) const;
	void ParseParam(const std::map<std::string, std::string> & params, const char *paramName, double & paramValue) const;
	void ParseParam(const std::map<std::string, std::string> & params, const char *paramName, std::string & paramValue) const;
	void ParseParam(const std::map<std::string, std::string> & params, const char *paramName, std::set<int> & paramValues, char delim = ',') const;
	void ParseParam(const std::map<std::string, std::string> & params, const char *paramName, std::vector<int> & paramValues, char delim = ',') const;
	void ParseParam(const std::map<std::string, std::string> & params, const char *paramName, bool & paramValue) const;

	void SplitString(std::string str, char delim, std::list<std::string> & resultingContainer) const;
	void ParsePeriods(const std::string & strPeriods, char delim, std::vector<interval> & resultingContainer) const;

	template <typename T>
	void SplitStringToNumericVector(const std::string & str, char delim, std::vector<T> & resultingContainer) const;

	template <typename T>
	void SplitStringToNumericSet(const std::string& str, char delim, std::set<T>& resultingContainer) const;

	void ParseFilter(const std::map<std::string, std::string>& params, CFilter & filter) const;
	int GetNewCalcId() const;
	const char delim = ',';
};

#include "CGetResultTaskFactory.h"
#include "CAssetSupplementTaskFactory.h"
#include "CTopAssetsTaskFactory.h"
#include "CDynamicsTaskFactory.h"
#include "CCheckStructureTaskFactory.h"
#include "CPriceOptimizationTaskFactory.h"

#endif /* TASKS_ITASKSFACTORY_H_ */
