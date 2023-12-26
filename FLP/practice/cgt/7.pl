% �������� ��� �������� ����� ��������� �������� � ������
�������(_, [], 0).
�������(Elem, [Elem|Tail], Count) :-
    �������(Elem, Tail, TailCount),
    Count is TailCount + 1.
�������(Elem, [_|Tail], Count) :-
    �������(Elem, Tail, Count).

% �������� ��� ������������ ������ �� ��� (�������, ���������� ���������)
���������_������([], []).
���������_������([Elem|Tail], [[Elem, Count]|Result]) :-
    �������(Elem, [Elem|Tail], Count),
    delete(Tail, Elem, NewTail), % ������� ��� ��������� �������� ��������
    ���������_������(NewTail, Result).

% ������ �������������:
����� :-
    InputList = [2, 1, 3, 1, 1, 6, 7, 6],
    ���������_������(InputList, ResultList),
    writeln(ResultList).
