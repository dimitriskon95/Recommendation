#ifndef ValidationNode_H
#define ValidationNode_H

class ValidationNode
{
	private:
		int userID;
		int item;
		int rating;
		ValidationNode *next;
	public:
		ValidationNode(int, int, int);
		~ValidationNode();
		
		ValidationNode* get_next();
		void set_next(ValidationNode *);
		
		int get_userID();
		int get_item();
		int get_rating();
		
		void printNode();
};	

#endif
