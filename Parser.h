/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Parser.h
 * Author: OMEN X
 *
 * Created on February 22, 2020, 2:27 PM
 */

#ifndef PARSER_H
#define PARSER_H
#include<string.h>
#include <vector>
#include <string>
#include "GlobalVariables.h"

using namespace std;

class Parser {
public:
    Parser(vector<pair<string,string>>);
    Parser(const Parser& orig);
    virtual ~Parser();
    
    void list_iterator(vector<pair<string,string>> my_element_list);
    vector<g_functional_constructs> functional_order;
    
private:
    vector<pair<string,string>> element_list;
    vector<pair<string,string>> element_buffer;
    vector<pair<string,string>> stmt_buffer;
    
    
        
    int list_index = 0;
    int level = 0;
    int identifier_count = 0;
    int stmt_index = 0;
    
    string tabs = "";
    string pkg_start = "package";
    string import_start = "import";
    string class_start = "class";
    string main_func_start = "main";
    string IDENTIFIER = "IDENTIFIER";
    string KEYWORD = "KEYWORD";
    string NUMERIC_LITERAL = "NUMERIC LITERAL";
    string types[5] = {"int","double","long","boolean","string"};
    string ctrl_types[3] = {"if","for","while"};
    
    string stmt_end;
    string name_sep;
    string opening_brace;
    string closing_brace;
    string opening_bracket;
    string closing_bracket;
    string equ_sign;
    string plus, minus, div, mul, mod;
    string less_than, greater_than;
    
    string math_ops[5] = {plus, minus, div, mul, mod};
    string rltn_ops[2] = {less_than, greater_than};
    
    bool pkg_name_found = false;
    bool class_name_found = false;
    bool name_sep_found = false;
    bool opening_brace_found = false;
    bool closing_brace_found = false;
    bool opening_bracket_found = false;
    bool closing_bracket_found = false;
    bool stmt_end_found = false;
    bool identifier_found = false;
    bool op_found = false;
    bool first_stmt_parser_call = true;
    
    void packageDecParser(vector<pair<string,string>> element_buffer);
    void packageNameParser(pair<string,string> element);
    void importDecsParser(vector<pair<string,string>> element_buffer);
    void importDecParser(vector<pair<string,string>> element_buffer);
    void classDecParser(vector<pair<string,string>> element_buffer);
    void classNameParser(pair<string,string> element);
    void classBodyParser(vector<pair<string,string>> element_buffer);
    void statementsParser(vector<pair<string,string>> element_buffer);
    void statementParser(vector<pair<string,string>> element_buffer);
    void mainFunctionParser(vector<pair<string,string>> element_buffer);
    void functionDecParser(vector<pair<string,string>> element_buffer);
    void declarationsParser(vector<pair<string,string>> element_buffer);
    void declarationParser();
    void expressionsParser(vector<pair<string,string>> element_buffer);
    void expressionParser(bool inForLoop);
    void controlFlowParser();
    void typeParser(vector<pair<string,string>> element_buffer);
    void initializationParser(vector<pair<string,string>> element_buffer);
    void functioncallParser(vector<pair<string,string>> element_buffer);
    void paramsParser(vector<pair<string,string>> element_buffer);
    void functionNameParser(pair<string,string> element);
    void ifParser();
    void forParser();
    void whileParser();
    void relationParser(bool inForLoop);
    void funcPrototypeParser(vector<pair<string,string>> element_buffer);
    void argumentsParser(vector<pair<string,string>> element_buffer);
    void argumentParser(vector<pair<string,string>> element_buffer);
    void operatorParser(vector<pair<string,string>> element_buffer);
    void inrementParser();
    
    char tab = '\t';
    
    void tabsCheck();

};

#endif /* PARSER_H */

