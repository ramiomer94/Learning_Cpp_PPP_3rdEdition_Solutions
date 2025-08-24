#include "PPPheaders.h"

bool is_date_valid(int year, int month, int day){
    if(year < 0) return false;
    if(month < 1 || month > 12) return false;
    if((month == 4 || month == 6 || month == 9 || month == 11) && (day < 1 || day > 30)) return false;
    if((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && (day < 1 || day > 31)) 
       return false;
    if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        if(month == 2 && (day > 29 || day < 1)) return false;
    } else {
        if(month == 2 && (day > 28 || day < 1)) return false;
    }
    return true;
}

class Date { 
    public:
        Date(int yy, int mm, int dd) :y{yy}, m{mm}, d{dd}{ 
           if(!is_date_valid(y,m,d))error("invalid date");
        }
        void add_day(int n) {
            if(n <= 0) error("positive int expected");
            for(int i = 1; i <= n; ++i){
                ++d;
                if(!is_date_valid(y,m,d)){
                    if(m == 12){
                        ++y;
                        m = 1;
                        d = 1;
                    }
                    else{
                        ++m;
                        d = 1;
                    }
                }
            }
           
        }
        int year(){return y;}
        int month() { return m;}
        int day(){return d;}
    private:
        int y, m, d; // year, month, day 
};

ostream& operator<<(ostream& os, Date d) {
return os << d.year() << '/' << d.month() << '/' << d.day(); }

int main(){
    try {
        Date today {2025,8,17};
        today.add_day(42);
        cout << today << '\n';
    } catch(exception& e){
        cerr << e.what() << '\n';
    }

}