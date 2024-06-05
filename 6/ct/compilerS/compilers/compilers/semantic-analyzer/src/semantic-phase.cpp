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

using StrSet = std::unordered_set<std::string>;
using StrSetInsResult = std::pair<StrSet::iterator, bool>;
using StrToStr = std::unordered_map<std::string, std::string>;

int err_count = 0;

void semantics_error(std::string error_msg) {
    std::cerr << "\033[1;31m- semantic error: " << error_msg << "\033[0m" << '\n';
    err_count++;
}

// выводит элементы множества строк set в стандартный поток ошибок std::cerr
void sequence_out(std::string title, StrSet set) {
    std::cerr << title << ": ";
    for (auto s : set) {
        std::cerr << s << ' ';
    }
    std::cerr << '\n';
}

// Проверка иерархии наследования
bool detect_cycle(StrToStr hierarchy) {
    StrSet visited;
    StrSet currentlyVisiting;
    std::function<bool(const std::string &)> dfs = [&](const std::string &className) {
        // Если вы уже посещали эту вершину, нет необходимости посещать ее снова
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
        //Отметить как посещенный и удалить из списка посещаемых в данный момент
        visited.insert(className);
        currentlyVisiting.erase(className);
        return false;
    };

    // Выполнить итерацию по каждому классу в иерархии
    for (const auto &entry : hierarchy) {
        // Проверить наличие родительского элемента
        if (hierarchy.find(entry.second) == hierarchy.end() && entry.second != "Object") {
            semantics_error("parent of class '" + entry.first + "' ('" + entry.second + "') doesn't exist");
        }
        if (dfs(entry.first)) {
            return true;  // Обнаружена петля
        }
    }
    return false;  // Петля не обнаружена
}


// для проверки сигнатур двух методов m1 и m2. 
// Она осуществляет проверку на совпадение типов возвращаемых значений, количества и типов формальных параметров.
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

    // Get formals
    FormalsVisitor m1_formals_visitor;
    FormalsVisitor m2_formals_visitor;
    m1->accept(m1_formals_visitor);
    m2->accept(m2_formals_visitor);
    Formals m1_formals = m1_formals_visitor.formals;
    Formals m2_formals = m2_formals_visitor.formals;

    // Check formals count
    if (m1_formals->len() != m2_formals->len()) {
        return false;
    }

    // Цикл формальных
    for (int i = m1_formals->first(); m1_formals->more(i); i = m1_formals->next(i)) {
        formal_class *m1_formal = dynamic_cast<formal_class *>(m1_formals->nth(i));
        formal_class *m2_formal = dynamic_cast<formal_class *>(m2_formals->nth(i));

        // Get formal names
        NameVisitor m1_formal_generalVisitor;
        NameVisitor m2_formal_generalVisitor;
        m1_formal->accept(m1_formal_generalVisitor);
        m2_formal->accept(m2_formal_generalVisitor);
        std::string name1 = m1_formal_generalVisitor.name;
        std::string name2 = m2_formal_generalVisitor.name;
        // Check formal names
        if (name1 != name2) {
            return false;
        }

        // Get formal types
        TypeVisitor m1_formal_type_visitor;
        TypeVisitor m2_formal_type_visitor;
        m1_formal->accept(m1_formal_type_visitor);
        m2_formal->accept(m2_formal_type_visitor);
        std::string type1 = m1_formal_type_visitor.type;
        std::string type2 = m2_formal_type_visitor.type;
        // Check formal types
        if (type1 != type2) {
            return false;
        }
    }
    // All good - signatures are the same
    return true;
}

// выполняет итерацию по списку классов и сравнивает имя каждого класса с заданным именем. 
// Если имя совпадает, функция возвращает указатель на этот класс.
class__class *get_class(std::string name, Classes classes) {
    for (int i = classes->first(); classes->more(i); i = classes->next(i)) {
        NameVisitor generalVisitor;
        class__class *cur_class = dynamic_cast<class__class *>(classes->nth(i));
        cur_class->accept(generalVisitor);
        if (name == generalVisitor.name) return cur_class;
    }
    return nullptr;
}

// для вывода содержимого символьных таблиц (таблиц идентификаторов, строк и целых чисел) в стандартный поток ошибок
void dump_symtables(IdTable idtable, StrTable strtable, IntTable inttable) {
    ast_root->dump_with_types(std::cerr, 0);
    std::cerr << "# Identifiers:\n";
    idtable.print();
    std::cerr << "# Strings:\n";
    stringtable.print();
    std::cerr << "# Integers:\n";
    inttable.print();
}

//для проверки корректности инициализации встроенных типов в Cool.
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

// для проверки переопределения (override) функций и атрибутов в дочерних классах по отношению к их родительским классам
void check_parent_features(Features parent_features, NameVisitor &generalVisitor, std::string parent_name, std::string class_name, std::string feature_name,
                           Feature feature) {
    for (int a = parent_features->first(); parent_features->more(a); a = parent_features->next(a)) {
        Feature parent_feature = parent_features->nth(a);

        // Get feature name
        parent_feature->accept(generalVisitor);
        std::string parent_feature_name = generalVisitor.name;

        // Если есть родительская функция с таким же именем
        if (parent_feature_name == feature_name) {
            // Проверьте, относится ли объект к тому же типу
            if (parent_feature->get_feature_type() != feature->get_feature_type()) {
                analyzer::semantics_error("wrong override of feature '" + feature_name + "' from class '" + parent_name + "' in class '" + class_name + "'");
            }

            // Check method signatures
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
}

// используется для анализа блока выражений (expression block) в методе класса.
void check_block(Expressions exprs, NameVisitor &generalVisitor, std::string class_name, std::string feature_name, std::string type, std::string formal_name,
                 StrSetInsResult result, StrSet formals_names, StrSet classes_names, TypeVisitor &type_visitor) {
    for (int l = exprs->first(); exprs->more(l); l = exprs->next(l)) {
        Expression current = exprs->nth(l);

        // let
        if (current->get_expr_type() == "let_class") {
            // Получить имя переменной let-expr
            current->accept(generalVisitor);
            formal_name = generalVisitor.name;

            // 'self' name check
            if (formal_name == "self") {
                analyzer::semantics_error(
                    "can't use 'self' as new local "
                    "variable name");
            }

            // Check unique of nested (вложенный) formal
            result = formals_names.insert(formal_name);
            if (!result.second) {
                analyzer::semantics_error("formal '" + std::string(formal_name) + "' in '" + feature_name + "' from '" + class_name + "' already exists!");
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

// используется для проверки формальных параметров метода или атрибута класса.
void check_formals(Formals formals, StrSetInsResult result, NameVisitor &generalVisitor, TypeVisitor &type_visitor, StrSet formals_names, StrSet classes_names,
                   std::string feature_name, Features features, std::string type, std::string class_name, int j) {
    for (int k = formals->first(); formals->more(k); k = formals->next(k)) {
        // Get formal name
        formals->nth(k)->accept(generalVisitor);
        std::string formal_name = generalVisitor.name;

        // 'self' name check
        if (formal_name == "self") analyzer::semantics_error("Can't use 'self' as formal name");

        // Unique name check
        result = formals_names.insert(formal_name);
        if (!result.second) analyzer::semantics_error("Formal '" + std::string(formal_name) + "' in '" + feature_name + "' already exists!");

        // Get formal type
        formals->nth(k)->accept(type_visitor);
        type = type_visitor.type;

        // Check formal type
        if (classes_names.find(type) == classes_names.end()) analyzer::semantics_error("Unknown type '" + type + "' in " + formal_name);

        // Get method expression
        ExpressionVisitor expr_visitor;
        features->nth(j)->accept(expr_visitor);
        Expression expr = expr_visitor.expr;

        // block_class check
        if (expr->get_expr_type() == "block_class") {
            // Get expressions from block
            ExpressionsVisitor exprs_visitor;
            expr->accept(exprs_visitor);
            Expressions exprs = exprs_visitor.exprs;

            // Block expressions check
            check_block(exprs, generalVisitor, class_name, feature_name, type, formal_name, result, formals_names, classes_names, type_visitor);
        }
    }
}

//  предназначена для проверки всех атрибутов и методов класса
void check_features(Features features, StrSet &features_names, StrSet &classes_names, std::string class_name, NameVisitor &generalVisitor, StrSetInsResult result,
                    std::string &parent_name) {
    for (int j = features->first(); features->more(j); j = features->next(j)) {
        // Current feature
        Feature feature = features->nth(j);

        // Get feature name
        feature->accept(generalVisitor);
        std::string feature_name = generalVisitor.name;

        // 'self' name check
        if (feature_name == "self") analyzer::semantics_error("Name 'self' can't be used as feature name");

        // Check unique feature name
        result = features_names.insert(feature_name);
        if (!result.second) analyzer::semantics_error("Feature '" + feature_name + "' in '" + class_name + "' already exists");

        // Get feature type: methods - return_type, attrs - type_decl
        TypeVisitor type_visitor;
        feature->accept(type_visitor);

        // Type existence check
        std::string type = type_visitor.type;
        if (classes_names.find(type) == classes_names.end()) analyzer::semantics_error("Unknown type '" + type + "' in " + feature_name);

        // SELF_TYPE check
        if (type == "SELF_TYPE") analyzer::semantics_error("SELF_TYPE can't be used as a type inside class");

        if (feature->get_feature_type() == "method_class") {
            // Methods formals
            FormalsVisitor formals_visitor;
            feature->accept(formals_visitor);
            Formals formals = formals_visitor.formals;

            // Check method overrides - must have same signature
            if (std::string(parent_name) != "Object") {
                // Get parent class features
                FeaturesVisitor parent_featuresVisitor;
                class__class *parent = analyzer::get_class(parent_name, parse_results);
                if (parent) {
                    parent->accept(parent_featuresVisitor);
                    Features parent_features = parent_featuresVisitor.features;

                    // Loop through parent features
                    check_parent_features(parent_features, generalVisitor, parent_name, class_name, feature_name, feature);
                } else {
                    analyzer::semantics_error("parent class '" + std::string(parent_name) + "' of class '" + class_name + "' doesn't exist");
                }
            }

            // Local formals names
            StrSet formals_names;

            // Loop through formals
            check_formals(formals, result, generalVisitor, type_visitor, formals_names, classes_names, feature_name, features, type, class_name, j);
        } else {
            // Check init expression attr_class
            attr_class *attr = dynamic_cast<attr_class *>(feature);
            attr->accept(type_visitor);
            ExpressionVisitor getExpr;
            attr->accept(getExpr);
            analyzer::check_builtin_types_init(type_visitor.type, getExpr.expr);
        }
    }
}

// используется для проверки всех классов программы
void check_classes(Classes parse_results, IdTable idtable, IntTable inttable, StrSet &classes_names, StrToStr &classes_hierarchy, StrSet &non_inherited_types,
                   NameVisitor &generalVisitor) {
    for (int i = parse_results->first(); parse_results->more(i); i = parse_results->next(i)) {
        // Get current class name
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
        StrSet features_names;

        /* ---------------------------- Циклический просмотр объектов --------------------------------------* */
        check_features(features, features_names, classes_names, class_name, generalVisitor, result, parent_name);

        // Проверьте наличие метода main в классе Main
        if (class_name == "Main" && features_names.find("main") == features_names.end()) analyzer::semantics_error("Method 'main' in 'Main' class does not exist");

        // Сбросить все функции
        // анализатор::sequence_out("\033[1;33m Функций (методов + атрибутов) '" + class_name + '\" + "\033[0m", имена функций);
    }
}

};  // namespace analyzer

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

        analyzer::StrSet non_inherited_types{"Bool", "Int", "String", "SELF_TYPE"};
        analyzer::StrSet classes_names{"Object", "Bool", "Int", "String", "SELF_TYPE"};
        analyzer::StrToStr classes_hierarchy;
        NameVisitor generalVisitor;

        // Цикл по классам
        analyzer::check_classes(parse_results, idtable, inttable, classes_names, classes_hierarchy, non_inherited_types, generalVisitor);

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
