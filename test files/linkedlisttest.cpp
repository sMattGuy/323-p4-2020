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
	void printNode(ofstream& debugFile){
		debugFile<<"printnode\n";
		debugFile<<"("<< this->chStr <<","<< this->frequency <<","<< this->code;
		if(this->next!=nullptr){
			debugFile<<","<<this->next->chStr;
		}
		else{
			debugFile<<",NULL";
		}
		if(this->left!=nullptr){
			debugFile<<","<<this->left->chStr;
		}
		else{
			debugFile<<",NULL";
		}
		if(this->right!=nullptr){
			debugFile<<","<<this->left->chStr;
		}
		else{
			debugFile<<",NULL";
		}
		debugFile<<");\n";
	}
	bool isLeaf(){
		if(this->left==nullptr && this->right==nullptr)
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
		this->head = treeNode("dummy",0,"",nullptr,nullptr,nullptr);
	}
	//methods
	treeNode findSpot(treeNode newNode){
		treeNode spot = this->head;
		while(spot.next != nullptr){
			if(spot.next->frequency >= newNode.frequency)
				return spot;
			spot = *spot.next;
		}
		return spot;
	}
	treeNode removeHead(){
		if(this->isEmpty()){
			cout<<"ERROR: attempting to remove from empty list\n";
			exit(0);
		}
		treeNode temp = *head.next;
		this->head.next = this->head.next->next;
		return temp;
	}
	void insertOneNode(treeNode spot, treeNode newNode){
		newNode.next = spot.next;
		spot.next = &newNode;
	}
	void printList(ofstream& debugFile){
		cout<<"print list\n";
		debugFile<<"Printing Linked List...\n";
		treeNode* readHead = this->head.next;
		while(readHead->next!=nullptr){
			readHead->printNode(debugFile);
			readHead = readHead->next;
		}
		readHead->printNode(debugFile);
	}
	bool isEmpty(){
		if(this->head.next == nullptr)
			return true;
		return false;
	}
};

int main(){
	linkedList test;
	
}