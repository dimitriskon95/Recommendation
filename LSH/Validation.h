#ifndef Validation_H
#define Validation_H

#include "ValidationList.h"
#include "ValidationNode.h"

class Validation
{
	private:
		int J; 			//NumUsers
		int NumLists;
		ValidationList *vlist;
	public:
		Validation(int);
		~Validation();
		
		void InsertNode(int listIndex, ValidationNode *node);
		int get_NumLists();
		int get_J();
		
		ValidationNode *get_List_start(int i);
		
		void printLists();
};	

#endif
