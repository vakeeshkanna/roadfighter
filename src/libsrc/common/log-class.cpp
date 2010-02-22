#include "log-class.h"

Log* Log::log = NULL;
Log::Log()
{
	isFileOpen = no;
	char temp[LOG_MAX_PATH];
	String tmp = getcwd(temp,LOG_MAX_PATH);
#ifdef WIN32
	fname = tmp + "\\" +"default.log";
#else
	fname = tmp + "/" + "default.log";
#endif
}

Log::Log(String &name)
{
	isFileOpen = no;
	fname = name;
}

Log::~Log()
{
	if(isOpen())
	{
		fclose(fp);
		isFileOpen = no;
	}
}

void Log::open()
{
	if(!isOpen())
	{
		fp = fopen(fname.str(),"a");
		isFileOpen = yes;
	}
}

void Log::close()
{
	if(isOpen())
	{
		fclose(fp);
		isFileOpen = no;
	}
}

Logical Log::isOpen()
{
	return isFileOpen;
}

Log* Log::getInstance()
{
	if(log == NULL)
		log = new Log();
	log->open();
	return log;
}

int Log::vpf (const char *format, va_list args)
{
	char	buffer[0x4000] ;
	int r = vsprintf (buffer, format, args) ;

	assert (r < 0x4000) ;
	assert(fp);
	fprintf(fp, "%s", buffer);

	fflush(fp);
	close();
	return r ;
}
int lprintf (const char *format, ...)
{
	int r;
	va_list	vargs;
#ifdef WIN32
	// Since these lines go into a logfile, zap any log comments which
	// have a carriage return but no linefeed.
	if(strchr(format,'\r') != NULL) return -1;
#endif
	Log *log = Log::getInstance();
	va_start (vargs, format) ;
	r = log->vpf (format, vargs) ;
	va_end(vargs) ;
	return r ;
}


