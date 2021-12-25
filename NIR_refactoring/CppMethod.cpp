// файл CppMethod.cpp - реализация методов класса CppMethod

#include "CppMethod.h"

CppMethod::CppMethod() {
    lexemes = {};
    parameters = {};
    body = {};
    type = "";
    name = "";
}

CppMethod::CppMethod(vector<string> lexemes_) {
    lexemes = lexemes_;
    parameters = this->GetMethodParameters();
    body = this->GetMethodBody();
    type = this->GetMethodType();
    name = this->GetMethodName();
}

vector<Parameter> CppMethod::GetMethodParameters() {
    vector<string> lexemes = this->lexemes;
    vector<Parameter> parameters = {};
    bool params = false;
    for (int i = 0; i < lexemes.size(); i++) {
        Parameter parameter = {};
        if (lexemes[i] == "(") {
            params = true;
            continue;
        }
        if (lexemes[i] == ")") {
            params = false;
            break;
        }
        if (params) {
            if (lexemes[i + 2] == ",") {
                parameter.type = lexemes[i];
                parameter.name = lexemes[i];
                i += 2;
            }
            parameters.push_back(parameter);
        }
    }
    return parameters;
}

vector<string> CppMethod::GetMethodBody() {
    vector<string> lexemes = this->lexemes,
        bodyLexemes = {};
    int depth = 0, 
        i = 0, 
        start = -1;
    for (i = 0; i < lexemes.size(); i++) {
        if (lexemes[i] == "{") {
            depth++;
            if (start == -1)
                start = i;
        }
        if (lexemes[i] == "}")
            if (depth > 0)
                depth--;
            else
                break;
    }
    for (int j = start; j < i; j++) {
        bodyLexemes.push_back(lexemes[j]);
    }
    return bodyLexemes;
}

string CppMethod::GetMethodType() {
    return this->lexemes[0];
}

string CppMethod::GetMethodName() {
    return this->lexemes[1];
}

vector<string> CppMethod::ToLexemes() {
    vector<string> lexemes = {};
    lexemes.push_back(this->type);
    lexemes.push_back("(");
    for (int i = 0; i < this->parameters.size(); i++) {
        lexemes.push_back(this->parameters[i].type);
        lexemes.push_back(this->parameters[i].name);
        if (i < this->parameters.size() - 1)
            lexemes.push_back(",");
    }
    lexemes.push_back(")");
    for (int i = 0; i < this->body.size(); i++) {
        lexemes.push_back(this->body[i]);
    }
    return lexemes;
}