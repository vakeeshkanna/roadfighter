#ifndef STRING_CLASS_H 
#define STRING_CLASS_H
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>


class EXPORT StringImpl 
{
public:
	int isNull();
	int getLength();
	char* getPtr();
	StringImpl* clone(StringImpl*);

	StringImpl()
	{
		str = new char[1];
		*str = '\0';
		length = 0;
	}

	StringImpl(const char* _str)
	{
		int length;
		if(_str == NULL)
			return;
		else {
			length = strlen(_str);
			if( length> -1) {
				str = new char[length + 1];
				strcpy(str,_str);
				this->length = length;
			}
		}
	}

	StringImpl(StringImpl& _str)
	{
		if(_str.isNull())
			str = _str.str;
		else {
			str = _str.str;
			length= _str.length;
		}
	}

	~StringImpl()
	{
		if(str != NULL)
			delete []str;
		str = NULL;
		length = -1;
	}
private:
	char *str;
	int length;
};

class EXPORT String 
{
public:
	inline int getLength() { return _str->getLength();}
	int isNull();
	String& operator=(const char* src);
	String& operator=(const String& src);
	String& operator+(const char* src);
	String& operator+(const String& src);
	void toUpper();
	void toLower();
//	static Array < String > & prepareStringArray(const char *str, ...);

	String()
	{
		_str = new StringImpl();

		//length = -1;
	}

	String(const char* str)
		//:_str(str)
	{
		_str = new StringImpl(str);
	}

	String(const String& str) 
	{
		_str = new StringImpl(str._str->getPtr());
	}

	char* str()
	{
		return _str->getPtr();
	}

	~String()
	{
		if(_str != NULL)
			delete _str;
		_str = NULL;
	}

private:
	StringImpl *_str;
};
#endif
