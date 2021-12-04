#ifndef TWOTHREE_H
#define TWOTHREE_H

struct TwoThree{
	
	int type; // number of childs: 0, 2, 3, 4
	TwoThree* parent;
	
	//leaf vars
	int val;
	TwoThree* next; // next leaf
	TwoThree* prev; // previous leaf
	
	// internal node vars
	TwoThree** childs;
	int* max_values;
	
};

int testTwoThree(std::string filename);
void printTree(TwoThree* p, int depth);
bool checkLinksDuality(TwoThree* p);
bool checkTreeLinks(TwoThree* parent, TwoThree* p);
bool checkLeafListLinks(TwoThree* l, TwoThree* r);
TwoThree* emptyLeaf(int val);
TwoThree* emptyNode();
TwoThree* search(TwoThree* root, int val, int& child_index);
void appendLeafInChildList(TwoThree* parent, TwoThree* p, int child_index);
void insert(TwoThree*& root, int val);
void balance(TwoThree*& root, TwoThree* start_node);
void separateNode(TwoThree* parent, TwoThree* p);
void appendNodeInChildList(TwoThree* parent, TwoThree* p, int child_index);
void deleteKey(TwoThree*& root, int val);
void deleteNode(TwoThree*& root, TwoThree* p, int child_index);
TwoThree* getNeighbor(TwoThree* p, int& child_index, int& neighbor_child_index);
int getChildIndex(TwoThree* p);
void updateLeafOrder(TwoThree* p);
void freeNodeMemo(TwoThree* p);
void updateMaxValues(TwoThree* parent, TwoThree* p);
TwoThree* nextNode(TwoThree* root, int val);
TwoThree* prevNode(TwoThree* root, int val);

#endif