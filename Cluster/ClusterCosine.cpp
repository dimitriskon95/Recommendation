#include <fstream>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <time.h>
#include "Random.h"
#include "ClusterCosine.h"
#include "Cluster.h"
#include "List.h"
#include "Node.h"
#include "Validation.h"
#include "ValidationList.h"
#include "ValidationNode.h"

using namespace std;

ClusterCosine::ClusterCosine(int k_value, int dims, int hfunctions, int htables, int cfraction, int citerations)
{
	cout << "A Cluster Cosine class was created." << endl;
	dimensions = dims;
	k = k_value;
	
	hash_functions = hfunctions;
	hash_tables = htables;
	clarans_franction = cfraction;
	clarans_iterations = citerations;
	
	metric = 'e';
	
	Objective_Function_Value = 0;
	Objective_Function_Updated = new bool[k];
	for (int i=0; i <= k - 1; i++)	
		Objective_Function_Updated[i] = true;
	
	all_items_list = new List<double>;
	cluster = new Cluster<double>[k];
	for (int i=0; i <= k - 1; i++)
	{
		cluster[i].Cluster_Initialization(i);
	}
}

ClusterCosine::~ClusterCosine()
{
	delete all_items_list;
	delete [] cluster;
	cout << "Delete Cosine class" << endl;
}


int ClusterCosine::get_DimensionsNumber(){
	return dimensions;
}

int ClusterCosine::get_ClusterNumber(){
	return k;
}


void ClusterCosine::set_Pnumber(char *line)
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


//-------------------------------------------------------------------MENU FUNCTION-----------------------------------------------------------------------------
void ClusterCosine::ClusterCosine_Menu(ifstream& infile1, ifstream& infile2, ofstream& outfile)
{
	outfile << "<Cosine Clustering>" << '\n';
	cout << "ClusterEuclidean_Menu" << endl;
	double time;
	double *data;
	Node<double> *current;
	Node<double> *new_node;
	
	const clock_t begin_time = clock();
	//Fisrt thing is to read all items from infile
	Cosine_User_Reader(infile1, infile2);
	
	//all_items_list->printList();
	
	usersList = new List<double>;			//has a copy of all_items_list users, in order to use it to check every single user
	current = all_items_list->get_start();
	while(current != NULL)
	{
		data = new double[current->get_size()];
		for(int i=0; i < current->get_size(); i++)
			data[i] = current->get_data()[i];
		new_node = new Node<double>(data, current->get_size(), current->get_Number(), current->get_Number_Table(), current->get_Average());
		usersList->Insert_Node_End(new_node);
		current = current->get_next();
	}
	
	//Default
	this->optionInit = '1';
	this->optionAssign = '1';
	this->optionUpdate = '1';
				
	cout << "Algorithm: I" << optionInit << "A" << optionAssign << "U" << optionUpdate << ", x = {1,2} " << endl;
	cout << "Users : " << all_items_list->get_ListSize() << endl;

	//Initialization				
	Cosine_Cluster_Initialization(optionInit);	
			
	if (optionUpdate == '1')			//A la Lloyd
		Lloyd_Menu(outfile);
	else
		Clarans_Menu(outfile);
				
	time = (double)((clock() - begin_time ) /  (double)CLOCKS_PER_SEC);
	outfile << "clustering_time: " << time << " //in seconds" << '\n';
			
	outfile << "Silhouette: [";
	for(int i=0; i <= k - 1; i++)
		outfile << Silhouette_table[i] << ",";
	outfile << Silhouette_table[k] << "]" << '\n' << '\n';
	
	Node<double>* Clustering_node;
	int UserCounter=0;
	double *items_data;
	Clustering_node = usersList->get_start();						
	while(Clustering_node != NULL && UserCounter < 20)			//For each user 
	{	
		items_data = new double[dimensions];
		items_data = Cosine_Clustering(Clustering_node, outfile);
		Clustering_node = Clustering_node->get_next();
		UserCounter++;
		delete [] items_data;
	}
	
	time = (double)((clock() - begin_time) /  (double)CLOCKS_PER_SEC);
	outfile << "Execution Time: " << time << " miliseconds" << "\n";
				
	delete [] cluster;
	cluster = new Cluster<double>[k];
				
	all_items_list->set_start(usersList->get_start());
	all_items_list->set_List_size(usersList->get_ListSize());
				
	usersList->set_start(NULL);
	delete usersList;
}


void ClusterCosine::Cosine_User_Reader(std::ifstream& infile, std::ifstream& tmp_file)
{
	int userID = 0, temp_userID = 0, item = 0, rating = 0, tmp_userID, counter = 0, Ucounter=0;
	int line_count = 0, user_count = 0, item_count = 0, rate_count = 0, tmp_user_count = 0, tmp_line_count = 0;
	double sum = 0, average = 0;
	string line, tmp_line;
	char *user = new char[10];
	char *tmp_user = new char[10];
	char *item_number = new char[10];
	char *rate = new char[2];
	Node<double>* node;
	double *coordinate;
	
	cout << "Cosine Menu was called." << endl;
	
	if (infile.is_open())
	{
		//Read number of closest neighbors
		getline(infile, line);
		getline(tmp_file, tmp_line);
		set_Pnumber(&line[0]);
		
		//Read Input file and create items . Then put them in all_items_list
		getline(tmp_file, tmp_line);	//1 bhma mprosta
		while(getline(infile, line) && Ucounter<1000)			// && Ucounter<1000
		{				
			getline(tmp_file, tmp_line);
			
			line_count = user_count = item_count = rate_count = 0;			
			coordinate = new double[dimensions+1];
			
			//Initialization
			for(int i=0; i < dimensions; i++)
				coordinate[i] = 0;
			coordinate[dimensions] = '\0';
				
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
					counter=0;
					for(int i=0; i < dimensions; i++)
					{
						if (coordinate[i] != 0)
						{
							counter++;
							sum += coordinate[i];
						}	
					}
					average = sum / counter;
					
					//New user detected
					//Save previous user
					node = new Node<double>(coordinate, dimensions, userID, Ucounter, average);
					all_items_list->Insert_Node_End(node);
					Ucounter++;
				}
			}while(tmp_userID == userID);
		}		
	}
	
	//Free memory
	delete [] user;
	delete [] tmp_user;
	delete [] item_number;
	delete [] rate;	
}


double *ClusterCosine::Cosine_Clustering(Node<double> *node, ofstream& outfile)
{
	cout << "Function Cosine Clustering was called for user with id: " << node->get_Number() << endl;
	int NumberItems = usersList->get_ListSize();
	double sim = 0, z = 0, sum = 0, max = 0, min = 10000, distance = 0;
	int count = 0, index = 0, min_index = 0;
	
	Node<double> *neighbor;
	List<double> *clist;									//list with the all neighbors (in the same cluster)
	List<double> *ulist = new List<double>;					//same as clist
	List<double> *plist = new List<double>;					//list with the closest "Pnumber" neighbors 
	
	//Find closest centroid that is not empty
	for(int i=0; i<=k-1 ; i++)
	{
		distance = Get_Distance((cluster[i].get_Centroid())->get_Number_Table(), node->get_Number_Table());		
		if (distance < min && !((cluster[i].get_List())->is_empty()))		//List must have at least one item
		{
			min_index = i;
			min = distance;
		}
	}
	
	//Get the list that belongs to the closest centroid
	clist = cluster[min_index].get_List();			//min_index ->the number of the closest not empty cluster for the user 	
	
	//Copy list
	ulist->Insert_List(clist);
	
	//1.Find the closest P neighbors
	int counter = 0;
	while(counter < Pnumber)
	{
		neighbor = Cosine_close_Neighbor(node, ulist);
		if (neighbor != NULL)
			plist->Insert_Node_End(neighbor);
		else
			counter = Pnumber;
		counter++;
	}
	
	cout << "User : " << node->get_Number() << endl;
	cout << "Most Similar Users : " << '\t';
	plist->printList();
	
	//2.Calculate new values for node's data
	double average = node->get_Average();
	double *data = new double[node->get_size()];
	for(int i=0; i < node->get_size(); i++)
		data[i] = node->get_data()[i];
		
	//Kanonikopoihsh
	for(int i=0; i < dimensions; i++)
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
			data[i] = sum / z;
	}
	
	double *unharmed_data = new double[dimensions];	//has all changes
	for(int i = 0; i < dimensions; i++)
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
	
	delete plist;
	delete [] data;
	
	return unharmed_data;		//used by validation
}


Node<double> *ClusterCosine::Cosine_close_Neighbor(Node<double> *node, List<double> *ulist)
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
			distance = Get_Distance(node->get_Number_Table(), current->get_Number_Table());
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

	
double ClusterCosine::Similarity(Node<double>* user, Node<double>* node)
{
	int dims = dimensions;	
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
			user_sum = node->get_Average();		//return average
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
			node_sum = node->get_Average();		//return average
			for(int i=0; i <= dims - 1; i++)
				node_data[i] += node_sum;
		}
	}				
	
	//Calculate distance for non zero vectors
	for(int i=0; i<= node->get_size()-1; i++)
	{
		sum += (user_data[i])*(node_data[i]);
		normX += pow(user_data[i], 2);
		normY += pow(node_data[i], 2);
	}
	normX = sqrt(normX);
	normY = sqrt(normY);
	
	distance = (double)(sum /(normX*normY));
	
	delete [] user_data;
	delete [] node_data;
	
	return distance;
}


void ClusterCosine::Cosine_Validation_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile, int usersNumber)
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
		set_Pnumber(&line[0]);
		
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
	double *coordinate;
	
	//Read Input file and create items only for training
	for(int valid=0; valid < validation->get_NumLists(); valid++)
	{	
		validation_list = valid;		//validation subject	
		usersList = new List<double>;			//has a copy of all_items_list users, in order to use it to check every single user
		
		
		//First we put the users of the training lists in usersList (validation list rating not used)
		for(int u=1; u <= usersNumber; u++)
		{		
			//Initialization coordinate (coordinate has the data for each user)
			coordinate = new double[dimensions];
			for(int i=0; i < dimensions; i++)
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
			for(int i=0; i < dimensions; i++)
			{
				if (coordinate[i] != 0)
				{
					count++;
					sum += coordinate[i];
				}	
			}
			average = sum / count;
			
			new_user = new Node<double>(coordinate, dimensions, u, u-1, average);					
			all_items_list->Insert_Node_End(new_user);				//all_items_List contains all users we some small different ratings (10%)
		}
		//all_items_list->printList();
		
		double *data;
		current = all_items_list->get_start();
		while(current != NULL)
		{
			data = new double[current->get_size()];
			for(int i=0; i < current->get_size(); i++)
				data[i] = current->get_data()[i];
			new_node = new Node<double>(data, current->get_size(), current->get_Number(), current->get_Number_Table(), current->get_Average());
			usersList->Insert_Node_End(new_node);
			current = current->get_next();
		}
		
		//Default
		this->optionInit = '1';
		this->optionAssign = '1';
		this->optionUpdate = '1';
					
		cout << "Algorithm: I" << optionInit << "A" << optionAssign << "U" << optionUpdate << ", x = {1,2} " << endl;
		cout << "Users : " << all_items_list->get_ListSize() << endl;
	
		//Initialization				
		Cosine_Cluster_Initialization(optionInit);	
			
		if (optionUpdate == '1')			//A la Lloyd
			Lloyd_Menu(outfile);
		else
			Clarans_Menu(outfile);
				
		outfile << "Silhouette: [";
		for(int i=0; i <= k - 1; i++)
			outfile << Silhouette_table[i] << ",";
		outfile << Silhouette_table[k] << "]" << '\n' << '\n';
		
		Node<double>* Clustering_node;
		int UserCounter=0;
		double *predict_array;
		double *actual_array;
		sum = 0;
		Clustering_node = usersList->get_start();						
		while(Clustering_node != NULL) // && UserCounter < 20)			//For each user 
		{	
			predict_array = new double[dimensions];
			actual_array = new double[dimensions];
			
			predict_array = Cosine_Clustering(Clustering_node, outfile);
			
			for(int i=0; i < dimensions; i++)
				actual_array[i] = predict_array[i];
			
			Vcurrent = validation->get_List_start(validation_list);		//find in validation list the true ratings of the user
			while(Vcurrent != NULL)
			{
				if (Vcurrent->get_userID() == Clustering_node->get_Number())
					actual_array[Vcurrent->get_item()-1] = Vcurrent->get_rating();
				Vcurrent = Vcurrent->get_next();
			}
			
			for (int i=0; i <dimensions; i++)
			{
				if (actual_array[i] >= predict_array[i])
					sum += actual_array[i] - predict_array[i]; 
				else
					sum += predict_array[i] - actual_array[i]; 
			}
			
			
			Clustering_node = Clustering_node->get_next();
			UserCounter++;
			delete [] predict_array;
			delete [] actual_array;
		}
		
		avgValidation += sum;
		
		cout << "For subject " << validation_list << " MAE is " << sum << endl;
				
		delete [] cluster;
		cluster = new Cluster<double>[k];
		
		delete all_items_list;
		all_items_list = new List<double>;
		
		delete usersList;
	}
	
	avgValidation /= validation->get_NumLists();
	outfile << "<Cosine Clustering> MAE: " << avgValidation/validation->get_J() << "\n";
}



void ClusterCosine::Lloyd_Menu(ofstream& outfile)
{
	while(Keep_Update())
		Update_Lloyd_Change_All_medoids();				//or 	 Update_Lloyd()				
	
	Silhouette(outfile);
			
	for(int i=0; i <= k-1; i++)	
		outfile <<  "CLUSTER-" << i << " {size: " << (cluster[i].get_List())->get_ListSize() << ", medoid: item" << (cluster[i].get_Centroid())->get_Number() << "}" << '\n';
}


void ClusterCosine::Clarans_Menu(ofstream& outfile)
{
	int Clarans_Objective_Function_Best = 0;
	int *clarans_centroid_ItemNumber = new int[k];
	int *clarans_centroid_ListSize = new int[k];
	for(int s=1; s <= clarans_iterations; s++)
	{
		Update_Clarans_One_Swap();			//or     	Update_Clarans();
		
		if (s == 1)
		{
			Silhouette(outfile);
			for(int i=0; i<=k -1; i++)
			{
				clarans_centroid_ItemNumber[i] = (cluster[i].get_Centroid())->get_Number();
				clarans_centroid_ListSize[i] = (cluster[i].get_List())->get_ListSize();
			}
			Clarans_Objective_Function_Best = Objective_Function_Value;
		}
		
		if (Objective_Function_Value < Clarans_Objective_Function_Best)
		{
			Silhouette(outfile);
			for(int i=0; i<=k -1; i++)
			{
				clarans_centroid_ItemNumber[i] = (cluster[i].get_Centroid())->get_Number();
				clarans_centroid_ListSize[i] = (cluster[i].get_List())->get_ListSize();
			}
			Clarans_Objective_Function_Best = Objective_Function_Value;
		}
		//clear lists
		all_items_list->set_List_size(0);
		List<double> *clusterlist;
		for(int i=0 ; i <= k - 1; i++)					//Put all non centroid items at all_items_list
		{
			clusterlist = cluster[i].get_List();
			all_items_list->Insert_Cluster_List(clusterlist);
			all_items_list->Insert_Node_End(cluster[i].get_Centroid());
			(cluster[i].get_Centroid())->set_notCentroidValue();
			cluster[i].set_List_empty();
		}
	
		if (s < clarans_iterations)
		{		
			if (optionInit == '1')
			{
				cout << "k-medoids++ Initialization" << endl;
				KmedoidsInitialization();
			}
			else if (optionInit == '2')
			{
				cout << "Park-Jun Initialization" << endl;
				ConcentrateInitialization();
			}
		}
	}
	Objective_Function_Value = Clarans_Objective_Function_Best;
						
	for(int i=0; i <= k-1; i++)	
		outfile <<  "CLUSTER-" << i << " {size: " << clarans_centroid_ListSize[i] << ", medoid: item" << clarans_centroid_ItemNumber[i] << "}" << '\n';
	
}


void ClusterCosine::Clara_Menu(ofstream& outfile)
{
	int Clara_Objective_Function_Previous = 20000;
	Node<double> *node;
	int *clara_centroid_ItemNumber = new int[k];
	int *clara_centroid_ListSize = new int[k];
	double time_LSH, time_ALL;
	const clock_t begin_time_LSH = clock();
	optionAssign = '1';
	for (int s=0; s<= 4; s++)
	{
		Clara_Function();
					
		if (Objective_Function_Value < Clara_Objective_Function_Previous)
		{
			Silhouette(outfile);
			for(int i=0; i<=k -1; i++)
			{
				node = cluster[i].get_Centroid();
				clara_centroid_ItemNumber[i] = node->get_Number();
				clara_centroid_ListSize[i] = (cluster[i].get_List())->get_ListSize();
			}
				
			Clara_Objective_Function_Previous = Objective_Function_Value;
		}
						
		all_items_list->set_start(clara_list->get_start());
		all_items_list->set_List_size(clara_list->get_ListSize());
		
		clara_list->set_start(NULL);
		clara_list->set_List_size(0);
			
		if (s < 4)
		{
			delete [] cluster;
			cluster = new Cluster<double>[k];
		}
		
		for(int l=0; l <= Number_of_Items -1; l++)
			delete [] Distance_Table[l];
		delete [] Distance_Table;
	}
		
	outfile << "Algorithm: CLARA";
	outfile << '\n';
		
	for(int i=0; i <= k-1; i++)	
		outfile <<  "CLUSTER-" << i << " {size: " << clara_centroid_ListSize[i] << ", medoid: item" << clara_centroid_ItemNumber[i] << "}" << '\n';
	
	delete [] clara_centroid_ItemNumber;
	delete [] clara_centroid_ListSize;
	
	delete [] cluster;
	cluster = new Cluster<double>[k];
	
	time_LSH = (double)((clock() - begin_time_LSH ) /  (double)CLOCKS_PER_SEC);
	outfile << "clustering_time: " << time_LSH << " //in seconds" << '\n';
				
	outfile << "Silhouette: [";
	for(int i=0; i <= k - 1; i++)
		outfile << Silhouette_table[i] << ",";
	outfile << Silhouette_table[k] << "]" << '\n';
}

void ClusterCosine::Cosine_Reader(char *line, int NumberTable){
	int line_count = 0;
	char str[20];
	char NumItem[10];
	int j, it = 0;
	int ItemNumber = 0;
	double *coordinate;
	coordinate = new double[get_DimensionsNumber()];
	Node<double>* node;
	
	while(line[line_count] != 'm')		//1' || line[line_count] != '2' || line[line_count] != '3' || line[line_count] != '4' || line[line_count] != '5' || line[line_count] != '6' || line[line_count] != '7' || line[line_count] != '8' || line[line_count] != '9')
		line_count++;
	line_count++;
	
	while(line[line_count] != '\t')
	{
		NumItem[it] = line[line_count];
		line_count++;
		it++;
	}
	NumItem[it] = '\0';
	line_count++;
	
	ItemNumber = atoi(NumItem);
	
//Read all coordinates from pointer
	for(int i=0; i <= get_DimensionsNumber() - 1; i++)
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
	
	node = new Node<double>(coordinate, get_DimensionsNumber(), ItemNumber, NumberTable, 0);
	
	all_items_list->Insert_Node_End(node);			//put item at the list
}


void ClusterCosine::Cosine_Cluster_Initialization(char option)
{
	//Calculate Distance_Table
	Distance_Table_Calculation();
	
	if (option == '1')
	{
		cout << "k-medoids++ Initialization" << endl;
		KmedoidsInitialization();
	}
	else if (option == '2')
	{
		cout << "Park-Jun Initialization" << endl;
		ConcentrateInitialization();
	}
}



//Dhmiourgei enan anw trigwniko pinaka me tis apostaseis
void ClusterCosine::Distance_Table_Calculation()
{
	int number_of_items = all_items_list->get_ListSize();
	Number_of_Items = number_of_items;
	int line = 0, column = 0;
	Node<double> *current_node_line = all_items_list->get_start();
	Node<double> *current_node_column;
	
	Distance_Table = new double*[number_of_items];
	for(int i=0; i<= number_of_items - 1; i++)
		Distance_Table[i] = new double[number_of_items];
	
	line=0;
	while(current_node_line != NULL)
	{
		current_node_column = current_node_line;
		column = line;
		while(current_node_column != NULL)
		{
			Distance_Table[line][column] = Cluster_Cosine_Distance(current_node_line, current_node_column);
			column++;
			current_node_column = current_node_column->get_next();
		}
		line++;
		current_node_line = current_node_line->get_next();
	}
	
/*	for(int i=0; i<= number_of_items - 1; i++)
	{
		for(int j=0; j<= number_of_items - 1; j++)
		{
			if (i <= j)
				cout << Distance_Table[i][j] << " ";
			else
				cout << "-1" << " ";
		}
		cout << endl;
	}*/
}


double ClusterCosine::Cluster_Cosine_Distance(Node<double>* cnode, Node<double>* inode)
{
	double distance = 0, sum = 0, normX = 0, normY = 0;
	for(int i=0; i<= cnode->get_size()-1; i++)
	{
		sum += (cnode->get_data()[i])*(inode->get_data()[i]);
		normX += pow(cnode->get_data()[i], 2);
		normY += pow(inode->get_data()[i], 2);
	}
	normX = sqrt(normX);
	normY = sqrt(normY);
	
	distance = 1 - (double)(sum /(normX*normY));
	
	return distance;
}


//Return the distance of two items from the Distance_Table (instead of calculating again)
double ClusterCosine::Get_Distance(int i, int j)
{
	if (i <= j)
		return Distance_Table[i][j];
	else
		return Distance_Table[j][i];
}


void ClusterCosine::KmedoidsInitialization()
{
	int random = 0, number_of_items = all_items_list->get_ListSize(), cluster_count = 0;
	
//1st Step : Choose a random centroid
	random = uniform_distribution(0, number_of_items-1);
	Node<double>* centroid = all_items_list->get_random_Node(random);
	cluster[0].set_Centroid(centroid);			//set the first centroid randomly
	
//2nd step, 3, 4
	float *P_Table = new float[number_of_items];
	float **Distance;
	float *final_Distance = new float[number_of_items];
	float sum = 0, float_random;
	int j=0, Dmax = 0, x = 0, euclidean_dist;
		
	Distance = new float*[get_ClusterNumber()];
	for(int i=0; i <= get_ClusterNumber()-1; i++)
		Distance[i] = new float[number_of_items];
	
	Node<double> *current = all_items_list->get_start();	

	while(cluster_count < get_ClusterNumber()-1)
	{
		current = all_items_list->get_start();
		j=0;
		Dmax = 0;
		sum = 0;
		x = 0;
		while(current != NULL)
		{
			euclidean_dist = Get_Distance((cluster[cluster_count].get_Centroid())->get_Number_Table(), current->get_Number_Table());
			Distance[cluster_count][j] = euclidean_dist;
			j++;
			current = current->get_next();
		}
		
		//This is used to avoid computing distances between centroids, make their value equal to zero
		for(int j=0; j <= number_of_items-1; j++)
			for(int i=0; i <= cluster_count; i++)
				if (Distance[i][j] == 0)
					for(int item=0; item <= cluster_count; item++)
						Distance[item][j] = 0;
		
		//Estimation of minimal distance 
		for(int i=0; i <= cluster_count; i++)
		{
			if (i == 0)				//Initialization for Distance with the first cluster
			{
				for(int j=0; j <= number_of_items-1; j++)
					final_Distance[j] = Distance[i][j];	
			}
			else
			{
				for(int j=0; j <= number_of_items-1; j++)
					if (final_Distance[j] > Distance[i][j])
						final_Distance[j] = Distance[i][j];
			}
		}
		
		//Estimate max at Distance Table
		for(int j=0; j <= number_of_items-1; j++)
		{
			if (final_Distance[j] > Dmax)
				Dmax = final_Distance[j];
		}
		
		//Divide each element with Dmax
		for(int j=0; j <= number_of_items-1; j++)
			final_Distance[j] /= Dmax;
		
		//Estimate P(r) table (value is zero for the distance from c to c' (c,c' from C))
		for(int j=0; j <= number_of_items-1; j++)
		{
			P_Table[j] = pow(final_Distance[j], 2) + sum;
			sum = P_Table[j];
		}
		
		do{
			float_random = uniform_distribution_float(0, sum);
			for(int j=0; j <= number_of_items-1; j++)
			{
				if (float_random <= P_Table[j])		//Check every single element. Not need to check if P_Table[j-1] < float_random because it is already checked as P_table[j-1] is checked a loop before . We can use binary search instead
				{
					x = j;
					break;
				}
			}
			centroid = all_items_list->get_random_Node(x);
		}while(centroid->get_CentroidValue());						//Check if the centroid is already used	
		
		cluster_count++;
		
		cluster[cluster_count].set_Centroid(centroid);
	}
	
	//Assignment items with centroid value = false from all_items_list to new centroids
	//Assignment items with centroid value = false from all_items_list to new centroids
	Assignment_Initialization();
	
	//Print
//	printClusters();
	
	for(int i=0; i <= get_ClusterNumber()-1; i++)
		delete [] Distance[i];
	delete [] Distance;
	delete [] P_Table;
	delete [] final_Distance;
}


void ClusterCosine::ConcentrateInitialization()
{
	//Step 1 : array nxn
	int NumItems = Number_of_Items;
	
	//Step 2 : Vi
	double *V_Table = new double[NumItems];
	double *divide_sum = new double[NumItems];
	double sum = 0;
	
	//Calculate for all j one time, less time (+)
	for(int j=0; j <= NumItems - 1; j++)
		for(int t=0; t <= NumItems - 1; t++)
			divide_sum[j] += Get_Distance(j, t);
	
	for(int i=0; i <= NumItems - 1; i++)
	{
		sum = 0;
		for(int j=0; j <= NumItems - 1; j++)
			sum += (double)(Get_Distance(i, j) / divide_sum[j]) ;
		V_Table[i] = sum;
	}
	
//Step 3 : find the k minimal Vi
	double min = 100;
	int *Index = new int[k];
	int min_index = 0;
	
	for(int kCluster = 0; kCluster <= k - 1; kCluster++)
	{
		min = 100;
		for(int i=0; i <= NumItems - 1; i++)
		{
			if (V_Table[i] < min && V_Table[i] != 0)
			{
				min = V_Table[i];
				min_index = i;
			}
		}
		V_Table[min_index] = 0;
		Index[kCluster] = min_index;
	}
	
//Create Centroid with Index array
	Node<double> *centroid;
	for(int kCluster = 0; kCluster <= k - 1; kCluster++)
	{
		centroid = all_items_list->get_random_Node(Index[kCluster]);
		cluster[kCluster].set_Centroid(centroid);
	}
	
	//Assignment items with centroid_value = false from all_items_list to new centroids
	Assignment_Initialization();					//It will also estimate Objective Function Value		//Only for the Initialization
	
	//Print
//	printClusters();	
	
	delete [] V_Table;
	delete [] divide_sum;
	delete [] Index;	
}


//----------------------------------------------------------------------Assignment----------------------------------------------------------------------
void ClusterCosine::Assignment_Initialization()		//It assigns both the centroid and the non centroids items
{
	int min = 10000, min_index = 0;
	Node<double> *min_centroid;
	Node<double> *current = all_items_list->get_start();
	while(current != NULL)
	{
		if (current->get_CentroidValue())
		{
			all_items_list->set_start(current->get_next());
			current->set_next(NULL);
			current = all_items_list->get_start();
		}
		else
		{
			//Calculate k distances from each centroid
			double *distance = new double[k];
			all_items_list->set_start(current->get_next());
			for(int i=0; i <= k - 1; i++)
				distance[i] = Cluster_Cosine_Distance(cluster[i].get_Centroid(), current);
				
			min = distance[0];
			min_index = 0;
			
			for(int i=0; i <= k - 1; i++)
			{
				if (distance[i] < min)
				{
					min = distance[i];
					min_index = i;
					min_centroid = cluster[i].get_Centroid();
				}
			}
			
			current->set_next(NULL);
			cluster[min_index].Cluster_Assignment(current);
			current = all_items_list->get_start();
			delete [] distance;
		}
	}
	Objective_Function_Value = Objective_Function();			//Only for the Initialization
	cout << "Initial Objective Function is " << Objective_Function_Value << endl;
}


void ClusterCosine::Assignment()						//It assign only the non centroid items
{
	int min = 10000, min_index = 0;
	Node<double> *min_centroid;
	Node<double> *current = all_items_list->get_start();
	while(current != NULL)
	{
		//Calculate k distances from each centroid
		double *distance = new double[k];
		all_items_list->set_start(current->get_next());
		for(int i=0; i <= k - 1; i++)
			distance[i] = Cluster_Cosine_Distance(cluster[i].get_Centroid(), current);
			
		min = distance[0];
		min_index = 0;
		
		for(int i=0; i <= k - 1; i++)
		{
			if (distance[i] < min)
			{
				min = distance[i];
				min_index = i;
				min_centroid = cluster[i].get_Centroid();
			}
		}
		
		current->set_next(NULL);
		cluster[min_index].Cluster_Assignment(current);
		current = all_items_list->get_start();
		delete [] distance;
	}
}


void ClusterCosine::PAM_Assignment()
{
//	cout << "PAM Assignment" << endl;
	List<double> *clusterlist;
	for(int i=0 ; i <= k - 1; i++)					//Put all non centroid items at all_items_list
	{
		clusterlist = cluster[i].get_List();
		all_items_list->Insert_Cluster_List(clusterlist);
		cluster[i].set_List_empty();
	}
	
	Assignment();									//assign all the items in all_items_list (they are only non centroid items)
	
	//Print
//	printClusters();	
}


void ClusterCosine::LSH_Assignment()
{
//	cout << "LSH Assignment" << endl;
	
	List<double> *clusterlist;
	for(int i=0 ; i <= k - 1; i++)			//put all items out of the list like PAM
	{
		clusterlist = cluster[i].get_List();
		all_items_list->Insert_Cluster_List(clusterlist);
		cluster[i].set_List_empty();
	}
	
	List<double> *LSH_centroid_list;
	LSH_centroid_list = new List<double>[k];
	
//	for(int i=0; i <= k - 1; i++)
	//	LSH_centroid_list[i].set_start((cosine->LSH_Cluster_Cosine(cluster[i].get_Centroid()))->get_start());			//LSH cluster Euclidean is made in class Euclidean
	
	Node<double> *c_current;
	Node<double> *c_previous;
	Node<double> *c_deleted_node;
	
	//Make the lists from LSH Hashtables
	for(int i=0; i <= k - 1; i++)
	{
		c_current = LSH_centroid_list[i].get_start();
		c_previous = LSH_centroid_list[i].get_start();
		while(c_current != NULL)
		{
			if (IsCentroid(c_current))
			{
				c_deleted_node = c_current;
				if (c_previous == c_current)			//1st at list
					LSH_centroid_list[i].set_start(c_current->get_next());
				else
					c_previous->set_next(c_current->get_next());
				
				c_current = c_current->get_next();
				delete c_deleted_node;
			}
			else
			{
				c_previous = c_current;
				c_current = c_current->get_next();
			}
		}
	}

//Generate R (Initialize it)	
	int Rmin = Cluster_Cosine_Distance(cluster[0].get_Centroid(), cluster[1].get_Centroid());
	int dist;
	for(int i=0; i <= k - 1; i++)
	{
		for(int j = i+1; j <= k - 1; j++)
		{
			dist = Cluster_Cosine_Distance(cluster[i].get_Centroid(), cluster[j].get_Centroid());
			if (dist < Rmin)
			{
				Rmin = dist;
			}
		}
	}
	Rmin = Rmin / 2;
	//cout << "R min is " << Rmin << endl;					
					
	Node<double> *current;
	Node<double> *previous;
	Node<double> *del_node;					
	Node<double> *item;
					
	Node<double> *current_list;	//from other list
	Node<double> *previous_list;
	Node<double> *del_node_list;
	
	int itemNumber, cluster_number = 0, Rtemp, R = Rmin;
	double distance = 0;
	
	bool *flag;
	flag = new bool[k];
	for(int fi=0; fi <= k -1; fi++)
		flag[fi] = true;
		
	
	while(LSH_Keep_Update(flag))					//Gia kapoio R
	{
	//	cout << "R is " << R << endl;
		for(int fi=0; fi <= k -1; fi++)
			flag[fi] = false;
		
		for(int i=0; i <= k -1; i++)				//Gia ka8e centroid
		{
			current = LSH_centroid_list[i].get_start();
			previous = LSH_centroid_list[i].get_start();
			
			while(current != NULL)					//Gia ka8e item
			{
				distance = Get_Distance((cluster[i].get_Centroid())->get_Number_Table(), current->get_Number_Table());
				if (distance <= R)
				{
					flag[i] = true;
					
					Rtemp = distance;				//temporary value for R until we check all lists
					item = current;
					itemNumber = current->get_Number();
					cluster_number = i;						//keep centroid number for this item
						
					if (previous == current)			//1st at list
					{
						LSH_centroid_list[i].set_start(current->get_next());			//Delete it
						previous = current->get_next();
					}
					else
						previous->set_next(current->get_next());						//Delete it
					
					for(int j=0; j <= k - 1; j++)		//Check all other lists for the same item and see if there is a centroid that is closer than the centroid of the i cluster 
					{
						if (i != j)
						{
							current_list = LSH_centroid_list[j].get_start();
							previous_list = LSH_centroid_list[j].get_start();
							
							while(current_list != NULL)
							{
								if (itemNumber == current_list->get_Number())
								{
									del_node_list = current_list;
									if (previous_list == current_list)			//1st at list
									{
										LSH_centroid_list[j].set_start(current_list->get_next());
										previous_list = current_list->get_next();
									}
									else
										previous_list->set_next(current_list->get_next());
									
									distance = Get_Distance((cluster[j].get_Centroid())->get_Number_Table(), current_list->get_Number_Table());
									if (distance < Rtemp)
									{
										Rtemp = distance;
										cluster_number = j;
									}
									
									current_list = current_list->get_next();
									delete del_node_list;
								}
								else
								{
									previous_list = current_list;
									current_list = current_list->get_next();
								}							
							}
						}
					}
					//Find it at all items list
					Node<double> *current_all_list = all_items_list->get_start();
					Node<double> *previous_all_list = all_items_list->get_start();
					while(current_all_list != NULL)
					{
						if (current_all_list->get_Number() == itemNumber)
						{
							if (previous_all_list == current_all_list)								//1st at list
							{
								all_items_list->set_start(current_all_list->get_next());			//Delete it from all_items_list
								previous_all_list = current_all_list->get_next();					
							}
							else
								previous_all_list->set_next(current_all_list->get_next());			//Delete it from all_items_list
							
							current_all_list->set_next(NULL);
							all_items_list->set_List_size(all_items_list->get_ListSize()-1);
							cluster[cluster_number].Cluster_Assignment(current_all_list);			//Assign it to the cluster[centroid number]
							
							current_all_list = NULL;
						}
						else
						{
							previous_all_list = current_all_list;
							current_all_list = current_all_list->get_next();
						}
					}
					current = current->get_next();
					delete item;																	//	delete item;
				}
				else
				{
					previous = current;
					current = current->get_next();
				}
			}
		}
		R = R*2;
	}
		
	Assignment(); 		//Gia ka8e remaining item stin all_items_list 			//Find the closest centroid			//Assign it to the cluster[centroid number]	   		//Delete it from all_items_list
	
	//printClusters();			//Print
}


void ClusterCosine::printClusters()
{
	for(int i=0; i <= k -1; i++)
	{
		cout << "Cluster " << i << " with centroid: ";
		cluster[i].printCentroid();
		cout << endl;
		cluster[i].printList();
	}
	cout << "Objective  Value   J = " << Objective_Function_Value << endl;
}


bool ClusterCosine::LSH_Keep_Update(bool *flag)
{
	int count = 0;					//number of cluster that has at least one item assign from the bucket
	for(int i=0; i <= k - 1; i++)
		if (flag[i])
			count++;
	if (count > k/2)				//more than the half
		return true;
	else
		return false;
}

//----------------------------------------------------------------------Objective_Function----------------------------------------------------------------------


double ClusterCosine::Objective_Function()
{
	double sum = 0;
	Node<double> *centroid;
	Node<double> *current;
	List<double> *items_list;
	for(int i =0; i <= k -1 ; i++)
	{
		centroid = cluster[i].get_Centroid();
		items_list = cluster[i].get_List();
		if (items_list->get_start() != NULL)
		{
			current = items_list->get_start();
			while(current != NULL)
			{
				sum += Get_Distance(centroid->get_Number_Table(), current->get_Number_Table());
				current = current->get_next();
			}
		}
	}
	return sum;
}


double ClusterCosine::Objective_Function_New_Centroid(int ClusterNumber, int ItemClusterNumber, Node<double> *new_centroid)
{
	double sum = 0;
	double distance = 0;
	Node<double> *previous_centroid;
	Node<double> *centroid;
	Node<double> *second_best_Centroid;
	Node<double> *current;
	List<double> *items_list;
	for(int i =0; i <= k -1 ; i++)
	{
		if (i == ClusterNumber)
		{
			previous_centroid = cluster[i].get_Centroid();
			items_list = cluster[i].get_List();		//there is not here the cluster[i].get_Centroid(), so will calculate it later
			
			current = items_list->get_start();
			while(current != NULL)
			{
				second_best_Centroid = get_second_best_Centroid(ClusterNumber, current);
				if (Get_Distance(new_centroid->get_Number_Table(), current->get_Number_Table()) <=  Get_Distance(second_best_Centroid->get_Number_Table(), current->get_Number_Table()))
				{
					sum += Get_Distance(new_centroid->get_Number_Table(), current->get_Number_Table());
				}
				else
				{
					sum += Get_Distance(second_best_Centroid->get_Number_Table(), current->get_Number_Table());
				}
				current = current->get_next();
			}
			
			
			//this for the previous centroid m that is faced as a non-centroid
			second_best_Centroid = get_second_best_Centroid(ClusterNumber, previous_centroid);				//the first best centroid is itself, so find the second from the other k-1 clusters
			if (Get_Distance(new_centroid->get_Number_Table(), previous_centroid->get_Number_Table()) <=  Get_Distance(second_best_Centroid->get_Number_Table(), previous_centroid->get_Number_Table()))			//Compare second best with new centroid t
			{
				sum += Get_Distance(new_centroid->get_Number_Table(), previous_centroid->get_Number_Table());
			}
			else
			{
				sum += Get_Distance(second_best_Centroid->get_Number_Table(), previous_centroid->get_Number_Table());
			}
		}
		else
		{
			centroid = cluster[i].get_Centroid();
			items_list = cluster[i].get_List();		//there is not here the cluster[i].get_Centroid(), so will calculate it later
			
			current = items_list->get_start();
			while(current != NULL)
			{	
				if (Get_Distance(new_centroid->get_Number_Table(), current->get_Number_Table()) <=  Get_Distance(centroid->get_Number_Table(), current->get_Number_Table()))
				{
					sum += Get_Distance(new_centroid->get_Number_Table(), current->get_Number_Table());
				}
				else
				{
					sum += Get_Distance(centroid->get_Number_Table(), current->get_Number_Table());
				}
				current = current->get_next();
			}
			if (i == ItemClusterNumber)									//Dont calculate the distance between new centroid and its centroid before the swap, only for Clarans
				sum -= Get_Distance(centroid->get_Number_Table(), new_centroid->get_Number_Table());
		}
	}
	return sum;
}

bool ClusterCosine::IsCentroid(Node<double> *node)
{
	for(int i=0; i <= k-1; i++)
	{
		if (node->get_Number() == (cluster[i].get_Centroid())->get_Number())
			return true;
	}
	return false;
}

Node<double> * ClusterCosine::get_second_best_Centroid(int NumCluster, Node<double> *node)
{
	int min_index = 0;
	double min = 10000;
	double dist = 0;
	for(int i = 0; i <= k -1 ; i++)
	{
		if (i != NumCluster)
		{
			dist = Get_Distance((cluster[i].get_Centroid())->get_Number_Table(), node->get_Number_Table());
			if (dist < min)
			{
				min_index = i;
				min = dist;
			}
		}
	}
	return cluster[min_index].get_Centroid();
}


Node<double> * ClusterCosine::Cluster_Medoid_Calculation(int NumCluster)
{
	List<double> *items_list = cluster[NumCluster].get_List();	
	Node<double> *current;
	Node<double> *centroid = cluster[NumCluster].get_Centroid();
	if (items_list->is_empty())
	{
		cout << "Problem: empty list" << endl;
		return NULL;	
	}
	
	double min;
	int min_index = 0;
	double *Sum = new double[items_list->get_ListSize() + 1];
	
	for(int i = 0; i <= items_list->get_ListSize(); i++)
		Sum[i] = 0;
	
	int line = 0;
	Node<double> *current_node_line = items_list->get_start();	//current_node_line is t	
	Node<double> *current_node_column;							//current_node_line is i
	while(current_node_line != NULL)	
	{
		current_node_column = items_list->get_start();
		while(current_node_column != NULL)
		{
			Sum[line] += Get_Distance(current_node_line->get_Number_Table(), current_node_column->get_Number_Table());
			current_node_column = current_node_column->get_next();
		}
		Sum[line] += Get_Distance(current_node_line->get_Number_Table(), centroid->get_Number_Table());				//+ the distance from the center of the cluster
		line++;
		current_node_line = current_node_line->get_next();
	}
	
	//the same thing for the centoid. The last item in array Sum is the Distances for centroid
	current_node_column = items_list->get_start();
	while(current_node_column != NULL)
	{
		Sum[line] += Get_Distance(centroid->get_Number_Table(), current_node_column->get_Number_Table());
		current_node_column = current_node_column->get_next();
	}
	
	//find the min in Sum
	min = Sum[0];
	for(int i = 1; i <= items_list->get_ListSize() ; i++)		//for(int i = 1; i <= items_list->get_ListSize() + 1 - 1; i++)
	{
		if (Sum[i] < min)
		{
			min = Sum[i];
			min_index = i;
		}
	}
	
	if (min_index == items_list->get_ListSize())				//medoid t is already centroid
		return centroid;
	else
		return (items_list->get_random_Node(min_index));		//find this item and return it
	
	delete [] Sum;	
}


bool ClusterCosine::Keep_Update()
{
	for(int i = 0; i <= k - 1; i++)
		if (Objective_Function_Updated[i])
			return true;
	return false;
}


void ClusterCosine::Update_Lloyd_Change_All_medoids()
{
	float new_ObjFunction_Value = 0;
	int ItemNumber = 0;
	int ItemClusterNumber = 0;
	Node<double> *medoid_centroid;
	Node<double> *new_centroid;
	Node<double> *previous_centroid;
	List<double> *temporary_list;
	temporary_list = new List<double>[k];
	
	for(int i =0 ; i<= k - 1; i++)
		Objective_Function_Updated[i] = false;
	
	for(int i =0 ; i<= k - 1; i++)
	{
		//Calculate new centroid t
		temporary_list[i].set_start(cluster[i].get_Centroid());
		medoid_centroid = Cluster_Medoid_Calculation(i);
		new_centroid = medoid_centroid;
		if (new_centroid != cluster[i].get_Centroid() && new_centroid != NULL)				
		{
			//Calculate New Objective Function
			new_ObjFunction_Value = Objective_Function_New_Centroid(i, i ,new_centroid);
			
			//Swap
			cluster[i].Swap(new_centroid);
		}	
	}		
	
	if (optionAssign == '1')				//chosen Assignment
		PAM_Assignment();
	else									//chosen Assignment
		LSH_Assignment();
	new_ObjFunction_Value = Objective_Function();
	
	if (new_ObjFunction_Value < Objective_Function_Value)			//if J' < J
	{							//swap -> set new centroid, put previous centroid at items list
		Objective_Function_Updated[0] = true;					
		Objective_Function_Value = new_ObjFunction_Value;
	}
	else
	{
		for(int i =0 ; i<= k - 1; i++)
		{
			previous_centroid = temporary_list[i].get_start();
			ItemNumber = previous_centroid->get_Number();
			//Find item's cluster
			int j=0;
			while(j <= k-1)
			{
				new_centroid = (cluster[j].get_List())->get_Node(ItemNumber);
			
				if (new_centroid != NULL)													//Found it
				{
					ItemClusterNumber = j;
					j=k;
				}
				j++;
			}
			Clarans_Swap(i, ItemClusterNumber, previous_centroid);
		}
		if (optionAssign == '1')				//chosen Assignment
			PAM_Assignment();
		else									//chosen Assignment
			LSH_Assignment();
	}
}


void ClusterCosine::Update_Lloyd()
{
	float new_ObjFunction_Value = 0;
	Node<double> *medoid_centroid;
	Node<double> *new_centroid;
	
	for(int i =0 ; i<= k - 1; i++)
		Objective_Function_Updated[i] = false;
	
	for(int i =0 ; i<= k - 1; i++)
	{
		//Calculate new centroid t
		medoid_centroid = Cluster_Medoid_Calculation(i);
		new_centroid = medoid_centroid;
		if (new_centroid != cluster[i].get_Centroid() && new_centroid != NULL)				
		{
			//Calculate New Objective Function
			new_ObjFunction_Value = Objective_Function_New_Centroid(i, i ,new_centroid);
			
			//Swap
			if (new_ObjFunction_Value < Objective_Function_Value)			//if J' < J
			{
				cluster[i].Swap(new_centroid);							//swap -> set new centroid, put previous centroid at items list
				Objective_Function_Updated[i] = true;					
				Objective_Function_Value = new_ObjFunction_Value;
				if (optionAssign == '1')				//chosen Assignment
					PAM_Assignment();
				else									//chosen Assignment
					LSH_Assignment();
			}
		}		
	}
}



void ClusterCosine::Update_Clarans()
{
	int Qnum = 0, nonCentroid_number = 0;
	//plh8ari8mos tou Q (|Q|)
	for(int i=0; i <= k -1; i++)
		nonCentroid_number += (cluster[i].get_List())->get_ListSize();
	Qnum = nonCentroid_number * k * (0.12);
	if (Qnum < 250)
		Qnum = 250;
	
	int random_x;
	int Q[Qnum][2];
	
	for(int i = 0; i <= Qnum - 1; i++)
	{
		random_x = uniform_distribution(0, (nonCentroid_number+k)*k - 1);
		Q[i][0] = random_x % k;
		Q[i][1] = floor(random_x / k);	
		//cout << "(" << Q[i][0] << "," << Q[i][1] << ")" << '\t';
	}
	
	Node<double> *new_centroid;
	float new_ObjFunction_Value;
	int  ClusterNumber = 0, ItemClusterNumber = 0, ItemNumber = 0;
	for(int index = 0; index <= Qnum - 1; index++)
	{
		ClusterNumber = Q[index][0];			//set cluster from Q[index][0]
		ItemNumber = Q[index][1];				//set item from Q[index][1]
		//Find item's cluster
		int j=0;
		while(j <= k-1)
		{
			new_centroid = (cluster[j].get_List())->get_Node_Table_Number(ItemNumber);
			if (new_centroid == NULL)						//If it is not at the list , then it could be the centroid of the cluster
				if (ItemNumber == (cluster[j].get_Centroid())->get_Number_Table())			//It is the centroid
					new_centroid = cluster[j].get_Centroid();
			
			if (new_centroid != NULL)													//Found it
			{
				ItemClusterNumber = j;
				j=k;
			}
			j++;
		}
		
		if (new_centroid != cluster[ClusterNumber].get_Centroid() && new_centroid != NULL)		//if it is no changes			//	new_centroid != cluster[new_centroid_cluster_number].get_Centroid() && new_centroid != NULL
		{
			new_ObjFunction_Value = Objective_Function_New_Centroid(ClusterNumber, ItemClusterNumber, new_centroid);			//Calculate New Objective Function
			
			if (new_ObjFunction_Value < Objective_Function_Value)			//if J' < J
			{
				Clarans_Swap(ClusterNumber, ItemClusterNumber, new_centroid);							//swap -> set new centroid, put previous centroid at items list				
				Objective_Function_Value = new_ObjFunction_Value;										//Update Objective Function
				if (optionAssign == '1')			//chosen Assignment 
					PAM_Assignment();
				else								//chosen Assignment 
					LSH_Assignment();	 
			}
		}
	}	
}


void ClusterCosine::Update_Clarans_One_Swap()
{
	int Qnum = 0, nonCentroid_number = 0;
	//plh8ari8mos tou Q (|Q|)
	for(int i=0; i <= k -1; i++)
		nonCentroid_number += (cluster[i].get_List())->get_ListSize();
	Qnum = nonCentroid_number * k * (0.12);
	if (Qnum < 250)
		Qnum = 250;
	
	int random_x;
	int Q[Qnum][3];
	
	for(int i = 0; i <= Qnum - 1; i++)
	{
		random_x = uniform_distribution(0, (nonCentroid_number+k)*k - 1);
		Q[i][0] = random_x % k;
		Q[i][1] = floor(random_x / k);	
		//cout << "(" << Q[i][0] << "," << Q[i][1] << ")" << '\t';
	}
	
	bool is_a_centroid = false;
	Node<double> *new_centroid;
	float new_ObjFunction_Value; 
	int ClusterNumber = 0, ItemClusterNumber = 0, ItemNumber = 0;
	for(int index = 0; index <= Qnum - 1; index++)
	{
		is_a_centroid = false;
		ClusterNumber = Q[index][0];			//set cluster from Q[index][0]
		ItemNumber = Q[index][1];				//set item from Q[index][1]
		//Find item's cluster
		int j=0;
		while(j <= k-1)
		{
			new_centroid = (cluster[j].get_List())->get_Node_Table_Number(ItemNumber);
			if (new_centroid == NULL)
			{											//If it is not at the list , then it could be the centroid of the cluster
				if (ItemNumber == (cluster[j].get_Centroid())->get_Number_Table())			//It is the centroid
				{
					new_centroid = cluster[j].get_Centroid();
					is_a_centroid = true;
				}
			}
			
			if (new_centroid != NULL)													//Found it
			{
				ItemClusterNumber = j;
				j=k;
			}
			j++;
		}
		
		if (is_a_centroid)		//if it is no changes			//	new_centroid != cluster[new_centroid_cluster_number].get_Centroid() && new_centroid != NULL
			Q[index][2] = 0;
		else
			Q[index][2] = Objective_Function_New_Centroid(ClusterNumber, ItemClusterNumber, new_centroid);			//Calculate New Objective Function
	}
	
	int min = Q[0][2], min_index = 0;
	for(int i=0; i < Qnum - 1; i++)
	{
		if (Q[i][2] < min && Q[i][2] != 0)
		{
			min = Q[i][2];
			min_index = i;
		}
	}
	
	ClusterNumber = Q[min_index][0];			//set cluster from Q[index][0]
	ItemNumber = Q[min_index][1];				//set item from Q[index][1]
	int j=0;
	while(j <= k-1)
	{
		new_centroid = (cluster[j].get_List())->get_Node_Table_Number(ItemNumber);
		if (new_centroid == NULL)													//If it is not at the list , then it could be the centroid of the cluster
			if (ItemNumber == (cluster[j].get_Centroid())->get_Number_Table())			//It is the centroid
				new_centroid = cluster[j].get_Centroid();
		
		if (new_centroid != NULL)													//Found it
		{
			ItemClusterNumber = j;
			j=k;
		}
		j++;
	}
	new_ObjFunction_Value = Objective_Function_New_Centroid(ClusterNumber, ItemClusterNumber, new_centroid);			//Calculate New Objective Function
	
	if (new_ObjFunction_Value < Objective_Function_Value)			//if J' < J
	{
		Clarans_Swap(ClusterNumber, ItemClusterNumber, new_centroid);							//swap -> set new centroid, put previous centroid at items list				
		Objective_Function_Value = new_ObjFunction_Value;										//Update Objective Function
		if (optionAssign == '1')			//chosen Assignment 
			PAM_Assignment();
		else								//chosen Assignment 
			LSH_Assignment();	 
	}
}


void ClusterCosine::Clarans_Swap(int ClusterNumber, int ItemClusterNumber, Node<double> *new_centroid)
{
	Node<double> *previous_centroid = cluster[ClusterNumber].get_Centroid();
	Node<double> *current_node = (cluster[ItemClusterNumber].get_List())->get_start();
	Node<double> *previous_node = (cluster[ItemClusterNumber].get_List())->get_start();
	if (previous_centroid != new_centroid)
	{
		while(current_node != NULL)
		{
			//find t in list
			if (current_node == new_centroid)
			{
				if (previous_node != current_node)
				{
					previous_node->set_next(current_node->get_next());		//Delete it from list
				}
				else				//first item at list
				{
					(cluster[ItemClusterNumber].get_List())->set_start(current_node->get_next());				//Delete it from list
				}
				(cluster[ItemClusterNumber].get_List())->set_List_size((cluster[ItemClusterNumber].get_List())->get_ListSize() - 1);
				break;
			}
			previous_node = current_node;
			current_node = current_node->get_next();
		}
		//put past_centroid at the end of its list (we dont care where it will be put at)
		previous_centroid->set_notCentroidValue();
		(cluster[ClusterNumber].get_List())->Insert_Node_End(previous_centroid);
	
		//set t as a centroid
		new_centroid->set_next(NULL);
		cluster[ClusterNumber].set_Centroid(new_centroid);
	}
}

//---------------------------------------------------------------------------CLARA---------------------------------------------------------------------------
void ClusterCosine::Clara_Function()
{
//	cout << "Clara" << endl;
	
	int NumItems = 40 + 2*k;
	Number_of_Items = NumItems;
	Clara_New_Sample(NumItems);
	
	//Initialization
	Cosine_Cluster_Initialization('1');
	
	//Update
	PAM_Update();
	
//	printClusters();
}


void ClusterCosine::Clara_New_Sample(int NumItems)
{
	int random = 0;
	int count = 0;
	
	clara_list = new List<double>;
	Node<double> *current;
	Node<double> *previous;
	Node<double> *node;
	List<double> *new_list;
	new_list = new List<double>;
	
	for(int i=0; i <= NumItems-1; i++)
	{
		count = 0;
		do
		{
			random = uniform_distribution(0, all_items_list->get_ListSize() - 1);
			current = all_items_list->get_random_Node(random);
		}while(clara_list->get_Node(current->get_Number()) != NULL);
		
		node = new Node<double>(current->get_data(), current->get_size(), current->get_Number(), i, 0);	
		clara_list->Insert_Node_End(node);
	}
	
	Node<double> *temp;
	int list_size_temp = 0;
	
	temp = clara_list->get_start();
	list_size_temp = clara_list->get_ListSize();
	
	clara_list->set_start(all_items_list->get_start());
	clara_list->set_List_size(all_items_list->get_ListSize());
	
	all_items_list->set_start(temp);
	all_items_list->set_List_size(list_size_temp);
	
}


void ClusterCosine::PAM_Update()
{
	float new_ObjFunction_Value = 0;
	Node<double> *centroid;
	Node<double> *current;
	
	for(int i=0; i <= k-1; i++)
	{
		centroid = cluster[i].get_Centroid();
		for(int j=0; j <= k -1; j++)
		{
			current = (cluster[j].get_List())->get_start();
			while(current != NULL)
			{
				new_ObjFunction_Value = Objective_Function_New_Centroid(i, j, current);
				if(new_ObjFunction_Value < Objective_Function_Value)
				{
					Clarans_Swap(i, j, current);													//swap -> set new centroid, put previous centroid at items list				
					Objective_Function_Value = new_ObjFunction_Value;										//Update Objective Function
					PAM_Assignment();
				}
				current = current->get_next();
			}
		}
	}
}

//---------------------------------------------------------------------------Silhouette---------------------------------------------------------------------------
void ClusterCosine::Silhouette(std::ofstream& outfile)
{
	double *S = new double[Number_of_Items];
	double *A = new double[Number_of_Items];
	double *B = new double[Number_of_Items];
	double *average_S = new double[k];
	double sum = 0;
	int count =0;
	
	Node<double> *item;
	Node<double> *current;
	Node<double> *centroid;
	List<double> *cluster_list;
	for(int i=0; i <= k - 1; i++)
	{
		cluster_list = cluster[i].get_List();
		centroid = cluster[i].get_Centroid();
		item = cluster_list->get_start();
		while (item != NULL)
		{
			sum = 0;
			count = 0;
			current = cluster_list->get_start();
			while(current != NULL)
			{
				sum += Get_Distance(item->get_Number_Table(), current->get_Number_Table());
				count++;
				current = current->get_next();
			}
			sum += Get_Distance(item->get_Number_Table(), centroid->get_Number_Table());
			count++;
			A[item->get_Number_Table()] = sum/count;
			item = item->get_next();
		}
		item = centroid;
		sum = 0;
		count = 0;
		current = cluster_list->get_start();
		while(current != NULL)
		{
			sum += Get_Distance(item->get_Number_Table(), current->get_Number_Table());
			count++;
			current = current->get_next();
		}
		
		if (count != 0)
			A[item->get_Number_Table()] = sum/count;
		else
			A[item->get_Number_Table()] = 0;		
	}
		
	int cluster_number = 0;
	Node<double> *second_best_centroid;
	List<double> *second_cluster_list;
	
	for(int i=0; i <= k - 1; i++)
	{
		cluster_list = cluster[i].get_List();
		centroid = cluster[i].get_Centroid();
		item = cluster_list->get_start();
		while (item != NULL)
		{
			second_best_centroid = get_second_best_Centroid(i, item);
			for(int j = 0; j <= k - 1; j++)
				if (second_best_centroid->get_Number() == (cluster[j].get_Centroid())->get_Number())
					cluster_number = j;
				
			second_cluster_list = cluster[cluster_number].get_List();
			sum = 0;
			count = 0;
			current = second_cluster_list->get_start();
			while(current != NULL)
			{
				sum += Get_Distance(item->get_Number_Table(), current->get_Number_Table());
				count++;
				current = current->get_next();
			}
			sum += Get_Distance(item->get_Number_Table(), second_best_centroid->get_Number_Table());
			count++;
			B[item->get_Number_Table()] = sum/count;
			item = item->get_next();
		}
		
		item = centroid;
		second_best_centroid = get_second_best_Centroid(i, item);
		for(int j = 0; j <= k - 1; j++)
			if (second_best_centroid->get_Number() == (cluster[j].get_Centroid())->get_Number())
				cluster_number = j;
				
		second_cluster_list = cluster[cluster_number].get_List();
		sum = 0;
		count = 0;
		current = second_cluster_list->get_start();
		while(current != NULL)
		{
			sum += Get_Distance(item->get_Number_Table(), current->get_Number_Table());
			count++;
			current = current->get_next();
		}
		if (count != 0)
			B[item->get_Number_Table()] = sum/count;
		else
			B[item->get_Number_Table()] = 0;		
	}
	
	double max;
	for(int i=0; i <= k - 1; i++)
	{
		cluster_list = cluster[i].get_List();
		centroid = cluster[i].get_Centroid();
		item = cluster_list->get_start();
		while (item != NULL)
		{
			if (A[item->get_Number_Table()] < B[item->get_Number_Table()])
				max = B[item->get_Number_Table()];
			else
				max = A[item->get_Number_Table()];
			if (max != 0)
				S[item->get_Number_Table()] = (B[item->get_Number_Table()] - A[item->get_Number_Table()]) / max;
			else
				S[item->get_Number_Table()] = 0;
			item = item->get_next();
		}
		item = centroid;
		if (A[item->get_Number_Table()] < B[item->get_Number_Table()])
			max = B[item->get_Number_Table()];
		else
			max = A[item->get_Number_Table()];
		if (max != 0)
			S[item->get_Number_Table()] = (B[item->get_Number_Table()] - A[item->get_Number_Table()]) / max;
		else
			S[item->get_Number_Table()] = 0;		
	}
	
	for(int i=0; i <= k - 1; i++)
	{
		sum = 0;
		count  = 0;
		cluster_list = cluster[i].get_List();
		centroid = cluster[i].get_Centroid();
		item = cluster_list->get_start();
		while (item != NULL)
		{
			sum += S[item->get_Number_Table()];
			count++;
			item = item->get_next();
		}
		item = centroid;
		sum += S[item->get_Number_Table()];	
		count++;
		if (count != 0)
			average_S[i] = sum / count;
		else
			average_S[i] = 0;
	}
	
	double S_total = 0;
	Silhouette_table = new double[k+1];
	//cout << endl << "Sillouette" << endl;
	for(int i=0; i <= k - 1; i++)
	{
		Silhouette_table[i] = average_S[i];
		S_total += average_S[i];
	}
	Silhouette_table[k] = S_total / k;
	
	delete [] average_S;
	delete [] S;
	delete [] A;
	delete [] B;
}


/*

void Euclidean::Euclidean_LSH(char *line, ofstream& output, int Number, char option){  //Dont care to create one node for all hashtables 
	double time_LSH, time_ALL;
	double dist;
	int line_count = 0;
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
	//	cout << setprecision(12) << coordinate[i] << "  " << i << endl;
		line_count++;
	}
		
	node = new Node<double>(coordinate, dims, Number);
	
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
		hashtable[i].HashTable_LSH(node, qlist, random);
		final_list_LSH->Insert_List(qlist, NumberItems);
	}
	dist = final_list_LSH->Euclidean_Distance(node, R, c, output, 0);			//0 for LSH
	output << "distanceLSH: ";
	output << dist;
	output << '\n';
	
	time_LSH = (double)((clock() - begin_time_LSH ) /  (double)CLOCKS_PER_SEC);
	
	//Search closer neighbor for ALL
	const clock_t begin_time_ALL = clock();							//time
	hashtable[0].HashTable_Search_All(node, final_list_ALL);
	
	dist = final_list_ALL->Euclidean_Distance(node, R, c, output, 1);			//1 for ALL
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



void Euclidean::printList(){
	for (int i=0; i <= L - 1; i++)
	{
		cout << "Hashtable " << i << endl;
		hashtable[i].printList();
	}
}
*/


