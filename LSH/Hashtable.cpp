#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>  //rand, RAND_MAX
#include <iomanip>
#include "Hashtable.h"
#include "Heuristic.h"
#include "List.h"
#include "Node.h"
#include "Random.h"

using namespace std;

//Hashtable

template <class T1,class T2>
HashTable<T1,T2>::HashTable()
{
}

template <class T1,class T2>
HashTable<T1,T2>::~HashTable()
{
	delete[] list;
	if (metric == 'h')
		delete gfunction;
	else
		delete[] gfunction;
//	cout << "A Hashtable was destructed" << endl;
}

template <class T1,class T2>
int HashTable<T1,T2>::get_Hashtable_Number()
{
	return Hashtable_Number;
}
template <class T1,class T2>
int HashTable<T1,T2>::get_k()
{
	return k;
}

template <class T1,class T2>
void HashTable<T1,T2>::HashTable_Init(int hash_index, int var_k, int dims, int tsize , char m, int window)    //window is the number of items for distance
{
	Hashtable_Number = hash_index;
	metric = m;
	k = var_k;
	W = window; //used by euclidean or (distance matrix, W = number of items in order to avoid more arguments)
	dimensions = dims;
	TableSize = tsize;
	list = new List<T1>[TableSize];
	if (metric == 'h')
	{
		//cout << "A Hashtable for Hamming was created." << endl;
		gfunction = new Heuristic<T2>;
		gfunction->Heuristic_Init(metric, k);
		gfunction->Hamming_Random(dims);
	}
	else if (metric == 'e')
	{
		//cout << "A Hashtable for Euclidean was constructed" << endl;
		gfunction = new Heuristic<T2>[k];
		for (int i=0; i <= k-1; i++)
		{
			gfunction[i].Heuristic_Init(metric, dims);
			gfunction[i].Euclidean_Random(W);
		}
	}
	else if (metric == 'c')
	{
		//cout << "A Hashtable for Cosine was constructed" << endl;
		gfunction = new Heuristic<T2>[k];
		for (int i=0; i <= k-1; i++)
		{
			gfunction[i].Heuristic_Init(metric, dims);
			gfunction[i].Cosine_Random();
		}
	}
	else if (metric == 'd')
	{
		//cout << "A Hashtable for Distance Matrix was constructed" << endl;
		gfunction = new Heuristic<T2>[k];
		for (int i=0; i <= k-1; i++)
		{
			gfunction[i].Heuristic_Init(metric, 2);		//to save x1 and x2 , 2 vars => k * 2
			gfunction[i].DistanceMatrix_Random(W);
		}
	}
}


template <class T1,class T2>
int HashTable<T1, T2>::Choose_List(Node<T1> *node, int **table)
{
	int list_index = 0;
	//for hamming and cosine
	int exponent = k-1;
	char *array = new char[k];
	//for distance
	int X1, X2;
	double type[k];
	double t1 = 0;
	//for euclidean
	long long int M = pow(2, 32) - 5;
	int f = 0;
	int ID;
	int h[k];
	char c_h[k];
	double calculation;
	if (metric == 'h')
	{
		for (int i=0; i <= k - 1; i++)
			array[i] = Hreturn(node->Data_Return(), gfunction->Hamming_Hreturn(i));
		for (int i=0; i <= k - 1; i++)
		{
			if (array[i] == '1')
				list_index += pow(2, exponent);        // k bits -> integer
			exponent--;
		}
	}
	else if (metric == 'e')
	{
		for (int i=0; i <= k -1; i++)
		{
			calculation = Eswteriko_Ginomeno(node->Data_Return(), gfunction[i].get_hfunctions());
			calculation = calculation + gfunction->get_t();
			calculation = calculation / W;
			h[i] = floor(calculation);
		}
		for (int i=0; i <= k -1; i++)
			f += table[get_Hashtable_Number()][i]*h[i];
		if (f < 0)
			f = f *(-1);
		ID = f % M;
		list_index = ID % TableSize;
	}
	else if (metric == 'c')
	{
		for (int i=0; i <= k -1; i++)
		{
			calculation = Eswteriko_Ginomeno(node->Data_Return(), gfunction[i].get_hfunctions());
			if (calculation >= 0)
				c_h[i] = '1';
			else
				c_h[i] = '0';
		}
		for (int i=0; i <= k - 1; i++)
		{
			if (c_h[i] == '1')
				list_index += pow(2, exponent);        // k bits -> integer
			exponent--;
		}
	}
	else if (metric == 'd')
	{
		for (int i=0; i <= k -1; i++)
		{
			X1 = (gfunction[i].get_hfunctions())[0];
			X2 = (gfunction[i].get_hfunctions())[1];
			type[i] = (double)(pow((node->get_data())[X1], 2) + pow((node->get_data())[X2], 2) - pow(table[X1][X2], 2))/(2*table[X1][X2]);
		}
		
		for (int i=0; i <= k -1; i++)
		{
			if (type[i] >= W)		// W is the median (it is the same for all L Hashtables
				array[i] = '1';
			else
				array[i] = '0';
		}
		
		for (int i=0; i <= k - 1; i++)
		{
			if (array[i] == '1')
				list_index += pow(2, exponent);        // k bits -> integer
			exponent--;
		}
	}
	//memory dismiss
	delete [] array;
	
	return list_index;
}


template <class T1,class T2>
void HashTable<T1, T2>::Insert_Node(Node<T1> *node, int **table)
{
	int list_index = 0;
	list_index = Choose_List(node, table);	
	list[list_index].Insert_Node(node);
}


template <class T1,class T2>
void HashTable<T1, T2>::HashTable_LSH(Node<T1> *node, List<T1> *qlist, int **table)
{	
	int list_index = 0;
	list_index = Choose_List(node, table);
	qlist->set_start(list[list_index].get_start());
}


template <class T1,class T2>
char HashTable<T1, T2>::Hreturn(T1* str, int i)
{
	return str[i];
}


template <class T1,class T2>
void HashTable<T1, T2>::printList()
{
	for (int i=0; i <= TableSize - 1; i++)
	{
		cout << "List " << i << " : ";
		list[i].printList();
		cout << endl;
	}
}

template <class T1,class T2>
double HashTable<T1, T2>::Eswteriko_Ginomeno(T1 *p, T2 *vector)
{
	//cout << "eswteriko Ginomeno" << endl;
	double sum = 0;
	for(int i=0; i<= dimensions-1; i++)
	{
		sum += p[i]*vector[i];
	}
	return sum;
}

template <class T1,class T2>
void HashTable<T1, T2>::HashTable_Search_All(Node<T1> *node, List<T1> *flist)
{
	for(int i=0; i <= TableSize - 1; i++)
	{
		if (!list[i].is_empty())
			flist->Insert_List(&list[i], 1000);
	}
}

template <class T1,class T2>
void HashTable<T1, T2>::t1_Estimation(Node<T1> *node, int **table, double *type)
{
	int X1, X2;
	for (int i = 0; i <= k -1; i++)
	{
		X1 = (gfunction[i].get_hfunctions())[0];
		X2 = (gfunction[i].get_hfunctions())[1];
		type[i] = (double)(pow((node->get_data())[X1], 2) + pow((node->get_data())[X2], 2) - pow(table[X1][X2], 2))/(2*table[X1][X2]);
	}
}

template <class T1,class T2>
void HashTable<T1, T2>::set_Median(int median)
{
	W = median;
}


template class HashTable<char, int>;
template class HashTable<double, double>;
template class HashTable<int, int>;


/*		
		int length = k - 1;
		int end = k - 1;
		double temp;
		double *myarray = new double[k];
		
		for (int i=0; i <= k -1; i++)
			myarray[i] = type[i];
		
		for(int count = k ; count > 0; count--)
		{	
			for (int index =0; index < end; index++)
			{
				if (myarray[index] > myarray[index+1])
				{
					temp = myarray[index+1];
					myarray[index+1] = myarray[index];
					myarray[index] = temp;
				}
			}
			end--;
		}
		
		for (int i=0; i <= k -1; i++)
			cout << myarray[i] << " ";
		
		t1 = myarray[(k-1)/2];
*/

