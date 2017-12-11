#ifndef ClusterHamming_H
#define ClusterHamming_H

#include <fstream>
#include <iostream>
#include "Cluster.h"
#include "List.h"
#include "Node.h"
#include "Validation.h"

class ClusterHamming
{
	private:
		int k;			//number of clusters
		int number_of_bits;
		int Pnumber;	//plh8os twn kontinoterwn geitonwn pou 8a elegx8oun
		
		int hash_functions;
		int hash_tables;
		int clarans_franction;
		int clarans_iterations;
		
		int Number_of_Items;
		bool *Objective_Function_Updated;
		long int Objective_Function_Value;
		char optionInit;
		char optionAssign;
		char optionUpdate;
		char metric;
		
		double **Distance_Table;
		List<char> *usersList;
		List<char> *all_items_list;
		List<char> *clara_list;
		Cluster<char> *cluster;
		
		Validation *validation;
		
		double *Silhouette_table;
	public:
		ClusterHamming(int, int, int, int, int, int);
		~ClusterHamming();
		
		int get_ClusterNumber();
		int get_BitsNumber();
		
		void ClusterHamming_Menu(std::ifstream& infile,std::ifstream& cfile, std::ofstream& outfile);
		
		void Lloyd_Menu(std::ofstream& outfile);
		void Clarans_Menu(std::ofstream& outfile);
		void Clara_Menu(std::ofstream& outfile);
		
		void Hamming_Reader(char *, int);
		
		void Hamming_Cluster_Initialization(char option = '1'); //default kmedoids++
		
		void Distance_Table_Calculation();
		double Get_Distance(int , int );
		
		void KmedoidsInitialization();
		void ConcentrateInitialization();
		
		int Cluster_Hamming_Distance(Node<char>* cnode, Node<char>* inode);
		
		void Assignment_Initialization();
		
		void Assignment();
		
		void PAM_Assignment();
		void LSH_Assignment();
		
		bool LSH_Keep_Update(bool *);
		
		bool IsCentroid(Node<char> *);
		
		long int Objective_Function();
		
		int Objective_Function_New_Centroid(int ClusterNumber, int ItemClusterNumber, Node<char> *new_centroid);
		
		void Update_Lloyd();
		
		void Update_Lloyd_Change_All_medoids();
		
		void Update_Clarans();
		
		void Update_Clarans_One_Swap();
		
		Node<char> * Cluster_Medoid_Calculation(int);
		bool Keep_Update();
		
		Node<char> * get_second_best_Centroid(int, Node<char> *);
		
		void Clarans_Swap(int new_cluster_number, int new_centroid_cluster_number, Node<char> *new_centroid);
		
		void printClusters();
		
		void Clara_Function();
		
		void Clara_New_Sample(int NumItems);
		
		void PAM_Update();
		
		void Silhouette(std::ofstream& outfile);
		
		void Hamming_User_Reader(std::ifstream& infile, std::ifstream& tmp_file);
		void set_Pnumber(char *line);
		char *Hamming_Clustering(Node<char> *node, std::ofstream& outfile);
		Node<char> * Hamming_close_Neighbor(Node<char> *node, List<char> *ulist);
		int Similarity(Node<char>* user, Node<char>* node);
		
		void Hamming_Validation_Menu(std::ifstream& infile, std::ifstream& tmp_file, std::ofstream& outfile, int usersNumber);
};

#endif
