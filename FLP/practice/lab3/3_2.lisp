; для bynikа
(defun diff (set1 set2 &optional (result '())) ;специальный параметр для определения необязательного
  (cond ;аргумента result, который используется для накопления результата.
    ((null set1) (nreverse result)); nreverse изменяет порядок элементов на обратный.
    ((member (car set1) set2); есть ли первый элемент set1 в set2?.
     (diff (cdr set1) set2 result)); если есть, то вызываем diff, передавая set1 без 1-го элемента
																	 ; и set2, оставляем result без изменений.
    (t                             ; иначе.
     (diff (cdr set1) set2 (cons (car set1) result)) ; передаем set1 без 1-го элемента, set2 и 
		)																								 ; result, в который добавляем 1 элемент set1
	)
)