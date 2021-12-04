#include <iostream>
#include <fstream>
#include <string>
#include "TwoThree.h"

using namespace std;

int testTwoThree(string filename){
	TwoThree* root = 0;
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
				deleteKey(root, cmd_val);
				break;
			}
			case 'x':{ // neXt
				commands>>cmd_val;
				TwoThree* p = nextNode(root, cmd_val);
				if(p)
					cout<<p->val<<endl;
				break;
			}
			case 'v':{ // preV
				commands>>cmd_val;
				TwoThree* p = prevNode(root, cmd_val);
				if(p)
					cout<<p->val<<endl;
				break;
			}
			case 'c':{
				if (!checkLinksDuality(root)) return 1;
				break;
			}
			default:
				cout << "No such command\n"; break;
			
		}
	}
	commands.close();
	return 0;
}

void printTree(TwoThree* p, int depth){
	if (!p) return;
	if (p->type == 0){
		for (int i = 0; i < depth; ++i) cout << '\t';
		cout << p->val << endl;
		return;
	}
	for (int i = p->type - 1; i >= (p->type) / 2; --i){
		printTree(p->childs[i], depth + 1);
	}
	for (int i = 0; i < depth; ++i) cout << '\t';
	cout << '(';
	int i = 0;
	for (; i < p->type - 1; ++i) cout << p->max_values[i] << ", ";
	cout << p->max_values[i] << ')' << endl;
	for (int i = p->type / 2 - 1; i >= 0; --i){
		printTree(p->childs[i], depth + 1);
	}
}

bool checkLinksDuality(TwoThree* p){
	if (!p) return true;
	TwoThree* parent = p->parent;
	TwoThree* min_leaf = p;
	TwoThree* max_leaf = p;
	while(min_leaf->type > 0){
		min_leaf = min_leaf->childs[0];
	}
	while(max_leaf->type > 0){
		max_leaf = max_leaf->childs[max_leaf->type - 1];
	}
	
	bool r_1 = checkTreeLinks(parent, p);
	bool r_2 = checkLeafListLinks(min_leaf, max_leaf);
	return r_1 && r_2;
}

bool checkTreeLinks(TwoThree* parent, TwoThree* p){
	bool flag = true;
	for (int i = 0; i < p->type; ++i){
		flag = flag && checkTreeLinks(p, p->childs[i]);
	}
	return (p->parent == parent) && flag;
}

bool checkLeafListLinks(TwoThree* l, TwoThree* r){
	TwoThree* q = l;
	TwoThree* prev_leaf;
	while (q != r){
		prev_leaf = q;
		q = q->next;
		if (prev_leaf->next != q || prev_leaf != q->prev) {
			return false;
		}
		//cout <<prev_leaf->val<< ' ' << q->val << endl;
	}
	return true && (!l->prev && !r->next);
}

TwoThree* emptyLeaf(int val){
	TwoThree* p = new TwoThree();
	p->val = val;
	p->type = 0;
	p->childs = 0;
	p->max_values = 0;
	p->next = 0;
	p->prev = 0;
	return p;
}

TwoThree* emptyNode(){
	TwoThree* p = new TwoThree();
	p->childs = new TwoThree*[4];
	for (int i = 0; i < 4; ++i){
		p->childs[i] = 0;
	}
	p->parent = 0;
	p->max_values = new int[4];
	p->next = 0;
	p->prev = 0;
	return p;
}

TwoThree* search(TwoThree* root, int val, int& child_index){
	// node = 0, index = -1 <=> empty tree
	// node = 0, index > 0 <=> root is a leaf
	// node != 0 index > 0 <=> node is parent to <node with key = val> (might not exist)
	if (!root) return 0;
	if (root->type == 0){
		if (val <= root->val) child_index = 0;
		else child_index = 1;
		return 0;
	}
	TwoThree* p = root; // current node
	TwoThree* parent = root; // parent node
	while (true){
		
		// check current node
		if (p->type == 0) { // p is a leaf
			return parent;
		}
		
		// go deeper
		parent = p;
		if (p->type == 2){
			if (val <= p->max_values[0]){
				p = p->childs[0];
				child_index = 0;
			}
			else {
				p = p->childs[1];
				child_index = 1;
			}
		}
		else if (p->type == 3){
			if (val <= p->max_values[0]){
				p = p->childs[0];
				child_index = 0;
			}
			else if (val <= p->max_values[1]){
				p = p->childs[1];
				child_index = 1;
			}
			else {
				p = p->childs[2];
				child_index = 2;
			}
		}
	}
}


void appendLeafInChildList(TwoThree* parent, TwoThree* p, int child_index){
	p->parent = parent;
	if (parent->type - 1 == child_index && p->val > parent->childs[child_index]->val){
		//insert leaf
		parent->childs[child_index + 1] = p;
		parent->max_values[child_index + 1] = p->val;
		
		//update leaf's links
		TwoThree* next = parent->childs[child_index]->next;
		if (next != p){ // == when appending leaf from neighbor parent
			p->next = next;
			if (next) next->prev = p;
			p->prev = parent->childs[child_index];
			parent->childs[child_index]->next = p;
		}
	}
	else{
		int i = parent->type - 1;
		for(; i >= child_index; --i){
			parent->childs[i + 1] = parent->childs[i];
			parent->max_values[i + 1] = parent->max_values[i];
		}
		++i;
		
		//update leaf's links
		TwoThree* previous = parent->childs[i]->prev;
		if (previous != p){ // == when appending leaf from neighbor parent
			p->next = parent->childs[i + 1];
			parent->childs[i + 1]->prev = p;
			p->prev = previous;
			if (previous) previous->next = p;
		}
		
		//insert leaf
		parent->childs[i] = p;
		parent->max_values[i] = p->val;
	}
	parent->type += 1;
}

void insert(TwoThree*& root, int val){
	int child_index = -1;
	TwoThree* prev_node = search(root, val, child_index);
	if (prev_node){
		if (prev_node->childs[child_index]->val == val) {return; }// such key already exist
		TwoThree* new_node = emptyLeaf(val);
		appendLeafInChildList(prev_node, new_node, child_index);
		balance(root, prev_node);
	}
	else{
		if (child_index < 0){ // empty tree
			root = emptyLeaf(val);
			root->parent = 0;
		}
		else{ // root is a leaf
			if (root->val == val) return; // such key already exist
			TwoThree* p = root;
			TwoThree* new_root = emptyNode();
			new_root->type = 2;
			new_root->parent = 0;
			
			TwoThree* child = emptyLeaf(val);
			if (child_index == 0){ // new node key < current node key
				child->next = p;
				p->prev = child;
				new_root->childs[0] = child;
				new_root->childs[1] = p;
				new_root->max_values[0] = child->val;
				new_root->max_values[1] = p->val;
			}
			if (child_index == 1){ // new node key > current node key
				child->prev = p;
				p->next = child;
				new_root->childs[0] = p;
				new_root->childs[1] = child;
				new_root->max_values[0] = p->val;
				new_root->max_values[1] = child->val;
			}
			child->parent = new_root;
			p->parent = new_root;
			root = new_root;//***
		}
	}
}

void updateMaxValues(TwoThree* parent, TwoThree* p){
	while(parent){
		for (int i = 0; i < parent->type; ++i){
			if (parent->childs[i] == p) {
				if (p->type == 0){
					parent->max_values[i] = p->val;
				}
				else{
					parent->max_values[i] = p->max_values[p->type - 1];
				}
			}
		}
		p = parent;
		parent = p->parent;
	}
	
	for (int i = 0; i < p->type; ++i){
		TwoThree* child = p->childs[i];
		if (child->type == 0){
			p->max_values[i] = child->val;
		}
		else{
			p->max_values[i] = child->max_values[child->type - 1];
		}
	}
}

void balance(TwoThree*& root, TwoThree* p){
	TwoThree* parent = p->parent;
	while (parent && p->type == 4){
		separateNode(parent, p);
		p = parent;
		parent = p->parent;
	}
	if (p->type == 4){ // root is overloaded 
		TwoThree* new_root = emptyNode();
		new_root->type = 2;
		separateNode(new_root, p);
		root = new_root;
	}
	else{ 
		updateMaxValues(parent, p);
	}
}

void separateNode(TwoThree* parent, TwoThree* p){
	TwoThree* p_half = emptyNode(); // right half of p node
	int child_index = getChildIndex(p);
	
	//set new half of p node
	p_half->type = 2;
	p_half->parent = parent;
	p_half->childs[0] = p->childs[2];
	p_half->childs[1] = p->childs[3];
	p_half->max_values[0] = p->max_values[2];
	p_half->max_values[1] = p->max_values[3];
	
	// update p node
	p->type = 2;
	p->childs[2] = 0;
	p->childs[3] = 0;
	p->max_values[2] = 0;
	p->max_values[3] = 0;
	
	// update moved childs
	p_half->childs[0]->parent = p_half;
	p_half->childs[1]->parent = p_half;
	
	//update parent ---> child links
	if (child_index < 0){ // separation node is root (p node)
		p->parent = parent;
		parent->childs[0] = p;
		parent->max_values[0] = p->max_values[p->type - 1];
		parent->childs[1] = p_half;
		parent->max_values[1] = p_half->max_values[p_half->type - 1];
	}
	else{
		appendNodeInChildList(parent, p_half, child_index);
		parent->max_values[child_index] = p->max_values[p->type - 1];
	}
}

void appendNodeInChildList(TwoThree* parent, TwoThree* p, int child_index){
	p->parent = parent;
	
	for (int i = parent->type - 1; i > child_index; --i){
		parent->childs[i + 1] = parent->childs[i];
		parent->max_values[i + 1] = parent->max_values[i];
	}
	parent->childs[child_index + 1] = p;
	parent->max_values[child_index + 1] = p->max_values[p->type - 1];
	parent->type += 1;
	
	TwoThree* prev_p = parent->childs[child_index];
	if (prev_p->max_values[prev_p->type - 1] > p->max_values[p->type - 1]){

		//swap prev_p and p
		TwoThree* tmp = prev_p;
		parent->childs[child_index] = p;
		parent->max_values[child_index] = p->max_values[p->type - 1];
		parent->childs[child_index + 1] = prev_p;
		parent->max_values[child_index + 1] = prev_p->max_values[prev_p->type - 1];
	}
}

void deleteKey(TwoThree*& root, int val){
	int child_index = -1;
	TwoThree* prev_node = search(root, val, child_index);
	if (prev_node){
		if (prev_node->childs[child_index]->val != val) return; // no such key
		deleteNode(root, prev_node->childs[child_index], child_index);
	}
	else{
		if (child_index < 0) return; // empty tree
		else{ // root is a leaf
			if (root->val != val) return; // no such key
			freeNodeMemo(root);
			root = 0;
		}
	}
}

void deleteNode(TwoThree*& root, TwoThree* p, int child_index){
	
	TwoThree* parent = p->parent;
	if (parent->type > 2){ // 3 or 4
		updateLeafOrder(p);
		// delete node and update parent's childs links 
		freeNodeMemo(p);
		for (int i = child_index; i <= parent->type - 2; ++i){ // shift childs
			parent->childs[i] = parent->childs[i + 1];
			parent->max_values[i] = parent->max_values[i + 1];
		}
		parent->childs[parent->type - 1] = 0;
		parent->type -= 1;
		return;
	}
	
	if (parent->type == 2){
		
		// delete node
		updateLeafOrder(p);
		freeNodeMemo(p);
		p = 0;
		
		int other_child_index = 1 - child_index;
		parent->childs[0] = parent->childs[other_child_index];
		parent->max_values[0] = parent->max_values[other_child_index];
		parent->childs[1] = 0;
		parent->type -= 1;
		
		// update links depends on structure variant
		TwoThree* grand_parent = parent->parent;
		
		//1 variant
		if (!grand_parent){ // parent == root
			if (parent != root) cout << "WARNING: parent != root deletion\n";
			
			// new root is other child
			root = parent->childs[0];
			root->parent = 0;
			freeNodeMemo(parent);
			parent = 0;
			return;
		}
		
		int parent_child_index = -1;
		int parent_neighbor_child_index = -1;
		TwoThree* parent_neighbor = getNeighbor(parent, parent_child_index, 
												parent_neighbor_child_index);
		if (parent_child_index < 0 || parent_neighbor_child_index < 0) 
			cout << "WARNING negative child index\n";
		
		//2 variant
		if (grand_parent->type >= 2 && parent_neighbor->type == 2){
			TwoThree* other_child = parent->childs[0];
			other_child->parent = parent_neighbor;
			int connection_child_index = -1;
			if (parent_child_index - parent_neighbor_child_index > 0)
				connection_child_index = parent_neighbor->type - 1;
			else 
				connection_child_index = 0;
			
			
			if (other_child->type == 0){
				appendLeafInChildList(parent_neighbor, other_child, connection_child_index);
			}
			else{
				appendNodeInChildList(parent_neighbor, other_child, connection_child_index);
			}
			
			parent_child_index = getChildIndex(parent); // might change after appending
			if (grand_parent->type == 2){
				balance(root, parent_neighbor);
				deleteNode(root, parent, parent_child_index);
			}
			else if (grand_parent->type == 3){
				//delete parent
				freeNodeMemo(parent);
				parent = 0;
				
				//shift grandparent's childs
				for (int i = parent_child_index; i <= grand_parent->type - 2; ++i){
					grand_parent->childs[i] = grand_parent->childs[i + 1];
					grand_parent->max_values[i] = grand_parent->max_values[i + 1];
				}
				grand_parent->childs[grand_parent->type - 1] = 0;
				grand_parent->type -= 1;
				updateMaxValues(grand_parent, parent_neighbor);
			}
			else{
				cout<< "UNDEFINED DELETION VARIANT 1\n";
			}
			return;
		}
		
		//3 variant
		if (1 < grand_parent->type && grand_parent->type < 4 && parent_neighbor->type == 3){
			TwoThree* neighbor_p;
			
			// select neighbor_p 
			if (parent_child_index - parent_neighbor_child_index > 0){
				neighbor_p = parent_neighbor->childs[parent_neighbor->type - 1];
			}
			else{
				neighbor_p = parent_neighbor->childs[0];
				
				//shift neighbor parent's childs
				for (int i = 0; i <= parent_neighbor->type - 2; ++i){
					parent_neighbor->childs[i] = parent_neighbor->childs[i + 1];
					parent_neighbor->max_values[i] = parent_neighbor->max_values[i + 1];
				}
				
			}
			parent_neighbor->childs[parent_neighbor->type - 1] = 0;
			parent_neighbor->type -= 1;
			
			//append selected node to parent of deleted node
			if (neighbor_p->type == 0){
				appendLeafInChildList(parent, neighbor_p, 0);
			}
			else{
				appendNodeInChildList(parent, neighbor_p, 0);// todo
			}
			
			
			//update max values
			updateMaxValues(grand_parent, parent);
			if (parent_child_index - parent_neighbor_child_index > 0){
				updateMaxValues(grand_parent, parent_neighbor);
			}
			return;
		}
		cout<< "UNDEFINED DELETION VARIANT 2\n";
	}
	
}

TwoThree* getNeighbor(TwoThree* p, int& child_index, int& neighbor_child_index){
	TwoThree* parent = p->parent;
	if (!parent) return 0;
	for (int i = 0; i < parent->type; ++i){
		if (parent->childs[i] == p){
			child_index = i;
			if(i > 0){
				neighbor_child_index = i - 1;
				return parent->childs[i - 1];
			}
			else{
				neighbor_child_index = i + 1;
				return parent->childs[i + 1];
			}
		}
	}
	return 0;
}

int getChildIndex(TwoThree* p){
	TwoThree* parent = p->parent;
	if (parent){
		for (int i = 0; i < parent->type; ++i){
			if (parent->childs[i] == p) return i;
		}
	}
	return -1;
}

void updateLeafOrder(TwoThree* p){
	if (p->type == 0){ 
		TwoThree* previous = p->prev;
		TwoThree* next = p->next;
		if (previous) previous->next = next;
		if (next) next->prev = previous;
	}
}

void freeNodeMemo(TwoThree* p){
	if (p->type != 0){
		delete[] p->childs;
		delete[] p->max_values;
	}
	delete p;
}

TwoThree* nextNode(TwoThree* root, int val){
	int child_index = -1;
	TwoThree* parent = search(root, val, child_index);
	if(!parent) return 0;
	else {
		TwoThree* p = parent->childs[child_index];
		if (p->val > val) return p;
		else return p->next;
	}
}

TwoThree* prevNode(TwoThree* root, int val){
	int child_index = -1;
	TwoThree* parent = search(root, val, child_index);
	if(!parent) return 0;
	else {
		TwoThree* p = parent->childs[child_index];
		if (p->val < val) return p;
		else return p->prev;
	}
}