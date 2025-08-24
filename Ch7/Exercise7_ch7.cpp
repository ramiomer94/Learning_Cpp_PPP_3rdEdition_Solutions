#include "PPPheaders.h"


//--------------------------------------------
// Helper function: ignore_input()
//--------------------------------------------
/*
 * Purpose:
 *  - Reads and discards characters from the input buffer until a newline '\n' is found.
 * 
 * Usage:
 *  - Used when input fails or an error occurs to "flush" remaining unwanted characters.
 */
void ignore_input(){
    
    char c = '0';
    while(cin.get(c)){
        if (c == '\n') return;
    }
}

//--------------------------------------------
// Helper function: clean_up_mess()
//--------------------------------------------
/*
 * Purpose:
 *  - Resets the input stream state and clears any leftover characters from the input buffer.
 * 
 * Steps:
 *  1. `cin.clear()` → clears the failbit/badbit so further I/O can happen.
 *  2. Calls ignore_input() to discard remaining junk input.
 */
void clean_up_mess(){
    cin.clear();
    ignore_input();
}


//--------------------------------------------
// Function: collect_names()
//--------------------------------------------
/*
 * Purpose:
 *  - Prompts the user to enter names (alphabetic only) for 5 people.
 *  - Ensures no duplicate names.
 * 
 * Behavior:
 *  - Reads names character-by-character, skipping whitespace.
 *  - Stops reading each name once a non-alphabetic character (or whitespace after name) is found.
 *  - Throws an error if:
 *      • Name contains non-alphabetic characters (other than separating space)
 *      • Duplicate names are entered
 * 
 * Returns:
 *  - vector<string> containing exactly 5 names.
 */
vector<string> collect_names (){

    cout << "Enter the names of 5 people: ";
    vector<string> names;
    while(names.size() < 5){
        char c;
        cin.get(c);
        while(isspace(c)) // Skip leading spaces/newlines
            cin.get(c);
        cin.putback(c); // Put the first letter back so it can be read in the next loop
        string s;
        while(cin.get(c) && isalpha(c)) // Collect alphabetic characters into a string
            s += c;
        if(!isspace(c)) error("expected a string of alphabetic characters");

        // Check for duplicate name
        for(const string& n : names){
            if(s == n) error("duplicates not allowed");
        }

        names.push_back(s);
    }
    
    return names;
}

//--------------------------------------------
// Function: collect_ages()
//--------------------------------------------
/*
 * Purpose:
 *  - Prompts the user to enter an age for each name collected.
 *  - Ensures age is positive and non-zero.
 * 
 * Parameters:
 *  - names: vector<string> containing names for which to collect ages.
 * 
 * Returns:
 *  - vector<double> containing corresponding ages.
 */
vector<double> collect_ages(const vector<string>& names){

    cout << "For each name shown please enter the corresponding age.\n";
    double a;
    vector<double> ages;
    for(const string& n: names){
        cout << n << ": ";
        cin >> a;
        if(a <= 0) error("age can't be zero or negative");
        ages.push_back(a);
    }

    return ages;
}

//--------------------------------------------
// Function: print()
//--------------------------------------------
/*
 * Purpose:
 *  - Displays the list of name-age pairs.
 * 
 * Parameters:
 *  - label: A string to print before the list.
 *  - names: vector<string> of names.
 *  - ages: vector<double> of ages.
 */
void print(const string& label, const vector<string>& names, const vector<double>& ages){

    cout << label << '\n';
    for(int i = 0; i < names.size(); ++i){
        cout << '(' << names[i] << ',' << ages[i] << ")\n";
    }
}

//--------------------------------------------
// Function: sort_age()
//--------------------------------------------
/*
 * Purpose:
 *  - After sorting names alphabetically, reorders ages so each age matches its name.
 * 
 * Parameters:
 *  - names_before_sorting: Original order of names before sorting.
 *  - names: Sorted names.
 *  - ages: Ages corresponding to names_before_sorting.
 * 
 * Returns:
 *  - vector<double> containing ages rearranged to match the sorted names.
 */
vector<double> sort_age(const vector<string>& names_before_sorting, const vector<string>& names, const vector<double>& ages){

    vector<double> ages_after_sorting;
    for(int i = 0; i < names.size(); ++i){
        for(int j = 0; j < names.size(); ++j){
            if(names[i] == names_before_sorting[j]){
                ages_after_sorting.push_back(ages[j]);
                break;
            }
        }
    }
    return ages_after_sorting;
}

//--------------------------------------------
// Function: run()
//--------------------------------------------
/*
 * Purpose:
 *  - Main loop for collecting names, ages, printing, and sorting.
 *  - Uses exception handling for input validation and cleanup.
 * 
 * Behavior:
 *  - Runs indefinitely until program termination.
 */
void run(){

    while(true){
        try{
            vector<string> name = collect_names();
            vector<double> age = collect_ages(name);
            print("Name-age pairs before sorting", name, age);
            vector<string> names_before_sorting = name;
            // Sort names alphabetically
            sort(name.begin(),name.end());
            // Reorder ages to match sorted names
            vector<double> age_after_sorting = sort_age(names_before_sorting, name, age);
            print("Name-age pairs after sorting", name, age_after_sorting);

        } catch(exception& e){
            cerr << e.what() << '\n';
            clean_up_mess(); // Reset stream and discard bad input
        }
    }
}

int main(){
    try{
        run();
    } catch(exception& e){
        cerr << e.what() << '\n';
    } catch(...){
        cerr << "Exception caught\n";
    }
}