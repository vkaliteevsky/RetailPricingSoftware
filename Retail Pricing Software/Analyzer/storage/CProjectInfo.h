/*
 * CProjectInfo.h
 *
 *  Created on: Feb 23, 2019
 *      Author: kraychik
 */

#ifndef STORAGE_CPROJECTINFO_H_
#define STORAGE_CPROJECTINFO_H_

#include <memory>
#include <string>
#include "../structures/CSalesHistory.h"
#include "../structures/CPriceList.h"
#include "../supply/CDataBase.h"
#include "../analizer/COptimizerParams.h"

class CProjectInfo {
public:
	CProjectInfo(int projectId);
	CProjectInfo(int projectId, shared_ptr<CSalesHistory> salesHistory, shared_ptr<CPriceList> curPriceList);
	shared_ptr<CSalesHistory> GetSalesHistory() const;
	shared_ptr<const CPriceList> GetCurrentPriceList() const;
	shared_ptr<CSalesHistory> LoadSalesHistory();
	shared_ptr<CSalesHistory> ReloadSalesHistory();
	shared_ptr<COptimizerParams> GetOrLoadOptimizerParams();
	void ClearSalesHistory();
	void LoadCurrentPriceList();
	void ClearCurrentPriceList();
	int GetProjectId() const;
	void Update();
	virtual ~CProjectInfo();
private:
	void InitializeFromDB();
	int projectId;
	shared_ptr<CSalesHistory> salesHistory = nullptr;
	shared_ptr<CPriceList> curPriceList = nullptr;
};

#endif /* STORAGE_CPROJECTINFO_H_ */
