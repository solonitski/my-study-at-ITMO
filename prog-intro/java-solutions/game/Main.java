package game;

import java.util.Scanner;

import static java.lang.Math.max;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.println("Введите последовательно m, n и k: ");
        int m = 0, n = 0, k = 0;
        boolean isInputCorrect = false;

        while (!isInputCorrect) {
            try {
                String input_m = sc.next();
                m = Integer.parseInt(input_m);

                String input_n = sc.next();
                n = Integer.parseInt(input_n);

                String input_k = sc.next();
                k = Integer.parseInt(input_k);

                if (m <= 0 || n <= 0 || k <= 0 || k > max(m, n)) {
                    throw new Exception();
                }

                isInputCorrect = true;
            } catch (Exception e) {
                System.err.println("Вы ввели некорректные данные. Введите новые корректные данные.");
            }
        }

        int isDuel = -1;
        System.out.println("Хотите ли вы сыграть в дуэль до фиксированного количества побед?");
        System.out.println("Введите 1, если да, иначе введите 0 и начнется партия между Вами и вашим соперником.");
        boolean isDuelCorrect = false;

        while (!isDuelCorrect) {
            try {
                String input_Duel = sc.next();
                isDuel = Integer.parseInt(input_Duel);

                if (!((isDuel == 1 && isDuel != 0) || (isDuel != 1 && isDuel == 0))) {
                    throw new Exception();
                }
                isDuelCorrect = true;
            } catch (Exception e) {
                System.err.println("Вы ввели некорректные данные. Введите новые корректные данные.");
            }
        }

        if (isDuel == 1) {
            System.out.println("Введите до какого числа выигрышей продолжать дуэль.");

            boolean isWinsCorrect = false;
            int wins = 1;
            while (!isWinsCorrect) {
                try {
                    String input_wins = sc.next();
                    wins = Integer.parseInt(input_wins);

                    if (wins <= 0) {
                        throw new Exception();
                    }
                    isWinsCorrect = true;
                } catch (Exception e) {
                    System.err.println("Вы ввели некорректные данные. Введите новые корректные данные.");
                }
            }

            System.out.println("С кем вы хотите играть? Если с человеком - введите 1, если с ботом - 0. При ином вводе вы играть не будете.");
            int what;
            try {
                String input_what = sc.next();
                what = Integer.parseInt(input_what);
                if (!((what == 1 && what != 0) || (what != 1 && what == 0))) {
                    throw new Exception();
                }
            } catch (Exception e) {
                what = 2;
            }

            Player player1;
            Player player2;

            if (what == 1) {
                player1 = new HumanPlayer(m, n);
                player2 = new HumanPlayer(m, n);
            } else if (what == 0) {
                player1 = new HumanPlayer(m, n);
                player2 = new RandomPlayer(m, n);
            } else {
                player1 = new RandomPlayer(m, n);
                player2 = new RandomPlayer(m, n);
            }

            int countGames = 0;
            int winsFirst = 0, winsSecond = 0;
            while (true) {
                final TwoPlayerGame game;
                int result;
                System.out.println("Да начнется новая MNK партия! Начинает игрок номер " + (2 - ((countGames + 1) % 2)) + "!");
                if (countGames % 2 == 0) {
                    game = new TwoPlayerGame(new MNKBoard(m, n, k), player1, player2);
                } else {
                    game = new TwoPlayerGame(new MNKBoard(m, n, k), player2, player1);
                }
                result = game.play(false);
                if (result > 0) {
                    if (countGames % 2 == 0) {
                        if (result == 1) {
                            winsFirst++;
                            System.out.println("В данном поединке выиграл игрок номер 1");
                            System.out.println("До полной победы игроку номер 1 осталось выиграть " + (wins - winsFirst) + " раз.");
                        } else if (result == 2) {
                            winsSecond++;
                            System.out.println("В данном поединке выиграл игрок номер 2");
                            System.out.println("До полной победы игроку номер 2 осталось выиграть " + (wins - winsSecond) + " раз.");
                        }
                    } else {
                        if (result == 2) {
                            winsFirst++;
                            System.out.println("В данном поединке выиграл игрок номер 1");
                            System.out.println("До полной победы игроку номер 1 осталось выиграть " + (wins - winsFirst) + " раз.");
                        } else if (result == 1) {
                            winsSecond++;
                            System.out.println("В данном поединке выиграл игрок номер 2");
                            System.out.println("До полной победы игроку номер 2 осталось выиграть " + (wins - winsSecond) + " раз.");
                        }
                    }
                } else {
                    if (result == 0) {
                        System.out.println("Ничья! Борьба была равна.");
                    } else {
                        System.out.println("Произошла ошибка.");
                    }
                }

                if (winsFirst == wins) {
                    System.out.println("Поздравляем! Игрок номер 1 выиграл дуэль!");
                    break;
                } else if (winsSecond == wins) {
                    System.out.println("Поздравляем! Игрок номер 2 выиграл дуэль!");
                    break;
                }
                countGames++;
            }

        } else {
            int result;
            System.out.println("Обычная одиночная партия между двумя людьми.");
            System.out.println("Да начнется новая MNK партия!");
            final TwoPlayerGame game = new TwoPlayerGame(new MNKBoard(m, n, k), new HumanPlayer(m, n), new HumanPlayer(m, n));
            result = game.play(false);
            System.out.println("Игрок номер " + result + " выиграл!");
        }
    }
}
