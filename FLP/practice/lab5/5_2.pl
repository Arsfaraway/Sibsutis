�����_��������� :-    %���������� ���������
    repeat,           %���������� ��������, ������� ������� ����������� ����
    write('������� ����� ����� ��������� (������������� ����� ��� ������): '),
    read(N),          %��������� ����� �����
    (N < 0 ->
        ! ;           % ����� �� �����, ���� ������� ������������� �����
        func(N, Res), % ����� ���������, ����� ����� �����, ��������� - Result
        write('����� ��������� � ������� '), write(N), write(' ����� '), write(Res), nl,
        fail          % ��������� ����
    ).

func(0, 1). % �������, ���� N ����� 0, �� ��������� ����� 1
func(1, 1). % �������, ���� N ����� 1, �� ��������� ����� 1
func(N, Res) :-  % ������� ��� ������ ������, ����� N ������ 1
    N > 1,       % ������� ���������, ��� N ������ 1
    N1 is N - 1, % �������� ���������� �������� ��� N - 1, � ��������� �����������
                 % � ���������� F1
    N2 is N - 2, % ����������
    func(N1, R1),% ��������� ��������� � R1
    func(N2, R2),
    Res is R1 + R2. % Res=R1+R2
