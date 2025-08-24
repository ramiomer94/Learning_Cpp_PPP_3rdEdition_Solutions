
#include "PPPheaders.h"


int main(){
    cout << "Enter a set of name-and-value pairs, such as Joe 17 and Barbara 22. Terminate input with 'NoName 0': ";
    vector<int> scores;
    vector<string> names;

    string name;
    int score;
    
    // Continuously read name-score pairs until "NoName 0" is entered
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
    
    // Prompt the user to enter a score to retrieve associated names
    cout << "\nPlease enter a value to retrieve the list of associated names: ";
    int score_value;  
    
    while(cin >> score_value){ // Loop continuously to accept lookup requests

        bool found = false; // Track whether the score was found in the list

        // Search through scores to find matches
        for (int i = 0; i < names.size(); ++i)
            if(score_value == scores[i]) { // If a match is found, display the score and corresponding name
                cout << scores[i] << " --> " << names[i] << '\n';
                found = true;
            }
        
        // If name was not found in the list, inform the user
        if (!found)
            cout << "score not found.\n";
        
         cout << "Please enter a value to retrieve the list of associated names: ";
            
    }
                
    cout << '\n';

}