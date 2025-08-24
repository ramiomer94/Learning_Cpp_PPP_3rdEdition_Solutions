#include "PPPheaders.h"

enum class Month {
    jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
};

int to_int(Month m){
	return static_cast<int>(m);
}

Month operator++(Month& m){
    m = (m == Month::dec ? m = Month:: jan : Month{to_int(m)+1});
    return m;
}
vector<string> month_tbl = {"not a month", "January", "February", "March", "April", "May", "June", "July", "August", "September"
"October", "November", "December"};
ostream& operator<<(ostream& os, Month m){
    return os<<month_tbl[to_int(m)];
}

int main(){
    Month m1 = Month::jan;
    ++m1;
    cout << m1 << '\n';
    Month m2 = Month::dec;
    ++m2;
    cout << m2 << '\n';
   
    return 0;
}