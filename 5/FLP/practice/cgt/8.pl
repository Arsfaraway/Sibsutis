% Определение предиката для подсчета количества слов в строке с обработкой пробелов.
count_words_in_line(Line, WordCount) :-
    trim_whitespace(Line, TrimmedLine),
    split_string(TrimmedLine, " ", " ", Words),
    exclude(=(''), Words, NonEmptyWords),
    length(NonEmptyWords, WordCount).

% Предикат для удаления пробелов в начале и конце строки.
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

% Основной предикат для обработки файла.
process_file(InputFile, OutputFile) :-
    open(InputFile, read, InputStream),
    open(OutputFile, write, OutputStream),
    process_lines(InputStream, OutputStream, 1),
    close(InputStream),
    close(OutputStream).

% Предикат для обработки строк и записи результата в новый файл.
process_lines(InputStream, OutputStream, LineNumber) :-
    read_line_to_string(InputStream, Line),
    ( Line = end_of_file ->
        true
    ;
        count_words_in_line(Line, WordCount),
        format(OutputStream, 'строка ~d содержит ~d слов.~n', [LineNumber, WordCount]),
        NextLineNumber is LineNumber + 1,
        process_lines(InputStream, OutputStream, NextLineNumber)
    ).

% Пример использования:
вызов :-
    process_file('C:/Users/hasee/Desktop/input.txt', 'C:/Users/hasee/Desktop/output.txt').

