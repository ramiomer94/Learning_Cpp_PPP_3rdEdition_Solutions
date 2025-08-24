#include "PPPheaders.h"

class Token{
    public:
             char x;
            Token(char ch): x{ch}{};
};

int main(){

    char guess = 'A';
    Token t = {guess};
    Token x = {char(guess + 32)};

    cout << t.x << " " << x.x << '\n';
}