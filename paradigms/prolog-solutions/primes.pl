prime(X) :- \+ check_div(X, 2, X).

check_div(X, I, N) :- I * I =< N,
                     0 is mod(X, I).

check_div(X, I, N) :- I * I =< N,
                      IPP is I + 1,
                      check_div(X, IPP, N).

composite(X) :- \+ prime(X).

prime_divisors(X, []) :- X =< 1.
prime_divisors(X, [H | T]) :- X >= 2,
                              min_prime_div(X, 2, H),
                              N is div(X, H),
                              prime_divisors(N, T).

min_prime_div(X, I, R) :- I * I > X,
                    R is X, !.

min_prime_div(X, I, R) :- I * I =< X,
                    \+ prime(I),
                    N is I + 1,
                    min_prime_div(X, N, R),
                    !.

min_prime_div(X, I, R) :- I * I =< X,
                    0 is mod(X, I),
                    R is I, !.

min_prime_div(X, I, R) :- I * I =< X,
                    N is I + 1,
                    min_prime_div(X, N, R).

sorted([], []).
sorted([X|Xs], List) :-
    partition(Xs, X, L, R),
    sorted(L, SL),
    sorted(R, SR),
    append(SL, [X|SR], List).

partition([], _, [], []).
partition([Y|Ys], P, [Y|L], R) :-
    Y =< P,
    partition(Ys, P, L, R).

partition([Y|Ys], P, L, [Y|R]) :-
    Y > P,
    partition(Ys, P, L, R).

square_divisors(N, L) :- prime_divisors(N, L1),
                         append(L1, L1, L3),
                         sorted(L3, L).