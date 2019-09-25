/*
 * CSupplementMatrix.h
 *
 *  Created on: Feb 4, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_CSUPPLEMENTMATRIX_H_
#define STRUCTURES_CSUPPLEMENTMATRIX_H_

#pragma once
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <list>
#include <iostream>
#include "../supply/CSmartException.h"
#include "CCheck.h"
#include "CStructureMap.h"
#include "CFilterAdvanced.h"
#include "../http/CJSONCreator.h"

class CSupplementMatrix;
class CSupplementMatrix {
public:
	//CSupplementMatrix();
	CSupplementMatrix(const std::vector<int> & assetIds);
	CSupplementMatrix(const std::set<int> & assetIds);
	CSupplementMatrix(const std::vector<int> & assetIds, const std::list<std::shared_ptr<CCheck>> & checks, const CFilterAdvanced& filter);
	double * operator[](int assetId);
	double operator()(int assetId_i, int assetId_j) const;
	std::shared_ptr<CStructureMap<int, double>> GetSupplementBasket(int assetId) const;
	void SliceAndBuildStructureMap(const std::vector<int> & targetAssetIds, std::list<std::shared_ptr<CStructureMap<int, double>>> & resultingList) const;
	virtual ~CSupplementMatrix();
	std::shared_ptr<CStructureMap<int, double>> GetColumn(int assetId) const;
	std::shared_ptr<std::map<int, std::shared_ptr<CStructureMap<int, double> > > > GetAllColumns() const;
	std::string ToString() const;
	std::vector<int> GetAssetIds() const;
	std::string ToJSONArray(int assetId) const;
	void NormalizeMatrix();
	void MultValueByRowAsset(int assetId, double value, map<int, double> & resultDeltas) const;
	void CalcGroupMatrix(shared_ptr<const CGroupsInfo> groupsInfo, const set<int> & assetIdsFilter);
	std::shared_ptr<CSupplementMatrix> GetGroupMatrix() const;
	void AddSumValue(int iAssetId, int jAssetId, double value);
	int Size() const;
	void SetAmountOfChecks(const std::vector<int> & amts);
	void AddSumCheckAmt(int assetId, int value);
	void DevideByAmountOfChecks();
	void ChooseTopDenseAssets(const int amountOfAssets, set<int> & denseAssetIds) const;
	void FillAssetIds(set<int> & assetIds) const;
	std::vector<double *> GetInnerMatrix() const;
private:
	double Sum(const set<int> & indexes) const;
	void FillIndexesOfMaxValue(int &i, int &j) const;
	int FindAssetIdByIndex(int index) const;
	void FreeMemory();
	void InitializeMemory(int numberOfAssets);
	int FindIndex(int assetId) const;
	void InitializeWithChecks(const std::list<std::shared_ptr<CCheck>> & checks, const CFilterAdvanced & filter);
	void SetMatrixWithValue(const double v);
	int dim = 0;
	std::vector<double *> matrix;
	std::shared_ptr<CSupplementMatrix> groupMatrix = nullptr;
	std::vector<int> amountOfChecks;
	std::vector<int> assetIds;
	std::map<int, int> mapAssetIdIndex;
};

#endif /* STRUCTURES_CSUPPLEMENTMATRIX_H_ */
