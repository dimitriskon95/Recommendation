#ifndef Euclidean_H
#define Euclidean_H

#include <fstream>
#include "Hashtable.h"
#include "Validation.h"
#include <iostream>
#include <fstream>

class Euclidean
{
	private:
		int L;
		int k;
		int tSize;
		int dims;
		double R;
		double c;
		int **random;
		int Pnumber;	//plh8os twn kontinoterwn geitonwn pou 8a elegx8oun
		
		Validation *validation;
		List<double> *usersList;
		HashTable<double,double> *hashtable; 
	public:
		Euclidean(int, int, int, int, int**);
		~Euclidean();
		
		void Euclidean_Reader(char *, int, int);
		int get_L();
		
		void set_R(double);
		void set_c(double);

		void printList();
		
		void Euclidean_LSH(char *, std::ofstream& output, int, char);
		
		void set_Pnumber(char *line);
		void Euclidean_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile);
		double * Euclidean_nnLSH(Node<double> *node, std::ofstream& outfile);
		Node<double> * Euclidean_close_Neighbor(Node<double> *node, List<double> *ulist);
		double Similarity(Node<double>* user, Node<double>* node);
		
		void Euclidean_Validation_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile, int usersNumber);
		
		bool Check_Euclidean_Distance(Node<double>* user1, Node<double>* user2, double);
};

#endif
