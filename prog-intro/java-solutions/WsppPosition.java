import java.io.*;
import java.util.Map;
import java.util.LinkedHashMap;
import java.util.ArrayList;
import java.nio.charset.StandardCharsets;
import java.nio.file.NoSuchFileException;
import java.lang.IllegalArgumentException;
import java.lang.StringBuilder;

public class WsppPosition {
    private static boolean isGoodSymbol(char symbol) {
        return ((Character.isLetter(symbol)) || (symbol == '\'') || (Character.getType(symbol) == Character.DASH_PUNCTUATION));
    }

    public static void main(String[] args) throws IOException {
        if (args.length < 2) {
            if (args.length == 0) {
                System.out.println("Error. Zero arguments entered. Need two arguments.");
                return;
            } else if (args.length == 1) {
                System.out.println("Error. One arguments entered. Need two arguments.");
                return;
            }
            System.out.println("Error!");
            return;
        }

        String inputFileName = args[0];
        String outputFileName = args[1];
        Map<String, ArrayList<String>> wordsAndStats = new LinkedHashMap<String, ArrayList<String>>();
        
        try {
            FastScanner sc = new FastScanner(new File(inputFileName));
            int countLines = 0;
            while (sc.hasNext()) {
                String line = sc.nextLine();
                line = line.concat(" ");
                int beginPos = 0;
                countLines++;
                int countWords = 0;

                for (int i = 0; i < line.length(); i++) {
                    char symbol = line.charAt(i);
                    if (!isGoodSymbol(symbol)) {
                        String word = line.substring(beginPos, i).toLowerCase();
                        if (!word.isEmpty()) {
                            countWords++;
                            ArrayList<String> temp = wordsAndStats.getOrDefault(word, new ArrayList<>());
                            temp.add(countLines + ":" + countWords);
                            wordsAndStats.put(word, temp);
                        }
                        beginPos = i + 1;
                    }
                }
            }
            sc.close();
        } catch(FileNotFoundException e){
            e.getMessage();
            System.out.println("Error. Can't find input file.");
        } catch(IllegalArgumentException e){
            System.out.println("Error. Passed an illegal or inappropriate argument");
        } catch(IOException e){
            System.out.println("Error. Can't read file.");
        }

        try (BufferedWriter out = new BufferedWriter(new FileWriter(outputFileName, StandardCharsets.UTF_8))) {
            for (Map.Entry<String, ArrayList<String>> entry : wordsAndStats.entrySet()) {
                out.write(entry.getKey() + " " + entry.getValue().size());
                for (int i = 0; i < entry.getValue().size(); i++) {
                    out.write(" ");
                    out.write(entry.getValue().get(i));
                }
                out.newLine();
            }
        } catch (IllegalArgumentException e) {
            System.out.println("Error. Passed an illegal or inappropriate argument");
        } catch (NoSuchFileException e) {
            System.out.println("Error. Output file not created.");
        } catch (IOException e) {
            System.out.println("Error. Can't write file.");
        }
    }
}
