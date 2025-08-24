#include "PPPheaders.h"


// Structure to store the result of the print_until_ss() function
struct Printing_result{
    int number_printed_strings = 0; // Count of how many strings were printed before stopping
    int quit_occurrence_count = 0; // How many times the 'quit' was encountered
    bool found_two_quits = false; // Flag: true if 'quit' symbol was encountered twice
};

// Function: print_until_ss
// Purpose: Prints each string in vector v until the string storing the given quit value
//          is found twice. The quit value itself is never printed.
// Parameters:
//   v->  the vector of strings to process
//   quit -> the quit string to watch for
// Returns:
//   A Printing_result struct with counts and flags describing the outcome
Printing_result print_until_ss(const vector<string>& v, string quit){

    Printing_result result;
    for (const string& s : v) {
         // If the current string matches the quit string, set the flag and stop
        if (s==quit) { 
            ++result.quit_occurrence_count; // Track occurrence

            // If this is the second time we've seen the given quit value
            if(result.quit_occurrence_count == 2) {
                result.found_two_quits = true; // Mark that we found two quits
                break; // Stop processing the vecote 
            }
            continue; // Skip printing this given quit value
        }
            cout << s << '\n'; 
            ++result.number_printed_strings;  // Increase the count of printed strings
    }
    return result; // Return the result to caller
}

// Helper to run a single test case
void run_test(int id, const vector<string>& v, const string& quit) {
    cout << "--------------------Test " << id << "---------------------\n";
    Printing_result r = print_until_ss(v, quit);
    cout << "[Summary] printed=" << r.number_printed_strings
         << ", occ=" << r.quit_occurrence_count
         << ", found_two_quits=" << (r.found_two_quits ? "true" : "false")
         << "\n\n";
}

int main(){
    
    run_test(1,  {"a","quit","b","quit","c"}, "quit");          // Two quits separated by other items
    run_test(2,  {"quit","quit","a"}, "quit");                  // Two quits at start
    run_test(3,  {"a","b","quit"}, "quit");                     // One quit at end
    run_test(4,  {"a","b"}, "quit");                            // No quits at all
    run_test(5,  {}, "quit");                                   // Empty vector  
    run_test(6,  {"a","quit","quit","b"}, "quit");              // Back-to-back quits in middle
    run_test(7,  {"Quit","quit","Q u i t","quit"}, "quit");     // Case and spacing variations
    run_test(8,  {" quit","quit ","quit"}, "quit");             // Whitespace variants
    run_test(9,  {"quits","quit!","quit"}, "quit");             // Punctuation look-alikes
    run_test(10, {"0","-1","1","-1","2"}, "-1");                // Different quit value (-1)
    run_test(11, {"","a","","b"}, "");                          // Empty string as the given quit value
    run_test(12, {"quit"}, "quit");                             // Single element that is quit value

    return 0;
}