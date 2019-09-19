#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

typedef unsigned char uchar;

//********************************
//note class
//********************************
class Node {
public:
	string chStr;
	int prob;
	Node *next;
	Node *left;
	Node *right;
	string code;

	Node(string s, int p) {
		chStr = s;
		prob = p;
		next = NULL;
		left = NULL;
		right = NULL;
	}

	Node(uchar s, int p) {
		chStr = s;
		prob = p;
		next = NULL;
		left = NULL;
		right = NULL;
	}

	void printNode(ofstream& outFile) {
		string line = "Node:\"" + chStr + "\",\tNode's prob:" + to_string(prob);
		if (next != NULL) {
			line += ",\tNextNode:\"" + next->chStr + "\"";
		}
		else {
			line += ",\tNextNode:NULL";
		}
		if (!isLeaf()) {
			line += ",\tleft:\"" + left->chStr + "\",\tright:\"" + right->chStr + "\"\n\n";
		}
		else {
			line += ",\tleft: NULL,\t right: NULL\n\n";
		}
		outFile << line;
	}

	bool isLeaf() {
		return left == NULL && right == NULL;
	}
};

//********************************
//Huffman Linked List class
//********************************
class HuffmanLList {
public:
	Node *listHead;

	HuffmanLList() {listHead = NULL;}

	void insertNewNode(Node *newN) {
		Node *spot = findSpot(newN);
		newN->next = spot->next;
		spot->next = newN;
	}

	void printList(ofstream &outFile) {
		Node *spot = listHead;
		while (spot->next != NULL) {
			outFile << "(\"" + spot->chStr + "\", " + to_string(spot->prob) + ", \"" + spot->next->chStr + "\")-->";
			spot = spot->next;
		}
		outFile << "(\"" + spot->chStr + "\", " + to_string(spot->prob) + ", \"NULL\")-->NULL\n";
	}

	void constructHuffmanLList(int charCounts[], ofstream &outFile) {
		Node *lh = new Node("dummy", 0);
		listHead = lh;
		Node *newNode = NULL;
		for (int index = 0; index < 256; index++) {
			if (charCounts[index] > 0) {
				newNode = new Node((uchar)index, charCounts[index]);
				insertNewNode(newNode);
			}
		}
		outFile << endl;
		printList(outFile);
	}

private:
	Node *findSpot(Node *newN) {
		Node *spot = listHead;
		while (spot->next != NULL && spot->next->prob < newN->prob) {
			spot = spot->next;
		}
		return spot;
	}
};

//********************************
//huffman Binary Class class
//********************************
class HuffmanBiTree {
public:
	Node *root;
	HuffmanLList *LList;
	string charCode[256] = { "" };

	HuffmanBiTree(HuffmanLList *ll) {
		root = NULL;
		LList = ll;
	}

	void constructTree(ofstream& outFile) {
		Node *newNode = NULL;
		Node *newHead = LList->listHead;
		outFile << endl;
		while (newHead->next->next != NULL) {
			newNode = new Node(newHead->next->chStr + newHead->next->next->chStr,
								newHead->next->prob + newHead->next->next->prob);
			newNode->left = newHead->next;
			newNode->right = newHead->next->next;
			LList->insertNewNode(newNode);
			newHead->next = newHead->next->next->next;
			LList->printList(outFile);
			newNode->printNode(outFile);
		}
		root = LList->listHead->next;
	}

	void getCode(Node *t, string code, ofstream &outFile) {
		if (t->isLeaf()) {
			t->code = code;
			uchar c = (t->chStr)[0];
			charCode[(int)c] = code;
			outFile << t->chStr << "  " + t->code << endl;
		}
		else {
			getCode(t->left, code + "0", outFile);
			getCode(t->right, code + "1", outFile);
		}
	}

	void encode(ifstream& inFile, ofstream& outFile) {
		int index;
		while (true) {
			uchar charIn = inFile.get();
			if (inFile.eof()) { break; }
			index = (int)charIn;
			if (index < 256 && index != 152)
				outFile << charCode[index];
		}
	}

	void decode(ifstream& inFile, ofstream& outFile, Node *node) {
		if (node->isLeaf()) {												//without changing the system stack size for the project
			outFile << node->chStr;											//some machine may encounter stack overflow using this recursion with larger inputFile3
			decode(inFile, outFile, root);									//however the function below can avoid that
		}
		else {
			if (inFile.eof() && node != root) {
				cout << "ERROR: the encode file is corrupted file." << endl;
			}
			else {
				uchar charIn = inFile.get();
				if (charIn == '0')
					decode(inFile, outFile, node->left);
				else if (charIn == '1')
					decode(inFile, outFile, node->right);
			}
		}
	}

	/*void decode(ifstream& inFile, ofstream& outFile, Node *node) {
		Node *current = node;
		while (true) {
			uchar charIn = inFile.get();
			if (inFile.eof()) { break; }
			if (charIn == '0')
				current = current->left;
			if (charIn == '1')
				current = current->right;
			if (current->isLeaf()) {
				outFile << current->chStr;
				current = root;
			}
		}
		if(current != root)
			cout << "the encode file is corrupted file." << endl;
	}*/

	~HuffmanBiTree() { deleteTree(root); }

private:
	void deleteTree(Node *t) {
		if (t == NULL)
			return;
		deleteTree(t->left);
		deleteTree(t->right);
		delete t;
	}
};

void computeCount(int charCounts[], ifstream& inFile, ofstream& outFile);
void printAry(ofstream& outFile, int charCounts[]);

//********************************
//main class
//********************************
int main(int argc, char** argv) {
	ifstream inFile1, inFile2, inFile3;
	ofstream outFile1, outFile2, outFile3, outFile4;
	string decodeFileName;
	int charCounts[256] = {0};
	HuffmanLList *LList = new HuffmanLList();
	HuffmanBiTree *huffmanBinTree = new HuffmanBiTree(LList);


	if (argc != 7) {
		cout << "ERROR: missing file(s) name input.\n";
		exit(1);
	}

	inFile1.open(argv[1]);
	inFile2.open(argv[2]);
	if (inFile1.fail()) {
		cout << "ERROR: counld not locate \"" << argv[1] << "\"\n";
		exit(1);
	}
	if (inFile2.fail()) {
		cout << "ERROR: counld not locate \"" << argv[2] << "\"\n";
		exit(1);
	}
	outFile1.open(argv[3]);
	outFile2.open(argv[4]);
	outFile3.open(argv[5]);
	outFile4.open(argv[6]);

	computeCount(charCounts, inFile1, outFile4);
	LList->constructHuffmanLList(charCounts, outFile4);
	huffmanBinTree->constructTree(outFile4);
	huffmanBinTree->getCode(huffmanBinTree->root, "", outFile1);
	huffmanBinTree->encode(inFile2, outFile2);

	inFile1.close();
	inFile2.close();
	outFile1.close();
	outFile2.close();
	outFile4.close();

	cout << "Enter the name of the file to be decode: ";
	cin >> decodeFileName;
	inFile3.open(decodeFileName);
	while (inFile3.fail()) {
		cout << "ERROR: the file doesn't exist.\n" << "Enter the name of the file to be decode : ";
		cin >> decodeFileName;
		inFile3.open(decodeFileName);
	}
	huffmanBinTree->decode(inFile3, outFile3, huffmanBinTree->root);
	inFile3.close();
	outFile3.close();
	return 0;
}

void computeCount(int charCounts[], ifstream& inFile, ofstream& outFile) {
	uchar charIn;
	int index;
	while(true){
		charIn = inFile.get();
		if (inFile.eof()) { break; }
		index = (int)charIn;
		if (index < 256 && index != 152)
			charCounts[index]++;
	}
	printAry(outFile, charCounts);
}

void printAry(ofstream& outFile, int charCounts[]) {
	for (int index = 0; index < 256; index++) {
		if (charCounts[index] > 0) {
			outFile << (uchar)index << " " << charCounts[index] << endl;
		}
	}
}