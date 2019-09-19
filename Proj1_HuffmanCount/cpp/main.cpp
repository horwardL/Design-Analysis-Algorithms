#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

void printAry(int charCountsAry[]);
void outResult(string outfName,int charCountsAry[]);

int main(int argc, char** argv) {
	char charInt;
	int charCounts[256] = { 0 };
	ifstream fin;
	fin.open(argv[1]);
	if (fin.fail()) {
		cerr << "ERROR: counld not locate \"" << argv[1] << "\"" << endl;
		exit(1);
	}
	while (fin.get(charInt)) {		//don't want to use >> for reading file 
		if ((int)charInt < 256) {	//because it will skip whitespace and enter which are also in ASCII
			charCounts[(int)charInt]++;
		}
		else {
			cout << "ERROR: \'" << charInt << "\' is not included in ASCII" << endl;
		}
	}
	fin.close();
	printAry(charCounts);
	outResult(argv[2], charCounts);
	return 0;
}

void printAry(int charCountsAry[]) {
	for (int i = 0; i < 256; i++) {
		if (charCountsAry[i] > 0) {
			printf("%c <-- %d\n", (char)i, charCountsAry[i]);
		}
	}
}

void outResult(string outfName, int charCountsAry[]) {
	ofstream fout;
	fout.open(outfName);
	for (int i = 0; i < 256; i++) {
		if (charCountsAry[i] > 0) {
			fout << (char)i << " \t# " << charCountsAry[i] << endl;
		}
	}
	fout.close();
}
