(defun neighbour2 (v graph)
  (cond ((null graph) nil)
        ((eq v (caar graph))
         (cons (cdar graph) (neighbour2 v (cdr graph))))
        (t (neighbour2 v (cdr graph)))))

(defun member-eq (item lst)
  (cond ((null lst) nil)
        ((eq item (car lst)) t)
        (t (member-eq item (cdr lst)))))


(defun count-simple-paths (current end graph path)
  (cond ((eq current end) 1)
        (t (let ((neighbours (neighbour2 current graph)))
             (reduce #'+ 
                     (mapcar #'(lambda (next)
                                 (if (member-eq next path)
                                     0
                                     (count-simple-paths next end graph (cons next path))))
                             neighbours)
                     :initial-value 0)))))


(defun get-all-vertices (graph)
  (labels ((collect (g verts)
             (cond ((null g) verts)
                   (t (collect (cdr g)
                               (add-if-new (add-if-new verts (caar g))
                                           (cdar g))))))
           (add-if-new (verts v)
             (cond ((member-eq v verts) verts)
                   (t (cons v verts)))))
    (collect graph nil)))



(defun connectivity-degree (graph)
  (let ((vertices (get-all-vertices graph)))
    (cond ((or (null vertices) (null (cdr vertices))) 0)
          (t
           (let ((all-path-counts
                   (mapcan #'(lambda (u)
                               (mapcar #'(lambda (v)
                                           (if (eq u v)
                                               999999          ; пропускаем u == v
                                               (count-simple-paths u v graph (list u))))
                                       vertices))
                           vertices)))
             (reduce #'min all-path-counts :initial-value 999999))))))


(let ((g '((1 . 2) (1 . 3) (2 . 3) (3 . 1) (3 . 5) (5 . 1))))
  (print "Степень связности орграфа:")
  (print (connectivity-degree g)))

(let ((g '((1 . 2) (1 . 3) (2 . 3) (3 . 2) (2 . 1) (3 . 1))))
  (print "Степень связности орграфа:")
  (print (connectivity-degree g)))