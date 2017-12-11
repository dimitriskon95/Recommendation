#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include "Heuristic.h"
#include "Random.h"

using namespace std;


//---------------------------Heuristic-----------------------

template <class T>
Heuristic<T>::Heuristic()
{
	
}

template <class T>
Heuristic<T>::~Heuristic()
{
	delete [] hfunctions;
	//cout << "A Heuristic was deleted" << endl;
}

template <class T>
void Heuristic<T>::Heuristic_Init(char m, int dimensions)
{
//	cout << "A new h was constructed" << endl;
	metric = m;
	dims = dimensions;
	hfunctions = new T[dims];
	t = 0;
}


template <class T>
void Heuristic<T>::Hamming_Random(int bsize)
{
	for (int i=0; i <= dims - 1; i++)   //dims = k only for hamming
		hfunctions[i] = rand() % (bsize - 1);
}


template <class T>
void Heuristic<T>::Euclidean_Random(int w)
{
	for (int i = 0; i <=dims -1; i++)
	{
		hfunctions[i] = uniform_distribution_Marsaglia(-1, 1);
	}
	t = uniform_distribution_real(0, w);
}

template <class T>
void Heuristic<T>::Cosine_Random()
{
	for (int i = 0; i <=dims -1; i++)
		hfunctions[i] = uniform_distribution_Marsaglia(-1, 1);
}

template <class T>
void Heuristic<T>::DistanceMatrix_Random(int nItems)
{
	for (int i = 0; i <= dims/2 - 1; i++)
	{
		hfunctions[i] = uniform_distribution(1, nItems);
		do
		{
			hfunctions[dims/2 + i] = uniform_distribution(1, nItems);
		}while(hfunctions[i] == hfunctions[dims/2 + i]);
	}
}

template <class T>
T Heuristic<T>::Hamming_Hreturn(int i)
{
	return hfunctions[i];
}

template <class T>
T Heuristic<T>::get_t()
{
	return t;
}

template <class T>
T* Heuristic<T>::get_hfunctions()
{
	return hfunctions;
}
/*
template <class T>
void Heuristic<T>::set_median(double median)
{
	t = median;
}
*/

template class Heuristic<int>;
template class Heuristic<double>;
