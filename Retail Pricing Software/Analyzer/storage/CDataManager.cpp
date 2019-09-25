/*
 * CDataManager.cpp
 *
 *  Created on: Feb 23, 2019
 *      Author: kraychik
 */

#include "CDataManager.h"

CDataManager::~CDataManager() {
	// TODO Auto-generated destructor stub
}

void CDataManager::LoadProject(int projectId) {
	if (!IsLoaded(projectId)) {
		auto newProject = make_shared<CProjectInfo>(projectId);
		mapProjects[projectId] = newProject;
	}
}

bool CDataManager::IsLoaded(int projectId) const {
	return (mapProjects.find(projectId) != mapProjects.end());
}

shared_ptr<CProjectInfo> CDataManager::GetProjectInfo(int projectId) const {
	auto it = mapProjects.find(projectId);
	if (it == mapProjects.end()) {
		return nullptr;
	} else {
		return it->second;
	}
}

void CDataManager::UpdateProject(int projectId) {
	auto it = mapProjects.find(projectId);
	if (it == mapProjects.end()) {
		LoadProject(projectId);
	} else {
		it->second->Update();
	}
}

void CDataManager::UnloadProject(int projectId) {
}

CDataManager *CDataManager::GetInstance() {
	if (instance == nullptr) {
		instance = new CDataManager();
	}
	return instance;
}

shared_ptr<CProjectInfo> CDataManager::operator [](int projectId) const {
	const auto it = mapProjects.find(projectId);
	return (it == mapProjects.end()) ? nullptr : (it->second);
}

shared_ptr<CProjectInfo> CDataManager::GetOrLoadProjectInfo(int projectId) {
	if (!IsLoaded(projectId)) {
		LoadProject(projectId);
	}
	return GetProjectInfo(projectId);
}

CDataManager::CDataManager() {
	mLock.unlock();
}

CDataManager *CDataManager::instance = nullptr;
