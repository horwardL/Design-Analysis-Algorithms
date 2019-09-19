import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class mainClass {
	public static void main(String args[]) {
		huffmanBiTree tree = new huffmanBiTree();

		try {
			Scanner inFile = new Scanner(new File(args[0]));
			PrintWriter outFile1 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[1]))));
			PrintWriter outFile2 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[2]))));
			PrintWriter outFile3 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[3]))));
			PrintWriter outFile4 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[4]))));
			PrintWriter outFile5 = new PrintWriter(new BufferedWriter(new FileWriter(new File(args[5]))));
			tree.constructHuffmanLList(inFile, outFile5);
			tree.constructHuffmanBinTree(tree.LList.listHead, outFile5);
			tree.getCode(tree.root, "", outFile1);
			outFile2.println("preOrder Traveral");
			tree.preOrderTraveral(tree.root, outFile2);
			outFile3.println("inOrder Traveral");
			tree.inOrderTraveral(tree.root, outFile3);
			outFile4.println("postOrderTraveral");
			tree.postOrderTraveral(tree.root, outFile4);
			inFile.close();
			outFile1.close();
			outFile2.close();
			outFile3.close();
			outFile4.close();
			outFile5.close();
		} catch (IOException e) {
			System.out.printf("ERROR: ", e);
		}
	}
}
