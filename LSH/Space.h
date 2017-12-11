#ifndef Space_H
#define Space_H

#include <fstream>
#include <sstream>
#include <string.h>
#include <iostream>
#include "Hamming.h"
#include "Euclidean.h"
#include "Cosine.h"

class Space
{
	public:
		char metric_space;
		int item_dimensions;
		
		int hash_functions;
		int hash_tables;
		
		Hamming *hamming;
		Euclidean *euclidean;
		Cosine *cosine;
	public:
		Space(char metric, int items);
		~Space();
		
		void Space_Menu(std::ifstream& infile1, std::ifstream& infile2, std::ifstream& qfile, std::ofstream& outfile, bool validate);
		int Space_Get_Euclidean_TableSize(std::ifstream& infile);
}; 

#endif
