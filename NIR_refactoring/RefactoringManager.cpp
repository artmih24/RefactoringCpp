// файл RefactoringManager.cpp - реализаци€ методов класса RefactoringManager

#include "RefactoringManager.h"

/// <summary> онструктор объекта класса RefactoringManager</summary>
/// <param name="filePath">- путь к файлу с исходным кодом</param>
RefactoringManager::RefactoringManager(string filePath) {
    this->filePath = filePath;
    this->outFilePath = this->GetOutputFileName(this->filePath);
    cout << this->outFilePath << endl;
}

/// <summary>ѕроведение рефакторинга</summary>
void RefactoringManager::Refactoring() {
    this->GetFileContent(this->filePath);
    this->code = CppCode(this->fileContent);
    this->RemoveAssignmentsToParametersRefactoring(this->code);
    this->RemoveParametersRefactoring(this->code);
    this->fileContent = this->code.ToString();
    WriteFile(this->outFilePath);
}

/// <summary>ѕроведение рефакторинга методом удалени€ присваиваний параметрам</summary>
/// <param name="code">- исходный код программы</param>
void RefactoringManager::RemoveAssignmentsToParametersRefactoring(CppCode code) {
    vector<CppMethod> methods = code.GetMethods();
    for (int i = 0; i < methods.size(); i++) {
        methods[i] = this->RemoveAssignmentsToParameters(methods[i]);
    }
    code.UpdateMethods(methods);
    code.UpdateLexemes();
    this->code = code;
}

/// <summary>”даление присваиваний параметрам метода</summary>
/// <param name="method">- изначальный метод</param>
/// <returns>новый метод</returns>
CppMethod RefactoringManager::RemoveAssignmentsToParameters(CppMethod method) {
    CppMethod newCppMethod,
        var_method = method;
    // цикл по всем параметрам
    for (int i = 0; i < method.parameters.size(); i++) {
        newCppMethod.body = {};
        newCppMethod.parameters = method.parameters;
        bool assignment = false;
        // проверка тела метода на наличие присваиваний параметру в нем
        for (int j = 0; j < var_method.body.size(); j++) {
            // если есть присваивание
            if ((var_method.body[j] == var_method.parameters[i].name) &&
                (((var_method.body[j + 1] == "=") && (j < var_method.body.size() - 1)) ||
                    ((((var_method.body[j + 1] == "+") || (var_method.body[j + 1] == "-") ||
                        (var_method.body[j + 1] == "*") || (var_method.body[j + 1] == "/") ||
                        (var_method.body[j + 1] == "%") || (var_method.body[j + 1] == "&") ||
                        (var_method.body[j + 1] == "|") || (var_method.body[j + 1] == "^")) &&
                        (var_method.body[j + 2] == "=")) && (j < var_method.body.size() - 2)) ||
                    (((var_method.body[j + 1] == "<") || (var_method.body[j + 1] == ">")) &&
                        (var_method.body[j + 2] == var_method.body[j + 1]) &&
                        (var_method.body[j + 3] == "=") && (j < var_method.body.size() - 3)))) {
                assignment = true;
                break;
            }
        }
        // если есть присваивание
        if (assignment) {
            int start = -1;
            for (int j = 0; j < var_method.body.size(); j++) {
                // замен€ем его на новую переменную, остальные фрагменты кода записываем как есть
                if (var_method.body[j] == var_method.parameters[i].name)
                    newCppMethod.body.push_back("var_" + var_method.body[j]);
                else
                    newCppMethod.body.push_back(var_method.body[j]);
                // в начало тела метода добавл€ем новую переменную, которой затем присвоим значение параметра
                if ((var_method.body[j] == "{") && (start == -1)) {
                    newCppMethod.body.push_back(var_method.parameters[i].type);
                    newCppMethod.body.push_back("var_" + var_method.parameters[i].name);
                    newCppMethod.body.push_back("=");
                    newCppMethod.body.push_back(var_method.parameters[i].name);
                    newCppMethod.body.push_back(";");
                    start = j;
                }
            }
            // запомним метод перед следующей итерацией
            var_method = newCppMethod;
        }
    }
    return var_method;
}

/// <summary>ѕроведение рефакторинга методом удалени€ параметров</summary>
/// <param name="code">- исходный код программы</param>
void RefactoringManager::RemoveParametersRefactoring(CppCode code) {
    vector<CppMethod> methods = code.GetMethods();
    for (int i = 0; i < methods.size(); i++) {
        methods[i] = this->RemoveParameters(methods[i]);
    }
    code.UpdateMethods(methods);
    code.UpdateLexemes();
    this->code = code;
}

/// <summary>”даление параметров метода</summary>
/// <param name="method">- изначальный метод</param>
/// <returns>новый метод</returns>
CppMethod RefactoringManager::RemoveParameters(CppMethod method) {
    CppMethod newCppMethod,
        var_method = method;
    // цикл по всем параметрам
    for (int i = 0; i < method.parameters.size(); i++) {
        newCppMethod.body = method.body;
        newCppMethod.parameters = {};
        bool usingParameter = false;
        // провер€ем, используетс€ ли параметр в теле метода
        for (int j = 0; j < var_method.body.size(); j++) {
            // если параметр используетс€
            if (var_method.body[j] == var_method.parameters[i].name) {
                usingParameter = true;
                break;
            }
        }
        // если используетс€, то добавл€ем его в список параметров нового метода
        if (usingParameter) {
            newCppMethod.parameters.push_back(var_method.parameters[i]);
            // запомним метод перед следующей итерацией
            var_method = newCppMethod;
        }
    }
    return var_method;
}

/// <summary>—оставление названи€ файла, в котором будет содержатьс€ измененный код</summary>
/// <param name="filePath">- полное название исходного файла</param>
/// <returns>название файла, в котором будет содержатьс€ измененный код</returns>
string RefactoringManager::GetOutputFileName(string filePath) {
    if (filePath.substr(filePath.length() - 4, filePath.length()) == ".cpp") {
        filePath.resize(filePath.length() - 4);
        filePath += "_refactored.cpp";
    }
    else if (filePath.substr(filePath.length() - 2, filePath.length()) == ".c") {
        filePath.resize(filePath.length() - 2);
        filePath += "_refactored.c";
    }
    return filePath;
}

/// <summary>„тение файла</summary>
/// <param name="filePath">- полное название файла</param>
void RefactoringManager::GetFileContent(string filePath) {
    string fileString = "";
    this->fileStream.open(filePath);
    while (!fileStream.eof()) {
        getline(this->fileStream, fileString);
        this->stringStream << fileString << endl;
    }
    this->fileContent = this->stringStream.str();
    this->fileStream.close();
}

/// <summary>«апись в файл</summary>
/// <param name="filePath">- полное название файла</param>
void RefactoringManager::WriteFile(string filePath) {
    string outString = "";
    this->outFileStream.open(filePath, ios::out);
    this->outStringStream << this->fileContent;
    while (!outStringStream.eof()) {
        getline(this->outStringStream, outString);
        this->outFileStream << outString << endl;
    }
    this->outFileStream.close();
}