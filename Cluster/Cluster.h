#ifndef Cluster_H
#define Cluster_H

#include <iostream>
#include "Centroid.h"
#include "List.h"
#include "Node.h"

template <class T>
class Cluster
{
	private:
		int Cluster_ID;
		Centroid<T> *centroid;
		List<T> *list;
	public:
		Cluster();
		~Cluster();
		
		void Cluster_Initialization(int id);
		
		void set_Centroid(Node<T> *);
		Node<T> * get_Centroid();
		
		void Cluster_Assignment(Node<T> *);
		
		List<T> * get_List();
		void set_List_empty();
		void set_Centroid_empty();
		
		void printList();
		void printCentroid();
		
		void Swap(Node<T> * new_centroid);
	
}; 

#endif
