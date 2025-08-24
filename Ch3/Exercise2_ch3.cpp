#include "PPPheaders.h"

//// Ask the user for a string and print each character along with its ASCII integer value.
int main()
{
    cout << "Please enter a string of characters: ";
    string line = "";

    cin >> line;

    for (char c : line)
        cout << c << '\t' << int{c} << '\n'; 
    
}