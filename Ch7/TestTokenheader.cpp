#include "token.h"


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
		Print : ';' or new line '\n'
		Quit: 'quit'
        Help: 'help'
		Statement calculation
    
    Statement:
		Declaration
        Declaration; statement
        Assignment
        Assignment; statement
    	Expression
        Expression; statement
    
    Declaration:
		"let" name "=" Expression
        "const" name "=" Expression
    
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
		a string of character: must start with an alphabet and can contain only alphabet and digit charachters and '_'
       
        Input comes from cin through the Token_stream called ts.
   */



//------------------------------------------------------------------------------

Token_stream ts;      // provides get(), putback() and ignore() functions

//-------------------------------------------------------------------------------

Symbol_table st; // provides get_value(), set_value(), is_declared(), and define_name function to handle user_defined variables

//-------------------------------------------------------------------------------

double expression();  // read and evaluate an Expression

//-------------------------------------------------------------------------------

void clean_up_mess(){ 
    //handling errors (bad tokens) without terminating program
    cin.clear();        // clear fail state
    ts.ignore(print); // eat all character up to and including print ';'
}

//-----------------------------------------------------------------------------

void help_calculator()
// print out some instructions for how to use the calculator if the user presses the H key (both upper- and lowercase).
{
    cout << "Welcome to the simple calculator application.\n";
    cout << "You can enter expressions using integers.\n\n";

    cout << "Supported operations include:\n";
    cout << " - Addition ('+')\n";
    cout << " - Subtraction ('-')\n";
    cout << " - Multiplication ('*')\n";
    cout << " - Division ('/')\n";
    cout << " - Remainder ('%') using integer values\n";
    cout << " - Grouping using parentheses: (expression)\n\n";

    cout << "You can also declare and assign variables or constants, and perform operations on them.\n";
    cout << "A variable name must start with an alphabet and can only consist of alphanumeric characters\n"
         << " and the underscore '_' character.\nVariable names also can't be keywords.\n"
         << "The following is a list of the keywords: \"let\", \"const\", q, h, and H\n";
    cout << " - To declare a variable:     let \"name\" = \"Expression\"\tex: let x = 10\n";
    cout << " - To declare a constant:     const \"name\" = \"Expression\"\tec: const pi = 3;\n";
    cout << " - To reassign a variable:    name = \"Expression\"\tex: x = 20;\n";
    cout << "   (constants cannot be reassigned)\n\n";
    cout << " - 'ans' is a special variable that stores the result of the last calculation.\n";

    cout << "To display the result, end your expression with a semicolon (';') or press Enter.\n";
    cout << "To quit the calculator, enter 'q'.\n";
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
            if(!st.is_declared(t.name))  // if the variable name already exists in the var_table, produce an error message
                error("undefined variable ", t.name);
            Token t2 = ts.get();

		    if(t2.kind == '=') 
			    error ("expected \"let\" before ", t.name); // '=' requires 'let' before the variable name
		    ts.putback(t2);
            return st.get_value(t.name); // return the value associated with the variable name
        }
    case number: // floating-point literal          
        return narrow<int>(t.value);  // return the number's value and check for overflow/underflow and information loss
    
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
            {
                int i1 = narrow<int>(left); //check for overflow/underflow and information loss
                int i2 = narrow<int>(primary());
                left = narrow<int>(left * i2); // read and evaluate a primary and then multiply
                t = ts.get();
                break;
            }
        case '/':
            {    
                int i1 = narrow<int>(left);
                int i2 = narrow<int>(primary()); 
                if (i2 == 0) error("/:division by zero");
                left = narrow<int>(left/i2); 
                t = ts.get();
                break;
            }
       case '%':
            { 
                int i1 = narrow<int>(left);
                int i2 = narrow<int>(primary()); 
                if (i2 == 0) error("%:divide by zero");
                left = narrow<int>(i1%i2); // compute the floating-point remainder
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
            {
                int i1 = narrow<int>(left);
                int i2 = narrow<int>(term());
                left = narrow<int>(left + i2);    // evaluate Term and add
                t = ts.get();
                break;
            }
        case '-':
            {
                int i1 = narrow<int>(left);
                int i2 = narrow<int>(term());
                left = narrow<int>(left - i2);    // evaluate Term and subtract
                t = ts.get();
                break;
            }
        default:
            ts.putback(t);
            return left;       // finally: no more + or -: return the answer
        }
    }
}

//------------------------------------------------------------------------------

double assignment(string var_name){
    // assign a new value to an existing variable
    if(!st.is_declared(var_name)) error (var_name, " not defined");
    int i1 = narrow<int>(expression()); // read and evaluate the new value
    st.set_value(var_name, i1); // update the value of the variable in var_table
    return i1;
}

//------------------------------------------------------------------------------

double declaration()
// assume we have seen either "let” or "const"
// handle: <name> "=" <expression>
// declare a variable called "name” with the initial value "expression”
{
    Token t = ts.get(); // "let" or "const" token
    Token t2 = ts.get(); 
    string var_name = t2.name; 
    if (t2.kind != name) // if the token doesn't represent a variable name, produce an error message
        error ("name expected in declaration");
    Token t3 = ts.get(); 
    if (t3.kind != '=') 
        error("= missing in declaration of ", var_name); 
    int i1 = narrow<int>(expression()); // read and evaluate the expression after the '='
    bool is_constant = true; // a flag to distinguish between constant declaration and a variable declaration
    if(t.kind == constant){
        st.define_name(var_name,i1,is_constant); // constant name defined in the var_table
        return i1;
    }
    st.define_name(var_name,i1,!is_constant); // define the variable and add its name and associated numerical value to the var_table
    return i1; // return the value associated with the variable
}

//-----------------------------------------------------------------------------------

double statement()
// read and evaluate a statement: either an expression or a declaration
{
    Token t = ts.get(); 
    switch (t.kind) { 
    case let: case constant: // if a statement starts with "let" or "const", it is a declaration statement
        ts.putback(t);
        return declaration();
    case name: // might be an assignment or a regular expression using a variable
    {
        char c = cin.get(); 
        while (isspace(c) && c != '\n') // skip whitespace and newline characters to detect assignment
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
        if(t.kind == help){
            help_calculator(); // print out the instructions
            continue; // start over
        }
    
        ts.putback(t);
        double output = statement(); // evaluate the statement
        cout << result << output << '\n'; 
        st.set_value(anskey,output); //update the value of 'ans' after each operation

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
        bool constant = true;
        st.define_name(anskey,0,!constant); // predifine 'ans' as a non-constant variable in the var_table
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

