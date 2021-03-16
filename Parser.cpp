#include "Parser.h"
#include<iostream>
#include<ctype.h>
#include<stdlib.h>
#include<windows.h>
#include <algorithm>
#include <iterator>
#include "GlobalVariables.h"

using namespace std;

Parser::Parser(vector<pair<string,string>> my_element_list): element_list(my_element_list) {
    name_sep = name_sep.append(1,'.');
    stmt_end = stmt_end.append(1,';');
    opening_brace = opening_brace.append(1,'{');
    closing_brace = closing_brace.append(1,'}');
    opening_bracket = opening_bracket.append(1,'(');
    closing_bracket = closing_bracket.append(1,')');
    equ_sign = equ_sign.append(1,'=');
    plus = plus.append(1,'+');
    minus = minus.append(1,'-');
    div = div.append(1,'/');
    mul = mul.append(1,'*');
    mod = mod.append(1,'%');
    less_than = less_than.append(1,'<');
    greater_than = greater_than.append(1,'>');
    list_iterator(element_list);
}

Parser::Parser(const Parser& orig) {
}

Parser::~Parser() {
}

void Parser::list_iterator(vector<pair<string, string> > element_list) {
    cout<<"\n__________________________________________________\n";
    cout<<"\n******************Parsing begins******************\n\n";
    if(element_list[list_index].second.compare(pkg_start)==0){
        element_buffer.clear();
        do{
           element_buffer.push_back(element_list[list_index]);
           list_index++;
        } while (!(element_list[list_index].second.compare(class_start) == 0 || element_list[list_index].second.compare(import_start) == 0));
        packageDecParser(element_buffer);
    } else{
        cout<<"Error: Package declaration expected column 0 which contains "<<element_list[0].second<<endl;
    }
    
    if(element_list[list_index].second.compare(import_start)==0){
        element_buffer.clear();
        do{
           element_buffer.push_back(element_list[list_index]);
           list_index++;
        } while (element_list[list_index].second.compare(class_start) != 0);
        importDecsParser(element_buffer);
    }
    
    if(element_list[list_index].second.compare(class_start)==0){
        element_buffer.clear();
        do{
           element_buffer.push_back(element_list[list_index]);
           list_index++;
        } while (list_index != element_list.size());
        classDecParser(element_buffer);
    } else{
        cout<<"Error: Class declaration expected at column "<<list_index<<" which contains "<<element_list[list_index].second<<endl;
    }
}

void Parser::packageDecParser(vector<pair<string,string> > element_buffer){
    int local_index = 1;
    
    cout<<"Package declaration found!"<<endl;
    functional_order.push_back(pkgDec);
    
    cout<<element_buffer[0].first<<" :- "<<element_buffer[0].second<<endl;
    
    if(element_buffer.size() < 3){
        element_buffer.resize(3);
    }
    
    ++level;
    packageNameParser(element_buffer[local_index]);
    
    if(pkg_name_found){
        local_index++;
        pkg_name_found = false;
    }
    
    if(element_buffer[local_index].second.compare(stmt_end)==0){
        cout<<element_buffer[local_index].first<<" :- "<<element_buffer[local_index].second<<endl;
    } else {
        cout<<"Error: \';\' expected at package declaration column "<<local_index<<" which contains "<<element_buffer[local_index].second<<endl;
    }
    
    if(element_buffer.size() > 3){
        cout<<"Error: Unexpected tokens after package declaration column 2"<<endl;
    }
    
}

void Parser::packageNameParser(pair<string,string> element){
    tabsCheck();
    if(element.first.compare("IDENTIFIER")==0){
            pkg_name_found = true;
            cout<<tabs<<"Package name found!"<<endl;
            tabs.append(1,'\t');
            cout<<tabs<<element.first<<" :- "<<element.second<<endl;
        } else {
            cout<<tabs<<"Error: Package name expected at package name column 0 which contains "<<element.second<<endl;
        }
//    --level;
    tabsCheck();
}

void Parser::importDecsParser(vector<pair<string,string> > element_buffer){
    int local_index = 0;
    vector<pair<string,string> > element_buffer1;
    
    tabsCheck();
    cout<<tabs<<"Import declarations found!"<<endl;
    tabs.append(1,'\t');    
    element_buffer1.push_back(element_buffer[local_index]);
    
    for(++local_index; local_index < element_buffer.size(); local_index++){  
        if(element_buffer[local_index].second.compare(import_start) != 0){
            element_buffer1.push_back(element_buffer[local_index]);
        } else{
            break;
        }
    }
    
    ++level;
    importDecParser(element_buffer1);

    element_buffer.erase(element_buffer.begin(), element_buffer.begin() + local_index);

    if(element_buffer.size() > 0){
       if(element_buffer[0].second.compare(import_start)==0){
//            ++level;
            importDecsParser(element_buffer);
        } 
    }
    --level;
}

void Parser::importDecParser(vector<pair<string,string> > element_buffer){
    int local_index = 0;
    tabsCheck();
    
    cout<<tabs<<"Import declaration found!"<<endl;
    functional_order.push_back(importDec);
    tabs.append(1,'\t');
    
    cout<<tabs<<element_buffer[0].first<<" :- "<<element_buffer[0].second<<endl;
    
    if(element_buffer.size() < 5){
        element_buffer.resize(5);
    }
    
    local_index++;
    
    ++level;
    packageNameParser(element_buffer[local_index]);
    
    if(pkg_name_found){
        local_index++;
        pkg_name_found = false;
    }
    
    if(element_buffer[local_index].second.compare(name_sep)==0){
        cout<<tabs<<element_buffer[local_index].first<<" :- "<<element_buffer[local_index].second<<endl;
        name_sep_found = true;
    } else {
        cout<<tabs<<"Error: \'.\' expected at import declaration column "<<local_index<<" which contains "<<element_buffer[local_index].second<<endl;
    }
    
    if(name_sep_found){
        local_index++;
        name_sep_found = false;
    }
    
    classNameParser(element_buffer[local_index]);
    
    if(class_name_found){
        local_index++;
        class_name_found = false;
    }
    
    if(element_buffer[local_index].second.compare(stmt_end)==0){
        cout<<tabs<<element_buffer[local_index].first<<" :- "<<element_buffer[local_index].second<<endl;
    } else {
        cout<<tabs<<"Error: \';\' expected at package declaration column "<<local_index<<" which contains "<<element_buffer[local_index].second<<endl;
    }
    
    if(element_buffer.size() > 5){
        cout<<tabs<<"Error: Unexpected tokens after import declaration column 4"<<endl;
    }
    --level;
}

void Parser::classDecParser(vector<pair<string,string> > element_buffer){
    int local_index = 1;
    vector<pair<string,string> > element_buffer1;
    
    cout<<"Class declaration found!"<<endl;
    functional_order.push_back(classDec);
    
    cout<<element_buffer[0].first<<" :- "<<element_buffer[0].second<<endl;
    
    if(element_buffer.size() < 5){
        element_buffer.resize(5);
    }
    
    classNameParser(element_buffer[local_index]);
    
    if(class_name_found){
        local_index++;
        class_name_found = false;
    }
    
    if(element_buffer[local_index].second.compare(opening_brace)==0){
        cout<<element_buffer[local_index].first<<" :- "<<element_buffer[local_index].second<<endl;
        opening_brace_found = true;
    } else {
        cout<<"Error: \'{\' expected at class declaration column "<<local_index<<" which contains "<<element_buffer[local_index].second<<endl;
    }
    
    if(opening_brace_found){
        local_index++;
        opening_brace_found = false;
    }
    
    do{
        element_buffer1.push_back(element_buffer[local_index]);
        local_index++;
    } while (local_index != element_buffer.size() - 1);
//    element_buffer1.pop_back();
    
    ++level;
    classBodyParser(element_buffer1);
    
    if(element_buffer[local_index].second.compare(closing_brace)==0){
        cout<<element_buffer[local_index].first<<" :- "<<element_buffer[local_index].second<<endl;
    } else {
        cout<<"Error: \'}\' expected at class declaration column "<<local_index<<" which contains "<<element_buffer[local_index].second<<endl;
    }
    
}


void Parser::classNameParser(pair<string,string> element){
    
    tabsCheck();
    if(element.first.compare("IDENTIFIER")==0){
            class_name_found = true;
            cout<<tabs<<"Class name found!"<<endl;
            tabs.append(1,'\t');
            cout<<tabs<<element.first<<" :- "<<element.second<<endl;
        } else {
            cout<<tabs<<"Error: Class name expected at class name column 0 which contains "<<element.second<<endl;
        }
//    --level;
    tabsCheck();
}

void Parser::classBodyParser(vector<pair<string,string>> element_buffer){
    int local_index = 0;
    vector<pair<string,string> > element_buffer1;
    
    if(element_buffer[local_index].second.compare(main_func_start)==0){
        do{
           element_buffer1.push_back(element_buffer[local_index]);
           local_index++;
        } while (local_index != element_buffer.size());
        mainFunctionParser(element_buffer1);
    } else{
        cout<<"Error: Main function expected at column "<<local_index<<" which contains "<<element_buffer[local_index].second<<endl;
    }
};

void Parser::statementsParser(vector<pair<string,string>> element_buffer){
    bool stmt_found = false;
    if(first_stmt_parser_call){
        stmt_buffer = element_buffer;
    }
    first_stmt_parser_call = false;
    
    tabsCheck();
    for(int i = 0; i <= 4; i++){
        if(stmt_buffer[stmt_index].second.compare(types[i])==0){
            cout<<tabs<<"Statements found!"<<endl;
            ++level;
            declarationParser();
            stmt_found = true;
            break;
        }
    }
    
    if (!stmt_found) {
        if(stmt_buffer[stmt_index].first.compare(IDENTIFIER)==0){
            cout<<tabs<<"Statements found!"<<endl;
            ++level;
            expressionParser(false);
            stmt_found = true;
        }
    }

    if(!stmt_found){
        for(int i = 0; i <= 2; i++){
            if(stmt_buffer[stmt_index].second.compare(ctrl_types[i])==0){
                cout<<tabs<<"Statements found!"<<endl;
                ++level;
                controlFlowParser();
                stmt_found = true;
                break;
            }
        }
    }
    
    
        --level;
}

void Parser::mainFunctionParser(vector<pair<string,string>> element_buffer){
    int local_index = 1;
    vector<pair<string,string> > element_buffer1;
    
    tabsCheck();
    cout<<tabs<<"Main function found!"<<endl;
    functional_order.push_back(mainFuncDec);
    
    tabs.append(1,'\t');
    cout<<tabs<<element_buffer[0].first<<" :- "<<element_buffer[0].second<<endl;
    
    if(element_buffer.size() < 5){
        element_buffer.resize(5);
    }
    
    if(element_buffer[local_index].second.compare(opening_bracket)==0){
        cout<<tabs<<element_buffer[local_index].first<<" :- "<<element_buffer[local_index].second<<endl;
        opening_bracket_found = true;
    } else {
        cout<<tabs<<"Error: \'(\' expected at package declaration column "<<local_index<<" which contains "<<element_buffer[local_index].second<<endl;
    }
    
    if(opening_bracket_found){
        local_index++;
        opening_bracket_found = false;
    }
    
    if(element_buffer[local_index].second.compare(closing_bracket)==0){
        cout<<tabs<<element_buffer[local_index].first<<" :- "<<element_buffer[local_index].second<<endl;
        closing_bracket_found = true;
    } else {
        cout<<"Error: \')\' expected at package declaration column "<<local_index<<" which contains "<<element_buffer[local_index].second<<endl;
    }
    
    if(closing_bracket_found){
        local_index++;
        closing_bracket_found = false;
    }
    
    if(element_buffer[local_index].second.compare(opening_brace)==0){
        cout<<tabs<<element_buffer[local_index].first<<" :- "<<element_buffer[local_index].second<<endl;
        opening_brace_found = true;
    } else {
        cout<<"Error: \'{\' expected at package declaration column "<<local_index<<" which contains "<<element_buffer[local_index].second<<endl;
    }
    
    if(opening_brace_found){
        local_index++;
        opening_brace_found = false;
    }
    
    do{
        element_buffer1.push_back(element_buffer[local_index]);
        local_index++;
    } while (local_index != element_buffer.size() - 1);
    
    ++level;
    statementsParser(element_buffer1);
    
    tabsCheck();
    if(element_buffer[local_index].second.compare(closing_brace)==0){
        cout<<tabs<<element_buffer[local_index].first<<" :- "<<element_buffer[local_index].second<<endl;
    } else {
        cout<<tabs<<"Error: \'}\' expected at main function column "<<local_index<<" which contains "<<element_buffer[local_index].second<<endl;
    }
}

void Parser::functionDecParser(vector<pair<string,string>> element_buffer){

}

void Parser::declarationParser(){
    
    tabsCheck();
    cout<<tabs<<"Declaration found!"<<endl;
    functional_order.push_back(decStmt);
    tabs.append(1,'\t');
    
    cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
    stmt_index++;
    
    if(stmt_buffer[stmt_index].first.compare(IDENTIFIER)==0){
        cout<<tabs<<element_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        identifier_found = true;
    } else {
        cout<<tabs<<"Error: Identifier expected at declaration column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    if(identifier_found){
        stmt_index++;
        identifier_found = false;
    }
    
    if(stmt_buffer[stmt_index].second.compare(stmt_end)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_end_found = true;
    } else {
        cout<<tabs<<"Error: \';\' expected at declaration column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    if(stmt_end_found){
        stmt_index++;
        stmt_end_found = false;
    }
    
    if(stmt_index != stmt_buffer.size()){
        statementsParser(stmt_buffer);
    }
};

void Parser::expressionParser(bool inForLoop){
    tabsCheck();
    
    if(inForLoop){
        if(stmt_buffer[stmt_index].first.compare(IDENTIFIER)==0){
            cout<<tabs<<"Expression found!"<<endl;
    
            tabs.append(1,'\t');
            cout<<tabs<<element_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
            stmt_index++;
        } else {
            cout<<tabs<<"Error: Identifier expected at declaration column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
        }
    } else {
        cout<<tabs<<"Expression found!"<<endl;
    
        tabs.append(1,'\t');
            
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    }
    
    if(stmt_buffer[stmt_index].second.compare(equ_sign)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        op_found = true;
        if(!inForLoop)
            functional_order.push_back(assignStmt);
    } else {
        if(stmt_buffer[stmt_index].second.compare(plus)==0 || stmt_buffer[stmt_index].second.compare(minus)==0
                || stmt_buffer[stmt_index].second.compare(mod)==0 || stmt_buffer[stmt_index].second.compare(div)==0 || stmt_buffer[stmt_index].second.compare(mul)==0){
            cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
            op_found = true;
            if(!inForLoop)
                functional_order.push_back(arithSmt);
        }
    }
    
    if(op_found){
        stmt_index++;
        op_found = false;
    } else {
        cout<<tabs<<"Error: \'=\' or an arithmetic operator expected at expression column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    if(stmt_buffer[stmt_index].first.compare(IDENTIFIER)==0 || stmt_buffer[stmt_index].first.compare(NUMERIC_LITERAL)==0){
        cout<<tabs<<element_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: Identifier or numeric literal expected at declaration column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    if(stmt_buffer[stmt_index].second.compare(stmt_end)==0){
         cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
         stmt_index++;
     } else {
         cout<<tabs<<"Error: \';\' expected at expression column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
     } 
    
    if(!inForLoop){
        if(stmt_index != stmt_buffer.size()){
            statementsParser(stmt_buffer);
        }
    }
};

void Parser::controlFlowParser(){
    tabsCheck();
    cout<<tabs<<"Control flow found!"<<endl;
    
    tabs.append(1,'\t');
    
    if(stmt_buffer[stmt_index].second.compare(ctrl_types[0])==0){
        ++level;
        ifParser();
    } else if (stmt_buffer[stmt_index].second.compare(ctrl_types[1])==0){
        ++level;
        forParser();
    } else if (stmt_buffer[stmt_index].second.compare(ctrl_types[2])==0){
        ++level;
        whileParser();
    }
    
    --level;

};
void Parser::typeParser(vector<pair<string,string>> element_buffer){

};

void Parser::initializationParser(vector<pair<string,string>> element_buffer){

};
void Parser::functioncallParser(vector<pair<string,string>> element_buffer){

};
void Parser::paramsParser(vector<pair<string,string>> element_buffer){

};
void Parser::functionNameParser(pair<string,string> element){
    if(element.first.compare("IDENTIFIER")==0){
            cout<<tab<<"Function name found!"<<endl;
            cout<<tab<<element.first<<" :- "<<element.second<<endl;
        } else {
            cout<<tab<<"Error: Function name expected at package name column 0 which contains "<<element.second<<endl;
        }
};

void Parser::ifParser(){
    tabsCheck();
    cout<<tabs<<"If statement found!"<<endl;
    functional_order.push_back(ifStmt);
    
    tabs.append(1,'\t');
    
    cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
    stmt_index++;
    
    if(stmt_buffer[stmt_index].second.compare(opening_bracket)==0){
        cout<<tabs<<element_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: \'(\' expected at for loop column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    level++;
    
    relationParser(false);
    
    tabsCheck();
    if(stmt_buffer[stmt_index].second.compare(closing_bracket)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: \')\' expected at for loop column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    tabsCheck();
    if(stmt_buffer[stmt_index].second.compare(opening_brace)==0){
        cout<<tabs<<element_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: \'{\' expected at for loop column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    level++;
    statementsParser(stmt_buffer);
    
    tabsCheck();
    if(stmt_buffer[stmt_index].second.compare(closing_brace)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: \'}\' expected at for loop column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    --level;
};
void Parser::forParser(){
    tabsCheck();
    cout<<tabs<<"For loop found!"<<endl;
    functional_order.push_back(forStmt);
    
    tabs.append(1,'\t');
    
    cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
    stmt_index++;
    
    if(stmt_buffer[stmt_index].second.compare(opening_bracket)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: \'(\' expected at for loop column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    level++;
    
    expressionParser(true);
    
    relationParser(true);
    
    inrementParser();
    
    tabsCheck();
    if(stmt_buffer[stmt_index].second.compare(closing_bracket)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: \')\' expected at for loop column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    tabsCheck();
    if(stmt_buffer[stmt_index].second.compare(opening_brace)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: \'{\' expected at for loop column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    level++;
    statementsParser(stmt_buffer);
    
    tabsCheck();
    if(stmt_buffer[stmt_index].second.compare(closing_brace)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: \'}\' expected at for loop column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    --level;
    
};

void Parser::inrementParser(){
    tabsCheck();
    
    if(stmt_buffer[stmt_index].first.compare(IDENTIFIER)==0){
        cout<<tabs<<"Increment/Decrement found!"<<endl;

        tabs.append(1,'\t');
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: Identifier expected at increment/decrement column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    if(stmt_buffer[stmt_index].second.compare(plus)==0 || stmt_buffer[stmt_index].second.compare(minus)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        op_found = true;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: \'+\' or \'-\' expected at increment/decrement column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    if(op_found){
        if(stmt_buffer[stmt_index].second.compare(stmt_buffer[stmt_index - 1].second)==0){
            cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
            stmt_index++;
        } else {
            cout<<tabs<<"Error: \'"<<stmt_buffer[stmt_index - 1].second<<"\' expected at relation column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
        }
        op_found = false;
    } else {
       if(stmt_buffer[stmt_index].second.compare(plus)==0 || stmt_buffer[stmt_index].second.compare(minus)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
        } else {
            cout<<tabs<<"Error: \'+\' or \'-\' expected at increment/decrement column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
        }
    }
}

void Parser::whileParser(){


};
void Parser::relationParser(bool inForLoop){
    tabsCheck();
    
    
    if(stmt_buffer[stmt_index].first.compare(IDENTIFIER)==0){
        cout<<tabs<<"Relation found!"<<endl;

        tabs.append(1,'\t');
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: Identifier expected at relation column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    if(stmt_buffer[stmt_index].second.compare(less_than)==0 || stmt_buffer[stmt_index].second.compare(greater_than)==0 || stmt_buffer[stmt_index].second.compare(equ_sign)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: \'<\',\'=\' or \'>\' expected at relation column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    if(stmt_buffer[stmt_index].first.compare(IDENTIFIER)==0 || stmt_buffer[stmt_index].first.compare(NUMERIC_LITERAL)==0){
        cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
        stmt_index++;
    } else {
        cout<<tabs<<"Error: Identifier or numeric literal expected at relation column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
    }
    
    if(inForLoop){
        if(stmt_buffer[stmt_index].second.compare(stmt_end)==0){
            cout<<tabs<<stmt_buffer[stmt_index].first<<" :- "<<stmt_buffer[stmt_index].second<<endl;
            stmt_index++;
        } else {
            cout<<tabs<<"Error: \';\' expected at relation column "<<stmt_index<<" which contains "<<stmt_buffer[stmt_index].second<<endl;
        }
    } else{
        --level;
    }
    
};
void Parser::funcPrototypeParser(vector<pair<string,string>> element_buffer){

};
void Parser::argumentsParser(vector<pair<string,string>> element_buffer){

};


void Parser::argumentParser(vector<pair<string,string>> element_buffer){

};

void Parser::operatorParser(vector<pair<string,string>> element_buffer){

};

void Parser::tabsCheck(){
    tabs.clear();
    for(int i = 1; i < level; i++){
        tabs.append(1,'\t');
    }
}





