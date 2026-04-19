day(mon).
day(tue).
day(wed).
day(thu).

friend(olga).
friend(piri).
friend(rozi).
friend(shari).

program(hairdresser).
program(dressmaker).
program(library).
program(boat).

schedule(visit(D1,F1,P1), visit(D2,F2,P2), visit(D3,F3,P3), visit(D4,F4,P4)) :-
    day(D1), day(D2), day(D3), day(D4),
    D1 \= D2, D1 \= D3, D1 \= D4,
    D2 \= D3, D2 \= D4,
    D3 \= D4,

    friend(F1), friend(F2), friend(F3), friend(F4),
    F1 \= F2, F1 \= F3, F1 \= F4,
    F2 \= F3, F2 \= F4,
    F3 \= F4,

    program(P1), program(P2), program(P3), program(P4),
    P1 \= P2, P1 \= P3, P1 \= P4,
    P2 \= P3, P2 \= P4,
    P3 \= P4,

    (F1 \= shari ; D1 = thu),
    (F2 \= shari ; D2 = thu),
    (F3 \= shari ; D3 = thu),
    (F4 \= shari ; D4 = thu),

    (P1 \= hairdresser ; D1 = thu),
    (P2 \= hairdresser ; D2 = thu),
    (P3 \= hairdresser ; D3 = thu),
    (P4 \= hairdresser ; D4 = thu),

    ( (F1 \= piri, F1 \= rozi) ; (P1 \= library, P1 \= hairdresser) ),
    ( (F2 \= piri, F2 \= rozi) ; (P2 \= library, P2 \= hairdresser) ),
    ( (F3 \= piri, F3 \= rozi) ; (P3 \= library, P3 \= hairdresser) ),
    ( (F4 \= piri, F4 \= rozi) ; (P4 \= library, P4 \= hairdresser) ),

    (D1 \= tue ; P1 \= boat),
    (D2 \= tue ; P2 \= boat),
    (D3 \= tue ; P3 \= boat),
    (D4 \= tue ; P4 \= boat),

    (D1 \= tue ; (F1 \= olga, F1 \= piri, F1 \= rozi)),
    (D2 \= tue ; (F2 \= olga, F2 \= piri, F2 \= rozi)),
    (D3 \= tue ; (F3 \= olga, F3 \= piri, F3 \= rozi)),
    (D4 \= tue ; (F4 \= olga, F4 \= piri, F4 \= rozi)).


run :-
    schedule(Visit1, Visit2, Visit3, Visit4).
run :-
    write('Катя врёт').

:- initialization(run).