#include "PPPheaders.h"


double ctok(double c) { // converts Celsius to Kelvin
    double k = c + 273.25; 
    return k;                       
}

int main() {
    
    try{
        double c = 0; 
        cin >> c; 
        // Absolute zero temp; that is -273.25 celsius is the lowest temperature that can be reached.
        if(c < -273.25) // input c can be lower than bsolute zero temp 
                        
            error("Temprature entered is below the lowest temperature that can be reached.\n");
        double k = ctok(c); 
        cout << k << '\n' ; 
    } catch (exception& e){
            cerr << "Caught: " << e.what() << '\n';
    }                  
}