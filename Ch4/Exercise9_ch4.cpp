#include "PPPheaders.h"

//Exercise 8 code modified to to write out an error if the result cannot be represented as an int.

int main(){

    try {
        cout << "Please enter the number of values you want to sum: ";
        int N;
        cin >> N;

        if(!cin) error("Expected an int value. Input is invalid or corrupted.");
        if(N <= 0) error("Expected N a positive integer.");    
            cout << "Please enter some integers (press '|' to stop): ";
            string input;
            vector<int> integer_numbers;

            while(cin >> input){

                if (input == "|") break;
                int start = 0;
                if (input[0] == '-' || input[0] == '+')
                    start = 1;
                for (int i = start; i < input.size(); ++i)
                    if (!isdigit(input[i]))
                        error("Expected input of type int: '" + input + "'");

            int integer_number = stoi(input);
            integer_numbers.push_back(integer_number);
            }
            
            if(integer_numbers.size() == 0)
                error("No values to calculate the running sum for. List of integers entered is empty.\n");
            
            if(N > integer_numbers.size())
                error("OUT OF BOUND ERROR: Expected N smaller or equal to the size of the list of numbers entered.\n");
            
            int sum_first_N = 0;
            // int boundaries to check for overflow
            constexpr int int_overflow_max = INT_MAX;
            constexpr int int_overflow_min = INT_MIN;

            for(int i = 0; i < N; ++i){
                int value = integer_numbers[i];
                // Check if adding value would exceed INT_MAX (2^31 - 1) or go below INT_MIN (-2^31)
                if( (value > 0 && sum_first_N > int_overflow_max - value) ||
                     (value < 0 && sum_first_N < int_overflow_min - value))
                     error("Overflow Error: Sum can't be represented as an integer.\n");
          
                sum_first_N += value;
            }
            cout << "The sum of the first " << N << " numbers (";
            for(int i = 0; i < N; ++i)
                cout << integer_numbers[i] << " ";
            cout << ") is " << sum_first_N << '\n';
            

    } catch (exception& e){
        cerr << "ERROR CAUGHT: " << e.what() << '\n';
    }
    
   
}