/*
 * CDataBase.h
 *
 *  Created on: Jan 21, 2019
 *      Author: kraychik
 */

#ifndef PROCESSING_CDATABASE_H_
#define PROCESSING_CDATABASE_H_

#include <pqxx/pqxx>
#include <string>
#include <mutex>
#include <memory>
#include <list>
#include "../structures/CHistoryItem.h"
#include "../http/CApp.h"
#include "../structures/CPriceItem.h"
#include "CDataReader.h"

using namespace std;

class COptimizerParams;
class CPriceList;
class CDataBase {
public:
	CDataBase();
	virtual ~CDataBase();
	static CDataBase * GetInstance();
	shared_ptr<list<CHistoryItem>> ReadSales(int projectId);
	pair<shared_ptr<list<int>>, shared_ptr<list<CPriceItem>>> ReadPrices(int companyId);
	shared_ptr<COptimizerParams> ReadBasicConstraints(int companyId);
	void InsertOptimalPrices(const int companyId, shared_ptr<const CPriceList> priceList);
private:
	time_t ParseDateToTimeT(const char *strDate) const;
	static CDataBase instance;
	pqxx::connection *conn;
	pqxx::work *work;
	mutex mLock;
	string connStr;
};

#endif /* PROCESSING_CDATABASE_H_ */
