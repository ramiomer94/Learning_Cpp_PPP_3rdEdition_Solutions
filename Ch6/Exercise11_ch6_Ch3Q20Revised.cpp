/* Question 20 from chapter 3 PPP (2024) is chosen to be revised
   to include the tips, information, and knowledge acquired from chapters 5 & 6.
*/

//-----------------------------------------------------------------------------------------------------------------------------------

/*
    This simple program prompts the user to enter name-score pairs. It stores the names in one list and their corresponding 
    scores in another. Input is terminated by entering the keyword "NoName" followed by the score 0.

    Valid names must consist of alphabetic characters only, excluding the reserved keywords "NoName" and "quit".
    "NoName" is used to signal the end of input, and "quit" is used to exit the program at any time.

    In the second phase, the program prompts the user to enter a score. It then searches for and prints all names associated 
    with that score. Valid scores are integer literals. The keyword "quit" can also be used during this phase to exit the program.
*/

//-----------------------------------------------------------------------------------------------------------------------------------

/*
Grammar for a simple program that records name-score pairs:

Input:
    PairList

PairList:
    Pair
    Pair PairList
    Terminate

Pair:
    Name Value

Name:
    alphabetic string (e.g., "Joe", "Barbara")

Value:
    integer (e.g., 17, 22)

Terminate:
    NoName 0

Rules:
- Each Name must be unique.
- Each Pair is printed as (Name, Value) after input ends.
- If a duplicate Name is entered, display an error and exit.
*/

//-----------------------------------------------------------------------------------------------------------------------------------

#include "PPPheaders.h"


class Token{
    public:
        char kind;                                      // what kind of token the input represents
        string name;                                    // for a token representing a name : a name
        int value;                                      // for a token representing a score: a value
        Token(char k): kind{k}{ };                      // Initialize kind of token with a char for a defined symbol 
        Token(char k, string n): kind{k}, name{n}{ };   // Initialize kind and name for the name side of the pair
        Token(char k, int val): kind{k}, value{val}{ }; // Initialize kind and value for the score side of the pair

};

//-----------------------------------------------------------------------------------------------------------------------------------

class Pair_list
// Provides utilities to manage name-score pairs using two parallel lists
{ 
    public:
            void add_name(string name);             // add a name to the names list
            void add_score(int val);                // add a value to the scores list        
            vector<string> get_name(int val);       // returns all names associated with the given score
            int get_score(string name);             // returns the score associated with the given name
            void print_pair_list();                 // prints all stored name-score pairs
    private:
            vector<string>names;                    // a list that stores names entered by the user
            vector<int>scores;                      // a list that stores scores entered by the user
};

void Pair_list::add_name(string name){ 
    if(name == "NoName") error(name, "reserved keyword");   
    for(const string& n : names){ // Search the names list for a duplicate name. 
        if (n == name) error("duplicate names detected"); // If a duplicate is found, throw an error.
    }
    names.push_back(name); // Add the unique input name to th names list
}

void Pair_list::add_score(int val){
    //add the input score to the scores list
    scores.push_back(val); 
}

vector<string> Pair_list::get_name(int val)
//return a vector with all the names associated with a given score
{
    vector<string>eq_val_names;

    // Search for all indices where the score matches 'val' and collect the corresponding names.
    for(int i = 0; i < scores.size(); ++i){ 
        if(scores[i] == val) 
            eq_val_names.push_back(names[i]);
    }
    if(eq_val_names.empty())
        error(to_string(val)," score not found in the scores list"); // If no names are found for the given score, throw an error.
    return eq_val_names;
}

int Pair_list::get_score(string name){

    // Search for all indices where the name in the list matches the given 'name' and collect the corresponding score.
    for(int i = 0; i < names.size(); ++i)
        if(name == names[i])
            return scores[i];
    error(name," not found in the names list"); // If no name is found, throw an error.
}

void Pair_list::print_pair_list(){
    //print all the name-value pairs. 
    //One pair per line in this format: (name,score) new line (name, score).....
    for(int i = 0; i < names.size(); ++i)
        cout << '(' << names[i] << ", " << scores[i] << ")\n"; 
}

//-----------------------------------------------------------------------------------------------------------------------------------

class Token_stream{
    public:
            Token get();            // reads input and returns the next Token
            void putback(Token t);  // puts a token back into the stream buffer
            void ignore (char c);   // discards input characters until (and including) a given character 'c'
    private:
            Token buffer{'0'};      // buffer to store a Token that has been put back
            bool full = false;      // indicates whether the buffer is currently full
};

void Token_stream::putback(Token t){
    if(full) error("putback() into a full buffer"); 

    buffer = t;     // copy t to buffer
    full = true;    // buffer is full
}

constexpr char number = '8';                //t.kind == number means the token represents a numeric value
constexpr char name = 'a';                  //t.kind == name means the token represnets a name (an identifier)  
constexpr char terminator_n = 'T';          //t.kind == terminator_n means the token representes the special terminator name token
constexpr char newline = '\n';              
constexpr char quit = 'Q';                  //t.kind == quit means t represents the quit command
const string terminator_name = "NoName";    // Reserved name used to terminate input    
const string quit_app = "quit";             // String command to quit the program
const int terminator_value = 0;             // Value associated with the terminator token


Token Token_stream::get()
// tokenizes the input stream and retrieves the next token
{
    if(full){ // if the buffer is full, retrive the token from the buffer
        full = false; // the buffer is now empty
        return buffer;
    }

    char ch;
    cin.get(ch);
    while(isspace(ch)) // skips whitespace (space, newline, tab, etc.)
        cin.get(ch);

    if(isdigit(ch)){ //if the first character is a digit
        cin.putback(ch); // put digit back into the input stream
        int val;
        cin >> val; // read an integer number
        if(!cin) error("int expected");
        return Token{number, val}; // return a token representing a numeric literal with value 'val'
    } 
    else if(isalpha(ch))
    // handles variable names and keywords
    // a name must start with an alphabetic character
    { 
        string pair_name; 
        pair_name += ch;
        while(cin.get(ch) && isalpha(ch)) // Continue reading while the characters are alphabetic
            pair_name +=ch;
        cin.putback(ch); //put back the first non-alphabetic character into the input stream

        if(pair_name == quit_app) return Token{quit};                   // return a token representing the "quit" command
        if(pair_name == terminator_name) return Token{terminator_n};    // return a token representing the keyword "NoName"
        return Token{name,pair_name};                                   // return a token representing a name
    }
    else
        error("Bad Token ", string(1,ch));
}

void Token_stream::ignore(char c)
// Skip input until a token of kind 'c' is found
{
    if(full && buffer.kind == c){ // first look in buffer
            full = false;
            return;
    }
    full = false; // Discard the buffered token if not matching

    // now search input:
    char ch;
    while(cin.get(ch)) // discard characters until we find 'c'
        if(ch == c) return;
}

//-----------------------------------------------------------------------------------------------------------------------------------

Token_stream ts;     // Token_stream instance: provides get(), putback(), and ignore() for processing input tokens

//-----------------------------------------------------------------------------------------------------------------------------------

Pair_list pl;       // Pair_list instance: manages the name-score pairs and provides utility functions for access and display           

//-----------------------------------------------------------------------------------------------------------------------------------

void clean_up_mess()
//handling errors (bad tokens) without terminating program
{
    cin.clear();            // clear fail state
    ts.ignore(newline);     // eat all character up to and including print '\n'
}

//-----------------------------------------------------------------------------------------------------------------------------------

constexpr char prompt = '>';
constexpr int quit_program = 0; // Command to indicate program termination
constexpr int print_result = 1; // Command to indicate result should be printed

int collect_pairs()
// read name-value pairs from input
// store names in the 'names' list and scores in the 'scores' list
{
    // prompt the user
    cout << "Enter a set of name-and-value pairs, such as Joe 17 and Barbara 22. Terminate input with 'NoName 0'\n";
    while (true) {
        try {
            cout << prompt;
            Token t1 = ts.get(); // get next token
            switch(t1.kind){
                case terminator_n: // token representing the keyword "NoName"
                    {   
                        Token t2 = ts.get();  
                        // if the next token is 0, terminate collection and proceed to print                                    
                        if(t2.kind == number && t2.value == 0) return print_result;
                        error("Expected 0 after NoName");
                    }
                case name: // token representing a valid identifier (name)
                {
                    pl.add_name(t1.name);                       // add the name to names list
                    Token t2 = ts.get();                        // expect a number token next
                    if(t2.kind == quit) return quit_program;    // early quit
                    if(t2.kind != number)                       
                    error("Expected integer value after name");
                    pl.add_score(t2.value);                     // add the numeric value to the 'scores' list
                    break;
                }
                case quit:  // // Quit command
                    return quit_program;                  
                default: 
                    error("Expected an alphabetical name or 'NoName'");
            }
        } catch(exception& e) { 
            cerr << e.what() << '\n'; 
            clean_up_mess(); // ignore input until next prompt
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------

int search_and_print()
// prompt the user for a score (integer literal)
// Searches and prints all names associated with that score
{
    cout << "Please enter a value to retrieve the list of associated names:\n";
    while(true){
        try{
            cout << prompt;
            Token t1 = ts.get(); // get next token
            switch(t1.kind){
                case number: // token represents a valid numeric value
                    {
                        // print the results
                        for(string n : pl.get_name(t1.value))
                            cout << n << " ";
                        cout << '\n';
                        break;
                    }
                case quit: // quit command
                    return quit_program; 
                default: 
                    error("int number expected");
            }
        
        }catch(exception& e){
            cerr << e.what() << '\n'; 
            clean_up_mess();
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------

int main() {

    try {
        // Phase 1: Collect name-score pairs
        int collect_result = collect_pairs(); 

         // If user entered "NoName 0", print all collected pairs
        if(collect_result == print_result) 
            pl.print_pair_list();
        // if collect returns quit_program (0), quit the program
        else if (collect_result == quit_program){
            return 0; 
        }
        
        // Phase 2: Search for names based on score
        int search_and_print_result = search_and_print();                            
        if(search_and_print_result == quit_program) return 0;

        return 0;

    } catch(exception& e) {
        cerr << e.what() << '\n'; 
        return 1;
    } catch(...) {
        cerr << "exception \n";
        return 2; 
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------
