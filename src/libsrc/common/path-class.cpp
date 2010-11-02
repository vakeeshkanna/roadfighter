#include "path-class.h"
#include "log-class.h"

Path::Path()
{

}


Path::~Path()
{

}

vector<string> Path::namesOfFilesInsideADirectory(string path)
{
	vector<string> names;
	names.clear();

	WIN32_FIND_DATA f;

	HANDLE h = FindFirstFile(path.c_str(), &f);
	if(h != INVALID_HANDLE_VALUE)
	{
		do
		{
			string name = f.cFileName;

			//skip . and ..
			if(name.compare(".") == 0 || name.compare("..") == 0)
			{
				continue;
			}
			names.push_back(name);
		}
		while(FindNextFile(h, &f));
	}
	else
	{
		lprintf("Error opening directory %s\n", path);
	}
	 return names;
}

int Path::getFileSize(string path)
{
	FILE *pFile = NULL;

	// get the file stream
	fopen_s( &pFile, path.c_str(), "rb" );

	// set the file pointer to end of file
	fseek( pFile, 0, SEEK_END );

	// get the file size
	int Size = ftell( pFile );

	// close stream and release buffer
	fclose( pFile );

	return Size;
}
