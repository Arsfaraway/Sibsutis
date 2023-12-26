% Предикат для подсчета числа вхождений элемента в списке
подсчет(_, [], 0).
подсчет(Elem, [Elem|Tail], Count) :-
    подсчет(Elem, Tail, TailCount),
    Count is TailCount + 1.
подсчет(Elem, [_|Tail], Count) :-
    подсчет(Elem, Tail, Count).

% Предикат для формирования списка из пар (элемент, количество вхождений)
формируем_список([], []).
формируем_список([Elem|Tail], [[Elem, Count]|Result]) :-
    подсчет(Elem, [Elem|Tail], Count),
    delete(Tail, Elem, NewTail), % Убираем все вхождения текущего элемента
    формируем_список(NewTail, Result).

% Пример использования:
вызов :-
    InputList = [2, 1, 3, 1, 1, 6, 7, 6],
    формируем_список(InputList, ResultList),
    writeln(ResultList).
