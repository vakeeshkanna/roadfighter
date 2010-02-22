#include "string-class.h"
#include "log-class.h"
#include "array-class.h"
#include "point-class.h"
#include "linkedlist.h"
#include "timer-class.h"

#include<iostream>

using namespace std;
//namespace bf = boost::filesystem;         //create an alias
void doNothing()
{
	Array <String> arr;
	arr[0] = "asdsad";
}

int main()
{
	LinkedListClass *myList = new LinkedListClass();
	//myList->insert(10);
	//myList->insert(20);
	//myList->insert(30);
	myList->traverseList();
	myList->remove(20);
	myList->traverseList();
	myList->remove(20);
	myList->remove(40);
	myList->remove(10);
	//myList->remove(30);
	myList->traverseList();
	delete myList;
//	if(bf::exists(p))
//		std::cout<<p.leaf()<<" exists.\n";
//	else
//
//		std::cout<<p.leaf()<<" does not exist.\n";
//	          
	String s = "abc";
	String r = "efg";
		
	s = s + "d";
	s = s + r;
	s.toUpper();
	cout << s.str()<<endl;
	doNothing();
	
	//Testing timer
	Timer t;
	t.forceLowResTimer();
	t.start();
	Sleep(1000);
	Sleep(5);
	cout<<"timeelapsed "<<(int)t.getTicks()<<endl;

	
	//lprintf("Hello World on Linux\n");
	return 0;
}