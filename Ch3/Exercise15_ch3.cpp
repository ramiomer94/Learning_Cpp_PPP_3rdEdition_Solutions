#include "PPPheaders.h"

int main(){
    cout << "Please enter a set of positive integers: ";
    vector<int> p_numbers;

    for(int p_number; cin >> p_number;){
        if (p_number < 0) // Reject negative values and inform the user
            cout << "The value entered is not positive and will be discarded."
                 << "Please enter only positive integers.\n";
        else
            p_numbers.push_back(p_number);
    }
    // Handle the case where no valid input was entered
    if(p_numbers.size() == 0){
        cout << "The list is empty. The program will be terminated!\n";
        return 0;
    }
    
    // Sort the list to make mode calculation easier
    ranges::sort(p_numbers);
    for(int x: p_numbers)
        cout << x << " ";
    cout <<'\n';

    
    int max_n_count = 0; // Highest frequency seen so far
    int current_n_count = 1; // Frequency of the current value being checked (accounts for the initial occurrence)
    vector<int> modes; // // To store one or more modes
    bool multiple_modes = false;
    

    for(int i = 0; i < p_numbers.size()-1; ++i){
        if(p_numbers[i] == p_numbers[i+1])
            ++current_n_count; // Continue counting same value
        else {
            if (max_n_count == current_n_count)
                modes.push_back(p_numbers[i]);
            else if(max_n_count < current_n_count){
                max_n_count = current_n_count; // Update max count
                modes.clear();                 // Clear previous modes
                modes.push_back(p_numbers[i]); // Save new mode
            }
            current_n_count = 1; // Reset counter for the next value
        }
    }

    // Final check for the last element (which isn't handled in the loop)
    if(current_n_count == max_n_count)
        modes.push_back(p_numbers[p_numbers.size()-1]);

    else if(max_n_count < current_n_count){
        max_n_count = current_n_count;
        modes.clear();
        modes.push_back(p_numbers[p_numbers.size()-1]);
    }
    

    if(modes.size() == 1)
        cout << "The mode of the entered set of positive integers is: " << modes[0] << ".\n";
    else {
        cout << "There are multiple modes in the provided set and they are: ";
        for(int x : modes)
            cout << " " << x << " ";  
    }
    cout << '\n';
}