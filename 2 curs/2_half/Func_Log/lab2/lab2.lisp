(defun neighb (v graf)
  (cond ((null graf) nil)
    ((eql v (first (first graf)))
      (cons (second (first graf)) (neighb v (cdr graf))))
    (t (neighb v (cdr graf)))))

(defun mb (i lst)
  (cond ((null lst) nil)
        ((eq i (car lst)) t)
        (t (mb i (cdr lst)))))


(defun countpaths (start end graf path)
  (cond ((eq start end) 1)
        (t (let ((neighbours (neighb start graf)))
          (reduce #'+ 
            (mapcar #'(lambda (next)
              (if (mb next path)
                0
                (countpaths next end graf (cons next path))))
            neighbours)
          :initial-value 0)))))


(defun all (graph)
  (labels ((collect (g verts)
    (cond ((null g) verts)
      (t (collect (cdr g)
        (add-if-new (add-if-new verts (first (first g)))
          (second (first g)))))))
        (add-if-new (verts v)
        (cond ((mb v verts) verts)
          (t (cons v verts)))))
  (collect graph nil)))



(defun f1 (graph)
  (let ((vertices (all graph)))
    (cond ((or (null vertices) (null (cdr vertices))) 0)
      (t
        (let ((counts
          (mapcan #'(lambda (u)
            (mapcar #'(lambda (v)
              (if (eq u v)
                999999 
                (countpaths u v graph (list u))))
            vertices))
            vertices)))
  (let ((result (remove 0 counts)))
    (if (null result)
        0
        (reduce #'min result :initial-value 999999))))))))


(let ((g '((1 2) (1 3) (2 3) (3 1) (3 5) (5 1))))
  (print "Степень связности орграфа:")
  (print (f1 g)))

(let ((g '((1 2) (1 3) (2 3) (3 2) (2 1) (3 1))))
  (print "Степень связности орграфа:")
  (print (f1 g)))

(let ((g '((1 2) (2 3))))
  (print "Степень связности орграфа:")
  (print (f1 g)))

(let ((g '((1 2) (1 3) (2 4) (3 4))))
  (print "Степень связности орграфа:")
  (print (f1 g)))

(let ((g '((1 2) (2 3) (3 1))))
  (print "Степень связности орграфа:")
  (print (f1 g)))

(let ((g '((1 2) (1 3) (2 1) (2 3) (3 1) (3 2))))
  (print "Степень связности орграфа:")
  (print (f1 g)))

(let ((g '((1 2) (3 4))))
  (print "Степень связности орграфа:")
  (print (f1 g)))