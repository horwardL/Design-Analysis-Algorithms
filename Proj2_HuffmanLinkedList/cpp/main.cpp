#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
using namespace std;

class listNode {
private:
	string chStr;
	int prob;
	listNode *nextNode;

public:
	listNode() {
		chStr = "";
		prob = 0;
		nextNode = NULL;
	}

	listNode(string s, int p, listNode *nN) {
		chStr = s;
		prob = p;
		nextNode = nN;
	}

	string getStr() {
		return chStr;
	}

	int getProb() {
		return prob;
	}

	listNode *getNext() {
		return nextNode;
	}

	void setNext(listNode* nN) {
		nextNode = nN;
	}
};

class linkedList {
public:
	linkedList(){}

	void insertNewNode(listNode *lh, listNode *nN) {
		listNode *spot = findSpot(lh, nN);
		nN->setNext(spot->getNext());
		spot->setNext(nN);
	}

	void printList(listNode *lh, string outfName) {
		listNode *spot = lh;
		ofstream outFile;
		outFile.open(outfName);
		string list = "listHead -->";
		while (spot->getNext() != NULL) {
			list+= "(\"" + spot->getStr() + "\", " + to_string(spot->getProb()) + ", \"" + spot->getNext()->getStr() + "\")-->";
			spot = spot->getNext();
		}
		list += "(\"" + spot->getStr() + "\", " + to_string(spot->getProb()) + ", \"NULL\")-->NULL";
		outFile << list;
		outFile.close();
	}

private:
	listNode* findSpot(listNode *lh, listNode *nN) {
		listNode *spot = lh;
		while (spot->getNext() != NULL && spot->getNext()->getProb() < nN->getProb()) {
			spot = spot->getNext();
		}
		return spot;
	}
};

int main(int argc, char** argv) {
	listNode *listHead = new listNode("dummy", 0, NULL);
	listNode *newNode;
	linkedList *LList = new linkedList();
	string line[2];
	string chr;
	int prob;
	ifstream inFile;
	inFile.open(argv[1]);
	if (inFile.fail()) {
		cerr << "ERROR: counld not locate the file \"" << argv[1] << "\"" << endl;
		exit(1);
	}
	inFile >> chr >> prob;
	string cchr = chr;
	newNode = new listNode(chr, prob, NULL);
	LList->insertNewNode(listHead, newNode);
	while (!inFile.eof()) {
		inFile >> chr >> prob;
		if (chr.compare(cchr) != 0) {
			cchr = chr;
			newNode = new listNode(chr, prob, NULL);
			LList->insertNewNode(listHead, newNode);
		}
	}
	if(argc >= 3)
		LList->printList(listHead, argv[2]);
	else {
		cerr << "ERROR: Didn't specified output file name.\n";
		LList->printList(listHead, "outputCPP.txt");
		cerr << "Default output file name used: 'outputJava.txt'";
	}
	return 0;
}