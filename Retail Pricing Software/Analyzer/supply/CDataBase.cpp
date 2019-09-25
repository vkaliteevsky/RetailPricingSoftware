/*
 * CDataBase.cpp
 *
 *  Created on: Jan 21, 2019
 *      Author: kraychik
 */

#include <iostream>

#include "CDataBase.h"
#include "../analizer/COptimizerParams.h"

CDataBase::CDataBase() {
	CApp *app = CApp::GetInstance();
	app->loadSettings();
	auto dbConnStringIt = app->SetupParams.find("DB");
	const char *connStr;
	if (dbConnStringIt != app->SetupParams.end()) {
		connStr = dbConnStringIt->second.c_str();
	} else {
		cout << "Add to setup file field: DB_Connection_String" << endl;
		throw CSmartException("Cannot connect to DB. Add config file");
	}
	//cout << "try conntect - s1" << endl;
	this->connStr = connStr;
	conn = new pqxx::connection (connStr);
	if (conn) {
		//cout << "s1 - ok" << endl;
	}
	work = new pqxx::work (*conn);
	if (work) {
		//cout << "s2 - ok" << endl;
	}
	//cout << "s3 - ok" << endl;
}

CDataBase::~CDataBase() {
	conn->disconnect();
	delete work;
	delete conn;
}

CDataBase* CDataBase::GetInstance() {
	static CDataBase instance;
	return &instance;
}

shared_ptr<list<CHistoryItem>> CDataBase::ReadSales(int projectId) {
	mLock.lock();
	pqxx::result result;
	try {
		//std::string request("SELECT check_id, extract (epoch from tx_date) as tx, branch_id, asset_id, price, quantity FROM tx_list WHERE project_id=");
		std::string request("SELECT check_id, extract (epoch from dt) as tx, shop_id, product_id, price, volume FROM sales_view WHERE company_id=");
		//cout << "d1" << endl;
		request += std::to_string(projectId);
		result = work->exec(request);
		//cout << "d1 - ok" << endl;
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		mLock.unlock();
		return nullptr;
	}
	auto resultingContainer = make_shared<list<CHistoryItem>>();
	for (auto row: result) {
		int checkId = atoi(row[0].c_str());
		//time_t checkTime = ParseDateToTimeT(row[1].c_str());
		time_t checkTime = stol(row[1].c_str());
		int shopId = atoi(row[2].c_str());
		int assetId = atoi(row[3].c_str());
		double price = atof(row[4].c_str());
		double volume = atof(row[5].c_str());
		double discount = 0.0;	// TODO
		//cout << row[1] << endl;
		//std::cout << "CheckTime: " << checkTime << std::endl;
		resultingContainer->push_back(CHistoryItem(checkId, checkTime, shopId, assetId, price, volume, discount));
	}
	mLock.unlock();
	//cout << "d1 - ok2" << endl;
	return resultingContainer;
}

pair<shared_ptr<list<int>>, shared_ptr<list<CPriceItem>>> CDataBase::ReadPrices(int companyId) {
	mLock.lock();
	pqxx::result result;
	try {
		std::string request("SELECT product_id, price, cost FROM price_list WHERE company_id=");
		//cout << "d1" << endl;
		request += std::to_string(companyId);
		result = work->exec(request);
		//cout << "d1 - ok" << endl;
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		mLock.unlock();
		return pair<shared_ptr<list<int>>, shared_ptr<list<CPriceItem>>>(nullptr, nullptr);
	}
	auto assetIds = make_shared<list<int>>();
	auto priceItems = make_shared<list<CPriceItem>>();
	for (const auto row : result) {
		const int assetId = atoi(row[0].c_str());
		const double price = atof(row[1].c_str());
		const double cost = atof(row[2].c_str());
		assetIds->push_back(assetId);
		priceItems->push_back(CPriceItem(price, cost));
	}
	mLock.unlock();
	return pair<shared_ptr<list<int>>, shared_ptr<list<CPriceItem>>>(assetIds, priceItems);
}

shared_ptr<COptimizerParams> CDataBase::ReadBasicConstraints(int companyId) {
	mLock.lock();
	pqxx::result result;
	try {
		std::string request("SELECT product_id, min_price, max_price, role_id, group_id, move_price_up, move_price_down, price_grid_type_id FROM basic_constraints WHERE company_id=");
		request += std::to_string(companyId);
		result = work->exec(request);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		mLock.unlock();
		return nullptr;
	}
	auto paramsPtr = make_shared<COptimizerParams>();

	for (const auto & row : result) {
		const int assetId = atoi(row[0].c_str());
		const double minPrice = atof(row[1].c_str());
		const double maxPrice = atof(row[2].c_str());
		const int roleId = atoi(row[3].c_str());
		const int groupId = atoi(row[4].c_str());
		paramsPtr->AddParamItem(assetId, make_shared<CParamsItem>(groupId, minPrice, maxPrice, roleId));
	}
	mLock.unlock();
	return paramsPtr;
}

void CDataBase::InsertOptimalPrices(const int companyId, shared_ptr<const CPriceList> priceList) {
	mLock.lock();
	const auto assetIds = priceList->GetAllAssets();
	vector<string> vls;
	for (const int assetId : *assetIds) {
		const double price = priceList->GetPrice(assetId);
		vls.push_back(string("(" + to_string(companyId) + ", " + to_string(assetId) + ", " + to_string(price) + ")"));
	}
	const int n = (int)vls.size();
	string request("insert into optimization_results (company_id, product_id, price) values ");
	for (int i = 0; i < n-1; ++i) {
		request += (vls[i] + ", ");
	}
	if (n > 0) {
		request += (vls[n-1] + ";");
	}

	pqxx::result result;
	try {
		pqxx::connection C(this->connStr);
		pqxx::work wk(C);
		//cout << "Deleting..."<< endl;
		result = wk.exec(string("delete from optimization_results where company_id = ") + to_string(companyId));
		wk.commit();
		C.disconnect();
		//cout << "Deleted." << endl;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		mLock.unlock();
		return;
	}
	try {
		pqxx::connection C(this->connStr);
		pqxx::work wk(C);
		//cout << request << endl;
		result = wk.exec(request);
		wk.commit();
		C.disconnect();
		//cout << "Insert done." << endl;
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		mLock.unlock();
		return;
	}
	mLock.unlock();
}

time_t CDataBase::ParseDateToTimeT(const char* strDate) const {
	int yy, month, dd, hh, mm, ss;
	sscanf(strDate, "%d-%d-%d %d:%d:%d", &yy, &month, &dd, &hh, &mm, &ss);
	struct tm whenStart;
    whenStart.tm_year = yy - 1900;
    whenStart.tm_mon = month - 1;
    whenStart.tm_mday = dd;
    whenStart.tm_hour = hh;
    whenStart.tm_min = mm;
    whenStart.tm_sec = ss;
    whenStart.tm_isdst = -1;
    //std::cout << "Time zone: " << whenStart.tm_gmtoff << std::endl;
    time_t tStart = mktime(&whenStart);
    //std::cout << tStart << std::endl;
    return tStart;
}
