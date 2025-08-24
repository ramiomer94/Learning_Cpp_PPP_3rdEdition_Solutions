#include "PPPheaders.h"

// Absolute zero is the lowest temperature that can be reached; it is -273.25C, or 0K.
// Argument temperature in Celsius (C) can't be below -273.25C
double ctok(double c) { // converts Celsius to Kelvin
    expect([&]{return c >= -273.25;}, "Temperature entered is below the lowest temprature that can be reached.\n");
    double k = c + 273.25; 
    return k;                       
}
// Argument temperature in Kelvin (K) can't be below 0K
double ktoc(double k){// converts  Kelvin to Celsius
    expect([&]{return k >= 0;}, "Temperature entered is below the lowest temprature that can be reached.\n");
    double c = k - 273.25;
    return c;
}

int main() {
    
    try{
        double temp = 0; 
        char unit;
        cin >> temp >> unit;
        if (unit == 'k' || unit == 'K'){
            double c = ktoc(temp); 
            cout << temp << "K" << " --> " << c << "C\n" ; 
        } else if (unit == 'c' || unit == 'C'){
            double k = ctok(temp); 
            cout << temp << "C" << " --> " << k << "K\n" ; 
        } else
            error("Unit entered is not defined! The program only reads Units in celsius (C or c) or Kelvin (K or k).\n");

    } catch (exception& e){
            cerr << "Caught: " << e.what() << '\n';
    }                  
}