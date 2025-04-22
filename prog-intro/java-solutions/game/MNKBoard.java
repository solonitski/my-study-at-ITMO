package game;

import java.util.Arrays;
import java.util.Map;

public class MNKBoard implements Board, Position {
    private static final Map<Cell, Character> SYMBOLS = Map.of(
            Cell.X, 'X',
            Cell.O, 'O',
            Cell.E, '.'
    );

    private final Cell[][] cells;
    private Cell turn;

    private final int m;
    private final int n;
    private final int k;

    public MNKBoard(int m, int n, int k) {
        this.m = m;
        this.n = n;
        this.k = k;
        this.cells = new Cell[n][m];
        for (Cell[] row : cells) {
            Arrays.fill(row, Cell.E);
        }
        turn = Cell.X;
    }

    @Override
    public Position getPosition() {
        return this;
    }

    @Override
    public Cell getCell() {
        return turn;
    }

    @Override
    public Result makeMove(final Move move) {
        if (!isValid(move)) {
            return Result.LOSE;
        }
        cells[move.getRow()][move.getColumn()] = move.getValue();

        int empty = 0;
        for (int u = 0; u < n; u++) {
            int inRow = 0;
            int inColumn = 0;
            for (int v = 0; v < m; v++) {
                if (cells[u][v] == turn) {
                    inRow++;
                }
                if (cells[v][u] == turn) {
                    inColumn++;
                }
                if (cells[u][v] == Cell.E) {
                    empty++;
                }
            }
            if (inRow == k || inColumn == k) {
                return Result.WIN;
            }
        }

        for (int r = 0; r <= m - k; r++) {
            for (int c = 0; c <= n - k; c++) {
                int inDiagonal = 0;
                for (int i = 0; i < k; i++) {
                    if (cells[r + i][c + i] == turn) {
                        inDiagonal++;
                    }
                }
                if (inDiagonal == k) {
                    return Result.WIN;
                }
            }

            for (int c = k - 1; c < n; c++) {
                int inDiagonal = 0;
                for (int i = 0; i < k; i++) {
                    if (cells[r + i][c - i] == turn) {
                        inDiagonal++;
                    }
                }
                if (inDiagonal == k) {
                    return Result.WIN;
                }
            }
        }

        if (empty == 0) {
            return Result.DRAW;
        }

        turn = turn == Cell.X ? Cell.O : Cell.X;
        return Result.UNKNOWN;
    }

    @Override
    public boolean isValid(final Move move) {
        return 0 <= move.getRow() && move.getRow() < m
                && 0 <= move.getColumn() && move.getColumn() < n
                && cells[move.getRow()][move.getColumn()] == Cell.E
                && turn == getCell();
    }

    @Override
    public Cell getCell(final int r, final int c) {
        return cells[r][c];
    }

    private String make_dash_string(int number) {
        StringBuilder sb = new StringBuilder();
        sb.append("      --");
        for (int i = 1; i < m; i++) {
            sb.append("-----");
        }
        sb.append("---");
        return sb.toString();
    }

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder().append("        ");
        for (int i = 0; i < m; i++) {
            if (i < 9) {
                sb.append(i + 1).append("    ");
            } else if (i < 99) {
                sb.append(i + 1).append("   ");
            } else if (i < 999) {
                sb.append(i + 1).append("  ");
            } else if (i < 9999) {
                sb.append(i + 1).append(" ");
            }
        }

        sb.append(System.lineSeparator()).append(make_dash_string(n)).append(System.lineSeparator());

        for (int r = 0; r < n; r++) {
            if (r < 9) {
                sb.append("   ").append(r + 1).append(" |  ");
            } else if (r < 99){
                sb.append("  ").append(r + 1).append(" |  ");
            } else if (r < 999){
                sb.append(" ").append(r + 1).append(" |  ");
            } else if (r < 9999){
                sb.append(r + 1).append(" |  ");
            }

            for (int i = 0; i < m; i++) {
                if (i != (m - 1)) {
                    sb.append(SYMBOLS.get(cells[r][i])).append("    ");
                } else {
                    sb.append(SYMBOLS.get(cells[r][i])).append("  ");
                }
            }
            sb.append("|").append(System.lineSeparator());
        }
        sb.append(make_dash_string(n)).append(System.lineSeparator());

        return sb.toString();
    }
}
