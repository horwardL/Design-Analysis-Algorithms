public class treeNode {
	String chStr;
	int prob;
	treeNode nextNode;
	treeNode left;
	treeNode right;
	String code;
	
	public treeNode(String s, int p){
		chStr = s;
		prob = p;
		nextNode = null;
		left = null;
		right = null;
	}

	boolean isLeaf() {
		return left == null && right == null;
	}
}
