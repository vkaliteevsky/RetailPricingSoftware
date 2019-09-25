#pragma once
#include <exception>
#include <string>
#include <iostream>

class CSmartException : public std::exception
{
public:
	CSmartException();
	CSmartException(int errCode, const char *errText = "Error", int importance = 0);
	CSmartException(const char *errText = "Error");
	CSmartException(const std::string & errText = "Error");
	~CSmartException();
	int errCode;
	std::string errText;
	int importance;
};
