#include "PPPheaders.h"


// checks if a string is a single digit between 0 and 9
bool isOneDigit(string digit);

// Simple calculator able to handle the four basic math operations (+,-,*,/) on two input values. 
// The user enters three arguments: two integer values between 0 and 9 (as digits or in the spelled-out form)
// and a character to represent an operation.

int main(){
    
    string first_operand;
    int val1;
    string second_operand;
    int val2;
    char operation;
    double result;

    vector<string> spelled_out_numbers = {"zero","one","two","three","four","five","six","seven","eight","nine"};

    cout << "Enter the integer values for the first and second operands (between 0 and 9 )" << 
            "and one of the following operations (+, -, *, /): ";

    while (cin >> first_operand >> second_operand >> operation) {

        bool found_val1 = false;
        bool found_val2 = false;
        bool division_by_zero = false;
        bool found_operation = true;

        if (first_operand.size() == 1 && isOneDigit(first_operand)) { 
            found_val1 = true;
            val1 = first_operand[0] - '0';
            cout << "The number " << val1 << " is spelled out as '" << spelled_out_numbers[val1] << "'.\n";
        } else {
            for (int i = 0; i < spelled_out_numbers.size(); ++i){
                if (first_operand == spelled_out_numbers[i]){
                    found_val1 = true;
                    val1 = i;
                    cout << "'" << first_operand << "'" << " is the spelled out version of " << val1 << ".\n";
                    break;
                }
            }
            if (!found_val1){
                cout << "Can't recognize the value entered for the first operand.\n";
            }
        }
        
        if (second_operand.size() == 1 && isOneDigit(second_operand)){
            found_val2 = true;
            val2 = second_operand[0] - '0';
            cout << "The number " << val2 << " is spelled out as '" << spelled_out_numbers[val2] << "'.\n";  
        } else {
            for (int i = 0; i < spelled_out_numbers.size(); ++i){
                if (second_operand == spelled_out_numbers[i]){
                    found_val2 = true;
                    val2 = i;
                    cout << "'" << second_operand << "'" << " is the spelled out version of " << val2 << ".\n";
                    break;
                }
            }
            if (!found_val2) {
                cout << "Can't recognize the value entered for the second operand.\n";
            }     
        }

        switch (operation){
            case '+':
            result = val1 + val2;
            break;

            case '-':
            result = val1 - val2;
            break;

            case '*':
            result = val1 * val2;
            break;

            case '/':
            if (val2 == 0){
                cout << "ERROR! Devision by Zero!!";
                division_by_zero = true;
            }

            result = static_cast<double> (val1)/ val2;
            break;

            default:
            cout << "Invalid Operation!!\n";
            found_operation = false;
            break;
        }

        if (found_val1 && found_val2 && found_operation && !division_by_zero)
            cout << val1 << " " << operation << " " << val2 << " == " << result << '\n';
        else
            cout << "Invalid Inputs!. Can't produce a valid output!\n";
        
        cout << "Enter the integer values for the first and second operands (between 0 and 9 )" << 
                "and one of the following operations (+, -, *, /): ";
        
    }

        return 0;
}


bool isOneDigit(string digit)
{
        if (digit == "0" ||
            digit == "1" ||
            digit == "2" ||
            digit == "3" ||
            digit == "4" ||
            digit == "5" ||
            digit == "6" ||
            digit == "7" ||
            digit == "8" ||
            digit == "9")
            return true;
        else{
            cout << "Can't recognize the value entered.\n";
            return false;
        }
}