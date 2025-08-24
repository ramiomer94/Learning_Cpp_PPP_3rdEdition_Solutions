#include "PPPheaders.h"

int main(){

    try {
        cout << "Please enter the number of values you want to sum: ";
        int N;
        cin >> N;

        //Check if 'cin' was corrupted (e.g., non-integer input)
        if(!cin) error("Expected an int value. Input is invalid or corrupted.\n");
        if(N <= 0) error("Expected N a positive integer.\n");    
            cout << "Please enter some integers (press '|' to stop): ";

            string input; // Temporarily hold input as a string
            vector<int> integer_numbers; // Store validated integers

            while(cin >> input){ // Loop to read inputs until the user types '|'

                if (input == "|") break;
                int start = 0;
                if (input[0] == '-' || input[0] == '+')  // To handle optional '+' or '-' signs
                    start = 1;
                for (int i = start; i < input.size(); ++i) // Check that all remaining characters are digits
                    if (!isdigit(input[i]))
                        error("Expected input of type int: '" + input + "'");

            // If input passed validation, convert it to int and store
            int integer_number = stoi(input);
            integer_numbers.push_back(integer_number);
            }
            
             // Error if the vector is empty (no numbers were entered)
            if(integer_numbers.size() == 0) error("No values to calculate the running sum for. List of integers entered is empty.\n");
            
            // Error if user asked for more values to sum than were entered
            if(N > integer_numbers.size()) error("OUT OF BOUND ERROR: Expected N smaller or equal to the size of the list of numbers entered.\n");
            
            int sum_first_N = 0;
            
            for(int i = 0; i < N; ++i)
                sum_first_N += integer_numbers[i];
            
            cout << "The sum of the first " << N << " numbers (";
            for(int i = 0; i < N; ++i)
                cout << integer_numbers[i] << " ";
            cout << ") is " << sum_first_N << '\n';
            

    } catch (exception& e){
        // Handle any thrown errors and output message
        cerr << "ERROR CAUGHT: " << e.what() << '\n';
    }
    
   
}