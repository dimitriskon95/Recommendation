#include <fstream>
#include <iostream>
#include <math.h>
#include <string.h>
#include "List.h"

using namespace std;

template <class T>
List<T>::List()
{
//	cout << "A list was created." << endl;
	list_size = 0;
	start = NULL;
}

template <class T>
List<T>::~List()
{
	Node<T>* current = start;
	Node<T>* del_node;
	
    while(current != NULL)  //diagrafoume enan enan tous komvous
    {
        del_node = current;
        current = current->get_next();
        list_size--;
        delete del_node;
    }
//  cout << "A list was deleted" << endl;
}

template <class T>
bool List<T>::is_empty()
{
	return (list_size == 0);
}

template <class T>
int List<T>::get_ListSize()
{
	return list_size;
}

template <class T>
void List<T>::Insert_Node(Node<T> *node)
{
	//insert node at the beginnning of the list
	//cout << "A node was inserted to a list " << endl;
	Node<T>* current = start;
	if (is_empty()){
		start = node;
	}
	else{
		node->set_next(start);
		start = node;
	}
	list_size++;
}


template <class T>
void List<T>::Insert_Node_End(Node<T> *node)
{
	//insert node at the beginnning of the list
	//cout << "A node was inserted to a list " << endl;
	Node<T>* current = start;
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

template <class T>
void List<T>::Clear_List()
{
	Node<T>* current = start;
	Node<T>* del_node;
	
    while(current != NULL)  //diagrafoume enan enan tous komvous
    {
        del_node = current;
        current = current->get_next();
        delete del_node;
    }
    
    list_size = 0;
    start = NULL;
}

template <class T>
void List<T>::printList()
{
	Node<T>* current = start;
	
	if (current == NULL)
		cout << "problem\n";
	
    while(current != NULL) 
    {
    	if (current->get_CentroidValue())
    		cout << "Centroid ";
        current->printNode();
        current = current->get_next();
    }
    cout << endl;
}

template <class T>
Node<T> *List<T>::get_start()
{
	return start;
}

template <class T>
void List<T>::set_start(Node<T> *s)
{
	start = s;
}

template <class T>
void List<T>::set_List_size(int num)
{
	list_size = num;
}

template <class T>
Node<T> *List<T>::get_random_Node(int Number)
{
	int count=0;
	Node<T>* current = start;
	
    while(current != NULL) 
    {
    	if (count == Number)
    	{
    		return current;
		}
        current = current->get_next();
        count++;
    }
	return NULL;
}

template <class T>
Node<T> *List<T>::get_Node(int Number)
{
	Node<T>* current = start;
	
    while(current != NULL) 
    {
    	if (current->get_Number() == Number)
    	{
    		return current;
		}
        current = current->get_next();
    }
	return NULL;
}



template <class T>
Node<T> *List<T>::get_Node_Table_Number(int Number)
{
	Node<T>* current = start;
	
    while(current != NULL) 
    {
    	if (current->get_Number_Table() == Number)
    	{
    		return current;
		}
        current = current->get_next();
    }
	return NULL;
}


template <class T>
void List<T>::Insert_List(List<T> *qlist, int N_Items)
{
	Node<T>* current = qlist->get_start();
	Node<T>* new_node;
	T* new_data;
	int count = 0;
	
	while(current != NULL && count < N_Items)
	{
		new_data = new T[current->get_size()];
		for(int i=0; i <= current->get_size() -1; i++)
			new_data[i] = current->get_data()[i];
		new_node = new Node<T>(new_data, current->get_size(), current->get_Number(), current->get_Number_Table(), current->get_Average());
		if (Check(new_node))
		{
			Insert_Node(new_node);
			count++;
		}
		else
		{
			delete new_node;
		}	
		current = current->get_next();
	}
}


template <class T>
void List<T>::Insert_List(List<T> *qlist)
{
	Node<T>* current = qlist->get_start();
	Node<T>* new_node;
	T* new_data;
	int count = 0;
	
	while(current != NULL)
	{
		new_data = new T[current->get_size()];
		for(int i=0; i <= current->get_size() -1; i++)
			new_data[i] = current->get_data()[i];
		new_node = new Node<T>(new_data, current->get_size(), current->get_Number(), current->get_Number_Table(), current->get_Average());
		if (Check(new_node))
		{
			Insert_Node(new_node);
			count++;
		}
		else
		{
			delete new_node;
		}	
		current = current->get_next();
	}
}


template <class T>
void List<T>::Insert_Cluster_List(List<T> *clist)
{
	Node<T> *current = start;
	if (start == NULL)
	{
		start = clist->get_start();
		list_size = clist->get_ListSize();
	}
	else
	{
		while(current->get_next() != NULL)
		{	
			current = current->get_next();
		}
		current->set_next(clist->get_start());
		list_size += clist->get_ListSize();
	}
}


template <class T>
bool List<T>::Check(Node<T>* node)
{
	Node<T> *current = start;
	while(current != NULL)
	{
		if (node->get_Number() == current->get_Number())
		{
			return false;
		}
		current = current->get_next();
	}
	return true;
}


template <class T>
int List<T>::Hamming_Distance(Node<T>* query, double R, double c, ofstream& output, int tag)
{
	Node<T> *current = start;
	Node<T> *closer_neighbor = start;
	int dist = 0;
	int min = 1000;				//A very big value
	if (tag == 0)
		output << "R-near neighbors:\n";
	if (R != 0)
	{
		while(current != NULL)
		{
			dist = 0;
			for(int i=0; i<= query->get_size()-1; i++)
			{
				if (current->get_data()[i] != query->get_data()[i])
					dist++;
			}
			if (dist <= R*c)
			{
				if (tag == 0){
					output << "Item";
					output << current->get_Number();
					output << '\n';	
				}
			}
			current = current->get_next();
		}
	}
	current = start;
	while(current != NULL)
	{
		dist = 0;
		for(int i=0; i<= query->get_size()-1; i++)
		{
			if (current->get_data()[i] != query->get_data()[i])
				dist++;
		}
		if (dist < min) //to avoid itself use (... && dist > 0)
		{
			min = dist;
			closer_neighbor = current;
		}
		current = current->get_next();
	}
	if (tag == 0){
		if (closer_neighbor != NULL)
		{
			cout << "Item" << closer_neighbor->get_Number() << '\t';
			//closer_neighbor->printNode();
			cout << " " << min << endl;
			output << "Nearest neighbor: ";
			output << closer_neighbor->get_Number();
			output << '\n';
		}
		else
		{
			output << "There is no nearest neighbor ";
		}	
	}
	return min;
}


template <class T>
double List<T>::Euclidean_Distance(Node<T>* query, double R, double c, ofstream& output, int tag)
{
	Node<T> *current = start;
	Node<T> *closer_neighbor = NULL;
	double dist = 0;
	double sum;
	double min = 1000;
	if (tag == 0)
		output << "R-near neighbors:\n";
	if (R != 0)
	{
		while(current != NULL)
		{
			dist = 0;
			sum = 0;
			for(int i=0; i<= query->get_size()-1; i++)
			{
				sum += pow(current->get_data()[i] - query->get_data()[i], 2);
			}
			dist = sqrt(sum);
			if (dist <= R*c)
			{
				if (tag == 0){
					output << "Item";
					output << current->get_Number();
					output << '\n';	
				}
			}
			current = current->get_next();
		}
	}
	current = start;
	while(current != NULL)
	{
		dist = 0;
		sum = 0;
		for(int i=0; i<= query->get_size()-1; i++)
		{
			sum += pow(current->get_data()[i] - query->get_data()[i], 2);
		}
		dist = sqrt(sum);
		if (dist < min) //to avoid itself use (... && dist > 0)
		{
			min = dist;
			closer_neighbor = current;
		}
		current = current->get_next();
	}
	if (tag == 0){
		if (closer_neighbor != NULL)
		{
			cout << "Item" << closer_neighbor->get_Number() << '\t';
			cout << " " << min << endl;
			output << "Nearest neighbor: ";
			output << closer_neighbor->get_Number();
			output << '\n';
		}
		else
		{
			output << "There is no nearest neighbor ";
		}
	}
	return min;
}


template <class T>
double List<T>::Cosine_Distance(Node<T>* query, double R, double c, ofstream& output, int tag)
{
	Node<T> *current = start;
	Node<T> *closer_neighbor = start;
	double dist = 0;
	double sum;
	double normX;
	double normY;
	double min = 1000;
	if (tag == 0)
		output << "R-near neighbors:\n";
	if (R != 0)
	{
		while(current != NULL)
		{
			dist = 0;
			sum = 0;
			normX = 0;
			normY = 0;
			for(int i=0; i<= query->get_size()-1; i++)
			{
				sum += (current->get_data()[i])*(query->get_data()[i]);
			}
			for(int i=0; i<= current->get_size()-1; i++)
			{
				normX += pow(current->get_data()[i], 2);
			}
			normX = sqrt(normX);
			for(int i=0; i<= query->get_size()-1; i++)
			{
				normY += pow(query->get_data()[i], 2);
			}
			normY = sqrt(normY);
			
			dist = 1 - (double)(sum /(normX*normY));			
			if (dist <= R*c)
			{
				if (tag == 0){
					output << "Item";
					output << current->get_Number();
					output << '\n';	
				}
			}
			current = current->get_next();
		}
	}
	current = start;
	while(current != NULL)
	{
		dist = 0;
		sum = 0;
		normX = 0;
		normY = 0;
		for(int i=0; i<= query->get_size()-1; i++)
		{
			sum += (current->get_data()[i])*(query->get_data()[i]);
		}
		for(int i=0; i<= current->get_size()-1; i++)
		{
			normX += pow(current->get_data()[i], 2);
		}
		normX = sqrt(normX);
		for(int i=0; i<= query->get_size()-1; i++)
		{
			normY += pow(query->get_data()[i], 2);
		}
		normY = sqrt(normY);
		
		dist = 1 - (double)sum / (normX*normY);
		if (dist < min)
		{
			min = dist;
			closer_neighbor = current;
		}
		current = current->get_next();
	}
	if (tag == 0){
		if (closer_neighbor != NULL)
		{
			cout << "Item" << closer_neighbor->get_Number() << '\t';
			cout << " " << min << endl;
			output << "Nearest neighbor: ";
			output << closer_neighbor->get_Number();
			output << '\n';
		}
		else
		{
			output << "There is no nearest neighbor ";
		}
	}
	return min;
}


template <class T>
int List<T>::DistanceMatrix_Distance(Node<T>* query, double R, double c, ofstream& output, int tag)
{
	Node<T> *current = start;
	Node<T> *closer_neighbor = start;
	int min = 1000;
	if (tag == 0)
		output << "R-near neighbors:\n";
	if (R != 0 )
	{
		while(current != NULL)
		{
			if ((query->get_data()[current->get_Number()-1]) <= R*c && (query->get_data()[current->get_Number()-1]) > 0)
			{
				if (tag == 0){
					output << "Item";
					output << current->get_Number();
					output << '\n';	
				}
			}
			current = current->get_next();
		}
	}
	//Closer Neighbor
	current = start;
	while(current != NULL)
	{
		if ((query->get_data()[current->get_Number()-1]) < min && (query->get_data()[current->get_Number()-1]) > 0)  //expect itself
		{
			min = query->get_data()[current->get_Number()-1];
			closer_neighbor = current;
		}
		current = current->get_next();
	}
	
	if (tag == 0){
		if (closer_neighbor != NULL)
		{
			cout << "Item" << closer_neighbor->get_Number() << '\t';
			cout << " " << min << endl;
			output << "Nearest neighbor: ";
			output << closer_neighbor->get_Number();
			output << '\n';
		}
		else
		{
			output << "There is no nearest neighbor ";
		}
	}
	return min;
}

template class List<int>;
template class List<char>;
template class List<double>;
