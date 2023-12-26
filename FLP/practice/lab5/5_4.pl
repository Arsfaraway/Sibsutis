most_frequent_elements(List, Result) :-
    % ������������ ������� ��������� ������� �������� � ������
    count_elements(List, ElementCounts),
    % ������� ������������ �������
    max_frequency(ElementCounts, MaxFrequency),
    % ��������� �������� � ������������ ��������
    find_most_frequent(ElementCounts, MaxFrequency, Result).

% ������� ������� ��������� � ������
count_elements([], []).
count_elements([H|T], Result) :-
    count_elements(T, TmpResult),
    update_element_count(H, TmpResult, Result).

% ���� ������� ��� ���� � ������ ������, ����������� ��� �������
update_element_count(Element, [], [element(Element, 1)]).
update_element_count(Element, [element(Element, Count)|T], [element(Element, NewCount)|T]) :-
    NewCount is Count + 1.
update_element_count(Element, [X|T], [X|NewT]) :-
    Element \= X,
    update_element_count(Element, T, NewT).

% ���������� ������������ ������� � ������ ������
max_frequency([element(_, Count)], Count).% � ������ �������. �������, ��� ������� �����������
max_frequency([element(_, Count)|Rest], MaxCount) :-
    max_frequency(Rest, TmpMax),
    MaxCount is max(Count, TmpMax).

% ���������� ��������� � ������������ ��������
find_most_frequent([], _, []).
find_most_frequent([element(Element, Count)|T], MaxFrequency, [Element|Result]) :-
    Count =:= MaxFrequency,
    find_most_frequent(T, MaxFrequency, Result).
find_most_frequent([_|T], MaxFrequency, Result) :-
    find_most_frequent(T, MaxFrequency, Result).
