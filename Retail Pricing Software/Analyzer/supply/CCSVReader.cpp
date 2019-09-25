/*
 * CExcelReader.cpp
 *
 *  Created on: May 4, 2019
 *      Author: kraychik
 */

#include "CCSVReader.h"

CCSVReader::CCSVReader() {
	// TODO Auto-generated constructor stub
}

CCSVReader::CCSVReader(const char* pathToSales, const char* pathToPrices) {
	SetPathToSales(pathToSales);
	SetPathToPrices(pathToPrices);
}

CCSVReader::~CCSVReader() {
	// TODO Auto-generated destructor stub
}

void CCSVReader::SetPathToSales(const char* pathToSales) {
	this->pathToSales = pathToSales;
}

void CCSVReader::SetPathToPrices(const char* pathToPrices) {
	this->pathToPrices = pathToPrices;
}

std::shared_ptr<std::list<CHistoryItem> > CCSVReader::ReadSales(int projectId) {
	std::fstream fin;
	fin.open("tx_list.csv", std::ios::in);
	int checkId, branchId, assetId, pId;
	std::string temp, line, word;
	double price, quantity;
	time_t dateTime;

	std::vector<std::string> row;
	//std::cout << "Running ReadSales..." << std::endl;
	auto resultingListPtr = std::make_shared<std::list<CHistoryItem>>();
	getline(fin, line); // читаем первую строку с заголовками
	while (getline(fin, line)) {
		row.clear();
		std::stringstream s(line);
		while (getline(s, word, ',')) {
			row.push_back(word);
		}
		//std::cout << row[0] << " " << row[1] << " " << row[2] << " " << row[3] << " " << row[4] << std::endl;
		checkId = stoi(row[0]);
		dateTime = ParseDateToTimeT(row[1].c_str());
		branchId = stoi(row[2]);
		assetId = stoi(row[3]);
		price = stof(row[4]);
		quantity = stof(row[5]);
		pId = stoi(row[6]);
		resultingListPtr->push_back(CHistoryItem(checkId, dateTime, branchId, assetId, price, quantity, 0));
		//std::cout << checkId << " " << dateTime << " " << branchId << " " << assetId << " " << price << " " << quantity << " " << pId << std::endl;
	}
	return resultingListPtr;
}

std::pair<std::shared_ptr<std::list<int> >, std::shared_ptr<std::list<CPriceItem> > > CCSVReader::ReadPrices(int projectId) {
	return std::pair<std::shared_ptr<std::list<int> >, std::shared_ptr<std::list<CPriceItem> > >(nullptr, nullptr);
}

time_t CCSVReader::ParseDateToTimeT(const char* strDate) const {
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
