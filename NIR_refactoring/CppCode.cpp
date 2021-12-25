// файл CppCode.cpp - реализация методов класса CppCode

#include "CppCode.h"

CppCode::CppCode() {
    this->cppCode = "";
    this->lexemes = {};
    this->methods = {};
}

/// <summary>Конструктор класса</summary>
CppCode::CppCode(string cppCode) {
    this->cppCode = cppCode;
    this->lexemes = this->GetLexemes();
    this->methods = this->GetMethods();
}

/// <summary>Дополнительные действия для разбиения кода на лексемы</summary>
string CppCode::Modify() {
    string modCode = this->cppCode;
    for (int i = 0; i < modCode.length(); i++) {
        if (((modCode[i + 1] == '(') || (modCode[i + 1] == ')') ||
            (modCode[i + 1] == '{') || (modCode[i + 1] == '}') ||
            (modCode[i + 1] == ';') ||
            (modCode[i + 1] == ',') ||
            (modCode[i + 1] == '=') || (modCode[i + 1] == '+') || (modCode[i + 1] == '-') ||
            (modCode[i + 1] == '*') || (modCode[i + 1] == '/') || (modCode[i + 1] == '%') ||
            (modCode[i + 1] == '&') || (modCode[i + 1] == '|') || (modCode[i + 1] == '^')) &&
            (i < (modCode.length() - 1)))
            modCode.insert(i++, " ");
        if (((modCode[i] == '(') || (modCode[i] == ')') ||
            (modCode[i] == '{') || (modCode[i] == '}') ||
            (modCode[i] == ';') || (modCode[i] == ':') ||
            (modCode[i] == ',') ||
            (modCode[i] == '=') || (modCode[i] == '+') || (modCode[i] == '-') ||
            (modCode[i] == '*') || (modCode[i] == '/') || (modCode[i] == '%') ||
            (modCode[i] == '&') || (modCode[i] == '|') || (modCode[i] == '^')) ||
            (((modCode[i] == '<') || (modCode[i] == '>')) &&
                (modCode[i - 1] == modCode[i]) && (i > 0)))
            modCode.insert((i + 1), " ");
        if (((modCode[i + 1] == '<') || (modCode[i + 1] == '>')) &&
            (modCode[i + 2] == modCode[i + 1]) && (i < (modCode.length() - 2)))
            modCode.insert((i += 2), " ");
    }
    return modCode;
}

/// <summary>Разбиение кода на лексемы</summary>
vector<string> CppCode::GetLexemes() {
    setlocale(LC_ALL, "Russian");
    vector<string> vec_lexemes = {};
    const char* separators = " \n\t";
    string modCppCode = this->Modify();
    istringstream ssCppCode(modCppCode + " ₽₴.");
    while (1) {
        string lexeme = "";
        ssCppCode >> lexeme;
        if (lexeme == "₽₴.") {
            break;
        }
        else {
            if (lexeme != "")
                vec_lexemes.push_back(lexeme);
        }
    }
    return vec_lexemes;
}

/// <summary>Получение типов данных, описанных в коде</summary>
/// <returns>- вектор пользовательских типов данных</returns>
vector<string> CppCode::GetTypes(string type) {
    vector<string> types = {};
    for (int i = 0; i < this->lexemes.size(); i++) {
        if (this->lexemes[i] == type) {
            types.push_back(this->lexemes[i + (type == "typedef" ? 2 : 1)]);
        }
    }
    return types;
}

/// <summary>Получение вектора методов</summary>
/// <returns>- вектор методов в коде</returns>
vector<CppMethod> CppCode::GetMethods() {
    vector<CppMethod> methods = {};
    vector<string> types = {
        "int",
        "float",
        "double",
        "long",
        "short",
        "string",
        "char",
        "bool",
        "void",
        "auto"
    };
    vector<string> typedefs = this->GetTypes("typedef"),
        structs = this->GetTypes("struct"),
        classes = this->GetTypes("class");
    for (int i = 0; i < typedefs.size(); i++)
        types.push_back(typedefs[i]);
    for (int i = 0; i < structs.size(); i++)
        types.push_back(structs[i]);
    for (int i = 0; i < classes.size(); i++)
        types.push_back(classes[i]);
    for (int i = 0; i < this->lexemes.size(); i++) {
        vector<string> curMethod = {};
        for (int j = 0; j < types.size(); j++) {
            cout << j << "\t" << types.size() << endl;
            // здесь я ловлю выход за границы массива
            if ((this->lexemes[i] == types[j]) &&
                (this->lexemes[i + 2] == "(") && 
                (i < this->lexemes.size() - 2)) {
                int depth = 0,
                    k = 0;
                for (k = i; k < this->lexemes.size(); k++) {
                    if (this->lexemes[k] == "{")
                        depth++;
                    if (this->lexemes[k] == "}")
                        if (depth > 0)
                            depth--;
                        else
                            break;
                }
                for (int l = i; l < k; l++) {
                    curMethod.push_back(this->lexemes[l]);
                }
                i = k;
            }
        }
        methods.push_back(CppMethod(curMethod));
    }
    return methods;
}

void CppCode::UpdateMethods(vector<CppMethod> methods) {
    this->methods = methods;
}

void CppCode::UpdateLexemes() {
    vector<string> newLexemes = {},
        curLexemes = this->lexemes;
    for (int i = 0; i < this->methods.size(); i++) {
        for (int j = 0; j < curLexemes.size(); j++) {
            newLexemes.push_back(curLexemes[j]);
            if ((curLexemes[j] == this->methods[i].type) && 
                (curLexemes[j + 1] == this->methods[i].name)) {
                int k = 0,
                    depth = 0;
                for (k = j + 1; k < curLexemes.size(); k++) {
                    if (curLexemes[k] == "{")
                        depth++;
                    if ((curLexemes[k] == "}") && (depth > 0)) {
                        depth--;
                        if (depth == 0)
                            break;
                    }
                }
                j = k;
                vector<string> newMethodLexemes = this->methods[i].ToLexemes();
                for (int l = 0; l < newMethodLexemes.size(); l++)
                    newLexemes.push_back(newMethodLexemes[l]);
            }
        }
        curLexemes = newLexemes;
    }
    this->lexemes = curLexemes;
}

string CppCode::ToString() {
    stringstream cppCodeStream;
    for (int i = 0; i < this->lexemes.size(); i++)
        cppCodeStream << this->lexemes[i];
    return cppCodeStream.str();
}