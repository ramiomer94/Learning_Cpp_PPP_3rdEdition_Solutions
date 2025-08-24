#include "PPPheaders.h"

string to_lower_case(string day_name){
    for(char& c: day_name)
        c = tolower(char{c});
    return day_name;
}

int main(){

    string monday = "MOndaY";
    monday = to_lower_case(monday);
    cout << monday << '\n';
    return 0;

}