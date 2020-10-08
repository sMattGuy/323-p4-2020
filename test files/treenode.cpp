#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

class treeNode{
	public:
	//object variables
	string chStr;
	int frequency;
	string code;
	treeNode* left;
	treeNode* right;
	treeNode* next;
	//constructor
	treeNode(){
		this->chStr = "";
		this->frequency = -1;
		this->code = "";
		this->left = nullptr;
		this->right = nullptr;
		this->next = nullptr;
	}
	treeNode(string chStr, int frequency, string code, treeNode* left, treeNode* right, treeNode* next){
		this->chStr = chStr;
		this->frequency = frequency;
		this->code = code;
		this->left = left;
		this->right = right;
		this->next = next;
	}
	//methods
	void printNode(){
		cout<<"printnode\n";
		cout<<"("<< this->chStr <<","<< this->frequency <<","<< this->code;
		if(this->next!=nullptr){
			cout<<","<<this->next->chStr;
		}
		else{
			cout<<",NULL";
		}
		if(this->left!=nullptr){
			cout<<","<<this->left->chStr;
		}
		else{
			cout<<",NULL";
		}
		if(this->right!=nullptr){
			cout<<","<<this->left->chStr;
		}
		else{
			cout<<",NULL";
		}
		cout<<");\n";
	}
	bool isLeaf(){
		if(this->left==nullptr && this->right==nullptr)
			return true;
		return false;
	}
};

int main(){
	treeNode help("fuck",69,"help",nullptr,nullptr,nullptr);
	treeNode me;
	treeNode god;
	
	help.next = &me;
	
	me.chStr = "shit";
	me.frequency = 420;
	me.code = "pain";
	me.left = NULL;
	me.right = NULL;
	me.next = &god;
	
	cout<<help.next->chStr<<endl;
	
	help.printNode();
}
