/*
 * CSalesMatrix.cpp
 *
 *  Created on: Jun 23, 2019
 *      Author: kraychik
 */

#include "CMatrix.h"
#include <algorithm>

CMatrix::CMatrix() : m(0) {

}

CMatrix::~CMatrix() {

}

/*
void CMatrix::AddColumn(const map<int, double>& columnData) {
	for (const auto & pair : columnData) {
		const int assetId = pair.first;
		const double value = pair.second;
		int rowInd = FindAssetIndex(assetId);
		if (rowInd == -1) {
			rowInd = AddNewEmptyRow(assetId);
		}
		rows[rowInd]->push_back(value);
	}
	m++;
}
*/

shared_ptr<const vector<double> > CMatrix::GetRow(int assetId) const {
	const int rowIndex = FindAssetIndex(assetId);
	return (rowIndex == -1) ? nullptr : rows[rowIndex];
}

shared_ptr<map<int, double> > CMatrix::GetColumn(time_t dt) const {
	const int colId = FindColIndex(dt);
	if (colId == -1) return nullptr;

	auto resMap = make_shared<map<int, double>>();
	for (const auto & pair : mapRowIndex) {
		const int assetId = pair.first;
		resMap->insert(std::pair<int, double>(assetId, rows[pair.second]->at(colId)));
	}
	return resMap;
}

double CMatrix::GetValue(int assetId, time_t dt) const {
	const int colId = FindColIndex(dt);
	if (colId == -1) {
		throw CSmartException("CMatrix::GetValue: cannot find dt column");
	}
	const int i = FindAssetIndex(assetId);
	if (i == -1) {
		throw CSmartException("CMatrix::GetValue: cannot find assetId");
	}
	return rows[i]->at(colId);
}

void CMatrix::SetValue(int assetId, time_t dt, double value) {
	int i = FindAssetIndex(assetId);
	if (i == -1) {
		i = AddNewEmptyRow(assetId);
	}
	int colId = FindColIndex(dt);
	if (colId == -1) {
		colId = AddNewEmptyCol(dt);
	}
	rows[i]->at(colId) = value;
}

int CMatrix::FindAssetIndex(int assetId) const {
	const auto it = mapRowIndex.find(assetId);
	if (it == mapRowIndex.end()) return -1;
	else return it->second;
}

int CMatrix::GetAmountOfRows() const {
	return (int)rows.size();
}

int CMatrix::GetAmountOfColumns() const {
	return m;
}

int CMatrix::FindColIndex(time_t dt) const {
	const auto it = mapColIndex.find(dt);
	return (it == mapColIndex.end()) ? -1 : it->second;
}

int CMatrix::AddNewEmptyRow(int assetId) {
	auto newRow = make_shared<vector<double>>(m, defaultValue);
	rows.push_back(newRow);
	mapRowIndex.insert(std::pair<int, int>(assetId, rows.size()-1));
	return (int)rows.size()-1;
}

void CMatrix::AddPlusValue(int assetId, time_t dt, double value) {
	int i = FindAssetIndex(assetId);
	if (i == -1) {
		i = AddNewEmptyRow(assetId);
	}
	int colId = FindColIndex(dt);
	if (colId == -1) {
		colId = AddNewEmptyCol(dt);
	}
	rows[i]->at(colId) += value;
}

string CMatrix::ToString() const {
	string res = "";
	const auto assetIds = GetAssetIds();
	for (const int assetId : assetIds) {
		const int i = FindAssetIndex(assetId);
		res += to_string(assetId) + ": " + RowToString(i);
	}
	res += "\n";
	return res;
}

int CMatrix::AddNewEmptyCol(time_t dt) {
	const int colId = FindColIndex(dt);
	if (colId != -1) return colId;
	for (auto rowPtr : rows) {
		rowPtr->push_back(defaultValue);
	}
	m++;
	mapColIndex.insert(std::pair<time_t, int>(dt, m-1));
	return m-1;
}

CMatrix CMatrix::Clone() const {
	CMatrix mtx;
	const auto assetIds = GetAssetIds();
	const auto times = GetTimes();
	for (int assetId : assetIds) {
		for (time_t dt : times) {
			mtx.SetValue(assetId, dt, this->GetValue(assetId, dt));
		}
	}
	return mtx;
}

double CMatrix::SumByAssetId(int assetId) const {
	const int rowInd = FindAssetIndex(assetId);
	if (rowInd == -1) {
		throw CSmartException("CMatrix::SumByAssetId: cannot find assetId");
	}
	const auto row = rows[rowInd];
	double s = 0.0;
	for (const double v : *row) {
		s += v;
	}
	return s;
}

double CMatrix::SumByTime(time_t dt) const {
	const int colInd = FindColIndex(dt);
	if (colInd == -1) {
		throw CSmartException("CMatrix::SumByTime: cannot find assetId");
	}
	double s = 0.0;
	for (const auto rowPtr : rows) {
		s += rowPtr->at(colInd);
	}
	return s;
}

void CMatrix::Filter(const set<int>& assetIds, CMatrix& outputMatrix) const {
	const set<time_t> dts = GetTimes();
	outputMatrix.Init(assetIds, dts);
	for (const int assetId : assetIds) {
		for (const time_t dt : dts) {
			const double value = this->GetValue(assetId, dt);
			outputMatrix.SetValue(assetId, dt, value);
		}
	}
}

CMatrix CMatrix::operator /(const map<time_t, int>& amtOfChecks) const {
	bool hasZeros = any_of(amtOfChecks.begin(), amtOfChecks.end(), [](pair<time_t, int> item) { return item.second == 0; });
	if (hasZeros) {
		throw CSmartException("Matrix::operator /: amtOfChecks cannot contain zeros");
	}
	CMatrix mtx;
	const set<int> assetIds = GetAssetIds();
	const set<time_t> dts = GetTimes();
	mtx.Init(assetIds, dts);

	for (const int assetId : assetIds) {
		for (const time_t dt : dts) {
			const double value = this->GetValue(assetId, dt);
			const auto it = amtOfChecks.find(dt);
			if (it == amtOfChecks.end()) {
				throw CSmartException("CMatrix::operator /: cannot execute division - time_t in amtOfChecks not found");
			}
			// it->second does't equal to zero (see start check of procedure)
			mtx.SetValue(assetId, dt, value / (it->second));
		}
	}
	return mtx;
}

string CMatrix::RowToString(int rowId) const {
	const auto rowPtr = rows[rowId];
	string res = "";
	for (int i = 0; i < m-1; ++i) {
		res += to_string(rowPtr->at(i)) + ", ";
	}
	res += to_string(rowPtr->at(m-1)) + "\n";
	return res;
}

set<int> CMatrix::GetAssetIds() const {
	set<int> res;
	for (const auto pair : mapRowIndex) {
		res.insert(pair.first);
	}
	return res;
}

void CMatrix::Init(const set<int> & assetIds, const set<time_t> & times) {
	Empty();
	for (int assetId : assetIds) {
		AddNewEmptyRow(assetId);
	}
	for (time_t dt : times) {
		AddNewEmptyCol(dt);
	}
}

void CMatrix::Empty() {
	rows.clear();
	m = 0;
	mapRowIndex.clear();
	mapColIndex.clear();
}

set<time_t> CMatrix::GetTimes() const {
	set<time_t> res;
	for (const auto pair : mapColIndex) {
		res.insert(pair.first);
	}
	return res;
}
