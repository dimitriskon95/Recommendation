#ifndef Cosine_H
#define Cosine_H

#include <fstream>
#include "Hashtable.h"
#include "Validation.h"
#include <iostream>
#include <fstream>

class Cosine
{
	private:
		int L;
		int k;
		int dims;
		double R;
		double c;
		int Pnumber;	//plh8os twn kontinoterwn geitonwn pou 8a elegx8oun
		
		Validation *validation;
		List<double> *usersList;
		HashTable<double,double> *hashtable; 
	public:
		Cosine(int, int, int);
		~Cosine();
		
		void Cosine_Reader(char *, int, int);
		int get_L();
		
		void set_R(double);
		void set_c(double);

		void printList();
		
		void Cosine_LSH(char *, std::ofstream& output, int, char);
		
		void set_Pnumber(char *line);
		void Cosine_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile);
		double * Cosine_nnLSH(Node<double> *node, std::ofstream& outfile);
		Node<double> * Cosine_close_Neighbor(Node<double> *node, List<double> *ulist);
		double Similarity(Node<double>* user, Node<double>* node);
		
		void Cosine_Validation_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile, int usersNumber);
};

#endif
