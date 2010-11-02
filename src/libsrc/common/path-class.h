#ifndef PATH_CLASS_H
#define PATH_CLASS_H

#include <map>
#include <string>
#include <vector>

using namespace std;

class EXPORT Path
{
public:
	Path();
	~Path();
	static vector<string> namesOfFilesInsideADirectory(string path);
	static int getFileSize(string path);
};
#endif
