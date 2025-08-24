#include "PPPheaders.h"

int main(){

    try {

        // Ask user how many values they want to compute differences between
        cout << "Please enter the number of values you want to be the differences between : ";
        int N;
        cin >> N;

        // Validate the input: must be a valid integer
        if(!cin) error("Expected an int value. Input is invalid or corrupted.\n");

        // N must be at least 2 to compute a difference between elements
        if(N <= 1) error("Expected N a positive integer bigger than one.");    
        cout << "Please enter some integers (press '|' to stop): ";

        double input;
        vector<double> double_numbers;

        // Read inputs into the vector until invalid input or '|' stops cin
        while(cin >> input)
            double_numbers.push_back(input);
         
        // Error if no values were entered
        if(double_numbers.size() == 0)
            error("No values entered. List of numbers entered is empty.\n");

        // N must be less than or equal to the number of values entered    
        if(N > double_numbers.size())
            error("OUT OF BOUND ERROR: Expected N smaller or equal to the size of the list of numbers entered.\n");
            

        vector<double> differences;
        for(int i = 0; i < N; ++i){ // Compute differences between adjacent elements
            double value = double_numbers[i+1] - double_numbers[i];

            // Check for overflow in double representation
            if (isinf(value)) error("Overflow Error: Difference can't be represented as a double!\n");
            differences.push_back(value);
        }

        // Output original list of first N elements
        cout << "The difference between each adjacent elements of the first " << N << " numbers (";
        for(int i = 0; i < N; ++i)
            cout << double_numbers[i] << " ";
        cout << ") is [";

        // Output computed differences in a formatted way
        for (int i = 0; i < differences.size(); ++i) {
            cout << differences[i];
            if(i != differences.size() - 1) cout << " ,";
        }
        
        cout << "]\n";  

    } catch (exception& e){
        cerr << "ERROR CAUGHT: " << e.what() << '\n';
    }
    
   
}