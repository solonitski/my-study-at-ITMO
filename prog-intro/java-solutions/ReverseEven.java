import java.util.ArrayList;
import java.util.Scanner;

public class ReverseEven {
    public static void main(String[] args) {
        Scanner scanLine = new Scanner(System.in);
        ArrayList<String> elements = new ArrayList<>();

        while (scanLine.hasNextLine()) {
		    elements.add(System.lineSeparator());
            String line = scanLine.nextLine();
            Scanner scanNumbers = new Scanner(line);
            while (scanNumbers.hasNext()) {
                int num = scanNumbers.nextInt();
                if (num % 2 == 0) {
                    elements.add(" ");
                    elements.add(Integer.toString(num));
                }
            }
        }

        for (int i = elements.size() - 1; i >= 0; i--) {
            System.out.print(elements.get(i));
        }
    }
}