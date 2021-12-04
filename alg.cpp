#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>
#include "AVL.h"
#include "TwoThree.h"

extern std::map<int, std::pair<std::pair<double, double>, std::pair<double, double>>>* sects;

using namespace std;

int tests(int tests_num);
int findIntersectionAVL(string filename);
int findIntersectionTwoThree();

int main(){
	cout<<tests(100);
	findIntersectionAVL("testalg.txt");
	return 0;
}

int tests(int tests_num){
	string num;
	bool tt = true;
	bool avl = true;
	for (int i = 0; i < tests_num; ++i){
		num = to_string(i);
		tt = testTwoThree("tests\\test" + num + ".txt");
		avl = testAVL("tests\\test" + num + ".txt");
		if (tt || avl) cout<<num<<endl;
	}
	return 0;
}

int findIntersectionAVL(string filename){
	Node* tree = 0;
	map<int, pair<pair<double, double>, pair<double, double>>> sections; // {key: ((x0, y0), (x1, y1))}
	vector<pair<int, pair<double, double>>> points; // (section_key, (x1, y1))
	ifstream file (filename);
	int n;
	double x0, y0, x1, y1;
	file>>n;
	for (int i = 0; i < n; ++i){
		file>>x0>>y0>>x1>>y1;
		if (x0 < x1)
			sections.insert(make_pair(i, make_pair(make_pair(x0, y0), make_pair(x1, y1))));
		else
			sections.insert(make_pair(i, make_pair(make_pair(x1, y1), make_pair(x0, y0))));
		
		points.push_back(make_pair(i, make_pair(x0, y0)));
		points.push_back(make_pair(i, make_pair(x1, y1)));
	}
	sort(points.begin(), points.end(), [](auto &left, auto &right){
		return left.second.first < right.second.first;
	});
	
	setSections(&sections);
	getRealKey(2, 0, 0);
	
	//alg
	for (pair<int, pair<double, double>> point: points){
		Node* next, prev;
		if (sections[point.first].first == point.second){ // start of section
			//insert section
			// check sub section and uppper section
		}
		//cout << point.second.first << ' ' << point.second.second  <<endl;
	}
	
	return 0;
}