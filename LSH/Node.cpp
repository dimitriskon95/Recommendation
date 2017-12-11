#include <iostream>
#include "Node.h"
#include <iomanip>

using namespace std;

template <class T>
Node<T>::Node(T* Dt, int s, int N, int Ntable, double avg)
{
	//cout << "A Node was created." << endl;
	number = N;
	number_in_table = Ntable;
	data = Dt;
	average = avg;
	size = s; //xreiazetai mono sthn cout opote mallon 8a afaire8ei
	is_centroid = false;
	LSH_is_assigned = false;
	next = NULL;
}

template <class T>
Node<T>::~Node()
{
	delete data;
}

template <class T>
T* Node<T>::Data_Return()
{
	return data;
}

template <class T>
void Node<T>::set_next(Node<T> *node)
{
	next = node;
}

template <class T>
Node<T>* Node<T>::get_next()
{
	return next;
}

template <class T>
int Node<T>::get_size()
{
	return size;
}

template <class T>
T* Node<T>::get_data()
{
	return data;
}

template <class T>
void Node<T>::set_data(int i, char value)
{
	data[i] = value;
}

template <class T>
double Node<T>::get_Average()
{
	return average;
}

template <class T>
void Node<T>::set_Number_Table(int i)
{
	number_in_table = i;
}

template <class T>
void Node<T>::set_CentroidValue()
{
	is_centroid = true;
}

template <class T>
bool Node<T>::get_CentroidValue()
{
	return is_centroid;
}


template <class T>
void Node<T>::set_notCentroidValue()
{
	is_centroid = false;
}

template <class T>
void Node<T>::printNode()
{
	cout << number << '\t' ;//<< data << endl;
//	for(int i=0; i <= size-1; i++)
//		cout << setprecision(12) << data[i] << '\t';
//	cout << endl;
}

template <class T>
int Node<T>::get_Number()
{
	return number;
}

template <class T>
int Node<T>::get_Number_Table()
{
	return number_in_table;
}
		

template class Node<int>;
template class Node<char>;
template class Node<double>;
