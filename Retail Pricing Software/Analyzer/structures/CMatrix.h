/*
 * CMatrix.h
 *
 *  Created on: Jun 23, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_CMATRIX_H_
#define STRUCTURES_CMATRIX_H_
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <string>
#include "../supply/CSmartException.h"
#include <ctime>

using namespace std;

// в строчках - активы, в столбцах - даты
class CMatrix {
public:
	CMatrix();
	virtual ~CMatrix();
	void Init(const set<int> & assetIds, const set<time_t> & times);
	void Empty();

	CMatrix operator / (const map<time_t, int> & amtOfChecks) const;

	shared_ptr<const vector<double>> GetRow(int assetId) const;
	shared_ptr<map<int, double>> GetColumn(time_t dt) const;
	double GetValue(int assetId, time_t dt) const;
	void Filter(const set<int> & assetIds, CMatrix & outputMatrix) const;

	void SetValue(int assetId, time_t dt, double value);
	void AddPlusValue(int assetId, time_t dt, double value);
	int GetAmountOfRows() const;
	int GetAmountOfColumns() const;
	string ToString() const;
	CMatrix Clone() const;
	set<int> GetAssetIds() const;
	set<time_t> GetTimes() const;

	double SumByAssetId(int assetId) const;
	double SumByTime(time_t dt) const;
	int AddNewEmptyCol(time_t dt);
protected:
	int FindAssetIndex(int assetId) const;
	int FindColIndex(time_t dt) const;
	int AddNewEmptyRow(int assetId);

	string RowToString(int rowId) const;
private:
	vector<shared_ptr<vector<double>>> rows;
	map<int, int> mapRowIndex;
	map<time_t, int> mapColIndex;
	int m; // amount of columns
	const double defaultValue = 0.0;
};

#endif /* STRUCTURES_CMATRIX_H_ */
