#include "PPPheaders.h"

int main() {

    try {
        cout << "Enter a number with up to 4 digits (or x to exit): ";
        string input;
    
        while (true) {
            cin >> input;

            if(!cin) error("Invalid Input. Expected char*");
            if (input == "x") break;

            bool is_negative = false;
            int start = 0;

            if (input[0] == '-') {
                is_negative = true;
                start = 1;
            }

            // Check that all characters are digits
            for (int i = start; i < input.size(); ++i) {
                if (!isdigit(input[i])) {
                    error ("Error: Invalid input, expected digits only.\n");
                    return 1;
                }
            }

            // Convert string of digits to int
            int number = 0;
            for (int i = start; i < input.size(); ++i) {
                number = number * 10 + (input[i] - '0');
            }

            if (is_negative) number *= -1;

            // Print int value
            cout << "Integer value: " << number << endl;

            // Output in words
            //int digits = input.size() - start;
            vector<string> labels = {"thousand", "hundred", "ten", "one"};
            vector<int> values(4, 0);

             // Fill from right to left
            int idx = 3;
            for (int i = input.size() - 1; i >= start && idx >= 0; --i, --idx) {
                values[idx] = input[i] - '0';
            }

            // Print descriptive output
            cout << abs(number) << " is ";
            if (values[0]) cout << values[0] << " thousand ";
            if (values[1]) cout << values[1] << " hundred ";
            if (values[2]) cout << values[2] << " ten ";
            if (values[3]) cout << values[3] << " one";
            cout << ".\n\n";

            cout << "Enter another number (or x to exit): ";
        }
    }catch(exception& e){
        cerr << "Error: " << e.what() << '\n';
    }
}