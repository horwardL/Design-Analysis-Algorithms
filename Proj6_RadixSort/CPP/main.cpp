#include <fstream>
#include <iostream>
using namespace std;

/***********************
 *list node class
 ***********************/
class ListNode {
public:
	int data;
	ListNode *next;

public:
	friend class LinkedListStack;
	friend class LinkedListQueue;
	friend class RadixSort;

	ListNode() {
		data = -1;
		next = NULL;
	}

	ListNode(int d) {
		data = d;
		next = NULL;
	}
};

/***********************
 *linked list stack class
 ***********************/
class LinkedListStack {
private:
	ListNode *top;

public:
	friend class RadixSort;

	LinkedListStack() {
		top = NULL;
	}

	void push(ListNode *newNode) {
		newNode->next = top;
		top = newNode;
	}

	ListNode *pop() {
		ListNode *spot = top;
		top = top->next;
		return spot;
	}

	int isEmpty() {
		return (top == NULL) ? 1 : 0;
	}

	void printStack(ofstream &outFile) {
		outFile << "TOP -> ";
		ListNode *spot = top;
		while (spot->next != NULL) {
			outFile << "( " << spot->data << ", " << spot->next->data << ") ->";
			spot = spot->next;
		}
		outFile << "( " << spot->data << ", NULL) -> NULL\n\n";
	}
};

/***********************
 *linked list queue class
 ***********************/
class LinkedListQueue {
private:
	ListNode *head;
	ListNode *tail;

	void deleteQueue(ListNode *n) {
		if (n == NULL)
			return;
		deleteQueue(n->next);
		delete n;
	}

public:
	friend class RadixSort;

	LinkedListQueue() {
		head = tail = NULL;
	}

	void addTail(ListNode *newTail) {
		if (head != NULL)
			tail->next = newTail;
		else
			head = newTail;
		tail = newTail;
		tail->next = NULL;
	}

	ListNode *deleteFront() {
		if (isEmpty() == 1)
			return NULL;
		ListNode *fNode = head;
		head = head->next;
		if (head == NULL)
			tail = NULL;
		return fNode;
	}

	int isEmpty() {
		return (head == NULL) ? 1 : 0;
	}

	void printQueue(int index, ofstream &outFile) {
		outFile << "Front(" << index << ") ->";
		ListNode *spot = head;
		while (spot->next != NULL) {
			outFile << "( " << spot->data << ", " << spot->next->data << ") ->";
			spot = spot->next;
		}
		outFile << "( " << spot->data << ", NULL) -> NULL\n";
		outFile << "Tail(" << index << ") -> ( " << spot->data << ", NULL) -> NULL\n\n";
	}

	void printData(ofstream &outFile) {
		ListNode *spot = head;
		bool oneData = true;
		while (spot->next != NULL) {
			outFile << spot->data << ", ";
			spot = spot->next;
			oneData = false;
		}
		if (!oneData)
			outFile << spot->data;
		else
			outFile << ", " << spot->data;
	}

	~LinkedListQueue() { deleteQueue(head); }
};

class RadixSort {
private:
	int tableSize = 10;
	LinkedListQueue *hashTable[2][10];
	int currentTable;
	int previousTable;
	int maxDigit;
	int currentDigit;
	int sortStep;

	void loadStack(ifstream &inFile, LinkedListStack *stack, ofstream &outFile) {
		int largestNum = 0;
		ListNode *newNode;
		while (true) {
			int d;
			inFile >> d;
			if (inFile.eof()) break;
			largestNum = (d > largestNum) ? d : largestNum;
			newNode = new ListNode(d);
			stack->push(newNode);
		}
		maxDigit = getMaxDigit(largestNum);
		stack->printStack(outFile);
	}

	void dumpStack(LinkedListStack *stack, int currentDigit, int currentTable) {
		ListNode *node;
		int index;
		while (stack->isEmpty() == 0) {
			node = stack->pop();
			index = getDigit(node->data, currentDigit);
			hashTable[currentTable][index]->addTail(node);
		}
	}

	void dumpQueue(LinkedListQueue *queue, int currentDigit, int currentTable) {
		ListNode *node;
		int index;
		while (queue->isEmpty() == 0) {
			node = queue->deleteFront();
			index = getDigit(node->data, currentDigit);
			hashTable[currentTable][index]->addTail(node);
		}
	}

	int getMaxDigit(int d) {
		if (d < 10)
			return 1;
		return 1 + getMaxDigit(d / 10);
	}

	int getDigit(int num, int p) {
		if (p == 0)
			return num % 10;
		return getDigit(num / 10, p - 1);
	}

	void printTable(LinkedListQueue **table, ofstream &outFile) {
		for (int index = 0; index < tableSize; ++index) {
			if (!table[index]->isEmpty()) {
				table[index]->printQueue(index, outFile);
			}
		}
	}

	void printSortedData(LinkedListQueue **table, ofstream &outFile) {
		outFile << "Sorted Data: ";
		for (int index = 0; index < tableSize; ++index) {
			if (!table[index]->isEmpty()) {
				table[index]->printData(outFile);
			}
		}
	}

public:
	RadixSort(ifstream &inFile, ofstream &outFile1, ofstream &outFile2) {
		LinkedListQueue *queue;
		for (int r = 0; r < 2; ++r) {
			for (int index = 0; index < 10; ++index) {
				queue = new LinkedListQueue();
				hashTable[r][index] = queue;
			}
		}

		LinkedListStack *stack = new LinkedListStack();
		currentDigit = 0;
		currentTable = 0;
		sortStep = 0;
		outFile2 << "Stack:\n------------------------------\n";
		loadStack(inFile, stack, outFile2);
		dumpStack(stack, currentDigit++, currentTable);
		outFile2 << "Radix Sort Steps\n\n";
		outFile2 << "Step " << sortStep++ << ":\n------------------------------\n";
		printTable(hashTable[currentTable], outFile2);

		while (currentDigit < maxDigit) {
			if (currentTable == 0) {
				currentTable = 1;
				previousTable = 0;
			}
			else {
				currentTable = 0;
				previousTable = 1;
			}
			for (int currentQ = 0; currentQ < 10; ++currentQ) {
				if (hashTable[previousTable][currentQ]->isEmpty() == 0) {
					dumpQueue(hashTable[previousTable][currentQ], currentDigit, currentTable);
				}
			}
			currentDigit++;
			outFile2 << "Step " << sortStep++ << ":\n------------------------------\n";
			printTable(hashTable[currentTable], outFile2);
		}
		printTable(hashTable[currentTable], outFile1);
		printSortedData(hashTable[currentTable], outFile1);
	}
};

int main(int argc, char **argv) {
	ifstream inFile1;
	ofstream outFile1, outFile2;
	if (argc < 4) {
		cout << "ERROR: missing file(s) name input.\n";
		exit(1);
	}
	inFile1.open(argv[1]);
	if (inFile1.fail()) {
		cout << "ERROR: could not locate \"" << argv[1] << "\"\n";
		exit(1);
	}
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);

	new RadixSort(inFile1, outFile1, outFile2);

	inFile1.close();
	outFile1.close();
	outFile2.close();
	return 0;
}