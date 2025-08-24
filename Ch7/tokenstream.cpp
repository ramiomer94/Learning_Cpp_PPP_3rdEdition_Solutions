#include "token.h"



//------------------------------------------------------------------------------


    Token::Token():kind{0}{};                                   // default constructor
    Token::Token(char ch):kind{ch}{};                           // Initialize kind with a char for a defined symbol   
    Token::Token(char ch, double val):kind{ch}, value{val}{};   // Initialize kind and value for a number
    Token::Token(char ch, string n ):kind{ch}, name{n}{};       // Initialize kind and name for a variable name



    Variable::Variable(string var_name, double var_val): name{var_name}, value{var_val}, constant{false}{};
    Variable::Variable(string var_name, double var_val, bool is_constant): name{var_name}, value{var_val}, constant{is_constant}{};



double Symbol_table::get_value(string s)
    // return the value of the Variable named s
{
    for (const Variable& v : var_table)
        if (v.name == s) return v.value;

    error("trying to read undefined variable ", s);  
}

void Symbol_table::set_value(string s, double d) 
    // set the Variable named s to d
{
    for (Variable& v : var_table)
        if (v.name == s) {
            if(v.constant) error("trying to write a constant ", v.name);
             v.value = d;
             return; 
        }
    error("trying to write undefined variable ", s); 
}

bool Symbol_table::is_declared(string var)
// is a variable with the anme var already in var_table?
{
    for (const Variable& v : var_table)
        if (v.name == var) 
            return true;
    return false; 
}

double Symbol_table::define_name(string var, double val, bool is_constant) 
// add Variable {var,val, constant flag} to var_table
{
    if (is_declared(var)) // if the variable name is already define produce an error message
        error(var," declared twice");
        
    var_table.push_back(Variable{var,val,is_constant});
    return val;
}

void Token_stream::putback(Token t)
{ 
    if (full) error("putback() into a full buffer");
    buffer = t;         // opy t to buffer
    full = true;        // buffer is now full
}



Token Token_stream::get()
// tokenizes the input stream and retrieves the next token
{
    if (full) { // if the buffer is full, retrive the token from the buffer
        full = false; // the buffer is now empty
        return buffer;
    }
    char ch;
    cin.get(ch); // doen't skips whitespace (space, newline, tab, etc.)

    while(isspace(ch)){
        if(ch == '\n') return Token{print};
        cin.get(ch);
    }
    switch (ch) {
        case print:                 // print == ';'
        case assignment_operator:   // assignment == '='    
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '_':
            return Token{ch};   // let each character represent itself
 
        case '.': // a floating-point-literal can start with a dot
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': // numeric literal
           {
                cin.putback(ch); // put digit back into the input stream
                double val;
                cin >> val;     // read a floating-point number
                if(!cin) error("not a number");
                return Token{number, val}; // return a token representing a numeric literal with value val
            } 
       
        default: // handles variable names and keywords
            if (isalpha(ch)) { // a name must start with an alphabetic character
                string s;
                s+=ch;

                // Continue reading while the characters are alphabetic, digits or underscore
                while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_'))  
                    s+=ch; 

                cin.putback(ch);                            //put back the first non-alphanumeric character into the input stream
                if (s == declkey)
                    return Token{let};                      // return a token representing the declaration keyword "let"
                if (s == const_declkey)
                    return Token{constant};                 // return a token representing the constant declaration keyword "const"
                if (s == "help")
                    return Token{help};                     // return a token representing the help command
                if (s == "quit") return Token{quit};        // return a token representing the "quit-the-calculator" command
                return Token{name,s};                       // return a token representing a variable name
            }
            error("Bad Token");
    }
}

void Token_stream::ignore(char c) 
    // Skip input until a token of kind 'c' is found
{
    if (full && c==buffer.kind) {  // first look in buffer
        full = false;
        return; 
    }

    full = false; // Discard the buffered token if not matching

    // now search input:
    char ch = 0;
    while (cin>>ch) // discard characters until we find 'c'
        if (ch==c) return;

}