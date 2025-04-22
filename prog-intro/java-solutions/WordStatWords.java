import java.util.Map;
import java.util.TreeMap;
import java.io.BufferedReader;
import java.io.FileReader;
import java.nio.charset.StandardCharsets;
import java.nio.file.NoSuchFileException;
import java.lang.IllegalArgumentException;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.BufferedWriter;
import java.io.FileWriter;

public class WordStatWords {
    static boolean isGoodSymbol(char symbol) {
        return ((Character.isLetter(symbol)) || (symbol == '\'') || (Character.getType(symbol) == Character.DASH_PUNCTUATION));
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Error. Not enough arguments entered.");
            return;
        }

        String inputFileName = args[0];
        String outputFileName = args[1];
        Map<String, Integer> wordsAndStats = new TreeMap<String, Integer>();

        try (BufferedReader reader = new BufferedReader(new FileReader(inputFileName, StandardCharsets.UTF_8))) {
            String line;
            while ((line = reader.readLine()) != null) {
                int beginPos = 0;
                line = line.concat(" ");
                for (int i = 0; i < line.length(); i++) {
                    char symbol = line.charAt(i);
                    if (!isGoodSymbol(symbol)) {
                        String word = line.substring(beginPos, i).toLowerCase();
                        if (!word.isBlank()) {
                            if (!wordsAndStats.containsKey(word)) {
                                wordsAndStats.put(word, 1);
                            } else {
                                Integer wordStat = (Integer) wordsAndStats.get(word);
                                wordsAndStats.put(word, wordStat + 1);
                            }
                        }
                        beginPos = i + 1;
                    }
                }
            }
        } catch (FileNotFoundException e) {
            System.out.println("Error. Can't find input file.");
        } catch (IllegalArgumentException e) {
            System.out.println("Error. Passed an illegal or inappropriate argument");
        } catch (IOException e) {
            System.out.println("Error. Can't read file.");
        }

        try (BufferedWriter out = new BufferedWriter(new FileWriter(outputFileName, StandardCharsets.UTF_8))) {
            for (Map.Entry<String, Integer> entry : wordsAndStats.entrySet()) {
                out.write(entry.getKey() + " " + entry.getValue() + "\n");
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
