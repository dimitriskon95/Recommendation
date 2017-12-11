#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <math.h>
#include <time.h>
#include "Cosine.h"
#include "Hashtable.h"
#include "List.h"
#include "Node.h"
#include "Validation.h"
#include "ValidationList.h"
#include "ValidationNode.h"

using namespace std;

Cosine::Cosine(int var_k, int var_L, int dimensions)
{
	cout << "A Cosine class was constructed" << endl;
	L = var_L;
	k = var_k;
	dims = dimensions;
	R = 1;
	c = 1;
	Pnumber = 20; //default
	hashtable = new HashTable<double,double>[L];
	for (int i=0; i <= L - 1; i++)
	{
		hashtable[i].HashTable_Init(i, var_k, dimensions, pow(2, var_k), 'c');
	}
}


Cosine::~Cosine()
{
	delete [] hashtable;
	cout << "Delete Cosine class" << endl;
}


int Cosine::get_L(){
	return L;
}

void Cosine::set_R(double varR)
{
	R = varR;
}

void Cosine::set_c(double varc)
{
	c = varc;
}


void Cosine::set_Pnumber(char *line)
{
	int line_count = 0, p_count = 0;
	char *p = new char[5];
	
	while(line[line_count] != ':')
		line_count++;
	line_count+=2;
	while(line[line_count] != '\0')
	{
		p[p_count] = line[line_count];
		line_count++;
		p_count++;
	}
	p[p_count] = '\0';
	Pnumber = atoi(&p[0]);
	cout << "Number of closest neighbors is " << Pnumber << endl;
	delete [] p;
}


void Cosine::Cosine_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile)
{
	int userID = 0, temp_userID = 0, item = 0, rating = 0, tmp_userID = 0, counter = 0;
	int line_count = 0, user_count = 0, item_count = 0, rate_count = 0, tmp_user_count = 0, tmp_line_count = 0;
	double sum =0, average = 0;
	double time;
	const clock_t begin_time = clock();						//time
	string line, tmp_line;
	char *user = new char[10];
	char *tmp_user = new char[10];
	char *item_number = new char[10];
	char *rate = new char[2];
	double *coordinate;
	Node<double>* node;
	
	outfile << "<Cosine LSH>" << '\n';
	cout << "Cosine Menu was called." << endl;
	
	if (infile.is_open())
	{
		//Read number of closest neighbors
		getline(infile, line);
		getline(tmp_file, tmp_line);
		set_Pnumber(&line[0]);
		
		//Read Input file and create items
		usersList = new List<double>;
		
		getline(tmp_file, tmp_line);	//1 bhma mprosta
		while(getline(infile, line)){
			getline(tmp_file, tmp_line);
			
			line_count = user_count = item_count = rate_count = 0;
			coordinate = new double[dims+1];
			
			//Initialization
			for(int i=0; i < dims; i++)
				coordinate[i] = 0;
			coordinate[dims] = '\0';
				
			while(line[line_count] != '\t')
			{
				user[user_count] = line[line_count];
				user_count++;
				line_count++;
			}
			user[user_count] = '\0';
			userID = atoi(user);
			
			line_count++;
			while(line[line_count] != '\t')
			{
				item_number[item_count] = line[line_count];
				item_count++;
				line_count++;
			}
			item_number[item_count] = '\0';
			item = atoi(item_number);
			
			line_count++;
			while(line[line_count] != '\0')
			{
				rate[rate_count] = line[line_count];
				rate_count++;
				line_count++;
			}
			rate[rate_count] = '\0';
			rating = atoi(rate);
			
			//Now put the first rating in bstring
			coordinate[item-1] = rating;
				
			//Get the next line to check if it is for the same user 
			do{
				tmp_line_count=0;
				tmp_user_count=0;
				while(tmp_line[tmp_line_count] != '\t')
				{
					tmp_user[tmp_user_count] = tmp_line[tmp_line_count];
					tmp_user_count++;
					tmp_line_count++;
				}
				tmp_user[tmp_user_count] = '\0';
				tmp_userID = atoi(tmp_user);
				
				if (tmp_userID == userID)
				{
					//Same user
					getline(infile, line);
					getline(tmp_file, tmp_line);
					
					line_count = user_count = item_count = rate_count = 0;
					
					while(line[line_count] != '\t')
					{
						user[user_count] = line[line_count];
						user_count++;
						line_count++;
					}
					user[user_count] = '\0';
					userID = atoi(user);
				
					line_count++;
					while(line[line_count] != '\t')
					{
						item_number[item_count] = line[line_count];
						item_count++;
						line_count++;
					}
					item_number[item_count] = '\0';
					item = atoi(item_number);
				
					line_count++;
					while(line[line_count] != '\0')
					{
						rate[rate_count] = line[line_count];
						rate_count++;
						line_count++;
					}
					rate[rate_count] = '\0';
					rating = atoi(rate);
				
					//Now put the first rating in bstring
					coordinate[item-1] = rating;
				}
				else
				{
					//Kanonikopoihsh twn timwn pou einai 0	
					sum = average = 0;
					counter = 0;
					for(int i=0; i < dims; i++)
					{
						if (coordinate[i] != 0)
						{
							counter++;
							sum += coordinate[i];
						}	
					}
					average = sum / counter;
													
					//New user detected , so save current user and put him in list
					node = new Node<double>(coordinate, dims, userID, 0, average);
					usersList->Insert_Node_End(node);
				}
			}while(tmp_userID == userID);			//Same user
		}		
	}	
	
	//Free memory
	delete [] user;
	delete [] tmp_user;
	delete [] item_number;
	delete [] rate;
	
	//Now we have a list (usersList) with all the users
	//We put them to the hashtables	
	Node<double>* current = usersList->get_start();
	Node<double>* new_node;
	double* new_data;
	
	for(int i=0; i < usersList->get_ListSize(); i++){
		for(int l=0; l < L; l++){
			new_data = new double[current->get_size()];
			for(int j=0; j <= current->get_size() -1; j++)
				new_data[j] = current->get_data()[j];
			new_node = new Node<double>(new_data, current->get_size(), current->get_Number(), current->get_Number_Table(), current->get_Average());
			hashtable[l].Insert_Node(new_node);
		}
		current = current->get_next();
	}
	//print Hashtables	
//	for(int l=0; l < L; l++)
//		hashtable[l].printList();

	Node<double>* LSH_node;
	int UserCounter=0;
	double *items_data;
	LSH_node = usersList->get_start();		
	while(LSH_node != NULL)	 // && UserCounter < 20)			//For each user 
	{	
		items_data = new double[dims];
		items_data = Cosine_nnLSH(LSH_node, outfile);
		LSH_node = LSH_node->get_next();
		//UserCounter++;
		delete [] items_data;
	}
	
	time = (double)((clock() - begin_time) /  (double)CLOCKS_PER_SEC);
	outfile << "Execution Time: " << time << " miliseconds" << "\n";
}


//Find the top 5 items
double *Cosine::Cosine_nnLSH(Node<double> *node, ofstream& outfile)
{
	cout << "Function NN-LSH was called for user with id: " << node->get_Number() << endl;
	int NumberItems = usersList->get_ListSize();
	double sim = 0, z = 0, sum = 0, max = 0;
	int count = 0, index = 0;
	
	Node<double> *neighbor;
	List<double>* qlist = new List<double>;				//temp list (assist creatin final list)
	List<double>* final_list_LSH = new List<double>;	//final list with all neighbors
	List<double> *plist = new List<double>;				//list with the closest "Pnumber" neighbors  
		
	//Search closer neighbor and if R > 0 find those with dist > R*c and the closest neighbor
	for(int l=0; l <= L -1; l++)
	{
		hashtable[l].HashTable_LSH(node, qlist);
		final_list_LSH->Insert_List(qlist, NumberItems);
	}
	
	//1.Find the closest P neighbors
	for(count=0; count < Pnumber; count++)
	{
		neighbor = Cosine_close_Neighbor(node, final_list_LSH);
		plist->Insert_Node_End(neighbor);
	}
	plist->printList();			//#=(Pnumber) closest users
	
	//2.Calculate new values for node's data
	double average = node->get_Average();
	double *data = new double[node->get_size()];
	for(int i=0; i < node->get_size(); i++)
		data[i] = node->get_data()[i];
	
	//Kanonikopoihsh
	for(int i=0; i < dims; i++)
	{
		if (data[i] != 0)
			data[i] = data[i] - average;
	}
	
	Node<double> *current_user;
	for(int i=0; i < node->get_size(); i++)
	{
		sim = z = sum = 0;
		current_user = plist->get_start();
		while(current_user != NULL)							//Gia ka8e item tou xrhsth
		{
			sim = Similarity(node, current_user);
			sum += sim * (current_user->get_data()[i]);
			z += sim;
			current_user = current_user->get_next();
		}
		if (z != 0)
			data[i] = sum / z + node->get_Average();				//actual ratings;
	}
	
	double *unharmed_data = new double[dims];	//has all changes
	for(int i = 0; i < dims; i++)
		unharmed_data[i] = data[i];
		
	//3.Find the top 5
	outfile << "<u" << node->get_Number() << ">" << "\t";
	for(int j=0; j < 5; j++)
	{
		max = -100;
		index = -1;
		for(int i=0; i < node->get_size(); i++){
			if (data[i] > max){
				max = data[i];
				index = i; 
			}
		}
		if (index >= 0){
			outfile << "<item" << index+1 << ">" << "\t";
			cout << "Item" << index + 1 << endl;
			data[index] = data[index] - 100;			//avoid taking the same item (data becomes harmed-data)
		}
	}
	outfile << "\n";
	
	qlist->set_start(NULL);
	delete qlist;
		
	delete final_list_LSH;	
	delete plist;
	delete [] data;
	
	return unharmed_data;		//used by validation
}


//Find the temporary closest neighbor
Node<double> *Cosine::Cosine_close_Neighbor(Node<double> *node, List<double> *ulist)
{
	double distance=0, min = 10000, sum=0;
	Node<double> *current = ulist->get_start();
	Node<double> *previous = ulist->get_start();
	Node<double> *last = ulist->get_start();
	Node<double> *closer_neighbor = NULL;
	Node<double> *previous_closer_neighbor = NULL;
	
	while(current != NULL)
	{
		if (current->get_Number() != node->get_Number())
		{
			distance = 0;
			sum = 0;
			for(int i=0; i<= node->get_size()-1; i++)
			{
				sum += pow(current->get_data()[i] - node->get_data()[i], 2);
			}
			distance = sqrt(sum);
			if (distance < min) 		//to avoid itself use (... && dist > 0)
			{
				min = distance;
				closer_neighbor = current;
				previous_closer_neighbor = previous;
			}
		}
		previous = current;
		current = current->get_next();
	}
	//last = previous;
	if (closer_neighbor == NULL)
		return NULL;
	
	if (previous_closer_neighbor == closer_neighbor)			//equal to (previous_closer_neighbor == ulist->get_start()) 	//first item
	{
		ulist->set_start(closer_neighbor->get_next());
		closer_neighbor->set_next(NULL);
		//last->set_next(closer_neighbor);			//put closer neighbor in the end
	}
	else
	{
		previous_closer_neighbor->set_next(closer_neighbor->get_next());
		closer_neighbor->set_next(NULL);
	}
	
	return closer_neighbor;
}

	
double Cosine::Similarity(Node<double>* user, Node<double>* node)
{
	double distance = 0, sum = 0, normX = 0, normY = 0, user_sum = 0 , node_sum = 0 ;
	bool node_zerovector_found = true, user_zerovector_found = true;
	
	double *user_data, *node_data;
	
	user_data = new double[dims];
	for(int i=0; i<= dims-1; i++)
		user_data[i] = user->get_data()[i];
		
	node_data = new double[dims];
	for(int i=0; i<= dims-1; i++)
		node_data[i] = node->get_data()[i];
		
	
	//Check : There is a zero vector?
	for(int i=0; i<= dims-1; i++)
	{
		if (node_data[i] != 0)
			node_zerovector_found = false;
		
		if (user_data[i] != 0)
			user_zerovector_found = false;
	}
	
	//If one at least is zero vector
	if (user_zerovector_found || node_zerovector_found)
	{	
		//3e-kanonikopoihsh
		if (user_zerovector_found)
		{
			//user is a zero vector
			for(int i=0; i <= dims - 1; i++)
				user_data[i] += 1;
		}
		else
		{	
			for(int i=0; i <= dims - 1; i++)
				user_sum += user_data[i];
			user_sum /= dims;
			for(int i=0; i <= dims - 1; i++)
				user_data[i] += user_sum;
		}
		
		if (node_zerovector_found)
		{
			//user is a zero vector
			for(int i=0; i <= dims - 1; i++)
				node_data[i] += 1;
		}
		else
		{	
			for(int i=0; i <= dims - 1; i++)
				node_sum += node_data[i];
			node_sum /= dims;
			for(int i=0; i <= dims - 1; i++)
				node_data[i] += node_sum;
		}
	}				
	
	//Calculate distance for non zero vectors
	for(int i=0; i<= node->get_size()-1; i++)
	{
		sum += (user_data[i])*(node_data[i]);
	}
	for(int i=0; i<= user->get_size()-1; i++)
	{
		normX += pow(user_data[i], 2);
	}
	normX = sqrt(normX);
	for(int i=0; i<= node->get_size()-1; i++)
	{
		normY += pow(node_data[i], 2);
	}
	normY = sqrt(normY);
	
	distance = 1 - (double)(sum /(normX*normY));
	
	delete [] user_data;
	delete [] node_data;
	
	return distance;
}


void Cosine::Cosine_Validation_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile, int usersNumber)
{
	int userID = 0, temp_userID = 0, item = 0, rating = 0, tmp_userID;
	string line, tmp_line;
	char *user = new char[10];
	char *tmp_user = new char[10];
	char *item_number = new char[10];
	char *rate = new char[2];
	double avgValidation = 0;
	int line_count = 0, user_count = 0, item_count = 0, rate_count = 0, tmp_user_count = 0, tmp_line_count = 0;
	ValidationNode *node;
	
	cout << "Cosine Validation Menu was called." << endl;
	
	validation = new Validation(usersNumber);
	
	int validation_list = 0;
	if (infile.is_open())
	{
		//Read number of closest neighbors
		getline(infile, line);
		
		while(getline(infile, line)){
			line_count = user_count = item_count = rate_count = 0;
				
			while(line[line_count] != '\t')
			{
				user[user_count] = line[line_count];
				user_count++;
				line_count++;
			}
			user[user_count] = '\0';
			userID = atoi(user);
			
			line_count++;
			while(line[line_count] != '\t')
			{
				item_number[item_count] = line[line_count];
				item_count++;
				line_count++;
			}
			item_number[item_count] = '\0';
			item = atoi(item_number);
			
			line_count++;
			while(line[line_count] != '\0')
			{
				rate[rate_count] = line[line_count];
				rate_count++;
				line_count++;
			}
			rate[rate_count] = '\0';
			rating = atoi(rate);
			
			node = new ValidationNode(userID, item, rating);
			
			validation->InsertNode(validation_list, node);
			validation_list++;
			if (validation_list == validation->get_NumLists())
				validation_list = 0;
		}
	}	
	//validation->printLists();
	
	int count = 0;
	Node<double> *new_user;		
	ValidationNode *Vcurrent;
		
	Node<double>* current;
	Node<double>* new_node;
	double* new_data;
	double sum=0, average=0;
	//Read Input file and create items only for training
	for(int valid=0; valid < validation->get_NumLists(); valid++)
	{
		validation_list = valid;		//validation subject		
		usersList = new List<double>;
		
		//First we put the users of the training lists in usersList (validation list rating not used)
		for(int u=1; u <= usersNumber; u++)
		{		
			//Initialization coordinate (coordinate has the data for each user)
			double *coordinate = new double[dims];
			for(int i=0; i < dims; i++)
				coordinate[i] = 0;
			
			//For all (training) lists except the validation_list
			for(int i=0; i < validation->get_NumLists(); i++)
			{
				if (i != validation_list)
				{
					Vcurrent = validation->get_List_start(i);
					while(Vcurrent != NULL)															//Check all items of the list
					{
						if (Vcurrent->get_userID() == u)											//Found the user with   ID = u
							coordinate[Vcurrent->get_item()-1] = Vcurrent->get_rating();			//Save rating
						Vcurrent = Vcurrent->get_next();
					}
				}
			}
			
			//Kanonikopoihsh twn timwn pou einai 0	
			sum = average = 0;
			count = 0;
			for(int i=0; i < dims; i++)
			{
				if (coordinate[i] != 0)
				{
					count++;
					sum += coordinate[i];
				}	
			}
			average = sum / count;
			
			new_user = new Node<double>(coordinate, dims, u, 0, average);
			usersList->Insert_Node_End(new_user);				//usersList contains all users we some small different ratings (10%)
		}
		//usersList->printList();
		
		//Now we have a list (usersList) with all the users
		//We can put them to the hashtables after we copy them
		current = usersList->get_start();
		for(int i=0; i < usersList->get_ListSize(); i++){
			for(int l=0; l < L; l++){
				new_data = new double[current->get_size()];
				for(int j=0; j <= current->get_size() -1; j++)
					new_data[j] = current->get_data()[j];
				new_node = new Node<double>(new_data, current->get_size(), current->get_Number(), current->get_Number_Table(), current->get_Average());
				hashtable[l].Insert_Node(new_node);
			}
			current = current->get_next();
		}
		
	//	for(int l=0; l < L; l++)
	//		hashtable[l].printList();
	
		Node<double>* LSH_node;
		int UserCounter=0;
		LSH_node = usersList->get_start();
		double *predict_array;
		double *actual_array = new double[dims];
		sum = 0;
		while(LSH_node != NULL) // && UserCounter < 20)
		{
			predict_array = Cosine_nnLSH(LSH_node, outfile);			//user data from nnLSH that we predict
			for(int i=0; i < dims; i++)
				actual_array[i] = predict_array[i];
				
			Vcurrent = validation->get_List_start(validation_list);		//find in validation list the true ratings of the user
			while(Vcurrent != NULL)
			{
				if (Vcurrent->get_userID() == LSH_node->get_Number())
					actual_array[Vcurrent->get_item()-1] = Vcurrent->get_rating();
				Vcurrent = Vcurrent->get_next();
			}
			
			for (int i=0; i <dims; i++)
			{
				if (actual_array[i] >= predict_array[i])
					sum += actual_array[i]- predict_array[i]; 
				else
					sum += predict_array[i] - actual_array[i]; 
			}
					
			LSH_node = LSH_node->get_next();
		//	UserCounter++;
		}
		
		avgValidation += sum;
		
		cout << "For subject " << validation_list << " MAE is " << sum << endl;
		delete [] actual_array;
		delete [] predict_array;
		
		delete usersList;
		
		delete [] hashtable;
		hashtable = new HashTable<double,double>[L];
		for (int i=0; i <= L - 1; i++)
			hashtable[i].HashTable_Init(i, k, dims, pow(2, k), 'c');		
	}
	
	outfile << "<Cosine LSH> MAE: " << avgValidation/validation->get_J() << "\n";
}




//Not used
void Cosine::Cosine_Reader(char *line, int Hash_index, int Number){
	int line_count = 0;
	char str[20];
	int j, i=0;
	double *coordinate;
	coordinate = new double[dims];
	Node<double>* node;
	while(line[line_count] != '\t')
		line_count++;
	line_count++;
		
//Read all coordinates from pointer
	for(int i=0; i <= dims - 1; i++)
	{
		j=0;
		while(line[line_count] != '\t') 
		{
			if (line[line_count] == '\0')
				break;
			str[j]= line[line_count];
			line_count++;
			j++;
		}
		str[j] = '\0';
		coordinate[i] = atof(str);
		line_count++;
	}

//	node = new Node<double>(coordinate, dims, Number);
	 
	hashtable[Hash_index].Insert_Node(node);
}


void Cosine::Cosine_LSH(char *line, ofstream& output, int Number, char option){
	double time_LSH, time_ALL;
	int line_count = 0;
	double dist = 0;
	char str[20];
	int j, i=0;
	double *coordinate;
	int NumberItems = 1000;
	coordinate = new double[dims];
	Node<double>* node;
	
	List<double>* qlist;
	List<double>* final_list_LSH;
	List<double>* final_list_ALL;
	
	while(line[line_count] != '\t')
		line_count++;
	line_count++;
		
//Read all coordinates from pointer
	for(int i=0; i <= dims - 1; i++)
	{
		j=0;
		while(line[line_count] != '\t') 
		{
			if (line[line_count] == '\0')
				break;
			str[j]= line[line_count];
			line_count++;
			j++;
		}
		str[j] = '\0';
		coordinate[i] = atof(str);
		line_count++;
	}

//	node = new Node<double>(coordinate, dims, Number);
	
	qlist = new List<double>;
	final_list_LSH = new List<double>;
	final_list_ALL = new List<double>;
	
	output << "Query: Item";
	output << node->get_Number();
	output << '\n';

	if (option == 'y')
		NumberItems = 3*get_L();
	
	//Search closer neighbor and if R > 0 find those with dist > R*c and the closest neighbor
	cout << "item_idS" << Number << '\t';
	const clock_t begin_time_LSH  = clock();						//time	
	for(int i=0; i <= L -1; i++)
	{
		hashtable[i].HashTable_LSH(node, qlist);
		final_list_LSH->Insert_List(qlist, NumberItems);
	}
	
	dist = final_list_LSH->Cosine_Distance(node, R, c, output, 0);			//0 for LSH
	output << "distanceLSH: ";
	output << dist;
	output << '\n';
		
	time_LSH = (double)((clock() - begin_time_LSH ) /  (double)CLOCKS_PER_SEC);
	
	//Search closer neighbor for ALL
	const clock_t begin_time_ALL = clock();							//time
	hashtable[0].HashTable_Search_All(node, final_list_ALL);
	
	dist = final_list_ALL->Cosine_Distance(node, R, c, output, 1);			//1 for True
	output << "distanceTrue: ";
	output << dist;
	output << '\n';
	time_ALL = (double)(( clock() - begin_time_ALL ) /  (double)CLOCKS_PER_SEC);			//time
	
	output << "tLSH: ";
	output << time_LSH;
	output << '\n';
	
	output << "tTrue: ";
	output << time_ALL;
	output << '\n';
	output << '\n';
	
	qlist->set_start(NULL);
	delete qlist;
	delete final_list_LSH;
	delete final_list_ALL;
}


void Cosine::printList(){
	for (int i=0; i <= L - 1; i++)
	{
		cout << "Hashtable " << i << endl;
		hashtable[i].printList();
	}
}

