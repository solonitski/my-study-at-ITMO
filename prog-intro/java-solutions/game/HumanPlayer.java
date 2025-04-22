package game;

import java.io.PrintStream;
import java.util.Scanner;

/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public class HumanPlayer implements Player {
    private final PrintStream out;
    private final Scanner in;
    private final int m;
    private final int n;

    public HumanPlayer(final PrintStream out, final Scanner in, final int m, final int n) {
        this.out = out;
        this.in = in;
        this.m = m;
        this.n = n;
    }

    public HumanPlayer(final int m, final int n) {
        this(System.out, new Scanner(System.in), m, n);
    }

    @Override
    public Move move(final Position position, final Cell cell) {
        while (true) {
            out.println("Position");
            out.println(position);
            out.println(cell + "'s move");
            out.println("Enter row and column");
            int x = 0, y = 0;

            boolean isCorrectInput = false;
            while (!isCorrectInput) {
                try {
                    String input_x = in.next();
                    x = Integer.parseInt(input_x);

                    String input_y = in.next();
                    y = Integer.parseInt(input_y);

                    if (x <= 0 || y <= 0) {
                        throw new Exception();
                    }

                    isCorrectInput = true;
                } catch (Exception e) {
                    System.err.println("Вы ввели некорректные данные. Введите новые корректные данные.");
                }
            }

            final Move move = new Move(x - 1, y - 1, cell);
            return move;
        }
    }
}
