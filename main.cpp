/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: OMEN X
 *
 * Created on February 21, 2020, 8:20 AM
 */

#include <cstdlib>
#include <iostream>
#include "LexicalAnalyzer.h"
#include "Parser.h"
#include "IntCodeGen.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    char filename[100] = "C:\\Users\\OMEN X\\Documents\\NetBeansProjects\\minijavaSample\\src\\minijavasample\\MinijavaSample.java";
    
    FILE *f1;
    
    f1 = fopen(filename,"r");
    if(f1==NULL)
    {
        cout<<"OOPS! File not found...! ";
        exit(0);
    }

    cout<<"******************************************\n";
    
    LexicalAnalyzer lexicalAnalyzer;
    lexicalAnalyzer.iterator(f1,filename);
    
    Parser parser(lexicalAnalyzer.element_list);
       
    IntCodeGen intCodeGen(parser.functional_order, lexicalAnalyzer.element_list);

    return 0;
}

