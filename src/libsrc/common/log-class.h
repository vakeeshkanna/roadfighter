#ifndef LOG_CLASS_H
#define LOG_CLASS_H
//#define MAX_FILE_PATH 100
#define LOG_MAX_PATH 100
#define	NULLBUFFERLENGTH	0x0400
#include "string-class.h"
#include "array-class.h"
#include <stdio.h>
#include "basedefs.h"
#include <stdarg.h>
#ifdef WIN32
#include <direct.h>
#endif
#ifdef LINUX
#include <unistd.h>
#endif
extern int EXPORT lprintf (const char *format, ...);

class EXPORT Log
{
	friend int EXPORT lprintf (const char *format, ...);
	FILE *fp;
	String fname;
	static Log* log;
	Logical isFileOpen;
public:
	Log();
	Log(String&);
	~Log();
	void open();
	void close();
	Logical isOpen();
	static Log* getInstance();
	int vpf(const char *format, va_list args);
};
#endif
