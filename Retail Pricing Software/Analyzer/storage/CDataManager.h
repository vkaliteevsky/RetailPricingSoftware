/*
 * CDataManager.h
 *
 *  Created on: Feb 23, 2019
 *      Author: kraychik
 */

#ifndef STORAGE_CDATAMANAGER_H_
#define STORAGE_CDATAMANAGER_H_

#include <map>
#include <string>
#include <vector>
#include <set>
#include <memory>
#include <mutex>
#include "CProjectInfo.h"

class CProjectInfo;
class CDataManager {
public:
	void operator =(const CDataManager &) = delete;
	CDataManager(const CDataManager &) = delete;
	virtual ~CDataManager();

	bool IsLoaded(int projectId) const;
	void LoadProject(int projectId);
	void UpdateProject(int projectId);
	void UnloadProject(int projectId);
	shared_ptr<CProjectInfo> GetProjectInfo(int projectId) const;
	shared_ptr<CProjectInfo> GetOrLoadProjectInfo(int projectId);
	shared_ptr<CProjectInfo> operator [](int projectId) const;

	static CDataManager *GetInstance();
private:
	CDataManager();
	static CDataManager *instance;
	map<int, shared_ptr<CProjectInfo>> mapProjects;
	mutex mLock;
};

#endif /* STORAGE_CDATAMANAGER_H_ */
