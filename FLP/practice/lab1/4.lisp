(defun func (lst)
(append (list (car lst)) (list(car (last lst))) (butlast (cddr lst)) (list (cadr lst)))

)