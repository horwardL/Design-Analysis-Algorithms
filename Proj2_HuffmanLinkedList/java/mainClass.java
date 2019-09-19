import java.io.*;
import java.util.Scanner;

public class mainClass {
	public static void main(String args[]) {
		listNode listHead = new listNode ("dummy", 0, null);
		listNode newNode = new listNode();
		linkedList LList = new linkedList();
		String[] line;
		String chr;
		int prob;
		
		try {
			Scanner inFile = new Scanner(new File(args[0]));
			while(inFile.hasNextLine()) {
				line = inFile.nextLine().split("\\s+");
				if(line.length < 2)
					break;
				chr = line[0];
				prob = Integer.parseInt(line[1]);
				newNode = new listNode(chr, prob, null);
				LList.insertNewNode(listHead, newNode);
			}
			inFile.close();
		}catch (IOException e) {
				System.out.printf("ERROR: Couldn't locate input file \"%s\"", args[0]);
		}
		
		try {
			LList.printList(listHead, args[1]);
		}catch (ArrayIndexOutOfBoundsException e) {
			System.out.printf("ERROR: Didn't specified output file name.\n");
			LList.printList(listHead, "outputJava.txt");
			System.out.printf("Default output file name used: 'outputJava.txt'");
		}
	}
}
