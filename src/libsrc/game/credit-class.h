#ifndef CREDIT_CLASS_H
#define CREDIT_CLASS_H

#include <map>

using namespace std;

class EXPORT Credit
{
public:
	Credit();
	~Credit();
	void addNewEntry(string key, string value);
	string getValue(string key, int index);
	vector<string> getKeys();
	int getNumValuesForAKey(string key);

	vector<string> keys;
	multimap<string, string> creditMap;
};
#endif
