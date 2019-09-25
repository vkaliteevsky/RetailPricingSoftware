/*
 * CSupplementMatrix.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: kraychik
 */

#include "CSupplementMatrix.h"
#include <algorithm>

std::shared_ptr<CStructureMap<int, double>> CSupplementMatrix::GetSupplementBasket(int assetId) const {
	auto resultingMap = std::make_shared<CStructureMap<int, double>>();
	int foundIndex = FindIndex(assetId);
	if (foundIndex < 0) {
		printf("No index in matrix\n");
		throw CSmartException("No index in matrix");
	}
	double *basket = matrix[foundIndex];
	for (int i = 0; i < dim; ++i) {
		resultingMap->AddItem(assetIds[i], basket[i]);
	}
	//resultingMap->insert(std::pair<int, double>(assetIds[i], basket[i]));
	return resultingMap;
}

CSupplementMatrix::~CSupplementMatrix() {
	FreeMemory();
}

void CSupplementMatrix::FreeMemory() {
	for (int i = 0; i < dim; ++i)
		delete [] matrix[i];
	//delete [] matrix;
	//delete [] amountOfChecks;
	//delete [] assetIds;
	dim = 0;
}

double* CSupplementMatrix::operator [](int assetId) {
	auto foundAsset = mapAssetIdIndex.find(assetId);
	if (foundAsset == mapAssetIdIndex.end()) {
		printf("Error in operator [] in matrix. Wrong index assetId");
		throw CSmartException("Wrong index");
	}
	return matrix[foundAsset->second];
}

std::string CSupplementMatrix::ToString() const {
	std::string res = "Assets: ";
	for (const auto assetId : assetIds) {
		res += std::to_string(assetId) + ", ";
	}
	res += std::string("\nAmount of Checks: ");
	for (const auto amt : amountOfChecks) {
		res += std::to_string(amt) + ", ";
	}
	res += std::string("\nMatrix:\n");
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			res += std::to_string(matrix[i][j]) + " ";
		}
		res += "\n";
	}
	return res;
}

double CSupplementMatrix::operator ()(int assetId_i, int assetId_j) const {
	int i = FindIndex(assetId_i);
	int j = FindIndex(assetId_j);
	if ((i == -1) || (j == -1)) {
		printf("Found wrong Index\n");
		throw CSmartException("Cannot find correct index");
	}
	return matrix[i][j];
}

void CSupplementMatrix::InitializeMemory(int numberOfAssets) {
	if (numberOfAssets <= 0) return;
	dim = numberOfAssets;
	amountOfChecks.resize(dim, 0);
	//matrix = new double *[dim];
	matrix.resize(dim, 0);
	for (int i = 0; i < dim; ++i) {
		matrix[i] = new double[dim];
		for (int j = 0; j < dim; ++j) {
			matrix[i][j] = 0.0;
		}
	}
}

std::vector<int> CSupplementMatrix::GetAssetIds() const {
	return assetIds;
}
void CSupplementMatrix::FillAssetIds(set<int> & assetIds) const {
	assetIds.clear();
	assetIds.insert(this->assetIds.begin(), this->assetIds.end());
}

int CSupplementMatrix::FindIndex(int assetId) const {
	auto foundIndex = mapAssetIdIndex.find(assetId);
	if (foundIndex == mapAssetIdIndex.end()) {
		throw CSmartException("CSupplementMatrix::FindIndex: Wrong assetId, cannot find it");
	}
	return foundIndex->second;
}

CSupplementMatrix::CSupplementMatrix(const std::vector<int>& assetIds, const std::list<std::shared_ptr<CCheck> >& checks, const CFilterAdvanced& filter)
 : CSupplementMatrix(assetIds) {
	InitializeWithChecks(checks, filter);
	NormalizeMatrix();
}

void CSupplementMatrix::InitializeWithChecks(const std::list<std::shared_ptr<CCheck> >& checks, const CFilterAdvanced& filter) {
	SetMatrixWithValue(0.0);
	for (const auto & check : checks) {
		std::list<std::shared_ptr<const CCheckItem> > resList;
		check->FilterItems(filter, resList);
		for (const auto & item : resList) {
			int index1 = FindIndex(item->assetId);
			amountOfChecks.at(index1) = amountOfChecks.at(index1) + 1;
			for (const auto & item2 : resList) {
				int index2 = FindIndex(item2->assetId);
				matrix[index1][index2] = matrix[index1][index2] + item2->volume;
			}
		}
	}
	for (int i = 0; i < dim; i++) {
		if (amountOfChecks[i] > 0) {
			for (int j = 0; j < dim; ++j) {
				matrix[i][j] = matrix[i][j] / amountOfChecks[i];
			}
		}
	}
}

std::string CSupplementMatrix::ToJSONArray(int assetId) const {
	int index = FindIndex(assetId);
	const double *values = matrix[index];
	return CJSONCreator::MakeArray(values, dim);
}

std::shared_ptr<CStructureMap<int, double>> CSupplementMatrix::GetColumn(int assetId) const {
	int index = FindIndex(assetId);
	auto result = std::make_shared<CStructureMap<int, double>>();
	for (uint i = 0; i < assetIds.size(); ++i) {
		result->AddItem(assetId, matrix[i][index]);
	}
	return result;
}

std::shared_ptr<std::map<int, std::shared_ptr<CStructureMap<int, double> > > > CSupplementMatrix::GetAllColumns() const {
	auto resultingContainer = std::make_shared<std::map<int, std::shared_ptr<CStructureMap<int, double> > >>();
	//resultingContainer->reserve(dim);
	for (const int assetId : assetIds) {
		resultingContainer->insert(std::pair<int, std::shared_ptr<CStructureMap<int, double> >>(assetId, this->GetColumn(assetId)));
	}
	return resultingContainer;

}

std::shared_ptr<CSupplementMatrix> CSupplementMatrix::GetGroupMatrix() const {
	return groupMatrix;
}

void CSupplementMatrix::AddSumValue(int iAssetId, int jAssetId, double value) {
	int i = FindIndex(iAssetId);
	int j = FindIndex(jAssetId);
	matrix[i][j] += value;
}

int CSupplementMatrix::Size() const {
	return dim;
}

void CSupplementMatrix::SetAmountOfChecks(const std::vector<int>& amts) {
	amountOfChecks = amts;
}

void CSupplementMatrix::AddSumCheckAmt(int assetId, int value) {
	int index = FindIndex(assetId);
	amountOfChecks[index] += value;
}

void CSupplementMatrix::DevideByAmountOfChecks() {
	const int n = dim;
	for (int i = 0; i < n; ++i) {
		if (amountOfChecks[i] > 0) {
			for (int j = 0; j < n; ++j) {
				matrix[i][j] /= amountOfChecks[i];
			}
		}
	}
}

CSupplementMatrix::CSupplementMatrix(const std::set<int>& assetIds) {
	const int n = (int)assetIds.size();
	InitializeMemory(n);
	this->assetIds.resize(n);
	copy(assetIds.begin(), assetIds.end(), this->assetIds.begin());
	for (int i = 0; i < n; ++i) {
		mapAssetIdIndex[this->assetIds[i]] = i;
	}
}

void CSupplementMatrix::ChooseTopDenseAssets(const int amountOfAssets, set<int>& denseAssetIds) const {
	denseAssetIds.clear();
	if (amountOfAssets > dim) {
		denseAssetIds.insert(assetIds.begin(), assetIds.end());
		return;
	}
	if (amountOfAssets == 0) {
		return;
	}
	if (amountOfAssets < 0) {
		throw CSmartException("CSupplementMatrix::ChooseTopDenseAssets: amountOfAssets is null");
	}
	set<int> indexes;
	int i = 0, j = 0;
	FillIndexesOfMaxValue(i, j);
	indexes.insert(i);

	i = 1;
	while (i < amountOfAssets) {
		vector<double> optionVals(dim, -100.0);
		for (j = 0; j < dim; j++) {
			const auto it = indexes.find(j);
			if (it != indexes.end()) {
				continue;
			}
			indexes.insert(j);
			optionVals[j] = Sum(indexes);
			indexes.erase(j);
		}
		const auto maxElement = std::max_element(optionVals.begin(), optionVals.end());
		const int bestIndex = distance(optionVals.begin(), maxElement);
		indexes.insert(bestIndex);
		i++;
	}

	for (const int index : indexes) {
		const int assetId = FindAssetIdByIndex(index);
		denseAssetIds.insert(assetId);
	}
}

void CSupplementMatrix::FillIndexesOfMaxValue(int& i, int& j) const {
	double mxValue = -1000000;
	for (int ii = 0; ii < dim; ii++) {
		for (int jj = 0; jj < dim; jj++) {
			if (matrix[ii][jj] > mxValue) {
				mxValue = matrix[ii][jj];
				i = ii; j = jj;
			}
		}
	}
}

double CSupplementMatrix::Sum(const set<int>& indexes) const {
	double s = 0.0;
	for (int i : indexes) {
		for (int j : indexes) {
			s += matrix[i][j];
		}
	}
	return s;
}

std::vector<double*> CSupplementMatrix::GetInnerMatrix() const {
	return matrix;
}

void CSupplementMatrix::SetMatrixWithValue(const double v) {
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			matrix[i][j] = v;
}

CSupplementMatrix::CSupplementMatrix(const std::vector<int>& assetIds) {
	InitializeMemory((int)assetIds.size());
	this->assetIds = assetIds;
	for (unsigned int i = 0; i < assetIds.size(); ++i) {
		mapAssetIdIndex[assetIds[i]] = i;
	}
}

void CSupplementMatrix::SliceAndBuildStructureMap(const std::vector<int>& targetAssetIds, std::list<std::shared_ptr<CStructureMap<int, double> > >& resultingList) const {
	resultingList.clear();
	for (const int targetAssetId : targetAssetIds) {
		int indexRow = FindIndex(targetAssetId);
		double *values = matrix[indexRow];
		auto newMap = std::make_shared<CStructureMap<int, double>>(this->assetIds, values, dim);
		resultingList.push_back(newMap);
	}
}

void CSupplementMatrix::NormalizeMatrix() {
	for (int i = 0; i < dim; ++i) {
		const double devisor = matrix[i][i];
		if (abs(devisor) <= 0.0001) {
			throw CSmartException("CSupplementMatrix::NormalizeMatrix: zero found on supplement matrix diagonal");
		}
		for (int j = 0; j < dim; ++j) {
			matrix[i][j] /= devisor;
		}
	}
}

int CSupplementMatrix::FindAssetIdByIndex(int index) const {
	if ((index >= dim) || (index < 0)) {
		throw CSmartException("CSupplementMatrix::FindAssetIdByIndex: index out of dimentions");
	}
	return assetIds[index];
}

void CSupplementMatrix::MultValueByRowAsset(int assetId, double value, map<int, double>& resultDeltas) const {
	resultDeltas.clear();
	const int indexRow = FindIndex(assetId);
	for (int i = 0; i < dim; i++) {
		const double v = matrix[indexRow][i];
		const double aId = FindAssetIdByIndex(i);
		resultDeltas.insert(make_pair(aId, v * value));
	}
}

void CSupplementMatrix::CalcGroupMatrix(shared_ptr<const CGroupsInfo> groupsInfo, const set<int> & assetIdsFilter) {
	set<int> groupIds;
	groupsInfo->FillGroups(assetIdsFilter, groupIds);

	groupMatrix = std::make_shared<CSupplementMatrix>(groupIds);
	for (int i = 0; i < dim; ++i) {
		const int iAssetId = FindAssetIdByIndex(i);
		const int iGroupId = groupsInfo->GetGroupByAsset(iAssetId);
		for (int j = 0; j < dim; j++) {
			const int jAssetId = FindAssetIdByIndex(j);
			const int jGroupId = groupsInfo->GetGroupByAsset(jAssetId);
			const double salesVolume = matrix[i][j] * amountOfChecks[i];
			groupMatrix->AddSumValue(iGroupId, jGroupId, salesVolume);
		}
		groupMatrix->AddSumCheckAmt(iGroupId, amountOfChecks[i]);
	}

}
