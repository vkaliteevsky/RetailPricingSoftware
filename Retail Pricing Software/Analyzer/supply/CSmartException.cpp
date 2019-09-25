#include "CSmartException.h"



CSmartException::CSmartException() : errCode(0), errText(""), importance(0)
{
	
}

CSmartException::CSmartException(int errCode, const char * errText, int importance)
{
	this->errCode = errCode;
	this->errText = errText;
	this->importance = importance;
}

CSmartException::CSmartException(const char * errText)
{
	this->errCode = 0;
	this->errText = errText;
	this->importance = 0;
	std::cout << errText << std::endl;
}


CSmartException::~CSmartException()
{
}

CSmartException::CSmartException(const std::string& errText) : CSmartException(errText.c_str()) {

}
