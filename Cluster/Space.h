#ifndef Space_H
#define Space_H

#include <fstream>
#include <sstream>
#include <string.h>
#include <iostream>
#include "ClusterHamming.h"
#include "ClusterEuclidean.h"
#include "ClusterCosine.h"

class Space
{
	private:
		char metric_space;
		int item_dimensions;
		int cluster_number;		//initialised by constructor
		
		int hash_functions;
		int hash_tables;
		int clarans_franction;
		int clarans_iterations;
		
		ClusterHamming *c_hamming;
		ClusterEuclidean *c_euclidean;
		ClusterCosine *c_cosine;
	public:
		Space(char metric, int items);
		~Space();
		
		void Space_Menu(std::ifstream& infile1, std::ifstream& infile2, std::ifstream& qfile, std::ofstream& outfile, bool validate);
		int Space_Get_TableSize(std::ifstream& infile);
}; 

#endif
