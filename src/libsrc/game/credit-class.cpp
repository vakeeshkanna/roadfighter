#include "credit-class.h"

Credit::Credit()
{
	creditMap.clear();
}

Credit::~Credit()
{

}

void Credit::addNewEntry(string key, string value)
{
	creditMap.insert(pair<string, string>(key, value));
}

string Credit::getValue(string key, int index)
{
	int count = 0;

	if(!creditMap.empty())
	{
		pair<multimap<string, string>::iterator, multimap<string, string>::iterator> ppp;

		// equal_range(key) returns pair<iterator,iterator> representing the range of element with key
		ppp = creditMap.equal_range(key);

		// Loop through range of maps of key
		for (multimap<string, string>::iterator it2 = ppp.first; it2 != ppp.second; ++it2)
		{
			if(index == count)
			{
				return (*it2).second;
			}
			count++;
		}
	}

	return string("");
}

int Credit::getNumValuesForAKey(string key)
{
	return creditMap.count(key);
}
