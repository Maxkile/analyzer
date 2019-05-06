/*
 * PavelLex v. 0.1 alpha. All right reserved. 2019
 */
#include "lex_analyzer.h"
#include "synth_analyzer.h"

extern unsigned int cur_line;

extern vector<Ident> id_table;
extern vector<Const> const_table;
using namespace std;


/*

〈программа〉 → program { 〈описания〉 〈операторы〉 }
〈описания〉 → { 〈описание〉; }
〈описание〉 → 〈тип〉 〈переменная〉 { , 〈переменная〉 }
〈тип〉 → int | string
〈переменная〉 → 〈идентификатор〉 |
〈идентификатор〉 = 〈константа〉
〈константа〉 → 〈целочисленная〉 | 〈строковая〉
〈целочисленная〉 → [〈знак〉] 〈цифра〉 { 〈цифра〉 }
〈знак〉 → + | −
〈строковая〉 → " { 〈литера〉 } "
〈операторы〉 → { 〈оператор〉 }
〈оператор〉 → if (〈выражение〉) 〈оператор〉 else 〈оператор〉 |
                while (〈выражение〉) 〈оператор〉|
                read (〈идентификатор〉); |
                write (〈выражение〉 { , 〈выражение〉 } ); |
〈составной оператор〉 | 〈оператор-выражение〉
〈составной оператор〉 → { 〈операторы〉 }
〈оператор-выражение〉 → 〈выражение〉;

equivalent to such statement diagram, which generates determinated state machine:

〈S〉 → program { 〈D〉 〈OP〉 }
〈D〉 → { 〈D1〉; }
〈D1〉 → 〈V〉 〈ID〉 { , 〈ID〉 }
〈T〉 → int | string
〈V〉 → 〈ID〉 |
〈ID〉 = 〈C〉
〈C〉 → 〈CI〉 | 〈CS〉
〈CI〉 → [〈знак〉] 〈цифра〉 { 〈цифра〉 }
〈знак〉 → + | −
〈CS〉 → " { 〈литера〉 } "
〈OP〉 → { 〈OP1〉 }
〈OP1〉 → if (〈EXPR〉) 〈OP1〉 else 〈OP1〉 |
                while (〈OP1〉) 〈OP1〉|
                read (〈ID〉); |
                write (〈EXPR〉 { , 〈EXPR〉 } ); |
〈COMPLEX_OP〉 | 〈EXXPR_OP〉
〈COMPLEX_OP〉 → { 〈OP〉 }
〈EXXPR_OP〉 → 〈EXPR〉;

 Scanner reads symbols from input file and parses it into sequence of Lexems
*/


int main(int argc, char **argv)
{
    if (argv[1] == nullptr)
    {
        cout << "Lex error: no input file(-s)!" << endl;
        exit(1);
    }
    try
    {
        Synparser syn_scan(argv[1]);
        syn_scan.START();
    }
    catch(char& c){ cout << "Lex error:" << " in line " << cur_line << " wrong expression " << "\"" << c << "\"" << " !" << endl; exit(2);}
    catch(string& str){ cout <<  "Lex error: " << str << "!" << endl; exit(3);}
    catch(ErrLex& err){err.getErr();exit(4);}
    catch(ErrSynthax& err){err.getErr();exit(5);}
}



















