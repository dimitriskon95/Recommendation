#include <fstream>
#include <iostream>
#include <math.h>
#include <string.h>
#include "Centroid.h"
#include "Node.h"

using namespace std;

template <class T>
Centroid<T>::Centroid()
{
//	cout << "A Centroid was constructed." << endl;
}

template <class T>
Centroid<T>::~Centroid()
{
	//cout << "A Centroid was destructed." << endl;
}

template <class T>
void Centroid<T>::set_CNode(Node<T> *node)
{
	center = node;
	center->set_CentroidValue();
}

template <class T>
Node<T>* Centroid<T>::get_center()
{
	return center;
}

template <class T>
void Centroid<T>::printCentroid()
{
	center->printNode();
}


template class Centroid<int>;
template class Centroid<char>;
template class Centroid<double>;


