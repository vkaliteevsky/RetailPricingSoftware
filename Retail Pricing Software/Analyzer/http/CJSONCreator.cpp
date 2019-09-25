/*
 * CJSONCreator.cpp
 *
 *  Created on: Feb 1, 2019
 *      Author: kraychik
 */

#include "CJSONCreator.h"

CJSONCreator::CJSONCreator() {
	// TODO Auto-generated constructor stub

}

CJSONCreator::~CJSONCreator() {
	// TODO Auto-generated destructor stub
}

template <typename K, typename V>
string CJSONCreator::MakeKeyValueArray(const std::map<K, V> & mapVals, const string & keyName, const string & valueName) {
	if (mapVals.empty()) {
		return string("[ ]");
	}
	vector<string> objects;
	for (const auto & item : mapVals) {
		string obj1 = CJSONCreator::CreateKeyValueItem(keyName, item.first);
		string obj2 = CJSONCreator::CreateKeyValueItem(valueName, item.second);
		objects.push_back(CJSONCreator::MakeObject(CJSONCreator::SeparateObjects(obj1, obj2)));
	}
	string result = CJSONCreator::MakeArray(objects);
	return result;
}

string CJSONCreator::PutQuotes(const string& str) {
	return string("\"") + str + "\"";
}

string CJSONCreator::MakeArray(const vector<string>& strObjects) {
	auto it = strObjects.begin();
	if (it == strObjects.end()) return string("[ ]");
	string res = *it;
	it++;
	while (it != end(strObjects)) {
		res = CJSONCreator::SeparateObjects(res, *it);
		it++;
	}
	res = string("[ ") + res + " ]";
	return res;
}

string CJSONCreator::MakeObject(const string strObject) {
	return string("{ ") + strObject + " }";
}

string CJSONCreator::CreateKeyValueItem(const string& keyName, const string& keyValue) {
	if (IsJSONObject(keyValue)) {
		return (PutQuotes(keyName) + ": " + keyValue);
	}
	else return (PutQuotes(keyName) + ": " + PutQuotes(keyValue));
}

string CJSONCreator::CreateKeyValueItem(const string& keyName, const int& keyValue) {
	return (PutQuotes(keyName) + ": " + to_string(keyValue));
}

string CJSONCreator::CreateKeyValueItem(const string& keyName, const bool keyValue) {
	return (PutQuotes(keyName) + ": " + (keyValue ? "true" : "false"));
}

string CJSONCreator::CreateKeyValueItem(const string& keyName, const double& keyValue) {
	return (PutQuotes(keyName) + ": " + to_string(keyValue));
}

string CJSONCreator::SeparateObjects(const string & object1, const string & object2) {
	return (object1 + ", " + object2);
}

string CJSONCreator::CreateClassicResponse(int calcId, const char* response, const char* error, bool isSuccess) {
	string res = CreateKeyValueItem("calcId", calcId);
	if (response != nullptr) {
		res = SeparateObjects(res, CreateKeyValueItem("response", string(response)));
	}
	if (error != nullptr) {
		res = SeparateObjects(res, CreateKeyValueItem("error", string(error)));
	}
	res = SeparateObjects(res, CreateKeyValueItem("success", isSuccess));
	res = MakeObject(res);
	return res;
}

string CJSONCreator::SeparateObjects(const vector<string>& objects) {
	auto it = objects.begin();
	if (it == objects.end()) return "";
	string result = *it;
	it++;
	while (it != objects.end()) {
		result = SeparateObjects(result, *it);
		it++;
	}
	return result;
}

bool CJSONCreator::IsJSONObject(const string& strObject) {
	if (strObject.empty()) return true;
	const char *str = strObject.c_str();
	while (*str == ' ')
		str++;
	return ((*str == '{') || (*str == '['));
}

template<typename T>
string CJSONCreator::MakeArray(const T* array, int amount) {
	vector<string> arr(amount);
	for (int i = 0; i < amount; ++i) {
		arr[i] = to_string(array[i]);
	}
	string result = MakeArray(arr);
	return result;
}

template<typename T>
string CJSONCreator::MakeArray(const vector<T> & array) {
	int amount = (int)array.size();
	vector<string> arr(amount);
	for (int i = 0; i < amount; ++i) {
		arr[i] = to_string(array[i]);
	}
	string result = MakeArray(arr);
	return result;
}
template string CJSONCreator::MakeKeyValueArray<int, double>(const std::map<int, double> &, const string &, const string &);
template string CJSONCreator::MakeKeyValueArray<int, int>(const std::map<int, int> &, const string &, const string &);
template string CJSONCreator::MakeKeyValueArray<int, std::string>(const std::map<int, std::string> &, const string &, const string &);

template string CJSONCreator::MakeArray(const double*, int);
template string CJSONCreator::MakeArray(const int*, int);
template string CJSONCreator::MakeArray(const vector<double> &);
template string CJSONCreator::MakeArray(const vector<int> &);
