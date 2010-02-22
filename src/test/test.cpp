#include "string-class.h"
#include "array-class.h"
#include "log-class.h"
#include <iostream.h>
int main()
{
	String s = "abc";
	cout<<s.str()<<endl;
	cout<<"Hello World!"<<endl;
	Log l;
	for(int i = 0; i < 100; i++)
	l.write("Strings are cool\n");
	return 0;
}