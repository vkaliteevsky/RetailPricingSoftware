/*
 * CSupplementTask.h
 *
 *  Created on: Feb 6, 2019
 *      Author: kraychik
 */

#ifndef TASKS_CSUPPLEMENTTASK_H_
#define TASKS_CSUPPLEMENTTASK_H_

#include "CReportTask.h"
#include "../structures/CFilter.h"
#include "../structures/Structures.h"
#include <vector>

class CSupplementTask: public CReportTask {
public:
	//CSupplementTask();
	CSupplementTask(int projectId, int calcId, const CFilter & filter, const std::vector<int> & targetAssetIds, bool useSupplementTargetAssets, ResponseFormat responseFormat);
	std::string ToString() const;
	virtual ~CSupplementTask();
	virtual int TypeId() const override;
	std::vector<int> targetAssetIds;
	ResponseFormat responseFormat = ResponseFormat::ArrayOfArrays;
	bool useSupplementTargetAssets = true;
};

#endif /* TASKS_CSUPPLEMENTTASK_H_ */
