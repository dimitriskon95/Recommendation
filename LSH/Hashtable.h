#ifndef Hashtable_H
#define Hashtable_H

#include "Heuristic.h"
#include "List.h"
#include "Node.h"

template <class T1,class T2>
class HashTable
{
	private:
		int Hashtable_Number;
		char metric;
		int k;			
		int TableSize; 	//number of lists
		int W;  		//for euclidean for the window and for Matrix for the median
		int dimensions; //for euclidean and cosine, for hamming mean the number of bits
		Heuristic<T2> *gfunction;
		List<T1> *list;
	public:
		HashTable();
		~HashTable();
		
		void HashTable_Init(int i, int k, int dims, int tsize, char metric, int w=0);
		
		int Choose_List(Node<T1> *, int ** table = 0);
		void Insert_Node(Node<T1> *, int ** table = 0);
		
		char Hreturn(T1*, int);
		
		void printList();
		
		void HashTable_LSH(Node<T1> *, List<T1> *, int ** table =0);
		
		double Eswteriko_Ginomeno(T1*, T2*);
		
		int get_Hashtable_Number();
		
		int get_k();
		
		void HashTable_Search_All(Node<T1> *, List<T1> *);
		
		void t1_Estimation(Node<T1> *node, int **table, double *type);
		
		void set_Median(int);
};

#endif
