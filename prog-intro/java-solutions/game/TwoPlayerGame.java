package game;

public class TwoPlayerGame {
    private final Board board;
    private final Player player1;
    private final Player player2;

    public TwoPlayerGame(Board board, Player player1, Player player2) {
        this.board = board;
        this.player1 = player1;
        this.player2 = player2;
    }

    public int play(boolean log) {
        while (true) {
            final int result1 = makeMove(player1, 1, log);
            if (result1 != -1)  {
                return result1;
            }
            final int result2 = makeMove(player2, 2, log);
            if (result2 != -1)  {
                return result2;
            }
        }
    }

    private int makeMove(Player player, int no, boolean log) {
        final Move move = player.move(board.getPosition(), board.getCell());
        final Result result = board.makeMove(move);
        if (log) {
            System.out.println();
            System.out.println("Player: " + no);
            System.out.println(move);
            System.out.println(board);
            System.out.println("Result: " + result);
        }
        if (result == Result.WIN) {
            if (log) {
                System.out.println("Player " + no + " won");
            }
            return no;
        } else if (result == Result.LOSE) {
            if (log) {
                System.out.println("Player " + no + " lose");
            }
            return 3 - no;
        } else if (result == Result.DRAW) {
            if (log) {
                System.out.println("Draw");
            }
            return 0;
        } else {
            return -1;
        }
    }
}
