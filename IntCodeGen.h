/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IntCodeGen.h
 * Author: OMEN X
 *
 * Created on April 13, 2020, 10:32 AM
 */

#ifndef INTCODEGEN_H
#define INTCODEGEN_H
#include <vector>
#include <string>
#include <variant>
#include "GlobalVariables.h"

using namespace std;

class IntCodeGen {
public:
    IntCodeGen(vector<g_functional_constructs>, vector<pair<string,string>>);
    IntCodeGen(const IntCodeGen& orig);
    virtual ~IntCodeGen();
    
    void listIterator(bool initialCall);
    
private:
    int order_index = 0;
    int list_index = 0;
    
    int temp_reach = 0, label_reach = 0;
    
    vector<string> vars;
    
    vector<pair<string,string>> element_list;
    vector<g_functional_constructs> functional_order;
    vector<pair<string,string>> registers {make_pair("$t0",""),make_pair("$t1",""),make_pair("$t2",""),
            make_pair("$t3",""),make_pair("$t4",""),make_pair("$t5",""),make_pair("$t6",""),make_pair("$t7","")};
    
    void pkgDecGen();
    void importDecGen();
    void classDecGen();
    void mainFuncDecGen();
    void decStmtGen();
    void assignStmtGen();
    void arithStmtGen();
    void ifStmtGen();
    void whileStmtGen();
    void forStmtGen();
    void comparisonGen(string limit);
    
    void microIterator();
    
    int getReg();
    int checkReg(string x);
};

#endif /* INTCODEGEN_H */

