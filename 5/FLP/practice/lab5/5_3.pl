% �������� ��� ��������� ������ �� ��� ���������
split_list(List, Low, High, Less, Between, More) :-
    % �������� ��������� ��� ���������� ������
    split_list(List, Low, High, [], [], [], Less, Between, More).

% ������� ������: ���� ������ ������, �� ���������� ���������
split_list([], _, _, Less, Between, More, Less, Between, More).

% ��������� ������ �� ��� ��������� � ����������� �� �������� ������
split_list([X | Rest], Low, High, LTL, BLAH, MTH, Less, Between, More) :-
    X < Low, % ���� ������� ������ Low
    append(LTL, [X], NewLTL), % ��������� ��� � ������ "������ Low"
    split_list(Rest, Low, High, NewLTL, BLAH, MTH, Less, Between, More).

split_list([X | Rest], Low, High, LTL, BLAH, MTH, Less, Between, More) :-
    X >= Low, X =< High, % ���� ������� � �������� �� Low �� High ������������
    append(BLAH, [X], NewBLAH), % ��������� ��� � ������ "����� Low � High"
    split_list(Rest, Low, High, LTL, NewBLAH, MTH, Less, Between, More).

split_list([X | Rest], Low, High, LTL, BLAH, MTH, Less, Between, More) :-
    X > High, % ���� ������� ������ High
    append(MTH, [X], NewMTH), % ��������� ��� � ������ "������ High"
    split_list(Rest, Low, High, LTL, BLAH, NewMTH, Less, Between, More).
