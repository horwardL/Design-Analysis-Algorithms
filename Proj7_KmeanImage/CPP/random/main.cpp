#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
using namespace std;

//***********************************************************************
class Point{
private:
	int Xcoord;
	int Ycoord;
	int lable;
	double distance;
	friend class Kmean;

public:
	Point() {
		Xcoord = -1;
		Ycoord = -1;
		lable = 0;
		distance = 9999.0;
	}
};

//****************************************************************************
class xyCoord {
private:
	double Xcoord;
	double Ycoord;
	int lable;
	friend class Kmean;

public:
	xyCoord() {
		Xcoord = 0;
		Ycoord = 0;
		lable = 0;
	}
};

//************************************************************************
class Kmean {
private:
	int k;
	int numPts;
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int change;
	xyCoord **Kcentroids;
	Point **pointSet;
	int **imgAry;
	int *totalPt;

	void deletePointSet() {
		for (int i = 0; i < numPts; ++i)
			pointSet[i] = NULL;
		delete pointSet;
	}

	void deleteKcentroids() {
		for (int i = 0; i <= k; ++i)
			Kcentroids[i] = NULL;
		delete Kcentroids;
	}

public:
	Kmean(int numk) {
		k = numk;
		numPts = 0;
		numRows = 0;
		numCols = 0;
		minVal = -1;
		maxVal = -1;
		change = 0;
		totalPt = new int[k + 1];
		totalPt[0] = 0;
		Kcentroids = new xyCoord*[k + 1];
		Kcentroids[0] = NULL;
		for (int i = 1; i <= k; ++i) {
			Kcentroids[i] = new xyCoord();
			Kcentroids[i]->lable = i;
			totalPt[i] = 0;
		}
	}
//-------------------------------------------------------
	void extractPts(ifstream& inFile, ofstream& outFile) {
		int val;
		inFile >> numRows;
		inFile >> numCols;
		inFile >> minVal;
		inFile >> maxVal;
		for (int i = 0; i < numRows; ++i) {
			for (int j = 0; j < numCols; ++j) {
				inFile >> val;
				if (inFile.eof()) {
					cout << "ERROR: corrupted image file.\n";
					exit(1);
				}
				if (val > 0) {
					++numPts;
					outFile << i << "\t" << j << endl;
				}
			}
		}
		imgAry = new int*[numRows];
		for (int r = 0; r < numRows; ++r) {
			imgAry[r] = new int[numCols];
			for (int c = 0; c < numCols; ++c)
				imgAry[r][c] = 0;
		}
	}
//-------------------------------------------------------
	void loadPointSet(ifstream& outFile1) {
		pointSet = new Point*[numPts];
		for (int numP = 0; numP < numPts; ++numP) {
			pointSet[numP] = new Point();
			outFile1 >> pointSet[numP]->Xcoord;
			outFile1 >> pointSet[numP]->Ycoord;
		}
	}
//-------------------------------------------------------
	void kMeanClustering(ofstream& outFile) {
		int iteration = 0;

		assignLable();
		do {
			change = 0;
			computeCentroids();
			point2Image();
			printImage(iteration, outFile);
			for (int index = 0; index < numPts; ++index) {
				distanceMinLable(pointSet[index]);
			}
			++iteration;
		} while (change > 0);
	}
//-------------------------------------------------------
	/*  
	-divide in to k part
	-first numberPoint / k is 1

	void assignLable() {
		int div = numPts / k;
		int index = 0;
		int p = 0;
		for (int i = 1; i <= k; ++i) {
			p = 0;
			while(index < numPts) {
				if (p == div && i < k) { break; }
				pointSet[index]->lable = i;
				++index;
				++p;
			}
		}
	}
	*/

	//-randomly assign labels evenly k

	void assignLable(){
		int div = numPts / k;
		int index = 0;
		int label = 1;
		int d = 0;
		while(label < k){
			d = 0;
			while(d < div){
				index = rand() % numPts;
				if(pointSet[index]->lable == 0){
					pointSet[index]->lable = label;
					++d;
				}
			}
			++label;
		}
		for(int index = 0; index<numPts; ++index){
			if(pointSet[index]->lable == 0){
				pointSet[index]->lable = label;
			}
		}
	}
	
	/*
		-if k = 4
		-label each point as below
		-[1,3,1,3,...,4,2,4,2]
		
	void assignLable(){
		int front = 0;
		int back = numPts-1;
		int label = 1;
		while(front <= back){
			if(label > k){ label = 1;}
			pointSet[front++]->lable = label++;
			if (label > k) { label = 1; }
			pointSet[back--]->lable = label++;
		}
	}
	*/
	
//-------------------------------------------------------
	void point2Image() {
		int r = 0;
		int c = 0;
		for (int index = 0; index < numPts; ++index) {
			r = pointSet[index]->Xcoord;
			c = pointSet[index]->Ycoord;
			imgAry[r][c] = pointSet[index]->lable;
		}
	}
//-------------------------------------------------------
	void printImage(int iteration, ofstream& outFile) {
		outFile << "**** Result of iteration " << iteration << "****\n";
		for (int i = 0; i < numRows; ++i) {
			for (int j = 0; j < numCols; ++j) {
				if (imgAry[i][j] > 0)
					outFile << imgAry[i][j] << " ";
				else
					outFile << "  ";
			}
			outFile << endl;
		}
	}
//-------------------------------------------------------
	void computeCentroids() {
		int lable = 0;
		for (int i = 1; i <= k; ++i) {
			Kcentroids[i]->Xcoord = 0;
			Kcentroids[i]->Ycoord = 0;
			totalPt[i] = 0;
		}
		for (int index = 0; index < numPts; ++index) {
			lable = pointSet[index]->lable;
			Kcentroids[lable]->Xcoord += pointSet[index]->Xcoord;
			Kcentroids[lable]->Ycoord += pointSet[index]->Ycoord;
			++totalPt[lable];
		}
		for (int l = 1; l <= k; ++l) {
			if (totalPt[l] > 0) {
				Kcentroids[l]->Xcoord /= totalPt[l];
				Kcentroids[l]->Ycoord /= totalPt[l];
			}
			else {
				Kcentroids[l]->Xcoord = -1;
				Kcentroids[l]->Ycoord = -1;
			}
		}
	}
//-------------------------------------------------------
	void distanceMinLable(Point *point) {
		double minDist = 9999.0;
		int minLable = 0;
		double dist = 0.0;
		for (int lable = 1; lable <= k; ++lable) {
			dist = computeDist(point, Kcentroids[lable]);
			if (dist < minDist) {
				minLable = lable;
				minDist = dist;
			}
		}
		if (minLable != point->lable) {
			++change;
			point->distance = minDist;
			point->lable = minLable;
		}
	 }
//-------------------------------------------------------
	double computeDist(Point *point, xyCoord *centroid) {
		int x1 = point->Xcoord;
		int y1 = point->Ycoord;
		int x2 = centroid->Xcoord;
		int y2 = centroid->Ycoord;
		if (x2 < 0 || y2 < 0)
			return 9999.0;
		return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	}
//-------------------------------------------------------
	void writePtSet(ofstream& outFile) {
		outFile << numPts << endl << numRows << " " << numCols << endl;
		for (int l = 0; l < numPts; ++l) {
			outFile << pointSet[l]->Xcoord << "\t" << pointSet[l]->Ycoord << "\t" << pointSet[l]->lable << endl;
		}
	}
//-------------------------------------------------------
	~Kmean() {
		delete totalPt;
		delete imgAry;
		deletePointSet();
		deleteKcentroids();
	}
};

//**********************************************************************
int main(int argc, char **argv) {
	srand(time(0));
	ifstream inFile, inFile1;
	ofstream outFile1, outFile2, outFile3;
	int k = 0;
	if (argc < 4) {
		cout << "ERROR: missing file(s) name input.\n";
	}
	inFile.open(argv[1]);
	if (inFile.fail()) {
		cout << "ERROR: could not locate \"" << argv[1] << "\"\n";
		exit(1);
	}
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	outFile3.open(argv[4]);
	cout << "Enter number of k: ";
	cin >> k;
	while (k <= 0) {
		cout << "Enter a k that is greater than 0: ";
		cin >> k;
	}
	Kmean *kmean = new Kmean(k);
	kmean->extractPts(inFile, outFile1);
	inFile.close();
	outFile1.close();
	inFile1.open(argv[2]);
	kmean->loadPointSet(inFile1);
	kmean->kMeanClustering(outFile2);
	kmean->writePtSet(outFile3);
	outFile2.close();
	outFile3.close();
}
