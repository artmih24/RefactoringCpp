// ���� RefactoringManager.cpp - ���������� ������� ������ RefactoringManager

#include "RefactoringManager.h"

/// <summary>����������� ������� ������ RefactoringManager</summary>
/// <param name="filePath">- ���� � ����� � �������� �����</param>
RefactoringManager::RefactoringManager(string filePath) {
    this->filePath = filePath;
    this->outFilePath = this->GetOutputFileName(this->filePath);
    cout << this->outFilePath << endl;
}

/// <summary>���������� ������������</summary>
void RefactoringManager::Refactoring() {
    this->GetFileContent(this->filePath);
    this->code = CppCode(this->fileContent);
    this->RemoveAssignmentsToParametersRefactoring(this->code);
    this->RemoveParametersRefactoring(this->code);
    this->fileContent = this->code.ToString();
    WriteFile(this->outFilePath);
}

/// <summary>���������� ������������ ������� �������� ������������ ����������</summary>
/// <param name="code">- �������� ��� ���������</param>
void RefactoringManager::RemoveAssignmentsToParametersRefactoring(CppCode code) {
    vector<CppMethod> methods = code.GetMethods();
    for (int i = 0; i < methods.size(); i++) {
        methods[i] = this->RemoveAssignmentsToParameters(methods[i]);
    }
    code.UpdateMethods(methods);
    code.UpdateLexemes();
    this->code = code;
}

/// <summary>�������� ������������ ���������� ������</summary>
/// <param name="method">- ����������� �����</param>
/// <returns>����� �����</returns>
CppMethod RefactoringManager::RemoveAssignmentsToParameters(CppMethod method) {
    CppMethod newCppMethod,
        var_method = method;
    // ���� �� ���� ����������
    for (int i = 0; i < method.parameters.size(); i++) {
        newCppMethod.body = {};
        newCppMethod.parameters = method.parameters;
        bool assignment = false;
        // �������� ���� ������ �� ������� ������������ ��������� � ���
        for (int j = 0; j < var_method.body.size(); j++) {
            // ���� ���� ������������
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
        // ���� ���� ������������
        if (assignment) {
            int start = -1;
            for (int j = 0; j < var_method.body.size(); j++) {
                // �������� ��� �� ����� ����������, ��������� ��������� ���� ���������� ��� ����
                if (var_method.body[j] == var_method.parameters[i].name)
                    newCppMethod.body.push_back("var_" + var_method.body[j]);
                else
                    newCppMethod.body.push_back(var_method.body[j]);
                // � ������ ���� ������ ��������� ����� ����������, ������� ����� �������� �������� ���������
                if ((var_method.body[j] == "{") && (start == -1)) {
                    newCppMethod.body.push_back(var_method.parameters[i].type);
                    newCppMethod.body.push_back("var_" + var_method.parameters[i].name);
                    newCppMethod.body.push_back("=");
                    newCppMethod.body.push_back(var_method.parameters[i].name);
                    newCppMethod.body.push_back(";");
                    start = j;
                }
            }
            // �������� ����� ����� ��������� ���������
            var_method = newCppMethod;
        }
    }
    return var_method;
}

/// <summary>���������� ������������ ������� �������� ����������</summary>
/// <param name="code">- �������� ��� ���������</param>
void RefactoringManager::RemoveParametersRefactoring(CppCode code) {
    vector<CppMethod> methods = code.GetMethods();
    for (int i = 0; i < methods.size(); i++) {
        methods[i] = this->RemoveParameters(methods[i]);
    }
    code.UpdateMethods(methods);
    code.UpdateLexemes();
    this->code = code;
}

/// <summary>�������� ���������� ������</summary>
/// <param name="method">- ����������� �����</param>
/// <returns>����� �����</returns>
CppMethod RefactoringManager::RemoveParameters(CppMethod method) {
    CppMethod newCppMethod,
        var_method = method;
    // ���� �� ���� ����������
    for (int i = 0; i < method.parameters.size(); i++) {
        newCppMethod.body = method.body;
        newCppMethod.parameters = {};
        bool usingParameter = false;
        // ���������, ������������ �� �������� � ���� ������
        for (int j = 0; j < var_method.body.size(); j++) {
            // ���� �������� ������������
            if (var_method.body[j] == var_method.parameters[i].name) {
                usingParameter = true;
                break;
            }
        }
        // ���� ������������, �� ��������� ��� � ������ ���������� ������ ������
        if (usingParameter) {
            newCppMethod.parameters.push_back(var_method.parameters[i]);
            // �������� ����� ����� ��������� ���������
            var_method = newCppMethod;
        }
    }
    return var_method;
}

/// <summary>����������� �������� �����, � ������� ����� ����������� ���������� ���</summary>
/// <param name="filePath">- ������ �������� ��������� �����</param>
/// <returns>�������� �����, � ������� ����� ����������� ���������� ���</returns>
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

/// <summary>������ �����</summary>
/// <param name="filePath">- ������ �������� �����</param>
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

/// <summary>������ � ����</summary>
/// <param name="filePath">- ������ �������� �����</param>
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