/*
    Simple Calculator

    Revision history:
    Revision by Rami Omer (rami.ba.omer@gmail.com) August 2025
    Revised by Bjarne Stroustrup (bjarne@stroustrup.com) Novembe  2023
    Revised by Bjarne Stroustrup November 2013
    Revised by Bjarne Stroustrup May 2007
    Revised by Bjarne Stroustrup August 2006
    Revised by Bjarne Stroustrup August 2004
    Originally written by Bjarne Stroustrup (bs@cs.tamu.edu) Spr ng 2004.

    This program implements a basic expression calculator.
    Input from cin; output to cout.

    The grammar for input is:

    Calculation:
		Statement
		Print : ';'
		Quit: 'q'
		Statement calculation
    
    Statement:
		Declaration
        Assignment
    	Expression
    
    Declaration:
		"let" name "=" Expression
    
    Assignment:
        name "=" Expression

    Expression:
        Term
        Expression "+" Term
        Expression "-" Term
       
    Term:
        Primary
        Term "*" Primary
        Term "/" Primary
        Term "%" Primary
       
    Primary:
        Number
		Name
		"-" Primary
        "(" Expression ")"
        
    Number:
        floating-point-literal

	Name:
		a string of character: must start with an alphabet and can contain only alphabet and digit charachters
       
        Input comes from cin through the Token_stream called ts.
   */

#include "PPPheaders.h"

//------------------------------------------------------------------------------

class Token {
public:
    char kind;        // what kind of token
    double value;     // for a token representing a number: a value 
    string name;      // for a token representing a variable
    Token():kind{0}{};                                   // default constructor
    Token(char ch):kind{ch}{};                           // Initialize kind with a char for a defined symbol   
    Token(char ch, double val):kind{ch}, value{val}{};   // Initialize kind and value for a number
    Token(char ch, string n ):kind{ch}, name{n}{};       // Initialize kind and name for a variable name
};



class Variable { 
    public:
        string name;    // a name identifying a variable
        double value;   // The numeric value associated with the variable
};

vector<Variable> var_table; // Stores user-defined variables with their names and values

double get_value(string s)
    // return the value of the Variable named s
{
    for (const Variable& v : var_table)
        if (v.name == s) return v.value;

    error("trying to read undefined variable ", s);  
}

void set_value(string s, double d) 
    // set the Variable named s to d
{
    for (Variable& v : var_table)
        if (v.name == s) {
             v.value = d;
             return; 
        }

    error("trying to write undefined variable ", s); 

}

bool is_declared(string var)
// is a variable with the anme var already in var_table?
{
    for (const Variable& v : var_table)
        if (v.name == var) 
            return true;
    return false; 
}

double define_name(string var, double val) 
// add Variable {var,val} to var_table
{
    if (is_declared(var)) // if the variable name is already define produce an error message
        error(var," declared twice");
    var_table.push_back(Variable{var,val});
    return val;
}


class Token_stream { // handles input tokenization: retrieves, buffers, and manages tokens from the input stream
    public:
        Token get();            // get a Token
        void putback(Token t);  // put a Token back into a buffer
        void ignore(char c);    // discard characters up to and including a character 'c'
    private:
        bool full = false;      // is there a Token in the buffer?
        Token buffer = {'0'};   // where we store a 'putback' Token
};

void Token_stream::putback(Token t)
{ 
    if (full) error("putback() into a full buffer");
    buffer = t;         // opy t to buffer
    full = true;        // buffer is now full
}

constexpr char number = '8';                    // t.kind==number means that t is a number Token
constexpr char quit = 'q';                      // t.kind==quit means that t is a quit Token 
constexpr char print = ';';                     // t.kind==print means that t is a print Token
constexpr char assignment_operator = '=';       // t.kind==assignment means that t is an assignment token

constexpr char name = 'a';                      // t.kind==name means that t is a name token
constexpr char let = 'L';                       // t.kind==let means that t is a declaration token
const string declkey = "let";                   // declaration keyword



Token Token_stream::get()
// tokenizes the input stream and retrieves the next token
{
    if (full) { // if the buffer is full, retrive the token from the buffer
        full = false; // the buffer is now empty
        return buffer;
    }
    char ch;
    cin >> ch; // skips whitespace (space, newline, tab, etc.)

    switch (ch) {
        case print:                 // print == ';'
        case quit:                  // quit == 'q'  
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
                cin.putback(ch); //put back the first non-alphanumeric character into the input stream
                if (s == declkey)
                    return Token{let}; // return a token representing the declaration keyword "let"
                return Token{name,s};  // return a token representing a variable name
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

//------------------------------------------------------------------------------

Token_stream ts;      // provides get(), putback() and ignore() functions

//------------------------------------------------------------------------------

double expression();  // read and evaluate an Expression

//-----------------------------------------------------------------------------

void clean_up_mess(){ 
    //handling errors (bad tokens) without terminating program
    cin.clear();        // clear fail state
    ts.ignore(print); // eat all character up to and including print ';'
}

//-----------------------------------------------------------------------------

double primary() 
// reads and evaluates a : number (negative and positive), a variable name, and a parenthesized expression ("Expression")     
{
    Token t = ts.get(); // get the next token
    switch (t.kind) {
    case '(':    // handle '(' expression ')'
        {    
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected"); 
            return d;
        }
    case name: // variable 
        { 
            if(!is_declared(t.name))  // if the variable name already exists in the var_table, produce an error message
                error("undefined variable ", t.name);
            Token t2 = ts.get();

		    if(t2.kind == '=') 
			    error ("expected \"let\" before ", t.name); // '=' requires 'let' before the variable name
		    ts.putback(t2);
            return get_value(t.name); // return the value associated with the variable name
        }
    case number: // floating-point literal          
        return t.value;  // return the number's value
    
    case '-': // unary minus for negative numbers
        return -primary();
    
    default: // unrecognized token kind
        error("primary expected");
    }
}

//-----------------------------------------------------------------------------------

double term()
// reads and evaluates a Term
{
    double left = primary(); // read and evaluate a primary
    Token t = ts.get();     // get the next token

    while(true) {
        switch (t.kind) {
        case '*':
            left *= primary(); // read and evaluate a primary and then multiply
            t = ts.get();
            break;
        case '/':
            {    
                double d = primary(); 
                if (d == 0) error("/:division by zero");
                left /= d; 
                t = ts.get();
                break;
            }
       case '%':
            { 
                double d = primary();
                if (d == 0) error("%:divide by zero");
                left = fmod(left,d); // compute the floating-point remainder
                t = ts.get();
                break;
            }
        default: 
            ts.putback(t); // put back the token in the buffer
            return left;  // finally: no more * , /, or -, fmod: return the answer
        }
    }
}

//-----------------------------------------------------------------------------------

double expression()
{
    double left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token
    while(true) {    
        switch(t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term();    // evaluate Term and subtract
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;       // finally: no more + or -: return the answer
        }
    }
}

//------------------------------------------------------------------------------

double assignment(string var_name){
    // assign a new value to an existing variable
    if(!is_declared(var_name)) error (var_name, " not defined");
    double d = expression(); // read and evaluate the new value
    set_value(var_name, d); // update the value of the variable in var_table
    return d;
}

//------------------------------------------------------------------------------

double declaration()
// assume we have seen "let”
// handle: <name> "=" <expression>
// declare a variable called "name” with the initial value "expression”
{
    Token t = ts.get(); 
    if (t.kind != name) // if the token doesn't represent a variable name, produce an error message
        error ("name expected in declaration");
    Token t2 = ts.get(); // get the next token
    if (t2.kind != '=') 
        error("= missing in declaration of ", t.name); 
    double d = expression(); // read and evaluate the expression after the '='
    define_name(t.name,d); // define the variable and add its name and associated numerical value to the var_table
    return d; // return the value associated with the variable
}

//-----------------------------------------------------------------------------------

double statement()
// read and evaluate a statement: either an expression or a declaration
{
    Token t = ts.get(); 
    switch (t.kind) { 
    case let: // if a statement starts with "let", it is a declaration statement
        return declaration();
    case name: // might be an assignment or a regular expression using a variable
    {
        char c = cin.get(); 
        while (c  == ' ' || c == '\n') // skip whitespace and newline characters to detect assignment
            c = cin.get();  
        if(c == assignment_operator){
            return assignment(t.name); // direct assignment (example x = 5)
        } 
        // not an assignment — put characters back and treat as expression
        ts.putback(t); // put name token in the buffer
        cin.putback(c); 
        return expression();
    }
    default:
        ts.putback(t);
        return expression(); 
    }
}

//------------------------------------------------------------------------------
constexpr string prompt = "> "; // used to prompt a user to enter a statement
constexpr string result = "= "; // used to indicate that what follows is a result


void calculate()
// expression evaluation loop
// main loop for the calculator:
{
    while (true) {
        // handles reading input, evaluating statements, printing results, and processing quit commands,
    try{
        cout << prompt; 
        Token t = ts.get(); 
        
        while (t.kind == print)     // first discard all "prints"
            t=ts.get(); 
        
        if (t.kind == quit)  
            return;  // quit the calculator if the token is a quit token
        ts.putback(t);

        cout << result << statement() << '\n'; // evaluate the statement

    }
    catch(exception& e){ // managing exceptions
        cerr << e.what() << '\n';   // write error message
        clean_up_mess();
    }
    }
}

//------------------------------------------------------------------------------

int main() {

    try{
        // predefine names:
        define_name("pi",3.1415926535);
        define_name("e",2.7182818284);

        calculate();
        return 0; 

    } catch (exception& e) {
        cerr << e.what() << '\n'; 
        return 1;

    }catch (...) {
        cerr << "exception \n";
        return 2; 
    }
}

