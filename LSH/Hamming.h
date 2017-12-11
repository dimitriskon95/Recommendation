#ifndef Hamming_H
#define Hamming_H

#include <fstream>
#include <iostream>
#include "Validation.h"
#include "Hashtable.h"
#include "Node.h"

class Hamming
{
	private:
		int k;
		int L;
		int bsize;
		double R;
		double c;
		int Pnumber;	//plh8os twn kontinoterwn geitonwn pou 8a elegx8oun
		
		Validation *validation;
		List<char> *usersList;
		HashTable<char, int> *hashtable;	
	public:
		Hamming(int, int, int);
		~Hamming();
		
		int bsize_return();
		int get_L();
		
		void set_R(double);
		void set_c(double);

		void printList();
		
		void Hamming_Reader(char *, int, int);
		
		void Hamming_LSH(char *, std::ofstream& output, int, char);
		
		void set_Pnumber(char *line);
		void Hamming_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile);
		char * Hamming_nnLSH(Node<char> *node, std::ofstream& outfile);
		Node<char> *Hamming_close_Neighbor(Node<char> *node, List<char> *ulist);
		int Similarity(Node<char>* user, Node<char>* node);
		
		void Hamming_Validation_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile, int usersNumber);
};

#endif
