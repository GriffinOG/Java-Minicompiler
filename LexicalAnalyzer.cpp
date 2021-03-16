/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LexicalAnalyzer.cpp
 * Author: OMEN X
 * 
 * Created on February 21, 2020, 8:21 AM
 */

#include "LexicalAnalyzer.h"
#include<iostream>
#include<ctype.h>
#include<stdlib.h>
#include<windows.h>
#include <algorithm>
#include <iterator>

LexicalAnalyzer::LexicalAnalyzer() {
}

LexicalAnalyzer::LexicalAnalyzer(const LexicalAnalyzer& orig) {
}

LexicalAnalyzer::~LexicalAnalyzer() {
}

void LexicalAnalyzer::check_key(string p){
    int k,flag=0;
    for(k=0;k<=18;k++)
    {
        if(keywords[k].compare(p)==0)
        {
            if(stmt_found == true){
                cout<<"Error on line "<<l<<". ';' expected.\n";
                stmt_found = false;
            }
            
            cout<<p<<" :- KEYWORD "<<endl; //for checking if given character is keyword...
            element_list.push_back(pair<string, string>("KEYWORD", p));
            if(p.compare("boolean")==0 || p.compare("int")==0 || p.compare("string")==0){
                stmt_found = true;            
            }
            
            if(p.compare("class")==0){
                class_dec_found = true;
            }
            
            Sleep(50);
            count_keyword++;
            flag=1;
            break;
        }
    }
    if(flag==0)
    {
        if(isdigit(p[0]))
        {
            cout<<p<<" :- NUMERIC LITERAL "<<endl; //for checking if given character is Numeric Literal...
            element_list.push_back(pair<string, string>("NUMERIC LITERAL", p));
            Sleep(50);
            count_number++;
        }
        else
        {
            if(p[0]!='\0')
            {
                cout<<p<<" :- IDENTIFIER "<<endl; //for checking if given character is identifier...
                element_list.push_back(pair<string, string>("IDENTIFIER", p));
                if(class_dec_found == true){
                    class_reqs[0][0] = (char*)""+'i';
                }
                Sleep(50);
                count_identifier++;
            }
        }
    }
    
////    element_list[m] = p;
//    cout<< p<< ": this is my_christ[m] = p;";
    check_str.clear();
    
//    if(!p.empty()){
//        element_list.push_back(my_key);
//    }
    
    m++;
    i=-1;
}

void LexicalAnalyzer::iterator(FILE* f1, char filename[]){
    
    int j;
    f1 = fopen(filename,"r");
    if(f1==NULL)
    {
        cout<<"OOPS! File not found...! ";
        exit(0);
    }

    cout<<"******************************************\n";
    
    while((chr=fgetc(f1))!=EOF)
    {
        string my_chr;
        my_chr.append(1,chr);
        
        for(j=0;j<=20;j++)
        {
            if(chr==oprators[j]) //for checking if given character is operator...
            {
                cout<<chr<<" :- OPERATOR \n";
//                element_list[n] = my_chr;
//                element_list.push_back(my_chr);
                Sleep(50);
                count_operator++;
                check_key(check_str);
                element_list.push_back(pair<string, string>("OPERATOR", my_chr));
            }
        }
        for(j=0;j<=15;j++)
        {
            if(i==-1)
                break;
            if(chr==separators[j]) //for checking if given character is separator...
            {
                if(chr==';'||chr==','||chr=='<'||chr=='>'||chr=='{'||chr=='}'||chr=='('||chr==')'|| chr==':')
                {
                    cout<<chr<<" :- SEPARATOR\n";
//                    element_list.push_back(pair<string, string>("SEPARATOR", my_chr));
                    int n = m+1;
//                    element_list[n] = my_chr;
//                    element_list.push_back(my_chr);
                    count_separators++;
                    Sleep(50);
                } 
                
                if(chr == '{' && class_dec_found == true){
//                    class_reqs[1][0]=my_chr;
                    class_dec_found = false;
                }
                
                if(class_reqs[1][0] != (char*)""+'{' && class_dec_found){
                    cout<<"Error. \'{\' expected on line "<<l+1<<".\n";
                    class_dec_found = false;
                }
                
                if(class_reqs[0][0] != (char*)""+'i' && class_dec_found){
                    cout<<"Error. Identifier expected on line "<<l+1<<".\n";
                    class_dec_found = false;
                }
                
                if(chr == '('){
                    open_parenthesis_found = true;
                }
                
                if(chr == '{')
                    opening_braces_no++;
                
                if(chr == '}')
                    closing_braces_no++;
                
                if(stmt_found==1 && chr==';'){
                    stmt_found = false;
                }
                            
                if(chr=='"')
                {
                    string temp;
                    do
                    {
                        chr=fgetc(f1);
                        temp.push_back(chr);
                        printf("%c",chr);
                    }while(chr!='"');
                    cout<<"\b"<<" :- STRING \n"; //for checking if given character string...
                    element_list.push_back(pair<string, string>("STRING", temp));

                    int n = m+1;
//                    element_list[n] = my_chr;
//                    element_list.push_back(temp);
                    Sleep(50);
                    i=-1;
                    break;
                }
                
                
                check_key(check_str);
                if(chr==';'||chr==','||chr=='<'||chr=='>'||chr=='{'||chr=='}'||chr=='('||chr==')'|| chr==':')
                {
                    element_list.push_back(pair<string, string>("SEPARATOR", my_chr));
                }
                
                if (chr == '\r') {
                    ++l;
                }
            }
        }
        if(i!=-1)
        {
            check_str.push_back(chr);
            i++;
        }
        else
            i=0;
    }

    if(opening_braces_no > closing_braces_no){
        cout<<"Error on line "<<l<<". Closing brace expected.\n";
    }
    

	//various statistics of the program
	
    cout<<"______________________________________________________\n";
    cout<<"\nOverall Statistics of given JAVA program.\a\n";
    cout<<"\n\n*********\nNo. of Keywords in JAVA file are: "<<count_keyword;
    cout<<"\nNo. of Identifiers in JAVA file are: "<<count_identifier;
    cout<<"\nNo. of Operators in JAVA file are: "<<count_operator;
    cout<<"\nNo. of Numeric literals in JAVA file are: "<<count_number;
    cout<<"\nNo. of Separators in JAVA file are: "<<count_separators;
    cout<<"\n*********\n\n\n";
    
    cout<<"\n*********\n";
//    for(pair<string,string> x: element_list){
////        iter_swap(element_list.end() - 1, element_list.end() - 2);
//        
//	cout<<x.first<<" :- "<<x.second<<endl;
//    }
    
}
