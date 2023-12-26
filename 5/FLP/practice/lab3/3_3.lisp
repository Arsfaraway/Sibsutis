; для Арса
(defun func (predicate lst);аргументы - предикат и список.
  (cond
    ((null lst) nil)
    ((funcall predicate (car lst));если предикат выполняется для 1 элемента
     (cons '* (cons (car lst) (func predicate (cdr lst))));то выполняем это.
    )
    (t ;иначе если предикат не выполняется для 1 элемента.
     (cons (car lst) (func predicate (cdr lst)));просто добавляем элемент к результату
    )
  )
)

; Определение списка и предикатов (возвр.true/false)
(setq numbers '(1 2 3 4 5 -6 7 8 9 -10)
;функция setq для определения numbers.

 odd (lambda (x) (not (evenp x)));определяем odd, к-й присваиваем функцию
;с использ. lambda, которое проверяет not (четность) числа (возвр. t/nil).
 neg (lambda (x) (>= x 0));

; Вызов функции с предикатом для нечетных чисел
 result-odd (func odd numbers)

; Вызов функции с предикатом для неотрицательных чисел
 result-neg (func neg numbers))
 (format t "Результат для нечетных: ~a" result-odd)
 (format t "Результат для неотрицательных: ~a" result-neg)
; format- создавать строки
; t - это аргумент функции format, который указывает на стандартный вывод
; ~a -  вставлено в эту часть строки