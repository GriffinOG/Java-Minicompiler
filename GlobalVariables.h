/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GlobalVariables.h
 * Author: OMEN X
 *
 * Created on April 14, 2020, 10:31 AM
 */

#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include <vector>
#include <string>

using namespace std;

    enum g_functional_constructs {pkgDec, importDec, classDec, mainFuncDec, decStmt, assignStmt, arithSmt, ifStmt, whileStmt, forStmt};
    extern vector<pair<string,string>> g_element_list;
    
//    virtual ~GlobalVariables() = 0;


#endif /* GLOBALVARIABLES_H */

