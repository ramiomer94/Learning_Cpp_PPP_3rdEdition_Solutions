#include "PPPheaders.h"


// Leap year check function
bool is_leapYear(int year){

    expect([&]{return year > 0;}, "expected year > 0"); // guard: year must be positive
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); // divisible by 4, not by 100 OR divisible by 400

}

int main(){
    
    // test set includes negatives, zero, century, valid and invalid leap years
    vector<int> years_to_test {-1904, 0, 1900, 1904, 1908, 1912, 1914, 1916, 1920, 2000, 2002, 2012, 2016, 2020, 2025};
    for (const int& year : years_to_test){
        try{
            if(is_leapYear(year)) cout << year << " : leap year\n";
            else cout << year << " : not leap year\n";
        } catch (exception& e){
            cout << e.what() << '\n';
        }
    }
}