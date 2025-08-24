#include "PPPheaders.h"
#include <iomanip>


// The program calculate how many chess squares are required to give the chess inventor at least 1000 grains of rice,
// at least 1,000,000 grains, and at least 1,000,000,000 grains. The inventor asked for one grain of rice 
// for the first square, 2 for the second, 4 for the third, and so on, doubling for each of the 64 squares.

int main()
{
    
    constexpr int max1 = 1000;
    constexpr int max2 = 1000000;
    constexpr int max3 = 1000000000 ;
    vector<int> max = {max1,max2,max3};

    for (int x : max){
        cout << "The number of squares required to give the inventor at least " << x << " grains of rice.\n";
        cout << left << setw(10) << "Square" << setw(20) << "Reward" << setw(20) << "Total" << '\n';

        int current_square = 1;
        int current_number_of_grains = 1;
        int total_grains = 1;
        
        cout << left << setw(10) << current_square << setw(20) << current_number_of_grains << setw(20) << total_grains << '\n';

        while (total_grains < x ){
            ++current_square;
            current_number_of_grains *=2;
            total_grains += current_number_of_grains;
            cout << left << setw(10) << current_square << setw(20) << current_number_of_grains << setw(20) << total_grains << '\n'; 
        }
        cout << "\nThe number of squares required to give the inventor at least " << x << " grains of rice is: " << current_square << ".\n\n\n";
    }

}