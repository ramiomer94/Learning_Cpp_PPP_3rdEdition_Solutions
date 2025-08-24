#include "PPPheaders.h"



// Structure to store the result of the print_until_s() function
struct Printing_result{
    int number_printed_strings = 0; // Count of how many strings were printed before stopping
    bool quit_symbol_found = false; // Flag: true if 'quit' symbol was encountered
};

// Function: print_until_s
// Purpose: Print each string from vector 'v' until the string equal to 'quit' is found.
// Returns: Printing_result struct containing number of printed strings and quit detection flag.
Printing_result print_until_s(const vector<string>& v, string quit) 
{
    Printing_result result;
    for (const string& s : v) {
         // If the current string matches the quit string, set the flag and stop
        if (s==quit) { 
            result.quit_symbol_found = true;
            break;
        }
            cout << s << '\n'; 
            ++result.number_printed_strings;  // Increase the count of printed strings
    }
    return result; // Return the result to caller
}

int main(){

    /*
        What does make good test cases:
            - They validate boundary cases
            - They validate common slips vy the used
            - The test the behavior of the function/code against ambiguous inputs
    */
    
        //------------------Test 1---------------------
        // Quit string at the END of the list (should print all before it)
        cout << "--------------------Test 1---------------------\n";
        vector<string> test1 = {"1","abc","12.5 abc", "c", "-asu^","quit"};
        Printing_result r1 = print_until_s(test1,"quit");

        //------------------Test 2---------------------
        // Quit string at the START (should print nothing)
        cout << "--------------------Test 2---------------------\n";
        vector<string> test2 = {"quit", "123", "c", "^&%$@", "abdcol", " asnj 54590 ", "1", "0", "quit"};
         Printing_result r2 = print_until_s(test2,"quit");

        //------------------Test 3---------------------
        // Quit string in the MIDDLE (should print all before it)
        cout << "--------------------Test 3---------------------\n";
        vector<string> test3 = {"123", "c", "^&%$@", "abdcol", "quit", " asnj 54590 ", "1", "0"};
         Printing_result r3 = print_until_s(test3,"quit");

        //------------------Test 4---------------------
        // No quit string present (should print all elements, quit_symbol_found remains false)
        cout << "--------------------Test 4---------------------\n";
        vector<string> test4 = {"abcd", "123", "@#$%", "123ashdsjf%^&^", "c", "1", "2"};
         Printing_result r4 = print_until_s(test4,"quit");

        //------------------Test 5---------------------
        // Empty vector case (should print nothing, quit_symbol_found remains false)
        cout << "--------------------Test 5---------------------\n";
        vector<string> test5 = {};
         Printing_result r5 = print_until_s(test5,"quit");

        //------------------Test 6---------------------
        // Case sensitivity check (Quit != quit)
        cout << "--------------------Test 6---------------------\n";
        vector<string> test6 = {"abc", "123", "@#$#%", "Quit", "quit"};
        Printing_result r6 = print_until_s(test6,"quit");

        //------------------Test 7---------------------
        // Quit with leading/trailing spaces (not an exact match)
        cout << "--------------------Test 7---------------------\n";
        vector<string> test7 = {"abc", "123", "@#$#%", " quit", "quit ", "quit"};
        Printing_result r7 = print_until_s(test7,"quit");

        //------------------Test 8---------------------
        // Quit-like strings but not exact match (ensures exact equality check)
        cout << "--------------------Test 8---------------------\n";
        vector<string> test8 = {"abc", "123", "@#$#%", " quit", "quit ", "quits", "Quit", "quit!", "quit"};
        Printing_result r8 = print_until_s(test8,"quit");

        //------------------Test 9 ---------------------
        // Different quit symbol ("-1"), with quit appearing later
        cout << "--------------------Test 9 ---------------------\n";
        vector<string> test9 = {"quit", "123", "c", "^&%$@", "abdcol", " asnj 54590 ", "1", "10", "quit", "-","-1"};
        Printing_result r9 = print_until_s(test9,"-1");

        //------------------Test 10 ---------------------
        // Multiple occurrences of quit symbol ("-1"), should stop at first
        cout << "--------------------Test 10 ---------------------\n";
        vector<string> test10 = {"quit", "123", "c", "^&%$@", "abdcol", "-1", "-1", " asnj 54590 ", "1", "10", "quit", "-"};
        Printing_result r10 = print_until_s(test10,"-1");

        //------------------Test 11 ---------------------
        // Empty string as quit symbol (stops when encountering "")
        cout << "--------------------Test 11 ---------------------\n";
        vector<string> test11 = {"quit", "123", "c", "^&%$@", "abdcol", "-1", "", "-1", " asnj 54590 ", "1", "10", "quit", "-"};
        Printing_result r11 = print_until_s(test11,"");
        

    return 0;
}