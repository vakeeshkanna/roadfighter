#include "string-class.h"

int StringImpl::isNull()
{
	if(str == NULL)
		return 1;
	else
		return 0;
}

int StringImpl::getLength()
{
	return length;
}

char* StringImpl::getPtr()
{
	return str;
}

StringImpl* StringImpl::clone(StringImpl* src)
{
	StringImpl* result = NULL;
	if(!(src->isNull() && src->getLength() > 0)) {
		result->str = new char[src->getLength() + 1];
		strcpy(result->str,src->getPtr());
		result->length = src->getLength();
	}
	return result;
}

int String::isNull()
{
	return _str->isNull() ? 1 : 0;
}

String& String::operator=(const char* src)
{
	if(_str != NULL)
		delete _str;
	_str = new StringImpl(src);
	return *this;
}

String& String::operator=(const String& src)
{
	if(strcmp(_str->getPtr(),src._str->getPtr()) == 0)
		return *this;
		
	char *tmp = new char[src._str->getLength() + 1];
	strcpy(tmp,src._str->getPtr());
	if(_str != NULL)
		delete _str;
	_str = new StringImpl(tmp);
	delete []tmp;
	return *this;
}

String& String::operator+(const char* src)
{
	if(!_str->isNull() && src != NULL)
	{
		char *tmp =new char[_str->getLength() + strlen(src) + 1];
		strcpy(tmp,_str->getPtr());
		strcat(tmp,src);
		delete _str;
		_str = new StringImpl(tmp);
		delete []tmp;
	}
	return *this;
}

String& String::operator+(const String& src)
{
	if(!_str->isNull() && !src._str->isNull())
		{
			char *tmp =new char[_str->getLength() + src._str->getLength() + 1];
			strcpy(tmp,_str->getPtr());
			strcat(tmp,src._str->getPtr());
			delete _str;
			_str = new StringImpl(tmp);
			delete []tmp;
			
		}
		return *this;
}

void String::toLower()
{
	char *dst = _str->getPtr();
	
	while(*dst) {
		*dst = tolower(*dst);
		dst++;
	}
}

void String::toUpper()
{
	char *dst = _str->getPtr();
	while(*dst) {
		*dst = toupper(*dst);
		 dst++;
	}
}

/*Array < String > &String::prepareStringArray(const char *str, ...)
{
	Array < String> result;
	va_list args;
	va_start(args,str);
	//	result.init(2);
		for(int i = 0;i < 1;i++) {
			char *tmp = va_arg (args,char *);
			result[i] = tmp;//va_arg (args,char*);
		}
	va_end(args);
	Array < String > & ret = result; 
	return result;
}
*/


