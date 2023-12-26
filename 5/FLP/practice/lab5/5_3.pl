% Предикат для разбиения списка на три подсписка
split_list(List, Low, High, Less, Between, More) :-
    % Вызываем предикаты для разделения списка
    split_list(List, Low, High, [], [], [], Less, Between, More).

% Базовый случай: если список пустой, то возвращаем подсписки
split_list([], _, _, Less, Between, More, Less, Between, More).

% Разбиваем список на три подсписка в зависимости от значения границ
split_list([X | Rest], Low, High, LTL, BLAH, MTH, Less, Between, More) :-
    X < Low, % Если элемент меньше Low
    append(LTL, [X], NewLTL), % Добавляем его к списку "меньше Low"
    split_list(Rest, Low, High, NewLTL, BLAH, MTH, Less, Between, More).

split_list([X | Rest], Low, High, LTL, BLAH, MTH, Less, Between, More) :-
    X >= Low, X =< High, % Если элемент в пределах от Low до High включительно
    append(BLAH, [X], NewBLAH), % Добавляем его к списку "между Low и High"
    split_list(Rest, Low, High, LTL, NewBLAH, MTH, Less, Between, More).

split_list([X | Rest], Low, High, LTL, BLAH, MTH, Less, Between, More) :-
    X > High, % Если элемент больше High
    append(MTH, [X], NewMTH), % Добавляем его к списку "больше High"
    split_list(Rest, Low, High, LTL, BLAH, NewMTH, Less, Between, More).
