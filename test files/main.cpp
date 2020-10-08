#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;
ofstream debugFile;
char* nameDebugFile;

class huffman{
	public:
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
			this->left = NULL;
			this->right = NULL;
			this->next = NULL;
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
		void printNode(ofstream& debugFile){
			cout<<"printnode\n";
			if(this->next == NULL){
				debugFile<<"("<< this->chStr <<","<< this->frequency <<","<< this->code <<",NULL,NULL,NULL);\n";
			}
			else{
				debugFile<<"("<< this->chStr <<","<< this->frequency <<","<< this->code <<","<< this->next->chStr <<","<< this->left->chStr <<","<< this->right->chStr <<");\n";
			}
		}
		bool isLeaf(){
			if(this->left==NULL && this->right==NULL)
				return true;
			return false;
		}
	};
	class linkedList{
		public:
		//object variables
		treeNode head;
		//constructor
		linkedList(){
			this->head = new treeNode("dummy",0,"",NULL,NULL,NULL);
		}
		//methods
		treeNode* findSpot(treeNode* newNode){
			treeNode* spot = this->head;
			while(spot->next != NULL){
				if(spot->next->frequency >= newNode->frequency)
					return spot;
				spot = spot->next;
			}
			return spot;
		}
		treeNode* removeHead(){
			if(this->isEmpty())
				return NULL;
			treeNode* temp = head->next;
			this->head->next = this->head->next->next;
			return temp;
		}
		void insertOneNode(treeNode* spot, treeNode* newNode){
			newNode->next = spot->next;
			spot->next = newNode;
		}
		void printList(ofstream& debugFile){
			cout<<"print list\n";
			debugFile<<"Printing Linked List...\n";
			treeNode* readHead = this->head;
			while(readHead->next!=NULL){
				readHead->printNode(debugFile);
				readHead = readHead->next;
			}
			readHead->printNode(debugFile);
		}
		bool isEmpty(){
			if(this->head->next == NULL)
				return true;
			return false;
		}
	};
	class binaryTree{
		public:
		//object variables
		treeNode* root;
		//constructor
		binaryTree(){
			this->root = new treeNode("",-1,"",NULL,NULL,NULL);
		}
		//methods
		void preOrderTraversal(treeNode* root, ofstream& debugFile){
			if(root == NULL)
				return;
			root->printNode(debugFile);
			preOrderTraversal(root->left, debugFile);
			preOrderTraversal(root->right, debugFile);
		}
		void inOrderTraversal(treeNode* root, ofstream& debugFile){
			if(root == NULL)
				return;
			inOrderTraversal(root->left,debugFile);
			root->printNode(debugFile);
			inOrderTraversal(root->right,debugFile);
		}
		void postOrderTraversal(treeNode* root, ofstream& debugFile){
			if(root == NULL)
				return;
			postOrderTraversal(root->left,debugFile);
			postOrderTraversal(root->right,debugFile);
			root->printNode(debugFile);
		}
	};
	//huffman class object variables
	int charCountAry[256];
	string charCode[256];
	linkedList list;
	binaryTree tree;
	
	//constructor
	huffman(){
		cout<<"constructing huffman\n";
		for(int i=0;i<256;i++){
			this->charCountAry[i] = 0;
			this->charCode[i] = "";
		}
	}
	//methods
	void computeCharCounts(ifstream& input){
		cout<<"comparing char counts\n";
		char character;
		while(!input.eof()){
			input.get(character);
			int index = (int)character;
			this->charCountAry[index]++;
		}
	}
	void printCountary(ofstream& debugFile){
		cout<<"printing count array\n";
		for(int i=0;i<256;i++){
			if(charCountAry[i]!=0){
				debugFile<<"char"<<i<<" "<<charCountAry[i]<<endl;
			}
		}
	}
	void constructHuffmanLList(ofstream& debugFile){
		cout<<"making list\n";
		for(int i=0;i<256;i++){
			cout<<i<<" ";
			if(this->charCountAry[i]>0){
				cout<<this->charCountAry[i]<<endl;
				string s;
				s.push_back((char)i);
				cout<<s<<endl;
				treeNode* node = new treeNode(s,this->charCountAry[i],"",NULL,NULL,NULL);
				this->list.insertOneNode(this->list.findSpot(node),node);
				this->list.printList(debugFile);
			}
		}
	}
	void constructHuffmanBinTree(ofstream& debugFile){
		cout<<"making tree\n";
		while(!list.isEmpty()){
			treeNode* node1 = this->list.removeHead();
			treeNode* node2 = this->list.removeHead();
			treeNode* newnode = new treeNode(node1->chStr+","+node2->chStr,node1->frequency+node2->frequency,"",node1,node2,NULL);
			list.insertOneNode(list.findSpot(newnode),newnode);
			list.printList(debugFile);
		}
		this->tree.root = list.head;
	}
	void constructCharCode(treeNode* root,string code){
		cout<<"making char code\n";
		if(root->isLeaf()){
			root->code = code;
			int index = (int)root->chStr[0];
			this->charCode[index] = code;
		}
		else{
			constructCharCode(root->left,code+"0");
			constructCharCode(root->right,code+"0");
		}
	}
	void encode(ifstream& input, ofstream& output){
		cout<<"encoding\n";
		char character;
		while(!input.eof()){
			input.get(character);
			int index = (int)character;
			string code = this->charCode[index];
			output<<index<<":"<<code<<endl;
		}
	}
	void decode(ifstream& input, ofstream& output){
		cout<<"decoding\n";
		treeNode* spot = this->tree.root;
		char character;
		while(!input.eof()){
			if(spot->isLeaf()){
				output<<spot->chStr;
			}
			else{
				input.get(character);
				int oneBit = (int)character;
				if(oneBit == 0){
					spot = spot->left;
				}
				else if(oneBit == 1){
					spot = spot->right;
				}
				else{
					cout<<"Error! The compressed file is corrupted!\n";
					exit(0);
				}
			}
		}
		if(!spot->isLeaf()){
			cout<<"Error! The compressed file is corrupted!\n";
			exit(0);
		}
	}
	void userInterface(){
		string nameOrg;
		string nameCompress;
		string nameDecompress;
		char yesNo;
		question:
		cout<<"Would you like to encode a file? y/n\n";
		if(yesNo == 'n' || yesNo == 'N')
			exit(0);
		else{
			cout<<"Please enter the name of your file.\n";
			cin>>nameOrg;
			nameCompress = nameOrg + "_Compress";
			nameDecompress = nameOrg + "_DeCompress";
			
			int org = nameOrg.length();
			int comp = nameCompress.length();
			int decomp = nameDecompress.length();
			
			char orgAr[org+1];
			char compAr[comp+1];
			char decompAr[decomp+1];
			
			strcpy(orgAr, nameOrg.c_str());
			strcpy(compAr, nameCompress.c_str());
			strcpy(decompAr, nameDecompress.c_str());
			
			ifstream input(orgAr);
			
			ofstream outCompress;
			outCompress.open(compAr, ios::out | ios::app);
			
			ofstream outDecompress;
			outDecompress.open(decompAr, ios::out | ios::app);
			
			encode(input,outCompress);
			
			outCompress.close();
			ifstream input2(compAr);
			input2.open(compAr, ios::in);
			
			decode(input2, outDecompress);
			input.close();
			outCompress.close();
			outDecompress.close();
		}
		goto question;
	}
};

int main(int argc, char* argv[]){
	char* nameInFile = argv[1];
	ifstream input(nameInFile);
	nameDebugFile = strcat(nameInFile, "_Debug.txt");
	debugFile.open(nameDebugFile, ios::out | ios::app);
	huffman huff;
	huff.computeCharCounts(input);
	huff.printCountary(debugFile);
	huff.constructHuffmanLList(debugFile);
	huff.constructHuffmanBinTree(debugFile);
	huff.constructCharCode(huff.tree.root,"");
	huff.list.printList(debugFile);
	huff.tree.preOrderTraversal(huff.tree.root,debugFile);
	huff.tree.inOrderTraversal(huff.tree.root,debugFile);
	huff.tree.postOrderTraversal(huff.tree.root,debugFile);
	huff.userInterface();
	input.close();
	debugFile.close();
	return 0;
}
