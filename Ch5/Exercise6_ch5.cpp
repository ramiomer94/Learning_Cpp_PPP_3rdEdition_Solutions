#include "PPPheaders.h"


 // Vector to store the secret 4-digit sequence (no repeating digits)
vector<char> sequence_to_guess = {'r', 'a', 'm', 'i'}; //The sequence is hard-coded

class Token{
    public:
            char guess;
            bool isduplicate;
            Token(char ch):guess{ch}{};
};

class Token_stream{
    public:
            Token get(char letter);
            void add_token(Token t);
            bool isduplicate(Token t);
            void clear();
    private:
            bool full = false;
            Token buffer = {'0'};
            vector<Token> sequence;
};

Token Token_stream::get(char letter){

    switch(letter){
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
            return Token{char(letter + 32)};
        
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z': case '|':
            return Token{letter};
        
        default:
            error("Expected a letter for your guess or '|' to quit the game");
        
    }
}


void Token_stream::add_token(Token t){
    if (sequence.size() > 4) error("Invalid input size. Expected input size 4.\n");
    sequence.push_back(t);
}

bool Token_stream::isduplicate(Token t){
    bool isduplicate = false;

    if(sequence.size() == 0) return isduplicate;

    for(int i = 0; i < sequence.size(); ++i){
        if(t.guess == sequence[i].guess){
            isduplicate = true;
            break;
        }
    }
    return isduplicate;
}

void Token_stream::clear(){
    sequence.clear();
}


vector<int> players_guess(string guess);

Token_stream ts;

int main(){

    try {
        cout << "Welcome to Bulls and Cows!\n";
        cout << "I have selected a 4-letter string. The letters are unique and case insensitive.\n";
        cout << "Your job is to guess the string. After each guess, I'll tell you:\n";
        cout << " - How many letters are correct and in the correct position (bulls), and\n";
        cout << " - How many letters are correct but in the wrong position (cows).\n";
        cout << "Enter four letters (no repeats) like this: abcd\n";
        cout << "Let's begin!\n\n";
        cout << "Your guess: ";

        string input;
        while (cin >> input) {
            

            if(input == "|") return 0;
            vector<int> response = players_guess(input);
            int bulls = response[0];
            int cows = response[1];

            // Clear sequence after each guess
            ts.clear();

            if (bulls == -1 && cows == -1) {
                cout << "Invalid guess: do not use repeated letters!\n";
                continue;
            }

            // Print bulls and cows
            if (bulls == 1)
                cout << bulls << " bull and ";
            else if (bulls != 4)
                cout << bulls << " bulls and ";

            if (cows == 1)
                cout << cows << " cow.\n";
            else {
                if (bulls != 4)
                    cout << cows << " cows.\n";
            }

            if (bulls == 4) {
                cout << "CONGRATS!! YOU GUESSED CORRECT:\nThe computer generated 4-letter string is: ";
                for (char x : sequence_to_guess)
                    cout << x;
                cout << '\n';
                break;
            }
            cout << "Your guess: ";
        }

    } catch (exception& e) {
        cerr << "EXCEPTION CAUGHT: " << e.what() << '\n';
    }
}


vector<int> players_guess(string guess){
    vector<int> cows_and_bulls(2, 0);
    int cows = 0;
    int bulls = 0;

    if (guess.size() != 4) error("Expected a 4-letter string!");

    ts.clear();  // clear previous state

    for (int i = 0; i < 4; ++i) {
        char ch = guess[i];
        Token t = ts.get(ch);
        if (ts.isduplicate(t)) {
            return {-1, -1};  // Duplicate letter
        }

        ts.add_token(t);

        if (ch == sequence_to_guess[i]) {
            ++bulls;
        } else {
            for (int j = 0; j < sequence_to_guess.size(); ++j) {
                if (i != j && ch == sequence_to_guess[j]) {
                    ++cows;
                    break;
                }
            }
        }
    }

    cows_and_bulls[0] = bulls;
    cows_and_bulls[1] = cows;
    return cows_and_bulls;
}