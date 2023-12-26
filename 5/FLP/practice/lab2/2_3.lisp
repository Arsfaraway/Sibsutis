; для Арса
(defun func (n) ; сам расскажу
  (cond
    ((null n) 0)
    ((atom n) 0)
    (t (1+ (max (func (car n)) (- (func (cdr n)) 1))))
  )
)