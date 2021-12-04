#ifndef AVL_H
#define AVL_H

struct Node{
	int val;
	int rank;
	Node* left;
	Node* right;
	Node* parent;
};

extern std::map<int, std::pair<std::pair<double, double>, std::pair<double, double>>>* sects;

int testAVL(std::string filename);

void printTree(Node* p, int depth);
bool checkLinksDuality(Node* p);
bool checkTreeLinks(Node* parent, Node* p);
bool checkBalance(Node* p);
Node* Min(Node* p);
Node* Max(Node* p);
Node* next(Node* p);
Node* previous(Node* p);
void updateRank(Node* p);
Node* insert(Node*& root, int key);
Node* search(Node* p, int key, int& child_flag);
Node* search(Node* p, int key);
Node* sls(Node* a); // small left spin
Node* srs(Node* a); // small right spin
Node* bls(Node* a); // big left spin
Node* brs(Node* a); // big right spin
void balance(Node*& root, Node*& x);
void swap(Node*& root, Node* p1, Node* p2);
void deleteLeaf(Node*& root, Node*& x);
void deleteNode(Node*& root, Node*& x);
void setSections(std::map<int, std::pair<std::pair<double, double>, std::pair<double, double>>>* sections);
double getRealKey(int val, double x, double y);


#endif