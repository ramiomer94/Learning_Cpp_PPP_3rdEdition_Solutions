#include "PPPheaders.h"

double ctof(double c){  // converts Celsius to Fahrenheit
    constexpr double abs_zero_temp_c = -273.25; //Absolute zero temperature in Celsius; (lowest temp that can be reached)
    expect([&]{return c >= abs_zero_temp_c;}, "Temperature entered is below the lowest temprature that can be reached.\n");
    constexpr double conversion_factor_ctof = double(9)/5;
    double f = c * conversion_factor_ctof + 32;
    return f;
}

double ftoc(double f){ // converts Fahrenheit to Celsius
    constexpr double abs_zero_temp_f = -459.67; //Absolute zero temperature in Fahernheit; (lowest temp that can be reached)
    expect([&]{return f >= abs_zero_temp_f;}, "Temperature entered is below the lowest temprature that can be reached.\n");
    constexpr double conversion_factor_ftoc = double(5)/9;
    double c = (f-32) * conversion_factor_ftoc;
    return c;
}

int main(){
    try{
        double temp;
        char unit;
        cin >> temp >> unit;

        if(cin){
            if (unit == 'C' || unit == 'c'){
                double f = ctof(temp);
                cout << temp << "C --> " << f << "F\n";
            }
            else if (unit == 'F' || unit == 'f'){
                double c = ftoc(temp);
                cout << temp << "F --> " << c << "C\n";
            }
            else
                error("Unit entered is not defined! The program only reads Units in Celsius (C or c) or Fahernheit (F or f).\n");

        }
        else
            error("Input entered is courrupted or Invalid: first input not double or seond input not character.\n");
    
    } catch (exception& e){
        cerr << "Caught: " << e.what() << '\n';
    }
}