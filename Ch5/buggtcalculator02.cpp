
//
// This is example code from Chapter 6.7 "Trying the second version" of
// "Software - Principles and Practice using C++" by Bjarne Stroustrup
//

/*
    This file is known as calculator02buggy.cpp

    I have inserted 5 errors that should cause this not to compile
    I have inserted 3 logic errors that should cause the program to give wrong results

    First try to find an remove the bugs without looking in the book.
    If that gets tedious, compare the code to that in the book (or posted source code)

    Happy hunting!

*/

#include "PPPheaders.h"

//------------------------------------------------------------------------------

class Token{
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value 
    Token(char ch)    // make a Token from a char
        :kind{ch}, value{0} { }
    Token(char ch, double val)     // make a Token from a char and a double
        :kind{ch}, value{val} { }
};

//------------------------------------------------------------------------------

class Token_stream {
public:
  
    Token get();      // get a Token (get() is defined elsewhere)
    void putback(Token t);    // put a Token back
private:
    bool full = false;        // is there a Token in the buffer?
    Token buffer = {'0'};     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:


//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;       // copy t to buffer
    full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

Token Token_stream::get()
{
    if (full) {       // do we already have a Token ready?
        // remove token from buffer
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)

    switch (ch) {
    case '=':    // for "print"
    case 'q':    // for "quit"
    case '(': case ')': case '+': case '-': case '*': case '/': case '{': case '}': case '!':
        return Token(ch);        // let each character represent itself
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
        cin.putback(ch);         // put digit back into the input stream
        double val;
        cin >> val;              // read a floating-point number
        return Token('8', val);   // let '8' represent "a number"
    }
    default:
        error("Bad token");
    }
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback() 

//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()

//------------------------------------------------------------------------------

double term();

// deal with numbers and parentheses
double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '(':    // handle '(' expression ')'
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected)");
        return d;
    }
    case '{':
    {
        double d = expression();
        t = ts.get();
        if(t.kind != '}') error ("'}' expected)");
        return d;
    }
    case '8':            // we use '8' to represent a number
        return t.value;  // return the number's value
    case 'x':
        ts.putback(t);
        return -1;
    default:
        error("primary expected");
    }
}

//------------------------------------------------------------------------------
double factorial() {

    double left = primary();
    Token t = ts.get();

    while (true) {
        if (t.kind != '!') {
            ts.putback(t);
            return left;
        }

        int n = static_cast<int>(left);
        if (n == 0) {
            left = 1;
        } else {
            int result = 1;
            for (int i = 1; i <= n; ++i)
                result *= i;
            left = result;
        }

        t = ts.get();
    }

    //return left; // just in case
}

// deal with *, /, and %
double term()
{
    double left = factorial();
    Token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
        case '*':
            left *= factorial();
            t = ts.get();
            break;
        case '/':
        {
            double d = factorial();
            if (d == 0) error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);     // put t back into the token stream
            return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
    double left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token from token stream

    while (true) {
        switch (t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term();    // evaluate Term and subtract
            t = ts.get();
            break;
        default:
            ts.putback(t);     // put t back into the token stream
            return left;       // finally: no more + or -: return the answer
        }
    }
}

//------------------------------------------------------------------------------

int main() {
try
{
    cout << "Welcome to our simple calculator.\nPlease enter expressions using floating-point numbers.\n";
    cout << "The calculator performs the following operations:\nAddition ('+'), subtraction ('-'), Multiplication ('*')"
         << " and division ('/') of all floating-point numbers."
         << "\nTo ask for the output, end your expression with the '=' sign."
         << "\nTo quit the calculator enter 'x'\n";

    double val = 0.0;
    while (cin) {
        Token t = ts.get();
        if (t.kind == 'q') return 0; // 'q' for quit
        if (t.kind == '=')        // ';' for "print now"
            cout << val << '\n';
        else
            ts.putback(t);
        val = expression();
    }
    
}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n";
    return 2;
}
}
    

//------------------------------------------------------------------------------