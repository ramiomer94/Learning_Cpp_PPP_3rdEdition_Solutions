#include "PPPheaders.h"

enum class Month { jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };

int to_int(Month m) { return static_cast<int>(m); }

Month int_to_month(int x){
    if (x < to_int(Month::jan) || x > to_int(Month::dec)) error("Bad month");
    return Month{x};
}

ostream& operator<< (ostream& os, Month m){
    switch(to_int(m)){
        case 1: os << "Jan";    break;
        case 2: os << "Feb";    break;
        case 3: os << "Mar";    break;
        case 4: os << "Apr";    break;
        case 5: os << "May";    break;
        case 6: os << "Jun";   break;
        case 7: os << "Jul";   break;
        case 8: os << "Aug";    break; 
        case 9: os << "Sep";    break;
        case 10: os << "Oct";   break;
        case 11: os << "Nov";   break;
        case 12: os << "Dec";   break;
        default: os << "Invalid month.";
    }
    return os;
}

struct Year { int year; };

class Day{
public:
    Day() {}
    Day(int dd) : d{dd} { if (dd < 1) error("Bad day. Expected positive int"); }
    int day() const { return d; }
private:
    int d = 1;
};

bool is_date_valid(Year y, Month m, Day d){
    int year = y.year;
    int month = to_int(m);
    int day = d.day();

    if (year < 0) return false;
    if (month < 1 || month > 12) return false;
    if ((month==4 || month==6 || month==9 || month==11) && (day < 1 || day > 30)) return false;
    if ((month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12) && (day < 1 || day > 31)) return false;

    bool leap = (year%4==0 && year%100!=0) || (year%400==0);
    if (month==2) return (day >= 1 && day <= (leap ? 29 : 28));

    return true;
}

class Date {
public:
    Date() {}
    Date(Year yy, Month mm, Day dd) : y{yy}, m{mm}, d{dd} {
        if (!is_date_valid(yy, mm, dd)) error("Invalid Date");
    }
    Date(Year yy) : y{yy} {
        if (!is_date_valid(yy, m, d)) error("Invalid Date");
    }

    int get_day()   const { return d.day(); }        
    Month get_month() const { return m; }
    int get_month_code() const {return to_int(m); }
    int get_year()  const { return y.year; }

    void change_day(Day day);    
    void change_month(Month month);  
    void change_year(Year year);   

private:
    Year  y{2001};
    Month m = Month::jan;
    Day   d{1};
};

void Date::change_day(Day day){
    if (!is_date_valid(y,m,day)) error("Invalid date. Can't change day.");
    d = day;
}

void Date::change_month(Month month){
    if (!is_date_valid(y,month,d)) error("Invalid date. Can't change month.");
    m = month;
}

void Date::change_year(Year year){
    if (!is_date_valid(year,m,d)) error("Invalid date. Can't change year.");
    y = year;
}

ostream& operator<<(ostream& os, const Date& dt) {   
    return os << '(' << dt.get_day() << '/' << dt.get_month() << '/' << dt.get_year() << ')';
}

int main(){
    try {
    Date a{Year{2000}, Month::feb, Day{29}};   // OK (leap)
    cout << a << "\n";

    Date b{Year{1900}, Month::feb, Day{28}};   // OK (not leap)
    cout << b << "\n";

    bool threw = false;
    try { Date bad{Year{1900}, Month::feb, Day{29}}; } // should fail
    catch (const exception&) { threw = true; }
    cout << "1900-02-29 throws? " << (threw ? "yes\n" : "no\n");

    Date c{Year{2024}, Month::feb, Day{29}};
    // changing to non-leap year should fail
    bool threw2 = false;
    try { c.change_year(Year{2023}); } catch (const exception&) { threw2 = true; }
    cout << "Change 2024-02-29 -> 2023 throws? " << (threw2 ? "yes\n" : "no\n");

    // Month printing check
    cout << Month::jan << " " << Month::jun << " " << Month::dec << "\n";

    } catch (const exception& e) {
    cerr << "UNEXPECTED: " << e.what() << "\n";
    }
}