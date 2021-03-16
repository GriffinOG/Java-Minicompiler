/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LexicalAnalyzer.h
 * Author: OMEN X
 *
 * Created on February 21, 2020, 8:21 AM
 */

#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H
#include<string.h>
#include <vector>
#include <string>

using namespace std;

class LexicalAnalyzer {
public:
    LexicalAnalyzer();
    LexicalAnalyzer(const LexicalAnalyzer& orig);
    virtual ~LexicalAnalyzer();
    
    void iterator(FILE *f1, char filename[]);
    void check_key(string p);
    vector<pair<string,string>> element_list;
    
private:
    
    string keywords[19]={"boolean","class","do",
    "double","else","float","for","if","import",
    "int","new","package","public","return",
    "static","void","while","main","string"};

    int i=0,count_identifier=0,count_keyword=0,count_number=0,count_operator=0,count_separators=0,l=0,m=0;
    int opening_brackets_no=0,closing_brackets_no=0,opening_braces_no=0,closing_braces_no=0;
    bool stmt_found=false, class_dec_found=false, for_loop_found=false, open_parenthesis_found=false;
    char* class_reqs[2][5], for_loop_reqs[13][5];
    
    char chr,
            separators[25]={'\n','\t',';',':','(',')','{','}','[',']','#','"','<','>',' ','\r'},
            oprators[20]={'!','%','^','&','*','-','+','=','~','|','.','<','>','/','?'};
//            accepted_chars[62]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
//    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
//    '1','2','3','4','5','6','7','8','9','0'};
    
    bool accepted_char_found = false;
    string check_str;
    int element_list_index = 0;
    bool at_first_element = true; 
};

#endif /* LEXICALANALYZER_H */

