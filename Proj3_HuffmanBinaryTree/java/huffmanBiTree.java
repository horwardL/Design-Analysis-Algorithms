import java.io.PrintWriter;
import java.util.Scanner;

public class huffmanBiTree {
	public treeNode root;
	public linkedList LList;

	public huffmanBiTree() {
		root = null;
		LList = new linkedList();
	}

	public void constructHuffmanLList(Scanner inFile, PrintWriter outFile) {
		treeNode listHead = new treeNode("dummy", 0);
		treeNode newNode;
		String[] line;
		String chr;
		int prob;

		while (inFile.hasNextLine()) {
			line = inFile.nextLine().split("\\s+");
			if (line.length < 2)
				break;
			chr = line[0];
			prob = Integer.parseInt(line[1]);
			newNode = new treeNode(chr, prob);
			LList.insertNewNode(listHead, newNode);
		}
		LList.printList(listHead, outFile);
		outFile.println();
		LList.listHead = listHead;
	}
	
	public void constructHuffmanBinTree(treeNode lh, PrintWriter outFile) {
		treeNode newNode;
		while (lh.nextNode.nextNode != null) {
			newNode = new treeNode(lh.nextNode.chStr + lh.nextNode.nextNode.chStr,
									lh.nextNode.prob + lh.nextNode.nextNode.prob);
			newNode.left = lh.nextNode;
			newNode.right = lh.nextNode.nextNode;
			LList.insertNewNode(lh, newNode);
			lh.nextNode = lh.nextNode.nextNode.nextNode;
			LList.printList(lh, outFile);
			LList.printNode(newNode, outFile);
		}
		root = lh.nextNode;
	}

	void getCode(treeNode t, String code, PrintWriter outFile) {
		if (t.isLeaf()) {
			t.code = code;
			outFile.printf(t.chStr + "\t" + t.code + "\n");
		}else {
			getCode(t.left, code + "0", outFile);
			getCode(t.right, code + "1", outFile);
		}
	}

	void preOrderTraveral(treeNode t, PrintWriter outFile) {
		if (t == null)
			return;
		outFile.printf(t.chStr + "\t" + t.prob + "\n");
		preOrderTraveral(t.left, outFile);
		preOrderTraveral(t.right, outFile);
	}

	void inOrderTraveral(treeNode t, PrintWriter outFile) {
		if (t == null)
			return;
		inOrderTraveral(t.left, outFile);
		outFile.printf(t.chStr + "\t" + t.prob + "\n");
		inOrderTraveral(t.right, outFile);
	}

	void postOrderTraveral(treeNode t, PrintWriter outFile) {
		if (t == null)
			return;
		postOrderTraveral(t.left, outFile);
		postOrderTraveral(t.right, outFile);
		outFile.printf(t.chStr + "\t" + t.prob + "\n");
	}
}
