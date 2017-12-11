#include <iostream>
#include "ValidationNode.h"
#include <iomanip>

using namespace std;

ValidationNode::ValidationNode(int u, int i, int r)
{
	//cout << "A Node was created." << endl;
	userID = u;
	item = i;
	rating = r;
	next = NULL;
}

ValidationNode::~ValidationNode()
{
	
}

ValidationNode* ValidationNode::get_next()
{
	return next;
}

void ValidationNode::set_next(ValidationNode *node)
{
	next = node;
}

void ValidationNode::printNode()
{
	cout << "UserID : " << userID << "\tItem : " << item << "\tRating " << rating << endl;	
}

int ValidationNode::get_userID()
{
	return userID;
}

int ValidationNode::get_item()
{
	return item;
}

int ValidationNode::get_rating()
{
	return rating;
}

