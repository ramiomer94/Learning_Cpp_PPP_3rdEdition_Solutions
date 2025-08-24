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
    
    // Prompt the user to start querying values by name
    cout << "\nEnter a name you wish to retrieve the corresponding value for: ";
    string key_name;  // Name the user wants to look up
    
    
    while(cin >> key_name){ // Loop continuously to accept lookup requests

        bool found = false; // Flag to track if the name was found

        // Search for the input name in the names vector
        for (int i = 0; i < names.size(); ++i)
            if(key_name == names[i]) { // If found, print the corresponding score
                cout << key_name << " ---> " << scores[i] << '\n';
                found = true;
                break;
            }
        
        // If name was not found in the list, inform the user
        if (!found)
            cout << "name not found.\n";
        
         cout << "Enter a name you wish to retrieve the corresponding value for: ";
            
    }
                
    cout << '\n';

}