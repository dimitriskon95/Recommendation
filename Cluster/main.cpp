#include <fstream>
#include <sstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>  //rand, RAND_MAX
#include <ctime>
#include <iomanip>

#include "Space.h"

using namespace std;

int main(int argc, char *argv[])
{	
	char metric;
	int items_number = 1000;
	bool validate = false;
	Space *S;
	ifstream inFile1;
	ifstream inFile2;
	ifstream qFile;
	ofstream outFile;
	
    srand(time(NULL));
	
	if (argc == 0)
	{
		cout << "Too few arguments" << endl;
		return -1;
	}
	
	for(int i=0; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			inFile1.open(argv[i+1]);
			inFile2.open(argv[i+1]);
			qFile.open(argv[i+1]);
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			outFile.open(argv[i+1]);
		}
		else if (strcmp(argv[i], "-validate") == 0)
		{
			validate = true;
		}
	}

	cout << "Project 3 is starting." << endl;
	cout << "Please choose the metric space. Type h for hamming, e for euclidean or c for cosine." << endl;
	cin >> metric; 
	S = new Space(metric, items_number);
	S->Space_Menu(inFile1, inFile2, qFile, outFile, validate);
	
	delete S;
	return 0;
}

