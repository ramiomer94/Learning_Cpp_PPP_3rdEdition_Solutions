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
        case '7': case '8': case '9':
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

int unary(){
    int left = 0;
    Token t = ts.get();
    while (true){
        switch(t.kind){
            case '~':
                left = ~primary();
                t = ts.get();
                break;
        
            case '!':
                left = !primary();
                t = ts.get();
                break;

            default:
                ts.putback(t);
                return left;
        }
    }

}

int primary(){
    Token t = ts.get();
    
    switch(t.kind){
        case '(':
        {
            int x = or_operation();
            t = ts.get();
            if(t.kind != ')') error("Expected ')'");
            return x;
        }
        case '8':
            return t.value;
        case 'x':
            ts.putback(t);
            break;
        default:
            error("primary expected");
        
    }

}

int main(){

    try{
        cout << "Welcome to our simple bitwise calculator.\nPlease enter expressions using integer numbers.\n";
        cout << "The calculator performs the following operations:\nAnd ('&'), OR ('|'), XOR('^')"
             << " negation ('!') and complement ('~') of all integer numbers."
             << "\nTo ask for the output, end your expression with the '=' sign."
             << "\nTo quit the calculator enter 'x'\n";

        int output = 0;
        while(cin){
            Token t = ts.get();
            if (t.kind == 'x' ) return 0;
            if (t.kind == '=')
                cout << output << '\n';
            else{
                ts.putback(t);
                output = or_operation();
            }

        }
    }catch(exception& e){
        cerr << "Error: " << e.what() << '\n';
    }
}