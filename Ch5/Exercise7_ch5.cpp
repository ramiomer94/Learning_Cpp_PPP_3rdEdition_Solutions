#include "PPPheaders.h"

class Token{
    public:
            char kind;
            int value;
            Token(char k): kind{k}{};
            Token(char k, int val): kind{k},value{val}{};

};

class Token_stream{
    public:
            Token get();
            
};

Token Token_stream::get(){
    char digit;
    cin >> digit;

    switch(digit){
        case '0': case '1': case '2': case '3': case '4': case '5': 
        case '6': case '7': case '8': case '9': case '-': case '+':
            {
                int d = 0;
                cin.putback(digit);
                cin >> d;
                return Token{'8',d};
            } 
        case 'x':
            return Token{digit};
        default:
            error("Expected a digit, a '-', a '+', or 'x' to quit the program");
    }

}

Token_stream ts;

int main(){

    try{
        while(true){
            cout << "Enter an integer between -9999 and 9999: ";
            Token t = ts.get();

            if(t.kind == 'x') return 0;
            if(t.kind == '8' && (t.value < -9999 || t.value > 9999)) error("Expected an integer between [-9999, 9999]");

            if(t.value == 0){
                cout << t.value << " 0 thousands 0 hundereds 0 tens 0 ones\n";
                continue;
            } 
            cout << t.value << " is ";
            int thousands = abs(t.value/1000);
            int hundereds = abs((t.value%1000)/100);
            int tens = abs((t.value%100)/10);
            int ones = abs(t.value%10);

            if (thousands == 0);
            else if(thousands == 1) cout << thousands << " thousand ";
            else cout << thousands << " thousands ";

            if (hundereds == 0);
            else if(hundereds == 1) cout << hundereds << " hundered ";
            else cout << hundereds << " hundereds ";

            if (tens == 0);
            else if(tens == 1) cout << tens << " ten ";
            else cout << tens << " tens ";

            if (ones == 0) cout << '\n';
            else if(ones == 1) cout << ones << " one\n";
            else cout << ones << " ones.\n";
        }


    }catch(exception& e){
        cerr << "Error: " << e.what()<< '\n';
    }


}