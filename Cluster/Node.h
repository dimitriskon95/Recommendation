#ifndef Node_H
#define Node_H

template <class T>
class Centroid;

template <class T>
class Node
{
	private:
		int number;
		int number_in_table;
		double average;
		int size;
		bool is_centroid;
		bool LSH_is_assigned;
		T *data;
		Node *next;
		//Centroid<T> *Second_best;
	public:
		Node(T*, int, int, int, double);
		~Node();
		
		T* Data_Return();
		
		void set_next(Node<T> *);
		Node<T>* get_next();
		int get_size();
		T* get_data();
		void set_data(int, char);
		int get_Number();
		int get_Number_Table();
		double get_Average();
		
		void set_Number_Table(int);
		
		void set_CentroidValue();
		bool get_CentroidValue();
		
		void set_notCentroidValue();
		
		void printNode();
};	

#endif
