#include <fstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "Space.h"
#include "Random.h"
#define E_SIZE 8

using namespace std;

Space::Space(char metric, int items)
{
	cout << "A Space was constructed." << endl;
	metric_space = metric;
	item_dimensions = items;
	hamming = NULL;
	euclidean = NULL;
	cosine = NULL;
}

Space::~Space()
{
	if (metric_space == 'h' && hamming != NULL)
		delete hamming;
	else if (metric_space == 'e' && euclidean != NULL)
		delete euclidean;
	else if (metric_space == 'c' && cosine != NULL)
		delete cosine;
	cout << "A Space was destructed." << endl;
}


void Space::Space_Menu(ifstream& infile1, ifstream& infile2, ifstream& qfile, ofstream& outfile, bool validate)
{
	int tSize;
	int **random_array;
	
	hash_functions = 4;			//by default
	hash_tables = 5;			//by default
	
	//metric and dimension cant be calculated by functions (not in the files) .They are given as default values
	if (metric_space == 'h')
	{
		cout << "Metric space is Hamming." << endl;
		tSize = Space_Get_Euclidean_TableSize(infile2);		//Find the size of buckets for euclidean hashtables	
	}
	else if (metric_space == 'e')
	{
		cout << "Metric space is Euclidean." << endl;
		tSize = Space_Get_Euclidean_TableSize(infile2);		//Find the size of buckets for euclidean hashtables	
		
		random_array = new int*[hash_tables];				// for euclidean array = r (random)   , it s destructed in Euclidean
		for (int i = 0; i <= hash_tables - 1; i++)
			random_array[i] = new int[hash_functions];
			
		//Create an array with random values 
		for (int i = 0; i <= hash_tables - 1; i++)
			for (int j = 0; j <= hash_functions - 1; j++)
				random_array[i][j] = uniform_distribution(-32767, 32767);
				
	}
	else{																//in case metric_space != h , metric_space != e , metric_space != c 
		cout << "Metric space is Cosine." << endl;
		tSize = Space_Get_Euclidean_TableSize(infile2);		//Find the size of buckets for euclidean hashtables	
		metric_space = 'c';					
	}
	
		
	if (metric_space == 'h')
	{
		hamming = new Hamming(hash_functions, hash_tables, item_dimensions);
		if (validate == false)
			hamming->Hamming_Menu(infile1, qfile, outfile);
		else
			hamming->Hamming_Validation_Menu(infile1, qfile, outfile, tSize);
			
	}
	else if (metric_space == 'e')
	{
		euclidean = new Euclidean(hash_functions, hash_tables, item_dimensions, tSize/E_SIZE, random_array);
		if (validate == false)
			euclidean->Euclidean_Menu(infile1, qfile, outfile);
		else
			euclidean->Euclidean_Validation_Menu(infile1, qfile, outfile, tSize);			
	}
	else if (metric_space == 'c')
	{
		cosine = new Cosine(hash_functions, hash_tables, item_dimensions);
		if (validate == false)
			cosine->Cosine_Menu(infile1, qfile, outfile);
		else
			cosine->Cosine_Validation_Menu(infile1, qfile, outfile, tSize);
	}
		
}

int Space::Space_Get_Euclidean_TableSize(ifstream& infile)			//Estimate metric and dimensions for items
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





