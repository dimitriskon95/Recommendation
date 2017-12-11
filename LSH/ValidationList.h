#ifndef ValidationList_H
#define ValidationList_H

#include <fstream>
#include <iostream>
#include "ValidationNode.h"

class ValidationList
{
	private:
		int list_size;
		ValidationNode *start;
	public:
		ValidationList();
		~ValidationList();
		
		bool is_empty();
		int get_ListSize();
		void Insert_Node(ValidationNode *);
		void Insert_Node_End(ValidationNode *);
		
		ValidationNode *get_start();
		
		void printList();
};

#endif
