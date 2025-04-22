import java.io.*;
import java.util.InputMismatchException;
import java.lang.StringBuilder;
import java.nio.charset.StandardCharsets;

public class FastScanner implements Closeable {
    private final Reader reader;
    private final int BUFFER_SIZE = 1024;
    private char[] buffer = new char[BUFFER_SIZE];
    private int position;
    private int end;
    private boolean needInput = true;

    public FastScanner(String str) throws IOException {
        reader = new InputStreamReader(new ByteArrayInputStream(str.getBytes(StandardCharsets.UTF_8)), StandardCharsets.UTF_8);
        fillBuffer();
    }

    public FastScanner(InputStream in) throws IOException {
        reader = new InputStreamReader(in);
        fillBuffer();
    }

    public FastScanner(File file) throws FileNotFoundException, IOException {
        reader = new InputStreamReader(new FileInputStream(file), StandardCharsets.UTF_8);
        fillBuffer();
    }

    private void fillBuffer() {
        try {
            end = reader.read(buffer);
            while (end == 0) {
                end = reader.read(buffer);
            }
            position = 0;
        } catch (IOException e) {
            end = -1;
        }
        if (end == -1) {
            needInput = false;
        }
    }

    private void skipWhitespaces() throws IOException {
        while (Character.isWhitespace(buffer[position])) {
            position++;
            if (position >= end) {
                fillBuffer();
                if (!needInput) {
                    break;
                }
            }
        }
    }

    public boolean hasNext() throws IOException {
        skipWhitespaces();
        if (position < end) {
            return true;
        } else {
            fillBuffer();
            if (needInput) {
                return true;
            }
            return false;
        }
    }

    public String next() throws IOException {
        StringBuilder sb = new StringBuilder();
        skipWhitespaces();
        while (!Character.isWhitespace(buffer[position]) && needInput) {
            sb.append(buffer[position]);
            position++;
            if (position >= end) {
                fillBuffer();
                if (!needInput) {
                    break;
                }
            }
        }
        return sb.toString();
    }

    public String nextLine() {
        StringBuilder sb = new StringBuilder();
        while (needInput) {
            if (buffer[position] == '\n' || buffer[position] == '\r') {
                break;
            }
            sb.append(buffer[position]);
            position++;
            if (position >= end) {
                fillBuffer();
                if (!needInput) {
                    break;
                }
            }
        }
        return sb.toString();
    }

    public void close() throws IOException{
        reader.close();
    }
}