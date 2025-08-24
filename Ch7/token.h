#ifndef TOKEN_H  // 1️⃣ If CONSTANTS_H is NOT defined yet...
#define TOKEN_H // 2️⃣ ...define CONSTANTS_H (so next time we know it's included)
#include "PPPheaders.h"

const char number = '8';                    // t.kind==number means that t is a number Token
const char quit = 'q';                      // t.kind==quit means that t is a quit Token 
const char print = ';';                     // t.kind==print means that t is a print Token
const char assignment_operator = '=';       // t.kind==assignment means that t is an assignment token
const char help = 'H';                      // t.kind==help means that t is a help token
const char name = 'a';                      // t.kind==name means that t is a name token
const char let = 'L';                       // t.kind==let means that t is a declaration token
const char constant = 'C';                  // t.kind==constant means that t is a token that represents a constant
const string declkey = "let";                   // declaration keyword
const string const_declkey = "const";           // "const" is the constant name declaration keywork
const string anskey = "ans";                    // "ans" is the name of the variable holding the previous calculation value


class Token {
public:
    char kind;        // what kind of token
    double value;     // for a token representing a number: a value 
    string name;      // for a token representing a variable
    Token();
    Token(char ch);
    Token(char ch, double val);
    Token(char ch, string n );
};

class Variable { 
    public:
        string name;    // a name identifying a variable
        double value;   // The numeric value associated with the variable
        bool constant;
        Variable(string var_name, double var_val);
        Variable(string var_name, double var_val, bool is_constant);
};

class Symbol_table{ 
    public:
            double get_value(string s);                                     // return the value of the Variable named s                                      
            void set_value(string s, double d);                             // set the Variable named s to d                             
            bool is_declared(string var);                                   // is a variable with the anme var already in var_table?       
            double define_name(string var, double val, bool is_constant);   // add Variable {var,val, constant flag} to var_table
    private:
            vector<Variable> var_table; // Stores all user-defined variables, each with a name, value, and is_constant flag
};

class Token_stream { // handles input tokenization: retrieves, buffers, and manages tokens from the input stream
    public:
        Token get();            // get a Token
        void putback(Token t);  // put a Token back into a buffer
        void ignore(char c);    // discard characters up to and including a character 'c'
    private:
        bool full = false;      // is there a Token in the buffer?
        Token buffer = {'0'};   // where we store a 'putback' Token
};

#endif

