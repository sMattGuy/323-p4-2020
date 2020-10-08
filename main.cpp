#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;
ofstream debugFile;
//global file names for ease of access
char* nameDebugFile;
char* compressedname;
char* decompressname;
char* inputname;
char* input2name;

/**
* Matthew Flammia, 23661371
* CSCI 323.35 Project 4
* To compile, use the command:
* g++ -std=gnu++0x -o FlammiaM_Project4_CPP main.cpp
* You must include 1 Data File, any more or any less will terminate the program.
* When entering the filename during runtime, you MUST include the extension! Otherwise it will not find the file correctly
**/

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
			debugFile.open(nameDebugFile, ios::out | ios::app);
			debugFile<<"(<"<< this->chStr <<">,<"<< this->frequency <<">,<"<< this->code;
			//the following prevents null pointer issues
			if(this->next!=nullptr){
				debugFile<<">,<"<<this->next->chStr;
			}
			else{
				debugFile<<">,<NULL";
			}
			if(this->left!=nullptr){
				debugFile<<">,<"<<this->left->chStr;
			}
			else{
				debugFile<<">,<NULL";
			}
			if(this->right!=nullptr){
				debugFile<<">,<"<<this->right->chStr;
			}
			else{
				debugFile<<">,<NULL";
			}
			debugFile<<">);\n";
			debugFile.close();
		}
		bool isLeaf(){
			//checks if node is a leaf
			if(this->left==nullptr && this->right==nullptr)
				return true;
			return false;
		}
	};
	class linkedList{
		public:
		//object variables
		treeNode* head;
		//constructor
		linkedList(){
			this->head = new treeNode("dummy",0,"",nullptr,nullptr,nullptr);
		}
		//methods
		treeNode* findSpot(treeNode** headref, treeNode* newNode){
			//horrendous. you must use a reference to the head node as to not overwrite it. took 3 hours to figure out. other than that no different
			treeNode* spot = *headref;
			while(spot->next != nullptr){
				if(spot->next->frequency >= newNode->frequency)
					return spot;
				spot = spot->next;
			}
			return spot;
		}
		void insertOneNode(treeNode* spot, treeNode* newNode){
			//basic insert code
			newNode->next = spot->next;
			spot->next = newNode;
		}
		
		treeNode* removeHead(){
			if(this->isEmpty()){
				//checks incase you remove 
				cout<<"ERROR: attempting to remove from empty list\n";
				exit(0);
			}
			//also misserable. a pointer reference. took 2 hours to figure out
			treeNode* temp = *&this->head->next;
			this->head->next = *&this->head->next->next;
			return temp;
		}
		
		void printList(){
			debugFile.open(nameDebugFile, ios::out | ios::app);
			debugFile<<"Printing Linked List...\n";
			debugFile.close();
			treeNode* readHead = this->head;
			//simple loop, just calls print node until at end
			while(readHead->next!=nullptr){
				readHead->printNode();
				readHead = readHead->next;
			}
			readHead->printNode();
		}
		
		bool isEmpty(){
			//simple empty check
			if(this->head->next == nullptr)
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
			this->root = new treeNode("",-1,"",nullptr,nullptr,nullptr);
		}
		//methods
		//bunch of traversal prints, all work recursivly 
		void preOrderTraversal(treeNode* node){
			if(node == nullptr)
				return;
			node->printNode();
			preOrderTraversal(node->left);
			preOrderTraversal(node->right);
		}
		void inOrderTraversal(treeNode* node){
			if(node == nullptr)
				return;
			inOrderTraversal(node->left);
			node->printNode();
			inOrderTraversal(node->right);
		}
		void postOrderTraversal(treeNode* node){
			if(node == nullptr)
				return;
			postOrderTraversal(node->left);
			postOrderTraversal(node->right);
			node->printNode();
		}
	};
	//huffman class object variables
	int charCountAry[256];
	string charCode[256];
	linkedList list;
	binaryTree tree;
	
	//constructor
	huffman(){
		for(int i=0;i<256;i++){
			this->charCountAry[i] = 0;
			this->charCode[i] = "";
		}
	}
	//methods of huffman
	void computeCharCounts(ifstream& input){
		char character;
		while(!input.eof()){
			input.get(character);
			int index = (int)character;
			this->charCountAry[index]++;
		}
	}
	void printCountary(){
		//simple print, it prints count array
		debugFile.open(nameDebugFile, ios::out | ios::app);
		debugFile<<"Printing Count Array\n";
		for(int i=0;i<256;i++){
			if(charCountAry[i]!=0){
				debugFile<<"char"<<i<<" "<<charCountAry[i]<<endl;
			}
		}
		debugFile.close();
	}
	void constructHuffmanLList(){
		//creates the linked list, using find spot and insert methods
		debugFile.open(nameDebugFile, ios::out | ios::app);
		debugFile<<"making list\n";
		debugFile.close();
		for(int i=0;i<256;i++){
			if(this->charCountAry[i]>0){
				char input = i;
				string charToString(1,input);
				if(i==10)
					charToString = "\\n";
				treeNode* node = new treeNode(charToString,this->charCountAry[i],"",nullptr,nullptr,nullptr);
				this->list.insertOneNode(this->list.findSpot(&this->list.head,node),node);
				this->list.printList();
			}
		}
	}
	void constructHuffmanBinTree(){
		//creates binary tree
		debugFile.open(nameDebugFile, ios::out | ios::app);
		debugFile<<"Making Binary Tree\n";
		debugFile.close();
		while(this->list.head->next->next != nullptr){
			//takes first 2 nodes of linked list
			treeNode* node1 = this->list.head->next;
			treeNode* node2 = this->list.head->next->next;
			//merges chstr of nodes 1 and 2 
			string newchrstr = node1->chStr + "," + node2->chStr;
			//merges frequency of nodes 1 and 2
			int newfrequency = node1->frequency + node2->frequency;
			//creates new node 
			treeNode* newnode = new treeNode(newchrstr,newfrequency,"",node1,node2,nullptr);
			//inserts new node
			list.insertOneNode(list.findSpot(&this->list.head,newnode),newnode);
			//sets head->next to 2 positions next
			this->list.head->next = this->list.head->next->next->next;
			//prints the list after insert
			list.printList();
		}
		//once done sets the root to the lists beginning
		this->tree.root = this->list.head->next;
	}
	void constructCharCode(treeNode* root,string code){
		//recursivly calls to create a charcode
		//break statement
		if(root->isLeaf()){
			root->code = code;
			int index = root->chStr[0];
			//if index is a backslash, convert it to a new line character
			if(index == 92)
				index = 10;
			this->charCode[index] = code;
		}
		else{
			//recursive call
			constructCharCode(root->left,code+"0");
			constructCharCode(root->right,code+"1");
		}
	}
	void encode(ifstream& input, ofstream& output){
		input.open(inputname);
		output.open(compressedname);
		char character;
		debugFile.open(nameDebugFile, ios::out | ios::app);
		//loops until file end
		while(!input.eof()){
			input.get(character);
			int index = (int)character;
			string code = this->charCode[index];
			output<<code;
			debugFile<<index<<":"<<code<<endl;
		}
		debugFile.close();
		input.close();
		output.close();
	}
	void decode(ifstream& input, ofstream& output){
		treeNode* spot = this->tree.root;
		treeNode* reserve = this->tree.root;
		input.open(input2name);
		output.open(decompressname);
		char character;
		//loops until file end
		while(!input.eof()){
			//character found
			if(spot->isLeaf()){
				if(spot->chStr == "\\n"){
					output<<"\n";
				}
				else{
					output<<spot->chStr;
				}
				spot = this->tree.root;
			}
			else{
				//tests which direction to go
				input.get(character);
				int oneBit = (int)character - '0';
				if(oneBit == 0){
					spot = spot->left;
				}
				else if(oneBit == 1){
					spot = spot->right;
				}
				else{
					//happens if a 1 or 0 isnt read
					cout<<"Error! Non-Binary bit read.\n";
					cout<<"Bit read:"<<oneBit<<endl;
					exit(0);
				}
			}
		}
		output.close();
		input.close();
	}
	void userInterface(){
		string nameOrg;
		string nameCompress;
		string nameDecompress;
		char yesNo;
		//goto loop label
		question:
		cout<<"Would you like to encode a file? y/n\n";
		cin>>yesNo;
		if(yesNo == 'n' || yesNo == 'N')
			exit(0);
		else{
			cout<<"Please enter the name of your file.\n";
			cin>>nameOrg;
			//name creation for compressed and decompressed files
			nameCompress = nameOrg + "_Compress.txt";
			nameDecompress = nameOrg + "_DeCompress.txt";
			//gets length of names
			int org = nameOrg.length();
			int comp = nameCompress.length();
			int decomp = nameDecompress.length();
			//creates char arrays for names
			char orgAr[org+1];
			char compAr[comp+1];
			char decompAr[decomp+1];
			//converts string names into char arrays
			strcpy(orgAr, nameOrg.c_str());
			strcpy(compAr, nameCompress.c_str());
			strcpy(decompAr, nameDecompress.c_str());
			//sets global names to char array names
			compressedname = compAr;
			decompressname = decompAr;
			inputname = orgAr;
			input2name = compAr;
			//creates file streams
			ifstream input;
			ofstream outCompress;
			ifstream input2;
			ofstream outDecompress;
			//encodes or compresses
			encode(input,outCompress);
			//decodes based on compress
			decode(input2, outDecompress);
			cout<<"Done\n";
		}
		goto question;
	}
};

//helpful functions
void seperation(){
	debugFile.open(nameDebugFile, ios::out | ios::app);
	debugFile<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	debugFile.close();
}

int main(int argc, char* argv[]){
	if(argc != 2){
		cout<<"You must supply only 1 data file!\n";
		return 0;
	}
	//gets filename
	char* nameInFile = argv[1];
	//creates initial ifstream
	ifstream input(nameInFile);
	nameDebugFile = strcat(nameInFile, "_Debug.txt");
	//creates huffman class
	huffman huff;
	//data structure creation
	cout<<"Creating necessary structures, Please wait.\r";
	huff.computeCharCounts(input);
	cout<<"Creating necessary structures, Please wait..\r";
	seperation();
	huff.printCountary();
	cout<<"Creating necessary structures, Please wait...\r";
	seperation();
	huff.constructHuffmanLList();
	cout<<"Creating necessary structures, Please wait....\r";
	seperation();
	huff.constructHuffmanBinTree();	
	
	debugFile.open(nameDebugFile, ios::out | ios::app);
	debugFile<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	debugFile<<"Making Char Code\n";
	cout<<"Creating necessary structures, Please wait.....\r";
	huff.constructCharCode(huff.tree.root,"");
	debugFile.close();
	cout<<"Creating necessary structures, Please wait......\r";
	seperation();
	huff.list.printList();
	debugFile.close();
	
	debugFile.open(nameDebugFile, ios::out | ios::app);
	debugFile<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	debugFile<<"preOrder Traversal\n";
	debugFile.close();
	cout<<"Creating necessary structures, Please wait.......\r";
	huff.tree.preOrderTraversal(huff.tree.root);
	debugFile.open(nameDebugFile, ios::out | ios::app);
	debugFile<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	debugFile<<"InOrder Traversal\n";
	debugFile.close();
	cout<<"Creating necessary structures, Please wait........\r";
	huff.tree.inOrderTraversal(huff.tree.root);
	debugFile.open(nameDebugFile, ios::out | ios::app);
	debugFile<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	debugFile<<"PostOrder Traversal\n";
	debugFile.close();
	cout<<"Creating necessary structures, Please wait.........\r";
	huff.tree.postOrderTraversal(huff.tree.root);
	cout<<"Creating necessary structures, Please wait.........Done\n";
	//opens user interface for rest of program
	huff.userInterface();
	
	input.close();
	debugFile.close();
	return 0;
}
