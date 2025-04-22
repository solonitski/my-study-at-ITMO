public class Sum {
    public static void main(String[] args) {
        int result = 0;
        for (int i = 0; i < args.length; i++) {
            int beginPos = 0;
            if (!Character.isWhitespace(args[i].charAt(args[i].length() - 1))) {
                args[i] = args[i].concat(" ");
            }
            for (int j = 0; j < args[i].length(); j++) {
                char symbol = args[i].charAt(j);
                if (Character.isWhitespace(symbol)) {
                    if (!args[i].substring(beginPos, j).isEmpty()) {
                        result += Integer.parseInt(args[i].substring(beginPos, j));
                    }
                    beginPos = j + 1;
                }
            }
        }
        System.out.println(result);
    }
}