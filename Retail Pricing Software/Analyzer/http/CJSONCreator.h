/*
 * CJSONCreator.h
 *
 *  Created on: Feb 1, 2019
 *      Author: kraychik
 */

#ifndef HTTP_CJSONCREATOR_H_
#define HTTP_CJSONCREATOR_H_
#include <string>
#include <vector>
#include <map>

using namespace std;

class CJSONCreator {
public:
	CJSONCreator();
	virtual ~CJSONCreator();
	static string CreateKeyValueItem(const string & keyName, const string & keyValue);
	static string CreateKeyValueItem(const string & keyName, const int & keyValue);
	static string CreateKeyValueItem(const string & keyName, const double & keyValue);
	static string CreateKeyValueItem(const string& keyName, const bool keyValue);
	static string MakeObject(const string strObject);
	static string PutQuotes(const string & str);
	static string MakeArray(const vector<string> & strObjects);
	template <typename T>
	static string MakeArray(const T* array, int amount);
	template <typename T>
	static string MakeArray(const vector<T> & array);
	static string SeparateObjects(const string & object1, const string & object2);
	static string SeparateObjects(const vector<string> & objects);
	template <typename K, typename V>
	static string MakeKeyValueArray(const std::map<K, V> & mapVals, const string & keyName, const string & valueName);
	static string CreateClassicResponse(int calcId, const char* response, const char* error, bool isSuccess);
private:
	static bool IsJSONObject(const string & strObject);
};

#endif /* HTTP_CJSONCREATOR_H_ */
