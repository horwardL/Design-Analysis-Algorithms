import java.io.*;


public class mainClass {
	public static void main(String args[]){
		char charInt;
		int index;
		int[] charCounts = new int[256];
		
		try {
			FileInputStream inFile = new FileInputStream(new File(args[0]));
			while(inFile.available() > 0) {
				charInt = (char) inFile.read();
				index = (int)charInt;
				if(index > 255) {
					System.out.printf("ERROR: the character '%s' in the file is not included in ASCII", charInt);
					System.exit(0);
				}
				charCounts[index]++;
			}
			printAry(charCounts);
			inFile.close();
		} catch (IOException e) {
			System.out.printf("ERROR: Couldn't locate input file \"%s\"", args[0]);
		}
		outResult(args[1], charCounts);
	}
	
	private static void printAry(int[] charCountsAry) {
		for(int i=0; i<256; i++) {
			//if(charCountsAry[i] > 0) {
				System.out.printf("%d:%c <-- %d\n", i, (char)i, charCountsAry[i]);
			//}
		}
		char ch=(char) -10;;
		System.out.println(ch);
	}
	
	private static void outResult(String outfName, int[] charCountsAry) {
		try {
			PrintWriter outFile = new PrintWriter(new BufferedWriter(new FileWriter(new File(outfName))));
			for(int i=0; i<256; i++) {
				if(charCountsAry[i] > 0) {
					outFile.printf("%c \t # %d\n",(char)i, charCountsAry[i]);
				}
			}
			outFile.close();
		} catch (IOException e) {
			System.out.println("An I/O ERROR Occurred");
			System.exit(0);
		}
	}
}
