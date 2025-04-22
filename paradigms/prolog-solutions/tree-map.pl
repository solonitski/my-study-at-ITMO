map_build(ListMap, TreeMap) :-
    length(ListMap, Length),
    map_build(Length, ListMap, TreeMap).

map_build(0, [], null).
map_build(1, [(K, V)], node(K, V, null, null)).
map_build(Length, List, node(K, V, L, R)) :-
    LLength is Length // 2 - 1,
    RLength is Length // 2 + Length mod 2,
    split_list(LLength, List, LeftItems, [(K, V) | RightItems]),
    map_build(LLength, LeftItems, L),
    map_build(RLength, RightItems, R).

split_list(0, List, [], List).
split_list(N, [H | T], [H | L], R) :-
    N1 is N - 1,
    split_list(N1, T, L, R).

map_get(node(K, V, _, _), K, V).
map_get(node(X, _, L, _), K, V) :- K < X, map_get(L, K, V).
map_get(node(X, _, _, R), K, V) :- K > X, map_get(R, K, V).

map_keys(null, []).
map_keys(node(K, _, L, R), Keys) :-
    map_keys(L, LK),
    map_keys(R, RK),
    append(LK, [K | RK], Keys).