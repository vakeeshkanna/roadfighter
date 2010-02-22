#include "linkedlist.h"

using namespace std;
LinkedListClass::LinkedListClass()
{
	start = NULL;
}

LinkedListClass::~LinkedListClass()
{
	linkedlist *current = start;
	linkedlist *currentItemToBeRemoved;
	while(current != NULL)
	{
		currentItemToBeRemoved = current;
		cout<<"Deleting item "<<currentItemToBeRemoved->data<<endl;
		current = current->ptr;
		delete currentItemToBeRemoved;
	}
}

void LinkedListClass::insert(int data)
{
	if(start == NULL)
	{
		start = new linkedlist();
		start->data = data;
		start->ptr = NULL;
	}
	else
	{
		linkedlist *current = start;
		while(current->ptr != NULL)
		{
			current = current->ptr;
		}
		linkedlist *newItem = new linkedlist();
		newItem->data = data;
		newItem->ptr = NULL;
		current->ptr = newItem;	
	}
}

void LinkedListClass::remove(int data)
{
	linkedlist *current,*itemToBeRemoved;
	if(start == NULL)
	{
		cout<<"Cannot remove item as list is currently empty!"<<endl;
		return;
	}
	current = start;
	while(current != NULL)
	{
		if(current->data ==  data)
			break;
		current = current->ptr;
	}
	if(current == NULL)
	{
		cout<<data<<" not found!"<<endl;
		return;
	}
	current = start;
	//if first item in the list
	if(current->data == data)
	{
		itemToBeRemoved = current;
		start = current->ptr;
		delete itemToBeRemoved;
		cout<<data<<" removed"<<endl;
	}
	else
	{	
		while(current != NULL)
		{
			if(current->ptr->data == data)
			{
				itemToBeRemoved = current->ptr;
				current->ptr = itemToBeRemoved->ptr;
				delete itemToBeRemoved;
				cout<<data<<" removed"<<endl;
				break;
			}
			else
				current = current->ptr;
		}
	}
}

void LinkedListClass::traverseList()
{
	int i = 0;
	if(start != NULL)
	{
		cout<<"Item "<<i++<<": "<<start->data<<endl; 
		linkedlist *current = start->ptr;
		while(current != NULL)
		{
			cout<<"Item "<<i++<<": "<<current->data<<endl; 
			current = current->ptr;
		}
	}
	else
	{
		cout<<"linked list is empty!"<<endl;
	}
}

