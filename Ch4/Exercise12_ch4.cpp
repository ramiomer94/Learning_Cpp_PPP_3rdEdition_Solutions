#include "PPPheaders.h"

int main(){

    try {

        // Vector to store the secret 4-digit sequence (no repeating digits)
        vector<int> sequence_to_guess = {1, 4, 0, 5}; //The sequence is hard-coded

        cout << "Welcome to Bulls and Cows!\n";
        cout << "I have selected a 4-digit number made of different digits between 0 and 9.\n";
        cout << "Your job is to guess the number. After each guess, I'll tell you:\n";
        cout << " - How many digits are correct and in the correct position (bulls), and\n";
        cout << " - How many digits are correct but in the wrong position (cows).\n";
        cout << "Enter four digits (no repeats) like this: 1234\n";
        cout << "Let's begin!\n\n";
        cout << "Your guess: ";

        string guess;
        while(cin >> guess){ // Game loop: keep prompting until user guesses correctly

            if(guess.size() != 4) error("Invalid input size. Expected input size 4.\n");

            bool isduplicate;
            vector<int> guess_digits; // Vector to store the  4-digit guess (no repeating digits)
    
            for(int i = 0; i < 4; ++i){ // Convert input string to vector of digits and check for repeats
                    
                if(!isdigit(guess[i])) error("Invalid Input. Expected a digit.\n");
                
                isduplicate =false;
                int digit = guess[i] - '0';
                
                for(int j = 0; j < i; ++j){ // Check for repeated digits

                    if (digit == guess_digits[j]){
                        cerr << ("Expected 4 unique digits. No repeats allowed for your guess.\n");
                        isduplicate = true;
                        break;
                    }
                }

                if(isduplicate) break;    
                guess_digits.push_back(digit);
          
            }

            if(isduplicate){
                cout << "Enter your new guess (NO REPEATS): ";
                continue;
            } 

            int bulls = 0;
            int cows = 0;

            for(int i = 0; i < guess_digits.size(); ++i){  // Compare guessed digits with the sequence
                if(guess_digits[i] == sequence_to_guess[i])
                    ++bulls; // digit and position match
                else { 
                    for(int j = 0; j < sequence_to_guess.size(); ++j)
                        if(j != i && guess_digits[i] == sequence_to_guess[j]) 
                            ++cows; // digit exists elsewhere in the sequence
                }
            }

            if(bulls == 1)
                cout << bulls << " " << "bull and ";
            if(bulls != 1 && bulls != 4)
                cout << bulls << " " << "bulls and ";
            if(cows == 1)
                cout << cows << " cow.\n";
            else {
                if(bulls != 4)
                    cout << cows << " cows.\n";
            }
            
            if(bulls == 4){
                cout << "CONGRATS!! YOU GUESSED CORRECT:\nThe computer generated 4-digit number is: ";
                for(int x: sequence_to_guess)
                    cout << x;
                cout << " and your guess is ";
                for(int x: guess_digits)
                    cout << x; 
                cout << '\n';
                break;
            }
            cout << "Your guess: ";
        }

    } catch(exception& e){
         // Catch and display runtime errors
        cerr << "EXCEPTION CAUGHT: " << e.what() << '\n';
    }

}