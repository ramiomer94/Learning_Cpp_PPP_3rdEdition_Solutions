#include "PPPheaders.h"

class Token {
    public:
            char kind;
            int value;
            Token(char ch): kind{ch}, value{0}{}
            Token(char ch, int val): kind{ch}, value{val}{}

};

class Token_stream{
    public:
            Token get();
            void putback(Token t);
    private:
            bool full = false;
            Token buffer{'0'};

};

Token Token_stream::get(){

    if(full) {
        full = false;
        return buffer;
    }

    char ch;
    if (!(cin >> ch)) error("no input!");

    switch(ch){

        case '=':
        case 'x':
        case '!': case '^': case '~': case '&': case '|': case '(': case ')':
            return Token{ch};
        
        case '0': case '1': case '2': case '3': case '4': case '5': case '6':
        case '7': case '8': case '9': case '-':
        {
            cin.putback(ch);
            int val = 0;
            cin >> val;
            return Token{'8',val};
        }
        default:
            error("Bad Token!");

    }
}

void Token_stream::putback(Token t){

    if(full) error("putback() into a full buffer");

    buffer = t;
    full = true;
}

Token_stream ts;

int primary();
int unary();
int and_operation();
int xor_operation();
int or_operation();

int or_operation(){

    int left = xor_operation();
    Token t = ts.get();
    
    while (t.kind == '|'){
        left |= xor_operation();
        t = ts.get();
    }

    ts.putback(t);
    return left;
}

int xor_operation(){
    int left = and_operation();
    Token t = ts.get();

    while(t.kind == '^'){

        left ^= and_operation();
        t = ts.get();
    }
    
    ts.putback(t);
    return left;

}


int and_operation(){

    int left = unary();
    Token t = ts.get();

    while(t.kind == '&'){

        left &= unary();
        t = ts.get();        
    }
        
    ts.putback(t);
    return left;        
}


int unary() {
    int not_count = 0;
    int comp_count = 0;
    Token t = ts.get();

    // Count all '!' and '~' operators
    while (t.kind == '!' || t.kind == '~') {
        if (t.kind == '!') ++not_count;
        else ++comp_count;
        t = ts.get();
    }

    ts.putback(t);  // Put back the first non-'!'/'~' token

    int val = primary();  // Evaluate the base value once

    // Apply bitwise complement (~) if odd count
    if (comp_count % 2 != 0)
        val = ~val;

    // Apply logical NOT (!) if odd count
    if (not_count % 2 != 0)
        val = !val;
    
    if(not_count % 2 == 0 && not_count > 0)
        val = !!val;

    return val;
}

int primary(){
    Token t = ts.get();

    switch(t.kind){
        case '(':
        {
            int d = or_operation();
            t = ts.get();
            if(t.kind != ')') error("Expected ')'");
            return d;
        }
        case '8':
            return t.value;

        case 'x':
            ts.putback(t);
            return -1;

        default:
            error("primary expected");   
    }
}


int main() {
try
{
    cout << "Welcome to our simple calculator.\nPlease enter expressions using floating-point numbers.\n";
    cout << "The calculator performs the following operations:\nAddition ('+'), subtraction ('-'), Multiplication ('*')"
         << " and division ('/') of all floating-point numbers."
         << "\nTo ask for the output, end your expression with the '=' sign."
         << "\nTo quit the calculator enter 'x'\n";

    int val = 0;
    while (cin) {
        Token t = ts.get();
        if (t.kind == 'x') break; // 'q' for quit
        if (t.kind == '=')        // ';' for "print now"
            cout << val << '\n';
        else
            ts.putback(t);
        val = or_operation();
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