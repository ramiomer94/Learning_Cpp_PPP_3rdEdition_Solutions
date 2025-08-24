#include "PPPheaders.h"

// Guess a number the user is thinking of between 1 and 100 in no more than 7 yes/no questions.

int main()
{
    
    int guess = 0;
    int upper_limit = 100;
    int lower_limit = 1;
    char response = ' ';

    cout << "Think of a number between 1 and 100.\n";
    cout << "I will guess your number in at most 7 questions.\n";

    while (lower_limit < upper_limit){
        guess = (lower_limit + upper_limit) / 2;
        cout << "Is your number less than or equal " << guess << " ? (Y/N): ";
        cin >> response;
        if (response == 'Y' || response == 'y')
            upper_limit = guess;
        else if (response == 'N' || response == 'n')
            lower_limit = guess + 1;
        else
            cout << "Enter a valid answer!";
        
    }

    cout << "The number you are thinking of is: " << lower_limit << '\n';
   

}