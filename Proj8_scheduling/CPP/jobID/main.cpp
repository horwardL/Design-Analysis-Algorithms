#include <iostream>
#include <fstream>
using namespace std;
//************************************
class Node {
private:
	int jobID;
	int jobTime;
	Node *next;
	friend class LinkedList;
	friend class Schedule;
//____________________________________________________________
public:
	Node() {
		jobID = 0;
		jobTime = 0;
		next = NULL;
	}
};
//************************************
class LinkedList {
private:
	Node *listHead;
	friend class Schedule;
//____________________________________________________________
	LinkedList() {
		listHead = new Node();
	}

	int isEmpty() {
		return listHead->next == NULL;
	}
};
//************************************
class Schedule {
private:
	int numNodes;
	int totalJobTimes;
	int procGiven;
	int procUsed;
	int currentTime;
	int maxProcUsed;
	int **adjacencyMatrix;
	int **scheduleTable;
	//i = node's ID
	//jobTimeAry = adjacencyMatrix[i][i]
	LinkedList *OPEN;
	int *processJob;
	int *processTime;
	int *parentCount;
	//kidCount = adjacencyMatrix[i][0]
	//jobDone adjacencyMatrix[0][i]
	int *jobMarked;
	friend class LinkedList;
//____________________________________________________________
	void allocateArrays(ofstream &outFile) {
		//debugging
		for (int i = 0; i <= numNodes; ++i) {
			for (int j = 0; j <= numNodes; ++j) {
				outFile << adjacencyMatrix[i][j] << " ";
			}
			if (i == 0)
				outFile << "<-- jobDone";
			outFile << endl;
		}
		outFile << "^ kidCount\n";
		outFile << "\nTotal Job Time: " << totalJobTimes << endl;
		outFile << "\nGiven Porcessor(s): ";
		if (procGiven <= -1)
			outFile << "Unlimited\n";
		else
			outFile << procGiven << endl;

		if (procGiven > numNodes || procGiven == -1)
			procGiven = numNodes;

		scheduleTable = new int*[procGiven + 1];
		for (int i = 0; i <= procGiven; ++i) {
			scheduleTable[i] = new int[totalJobTimes + 1];
			for (int j = 0; j <= totalJobTimes; ++j) {
				scheduleTable[i][j] = 0;
			}
		}

		processJob = new int[numNodes + 1];
		processTime = new int[numNodes + 1];
		parentCount = new int[numNodes + 1];
		jobMarked = new int[numNodes + 1];
		outFile << "jobMarked\t\t\tprocessTime\t\t\tprocessJob\t\t\tjobDone\n";
		for (int i = 0; i <= numNodes; ++i) {
			processJob[i] = 0;
			processTime[i] = 0;
			parentCount[i] = 0;
			for (int j = 1; j <= numNodes; ++j) {
				if (i != j) { parentCount[i] += adjacencyMatrix[j][i]; }
			}
			jobMarked[i] = 0;
			//debugging parenCount
			if (i > 0 && i < 10) {
				outFile << "[" << i << "] = " << jobMarked[i];
				outFile << "\t\t\t\t[" << i << "] = " << processTime[i];
				outFile << "\t\t\t\t[" << i << "] = " << processJob[i];
				outFile << "\t\t\t\t[" << i << "] = " << adjacencyMatrix[0][i] << endl;
			}
			else if(i >= 10){
				outFile << "[" << i << "] = " << jobMarked[i];
				outFile << "\t\t\t[" << i << "] = " << processTime[i];
				outFile << "\t\t\t[" << i << "] = " << processJob[i];
				outFile << "\t\t\t[" << i << "] = " << adjacencyMatrix[0][i] << endl;
			}
		}
	}
//____________________________________________________________
	void deleteJobNode(Node *node) {
		node->next = NULL;
		delete node;
	}
//____________________________________________________________
	~Schedule() {
		delete adjacencyMatrix;
		delete scheduleTable;
		delete OPEN;
		delete processJob;
		delete processTime;
		delete parentCount;
		delete jobMarked;
	}

public:
//____________________________________________________________
	Schedule() {
		numNodes = 0;
		totalJobTimes = 0;
		procGiven = 0;
		procUsed = 0;
		currentTime = 0;
		maxProcUsed = 0;
		OPEN = new LinkedList();
	}
//____________________________________________________________
	void loadMatrix(ifstream &inFile) {
		int nodeL = 0;
		int nodeR = 0;
		inFile >> numNodes;
		//initialize adjacencyMatrix
		adjacencyMatrix = new int*[numNodes + 1];
		for (int i = 0; i <= numNodes; ++i) {
			adjacencyMatrix[i] = new int[numNodes + 1];
			for (int j = 0; j <= numNodes; ++j) {
				//jobDone initial to 0
				adjacencyMatrix[i][j] = 0;
			}
		}
		//read inFile1
		while (true) {
			inFile >> nodeL >> nodeR;
			if (inFile.eof()) { break; }
			adjacencyMatrix[nodeL][nodeR] = 1;
			//kidCount
			++adjacencyMatrix[nodeL][0];
		}
	}
//____________________________________________________________
	void computeTotalJobTimes(ifstream &inFile) {
		int time = 0;
		int node;
		int numN = 0;
		inFile >> numN;
		while (true) {
			inFile >> node >> time;
			if (inFile.eof()) { break; }
			//jobTimeAry
			adjacencyMatrix[node][node] = time;
			totalJobTimes += time;
		}
	}
//____________________________________________________________
	void getNumProcessor() {
		cout << "How many processors are needed for this scheduling\n";
		cout << "If the precessors is unlimited enter '-1': ";
		cin >> procGiven;
		while (procGiven <= 0 && procGiven != -1) {
			cout << "Enter a number greater than 0 or '-1': ";
			cin >> procGiven;
		}
	}
//____________________________________________________________
	void scheduling(ofstream &outFile1, ofstream &outFile2) {
		allocateArrays(outFile2);
		int orphenNode = 0;
		int availProc = 0;
		int job = 0;
		Node *newNode;
		Node *newJob;

		while (allJobDone() < 1) {
			orphenNode = getUnMarkOrphen();
			while (orphenNode > 0) {
				jobMarked[orphenNode] = 1;
				newNode = new Node();
				newNode->jobID = orphenNode;
				newNode->jobTime = adjacencyMatrix[orphenNode][orphenNode];
				insert2Open(newNode);
				orphenNode = getUnMarkOrphen();
			}
			printOPENList();
			while (!OPEN->isEmpty() && procUsed < procGiven) {
				availProc = findProcessor();
				if (availProc > 0) {
					++procUsed;
					newJob = getFirstJob();
					processJob[availProc] = newJob->jobID;
					processTime[availProc] = newJob->jobTime;
					updateTable(availProc, newJob);
					deleteJobNode(newJob);
				}
			}
			//debugging
			printScheduleTable();

			if (checkCycle() >= 1) {
				cout << "\nERROR: There is cycle in the graph\n";
				outFile1 << "\nERROR: There is cycle in the graph\n";
				outFile2 << "\nERROR: There is cycle in the graph\n";
				return;
			}
			outputDebugging(outFile2);
 			printTable(outFile1);
			++currentTime;
			decreseProcTime();
			job = findDoneJob();
			while (job > 0) {
				--procUsed;
				deleteNode(job);
				deleteEdge(job);
				job = findDoneJob();
			}
			
			outputDebugging(outFile2);
		}
		printTable(outFile1);
	}
//____________________________________________________________
	void printScheduleTable() {
		cout << endl;
		for (int i = 1; i <= procGiven; ++i) {
			for (int j = 0; j < totalJobTimes; ++j) {
				cout << scheduleTable[i][j] << " ";
			}
			cout << endl;
		}
	}
//____________________________________________________________
	int getUnMarkOrphen() {
		for (int i = 1; i <= numNodes; ++i) {
			if (jobMarked[i] == 0 && parentCount[i] == 0)
				return i;
		}
		return -1;
	}
//_____________________________________________________________
	void insert2Open(Node *newNode) {
		Node *spot = findSpot(newNode);
		newNode->next = spot->next;
		spot->next = newNode;
	}
//____________________________________________________________
	Node *findSpot(Node *newNode) {
		Node *spot = OPEN->listHead;
		while (spot->next != NULL && spot->next->jobID < newNode->jobID) {
			spot = spot->next;
		}
		return spot;
	}
//____________________________________________________________
	void printOPENList() {
		Node *spot = OPEN->listHead;
		cout << "\nOPEN: ";
		if (spot != OPEN->listHead || spot->next != NULL) {
			spot = spot->next;
			while (spot->next != NULL) {
				cout << spot->jobID << ", ";
				spot = spot->next;
			}
			cout << spot->jobID;
		}
		cout << "\ntime: " << currentTime << endl;
	}
//____________________________________________________________
	int findProcessor() {
		for (int i = 1; i <= procGiven; ++i) {
			if (processTime[i] <= 0)
				return i;
		}
		return -1;
	}
//____________________________________________________________
	Node *getFirstJob() {
		Node *frontJob = OPEN->listHead->next;
		OPEN->listHead->next = frontJob->next;
		return frontJob;
	}
//____________________________________________________________
	void updateTable(int availProc, Node *newJob) {
		int finalTime = currentTime + newJob->jobTime;
		for (int time = currentTime; time < finalTime; ++time) {
			scheduleTable[availProc][time] = newJob->jobID;
		}
	}
//____________________________________________________________
	int allJobDone() {
		for (int i = 1; i <= numNodes; ++i) {
			if (adjacencyMatrix[0][i] == 0) { return 0; }
		}
		return 1;
	}
//____________________________________________________________
	int checkCycle() {
		int allProcIdle = 1;
		for (int i = 1; i <= procGiven; ++i) {
			if (processTime[i] > 0) { 
				allProcIdle = 0; 
				break;
			}
		}
		if (OPEN->isEmpty() >= 1 && allJobDone() <= 0 && allProcIdle>= 1) { return 1; }
		return 0;
	}
//____________________________________________________________
	int findDoneJob() {
		int doneJobID = 0;
		for (int i = 1; i <= procGiven; ++i) {
			if (processTime[i] <= 0 && processJob[i] > 0) {
				doneJobID = processJob[i];
				processJob[i] = 0;
				return doneJobID;
			}
		}
		return doneJobID;
	}
//____________________________________________________________
	void decreseProcTime() {
		for (int i = 1; i <= procGiven; ++i) {
			if (processTime[i] > 0) {
				--processTime[i];
			}
		}
	}
//____________________________________________________________
	void deleteNode(int jobID) {
		adjacencyMatrix[0][jobID] = 1;
	}
//____________________________________________________________
	void deleteEdge(int jobID) {
		for (int kidID = 1; kidID <= numNodes; ++kidID) {
			if (adjacencyMatrix[jobID][kidID] > 0) {
				--parentCount[kidID];
			}
		}
	}
//____________________________________________________________
	void outputDebugging(ofstream &outFile) {
		outFile << "\nCurrent Time: " << currentTime << "\n-----------------\n";
		outFile << "jobMarked\t\t\tprocessTime\t\t\tprocessJob\t\t\tjobDone\n";
		for (int i = 1; i <= numNodes; ++i) {
			if (i < 10) {
				outFile << "[" << i << "] = " << jobMarked[i];
				outFile << "\t\t\t\t[" << i << "] = " << processTime[i];
				outFile << "\t\t\t\t[" << i << "] = " << processJob[i];
				outFile << "\t\t\t\t[" << i << "] = " << adjacencyMatrix[0][i] << endl;
			}
			else if (i >= 10) {
				outFile << "[" << i << "] = " << jobMarked[i];
				outFile << "\t\t\t[" << i << "] = " << processTime[i];
				outFile << "\t\t\t[" << i << "] = " << processJob[i];
				outFile << "\t\t\t[" << i << "] = " << adjacencyMatrix[0][i] << endl;
			}
		}
	}
//___________________________________________________________
	void printTable(ofstream &outFile) {
		outFile << "     ";
		for (int i = 0; i <= currentTime; ++i) {
			if (i < 10) {
				outFile << "--" << i << "--";
			}
			else {
				outFile << "-" << i << "--";
			}
		}
		outFile << ". . .\n";
		for (int i = 1; i <= procGiven; ++i) {
			outFile << "P(" << i << ")";
			for (int t = 0; t <= currentTime; ++t) {
				if (scheduleTable[i][t] == 0) {
					outFile << "| -- ";
				}
				else {
					if (scheduleTable[i][t] < 10) {
						outFile << "|  " << scheduleTable[i][t] << " ";
					}
					else {
						outFile << "| " << scheduleTable[i][t] << " ";
					}
				}
			}
			outFile << " . . .\n     ";
			for (int t = 0; t <= currentTime; ++t) {
				outFile << "-----";
			}
			outFile << endl;
		}
		outFile << endl;
	}
};
//************************************
int main(int argc, char **argv){
	ifstream inFile1, inFile2;
	ofstream outFile1, outFile2;

	if (argc < 4) {
		cout << "ERROR: need to provide 4 file name.\n(Graph, Job Time, scheduling, debugging) file name\n";
		exit(1);
	}
	inFile1.open(argv[1]);
	outFile1.open(argv[3]);
	outFile2.open(argv[4]);
	if (inFile1.fail()) {
		cout << "ERROR: cannot find \"" << argv[1] << "\"\n";
		exit(1);
	}
	inFile2.open(argv[2]);
	if (inFile2.fail()) {
		cout << "ERROR: cannot find \"" << argv[2] << "\"\n";
		exit(1);
	}

	Schedule *schedule = new Schedule();
	schedule->loadMatrix(inFile1);
	schedule->computeTotalJobTimes(inFile2);
	schedule->getNumProcessor();
	schedule->scheduling(outFile1, outFile2);

	inFile1.close();
	inFile2.close();
	outFile1.close();
	outFile2.close();
	return 0;
}