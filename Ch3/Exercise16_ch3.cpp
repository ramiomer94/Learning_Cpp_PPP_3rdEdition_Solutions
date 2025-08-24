#include "PPPheaders.h"


// a program that finds the min, max, and mode of a sequence of strings.
int main(){

    cout << "Please enter a sequence of strings: ";
    vector<string> sequence;

    for (string n; cin >> n; )
        sequence.push_back(n);

    // Check for empty input
    if(sequence.size() == 0){
        cout << "The user entered an empty list. The program will be terminated.\n";
        return 0;
    }

     // Sort the sequence alphabetically to simplify min, max, and mode finding
    ranges::sort(sequence);

    vector<string> modes; // Stores the most frequent string(s)
    int current_count = 1; // Count of current repeating string
    int max_count = 0; // Max repetition count found so far

     // cycle through the sorted sequence to find modes
    for(int i = 0; i < sequence.size() - 1; ++i){
        if(sequence[i] == sequence[i+1])
            ++current_count; // Increment count for repeating string
        else {// If current string has higher frequency, update mode
            if (max_count == current_count)
                modes.push_back(sequence[i]);
            else if(max_count < current_count){
                max_count = current_count;
                modes.clear(); // Clear previous modes
                modes.push_back(sequence[i]);
            }
            current_count = 1; // Reset count for next new string
        }
    }

      // Check last element group after loop finishes
    if(max_count == current_count)
        modes.push_back(sequence[sequence.size()-1]);
    else if (max_count < current_count){
        max_count = current_count;
        modes.clear();
        modes.push_back(sequence[sequence.size() - 1]);
    }

    // Min and Max strings from sorted sequence
    string min = sequence[0];
    string max = sequence[sequence.size()-1];
    cout << "The Min (alphabatically first) is: " << min
         << "\nThe Max (alphabatically last) is: " << max;
    
    if(modes.size() == 1)
        cout << "\nThe mode (most repeated string) is: " << modes[0];
    else {
        cout << "\nThe modes (most equally repeated strings) are: ";
        for(string x: modes)
            cout << x << " ";
    }
    cout << '\n';
    
    
}