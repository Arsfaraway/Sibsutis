most_frequent_elements(List, Result) :-
    % Подсчитываем частоту появления каждого элемента в списке
    count_elements(List, ElementCounts),
    % Находим максимальную частоту
    max_frequency(ElementCounts, MaxFrequency),
    % Извлекаем элементы с максимальной частотой
    find_most_frequent(ElementCounts, MaxFrequency, Result).

% Подсчет частоты элементов в списке
count_elements([], []).
count_elements([H|T], Result) :-
    count_elements(T, TmpResult),
    update_element_count(H, TmpResult, Result).

% Если элемент уже есть в списке частот, увеличиваем его счетчик
update_element_count(Element, [], [element(Element, 1)]).
update_element_count(Element, [element(Element, Count)|T], [element(Element, NewCount)|T]) :-
    NewCount is Count + 1.
update_element_count(Element, [X|T], [X|NewT]) :-
    Element \= X,
    update_element_count(Element, T, NewT).

% Нахождение максимальной частоты в списке частот
max_frequency([element(_, Count)], Count).% в списке единств. элемент, его частота максимальна
max_frequency([element(_, Count)|Rest], MaxCount) :-
    max_frequency(Rest, TmpMax),
    MaxCount is max(Count, TmpMax).

% Извлечение элементов с максимальной частотой
find_most_frequent([], _, []).
find_most_frequent([element(Element, Count)|T], MaxFrequency, [Element|Result]) :-
    Count =:= MaxFrequency,
    find_most_frequent(T, MaxFrequency, Result).
find_most_frequent([_|T], MaxFrequency, Result) :-
    find_most_frequent(T, MaxFrequency, Result).
