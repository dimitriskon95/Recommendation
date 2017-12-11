#include <fstream>
#include <iostream>
#include <math.h>
#include <string.h>
#include "ValidationList.h"

using namespace std;

ValidationList::ValidationList()
{
	//cout << "A Validation List was created." << endl;
	list_size = 0;
	start = NULL;
}

ValidationList::~ValidationList()
{
	ValidationNode* current = start;
	ValidationNode* del_node;
	
    while(current != NULL)  //diagrafoume enan enan tous komvous
    {
        del_node = current;
        current = current->get_next();
        list_size--;
        delete del_node;
    }
//  cout << "A list was deleted" << endl;
}


bool ValidationList::is_empty()
{
	return (list_size == 0);
}


int ValidationList::get_ListSize()
{
	return list_size;
}


void ValidationList::Insert_Node(ValidationNode *node)
{
	//insert node at the beginnning of the list
	//cout << "A node was inserted to a list " << endl;
	ValidationNode* current = start;
	if (is_empty()){
		start = node;
	}
	else{
		node->set_next(start);
		start = node;
	}
	list_size++;
}


void ValidationList::Insert_Node_End(ValidationNode *node)
{
	//insert node at the beginnning of the list
	//cout << "A node was inserted to a list " << endl;
	ValidationNode* current = start;
	if (is_empty()){
		start = node;
	}
	else{
		while (current->get_next() != NULL)
			current = current->get_next();
		current->set_next(node);
	}
	list_size++;
}

void ValidationList::printList()
{
	ValidationNode* current = start;
	
    while(current != NULL) 
    {
        current->printNode();
        current = current->get_next();
    }
    cout << endl;
}

ValidationNode* ValidationList::get_start()
{
	return start;
}



