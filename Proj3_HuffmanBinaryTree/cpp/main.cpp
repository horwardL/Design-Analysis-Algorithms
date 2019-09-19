#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
using namespace std;
//********************************************treeNode Class***************************************************************************
class treeNode {
public:
	string chStr;
	int prob;
	treeNode *nextNode;
	treeNode *left;
	treeNode *right;
	string code;

	treeNode(string s, int p) {
		chStr = s;
		prob = p;
		nextNode = NULL;
		left = NULL;
		right = NULL;
	}

	void printNode(ofstream& outFile) {
		string line = "Node\"" + chStr + "\",\tNode's prob:" + to_string(prob);
		if (nextNode != NULL) {
			line += ",\tNextNode:\"" + nextNode->chStr + "\"";
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
//**************************************************************************************************************************************

//***********************************************linkedList Class***********************************************************************
class linkedList {
public:
	treeNode *listHead;
	linkedList() {
		listHead = NULL;
	}

	void insertNewNode(treeNode *sp, treeNode *nN) {
		treeNode *spot = findSpot(sp, nN);
		nN->nextNode = spot->nextNode;
		spot->nextNode = nN;
	}

	void printList(treeNode *lh, ofstream& outFile) {
		treeNode *spot = lh;
		string list = "listHead -->";
		while (spot->nextNode != NULL) {
			list += "(\"" + spot->chStr + "\", " + to_string(spot->prob) + ", \"" + spot->nextNode->chStr + "\")-->";
			spot = spot->nextNode;
		}
		list += "(\"" + spot->chStr + "\", " + to_string(spot->prob) + ", \"NULL\")-->NULL\n";
		outFile << list;
	}
private:
	treeNode* findSpot(treeNode *lh, treeNode *nN) {
		treeNode *spot = lh;
		while (spot->nextNode != NULL && spot->nextNode->prob < nN->prob) {
			spot = spot->nextNode;
		}
		return spot;
	}
};
//**************************************************************************************************************************************

//**********************************************HuffmanBinaryTree***********************************************************************
class huffmanBiTree {
public:
	treeNode *root;
	linkedList *LList;

	huffmanBiTree(){
		root = NULL;
		LList = new linkedList();
	}

	void constructHuffmanLList(ifstream& inFile, ofstream& outFile) {
		treeNode *listHead = new treeNode("dummy", 0);
		treeNode *newNode;
		string line[2];
		string chr;
		int prob;

		inFile >> chr >> prob;
		string cchr = chr;
		newNode = new treeNode(chr, prob);
		LList->insertNewNode(listHead, newNode);
		while (!inFile.eof()) {
			inFile >> chr >> prob;
			if (chr.compare(cchr) != 0) {
				cchr = chr;
				newNode = new treeNode(chr, prob);
				LList->insertNewNode(listHead, newNode);
			}
		}
		inFile.close();
		LList->printList(listHead, outFile);
		outFile << "\n";
		LList->listHead = listHead;
	}

	void constructHuffmanBinTree(treeNode *lh, ofstream &outFile) {
		treeNode *newNode;
		while (lh->nextNode->nextNode != NULL) {
			newNode = new treeNode(lh->nextNode->chStr + lh->nextNode->nextNode->chStr,
									lh->nextNode->prob + lh->nextNode->nextNode->prob);
			newNode->left = lh->nextNode;
			newNode->right = lh->nextNode->nextNode;
			LList->insertNewNode(lh, newNode);
			lh->nextNode = lh->nextNode->nextNode->nextNode;
			LList->printList(lh, outFile);
			newNode->printNode(outFile);
		}
		root = lh->nextNode;
	}

	void getCode(treeNode *t, string code, ofstream &outFile) {
		if (t->isLeaf()) {
			t->code = code;
			outFile << t->chStr + "  " + t->code << endl;
		}else {
			getCode(t->left, code + "0", outFile);
			getCode(t->right, code + "1", outFile);
		}
	}

	void preOrderTraveral(treeNode *t, ofstream &outFile) {
		if (t == NULL)
			return;
		outFile << t->chStr + "\t" + to_string(t->prob) << endl;
		preOrderTraveral(t->left, outFile);
		preOrderTraveral(t->right, outFile);
	}

	void inOrderTraveral(treeNode *t, ofstream &outFile) {
		if (t == NULL)
			return;
		inOrderTraveral(t->left, outFile);
		outFile << t->chStr + "\t" + to_string(t->prob) << endl;
		inOrderTraveral(t->right, outFile);
	}

	void postOrderTraveral(treeNode *t, ofstream &outFile) {
		if (t == NULL)
			return;
		postOrderTraveral(t->left, outFile);
		postOrderTraveral(t->right, outFile);
		outFile << t->chStr + "\t" + to_string(t->prob) << endl;
	}

	~huffmanBiTree() {
		deleteTree(root);
	}

private:
	void deleteTree(treeNode *t) {
		if (t == NULL)
			return;
		deleteTree(t->left);
		deleteTree(t->right);
		delete t;
	}
};
//****************************************************************************************************************************************

int main(int argc, char** argv) {
	ifstream inFile;
	huffmanBiTree *tree = new huffmanBiTree();

	inFile.open(argv[1]);
	if (inFile.fail()) {
		cerr << "ERROR: counld not locate the file \"" << argv[1] << "\"" << endl;
		exit(1);
	}

	ofstream outFile1, outFile2, outFile3, outFile4, outFile5;
	outFile1.open(argv[2]);
	outFile1 << "char code\n";
	outFile2.open(argv[3]);
	outFile2 << "preOrder Traveral\n";
	outFile3.open(argv[4]);
	outFile3 << "inOrder Traveral\n";
	outFile4.open(argv[5]);
	outFile4 << "postOrder Traveral\n";
	outFile5.open(argv[6]);

	tree->constructHuffmanLList(inFile, outFile5);
	tree->constructHuffmanBinTree(tree->LList->listHead, outFile5);
	tree->getCode(tree->root, "", outFile1);
	tree->preOrderTraveral(tree->root, outFile2);
	tree->inOrderTraveral(tree->root, outFile3);
	tree->postOrderTraveral(tree->root, outFile4);

	outFile1.close();
	outFile2.close();
	outFile3.close();
	outFile4.close();
	outFile5.close();
	return 0;
}