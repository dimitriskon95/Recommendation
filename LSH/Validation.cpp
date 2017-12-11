#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>  //rand, RAND_MAX
#include <iomanip>
#include "Validation.h"
#include "ValidationList.h"
#include "ValidationNode.h"

using namespace std;

Validation::Validation(int users)
{
	cout << "A Validation class was constructed" << endl;
	J = users;
	NumLists = 10;
	vlist = new ValidationList[10];
}

Validation::~Validation()
{
	delete [] vlist;
}		

void Validation::InsertNode(int listIndex, ValidationNode *node)
{
	vlist[listIndex].Insert_Node_End(node);
}

int Validation::get_NumLists()
{
	return NumLists;
}

int Validation::get_J()
{
	return J;
}

void Validation::printLists()
{
	for(int i=0; i < NumLists; i++)
	{
		cout << "List " << i << " :" << endl;
		vlist[i].printList();
	}
}

ValidationNode *Validation::get_List_start(int i)
{
	return vlist[i].get_start();
}
