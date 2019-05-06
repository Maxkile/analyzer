#ifndef _lex_analyzer
#define _lex_analyzer

#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <map>
#include <string>

using namespace std;


enum type_of_lex{
    NOTHING1,//0
    AND,//1
    BREAK,
    CASE,
    CONTINUE,
    DEFAULT,
    DO,
    IF,
    INT,
    ELSE,
    FALSE,
    FOR,
    NOT,
    OF,
    OR,
    PROGRAM,
    READ,
    STRING,
    TRUE,
    UNTIL,
    WHILE,
    WRITE,
    IDENT,//23
    NUMBER,
    STR,
    NOTHING2,
    ASSIGN,//27
    CLOSE_BRACE,
    CLOSE_BRACKET,
    COLON,
    COMMA,
    CONMAS,
    COMMENT,
    DIVIDE,
    DOT,
    EQUAL,
    EXCLAMATION,
    GREATER,
    GREATER_EQ,
    LESS,
    LESS_EQ,
    MINUS,
    MULTIPLY,
    OPEN_BRACE,
    OPEN_BRACKET,
    SEMICOLON,
    PLUS,
    FIN,//47

};

//key words of our alphabet
const vector<string> key_words = {
        "",
        "and",
        "break",
        "case",
        "continue",
        "default",
        "do",
        "if",
        "int",
        "else",
        "false",
        "for",
        "not",
        "of",
        "or",
        "program",
        "read",
        "string",
        "true",
        "until",
        "while",
        "write",

};

//key signes of our alphabet
const vector<string> key_signs = {
        "",
        "=",
        "}",
        ")",
        ":",
        ",",
        "\"",
        "//",
        "/",
        ".",
        "==",
        "!=",
        ">",
        ">=",
        "<",
        "<=",
        "-",
        "*",
        "{",
        "(",
        ";",
        "+",
        "$",

};

static map<type_of_lex,string> lexemes = {
    {AND,"LEX_AND"},
    {BREAK,"LEX_BREAK"},
    {CASE,"LEX_CASE"},
    {CONTINUE,"LEX_CONTINUE"},
    {DEFAULT,"LEX_DEFAULT"},
    {DO,"LEX_DO"},
    {IF,"LEX_IF"},
    {INT,"LEX_INT"},//type
    {ELSE,"LEX_ELSE"},
    {FALSE,"LEX_FALSE"},
    {FOR,"LEX_FOR"},  
    {NOT,"LEX_NOT"},
    {OF,"LEX_OF"},
    {OR,"LEX_OR"},
    {PROGRAM,"LEX_PROGRAM"},
    {READ,"LEX_READ"},
    {STRING,"LEX_STRING"}, //type
    {TRUE,"LEX_TRUE"},
    {UNTIL,"LEX_UNTIL"},
    {STRING,"LEX_STRING"}, //string const
    {WHILE,"LEX_WHILE"},
    {WRITE,"LEX_WRITE"}, 
    {NUMBER,"LEX_NUM"}, //int const
    {STR,"LEX_STR"},
    {IDENT,"LEX_IDENT"}, 
    {ASSIGN,"LEX_ASSIGN"},
    {CLOSE_BRACE,"LEX_CLOSE_BRACE"},
    {CLOSE_BRACKET,"LEX_CLOSE_BRACKET"},
    {COLON,"LEX_COLON"},
    {COMMA,"LEX_COMMA"},
    {CONMAS,"LEX_CONMAS"},
    {COMMENT,"LEX_COMMENT"},
    {DIVIDE,"LEX_DIVIDE"},
    {DOT,"LEX_DOT"},
    {EQUAL,"LEX_EQUAL"},
    {EXCLAMATION,"LEX_EXCLAMATION"},
    {GREATER,"LEX_GREATER"},
    {GREATER_EQ,"LEX_GREATER_EQ"},
    {LESS,"LEX_LESS"},
    {LESS_EQ,"LEX_LESS_EQ"},
    {MINUS,"LEX_MINUS"},
    {MULTIPLY,"LEX_MULTIPLY"},
    {OPEN_BRACE,"LEX_OPEN_BRACE"},
    {OPEN_BRACKET,"LEX_OPEN_BRACKET"},
    {SEMICOLON,"LEX_SEMICOLON"},
    {PLUS,"LEX_PLUS"},
    {FIN,"LEX_FIN"}
};



struct ErrLex
{
    virtual inline void  getErr() const{};
};
struct OpenErr: public ErrLex
{
    inline void  getErr() const;
};
struct BracersErr: public ErrLex
{
    inline void  getErr() const;
};
struct BracketsErr: public ErrLex
{
    inline void  getErr() const;
};
struct ConmasErr: public ErrLex
{
    inline void  getErr() const;
};


class Lexeme
{
    type_of_lex type;
    int value;
    string strval;

public:
    explicit Lexeme(type_of_lex l_type = NOTHING1, int l_val = 0,string val = ""): type(l_type), value(l_val),strval(val){};
    Lexeme(const Lexeme& lex);

    int getVal() const;

    string getStr() const;
    
    type_of_lex getType() const;

};


class Const
{
    type_of_lex type;
    string val;
public:
    Const(const type_of_lex&,const string&);
    
    type_of_lex getType() const;

    string getVal() const;
};

//value is number of identifier in table of identifiers
class Ident
{
    string id_name;
    type_of_lex type;
    int value;
    bool decl,assign;

public:

    Ident();

    void chAss();

    void chDecl();

    void chName(string& name);

    bool isAss() const;

    bool isDecl() const;

    string getName() const;

    int getVal() const;

    type_of_lex getType() const;

};

//number of possible identifiers is not limited(table of identifiers)



int constPut(Const& con);
int idPut(Ident& id);

class Scanner
{
    enum lex_state{D,COMMENT,ID,NUM,S,DEN,REL,ERR};
    
    /** D - main state
     * ID - identifier state
     * NUM - number state
     * STR - string state
     * ERR - error state
     * COMMENT - state for "//" processing
     * DEN - state for '!' with '=' processing(denial)
     * REL - state for '>','<' or '=' with '=' processing(relation)
     * */

    char c;
    ifstream in;
    lex_state cur_state;
    
    char err_c;
    int cnt,num;
    int brace_cnt;
    int brack_cnt;
    bool isConmas;

public:
    Scanner(const string &filename1);

    void gc(ifstream& in);

    int findID(const string& str,const vector<string> table);

    void push_lex(Lexeme lex, list<string>& out);
    
    //generates Lexeme from input text
    Lexeme getLexem();

    ~Scanner();

};
#endif