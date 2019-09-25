/*
 * CStructureMap.cpp
 *
 *  Created on: Feb 6, 2019
 *      Author: kraychik
 */

#include "CStructureMap.h"

template<class A, class V>
CStructureMap<A, V>::CStructureMap() {
}

template<class A, class V>
CStructureMap<A, V>::CStructureMap(const std::vector<A>& keys, const std::vector<V>& values) {
	if (keys.size() != values.size()) {
		throw CSmartException("CStructureMap<A, V>::CStructureMap: Keys.size must be equal to Values.size");
	}
	for (unsigned int i = 0; i < keys.size(); ++i) {
		structure.insert(std::pair<A, V>(keys[i], values[i]));
	}
}

template<class A, class V>
CStructureMap<A, V>::~CStructureMap() {
}

template<class A, class V>
std::string CStructureMap<A, V>::ToString() const {
	std::string res = "StructureMap:\n";
	for (const auto & item : structure) {
		res += std::to_string(item.first) + ": " + std::to_string(item.second) + "\n";
	}
	return res;
}

template<class A, class V>
inline void CStructureMap<A, V>::AddItem(A key, V value) {
	structure.insert(std::pair<A, V>(key, value));
}

template<class A, class V>
inline void CStructureMap<A, V>::EraseItem(A key) {
	structure.erase(key);
}

template<class A, class V>
CStructureMap<A, V>::CStructureMap(const std::vector<A>& keys, const V* values, int amountOfValues) {
	if ((int)keys.size() != amountOfValues) {
		throw CSmartException("CStructureMap<A, V>::CStructureMap: Keys.size must be equal to amountOfValues");
	}
	for (int i = 0; i < amountOfValues; ++i) {
		structure.insert(std::pair<A, V>(keys[i], values[i]));
	}
}

template<class A, class V>
std::string CStructureMap<A, V>::ToJSON(const std::string & keyName, const std::string & valueName) const {
	//std::string res = "Hello";
	std::string res = CJSONCreator::MakeKeyValueArray<A, V>(structure, keyName, valueName);
	return res;
}

template<class A, class V>
V CStructureMap<A, V>::Sum() const {
	V resSum = 0;
	for (const auto & item : structure) {
		resSum += item.second;
	}
	return resSum;
}

template<class A, class V>
V CStructureMap<A, V>::Average() const {
	const int size = Size();
	if (size == 0) {
		return (V)0;
	} else
	{
		return Sum() / size;
	}
}

template<class A, class V>
int CStructureMap<A, V>::Size() const {
	return (int)structure.size();
}


template<class A, class V>
void CStructureMap<A, V>::Clear() {
	structure.clear();
}

template class CStructureMap<int, double>;
template class CStructureMap<int, int>;
