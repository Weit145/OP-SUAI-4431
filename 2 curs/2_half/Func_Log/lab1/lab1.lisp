(print (cadr '(WHERE STUDENT IVANOV ALEXEY IS STUDYING)))
(print (caaddr '( (4110) (GROUP OF) (STUDENT IVANOV (ALEXEY)))))
(print (car (cdddar (cdadar '((DMITRY (ANDREY (MAXIM ALL ARE STUDENTS))))))))

(print (cons (cadr '(A X C)) (cons (cadr '(F Y)) '(Z))))
(print (cons (cadadr '((A) (B X) (C D))) (cons (caar '((Y (Z)))) (cadar '((Y (Z)))))))
(print (cons (cadr '(A X B Y)) (cons (cadddr '(A X B Y)) (cons (cadr '(F Z)) nil))))


;; Вход: (1 2 3 4 5 6), (a b c), N = 3.
;; Выход: (1 2 3 a b c 4 5 6).

(defun f (ls1 ls2 N)
    (cond
        ((eq N 0) (if (null ls2) ls1 (cons (car ls2) (f ls1 (cdr ls2) 0))))
        ((null ls1) ( cons ls2 nil))
        (T (cons (car ls1) (f (cdr ls1) ls2 (- N 1))))
    )
)

(trace f)

(print (f '(1 2 3 4 5 6) '(a b c) 3))