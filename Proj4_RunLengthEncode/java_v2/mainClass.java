import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class mainClass {
	public static void main(String args[]) {
		try {
			PrintWriter outFile1 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[1]))));
			PrintWriter outFile2 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[2]))));
			PrintWriter outFile3 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[3]))));
			PrintWriter outFile4 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[4]))));
			Scanner inFile = new Scanner(new File(args[0]));
			method1(inFile, outFile1);
			inFile = new Scanner(new File(args[0]));
			method2(inFile, outFile2);
			inFile = new Scanner(new File(args[0]));
			method3(inFile, outFile3);
			inFile = new Scanner(new File(args[0]));
			method4(inFile, outFile4);
		} catch (IOException e) {
			System.out.printf("ERROR: %s", e);
		}
	}

	// encode 0s, no wrap_around
	public static void method1(Scanner inFile, PrintWriter outFile) {
		imgHeader header = new imgHeader(inFile.nextInt(), inFile.nextInt(), inFile.nextInt(), inFile.nextInt());
		outFile.printf("%3d %3d %3d %3d\n", header.numRows, header.numCols, header.minVal, header.maxVal);
		int r = 0, c = 0, count = 0;
		int currentVal = -1;
		int nextVal = -1;
		for (r = 0; r < header.numRows; r++) {
			c = 0;
			count = 1;
			currentVal = inFile.nextInt();
			outFile.printf("%3d %3d %3d ", r, c, currentVal);
			while (c + 1 < header.numCols) {
				nextVal = inFile.nextInt();
				c++;
				if (nextVal != currentVal) {
					outFile.printf("%3d\n", count);
					currentVal = nextVal;
					count = 1;
					outFile.printf("%3d %3d %3d ", r, c, currentVal);
				} else
					count++;
			}
			outFile.printf("%3d\n", count);
		}
		inFile.close();
		outFile.close();
	}

	// encode 0s, wrap_around
	public static void method2(Scanner inFile, PrintWriter outFile) {
		imgHeader header = new imgHeader(inFile.nextInt(), inFile.nextInt(), inFile.nextInt(), inFile.nextInt());
		outFile.printf("%3d %3d %3d %3d\n", header.numRows, header.numCols, header.minVal, header.maxVal);
		int r = 0, c = 0, count = 0;
		int currentVal = -1;
		int nextVal = -1;

		c = 0;
		count = 1;
		currentVal = inFile.nextInt();
		outFile.printf("%3d %3d %3d ", r, c, currentVal);
		for (r = 0; r < header.numRows; r++) {
			while (c + 1 < header.numCols) {
				nextVal = inFile.nextInt();
				c++;
				if (nextVal != currentVal) {
					outFile.printf("%3d\n", count);
					currentVal = nextVal;
					count = 1;
					outFile.printf("%3d %3d %3d ", r, c, currentVal);
				} else
					count++;
			}
			c = -1;
		}
		outFile.printf("%3d", count);
		inFile.close();
		outFile.close();
	}

	// ignore 0s, no wrap_around
	public static void method3(Scanner inFile, PrintWriter outFile) {
		imgHeader header = new imgHeader(inFile.nextInt(), inFile.nextInt(), inFile.nextInt(), inFile.nextInt());
		outFile.printf("%3d %3d %3d %3d\n", header.numRows, header.numCols, header.minVal, header.maxVal);
		int r = 0, c = 0, count = 0;
		int currentVal = -1;
		int nextVal = -1;
		for (r = 0; r < header.numRows; r++) {
			c = 0;
			count = 1;
			currentVal = inFile.nextInt();
			if (currentVal != 0)
				outFile.printf("%3d %3d %3d ", r, c, currentVal);
			while (c + 1 < header.numCols) {
				nextVal = inFile.nextInt();
				c++;
				if (nextVal != currentVal) {
					if (currentVal != 0)
						outFile.printf("%3d\n", count);
					currentVal = nextVal;
					count = 1;
					if (currentVal != 0)
						outFile.printf("%3d %3d %3d ", r, c, currentVal);
				} else
					count++;
			}
			if (currentVal != 0)
				outFile.printf("%3d\n", count);
		}
		inFile.close();
		outFile.close();
	}

	// ignore 0s, wrap_around
	public static void method4(Scanner inFile, PrintWriter outFile) {
		imgHeader header = new imgHeader(inFile.nextInt(), inFile.nextInt(), inFile.nextInt(), inFile.nextInt());
		outFile.printf("%3d %3d %3d %3d\n", header.numRows, header.numCols, header.minVal, header.maxVal);
		int r = 0, c = 0, count = 0;
		int currentVal = -1;
		int nextVal = -1;

		c = 0;
		count = 1;
		currentVal = inFile.nextInt();
		if (currentVal != 0)
			outFile.printf("%3d %3d %3d ", r, c, currentVal);
		for (r = 0; r < header.numRows; r++) {
			while (c + 1 < header.numCols) {
				nextVal = inFile.nextInt();
				c++;
				if (nextVal != currentVal) {
					if (currentVal != 0)
						outFile.printf("%3d\n", count);
					currentVal = nextVal;
					count = 1;
					if (currentVal != 0)
						outFile.printf("%3d %3d %3d ", r, c, currentVal);
				} else
					count++;
			}
			c = -1;
		}
		if (currentVal != 0)
			outFile.printf("%3d", count);
		inFile.close();
		outFile.close();
	}
}
