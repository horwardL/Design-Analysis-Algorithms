public class listNode {
	private String chStr;
	private int prob;
	private listNode nextNode;
	
	public listNode() {
		chStr = "";
		prob = 0;
		nextNode = null;
	}
	
	public listNode(String s, int p, listNode nN) {
		chStr = s;
		prob = p;
		nextNode = nN;
	}
	
	public String getStr() {
		return chStr;
	}
	
	public int getProb() {
		return prob;
	}
	
	public listNode getNext() {
		return nextNode;
	}
	
	public void setNext(listNode nN) {
		nextNode = nN;
	}
}
