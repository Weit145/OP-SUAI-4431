% Days
day(mon).
day(tue).
day(wed).
day(thu).

% Friends
friend(olga).
friend(piri).
friend(rozi).
friend(shari).

% Programs
program(hairdresser).
program(dressmaker).
program(library).
program(boat).

schedule_exists :-
    day(D1), day(D2), day(D3), day(D4),
    all_different([D1, D2, D3, D4]),
    Schedule = [visit(D1, F1, P1),
                visit(D2, F2, P2),
                visit(D3, F3, P3),
                visit(D4, F4, P4)],

    all_different([F1, F2, F3, F4]),
    all_different([P1, P2, P3, P4]),

    % Shari visits only on Thursday
    member(visit(thu, shari, _), Schedule),
    \+ ( member(visit(D, shari, _), Schedule), D \= thu ),

    % The hairdresser appointment is only on Thursday
    member(visit(thu, _, hairdresser), Schedule),
    \+ ( member(visit(D, _, hairdresser), Schedule), D \= thu ),

    % Neither Piri nor Rozi goes to the library
    forall( (member(visit(_, F, _), Schedule), (F = piri ; F = rozi)),
            P \= library ),

    % Neither Piri nor Rozi goes to the hairdresser
    forall( (member(visit(_, F, P), Schedule), (F = piri ; F = rozi)),
            P \= hairdresser ),

    % No boat visit on Tuesday
    \+ member(visit(tue, _, boat), Schedule),

    % No visits by Olga, Piri, or Rozi on Tuesday
    \+ member(visit(tue, olga, _), Schedule),
    \+ member(visit(tue, piri, _), Schedule),
    \+ member(visit(tue, rozi, _), Schedule),

    write('Schedule found:'), nl,
    maplist(show_visit, Schedule).

all_different([]).
all_different([H|T]) :- \+ member(H, T), all_different(T).

show_visit(visit(D, F, P)) :-
    write('  '), write(D), write(': '), write(F), write(' — '), write(P), nl.

check :-
    (   schedule_exists
    ->  write('Катя сказала правду (расписание найдено).'), nl
    ;   write('Катя лжёт! Расписание невозможно.'), nl
    ).