#include <iostream>
#include <fstream>
#include <math.h>
#include <map>
#include <utility>
#include <string>
#include "AVL.h"

std::map<int, std::pair<std::pair<double, double>, std::pair<double, double>>>* sects;


using namespace std;

int testAVL(string filename){
	Node* root = 0;
	ifstream commands (filename);
	char cmd;
	int cmd_val;
	while(commands>>cmd){
		switch(cmd)
		{
			case 'i':{
				commands>>cmd_val;
				insert(root, cmd_val);
				break;
			}
			case 'p':{
				cout<<"<"<<endl;
				printTree(root, 0);
				cout<<">"<<endl;
				break;
			}
			case 'd':{
				commands>>cmd_val;
				Node* p = search(root, cmd_val);
				deleteNode(root, p);
				break;
			}
			case 'x':{ // neXt
				commands>>cmd_val;
				Node* p = search(root, cmd_val);
				p = next(p);
				if(p)
					cout<<p->val<<endl;
				break;
			}
			case 'v':{ // preV
				commands>>cmd_val;
				Node* p = search(root, cmd_val);
				p = prev(p);
				if(p)
					cout<<p->val<<endl;
				break;
			}
			case 'c':{
				if (!checkLinksDuality(root)){ 
					//cout << "bad" << endl;
					return 1;
				}
				break;
			}
			default:
				cout << "No such command\n"; break;
			
		}
	}
	commands.close();
	return 0;
}

bool checkLinksDuality(Node* p){
	if (!p) return true;
	return checkTreeLinks(p->parent, p);
}

bool checkTreeLinks(Node* parent, Node* p){
	bool flag = true;
	flag = flag && checkBalance(p);
	if (p->left) {
		flag = flag && checkTreeLinks(p, p->left);
	}
	if (p->right){
		flag = flag && checkTreeLinks(p, p->right);
	}
	return flag && (p->parent == parent);
}

bool checkBalance(Node* p){
	if (!p->left && !p->right) return true;
	if (p->left && p->right){
		int diff = p->left->rank - p->right->rank;
		return (diff * diff <= 1); 
	}
	if (p->left) {
		return ((p->left->rank == 0)? true:false);
	}
	if (p->right) {
		return ((p->right->rank == 0)? true:false);
	}
	return false; // not happens
}

void printTree(Node* p, int depth){
	if (!p) return;
	if (p->right){
		printTree(p->right, depth + 1);
	}
	for (int i = 0; i < depth; ++i) cout << '\t';
	cout << p->val <<'('<<p->rank<<')'<< endl;
	if (p->left){
		printTree(p->left, depth + 1);
	}
}

Node* Min(Node* p){
	if (p)
		while (p->left)
			p = p->left;	
	return p;
}

Node* Max(Node* p){
	if (p)
		while (p->right)
			p = p->right;
	return p;
}


Node* next(Node* p){
	if (p){
		if (p->right)
			return Min(p->right);
		else {
			Node* parent = p;
			while (parent->parent != parent && parent->left != p){
				p = parent;
				parent = p->parent;
			}
			if (parent->left == p) return parent;
			else return 0;
		}
	}
	return 0;
}

Node* previous(Node* p){
	if (p){
		if (p->left)
			return Max(p->left);
		else{
			Node* parent = p;
			while(parent->parent != parent && parent->right != p){
				p = parent;
				parent = p->parent;
			}
			if (parent->right == p) return parent;
			else return 0;
		}
	}
	return 0;
}

void updateRank(Node* p){
	int r = -1;
	if (p->left)
		r = p->left->rank;
	if (p->right)
		r = max(r, p->right->rank);
	p->rank = ++r;
}

Node* insert(Node*& root, int key){
	int child = -1;
	Node* x = search(root, key, child);
	Node* new_node = new Node();
	if (child >= 0 || x == 0){
		new_node->val = key;
		new_node->left = 0;
		new_node->right = 0;
		new_node->rank = -1;
		
		if (x == 0) {
			root = new_node;
			new_node->parent = new_node;
		}
		else if (child == 0) {
			x->left = new_node;
			new_node->parent = x;
		}
		else if (child == 1){
			x->right = new_node;
			new_node->parent = x;
		}
		// update ranks
		x = new_node;
		while (x->parent != x){
			balance(root, x);
			updateRank(x);
			x = x->parent;
		}
		balance(root, x);
		updateRank(x);
	}
	return new_node;
}

Node* search(Node* p, int key, int& child_flag){
	Node* parent = p;
	while(p){
		if (key - p->val > 0) {
			parent = p;
			p = p->right; 
			child_flag = 1;
			continue;
		}
		else if (key - p->val < 0) {
			parent = p;
			p = p->left;
			child_flag = 0;
			continue;
		}
		child_flag = -1;
		return p;
	}
	return parent;
}

Node* search(Node* p, int key){
	while(p){
		if (key > p->val) {
			p = p->right;
			continue;
		}
		else if (key < p->val) {
			p = p->left;
			continue;
		}
		else return p;
	}
	return 0;
}

Node* sls(Node* a){
	Node* b = a->right;
	Node* bl = b->left;
	
	a->parent = b;
	b->left = a;
	a->right = bl;
	if (bl) bl->parent = a;
	
	updateRank(a);
	updateRank(b);
	return b;
}

Node* srs(Node* a){
	Node* b = a->left;
	Node* br = b->right;
	
	a->parent = b;
	b->right = a;
	a->left = br;
	if (br) br->parent = a;
	
	updateRank(a);
	updateRank(b);
	return b;
}

Node* bls(Node* a){
	Node* b = a->right;
	Node* c = b->left;
	Node* cl = c->left;
	Node* cr = c->right;
	
	a->parent = c;
	c->left = a;
	b->parent = c;
	c->right = b;
	
	a->right = cl;
	if (cl) cl->parent = a;
	b->left = cr;
	if (cr) cr->parent = b;
	
	updateRank(a);
	updateRank(b);
	updateRank(c);
	
	return c;
}

Node* brs(Node* a){
	Node* b = a->left;
	Node* c = b->right;
	Node* cl = c->left;
	Node* cr = c->right;
	
	a->parent = c;
	c->right = a;
	b->parent = c;
	c->left = b;
	
	b->right = cl;
	if (cl) cl->parent = b;
	a->left = cr;
	if (cr) cr->parent = a;
	
	updateRank(a);
	updateRank(b);
	updateRank(c);
	
	return c;
}

void balance(Node*& root, Node*& x){
	if (!x) return;
	if (!x->left && !x->right) return;
	bool changeRoot = false;
	if (root == x) changeRoot = true;
	Node* p = x;
	Node* parent = x->parent;
	int child = -1;
	if (parent->left == x) child = 0;
	if (parent->right == x) child = 1;
	int lh = -1, rh = -1;
	if (p->left) lh = p->left->rank;
	if (p->right) rh = p->right->rank;
	if (rh - lh == 2){ // left spin
		p = p->right;
		lh = -1, rh = -1;
		if (p->left) lh = p->left->rank;
		if (p->right) rh = p->right->rank;
		if (rh - lh >= 0){ // small
			x = sls(x);
		} 
		else // big
			x = bls(x);
	}
	if (rh - lh == -2){ // right spin
		p = p->left;
		lh = -1, rh = -1;
		if (p->left) lh = p->left->rank;
		if (p->right) rh = p->right->rank;
		if (lh - rh >= 0){ // small
			x = srs(x);
		}
		else // big
			x = brs(x);
	}
	if (child == -1)
		x->parent = x;
	else if (child == 0){
		x->parent = parent;
		parent->left = x;
	}
	else if (child == 1){
		x->parent = parent;
		parent->right = x;
	}
	if (changeRoot) root = x;
}

void swap(Node*& root, Node* p1, Node* p2){
	int tmp_rank = p1->rank;
	Node* tmp_left = p1->left;
	Node* tmp_right = p1->right;
	Node* tmp_parent;
	if (p1->parent == p1){ // p1 - root
		tmp_parent = p2;
	}
	else
		tmp_parent = p1->parent;
	
	p1->rank = p2->rank;
	if (p2->left) p2->left->parent = p1;
	p1->left = p2->left;
	if (p2->right) p2->right->parent = p1;
	p1->right = p2->right;
	
	if (p2->parent->left == p2 && p2->parent != p1) p2->parent->left = p1;
	else if (p2->parent->right == p2 && p2->parent != p1) p2->parent->right = p1;
	if (p2->parent == p1)
		p1->parent = p2;
	else 
		p1->parent = p2->parent;
	
	p2->rank = tmp_rank;
	if (tmp_left) tmp_left->parent = p2;
	if (tmp_left == p2) 
		p2->left = 0;
	else 
		p2->left = tmp_left;
	
	if (tmp_right) tmp_right->parent = p2;
	if (tmp_right == p2) 
		p2->right = 0;
	else
		p2->right = tmp_right;
	
	if (tmp_parent != p2){
		if (tmp_parent->left == p1) tmp_parent->left = p2;
		else tmp_parent->right = p2;
	}
	p2->parent = tmp_parent;
	if (root == p1) root = p2; //***
}

void deleteLeaf(Node*& root, Node*& x){
	Node* p = x->parent;
	if (p->left == x) p->left = 0;
	else p->right = 0;
	if (root == x) {root = 0; p = 0;}
	delete x;
	if (p){
		while(p->parent != p){
			balance(root, p);
			updateRank(p);
			p = p->parent;
		}
		balance(root, p);
		updateRank(p);
	}
}

void deleteNode(Node*& root, Node*& x){
	if (!x) return;
	if (x->right) {
		Node* p = next(x);
		swap(root, x, p);
		deleteNode(root, x);
	}
	else if (x->left){
		Node* p = previous(x);
		swap(root, x, p);
		deleteNode(root, x);
	}
	else deleteLeaf(root, x);
}

void setSections(map<int, pair<pair<double, double>, pair<double, double>>>* sections){
	sects = sections;
}

double getRealKey(int val, double x, double y){
	double eps = 1 / pow(10, 6);
	auto[x0, y0] = (*sects)[val].first;
	auto[x1, y1] = (*sects)[val].second;
	if (abs(x0 - x1) < eps){ // vertical section
		if (abs(y - y0) < eps) return y0;
		else return y;
	}
	
	// y = ax + b
	double a = (y1 - y0) / (x1 - x0);
	double b = -a * x0 + y0;
	return a * x + b;
}
