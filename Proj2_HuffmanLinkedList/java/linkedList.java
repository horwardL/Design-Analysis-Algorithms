import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class linkedList {
	
	public linkedList() {
	}
	
	public void insertNewNode(listNode lh, listNode nN) {
		listNode spot = findSpot(lh, nN);
		nN.setNext(spot.getNext());
		spot.setNext(nN);
	}
	
	private listNode findSpot(listNode lh, listNode nN) {
		listNode spot = lh;
		while(spot.getNext() != null && spot.getNext().getProb() < nN.getProb()) {
			spot = spot.getNext();
		}
		return spot;
	}
	
	public void printList(listNode lh, String outfName) {
		listNode spot = lh;
		String list = "listHead -->";
		while(spot.getNext() != null) {
			list+="(\""+spot.getStr()+"\", "+ spot.getProb() +", \""+spot.getNext().getStr()+"\")-->";
			spot = spot.getNext();
		}
		list+="(\""+spot.getStr()+"\", "+ spot.getProb() +", \"NULL\")-->NULL";
		
		try {
			PrintWriter outFile = new PrintWriter(new BufferedWriter(new FileWriter(new File(outfName))));
			outFile.printf(list);
			outFile.close();
		} catch (IOException e) {
			System.out.println("ERROR: " + e);
		}
	}
}
