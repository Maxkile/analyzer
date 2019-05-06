#ifndef _synth_analyzer
#define _synth_analyzer

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <map>
#include "lex_analyzer.h"

using namespace std;


class ErrSynthax
{
    Lexeme lex;
    enum type_of_lex type;
public:
    ErrSynthax(const Lexeme& ,enum type_of_lex t);
    inline void getErr() const
    {
        if (type == NOTHING1) cout << "Syntax error: description part after operators part!" << endl;
        else
        {
            cout << "Syntax error: unexpected token " << lexemes[lex.getType()];
            cout << ".Expected " << lexemes[type];    
        }
    }
};

class Synparser
{
    Scanner lex_scan;
    Lexeme temp;
    enum type_of_lex l_type;
    int l_val;
    string l_str;

public:
    Synparser(const string &filename1);
    void gl();

    void START();
    void MAIN();
    void DESCRS();
    void DESCR();
    void TYPE();
    void VAR();
    void ID();
    void CONST();
    void OPERS();
    void OPER();
    void OPEXPR();
    void COM_OP();
    void EXPR();
    void EXPR_1();
    void EXPR_2();
    void EXPR_3();
    void EXPR_4();
    void EXPR_4_1();
    void EXPR_5();
    void EXPR_5_1();
    void EXPR_6();
    void EXPR_6_1();
    void EXPR_7();
    void EXPR_8();
};

#endif