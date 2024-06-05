#include "tree.h"
#include "utilities.h"
#include "cool-tree.handcode.h"
#include "cool-tree.h"

int node_lineno = 1;

Program program_class::copy_Program() {
    return new program_class(classes->copy_list());
}

void program_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "program\n";
    classes->dump(stream, n + 2);
}

Class_ class__class::copy_Class_() {
    return new class__class(copy_Symbol(name), copy_Symbol(parent), features->copy_list(), copy_Symbol(filename));
}

void class__class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "class_\n";
    dump_Symbol(stream, n + 2, name);
    dump_Symbol(stream, n + 2, parent);
    features->dump(stream, n + 2);
    dump_Symbol(stream, n + 2, filename);
}

Feature method_class::copy_Feature() {
    return new method_class(copy_Symbol(name), formals->copy_list(), copy_Symbol(return_type), expr->copy_Expression());
}

void method_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "method\n";
    dump_Symbol(stream, n + 2, name);
    formals->dump(stream, n + 2);
    dump_Symbol(stream, n + 2, return_type);
    expr->dump(stream, n + 2);
}

Feature attr_class::copy_Feature() {
    return new attr_class(copy_Symbol(name), copy_Symbol(type_decl), init->copy_Expression());
}

void attr_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "attr\n";
    dump_Symbol(stream, n + 2, name);
    dump_Symbol(stream, n + 2, type_decl);
    init->dump(stream, n + 2);
}

Formal formal_class::copy_Formal() {
    return new formal_class(copy_Symbol(name), copy_Symbol(type_decl));
}

void formal_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "formal\n";
    dump_Symbol(stream, n + 2, name);
    dump_Symbol(stream, n + 2, type_decl);
}

Case branch_class::copy_Case() {
    return new branch_class(copy_Symbol(name), copy_Symbol(type_decl), expr->copy_Expression());
}

void branch_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "branch\n";
    dump_Symbol(stream, n+2, name);
    dump_Symbol(stream, n+2, type_decl);
    expr->dump(stream, n+2);
}

Expression assign_class::copy_Expression() {
    return new assign_class(copy_Symbol(name), expr->copy_Expression());
}

void assign_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "assign\n";
    dump_Symbol(stream, n+2, name);
    expr->dump(stream, n+2);
}

Expression static_dispatch_class::copy_Expression() {
    return new static_dispatch_class(expr->copy_Expression(), copy_Symbol(type_name), copy_Symbol(name), actual->copy_list());
}

void static_dispatch_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "static_dispatch\n";
    expr->dump(stream, n+2);
    dump_Symbol(stream, n+2, type_name);
    dump_Symbol(stream, n+2, name);
    actual->dump(stream, n+2);
}

Expression dispatch_class::copy_Expression() {
    return new dispatch_class(expr->copy_Expression(), copy_Symbol(name), actual->copy_list());
}

void dispatch_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "dispatch\n";
    expr->dump(stream, n+2);
    dump_Symbol(stream, n+2, name);
    actual->dump(stream, n+2);
}

Expression cond_class::copy_Expression() {
    return new cond_class(pred->copy_Expression(), then_exp->copy_Expression(), else_exp->copy_Expression());
}

void cond_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "cond\n";
    pred->dump(stream, n+2);
    then_exp->dump(stream, n+2);
    else_exp->dump(stream, n+2);
}

Expression loop_class::copy_Expression() {
    return new loop_class(pred->copy_Expression(), body->copy_Expression());
}

void loop_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "loop\n";
    pred->dump(stream, n+2);
    body->dump(stream, n+2);
}

Expression typcase_class::copy_Expression() {
    return new typcase_class(expr->copy_Expression(), cases->copy_list());
}

void typcase_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "typcase\n";
    expr->dump(stream, n+2);
    cases->dump(stream, n+2);
}

Expression block_class::copy_Expression() {
    return new block_class(body->copy_list());
}

void block_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "block\n";
    body->dump(stream, n+2);
}

Expression let_class::copy_Expression() {
    return new let_class(copy_Symbol(identifier), copy_Symbol(type_decl), init->copy_Expression(), body->copy_Expression());
}

void let_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "let\n";
    dump_Symbol(stream, n+2, identifier);
    dump_Symbol(stream, n+2, type_decl);
    init->dump(stream, n+2);
    body->dump(stream, n+2);
}

Expression plus_class::copy_Expression() {
    return new plus_class(e1->copy_Expression(), e2->copy_Expression());
}

void plus_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "plus\n";
    e1->dump(stream, n+2);
    e2->dump(stream, n+2);
}

Expression sub_class::copy_Expression() {
   return new sub_class(e1->copy_Expression(), e2->copy_Expression());
}

void sub_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "sub\n";
    e1->dump(stream, n+2);
    e2->dump(stream, n+2);
}

Expression mul_class::copy_Expression() {
    return new mul_class(e1->copy_Expression(), e2->copy_Expression());
}

void mul_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "mul\n";
    e1->dump(stream, n+2);
    e2->dump(stream, n+2);
}

Expression divide_class::copy_Expression() {
    return new divide_class(e1->copy_Expression(), e2->copy_Expression());
}

void divide_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "divide\n";
    e1->dump(stream, n+2);
    e2->dump(stream, n+2);
}

Expression neg_class::copy_Expression() {
    return new neg_class(e1->copy_Expression());
}

void neg_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "neg\n";
    e1->dump(stream, n+2);
}

Expression lt_class::copy_Expression() {
    return new lt_class(e1->copy_Expression(), e2->copy_Expression());
}

void lt_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "lt\n";
    e1->dump(stream, n+2);
    e2->dump(stream, n+2);
}

Expression eq_class::copy_Expression() {
    return new eq_class(e1->copy_Expression(), e2->copy_Expression());
}

void eq_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "eq\n";
    e1->dump(stream, n+2);
    e2->dump(stream, n+2);
}

Expression leq_class::copy_Expression() {
   return new leq_class(e1->copy_Expression(), e2->copy_Expression());
}

void leq_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "leq\n";
    e1->dump(stream, n+2);
    e2->dump(stream, n+2);
}

Expression comp_class::copy_Expression() {
    return new comp_class(e1->copy_Expression());
}

void comp_class::dump(std::ostream &  stream, int n) {
    stream << pad(n) << "comp\n";
    e1->dump(stream, n+2);
}

Expression int_const_class::copy_Expression() {
    return new int_const_class(copy_Symbol(token));
}


void int_const_class::dump(std::ostream &  stream, int n)
{
    stream << pad(n) << "int_const\n";
    dump_Symbol(stream, n+2, token);
}

Expression bool_const_class::copy_Expression()
{
    return new bool_const_class(copy_Boolean(val));
}

void bool_const_class::dump(std::ostream &  stream, int n)
{
    stream << pad(n) << "bool_const\n";
    dump_Boolean(stream, n+2, val);
}

Expression string_const_class::copy_Expression()
{
    return new string_const_class(copy_Symbol(token));
}

void string_const_class::dump(std::ostream &  stream, int n)
{
    stream << pad(n) << "string_const\n";
    dump_Symbol(stream, n+2, token);
}

Expression new__class::copy_Expression()
{
    return new new__class(copy_Symbol(type_name));
}

void new__class::dump(std::ostream &  stream, int n)
{
   stream << pad(n) << "new_\n";
   dump_Symbol(stream, n+2, type_name);
}


Expression isvoid_class::copy_Expression()
{
   return new isvoid_class(e1->copy_Expression());
}


void isvoid_class::dump(std::ostream &  stream, int n)
{
   stream << pad(n) << "isvoid\n";
   e1->dump(stream, n+2);
}

Expression no_expr_class::copy_Expression()
{
   return new no_expr_class();
}

void no_expr_class::dump(std::ostream &  stream, int n)
{
   stream << pad(n) << "no_expr\n";
}

Expression object_class::copy_Expression()
{
   return new object_class(copy_Symbol(name));
}

void object_class::dump(std::ostream &  stream, int n)
{
   stream << pad(n) << "object\n";
   dump_Symbol(stream, n+2, name);
}

// Interfaces used by Bison
Classes nil_Classes()
{
   return new nil_node<Class_>();
}

Classes single_Classes(Class_ e)
{
   return new single_list_node<Class_>(e);
}

Classes append_Classes(Classes p1, Classes p2)
{
   return new append_node<Class_>(p1, p2);
}

Features nil_Features()
{
   return new nil_node<Feature>();
}

Features single_Features(Feature e)
{
   return new single_list_node<Feature>(e);
}

Features append_Features(Features p1, Features p2)
{
   return new append_node<Feature>(p1, p2);
}

Formals nil_Formals()
{
   return new nil_node<Formal>();
}

Formals single_Formals(Formal e)
{
   return new single_list_node<Formal>(e);
}

Formals append_Formals(Formals p1, Formals p2)
{
   return new append_node<Formal>(p1, p2);
}

Expressions nil_Expressions()
{
   return new nil_node<Expression>();
}

Expressions single_Expressions(Expression e)
{
   return new single_list_node<Expression>(e);
}

Expressions append_Expressions(Expressions p1, Expressions p2)
{
   return new append_node<Expression>(p1, p2);
}

Cases nil_Cases()
{
   return new nil_node<Case>();
}

Cases single_Cases(Case e)
{
   return new single_list_node<Case>(e);
}

Cases append_Cases(Cases p1, Cases p2)
{
   return new append_node<Case>(p1, p2);
}

Program program(Classes classes)
{
  return new program_class(classes);
}

Class_ class_(Symbol name, Symbol parent, Features features, Symbol filename)
{
  return new class__class(name, parent, features, filename);
}

Feature method(Symbol name, Formals formals, Symbol return_type, Expression expr)
{
  return new method_class(name, formals, return_type, expr);
}

Feature attr(Symbol name, Symbol type_decl, Expression init)
{
  return new attr_class(name, type_decl, init);
}

Formal formal(Symbol name, Symbol type_decl)
{
  return new formal_class(name, type_decl);
}

Case branch(Symbol name, Symbol type_decl, Expression expr)
{
  return new branch_class(name, type_decl, expr);
}

Expression assign(Symbol name, Expression expr)
{
  return new assign_class(name, expr);
}

Expression static_dispatch(Expression expr, Symbol type_name, Symbol name, Expressions actual)
{
  return new static_dispatch_class(expr, type_name, name, actual);
}

Expression dispatch(Expression expr, Symbol name, Expressions actual)
{
  return new dispatch_class(expr, name, actual);
}

Expression cond(Expression pred, Expression then_exp, Expression else_exp)
{
  return new cond_class(pred, then_exp, else_exp);
}

Expression loop(Expression pred, Expression body)
{
  return new loop_class(pred, body);
}

Expression typcase(Expression expr, Cases cases)
{
  return new typcase_class(expr, cases);
}

Expression block(Expressions body)
{
  return new block_class(body);
}

Expression let(Symbol identifier, Symbol type_decl, Expression init, Expression body)
{
  return new let_class(identifier, type_decl, init, body);
}

Expression plus(Expression e1, Expression e2)
{
  return new plus_class(e1, e2);
}

Expression sub(Expression e1, Expression e2)
{
  return new sub_class(e1, e2);
}

Expression mul(Expression e1, Expression e2)
{
  return new mul_class(e1, e2);
}

Expression divide(Expression e1, Expression e2)
{
  return new divide_class(e1, e2);
}

Expression neg(Expression e1)
{
  return new neg_class(e1);
}

Expression lt(Expression e1, Expression e2)
{
  return new lt_class(e1, e2);
}

Expression eq(Expression e1, Expression e2)
{
  return new eq_class(e1, e2);
}

Expression leq(Expression e1, Expression e2)
{
  return new leq_class(e1, e2);
}

Expression comp(Expression e1)
{
  return new comp_class(e1);
}

Expression int_const(Symbol token)
{
  return new int_const_class(token);
}

Expression bool_const(Boolean val)
{
  return new bool_const_class(val);
}

Expression string_const(Symbol token)
{
  return new string_const_class(token);
}

Expression new_(Symbol type_name)
{
  return new new__class(type_name);
}

Expression isvoid(Expression e1)
{
  return new isvoid_class(e1);
}

Expression no_expr()
{
  return new no_expr_class();
}

Expression object(Symbol name)
{
  return new object_class(name);
}

//
// AST dump
//

void dump_Symbol(std::ostream &  stream, int padding, Symbol b);
void dump_Boolean(std::ostream &  stream, int padding, Boolean b);

// Простой рекурсивный обход абстрактного синтаксического дерева (AST)
// выводит каждый узел и любую связанную с ним информацию о типе
// Используйте dump_with_types для проверки результатов вывода типа.
//
//  Program_class
//     program_class
//
//  Class__class
//     class_
//
//  Feature_class
//     method
//     attr
//
//  Formal_class
//     formal
//
//  Case_class
//     branch_class
//
//  Expression_class
//     assign
//     static_dispatch
//     dispatch
//     cond
//     loop
//     typcase
//     block
//     let
//     plus
//     sub
//     mul
//     divide
//     neg
//     lt
//     eq
//     leq
//     comp
//     int_const
//     bool_const
//     string_const
//     new_
//     isvoid
//     no_expr
//     object
//
// Некоторые узлы AST содержат списки других узлов дерева в качестве компонентов.  Списки в
// AST создаются с использованием класса list_node, определенного в tree.h.  Список
// классов в Cool AST состоит из:
//
//  Classes      a list of Class_
//  Features     a list of Feature
//  Expressions  a list of Expression
//  Cases        a list of Case
//


//
// dump_type выводит тип выражения в выходной поток,
// после ввода правильного количества пробелов.  Выполняется проверка,
// чтобы убедиться, что узлу не присвоен тип.
//
// Обратите внимание, что элемент "type", о котором здесь идет речь, наследуется от tree_node
// всеми подклассами Expression_class.  Также обратите внимание, что dump_type является
// определен здесь для выражения типа и, следовательно, наследуется
// каждым отдельным подклассом выражения.
//

void Expression_class::dump_type(std::ostream &  stream, int n)
{
  if (type)
    { stream << pad(n) << ": " << type << std::endl; }
  else
    { stream << pad(n) << ": _no_type" << std::endl; }
}

void dump_line(std::ostream &  stream, int n, tree_node *t)
{
  stream << pad(n) << '#' << t->get_line_number() << '\n';
}

//
// program_class выводит "program", а затем каждый из
// классов компонентов программы, по одному, с
// большим отступом. Рекурсивный вызов
// "classes->nth(i)->dump_with_types(...)" показывает, насколько полезны
// и компактные виртуальные функции для такого рода вычислений.
//
// Обратите внимание на использование итератора для циклического просмотра всех
// классов.  Методы first, more, next и nth в списках AST
// определены в tree.h.
//
void program_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_program\n";
   for(int i = classes->first(); classes->more(i); i = classes->next(i))
     classes->nth(i)->dump_with_types(stream, n+2);
}

//
// Печатает компоненты класса, включая все функции.
// Обратите внимание, что печать функций - это еще одно применение итератора.
//
void class__class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_class\n";
   dump_Symbol(stream, n+2, name);
   dump_Symbol(stream, n+2, parent);
   stream << pad(n+2) << "\"";
   print_escaped_string(stream, filename->get_string());
   stream << "\"\n" << pad(n+2) << "(\n";
   for(int i = features->first(); features->more(i); i = features->next(i))
     features->nth(i)->dump_with_types(stream, n+2);
   stream << pad(n+2) << ")\n";
}


//
// dump_with_types для method_class сначала выводит, что это метод,
// затем выводит имя метода, за которым следуют формальные параметры
// (еще одно использование итератора, на этот раз доступ ко всем элементам списка
// типа Formal), возвращаемый тип и, наконец, рекурсивно вызывает dump_type
// в теле метода.

void method_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_method\n";
   dump_Symbol(stream, n+2, name);
   for(int i = formals->first(); formals->more(i); i = formals->next(i))
     formals->nth(i)->dump_with_types(stream, n+2);
   dump_Symbol(stream, n+2, return_type);
   expr->dump_with_types(stream, n+2);
}

//
// attr_class::dump_with_types выводит имя атрибута, объявление типа
// и любое выражение инициализации с соответствующим смещением.
//
void attr_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_attr\n";
   dump_Symbol(stream, n+2, name);
   dump_Symbol(stream, n+2, type_decl);
   init->dump_with_types(stream, n+2);
}

//
// formal_class::dump_with_types выводит имя и объявление типа
// формального параметра.
//
void formal_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_formal\n";
   dump_Symbol(stream, n+2, name);
   dump_Symbol(stream, n+2, type_decl);
}

//
// branch_class::dump_with_types выводит имя, объявление типа
// и тело любой ветви case.
//
void branch_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_branch\n";
   dump_Symbol(stream, n+2, name);
   dump_Symbol(stream, n+2, type_decl);
   expr->dump_with_types(stream, n+2);
}

//
// assign_class::dump_with_types выводит "assign", а затем (с отступом)
// присваиваемую переменную, выражение и, наконец, тип
// результата.  Обратите внимание на вызов dump_type (см. выше) в
// конце метода.
//
void assign_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_assign\n";
   dump_Symbol(stream, n+2, name);
   expr->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

//
// static_dispatch_class::dump_with_types выводит выражение,
// класс статической отправки, имя функции и фактические аргументы
// для любой статической отправки.  
//
void static_dispatch_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_static_dispatch\n";
   expr->dump_with_types(stream, n+2);
   dump_Symbol(stream, n+2, type_name);
   dump_Symbol(stream, n+2, name);
   stream << pad(n+2) << "(\n";
   for(int i = actual->first(); actual->more(i); i = actual->next(i))
     actual->nth(i)->dump_with_types(stream, n+2);
   stream << pad(n+2) << ")\n";
   dump_type(stream,n);
}
//
// dispatch_class::dump_with_types аналогичен 
// static_dispatch_class::dump_with_types 
//
void dispatch_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_dispatch\n";
   expr->dump_with_types(stream, n+2);
   dump_Symbol(stream, n+2, name);
   stream << pad(n+2) << "(\n";
   for(int i = actual->first(); actual->more(i); i = actual->next(i))
     actual->nth(i)->dump_with_types(stream, n+2);
   stream << pad(n+2) << ")\n";
   dump_type(stream,n);
}
//
// cond_class::dump_with_types выводит каждое из трех выражений
// в условное выражение, а затем в тип всего выражения.
//
void cond_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_cond\n";
   pred->dump_with_types(stream, n+2);
   then_exp->dump_with_types(stream, n+2);
   else_exp->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

//
// loop_class::dump_with_types выводит предикат, а затем
// тело цикла и, наконец, тип всего выражения.
//
void loop_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_loop\n";
   pred->dump_with_types(stream, n+2);
   body->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

//
// // тип case_class::dump_with_types выводит каждую ветвь
// Case_ по очереди.  Тип всего выражения
// выводится в конце.
//
void typcase_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_typcase\n";
   expr->dump_with_types(stream, n+2);
   for(int i = cases->first(); cases->more(i); i = cases->next(i))
     cases->nth(i)->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

//
// Остальные варианты выражения очень просты
// и не содержат ничего, чего бы уже не было в рассмотренном выше коде
//.
//
void block_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_block\n";
   for(int i = body->first(); body->more(i); i = body->next(i))
     body->nth(i)->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void let_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_let\n";
   dump_Symbol(stream, n+2, identifier);
   dump_Symbol(stream, n+2, type_decl);
   init->dump_with_types(stream, n+2);
   body->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void plus_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_plus\n";
   e1->dump_with_types(stream, n+2);
   e2->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void sub_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_sub\n";
   e1->dump_with_types(stream, n+2);
   e2->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void mul_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_mul\n";
   e1->dump_with_types(stream, n+2);
   e2->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void divide_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_divide\n";
   e1->dump_with_types(stream, n+2);
   e2->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void neg_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_neg\n";
   e1->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void lt_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_lt\n";
   e1->dump_with_types(stream, n+2);
   e2->dump_with_types(stream, n+2);
   dump_type(stream,n);
}


void eq_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_eq\n";
   e1->dump_with_types(stream, n+2);
   e2->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void leq_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_leq\n";
   e1->dump_with_types(stream, n+2);
   e2->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void comp_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_comp\n";
   e1->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void int_const_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_int\n";
   dump_Symbol(stream, n+2, token);
   dump_type(stream,n);
}

void bool_const_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_bool\n";
   dump_Boolean(stream, n+2, val);
   dump_type(stream,n);
}

void string_const_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_string\n";
   stream << pad(n+2) << "\"";
   print_escaped_string(stream,token->get_string());
   stream << "\"\n";
   dump_type(stream,n);
}

void new__class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_new\n";
   dump_Symbol(stream, n+2, type_name);
   dump_type(stream,n);
}

void isvoid_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_isvoid\n";
   e1->dump_with_types(stream, n+2);
   dump_type(stream,n);
}

void no_expr_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_no_expr\n";
   dump_type(stream,n);
}

void object_class::dump_with_types(std::ostream &  stream, int n)
{
   dump_line(stream,n,this);
   stream << pad(n) << "_object\n";
   dump_Symbol(stream, n+2, name);
   dump_type(stream,n);
}
