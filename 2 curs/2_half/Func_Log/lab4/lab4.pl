
my_member(X, [H|_]) :-
    X =:= H.

my_member(X, [H|T]) :-
    X =\= H,
    my_member(X, T).


my_not_member(_, []).

my_not_member(X, [H|T]) :-
    X =\= H,
    my_not_member(X, T).


neighb(_, [], []).

neighb(V, [[A, B]|T], [B|R]) :-
    V =:= A,
    neighb(V, T, R).

neighb(V, [[A, _]|T], R) :-
    V =\= A,
    neighb(V, T, R).


count_paths(Start, End, _, _, 1) :-
    Start =:= End.

count_paths(Start, End, Graph, Path, Count) :-
    Start =\= End,
    neighb(Start, Graph, Neighbours),
    count_paths_from_list(Neighbours, End, Graph, Path, Count).



count_paths_from_list([], _, _, _, 0).

count_paths_from_list([Next|Rest], End, Graph, Path, Count) :-
    my_member(Next, Path),
    count_paths_from_list(Rest, End, Graph, Path, Count).

count_paths_from_list([Next|Rest], End, Graph, Path, Count) :-
    my_not_member(Next, Path),
    count_paths(Next, End, Graph, [Next|Path], C1),
    count_paths_from_list(Rest, End, Graph, Path, C2),
    Count is C1 + C2.


add_if_new(List, V, List) :-
    my_member(V, List).

add_if_new(List, V, [V|List]) :-
    my_not_member(V, List).


all_vertices(Graph, Vertices) :-
    collect_vertices(Graph, [], Vertices).

collect_vertices([], Vertices, Vertices).

collect_vertices([[A, B]|T], Acc, Vertices) :-
    add_if_new(Acc, A, Acc1),
    add_if_new(Acc1, B, Acc2),
    collect_vertices(T, Acc2, Vertices).


my_min(A, B, A) :-
    A < B.

my_min(A, B, B) :-
    A >= B.


f1(Graph, Result) :-
    all_vertices(Graph, Vertices),
    f1_by_vertices(Vertices, Graph, Result).


f1_by_vertices([], _, 0).

f1_by_vertices([_], _, 0).


f1_by_vertices([A, B|T], Graph, Result) :-
    min_for_all_pairs([A, B|T], [A, B|T], Graph, 999999, Result).


min_for_all_pairs([], _, _, CurrentMin, CurrentMin).

min_for_all_pairs([U|Rest], Vertices, Graph, CurrentMin, Result) :-
    min_for_one_vertex(U, Vertices, Graph, CurrentMin, NewMin),
    min_for_all_pairs(Rest, Vertices, Graph, NewMin, Result).


min_for_one_vertex(_, [], _, CurrentMin, CurrentMin).


min_for_one_vertex(U, [V|Rest], Graph, CurrentMin, Result) :-
    U =:= V,
    min_for_one_vertex(U, Rest, Graph, CurrentMin, Result).


min_for_one_vertex(U, [V|Rest], Graph, CurrentMin, Result) :-
    U =\= V,
    count_paths(U, V, Graph, [U], Count),
    my_min(Count, CurrentMin, NewMin),
    min_for_one_vertex(U, Rest, Graph, NewMin, Result).