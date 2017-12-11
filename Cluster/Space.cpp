#include <fstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "Space.h"

using namespace std;

Space::Space(char metric, int items)
{
	cout << "A Space was constructed." << endl;
	cluster_number = 5;	//default
	metric_space = metric;
	item_dimensions = items;
	
	//Not used
	hash_functions = 4;
	hash_tables = 5;
	clarans_franction = 250;
	clarans_iterations = 2;
	
	c_hamming = NULL;
	c_euclidean = NULL;
	c_cosine = NULL;
}

Space::~Space()
{
	if (metric_space == 'h' && c_hamming != NULL)
		delete c_hamming;
	else if (metric_space == 'e' && c_euclidean != NULL)
		delete c_euclidean;
	else if (metric_space == 'c' && c_cosine != NULL)
		delete c_cosine;
	cout << "A Space was destructed." << endl;
}

void Space::Space_Menu(std::ifstream& infile1, std::ifstream& infile2, std::ifstream& qfile, std::ofstream& outfile, bool validate)
{
	int tSize = Space_Get_TableSize(qfile);		//Find the size of buckets for euclidean hashtables	
	
	if (metric_space == 'h')
	{
		c_hamming = new ClusterHamming(cluster_number, item_dimensions, hash_functions, hash_tables, clarans_franction, clarans_iterations);			//Construct all classes that we need for the chosen metric
		if (validate == false)
			c_hamming->ClusterHamming_Menu(infile1, infile2, outfile);
		else
			c_hamming->Hamming_Validation_Menu(infile1, qfile, outfile, tSize);
	}
	else if (metric_space == 'e')
	{
		c_euclidean = new ClusterEuclidean(cluster_number, item_dimensions, hash_functions, hash_tables, clarans_franction, clarans_iterations);		//Construct all classes that we need for the chosen metric
		if (validate == false)
			c_euclidean->ClusterEuclidean_Menu(infile1, infile2, outfile);
		else
			c_euclidean->Euclidean_Validation_Menu(infile1, infile2, outfile, tSize);			
	}
	else if (metric_space == 'c')
	{
		c_cosine = new ClusterCosine(cluster_number, item_dimensions, hash_functions, hash_tables, clarans_franction, clarans_iterations);				//Construct all classes that we need for the chosen metric
		if (validate == false)
			c_cosine->ClusterCosine_Menu(infile1, infile2, outfile);
		else
			c_cosine->Cosine_Validation_Menu(infile1, qfile, outfile, tSize);
	}
}


int Space::Space_Get_TableSize(ifstream& infile)			//Estimate metric and dimensions for items
{
	string line;
	int TableSize = 0, userID, user_count, line_count, tmp_userID =0;
	char *user = new char[10];
	getline(infile, line);
	while(getline(infile, line))
	{
		user_count = 0;
		line_count = 0;
		while(line[line_count] != '\t')
		{
			user[user_count] = line[line_count];
			user_count++;
			line_count++;
		}
		user[user_count] = '\0';
		userID = atoi(user);
		if (userID != tmp_userID)
		{
			TableSize++;
			tmp_userID = userID;	
		}
	}
	delete [] user;
	return TableSize;
}


