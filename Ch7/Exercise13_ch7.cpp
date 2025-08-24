#include "PPPheaders.h"

//------------------------------------------------------
// Function: string_length
// Purpose: Return the number of characters in a string.
//------------------------------------------------------
int string_length(const string& s){
    return s.size();
}

//------------------------------------------------------
// Function: find_lengths
// Purpose: Given a vector of strings, return a vector
//          containing the length of each string.
// Parameters:
//   v - vector of strings
// Returns:
//   vector<int> where each element is the length of
//   the corresponding string in v.
//------------------------------------------------------
vector<int> find_lengths(const vector<string>& v){
    vector<int> lengths;
    for(const string& s : v)
        lengths.push_back(string_length(s));
    
        return lengths;
}

//------------------------------------------------------
// Function: shortest_string
// Purpose: Find and return the shortest string in v.
//------------------------------------------------------
string shortest_string (const vector<string>& v){
    int shortest = string_length(v[0]); // store length of first string
    string shortest_string = v[0];      // store first string as current shortest
    for(const string& s : v)
        if(string_length(s) < shortest){
            shortest_string = s;
            shortest = string_length(s); // update shortest length tracker
        }
    return shortest_string;
}

//------------------------------------------------------
// Function: longest_string
// Purpose: Find and return the longest string in v.
//------------------------------------------------------
string longest_string (const vector<string>& v){
    int longest = string_length(v[0]);
    string longest_string = v[0];
    for(const string& s : v)
        if(string_length(s) > longest){
            longest_string = s;
            longest = string_length(s); // update longest length tracker
        }
    return longest_string;
}

//------------------------------------------------------
// Function: first_string
// Purpose: Return the lexicographically first string
//          from the vector.
// Implementation:
//   - Takes v by value so original vector is not modified
//   - Sorts the copy and returns the first element
//------------------------------------------------------
string first_string (vector<string> v){
    sort(v.begin(), v.end());
    return v.front();
}

//------------------------------------------------------
// Function: last_string
// Purpose: Return the lexicographically last string
//          from the vector.
// Implementation:
//   - Takes v by value so original vector is not modified
//   - Sorts the copy and returns the last element
//------------------------------------------------------
string last_string(vector<string> v){
    sort(v.begin(), v.end());
    return v.back();
}

//------------------------------------------------------
// Function: run_test
// Purpose: Display test output for the given vector.
//   1. Prints each string and its length
//   2. Prints shortest, longest, lexicographically first, last
//------------------------------------------------------
void run_test(int id, const vector<string>& v){
    cout << "--------------------------Test " << id << "------------------------------\n";
    vector<int> lengths = find_lengths(v);

    // Print each string with its length
    for (int i = 0; i < v.size(); ++i)
    cout << v[i] << " has " << lengths[i] << " characters\n";

    cout << "The shortest string is " << shortest_string(v)
         << "\nThe longest string is " << longest_string(v)
         << "\nThe lexicographically first string is " << first_string(v)
         << "\nThe lexicographically last string is " << last_string(v);
}
