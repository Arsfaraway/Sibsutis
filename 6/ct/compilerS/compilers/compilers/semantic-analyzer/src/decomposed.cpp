#include <unistd.h>
#include <cstdio>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "cool-parse.h"
#include "cool-tree.h"
#include "utilities.h"

std::FILE *token_file = stdin;
extern Classes parse_results;
extern Program ast_root;
extern int curr_lineno;
const char *curr_filename = "<stdin>";
extern int parse_errors;
// Debug flags
extern int yy_flex_debug;
extern int cool_yydebug;
int lex_verbose = 0;
extern int cool_yyparse();

namespace analyzer {

int err_count = 0;

void semantics_error(std::string error_msg) {
    std::cerr << "\033[1;31m- semantic error: " << error_msg << "\033[0m" << '\n';
    err_count++;
}

void sequence_out(std::string title, std::unordered_set<std::string> set) {
    std::cerr << title << ": ";
    for (auto s : set) {
        std::cerr << s << ' ';
    }
    std::cerr << '\n';
}

// Проверка иерархии наследования
bool detect_cycle(std::unordered_map<std::string, std::string> hierarchy) {
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> currentlyVisiting;
    std::function<bool(const std::string &)> dfs = [&](const std::string &className) {
        // Если вы уже посещали этот сайт, нет необходимости посещать его снова
        if (visited.find(className) != visited.end()) {
            return false;
        }
        // При посещении в данный момент обнаружен цикл
        if (currentlyVisiting.find(className) != currentlyVisiting.end()) {
            return true;
        }
        // Отметить как посещающего в данный момент
        currentlyVisiting.insert(className);
        // Получить родительский класс
        auto it = hierarchy.find(className);
        if (it != hierarchy.end()) {
            // Рекурсивный вызов DFS для родительского класса
            if (dfs(it->second)) {
                return true;
            }
        }
        // Отметить как посещенный и удалить из списка посещаемых в данный момент
        visited.insert(className);
        currentlyVisiting.erase(className);
        return false;
    };

   // Выполнить итерацию по каждому классу в иерархии
    for (const auto &entry : hierarchy) {
        // Check parent existence
        if (hierarchy.find(entry.second) == hierarchy.end() && entry.second != "Object") {
            semantics_error("parent of class '" + entry.first + "' ('" + entry.second + "') doesn't exist");
        }
        if (dfs(entry.first)) {
            return true;  // Обнаружена петля
        }
    }
    return false;  // Петля не обнаружена
}

bool signatures_verification(method_class *m1, method_class *m2) {
    // Получить возвращаемые типы
    TypeVisitor m1_type_visitor;
    TypeVisitor m2_type_visitor;
    m1->accept(m1_type_visitor);
    m2->accept(m2_type_visitor);
    // Методы проверки возвращаемых типов
    if (m1_type_visitor.type != m2_type_visitor.type) {
        return false;
    }

    // Стать официальным
    FormalsVisitor m1_formals_visitor;
    FormalsVisitor m2_formals_visitor;
    m1->accept(m1_formals_visitor);
    m2->accept(m2_formals_visitor);
    Formals m1_formals = m1_formals_visitor.formals;
    Formals m2_formals = m2_formals_visitor.formals;

    // Проверьте количество формальностей
    if (m1_formals->len() != m2_formals->len()) {
        return false;
    }

    // Цикл формальных
    for (int i = m1_formals->first(); m1_formals->more(i); i = m1_formals->next(i)) {
        formal_class *m1_formal = dynamic_cast<formal_class *>(m1_formals->nth(i));
        formal_class *m2_formal = dynamic_cast<formal_class *>(m2_formals->nth(i));

        // Получить официальные названия
        NameVisitor m1_formal_generalVisitor;
        NameVisitor m2_formal_generalVisitor;
        m1_formal->accept(m1_formal_generalVisitor);
        m2_formal->accept(m2_formal_generalVisitor);
        std::string name1 = m1_formal_generalVisitor.name;
        std::string name2 = m2_formal_generalVisitor.name;
        // Проверьте официальные названия
        if (name1 != name2) {
            return false;
        }

        // Получить формальные типы
        TypeVisitor m1_formal_type_visitor;
        TypeVisitor m2_formal_type_visitor;
        m1_formal->accept(m1_formal_type_visitor);
        m2_formal->accept(m2_formal_type_visitor);
        std::string type1 = m1_formal_type_visitor.type;
        std::string type2 = m2_formal_type_visitor.type;
        // Проверка формальных типов
        if (type1 != type2) {
            return false;
        }
    }
    // Все хорошие подписи одинаковы
    return true;
}

class__class *get_class(std::string name, Classes classes) {
    for (int i = classes->first(); classes->more(i); i = classes->next(i)) {
        NameVisitor generalVisitor;
        class__class *cur_class = dynamic_cast<class__class *>(classes->nth(i));
        cur_class->accept(generalVisitor);
        if (name == generalVisitor.name) return cur_class;
    }
    return nullptr;
}

void dump_symtables(IdTable idtable, StrTable strtable, IntTable inttable) {
    ast_root->dump_with_types(std::cerr, 0);
    std::cerr << "# Identifiers:\n";
    idtable.print();
    std::cerr << "# Strings:\n";
    stringtable.print();
    std::cerr << "# Integers:\n";
    inttable.print();
}

void check_builtin_types_init(std::string type, Expression expr) {
    std::string expr_type = expr->get_expr_type();
    if (expr_type == "no_expr_class") return;
    if (type == "Int" && expr_type != "int_const_class")
        semantics_error("Initialization of Int with non-integer value");
    else if (type == "Bool" && expr_type != "bool_const_class")
        semantics_error("Initialization of Bool with non-boolean value");
    else if (type == "String" && expr_type != "string_const_class")
        semantics_error("Initialization of String with non-string value");
}

// void check_classes

};  // анализатор пространства имен

int main(int argc, char **argv) {
    yy_flex_debug = 0;
    cool_yydebug = 0;
    lex_verbose = 0;
    for (int i = 1; i < argc; i++) {
        token_file = std::fopen(argv[i], "r");
        if (token_file == NULL) {
            std::cerr << "Error: specified file not found " << argv[i] << std::endl;
            std::exit(1);
        }
        curr_lineno = 1;
        cool_yyparse();
        if (parse_errors != 0) {
            std::cerr << "Error: parse errors\n";
            std::exit(1);
        }

        // Добавление встроенных типов toAST
        Symbol Object = idtable.add_string("Object");
        Symbol Bool = idtable.add_string("Bool");
        Symbol Int = idtable.add_string("Int");
        Symbol String = idtable.add_string("String");
        Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");

        std::unordered_set<std::string> non_inherited_types{"Bool", "Int", "String", "SELF_TYPE"};
        std::unordered_set<std::string> classes_names{"Object", "Bool", "Int", "String", "SELF_TYPE"};
        std::unordered_map<std::string, std::string> classes_hierarchy;
        NameVisitor generalVisitor;

        // Цикл по классам
        for (int i = parse_results->first(); parse_results->more(i); i = parse_results->next(i)) {
            // Получить текущее имя класса
            parse_results->nth(i)->accept(generalVisitor);
            std::string class_name = generalVisitor.name;

            /* ---------------------- Проверьте проверку уникального имени класса ---------------------------------* */
            auto result = classes_names.insert(class_name);
            if (!result.second) analyzer::semantics_error("class '" + class_name + "' already exists!");

            /* --------------------- Добавление класса в иерархию наследования --------------------------------* */
            ParentVisitor parentVisitor;
            parse_results->nth(i)->accept(parentVisitor);
            std::string parent_name = parentVisitor.name;
            classes_hierarchy[class_name] = parent_name;

            /* ----------- Убедитесь, что родительский класс не встроен (кроме 'Object'). ---------------------* */
            if (non_inherited_types.find(parent_name) != non_inherited_types.end())
                analyzer::semantics_error("class '" + class_name + "': shouldn't use '" + parent_name + "' as parent");

            /* ---------------------------- Получите классные возможности -----------------------------------------* */
            FeaturesVisitor featuresVisitor;
            parse_results->nth(i)->accept(featuresVisitor);
            Features features = featuresVisitor.features;
            std::unordered_set<std::string> features_names;

            /* ---------------------------- Циклический просмотр объектов --------------------------------------* */
            for (int j = features->first(); features->more(j); j = features->next(j)) {
                // Текущая функция
                Feature feature = features->nth(j);

                // Получить название функции
                feature->accept(generalVisitor);
                std::string feature_name = generalVisitor.name;

               // проверка "собственного" имени
                if (feature_name == "self") analyzer::semantics_error("Name 'self' can't be used as feature name");

                // Проверьте уникальное название функции
                result = features_names.insert(feature_name);
                if (!result.second) analyzer::semantics_error("Feature '" + feature_name + "' in '" + class_name + "' already exists");

                // Getfeaturetype: методы - return_type, атрибуты - type_decl
                TypeVisitor type_visitor;
                feature->accept(type_visitor);

                // Проверка существования типа
                std::string type = type_visitor.type;
                if (classes_names.find(type) == classes_names.end()) analyzer::semantics_error("Unknown type '" + type + "' in " + feature_name);

                // Проверка SELF_TYPE
                if (type == "SELF_TYPE") analyzer::semantics_error("SELF_TYPE can't be used as a type inside class");

                if (feature->get_feature_type() == "method_class") {
                    // Методы формальные
                    FormalsVisitor formals_visitor;
                    feature->accept(formals_visitor);
                    Formals formals = formals_visitor.formals;

                    // Проверка переопределения метода - должен иметь одинаковую подпись
                    if (std::string(parent_name) != "Object") {
                        // Получить функции родительского класса
                        FeaturesVisitor parent_featuresVisitor;
                        class__class *parent = analyzer::get_class(parent_name, parse_results);
                        if (parent) {
                            parent->accept(parent_featuresVisitor);
                            Features parent_features = parent_featuresVisitor.features;

                            // Перебор родительских функций
                            for (int a = parent_features->first(); parent_features->more(a); a = parent_features->next(a)) {
                                Feature parent_feature = parent_features->nth(a);

                               // Получить название функции
                                parent_feature->accept(generalVisitor);
                                std::string parent_feature_name = generalVisitor.name;

                                // Если есть родительская функция с таким же именем
                                if (parent_feature_name == feature_name) {
                                    // Проверьте, относится ли объект к тому же типу
                                    if (parent_feature->get_feature_type() != feature->get_feature_type()) {
                                        analyzer::semantics_error("wrong override of feature '" + feature_name + "' from class '" + parent_name + "' in class '" +
                                                                  class_name + "'");
                                    }

                                    // Проверка сигнатур методов
                                    method_class *cur_method = dynamic_cast<method_class *>(feature);
                                    method_class *parent_method = dynamic_cast<method_class *>(parent_feature);
                                    if (!analyzer::signatures_verification(cur_method, parent_method)) {
                                        analyzer::semantics_error("'" + feature_name + "' method from class '" + parent_name +
                                                                  "' doesn't match override version "
                                                                  "of it in class '" +
                                                                  class_name + "'");
                                    }
                                }
                            }
                        } else {
                            analyzer::semantics_error("parent class '" + std::string(parent_name) + "' of class '" + class_name + "' doesn't exist");
                        }
                    }

                    // Местные официальные названия
                    std::unordered_set<std::string> formals_names;

                    // Цикл формальных
                    for (int k = formals->first(); formals->more(k); k = formals->next(k)) {
                        // Получить официальное название
                        formals->nth(k)->accept(generalVisitor);
                        std::string formal_name = generalVisitor.name;

                        // проверка "собственного" имени
                        if (formal_name == "self") analyzer::semantics_error("Can't use 'self' as formal name");

                        // Проверка уникальности имени
                        result = formals_names.insert(formal_name);
                        if (!result.second) analyzer::semantics_error("Formal '" + std::string(formal_name) + "' in '" + feature_name + "' already exists!");

                       // Получить формальный тип
                        formals->nth(k)->accept(type_visitor);
                        type = type_visitor.type;

                        // Проверьте формальный тип
                        if (classes_names.find(type) == classes_names.end()) analyzer::semantics_error("Unknown type '" + type + "' in " + formal_name);

                        // Получить выражение метода
                        ExpressionVisitor expr_visitor;
                        features->nth(j)->accept(expr_visitor);
                        Expression expr = expr_visitor.expr;

                        // проверка блокового класса
                        if (expr->get_expr_type() == "block_class") {
                            // Получать выражения из блока
                            ExpressionsVisitor exprs_visitor;
                            expr->accept(exprs_visitor);
                            Expressions exprs = exprs_visitor.exprs;

                            // Проверка выражений блоков
                            for (int l = exprs->first(); exprs->more(l); l = exprs->next(l)) {
                                Expression current = exprs->nth(l);

                                // let
                                if (current->get_expr_type() == "let_class") {
                                    // Получить имя переменной let-expr
                                    current->accept(generalVisitor);
                                    formal_name = generalVisitor.name;

                                    // проверка "собственного" имени
                                    if (formal_name == "self") {
                                        analyzer::semantics_error(
                                            "can't use 'self' as new local "
                                            "variable name");
                                    }

                                    // Проверить уникальность вложенного формального
                                    result = formals_names.insert(formal_name);
                                    if (!result.second) {
                                        analyzer::semantics_error("formal '" + std::string(formal_name) + "' in '" + feature_name + "' from '" + class_name +
                                                                  "' already exists!");
                                    }

                                    // Let-проверка формального типа выражения
                                    current->accept(type_visitor);
                                    type = type_visitor.type;
                                    if (classes_names.find(type) == classes_names.end()) {
                                        analyzer::semantics_error("unknown type '" + type + "' in " + formal_name);
                                    }
                                }
                            }
                        }
                    }
                } else {
                    // Проверить инициализацию выражения attr_class
                    attr_class *attr = dynamic_cast<attr_class *>(feature);
                    attr->accept(type_visitor);
                    ExpressionVisitor getExpr;
                    attr->accept(getExpr);
                    analyzer::check_builtin_types_init(type_visitor.type, getExpr.expr);
                }
            }

            // Проверьте наличие метода main в классе Main
            if (class_name == "Main" && features_names.find("main") == features_names.end()) analyzer::semantics_error("Method 'main' in 'Main' class does not exist");

            // Сбросить все функции
            // analyzer::sequence_out("\033[1;33mFeatures (methods + attributes) of '" + class_name + '\'' + "\033[0m", features_names);
        }

        // Проверить наличие класса Main
        if (classes_names.find("Main") == classes_names.end()) analyzer::semantics_error("Start point - class 'Main' doesn't exist");

        // Сбросить все классы
        // analyzer::sequence_out("Classes (types)", classes_names);

        // Проверка цикла иерархии наследования
        if (analyzer::detect_cycle(classes_hierarchy)) {
            analyzer::semantics_error("loop detected in classes inheritance hierarchy");
            // std::cerr << "\\ program classes' hierarchy (child : parent)\n";
            // for (auto p : classes_hierarchy) {
            //     std::cerr << "\033[1;33m" << '\t' << p.first << " : " << p.second << "\033[0m"
            //               << "\n";
            // }
        }
        std::fclose(token_file);
    }

    std::cerr << "\n[I] Detected " << analyzer::err_count << " semantic errors\n";

    return analyzer::err_count;
}
