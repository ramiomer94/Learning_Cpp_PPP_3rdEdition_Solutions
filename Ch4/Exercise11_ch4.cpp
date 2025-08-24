#include "PPPheaders.h"

int main(){

    try {
        cout << "Enter how many of the first numbers in the Fibonacci series you would like displayed:";
        int N;
        cin >> N;

        // Check if the input was a valid integer
        if(!cin) error("Expected an int value. Input is invalid or corrupted.\n");

        // Initialize the vector with the first two Fibonacci numbers
        vector<int> fibonacci_numbers;
        fibonacci_numbers.push_back(1);
        fibonacci_numbers.push_back(1);

    
        constexpr int max_int = INT_MAX; // Maximum value an int can hold
        for(int i = 0; i < N-2; ++i){  // Generate and print the first N Fibonacci numbers, checking for overflow
            if(fibonacci_numbers[i+1] > max_int - fibonacci_numbers[i]) 
                error("Overflow Error: fibonacci number can't be stored as int.\n");

            int value = fibonacci_numbers[i] + fibonacci_numbers[i+1];
            fibonacci_numbers.push_back(value);
        }
        // Output the full list of computed Fibonacci numbers
        cout << "The first " << N << " fibonacci numbers are: ";
        for(int x: fibonacci_numbers)
            cout << x << " ";
        cout << '\n';
        
         // Clear and reset the vector for reuse in finding the maximum storable Fibonacci number
        fibonacci_numbers.clear();
        fibonacci_numbers.push_back(1);
        fibonacci_numbers.push_back(1);
        int max_fibonacci_int = 1; // Variable to track the largest valid Fibonacci number
        int i = 0;
    
        while(true){  // Generate Fibonacci numbers until we reach one that would overflow
            int a = fibonacci_numbers[i];
            int b = fibonacci_numbers[i+1];

            // If the next Fibonacci number would overflow, stop
            if(b > max_int - a) break;

            max_fibonacci_int = fibonacci_numbers[i] + fibonacci_numbers[i+1];
            fibonacci_numbers.push_back(max_fibonacci_int);
            ++i;
        }
        
        cout << "The max fibonacci number that can be stored in an int variable is: " << max_fibonacci_int << " \n";

    } catch (exception& e){
        // Catch and display any exceptions thrown
        cerr << "EXCEPTION CAUGHT: " << e.what() << '\n';
    }
        
}