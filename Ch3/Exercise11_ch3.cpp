#include "PPPheaders.h"

/*
program that plays the game “Rock, Paper, Scissors.
The machine should give random answers (i.e., select the next rock, paper, or scissors randomly)
randomness is incorporated into the code through building a vector with a sequence of values to be used 
as “the next value.”
The enters a sequence of moves for the machine and we randomize it by inverting the order of the moves.
*/

int main()
{
    cout << "Let's play a game of ROCK, PAPER, AND SCISSORS\n";
    cout << "Enter the machine's move sequence (ROCK, PAPER, SCISSORS): \n";

    // Vector to store the machine's move sequence
    vector<string> moves;

    // Read in a list of valid moves from the user until an invalid input is entered
    for(string move; cin >> move;){
         if (move == "ROCK" || move == "PAPER" || move == "SCISSORS")
            moves.push_back(move);
        else
            break; 
    }
    
    // Reverse the machine's move sequence to shuffle it (adds unpredictability)
    for (int i = 0; i < moves.size()/2; ++i){
        string temp;
        temp = moves[i];
        moves[i] = moves[moves.size()-(i+1)];
        moves[moves.size()-(i+1)] = temp;
    }
        
    cout << "The machine can't promise you to use the exact same order of moves.\n";

    // Define possible user choices as an enum for easier switch-case logic
    enum Choice {ROCK, PAPER, SCISSORS, INVALID};
    Choice user_choice;

    // Loop through each move in the machine's shuffled move sequence
    for (string machine_move : moves){
        cout << "Choose one of the following three moves ('ROCK', 'PAPER', 'SCISSORS'): ";
        string user_move;
        cin >> user_move;

        // Show what moves were made
        cout << "Your move is " << user_move << " and the machine's move is " << machine_move << ". The result is: \n";

        // Convert user input string to enum
        if (user_move == "ROCK") 
            user_choice = ROCK;
        else if (user_move == "PAPER") 
            user_choice = PAPER;
        else if (user_move == "SCISSORS")
            user_choice = SCISSORS;
        else
            user_choice = INVALID;
            
        // Handle result if it's a tie
        if (machine_move == user_move)
            cout << "It's a tie!\n";
        else{
            // Use switch-case on user_choice to determine winner
            switch (user_choice){
                case ROCK: if (machine_move == "PAPER") cout << "The machine WINS. Good luck next time.\n";
                           else cout << "You WIN :)\n";
                          break;
                case PAPER: if (machine_move == "SCISSORS") cout << "The machine WINS. Good luck next time.\n";
                            else cout << "You WIN :)\n";
                            break;
                case SCISSORS: if (machine_move == "ROCK") cout << "The machine WINS. Good luck next time.\n"; 
                            else cout << "The machine WINS. Good luck next time.\n";
                            break;
                case INVALID: cout << "Can't recognize the move entered!!\n"; break;
                default: cout << "Invalid Input!!\n"; 
                            break;
            }
        } 
        cout << '\n';
    }

    cout << "Game over. Thanks for playing!\n";

}