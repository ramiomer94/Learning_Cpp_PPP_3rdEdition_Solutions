
/*
	calculator08buggy.cpp

	Revision history:
	Revised by Rami Omer August 3rd
	- Fixed syntax errors
	- Added a cin.clear() to the Token_stream ignore to fix a bug that caused the program to be stuck in an inifinite loop
	when a invalid name is entered
    Revised by Bjarne Stroustrup (bjarne@stroustrup.com) November  2023
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
		Quit: 'Q' or "quit"
		Statement calculation

    Statement:
		Declaration
		Assignment
    	Expression
    
	Declaration:
		"let" name "=" Expression
	
	Assignment:
		"assign" name "=" Expression

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
		sqrt("Expression")
        
    Number:
        floating-point-literal

	Name:
		a string of character: must start with an alphabet and can contain only alphabet and digit charachters
		
       
        Input comes from cin through the Token_stream called ts.
   */


#include "PPPheaders.h"

struct Token {
	char kind;											// what kind of token
	double value;										// for numbers: a value 
	string name;										// for names: a string name
	Token(char ch) :kind{ch}{ }							
	Token(char ch, double val) :kind{ch}, value{val}{ } 
    Token(char ch, string n) :kind{ch}, name{n}{ };
};

//---------------------------------------------------------------------------------------------------------------------------

class Token_stream { // Handles input tokenization: retrieves, buffers, and manages tokens from the input stream
	bool full {false};
	Token buffer = {'0'};
public:
	Token get(); 			// get a token
	void unget(Token t);    // putback a token into the buffer
	void ignore(char c);    // discard all characters from the input stream up to and including character c
};


void Token_stream::unget(Token t)
{
    if (full) error("unget() into a full buffer"); // can't putback a token into a full buffer
    buffer = t;         // copy t to buffer
    full = true;        // buffer is now full
}

constexpr char let = 'L'; 			// declaration token kind
constexpr char quit = 'Q';  		// quit token kind
constexpr char print = ';'; 		// print token kind
constexpr char number = '8';		// number token kind
constexpr char name = 'a';			// name token kind
constexpr char assign = 'i';		// assignment token kind
constexpr char square_root = 'R';	// square_root token kind
constexpr char power = 'P';			// power token kind



Token Token_stream::get()
{ //reads from input stream and tokenizes input into one of the following three categories:
	// 1- a character (operators, paranthesis, print character, quit character)
	// 2- a number
	// 3- a variable name
	if (full) { full = false; return buffer; } 
    
	char ch;
	cin >> ch;		// Skips whitespace
	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%': 			// not used
	case print: 		// ';'
    case quit:			// 'Q' or 'quit'
	case '=':
	case ',':
	case '_':	
		return Token{ch};	// Initilize the token with a character 

	case '.': case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		{	
        	cin.unget(); // put digit back into the input stream
            double val;
            cin >> val;     // read a floating-point number
            if(!cin) error(
				"Invalid Input. Format can't be tokenized. Possible errors: not a number, not a valid variable name");
            return Token{number, val}; // Initialize the token with kind number and the double value of the number  
		}
	default:
		if (isalpha(ch)) { // variable name starts with an alphabet character
			string s;
			s += ch; 
			while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) // variable names can contain alphabets and numerical characters
                s += ch;
			cin.unget(); 
			if (s == "let") return Token{let}; 			// Initialize a variable declaration keyword token; let == 'L'
			if (s == "quit") return Token{quit}; 		// Initialize a quit token; quit == 'Q'
			if (s == "assign") return Token{assign};	// Initialize a assign token; assign == 'i'
			if (s == "sqrt") return Token{square_root}; // Initialize a square root token; square_root == 'R'
			if (s == "pow")	return Token{power};		// Initialize a power token; power == 'P'
			
			return Token{name, s}; 						// Initialize the token with the kind name and a string for the variable name
		}
		error("Bad token: ", string(1, ch) + " (ASCII: " + to_string(int{ch}) + ")");
	}
}

void Token_stream::ignore(char c)
// c represents the kind of Token
// Discard characters from the input stream upto and including character c
{
	if (full && c == buffer.kind) { // retriver character c from the buffer
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin >> ch) // Discard characters up to and including character c
		if (ch == c) return;
}

//---------------------------------------------------------------------------------------------------------------------------

struct Variable { 
	string name;  	// Name that identifies the variable
	double value;	// Hold the value stored in the variable

	Variable(string n, double v) :name{n}, value{v} { }  // Initialize a Variable type with a name that identifies it and a value stored in it
};

vector<Variable> var_table;  // A vector that stores a list of defined variables

double get_value(string s)
// retrieve the value of the variable named 's' in var_table
// Throws an error if the variable is not found
{
	for (const Variable& v : var_table) 
		if (v.name == s) return v.value;
	error("get: undefined name ", s); 
}

void set_value(string s, double d)
// Updates the value of the variable named 's' in var_table to 'd'
// Throws an error if the variable is not found
{
	for (Variable& v : var_table)
		if (v.name == s) { 	
			v.value = d; // update the value
			return;
		}
	error("set: undefined name ", s);
}

bool is_declared(string s)
// checks whether a variable named 's' already exists in var_table
{
	for (const Variable& v: var_table)
		if (v.name == s) return true;
	return false;
}

double define_name(string var, double val) 
// add {var,val} to var_table
{
    if (is_declared(var))
        error(var," declared twice");
    var_table.push_back(Variable{var,val});
    return val;
}

vector<char> symbols = {number, assign, name, let, power, square_root, '=', '_'};
bool is_valid_operator(Token t){
	
	for(const char& x: symbols)
		if(t.kind == x) 
			return false;
	
	return true;		
}

//---------------------------------------------------------------------------------------------------------------------------

Token_stream ts; 	// provides get(), unget(), and ignore(char c)

//---------------------------------------------------------------------------------------------------------------------------

double expression(); // reads and evaluates an Expression

//---------------------------------------------------------------------------------------------------------------------------


double primary()
{ // reads and evaluates a : number (negative and positive), a variable name, and a parenthesized expression ("Expression") 
	Token t = ts.get();
	switch (t.kind) {
	case '(':
	{	double d = expression();
	    t = ts.get();
	    if (t.kind != ')') error("'(' expected");
        return d;
	}
	case '-': // unary minus sign for negative numbers
		return -primary();
	case number:
	{	Token t2 = ts.get();
		if(!is_valid_operator(t2))
			error("invalid expression. Expected an operator or ';'");
		ts.unget(t2);
		return t.value;
	}
	case square_root:
	{	t = ts.get();
		if(t.kind != '(') error("'(' Expected ");
		double d = expression();
		if(d < 0) error("Math error: square root of a negative number is undefined for real numbers.");
		t = ts.get();
		if(t.kind != ')') error("')' Expected ");
		t = ts.get();
		if(!is_valid_operator(t)) 
			error("invalid expression. Expected an operator or ';'");
		ts.unget(t);
		return sqrt(d);
	}
	case power:
	{	t = ts.get();
		if(t.kind != '(') error("'(' Expected ");
		double x = expression();
		t = ts.get();
		if(t.kind != ',') error("',' Expected ");
		int i = narrow<int>(primary());
		t = ts.get();
		if(t.kind != ')') error("')' Expected ");
		t = ts.get();
		if(!is_valid_operator(t)) 
			error("invalid expression. Expected an operator or ';'");
		ts.unget(t);
		return pow(x,i);
	}
	case name: // name == 'a'
    {
		if(!is_declared(t.name)) // t.name represents the name of the variable
            error("undefined variable ", t.name);
		Token t2 = ts.get();
		if(t2.kind == '=') 
			error ("expected assign or let before ", t.name);
		ts.unget(t2);
		return get_value(t.name); // retriver the value stored in the variable t.name
	}
	default:
		ts.unget(t);
		error("primary expected");
	}
}

double term()
{ // reads and evaluates a Term
	double left = primary();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
		{	double d = primary();
		    if (d == 0) error("divide by zero");
		    left /= d;
		    break;
		}
		case '%':
        { 	double d = primary();
            if (d == 0) error("%:divide by zero");
            left = fmod(left,d); 
            break;
        }
		default:
			ts.unget(t);
			return left;
		}
	}
}

double expression()
// reads and evaluates an expression
// it handles addition and subtraction
{
	double left = term();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			ts.unget(t);
			return left;
		}
	}
}

double assignment(){
// Reads and evaluates a variable assignment of the form: "assign <name> = <expression>"
// Assumes the "assign" token has already been consumed
// updates the value of the variable (if it exists in the var_table) with the new expression value
		Token t = ts.get();
		if (t.kind != name) error("name expected in assignment");
		string var_name = t.name;
		if(!is_declared(var_name)) error(var_name, " is not defined"); // if the variable is not in the var_table
		Token t2 = ts.get();
		if(t2.kind != '=') error("= missing in assignment statement of ", var_name);
			
		double d = expression();
		set_value(var_name, d); // update the value of variavle 'var_name' with new value 'd'
		return d;
}
	

double declaration()
// Reads and evaluates a variable declaration of the form: "let <name> = <expression>"
// Assumes the "let" token has already been consumed
// Stores the variable and its value in var_table
{
	Token t = ts.get();
	if (t.kind != name) error("name expected in declaration");
	string var_name = t.name;
	if (is_declared(var_name)) error(var_name, " declared twice");
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of ", var_name);
	double d = expression(); // evaluate the expression after the '=' sign in the declaration statement
	var_table.push_back(Variable{var_name, d}); // store a variable's name and the value it hold in var_table
	return d;
}

double statement()
// read and evaluate a statement: either an expression or a declaration
{
	Token t = ts.get();
	switch (t.kind) {
	case let: // if a statement starts with let, it is a declaration statement
		return declaration();
	case assign:
		return assignment();
	default:
		ts.unget(t);
		return expression();
	}
}

void clean_up_mess()
// Clears any error state from cin and discards all input up to and including ';'
{	
	cin.clear();
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
// Main loop for the calculator:
// Handles reading input, evaluating statements, printing results,
// processing quit commands, and managing exceptions.
{
	while (true) 
    try { //try inside the while-loop catching an error doesn't necessaily terminates the program
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) // print == ';'
			t = ts.get();
		if (t.kind == quit)  	// quit = 'Q'
			return;
		ts.unget(t);

		cout << result << statement() << '\n'; // evaluate statements and print the results;
	}
	catch (exception& e) {
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

int main()
// Runs the calculator and handles any unexpected errors.
try {
	define_name("k",1000);
	calculate();
	return 0;
}
catch (exception& e) {
	// If a standard exception is thrown,its message is displayed, and input is cleared until ';' is found.
	cerr << "exception: " << e.what() << endl;
	char c;
	while (cin >> c && c != ';');
	return 1;
}
catch (...) {
	// If a non-standard or unknown exception is thrown, a generic message is displayed and input is cleared until ';' is found.
	cerr << "exception\n";
	char c;
	while (cin >> c && c != ';');
	return 2;
}

