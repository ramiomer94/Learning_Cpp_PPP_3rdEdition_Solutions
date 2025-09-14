#include "PPPheaders.h"

//------------------------------------------- Supporting Types (Date Class) -------------------------------------------//
constexpr int start_year = 1970;
// Month enum + helpers
enum class Month { jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };

// Provides safe conversions between int <-> Month
int to_int(Month m) { return static_cast<int>(m); } 

Month int_to_month(int x){
    if (x < to_int(Month::jan) || x > to_int(Month::dec)) error("Bad month"); 
    return Month{x};
}

Month& operator++ (Month& m){
    m = (m == Month::dec) ? Month::jan : Month{to_int(m) + 1};
    return m;
}

bool is_leapYear(int year);

// Returns number of days in given month/year
int month_length(int year, int month_code) {
    switch (month_code) {
        case 1: return 31;
        case 2: return is_leapYear(year) ? 29 : 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: error("bad month");
    }
}

ostream& operator<< (ostream& os, Month m){
    switch(to_int(m)){
        case 1:   os << "01";    break;
        case 2:   os << "02";    break;
        case 3:   os << "03";    break;
        case 4:   os << "04";    break;
        case 5:   os << "05";    break;
        case 6:   os << "06";    break;
        case 7:   os << "07";    break;
        case 8:   os << "08";    break; 
        case 9:   os << "09";    break;
        case 10:  os << "10";    break;
        case 11:  os << "11";    break;
        case 12:  os << "12";    break;
        default:  os << "Invalid month.";
    }
    return os;
}

class Year { 
    public:
            Year(int yy) : year{yy}{
                if(yy < start_year) error("Invalid date. Start year is 1970");
            }
            int get_year() const { return year; }
    private:
            int year = start_year;
};

class Day{
public:
    Day() {}
    Day(int dd) : d{dd} { if (dd < 1) error("Bad day. Expected positive int"); }
    int get_day() const { return d; }
private:
    int d = 1;
};

struct YMD {
        Year y;
        Month m;
        Day d;
};

ostream& operator<< (ostream& os, const YMD& date){
    return os << date.y.get_year() << '-' <<  date.m << "-" << date.d.get_day();
}

//------------------------------------ Date validation ------------------------------------//

// Calendar validation (year >= 1970, month 1–12, day within range)
bool is_date_valid(Year y, Month m, Day d) {
    if (y.get_year() < start_year) return false;
    int mm = to_int(m);
    int dd = d.get_day();
    if (mm < 1 || mm > 12) return false;
    return 1 <= dd && dd <= month_length(y.get_year(), mm);
}

// Leap year check function
bool is_leapYear(int year){

    expect([&]{return year > 0;}, "expected year > 0"); // guard: year must be positive
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); // divisible by 4, not by 100 OR divisible by 400

}

// Count days since Jan 1, 1970
// Day 0 = Jan 1, 1970
long int count_days(Year y, Month m, Day d){

    long int total_days = 0;

    // Add full years
    for(int year = start_year; year <= y.get_year(); ++year){
        if(year < y.get_year())
            total_days += is_leapYear(year) ? 366 : 365;

        // Add months of current year
        if(year == y.get_year()){
            for(int month = 1; month < to_int(m); ++month)
                total_days += month_length(year, month);
        }
    }

    // Add days of current month (0-based index)
    total_days += d.get_day() - 1;

    return total_days;       
}

//------------------------------------------------ Date Class ----------------------------------------------------//

class Date {
public:
    Date (long int dc) : day_count{dc} { // Construct from absolute day count (0 = 1970-01-01)
        if(dc < 0) error("Invalid Date. Expected day count >= 0");
    } 

    // Construct from Y-M-D
    Date(Year yy, Month mm, Day dd) : day_count{count_days(yy,mm,dd)} {
        if (!is_date_valid(yy, mm, dd)) error("Invalid Date");                      // validation on init
    }

     // Construct from year only -> Jan 1
    Date(Year yy) : day_count{count_days(yy, Month::jan, Day{1})}{                  // year-only constructor
        if (!is_date_valid(yy, Month::jan, Day{1})) error("Invalid Date");
    }

    long int get_day_count()  const { return day_count; }  
    void add_day(long int n_days);

    YMD ymd_from_daycount() const;                                                  // Convert stored day_count back to Y-M-D

    int get_date_year() const { return ymd_from_daycount().y.get_year(); }
    Month get_date_month() const { return ymd_from_daycount().m; }
    int get_date_month_code() const { return to_int(ymd_from_daycount().m); }
    int get_date_day() const { return ymd_from_daycount().d.get_day(); } 

private:                                                        
    long int day_count = 0;                                                        // day_count = 0 --> jan 1 , 1970
   
};

void Date::add_day(long int n_days){
    day_count += n_days;
    expect([&]{return day_count >= 0;}, "invalid date. Start date is jan 1 1970");
}

YMD Date::ymd_from_daycount() const{

    long int days_remaining = day_count;
    int date_year = start_year;
    int date_month = 1;
    int days_in_year = is_leapYear(date_year) ? 366 : 365;

     // Reduce day_count by full years, then by full months
    while(days_remaining >= days_in_year) {
        days_remaining -= days_in_year;
        ++date_year;
        days_in_year = is_leapYear(date_year) ? 366 : 365;
    }

    while(days_remaining >= month_length(date_year,date_month)){
        days_remaining -= month_length(date_year,date_month);
        ++date_month;
    }
    int date_day = days_remaining + 1; // Remaining offset = day of month
    YMD date {Year{date_year}, int_to_month(date_month), Day{date_day}};
    return date;
}


int main(){

    auto CHECK = [](bool cond, const string& msg){
        if (!cond) cerr << "FAIL: " << msg << '\n';
        else       cout << "PASS: " << msg << '\n';
    };

    try {
        // --- Daycount from YMD (day 0 = 1970-01-01) ---
        Date d1 {Year{1970}, Month::jan, Day{1}};
        CHECK(d1.get_day_count() == 0,   "1970-01-01 -> daycount 0");

        Date d2 {Year{1970}, Month::jan, Day{2}};
        CHECK(d2.get_day_count() == 1,   "1970-01-02 -> daycount 1");

        Date d3 {Year{1970}, Month::dec, Day{31}};
        CHECK(d3.get_day_count() == 364, "1970-12-31 -> daycount 364");

        Date d4 {Year{1971}, Month::jan, Day{1}};
        CHECK(d4.get_day_count() == 365, "1971-01-01 -> daycount 365");

        Date d5 {Year{1972}, Month::feb, Day{28}}; // 1972 is leap
        CHECK(d5.get_day_count() == 788, "1972-02-28 -> daycount 788");

        Date d6 {Year{1972}, Month::feb, Day{29}};
        CHECK(d6.get_day_count() == 789, "1972-02-29 -> daycount 789");

        // --- YMD from daycount ---
        Date d7 {789}; // should be 1972-02-29
        YMD  y7 = d7.ymd_from_daycount();
        CHECK(y7.y.get_year() == 1972 && to_int(y7.m) == 2 && y7.d.get_day() == 29,
              "daycount 789 -> 1972-02-29");

        // A couple of 1973 checks for clarity
        Date d8 {1096}; // 1973-01-01
        YMD  y8 = d8.ymd_from_daycount();
        CHECK(y8.y.get_year() == 1973 && to_int(y8.m) == 1 && y8.d.get_day() == 1,
              "daycount 1096 -> 1973-01-01");

        Date d9 {1154}; // 1973-02-27
        YMD  y9 = d9.ymd_from_daycount();
        CHECK(y9.y.get_year() == 1973 && to_int(y9.m) == 2 && y9.d.get_day() == 28,
              "daycount 1154 -> 1973-02-28");

        // --- add_day sanity ---
        d7.add_day(1); // 789 -> 790 (1972-03-01)
        YMD y7b = d7.ymd_from_daycount();
        CHECK(y7b.y.get_year() == 1972 && to_int(y7b.m) == 3 && y7b.d.get_day() == 1,
              "daycount 790 -> 1972-03-01 (after add_day)");

    } catch (exception& e) {
        cerr << "Exception: " << e.what() << '\n';
    }

}