#ifndef Centroid_H
#define Centroid_H

#include <iostream>
#include "Node.h"

template <class T>
class Centroid
{
	private:
		Node<T> *center;
	public:
		Centroid();
		~Centroid();
		
		void set_CNode(Node<T> *);
		Node<T>* get_center();
		
		void printCentroid();
	
}; 

#endif
