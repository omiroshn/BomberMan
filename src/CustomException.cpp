
#include "CustomException.hpp"

CustomException::CustomException(std::string const &err)
{
	text = "Exception::" + err;
};

const char* CustomException::what() const throw()
{
	return (text.c_str());
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
