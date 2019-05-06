#include "lex_analyzer.h"

unsigned int cur_line = 1;

vector<Ident> id_table;
vector<Const> const_table;

    inline void OpenErr::getErr() const
    {
        cout << "Error with opening files!" << endl;
    }

    inline void BracketsErr::getErr() const
    {
        cout << "Lex error: expected \'}\' at the end of an input!" << endl;
    }


    inline void BracersErr::getErr() const
    {
        cout << "Lex error: expected \')\' before  \'}\' token!" << endl;
    }


    inline void ConmasErr::getErr() const
    {
        cout << "Lex error: expected \" token!" << endl;
    }

    Lexeme::Lexeme(const Lexeme& lex): type(lex.type), value(lex.value),strval(lex.strval){};

    int Lexeme::getVal() const
    {
        return value;
    }

    string Lexeme::getStr() const
    {
        return strval;
    }
    
    type_of_lex Lexeme::getType() const
    {
        return type;
    }

    int constPut(Const& con)
    {
        const_table.push_back(con);
        return const_table.size();
    }

    Const::Const(const type_of_lex& t,const string& val): type(t),val(val){};

    type_of_lex Const::getType() const
    {
        return type;
    }

    string Const::getVal() const
    {
        return val;
    }

    int idPut(Ident& id)
    {
        id_table.push_back(id);
        return id_table.size();
    }

    Ident::Ident()
    {
        decl = false;
        assign = false;
    }

    void Ident::chAss()
    {
        assign = true;
    }

    void Ident::chDecl()
    {
        decl = true;
    }

    void Ident::chName(string& name)
    {
        id_name = name;
    }

    bool Ident::isAss() const
    {
        return assign;
    }

    bool Ident::isDecl() const
    {
        return decl;
    }

    string Ident::getName() const
    {
        return id_name;
    }

    int Ident::getVal() const
    {
        return value;
    }
    type_of_lex Ident::getType() const
    {
        return type;
    }

    Scanner::Scanner(const string &filename1):brace_cnt(0),brack_cnt(0),isConmas(false)
    {
        in.open(filename1,fstream::in);;
        if ((!in.is_open())) throw OpenErr();
        in.clear();
        cur_state = D;
    }

    void Scanner::gc(ifstream& in)
    {
        in.read(&c,sizeof(char)); 
    };

    int Scanner::findID(const string& str,const vector<string> table)//0 - if we haven't found ID
    {
        int res = 0;
        for (unsigned long p = 0; p != table.size();++p)
        {
            if (table[p] == str)
            {
                res =  p;
                break;
            }
        }
        return res;
    };


    Lexeme Scanner::getLexem()
    {
        string buf;
        for(;;)
        { 
            gc(in);
            if (c == '\n')cur_line++;
            switch(cur_state)
            {
                case D://base(default) state
                    if ((isspace(c)) || (c == '\r') || (c == '\t') || (c == '\n'));
                    else if (isalpha(c) || (c == '_'))
                    {
                        buf.push_back(c);
                        cur_state = ID;
                    }
                    else if (isdigit(c))
                    
                    {
                        num = c - '0';
                        cur_state = NUM;
                    }

                    else if (c == '\"') {
                        buf.push_back(c);
                        isConmas = !isConmas;
                        cur_state = S;
                    }

                    else if ((c == '=') || (c == '<') || (c == '>')) {
                        buf.push_back(c);
                        cur_state = REL;
                    }

                    else if (c == '!') {
                        err_c = c;
                        cur_state = DEN;
                        buf.push_back(c);
                    
                    }

                    else if ((c == '$') || in.eof())
                    {//final state
                        if (brace_cnt != 0) throw BracersErr();
                        else if (brack_cnt != 0) throw BracketsErr();
                            else if (isConmas == true)
                                 {
                                    cur_line--;
                                    throw ConmasErr();
                                 } 
                        return Lexeme(FIN);
                        
                    }

                    else if (c == '{') {
                        brack_cnt++;
                        return Lexeme(OPEN_BRACKET);
                    }

                    else if (c == '}') {
                        brack_cnt--;
                        return Lexeme(CLOSE_BRACKET);
                    }

                    else if (c == '(') {
                        brace_cnt++;
                        return Lexeme(OPEN_BRACE);
                    }

                    else if (c == ')') 
                    {
                        brace_cnt--;
                        return Lexeme(CLOSE_BRACE);
                    }
                    else if (c == '/')//comments
                    {
                        buf.push_back(c);
                        cur_state = COMMENT;
                    }
                    else
                    {
                        buf.push_back(c);
                        cnt = findID(buf,key_signs);
                        if (cnt == 0)
                        {
                            cur_state = ERR;
                            err_c = c;
                        }
                        else
                        {
                            buf.clear();
                            return Lexeme((type_of_lex)(CLOSE_BRACE + cnt - 2),cnt);
                        
                        }
                    }
                    break;

                case COMMENT:
                    buf.clear();
                    if (c == '/')
                    {
                        while(c != '\n') gc(in);
                        cur_state = D;
                        in.seekg(-1,in.cur);
                    }
                    else
                    {
                        cur_state = D;
                        in.seekg(-1,in.cur);
                        if (c == '\n') cur_line--;//to prevent program from increasing cur_line two times from one '\n'
                        return Lexeme(DIVIDE);
                        
                    }
                    break;

                case ID:
                    if (isalpha(c) || isdigit(c))
                    {
                        buf.push_back(c);
                    }
                    else//end of identifier
                    {
                        cur_state = D;
                        in.seekg(-1,in.cur);
                        if (c == '\n') cur_line--;//to prevent program from increasing cur_line two times from one '\n'
                        cnt = findID(buf,key_words);
                        if (cnt == 0)//then it's new variable-identifier
                        {
                            Ident temp;
                            temp.chName(buf);
                            int val = idPut(temp);
                            buf.clear();
                            return Lexeme(IDENT,val);
                        }
                        else//that's key word
                        {
                            buf.clear();
                            return Lexeme((type_of_lex)(cnt));
                        }
                    
                    }
                    break;

                case NUM:
                    if (isdigit(c)) num = num * 10 + (c - '0');
                    else if (isalpha(c))
                    {
                        cur_state = ERR;
                        err_c = c;
                    }
                    else
                    {
                        in.seekg(-1,in.cur);
                        cur_state = D;
                        Const tmp(NUMBER,to_string(num));
                        int val = constPut(tmp);
                        buf.clear();
                        if (c == '\n') cur_line--;//to prevent program from increasing cur_line two times from one '\n'
                        return Lexeme(NUMBER,val);
                    }
                    break;

                case S:
                    if (c == '\"')
                    {
                        buf.push_back(c);
                        isConmas = !isConmas;
                        cur_state = D;
                        Const tmp(STR,buf);
                        int val = constPut(tmp);
                        Lexeme temp(STR,val,buf);
                        buf.clear();
                        return temp;
                    }    
                    else if ((c == '$') || (c == EOF)) throw ConmasErr();
                    else buf.push_back(c);
                    break;

                case REL:
                    cur_state = D;
                    if (c == '=')
                    {
                        buf.push_back(c);
                        cnt = findID(buf,key_signs);
                        buf.clear();
                        return Lexeme((type_of_lex)(CLOSE_BRACE + cnt - 2));
                    }
                    else
                    {
                        in.seekg(-1,in.cur);
                        if (c == '\n') cur_line--;//to prevent program from increasing cur_line two times from one '\n'
                        cnt = findID(buf,key_signs);
                        buf.clear();
                        return Lexeme((type_of_lex)(CLOSE_BRACE + cnt - 2),cnt);
                    }
                    break;

                case DEN:
                    if (c == '=')
                    {
                        cur_state = D;
                        buf.push_back(c);
                        cnt = findID(buf,key_signs);
                        buf.clear();
                        return Lexeme((type_of_lex)(CLOSE_BRACE + cnt - 2),cnt);
                    
                    }
                    else
                    {
                        cur_state = ERR;
                    }
                    buf.clear();
                    break;

                case ERR:
                    throw err_c;//error on char c!
            }
        }
    }

    Scanner::~Scanner()
    {
        in.close();   
    }

