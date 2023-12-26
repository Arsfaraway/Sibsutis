% ����������� ��������� ��� �������� ���������� ���� � ������ � ���������� ��������.
count_words_in_line(Line, WordCount) :-
    trim_whitespace(Line, TrimmedLine),
    split_string(TrimmedLine, " ", " ", Words),
    exclude(=(''), Words, NonEmptyWords),
    length(NonEmptyWords, WordCount).

% �������� ��� �������� �������� � ������ � ����� ������.
trim_whitespace(Input, Output) :-
    atom_codes(Input, InputCodes),
    trim_whitespace_codes(InputCodes, TrimmedCodes),
    atom_codes(Output, TrimmedCodes).

trim_whitespace_codes(InputCodes, TrimmedCodes) :-
    drop_while(space, InputCodes, TrimmedStart),
    reverse(TrimmedStart, Reversed),
    drop_while(space, Reversed, TrimmedEnd),
    reverse(TrimmedEnd, TrimmedCodes).

drop_while(_, [], []).
drop_while(Pred, [H|T], Rest) :-
    call(Pred, H),
    drop_while(Pred, T, Rest).
drop_while(_, List, List).

space(Code) :-
    Code =:= 32.

% �������� �������� ��� ��������� �����.
process_file(InputFile, OutputFile) :-
    open(InputFile, read, InputStream),
    open(OutputFile, write, OutputStream),
    process_lines(InputStream, OutputStream, 1),
    close(InputStream),
    close(OutputStream).

% �������� ��� ��������� ����� � ������ ���������� � ����� ����.
process_lines(InputStream, OutputStream, LineNumber) :-
    read_line_to_string(InputStream, Line),
    ( Line = end_of_file ->
        true
    ;
        count_words_in_line(Line, WordCount),
        format(OutputStream, '������ ~d �������� ~d ����.~n', [LineNumber, WordCount]),
        NextLineNumber is LineNumber + 1,
        process_lines(InputStream, OutputStream, NextLineNumber)
    ).

% ������ �������������:
����� :-
    process_file('C:/Users/hasee/Desktop/input.txt', 'C:/Users/hasee/Desktop/output.txt').

