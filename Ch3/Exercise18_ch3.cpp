#include "PPPheaders.h"

/* 
A program where you first enter a set of name-and-value pairs,
such as Joe 17 and Barbara 22. For each pair, add the name to a vector called names
and the number to a vector called scores (in corresponding positions, so that 
if names[7]=="Joe" then scores[7]==17). Terminate input with NoName 0. 
Check that each name is unique and terminate with an error message if a name is entered twice.
Write out all the (name,score) pairs, one per line. 
*/


int main(){
    cout << "Enter a set of name-and-value pairs, such as Joe 17 and Barbara 22. Terminate input with 'NoName 0': ";
    vector<int> scores;
    vector<string> names;

    string name;
    int score;
    
    // Read input continuously until the termination condition is met
    while (cin >> name >> score){
        // Special condition to end the loop
        if(name == "NoName" && score == 0)
            break;
        else {
            // Special case for the first entry: no need to check for duplicates
            if (names.size() == 0) {
                names.push_back(name);
                scores.push_back(score);
            } else { // Check for duplicate name entries before adding
                for (int i = 0; i < names.size(); ++i) 
                    if (name == names[i]) {
                        cout << "ERROR! The program was terminated due to the inputting of two identical names.\n";
                        return -1; // Terminate with error code
                    }
                // If name is unique, store the name and score
                names.push_back(name);
                scores.push_back(score);                
            }                         
        }
    }
    cout << '\n';

    // After successful input, print all (name, score) pairs
    for (int i = 0; i < names.size(); ++i)
        cout << "(\"" << names[i] << "\", " << scores[i] << ")\n";
                
    cout << '\n';

}