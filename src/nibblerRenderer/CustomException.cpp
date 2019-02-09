
#include "CustomException.hpp"

CustomException::CustomException(std::string const &err)
{
	text = err;
};

const char* CustomException::what() const throw()
{
	std::string res;
	res = "Exception::" + text;
	return (res.c_str());
};

CustomException::~CustomException() throw()
{

};

CustomException::CustomException()
{

};

CustomException::CustomException(CustomException const &)
{

};

CustomException &CustomException::operator=(CustomException const &)
{
	return *this;
};
