#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <math.h>
#include <time.h>
#include "Hamming.h"
#include "Hashtable.h"
#include "List.h"
#include "Node.h"
#include "Random.h"
#include "Validation.h"
#include "ValidationList.h"
#include "ValidationNode.h"

using namespace std;

Hamming::Hamming(int var_k, int var_L, int bit_size)
{
	cout << "A Hamming class was created." << endl;
	L = var_L;
	k = var_k;
	bsize = bit_size;
	R = 1;
	c = 1;
	Pnumber = 20; //default
	hashtable = new HashTable<char, int>[L];
	for (int i=0; i <= L - 1; i++)
	{
		hashtable[i].HashTable_Init(i, k, bit_size, pow(2, k), 'h');   //no 5th argument, so w = 0 by default
	}
}

Hamming::~Hamming()
{
	delete[] hashtable;
	cout << "Delete Hamming class" << endl;
}


int Hamming::bsize_return(){
	return bsize;
}

int Hamming::get_L(){
	return L;
}

void Hamming::set_R(double varR)
{
	R = varR;
}

void Hamming::set_c(double varc)
{
	c = varc;
}

void Hamming::set_Pnumber(char *line)
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


void Hamming::Hamming_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile)
{
	int userID = 0, temp_userID = 0, item = 0, rating = 0, tmp_userID = 0, counter = 0;
	int line_count = 0, user_count = 0, item_count = 0, rate_count = 0, tmp_user_count = 0, tmp_line_count = 0;
	double time;
	const clock_t begin_time = clock();						//time
	string line, tmp_line;
	char *user = new char[10];
	char *tmp_user = new char[10];
	char *item_number = new char[10];
	char *rate = new char[2];
	char *bstring;
	Node<char>* node;
	
	outfile << "<Hamming LSH>" << '\n';
	cout << "Hamming Menu was called." << endl;
	
	if (infile.is_open())
	{
		//Read number of closest neighbors
		getline(infile, line);
		getline(tmp_file, tmp_line);
		set_Pnumber(&line[0]);
		
		//Read Input file and create items
		usersList = new List<char>;
		
		getline(tmp_file, tmp_line);	//1 bhma mprosta
		while(getline(infile, line)){
			getline(tmp_file, tmp_line);
			
			line_count = user_count = item_count = rate_count = 0;
			bstring = new char[bsize_return()+1];
			
			//Initialization
			for(int i=0; i < bsize_return(); i++)
				bstring[i] = '0';
			bstring[bsize_return()] = '\0';
				
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
			if (rating > 2)
				bstring[item-1] = '1';
				
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
					if (rating > 2)
						bstring[item-1] = '1';
				}
				else
				{
					//Cut off (done) 													
					//New user detected , so save current user and put him in list
					node = new Node<char>(&bstring[0], bsize_return(), userID, 0, 0);
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
	Node<char>* current = usersList->get_start();
	Node<char>* new_node;
	char* new_data;
	
	for(int i=0; i < usersList->get_ListSize(); i++){
		for(int l=0; l < L; l++){
			new_data = new char[current->get_size()];
			for(int j=0; j <= current->get_size() -1; j++)
				new_data[j] = current->get_data()[j];
			new_node = new Node<char>(new_data, current->get_size(), current->get_Number(), current->get_Number_Table(), current->get_Average());
			hashtable[l].Insert_Node(new_node);
		}
		current = current->get_next();
	}
	//print Hashtables	
//	for(int l=0; l < L; l++)
//		hashtable[l].printList();

	Node<char>* LSH_node;
	int UserCounter=0;
	char *items_data;
	LSH_node = usersList->get_start();		
	while(LSH_node != NULL) 	// && UserCounter < 20)			//For each user 
	{	
		items_data = new char[bsize];
		items_data = Hamming_nnLSH(LSH_node, outfile);
		LSH_node = LSH_node->get_next();
		//UserCounter++;
		delete [] items_data;
	}
	
	time = (double)((clock() - begin_time) /  (double)CLOCKS_PER_SEC);
	outfile << "Execution Time: " << time << " miliseconds" << "\n";
}


//Find the top 5 items
char *Hamming::Hamming_nnLSH(Node<char> *node, ofstream& outfile)
{
	cout << "Function NN-LSH was called for user with id: " << node->get_Number() << endl;
	int NumberItems = usersList->get_ListSize();
	double sim = 0, z = 0, sum = 0, max = 0, zero_percentage =0, one_percentage = 0;
	int count = 0, item_number = 0;
	
	Node<char> *neighbor;
	List<char>* qlist = new List<char>;				//temp list (assist creatin final list)
	List<char>* final_list_LSH = new List<char>;	//final list with all neighbors
	List<char> *plist = new List<char>;				//list with the closest "Pnumber" neighbors  
		
	//Search closer neighbor and if R > 0 find those with dist > R*c and the closest neighbor
	for(int l=0; l <= L -1; l++)
	{
		hashtable[l].HashTable_LSH(node, qlist);
		final_list_LSH->Insert_List(qlist, NumberItems);
	}
	
	//1.Find the closest P neighbors
	for(count=0; count < Pnumber; count++)
	{
		neighbor = Hamming_close_Neighbor(node, final_list_LSH);
		plist->Insert_Node_End(neighbor);
	}
	//plist->printList();			//#=(Pnumber) closest users
	
	//2.Calculate new values for node's data
	char *data = new char[node->get_size()];
	for(int i=0; i < node->get_size(); i++)
		data[i] = node->get_data()[i];
	
	Node<char> *current_user;
	for(int i=0; i < node->get_size(); i++)
	{
		sim = 0;
		current_user = plist->get_start();
		while(current_user != NULL)							//Gia ka8e item tou xrhsth
		{
			sim = Similarity(node, current_user);
/*			if (current_user->get_data()[i] == '1')
				one_percentage += sim;
			else
				zero_percentage += sim;
*/		
			if (current_user->get_data()[i] == '1')
				data[i] = '1';
			current_user = current_user->get_next();
		}
/*		if (one_percentage >= zero_percentage)
			data[i] = '1';
		else
			data[i] = '0';	
*/
/*		if (one_percentage > 0)
			data[i] = '1';
*/
	}
	
	char *unharmed_data = new char[bsize];	//has all changes
	for(int i = 0; i < bsize; i++)
		unharmed_data[i] = data[i];
		
	//3.Find the top 5
	outfile << "<u" << node->get_Number() << ">" << "\t";
	count = 0;
	for(int i=0; i < node->get_size(); i++)
	{
		if (data[i] == '1')
		{
			outfile << "<item" << i+1 << ">" << "\t";
			cout << "Item" << i+1 << endl;
			count++;
		}
		if (count == 5)
			break;
	}
	if (count < 5)
	{
		for(int i=0; i < 5 - count; i++)
		{
			//Ektupwse ta upoloipa items me random an vreis olous tous assous kai einai < 5
			do{
				item_number = uniform_distribution(0, bsize-1);
			}while(data[item_number] == '1');
			
			data[item_number] = '1';
			
			outfile << "<item" << item_number + 1 << ">" << "\t";
			cout << "rItem" << item_number + 1 << endl;
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
Node<char> *Hamming::Hamming_close_Neighbor(Node<char> *node, List<char> *ulist)
{
	int distance=0, min = 10000;
	Node<char> *current = ulist->get_start();
	Node<char> *previous = ulist->get_start();
	Node<char> *last = ulist->get_start();
	Node<char> *closer_neighbor = NULL;
	Node<char> *previous_closer_neighbor = NULL;
	
	while(current != NULL)
	{
		if (current->get_Number() != node->get_Number())
		{
			distance = 0;
			for(int i=0; i<= node->get_size()-1; i++)
			{
				if (current->get_data()[i] != node->get_data()[i])
					distance++;
			}
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

	
int Hamming::Similarity(Node<char>* user, Node<char>* node)
{
	int dist = 0;
	double distance = 0;
	
	for(int i=0; i<= node->get_size()-1; i++)
	{
		if (user->get_data()[i] != node->get_data()[i])
			dist++;
	}
	distance = 1 - (double)(dist/node->get_size());
	
	return distance;
}


void Hamming::Hamming_Validation_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile, int usersNumber)
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
	
	cout << "Euclidean Validation Menu was called." << endl;
	
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
	Node<char> *new_user;		
	ValidationNode *Vcurrent;
		
	Node<char>* current;
	Node<char>* new_node;
	char* new_data;
	//Read Input file and create items only for training
	for(int valid=0; valid < validation->get_NumLists(); valid++)
	{
		validation_list = valid;		//validation subject		
		usersList = new List<char>;
		
		//First we put the users of the training lists in usersList (validation list rating not used)
		for(int u=1; u <= usersNumber; u++)
		{		
			//Initialization coordinate (coordinate has the data for each user)
			char *bstring = new char[bsize];
			for(int i=0; i < bsize; i++)
				bstring[i] = '0';
			
			//For all (training) lists except the validation_list
			for(int i=0; i < validation->get_NumLists(); i++)
			{
				if (i != validation_list)
				{
					Vcurrent = validation->get_List_start(i);
					while(Vcurrent != NULL)															//Check all items of the list
					{
						if (Vcurrent->get_userID() == u)										//Found the user with   ID = u
							if (Vcurrent->get_rating() > 2)
								bstring[Vcurrent->get_item()-1] = '1';			//Save rating
						Vcurrent = Vcurrent->get_next();
					}
				}
			}
			
			new_user = new Node<char>(&bstring[0], bsize_return(), u, 0, 0);
			usersList->Insert_Node_End(new_user);				//usersList contains all users we some small different ratings (10%)
		}
		//usersList->printList();
		
		//Now we have a list (usersList) with all the users
		//We can put them to the hashtables after we copy them
		current = usersList->get_start();
		for(int i=0; i < usersList->get_ListSize(); i++){
			for(int l=0; l < L; l++){
				new_data = new char[current->get_size()+1];
				for(int j=0; j <= current->get_size() -1; j++)
					new_data[j] = current->get_data()[j];
				new_data[current->get_size()] = '\0';
				new_node = new Node<char>(new_data, current->get_size(), current->get_Number(), current->get_Number_Table(), current->get_Average());
				hashtable[l].Insert_Node(new_node);
			}
			current = current->get_next();
		}
		
	//	for(int l=0; l < L; l++)
	//		hashtable[l].printList();
	
		Node<char>* LSH_node;
		int UserCounter=0;
		LSH_node = usersList->get_start();
		char *predict_array;
		char *actual_array = new char[bsize];
		double sum = 0;
		while(LSH_node != NULL && UserCounter < 20)
		{
			predict_array = Hamming_nnLSH(LSH_node, outfile);			//user data from nnLSH that we predict
			for(int i=0; i < bsize; i++)
				actual_array[i] = predict_array[i];
				
			Vcurrent = validation->get_List_start(validation_list);		//find in validation list the true ratings of the user
			while(Vcurrent != NULL)
			{
				if (Vcurrent->get_userID() == LSH_node->get_Number())
					if (Vcurrent->get_rating() > 2)
						actual_array[Vcurrent->get_item()-1] = '1';			//Save rating
				Vcurrent = Vcurrent->get_next();
			}
			
			for (int i=0; i <bsize; i++)
			{
				if (actual_array[i] >= predict_array[i])
					sum += actual_array[i] - predict_array[i]; 
				else
					sum += predict_array[i] - actual_array[i]; 
			}
					
			LSH_node = LSH_node->get_next();
			UserCounter++;
		}
		
		avgValidation += sum;
		
		cout << "For subject " << validation_list << " MAE is " << sum << endl;
		delete [] actual_array;
		delete [] predict_array;
		
		delete usersList;
		
		delete [] hashtable;
		hashtable = new HashTable<char,int>[L];
		for (int i=0; i <= L - 1; i++)
			hashtable[i].HashTable_Init(i, k, bsize, pow(2, k), 'h');	//no 5th argument, so w = 0 by default
			
	}
	
	avgValidation /= validation->get_NumLists();
	outfile << "<Hamming LSH> MAE: " << avgValidation/validation->get_J() << "\n";	
}


//Not used
void Hamming::Hamming_Reader(char *line, int Hash_index, int Number){
	int line_count = 0;
	char *bstring;
	bstring = new char[bsize_return()];
	Node<char>* node;
	
	while(line[line_count] != '\t')
		line_count++;
	line_count++;
	
	strcpy(bstring, &line[line_count]);  //copy bit string from line to bstring
	
//	node = new Node<char>(&bstring[0], bsize_return(), Number);
	
	hashtable[Hash_index].Insert_Node(node);
}



void Hamming::Hamming_LSH(char *line, ofstream& output, int Number, char option){  //Dont care to create one node for all hashtables
	double time_LSH, time_ALL;
	int line_count = 0;
	int dist;
	char *bstring;
	bstring = new char[bsize_return()];
	char name[4];
	int NumberItems = 1000;
	Node<char>* node;
	
	List<char>* qlist;
	List<char>* final_list_LSH;
	List<char>* final_list_ALL;
	
	while(line[line_count] != '\t')
		line_count++;
	line_count++;
	
	strcpy(bstring, &line[line_count]);  //copy bit string from line to bstring
	
	node = new Node<char>(&bstring[0], bsize_return(), Number, 0, 0);
	
	qlist = new List<char>;
	final_list_LSH = new List<char>;
	final_list_ALL = new List<char>;
	
	output << "Query: Item";
	output << node->get_Number();
	output << '\n';

	if (option == 'y')
		NumberItems = 3*get_L();
		
	
	//Search closer neighbor and if R > 0 find those with dist > R*c and the closest neighbor
	cout << "item_idS" << Number << '\t';
	const clock_t begin_time_LSH = clock();						//time	
	for(int i=0; i <= L -1; i++)
	{
		hashtable[i].HashTable_LSH(node, qlist);
		final_list_LSH->Insert_List(qlist, NumberItems);
	}

	dist = final_list_LSH->Hamming_Distance(node, R, c, output, 0);			//0 for LSH
	output << "distanceLSH: ";
	output << dist;
	output << '\n';
	
	time_LSH = (double)((clock() - begin_time_LSH ) /  (double)CLOCKS_PER_SEC);
	
	//Search closer neighbor for ALL
	const clock_t begin_time_ALL = clock();							//time
	hashtable[0].HashTable_Search_All(node, final_list_ALL);

	dist = final_list_ALL->Hamming_Distance(node, R, c, output, 1);			//1 for ALL
	output << "distanceTrue: ";
	output << dist;
	output << '\n';
	time_ALL = (double)(( clock() - begin_time_ALL ) /  (double)CLOCKS_PER_SEC);			//time
	
	output << "tLSH: ";
	output <<  time_LSH;
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


void Hamming::printList(){
	for (int i=0; i <= L - 1; i++)
	{
		cout << "Hashtable " << i << endl;
		hashtable[i].printList();
	}
}


template class List<int>;
template class List<char>;
template class List<double>;



/*
void Hamming::Hamming_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile)
{
	int userID = 0, temp_userID = 0, item = 0, rating = 0, tmp_userID;
	string line, tmp_line;
	char *user = new char[10];
	char *tmp_user = new char[10];
	char *item_number = new char[10];
	char *rate = new char[2];
	int line_count = 0, user_count = 0, item_count = 0, rate_count = 0, tmp_user_count = 0, tmp_line_count = 0;
	Node<char>* node;
	
	cout << "Hamming Menu was called." << endl;
	
	if (infile.is_open())
	{
		//Read number of closest neighbors
		getline(infile, line);
		getline(tmp_file, tmp_line);
		set_Pnumber(&line[0]);
		
		//Read Input file and create items
		usersList = new List<char>;
		
		getline(tmp_file, tmp_line);	//1 bhma mprosta
		while(getline(infile, line)){
			getline(tmp_file, tmp_line);
			line_count = 0;
			user_count = 0;
			item_count = 0;
			rate_count = 0;
			char *bstring = new char[bsize_return()+1];
			
			//Initialization
			for(int i=0; i < bsize_return(); i++)
				bstring[i] = '0';
			bstring[bsize_return()] = '\0';
				
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
			if (rating > 2)
				bstring[item-1] = '1';
				
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
					line_count = 0;
					user_count = 0;
					item_count = 0;
					rate_count = 0;
					
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
					if (rating > 2)
						bstring[item-1] = '1';
				}
				else
				{
					//New user detected
					//Save previous user
					node = new Node<char>(&bstring[0], bsize_return(), userID, 0);
					usersList->Insert_Node_End(node);
				}
			}while(tmp_userID == userID);
		}		
	}
	
	//Now we have a list (usersList) with all the users
	//We can put them to the hashtables
	
	
	Node<char>* current = usersList->get_start();
	Node<char>* new_node;
	char* new_data;
	int count = 0;
	
	for(int i=0; i < usersList->get_ListSize(); i++){
		for(int l=0; l < L; l++){
			new_data = new char[current->get_size()+1];
			for(int j=0; j <= current->get_size() -1; j++)
				new_data[j] = current->get_data()[j];
			new_data[current->get_size()+1]= '\0';
			new_node = new Node<char>(new_data, current->get_size(), current->get_Number(), current->get_Number_Table());
			hashtable[l].Insert_Node(new_node);
		}
		current = current->get_next();
	}
	
//	for(int l=0; l < L; l++)
//		hashtable[l].printList();

	Node<char>* LSH_node;
	int UserCounter=0;
	LSH_node = usersList->get_start();
	while(LSH_node != NULL && UserCounter < 20)
	{
		Hamming_nnLSH(LSH_node, outfile);
		LSH_node = LSH_node->get_next();
		usersList->nnLSH_Clean();
		UserCounter++;
	}

}
*/



