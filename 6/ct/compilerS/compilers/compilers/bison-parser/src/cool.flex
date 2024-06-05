/*
 * Определение сканера для COOL.
 */

%{
#include <string>
#include "stringtab.h"
#include "utilities.h"
#include "cool-parse.h"

/* Максимальный размер строковых констант */
#define MAX_STR_CONST 1025
##define YY_NO_INPUT /* чтобы g++ был доволен */

extern FILE *token_file; /* we read from this file */

/* определяем YY_INPUT так, чтобы мы считывали данные из ФАЙЛА fin:
 * Это изменение позволяет использовать этот сканер в
 * классном компиляторе.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
    if ( (result = fread( (char*)buf, sizeof(char), max_size, token_file)) < 0) \
        YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int verbose_flag;
extern int curr_lineno;
extern char* curr_filename;

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

static int comment_layer = 0;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

%}

%option yylineno
%option noyywrap

DARROW          =>
DIGIT           [0-9]
%Start          COMMENTS
%Start          INLINE_COMMENTS
%Start          STRING

%%

/* Вложенные комментарии */
<INITIAL,COMMENTS,INLINE_COMMENTS>"(*" {
    comment_layer++;
    BEGIN COMMENTS;
}

<COMMENTS>[^\n(*]* { }

<COMMENTS>[()*] { }

<COMMENTS>"*)" {
    comment_layer--;
    if (comment_layer == 0) {
        BEGIN 0;
    }
}

<COMMENTS><<EOF>> {
    yylval.error_msg = "EOF in comment";
    BEGIN 0;
    return ERROR;
}

"*)" {
    yylval.error_msg = "Unmatched *)";
    return ERROR;
}


/* /* Однострочные комментарии */

/* "--", начать встроенный комментарий */
<INITIAL>"--" { BEGIN INLINE_COMMENTS; }

/* любой символ, отличный от '\n', является недопустимым во встроенных комментариях */
<INLINE_COMMENTS>[^\n]* { }

/* если во встроенном комментарии указано "\n", комментарий завершается */
<INLINE_COMMENTS>\n {
    curr_lineno++;
    BEGIN 0;
}

/* Строковые константы (синтаксис C)
   * Escape-последовательность \c используется для всех символов c. За исключением 
   * \n \t \b \f, результатом будет c.
   */

/* если вы видите "\", начните строку */
<INITIAL>(\") {
    BEGIN STRING;
    yymore();
}

/* Не удается прочитать '\\' '\"' '\n' */
<STRING>[^\\\"\n]* { yymore(); }

/* обычные экранирующие символы, а не \n */
<STRING>\\[^\n] { yymore(); }

/* если в конце строки стоит символ "\\", строка продолжается */
<STRING>\\\n {
    curr_lineno++;
    yymore();
}

/* встречается значение EOF в середине строки, ошибка */
<STRING><<EOF>> {
    yylval.error_msg = "EOF in string constant";
    BEGIN 0;
    yyrestart(yyin);
    return ERROR;
}

/* встречаем '\n' в середине строки без '\\', ошибка */
<STRING>\n {
    yylval.error_msg = "Unterminated string constant";
    BEGIN 0;
    curr_lineno++;
    return ERROR;
}

/* встречаете "\\0" ??? */
<STRING>"\\0" {
    yylval.error_msg = "Unterminated string constant";
    BEGIN 0;
    //curr_lineno++;
    return ERROR;
}

/* строка заканчивается, нам нужно разобраться с некоторыми экранирующими символами */
<STRING>\" {
    std::string input(yytext, yyleng);

    // remove the '\"'s on both sizes.
    input = input.substr(1, input.length() - 2);

    std::string output = "";
    std::string::size_type pos;

    if (input.find_first_of('\0') != std::string::npos) {
        yylval.error_msg = "String contains 0 character";
        BEGIN 0;
        return ERROR;
    }

    while ((pos = input.find_first_of("\\")) != std::string::npos) {
        output += input.substr(0, pos);
        switch (input[pos + 1]) {
        case 'b':
            output += "\b";
            break;
        case 't':
            output += "\t";
            break;
        case 'n':
            output += "\n";
            break;
        case 'f':
            output += "\f";
            break;
        default:
            output += input[pos + 1];
            break;
        }
        input = input.substr(pos + 2, input.length() - 2);
    }
    output += input;
    if (output.length() > 1024) {
      yylval.error_msg = "String constant too long";
      BEGIN 0;
      return ERROR;
    }
    stringtable.add_string(output.data());
    BEGIN 0;
    return STR_CONST;
}

/* ========
  * ключевые слова
  * ========
  */

 /* CLASS */
(?i:class) { return CLASS; }

 /* ELSE */
(?i:else) { return ELSE; }

 /* FI */
(?i:fi) { return FI; }

 /* IF */
(?i:if) { return IF; }

 /* IN*/
(?i:in) { return IN; }

 /* INHERITS */
(?i:inherits) { return INHERITS; }

 /* LET */
(?i:let) { return LET; }

 /* LOOP */
(?i:loop) { return LOOP; }

 /* POOL */
(?i:pool) { return POOL; }

 /* THEN */
(?i:then) { return THEN; }

 /* WHILE */
(?i:while) { return WHILE; }

 /* CASE */
(?i:case) { return CASE; }

 /* ESAC */
(?i:esac) { return ESAC; }

 /* OF */
(?i:of) { return OF; }

 /* NEW */
(?i:new) { return NEW; }

 /* ISVOID */
(?i:isvoid) { return ISVOID; }

 /* NOT */
(?i:not) { return NOT; }


 /* INT_CONST */
{DIGIT}+ {
    cool_yylval.symbol = inttable.add_string(yytext);
    return INT_CONST;
}

 /* BOOL_CONST */
t(?i:rue) {
    cool_yylval.boolean = 1;
    return BOOL_CONST;
}

f(?i:alse) {
    cool_yylval.boolean = 0;
    return BOOL_CONST;
}

 /* White Space */
[ \f\r\t\v]+ { }

 /* TYPEID */
[A-Z][A-Za-z0-9_]* {
    cool_yylval.symbol = idtable.add_string(yytext);
    return TYPEID;
}

 /* To treat lines. */
"\n" {
    curr_lineno++;
}

 /* OBJECTID */
[a-z][A-Za-z0-9_]* {
    cool_yylval.symbol = idtable.add_string(yytext);
    return OBJECTID;
}

 /* =========
  * operators
  * =========
  */

 /* ASSIGN */
"<-" { return ASSIGN; }

 /* LE */
"<=" { return LE; }

 /* DARROW */
"=>" { return DARROW; }

"+" { return int('+'); }

"-" { return int('-'); }

"*" { return int('*'); }

"/" { return int('/'); }

"<" { return int('<'); }

"=" { return int('='); }

"." { return int('.'); }

";" { return int(';'); }

"~" { return int('~'); }

"{" { return int('{'); }

"}" { return int('}'); }

"(" { return int('('); }

")" { return int(')'); }

":" { return int(':'); }

"@" { return int('@'); }

"," { return int(','); }

 /* =====
  * error
  * =====
  */

[^\n] {
    yylval.error_msg = yytext;
    return ERROR;
}

%%
