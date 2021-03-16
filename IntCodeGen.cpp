#include "IntCodeGen.h"
#include "GlobalVariables.h"
#include<iostream>
#include<ctype.h>
#include<stdlib.h>
#include<windows.h>
#include <algorithm>
#include <iterator>

using namespace std;

IntCodeGen::IntCodeGen(vector<g_functional_constructs> my_functional_order, vector<pair<string,string>> my_element_list): functional_order(my_functional_order), 
        element_list(my_element_list) {
    listIterator(true);
}

IntCodeGen::IntCodeGen(const IntCodeGen& orig) {
}

IntCodeGen::~IntCodeGen() {
}

void IntCodeGen::listIterator(bool initialCall){
    
    if(initialCall){
        cout<<"\n__________________________________________________\n";
        cout<<"\n******************Intermediate Code Generation begins******************\n\n";
    }
    
    for (order_index; order_index < functional_order.size(); order_index++) {
        switch(functional_order[order_index]){
            case pkgDec:
                pkgDecGen();
                break;
                
            case importDec:
                importDecGen();
                break;
                
            case classDec:
                classDecGen();
                break;
                
            case mainFuncDec:
                mainFuncDecGen();
                break;
                
            case decStmt:
                decStmtGen();
                break;
                
            case assignStmt:
                assignStmtGen();
                break;
                
            case arithSmt:
                arithStmtGen();
                break;
                
            case ifStmt:
                ifStmtGen();
                break;
                
            case forStmt:
                forStmtGen();
                break;
                
            case whileStmt:
                whileStmtGen();
                break;
                
        }        
    }

}

void IntCodeGen::pkgDecGen() {
    cout<<"\\\\Package declaration statement"<<endl;
    list_index = 3;
}

void IntCodeGen::importDecGen() {
    cout<<"\\\\Import declaration statement"<<endl;
    list_index = list_index + 5;
}

void IntCodeGen::classDecGen() {
    cout<<"\\\\Class declaration statement"<<endl;
    list_index = list_index + 3;
}

void IntCodeGen::mainFuncDecGen() {
    cout<<"\\\\Main function declaration statement"<<endl;
    list_index = list_index + 4;
}

void IntCodeGen::decStmtGen() {
    cout<<"\\\\Variable declaration statement"<<endl;
    list_index++;
    string id = element_list[list_index].second;
    vars.push_back(id);
    list_index = list_index + 2;
}

void IntCodeGen::assignStmtGen() {
    string id = element_list[list_index].second;
    list_index = list_index + 2;
    string value = element_list[list_index].second;
    cout<<"ASSIGN, "<<id<<" ,"<<value;
    cout<<"\t\\\\"<<id<<"="<<value<<endl;
    list_index = list_index + 2;
}

void IntCodeGen::arithStmtGen() {
    string op = element_list[list_index + 1].second;
    string op_string, op1, op2;
    op1 = element_list[list_index].second;
    op2 = element_list[list_index + 2].second;
   
    if(op.compare("+") == 0){
        op_string = "ADD";
    } else if(op.compare("-") == 0){
        op_string = "SUB";
    } else if(op.compare("*") == 0){
        op_string = "MUL";
    } else if(op.compare("//") == 0){
        op_string = "DIV";
    } else if(op.compare("%") == 0){
        op_string = "MOD";
    }
    
    cout<<op_string<<", t"<<++temp_reach<<", "<<op1<<", "<<op2;
    cout<<"\t\\\\t"<<temp_reach<<"="<<op1<<op<<op2<<endl;
    cout<<"ASSIGN, "<<op1<<", t"<<temp_reach;
    cout<<"\t\\\\"<<op1<<"="<<temp_reach<<endl;
    temp_reach--;
    list_index = list_index + 4;
}

void IntCodeGen::ifStmtGen() {
    cout<<"\nIf statement"<<endl;
    int limit_index = list_index + 4;
    string limit = element_list[limit_index].second;
    
    if(!(find(vars.begin(), vars.end(), limit) != vars.end())) {
        cout<<"ASSIGN, t"<<++temp_reach<<", "<<limit;
        cout<<"\t\\\\t"<<temp_reach<<"="<<limit<<endl;
        limit = "t"+to_string(temp_reach);
    }
    
    list_index = list_index + 2;
    comparisonGen(limit);
    
    if(element_list[list_index].second.compare("{") == 0){
        ++list_index;
        while(!(element_list[list_index].second.compare("}") == 0)){
            ++order_index;
            listIterator(false);
            ++list_index;
        }
    }
    
    cout<<"Label, L"<<label_reach;
    cout<<"\t\\\\L"<<label_reach<<":"<<endl;
}

void IntCodeGen::forStmtGen() {
    cout<<"\nFor loop"<<endl;
    int label = ++label_reach;
    cout<<"Label, L"<<label;
    cout<<"\t\\\\L"<<label<<":"<<endl;
    int counter_index = list_index + 2;
    string counter = element_list[counter_index].second;
    int limit_index = list_index + 8;
    string limit = element_list[limit_index].second;
    
    if(!(find(vars.begin(), vars.end(), limit) != vars.end())) {
        cout<<"ASSIGN, t"<<++temp_reach<<" , "<<limit;
        cout<<"\t\\\\t"<<temp_reach<<"="<<limit<<endl;
        limit = "t"+temp_reach;
    }
    
    list_index = list_index + 2;
    assignStmtGen();
    comparisonGen(limit);
    
    list_index = list_index + 4;
    
    if(element_list[list_index].second.compare("{") == 0){
        ++list_index;
        while(!(element_list[list_index].second.compare("}") == 0)){
            ++order_index;
            listIterator(false);
            ++list_index;
        }
    }
    
    cout<<"\nADD, t"<<++temp_reach<<", "<<counter<<", 1";
    cout<<"\t\\\\t"<<temp_reach<<"="<<counter<<"+1"<<endl;
    cout<<"ASSIGN, "<<counter<<", t"<<temp_reach;
    cout<<"\t\\\\"<<counter<<"=t"<<temp_reach<<endl;
    cout<<"GOTO, L"<<label;
    cout<<"\t\\\\goto L"<<label<<endl;
    cout<<"Label, L"<<++label;
    cout<<"\t\\\\L"<<label<<":"<<endl;
}

void IntCodeGen::whileStmtGen() {
    
}

void IntCodeGen::comparisonGen(string limit) {
    string comp;
    string op = element_list[list_index + 1].second;
    if(op.compare("<") == 0){
        comp = "LT";
    } else {
        comp = "GT";
    }
    
    cout<<comp<<", t"<<++temp_reach<<", "<<element_list[list_index].second<<", "<<limit;
    cout<<"\t\\\\t"<<temp_reach<<"="<<element_list[list_index].second<<op<<limit<<endl;
    cout<<"NOT, t"<<++temp_reach<<", t"<<temp_reach - 1;
    cout<<"\t\\\\t"<<temp_reach<<"=!t"<<temp_reach - 1<<endl;
    cout<<"IF, t"<<temp_reach<<", L"<<++label_reach;
    cout<<"\t\\\\if(t"<<temp_reach<<")goto L"<<label_reach<<endl;
    list_index = list_index + 4;
}