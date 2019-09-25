/*
 * CExcelReader.h
 *
 *  Created on: May 4, 2019
 *      Author: kraychik
 */

#ifndef SUPPLY_CCSVREADER_H_
#define SUPPLY_CCSVREADER_H_

#include "CDataReader.h"
#include <fstream>
#include <vector>
#include <sstream>

class CCSVReader: public CDataReader {
public:
	CCSVReader();
	CCSVReader(const char *pathToSales, const char *pathToPrices);
	virtual ~CCSVReader();

	void SetPathToSales(const char *pathToSales);
	void SetPathToPrices(const char *pathToPrices);

	virtual std::shared_ptr<std::list<CHistoryItem>> ReadSales(int projectId) override;
	virtual std::pair<std::shared_ptr<std::list<int>>, std::shared_ptr<std::list<CPriceItem>>> ReadPrices(int projectId) override;
private:
	const char *pathToSales = nullptr;
	const char *pathToPrices = nullptr;
	time_t ParseDateToTimeT(const char* strDate) const;
};

#endif /* SUPPLY_CCSVREADER_H_ */
