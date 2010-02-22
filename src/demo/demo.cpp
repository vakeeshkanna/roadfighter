//#include<boost/filesystem/operations.hpp>
#include <boost/shared_ptr.hpp>
#include "string-class.h"
#include "log-class.h"
#include "array-class.h"
#include "point-class.h"
#include "linkedlist.h"
#include "timer-class.h"

#include<iostream>

using namespace std;
//namespace bf = boost::filesystem;         //create an alias
using namespace boost;
void doNothing()
{
	Array <String> arr;
	arr[0] = "asdsad";
}
void testPoint()
{
	Point p(1,2,3);
//	Point *point = new Point(4,5,6);
	shared_ptr <Point> point(new Point(4,5,6));
	cout << "p.x=" << p.getX() << ",p.y=" << p.getY() << ",p.z=" << p.getZ()<<endl;
	cout << "p.x=" << point->getX() << ",p.y=" << point->getY() << ",p.z=" << point->getZ()<<endl;
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
	shared_ptr<int> a (new int[2]);
	a.get()[0] = 10;
	a.get()[1] = 20;
	cout<<"a="<<a.get()[0]<<endl;
	cout<<"a="<<a.get()[1]<<endl;
//	bf::path p("first.cpp");
//	if(bf::exists(p))
//		std::cout<<p.leaf()<<" exists.\n";
//	else
//
//		std::cout<<p.leaf()<<" does not exist.\n";
//	          
	testPoint();
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