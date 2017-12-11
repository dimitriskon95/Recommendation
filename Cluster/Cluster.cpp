#include <fstream>
#include <iostream>
#include <math.h>
#include <string.h>
#include "Cluster.h"
#include "Node.h"

using namespace std;

template <class T>
Cluster<T>::Cluster()
{
//	cout << "A cluster was constructed." << endl;
	centroid = new Centroid<T>;
	list = new List<T>;
}

template <class T>
Cluster<T>::~Cluster()
{
//	delete centroid;
//	delete list;
//	cout << "A cluster was destructed." << endl;
}

template <class T>
void Cluster<T>::Cluster_Initialization(int id)
{
	Cluster_ID = id;
}

template <class T>
void Cluster<T>::set_Centroid(Node<T> *node)
{
	centroid->set_CNode(node);
}

template <class T>
Node<T>* Cluster<T>::get_Centroid()
{
	return (centroid->get_center());
}

template <class T>
void Cluster<T>::Cluster_Assignment(Node<T> *node)
{
	list->Insert_Node_End(node);
}

template <class T>
List<T> * Cluster<T>::get_List()
{
	return list;
}

template <class T>
void Cluster<T>::set_List_empty()
{
	list->set_start(NULL);
	list->set_List_size(0);
}

template <class T>
void Cluster<T>::set_Centroid_empty()
{
	centroid->set_CNode(NULL);
}

template <class T>
void Cluster<T>::printCentroid()
{
	centroid->printCentroid();
}

template <class T>
void Cluster<T>::printList()
{
	list->printList();
}


template <class T>
void Cluster<T>::Swap(Node<T> * new_centroid)
{
	Node<T> *previous_centroid = this->get_Centroid();
	Node<T> *current_node = list->get_start();
	Node<T> *previous_node = list->get_start();
	
	while(current_node != NULL)
	{
		//find t in list
		if (current_node == new_centroid)
		{	
			if (previous_node != current_node)
			{
				previous_node->set_next(current_node->get_next());		//Delete it from list
			}
			else
			{
				list->set_start(current_node->get_next());				//Delete it from list
			}
			list->set_List_size(list->get_ListSize() - 1);
			break;
		}
		previous_node = current_node;
		current_node = current_node->get_next();
	}
	
	//put past_centroid at the end of the list
	previous_centroid->set_notCentroidValue();
	list->Insert_Node_End(previous_centroid);
	
	//set t as a centroid
	new_centroid->set_next(NULL);
	centroid->set_CNode(new_centroid);
}



template class Cluster<int>;
template class Cluster<char>;
template class Cluster<double>;
