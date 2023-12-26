; для Алексея
(defun func (n lst) 
	(cond 
		((null lst) (list n))
		((>= (car lst) n) (cons n lst)) ; присоединяем число к остатку списка.
		(t (cons (car lst) (func n (cdr lst)))) ; присоединяем текущий элемент списка к
		; результату рекурсии.
 )
)
