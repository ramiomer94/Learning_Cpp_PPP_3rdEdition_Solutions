#include "PPPheaders.h"


// checks if a string is a single digit between 0 and 9
bool isOneDigit(string digit);

/*
The program converts a digit to its corresponding spelled-out value; e.g., the input 7 gives the output seven. 
The same program, using the same input loop, convert spelled-out numbers into their digit form; e.g., the input seven gives the output 7.
*/
int main()
{
    vector<string> spelled_out_numbers = {"zero","one","two","three","four","five","six","seven","eight","nine"};
    string number;

    cout << "Choose a number between 0 and 9 (you may enter it as a digit or spell it out): ";

    while (cin >> number){

        bool found = false;
        if (number.size() == 1 && isOneDigit(number)){
            int index = number[0] - '0';
            cout << "The number " << number << " is spelled out as '" << spelled_out_numbers[index] << "'.\n";
    
        } else {
            
            for (int i = 0; i < spelled_out_numbers.size(); ++i){
                if (number == spelled_out_numbers[i]){
                     cout << "'" << number << "'" << " is the spelled out version of " << i << ".\n";
                     found = true;
                     break;
                }
            }
        
            if (!found)
                cout << "Can't recognize the value entered.\n";
            
        }

        cout << "Choose a number between 0 and 9 (you may enter it as a digit or spell it out): ";
            
            
    }
    
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