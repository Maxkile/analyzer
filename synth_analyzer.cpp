#include "synth_analyzer.h"



ErrSynthax::ErrSynthax(const Lexeme& l,enum type_of_lex t = NOTHING1): lex(l),type(t){};

Synparser::Synparser(const string &filename):lex_scan(filename){};

void Synparser::gl()
{
    temp = lex_scan.getLexem();
    l_type = temp.getType();
    l_val = temp.getVal();
    l_str = temp.getStr();
}

void Synparser::START()
{
    cout << "START()" << endl;
    gl();
    if (l_type == PROGRAM)
    {
        gl();
        if (l_type == OPEN_BRACKET)
        {
            gl();
            MAIN();
            gl();
            if (l_type != FIN) throw ErrSynthax(temp,FIN);//extra(always have end)
        } else throw ErrSynthax(temp,OPEN_BRACKET);
    } else throw ErrSynthax(temp,PROGRAM);
}

void Synparser::MAIN()
{
    cout << "MAIN()" << endl;
    DESCRS();
    OPERS();
    cout << "Synthax sucess!" << endl;
    
}
void Synparser::DESCRS()
{
    cout << "DESCRS()" << endl;
    while ((l_type == STRING) || (l_type == INT))
    {
        gl();
        DESCR();
        if (l_type != SEMICOLON) throw ErrSynthax(temp,SEMICOLON);
        else gl();  
    }
}

void Synparser::DESCR()
{
    cout << "DESCR()" << endl;
    VAR();
    while (l_type == COMMA)
    {
        gl();
        VAR();//if non then return to DESCRS()  
    } 
}

void Synparser::VAR()
{
    cout << "VAR()" << endl;
    ID();
    if (l_type == ASSIGN)
    {
      gl();
      CONST();//if non then return to DESCR()  
    } 
}

void Synparser::ID()
{
    cout << "ID()" << endl;
    if (l_type == IDENT)
    {
        gl();
    }
    else throw ErrSynthax(temp,IDENT);
}

void Synparser::CONST()
{
    cout << "CONST()" << endl;
    if (l_type != STR)
    {
        if ((l_type != PLUS) && (l_type != MINUS))
        {
            if (l_type != NUMBER) throw ErrSynthax(temp,NUMBER);
            else gl();
        }
        else if (l_type != NUMBER) throw ErrSynthax(temp,NUMBER);
            else gl();
    } else gl();
}

void Synparser::OPERS()
{
    cout << "OPERS()" << endl;
    while(1)
    {
        OPER();
        if (l_type == CLOSE_BRACKET) break;
    }
}

void Synparser::OPER()
{
    cout << "OPER()" << endl;
    if (l_type == IF)
    {
        gl();
        if (l_type == OPEN_BRACE)
        {
            gl();
            EXPR();
            if (l_type != CLOSE_BRACE) throw ErrSynthax(temp,CLOSE_BRACE);
            else gl();
            OPER();
            if (l_type != ELSE) throw ErrSynthax(temp,ELSE);
            else gl();
            OPER();
        } else throw ErrSynthax(temp,OPEN_BRACE);
    }
    else if (l_type == WHILE)
         {   
            gl();
            if (l_type == OPEN_BRACE)
            {
                gl();
                EXPR();
                if (l_type != CLOSE_BRACE) throw ErrSynthax(temp,CLOSE_BRACE);
                else gl();
                OPER();
            } else throw ErrSynthax(temp,OPEN_BRACE);
         }
         else if (l_type == READ)
            {  
                gl();
                if (l_type == OPEN_BRACE)
                {
                    gl();
                    ID();
                    if (l_type != CLOSE_BRACE) throw ErrSynthax(temp,CLOSE_BRACE);
                    else gl();
                    if (l_type != SEMICOLON) throw ErrSynthax(temp,SEMICOLON);
                    else gl();
                } else throw ErrSynthax(temp,OPEN_BRACE);
            }
            else if (l_type == WRITE)
                {  
                    gl();
                    if (l_type == OPEN_BRACE)
                    {
                        gl();
                        EXPR();
                        while (l_type == COMMA)
                        {
                            gl();
                            EXPR();
                        } 
                        if (l_type != CLOSE_BRACE) throw ErrSynthax(temp,CLOSE_BRACE);
                        else gl();
                        if (l_type != SEMICOLON) throw ErrSynthax(temp,SEMICOLON);
                        else gl();
                    } else throw ErrSynthax(temp,OPEN_BRACE);
                }
                else if (l_type == DO)
                     {
                        gl();
                        OPER();
                        if (l_type == WHILE)
                        {
                            gl();
                            if (l_type == OPEN_BRACE)
                            {
                                gl();
                                EXPR();
                                if (l_type == CLOSE_BRACE)
                                {
                                    gl();
                                    if (l_type == SEMICOLON) gl();
                                    else throw ErrSynthax(temp,SEMICOLON);
                                }
                                else throw ErrSynthax(temp,CLOSE_BRACE);
                            } else throw ErrSynthax(temp,OPEN_BRACE);
                        } else throw ErrSynthax(temp,WHILE);
                     } 
                    else if (l_type == CLOSE_BRACKET);
                        else if ((l_type != STRING) && (l_type != INT))
                            {
                                OPEXPR();
                                if (l_type != SEMICOLON)  throw ErrSynthax(temp,SEMICOLON);
                                else gl();
                            }
                            else throw ErrSynthax(temp);  

            
}


void Synparser::OPEXPR()
{
    EXPR();
}

void Synparser::EXPR()//EXPR() ALWAYS(if not an exception) gives lexem on output!!
{
    cout << "EXPR_ASSIGN()" << endl;
    EXPR_1();
    while (l_type == ASSIGN)
    {
        gl();
        EXPR_1();
    } 
}

void Synparser::EXPR_1()
{
    cout << "EXPR_OR()" << endl;
    EXPR_2();
    while (l_type == OR)
    {
        gl();
        EXPR_2();
    } 
}

void Synparser::EXPR_2()
{
    cout << "EXPR_AND()" << endl;
    EXPR_3();
    while (l_type == AND)
    {
        gl();
        EXPR_3();
    }  
}

void Synparser::EXPR_3()
{
    cout << "EXPR_SIGNS()" << endl;
    EXPR_4();
    while ((l_type == GREATER) || (l_type == GREATER_EQ) || (l_type == LESS) || (l_type == LESS_EQ) || (l_type == EQUAL) || (l_type == EXCLAMATION))
    {
        gl();
        EXPR_4();
    } 
}

void Synparser::EXPR_4()
{
    cout << "EXPR_PLUS_MINUS()" << endl;
    EXPR_5();
    while ((l_type == PLUS) || (l_type == MINUS))
    {
        gl();
        EXPR_5();
    } 
}

void Synparser::EXPR_5()
{
    cout << "EXPR_MUL_DIV()" << endl;
    EXPR_6();
    while ((l_type == DIVIDE) || (l_type  == MULTIPLY))
    {
        gl();
        EXPR_6();
    } 
}

void Synparser::EXPR_6()
{
    cout << "EXPR_MINUS()" << endl; 
    while (l_type == MINUS) gl();
    EXPR_7();
}

void Synparser::EXPR_7()
{
    cout << "EXPR_NOT()" << endl;
    while (l_type == NOT) gl();
    EXPR_8();
}

void Synparser::EXPR_8()
{
    cout << "EXPR_TERM()" << endl;
    if ((l_type != STR) && (l_type != NUMBER) && (l_type != IDENT) && (l_type != TRUE) && (l_type != FALSE)) throw ErrSynthax(temp,IDENT);
    else gl();
}
