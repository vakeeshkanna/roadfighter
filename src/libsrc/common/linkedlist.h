#ifndef LINKEDLISTCLASS_H_
#define LINKEDLISTCLASS_H_

struct linkedlist
{
	int data;
	linkedlist *ptr;
};

class EXPORT LinkedListClass
{
public:
	LinkedListClass();
	~LinkedListClass();
	void insert(int data);
	void remove(int data);
	void traverseList();
	
private:
	linkedlist *start;
};
#endif /*LINKEDLISTCLASS_H_*/
