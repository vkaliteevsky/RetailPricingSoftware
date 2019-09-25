/*
 * CProjectInfo.cpp
 *
 *  Created on: Feb 23, 2019
 *      Author: kraychik
 */

#include "CProjectInfo.h"

CProjectInfo::CProjectInfo(int projectId) {
	this->projectId = projectId;
	InitializeFromDB();
}

CProjectInfo::CProjectInfo(int projectId, shared_ptr<CSalesHistory> salesHistory, shared_ptr<CPriceList> curPriceList) {
	this->projectId = projectId;
	this->salesHistory = salesHistory;
	this->curPriceList = curPriceList;
}

shared_ptr<CSalesHistory> CProjectInfo::GetSalesHistory() const {
	return salesHistory;
}

shared_ptr<const CPriceList> CProjectInfo::GetCurrentPriceList() const {
	return curPriceList;
}

shared_ptr<CSalesHistory> CProjectInfo::LoadSalesHistory() {
	CDataBase *database = CDataBase::GetInstance();
	if (!database) {
		std::cout << "CProjectInfo::LoadSalesHistory: Connection Null Pointer" << std::endl;
	}
	auto historyItems = database->ReadSales(projectId);
	auto salesHistory = std::make_shared<CSalesHistory>();
	for (const auto & item : *historyItems) {
		salesHistory->AddLine(item);
	}
	this->salesHistory = salesHistory;
	return this->salesHistory;
	//loadedProjects.insert(projectId);
	//mapSales.insert(std::pair<int, std::shared_ptr<CSalesHistory>>(projectId, salesHistory));
	//return salesHistory;
}

shared_ptr<CSalesHistory> CProjectInfo::ReloadSalesHistory() {
	this->ClearSalesHistory();
	return this->LoadSalesHistory();
}

void CProjectInfo::ClearSalesHistory() {
	salesHistory->Clear();
}

void CProjectInfo::LoadCurrentPriceList() {
	CDataBase *database = CDataBase::GetInstance();
	if (!database) {
		std::cout << "CProjectInfo::LoadCurrentPriceList: Connection Null Pointer" << std::endl;
	}
	auto priceList = database->ReadPrices(projectId);
	curPriceList = make_shared<CPriceList>(priceList.first, priceList.second);
}

void CProjectInfo::ClearCurrentPriceList() {
	curPriceList->Clear();
}

CProjectInfo::~CProjectInfo() {
	// TODO Auto-generated destructor stub
}

int CProjectInfo::GetProjectId() const {
	return projectId;
}

void CProjectInfo::InitializeFromDB() {
	LoadSalesHistory();
	LoadCurrentPriceList();
}

void CProjectInfo::Update() {
	ReloadSalesHistory();
	ClearCurrentPriceList();
	LoadCurrentPriceList();
}

shared_ptr<COptimizerParams> CProjectInfo::GetOrLoadOptimizerParams() {
	CDataBase *database = CDataBase::GetInstance();
	if (!database) {
		std::cout << "CProjectInfo::GetOrLoadOptimizerParams: Connection Null Pointer" << std::endl;
	}
	auto params = database->ReadBasicConstraints(projectId);
	return params;
}
