/*
 * CStructureMap.h
 *
 *  Created on: Feb 6, 2019
 *      Author: kraychik
 */

#ifndef STRUCTURES_CSTRUCTUREMAP_H_
#define STRUCTURES_CSTRUCTUREMAP_H_

#include <string>
#include <map>
#include <vector>
#include "../http/CJSONCreator.h"
#include "../supply/CSmartException.h"

template <class A, class V>
class CStructureMap {
public:
	CStructureMap();
	CStructureMap(const std::vector<A> & keys, const std::vector<V> & values);
	CStructureMap(const std::vector<A> & keys, const V *values, int amountOfValues);
	virtual ~CStructureMap();
	void AddItem(A key, V value);
	void EraseItem(A key);
	void Clear();

	V Sum() const;
	V Average() const;
	int Size() const;
	std::string ToString() const;
	std::string ToJSON(const std::string & keyName, const std::string & valueName) const;
	std::map<A, V> structure;
};

#endif /* STRUCTURES_CSTRUCTUREMAP_H_ */
