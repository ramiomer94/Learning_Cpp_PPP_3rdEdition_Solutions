#include "PPPheaders.h"


// Function to generate a vector of 4 unique random digits using the provided seed
vector<int> generate_random_sequence (int seed_arg){

    vector<int> random_sequence;
    seed(seed_arg); // Set seed for random generation
    int sequence_size = 4;

    for (int i = 0; i < sequence_size; ++i) { // Generate 4 unique random digits from 0 to 9
        
        int digit;
        bool is_duplicate;
        
        do { // Keep generating a random digit until it's not a duplicate
            digit = random_int(0,9); // random number from 0 to 9
            is_duplicate = false;

            // Check if the digit already exists in the vector
            for (int j = 0; j < i; ++j) {
                
                if (random_sequence[j] == digit) {
                    is_duplicate = true;
                    break;
                }
            }
        } while (is_duplicate);

        random_sequence.push_back(digit); // Add the unique digit to the sequence
    }
    return random_sequence;
}


// Function to write out the result after each guess
void writeout_result(int bulls, int cows){

    // Validate the range of bulls and cows
    expect([&]{return bulls >= 0 && cows >= 0 && bulls < 5 && cows < 5;}, "Invalid COWS or BULLS value(s): Expected values [0,4]");

    if(bulls == 1)
        cout << bulls << " " << "bull and ";
    else if (bulls != 1 && bulls != 4)
        cout << bulls << " " << "bulls and ";
    else
        cout << "CONGRATS!! YOU GUESSED CORRECT:\nThe computer generated 4-digit number is: ";
    
    if(cows == 1)
        cout << cows << " cow.\n";
    else {
        if(bulls != 4)
            cout << cows << " cows.\n";
    }
}

int main(){

    try {
        while(true){ // Loop to allow multiple rounds of play

            // Ask user for a seed for random number generation
            cout << "Enter the sequence generator seed: ";
            double n;
            cin >> n;

            if(!cin) error("Invalid or corrupted Input. Expected an input of type double.\n");

            // Vector to store the secret 4-digit sequence (no repeating digits)
            vector<int> sequence_to_guess = generate_random_sequence(static_cast<int>(n));

            if(sequence_to_guess.size() != 4) 
                error("Game Crashed!Program generated an invalid sequence!Size of the sequence doesn't equal 4\n");
            
            // Game instructions
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

                vector<int> guess_digits;
                bool isduplicate;
                int digit;
    
                for(int i = 0; i < 4; ++i){ // Convert input string to vector of digits and check for repeats

                    if(!isdigit(guess[i])) error("Invalid Input. Expected a digit.\n");
                    isduplicate = false;
                    digit = guess[i] - '0';

                    for(int j = 0; j < i; ++j){ // Check for repeated digits
                        if (digit == guess_digits[j]){
                            cerr << "Expected 4 unique digits. No repeats allowed for your guess.\n";
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

                writeout_result(bulls,cows);
            
                if(bulls == 4){ // If all digits correct and in the correct position, end game
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
        }
    } catch(exception& e){
         // Catch and display runtime errors
        cerr << "EXCEPTION CAUGHT: " << e.what() << '\n';
    }

}