import java.io.PrintWriter;

public class linkedList {
	public treeNode listHead;

	linkedList() {
		listHead = null;
	}

	public void insertNewNode(treeNode lh, treeNode nN) {
		treeNode spot = findSpot(lh, nN);
		nN.nextNode = spot.nextNode;
		spot.nextNode = nN;
	}

	private treeNode findSpot(treeNode lh, treeNode nN) {
		treeNode spot = lh;
		while (spot.nextNode != null && spot.nextNode.prob < nN.prob) {
			spot = spot.nextNode;
		}
		return spot;
	}

	public void printList(treeNode lh, PrintWriter outFile) {
		treeNode spot = lh;
		String list = "listHead -->";
		while (spot.nextNode != null) {
			list += "(\"" + spot.chStr + "\", " + spot.prob + ", \"" + spot.nextNode.chStr + "\")-->";
			spot = spot.nextNode;
		}
		list += "(\"" + spot.chStr + "\", " + spot.prob + ", \"NULL\")-->NULL\n";
		outFile.printf(list);
	}

	public void printNode(treeNode t, PrintWriter outFile) {
		String line = "Node\"" + t.chStr + "\",\tNode's prob:" + t.prob;
		if (t.nextNode != null) {
			line += ",\tNextNode:\"" + t.nextNode.chStr + "\"";
		} else {
			line += ",\tNextNode:NULL";
		}
		if (!t.isLeaf()) {
			line += ",\tleft:\"" + t.left.chStr + "\",\tright:\"" + t.right.chStr + "\"\n\n";
		} else {
			line += ",\tleft: NULL,\t right: NULL\n\n";
		}
		outFile.printf(line);
	}
}
