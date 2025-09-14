#include "PPPheaders.h"

//------------------------------------------- Supporting Types (Date Class) -------------------------------------------//

// Month enum + helpers
enum class Month { jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };

int to_int(Month m) { return static_cast<int>(m); } 

Month int_to_month(int x){
    if (x < to_int(Month::jan) || x > to_int(Month::dec)) error("Bad month"); 
    return Month{x};
}

Month& operator++ (Month& m){
    m = (m == Month::dec) ? Month::jan : Month{to_int(m) + 1};
    return m;
}

ostream& operator<< (ostream& os, Month m){
    switch(to_int(m)){
        case 1:   os << "Jan";    break;
        case 2:   os << "Feb";    break;
        case 3:   os << "Mar";    break;
        case 4:   os << "Apr";    break;
        case 5:   os << "May";    break;
        case 6:   os << "Jun";    break;
        case 7:   os << "Jul";    break;
        case 8:   os << "Aug";    break; 
        case 9:   os << "Sep";    break;
        case 10:  os << "Oct";    break;
        case 11:  os << "Nov";    break;
        case 12:  os << "Dec";    break;
        default:  os << "Invalid month.";
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

enum class Day_name {
    monday = 0, tuesday, wednesday, thursday, friday, saturday, sunday
};

int day_to_int(Day_name dn) {
    return static_cast<int>(dn);
}
Day_name int_to_dayName (int dn_code){
    if (dn_code >= 0 && dn_code <= 6) return Day_name{dn_code};
    error("Invalid day. Expected int <= 6 and >= 0");
}

ostream& operator<< (ostream& os, Day_name dn){
    switch (day_to_int(dn)) {
        case 0: os << "Monday";     break;
        case 1: os << "Tuesday";    break;
        case 2: os << "Wednesday";  break;
        case 3: os << "Thursday";   break;
        case 4: os << "Friday";     break;
        case 5: os << "Saturday";   break;
        case 6: os << "Sunday";     break;
        default: os << "Bad day.";  break;
    }

    return os;
}
//------------------------------------ Date validation ------------------------------------//

// Calendar validation (Gregorian, leap-year aware)
bool is_date_valid(Year y, Month m, Day d){
    int year = y.year;
    int month = to_int(m);
    int day = d.day();

    if (year <= 0) return false; // starting point Jan 1st, 0001
    if (month < 1 || month > 12) return false;
    if ((month==4 || month==6 || month==9 || month==11) && (day < 1 || day > 30)) return false;
    if ((month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12) && (day < 1 || day > 31)) return false;

    bool leap = (year%4==0 && year%100!=0) || (year%400==0); // leap year check
    if (month==2) return (day >= 1 && day <= (leap ? 29 : 28));

    return true;
}

//------------------------------------------------ Date Class ----------------------------------------------------//

class Date {
public:
    Date() {}
    Date(Year yy, Month mm, Day dd) : y{yy}, m{mm}, d{dd} {
        if (!is_date_valid(yy, mm, dd)) error("Invalid Date");  // validation on init
    }
    Date(Year yy) : y{yy} {                                     // year-only constructor
        if (!is_date_valid(yy, m, d)) error("Invalid Date");
    }

    int get_day()   const { return d.day(); }        
    Month get_month() const { return m; }                        // month as int
    int get_month_code() const {return to_int(m); }
    int get_year()  const { return y.year; }

    void change_day(Day day);    
    void change_month(Month month);  
    void change_year(Year year);   
    void add_day(int day);

private:                                                        
    Year  y{1};                                                 // defaults to Jan 1st, 0001
    Month m = Month::jan;
    Day   d{1};
};

// Safely update individual components, validating each change
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

void Date::add_day(int day){
    if(day <= 0) error("positive int expected");
        for(int i = 1; i <= day; ++i){
            d = Day{get_day() + 1};
                if(!is_date_valid(y,m,d)){
                    if(m == Month::dec){
                        y = {y.year + 1};
                        m = Month::jan;
                        d = Day{1};
                    }
                    else{
                        ++m;
                        d =  Day{1};
                    }
                }
            }
}

//------------------------------------------------ Helper functions for Date Class ----------------------------------------------------//

// print Date as (dd/Mon/yyyy)
ostream& operator<<(ostream& os, const Date& dt) {   
    return os << '(' << dt.get_day() << '/' << dt.get_month() << '/' << dt.get_year() << ')';
}

// Leap year check function
bool is_leapYear(int year){

    expect([&]{return year > 0;}, "expected year > 0"); // guard: year must be positive
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); // divisible by 4, not by 100 OR divisible by 400

}

// Count days since reference_year/Jan/1 up to given date
int count_days(const Date& date, Year reference_year){

    const Date reference_date {reference_year}; // jan 1 0001
    vector<int> days_in_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int total_days = 0;

    // Add full years
    for(int year = reference_date.get_year(); year <= date.get_year(); ++year){
        if(year < date.get_year())
            total_days += is_leapYear(year) ? 366 : 365;

        // Add months of current year
        if(year == date.get_year()){
            for(int month = 1; month < date.get_month_code(); ++month)
                total_days += days_in_month[month-1];
        }
    }

    // Add days of current month (0-based index)
    total_days += date.get_day() - 1;

    
    if(is_leapYear(date.get_year()) && date.get_month_code() > 2) return ++total_days;
    return total_days;       
}

// Return the next workday after a given date
Date next_weekday(Date date) {

    int total_days = count_days(date, Year{1});
    int day_of_the_week = total_days % 7;

     // If Friday → skip to Monday
    if(int_to_dayName(day_of_the_week) == Day_name::friday) {
        date.add_day(3);
        return date;
    }
     // If Saturday → skip to Monday
    else if (int_to_dayName(day_of_the_week) == Day_name::saturday){
        date.add_day(2);
        return date;
    }
    // Otherwise → just add one day
    else{
        date.add_day(1);
        return date;
    }
}

// Given a Date, compute which day of the week it falls on.
Day_name day_of_week(const Date& date){

    // Count total days elapsed since the reference date (Jan 1, Year 1)
    int total_days = count_days(date,Year{1});

    // Map the total number of days into [0..6]
    // 0 = Monday, 1 = Tuesday, ... 6 = Sunday
    int day_of_the_week = total_days % 7;

    return int_to_dayName(day_of_the_week);
}

// Week number of the given date (week 1 has Jan 1; week starts Sunday)
int week_of_year(const Date& d) {

    // 0-based day-of-year
    const int day_of_year = count_days(d, Year{d.get_year()});

    // Weekday of Jan 1 for this year
    const Day_name jan1_dn = day_of_week(Date{Year{d.get_year()}, Month::jan, Day{1}});
    const int k = day_to_int(jan1_dn); 

    // Offset from the Sunday on/before Jan 1 to Jan 1
    const int base = (k == day_to_int(Day_name::sunday)) ? 0 : (k + 1);

    // Week index: add offset, divide by 7, then 1-index
    return (base + day_of_year) / 7 + 1;
}

//------------------------------------------------ Test Date Class and its Helper Functions ----------------------------------------------------//

int main(){
      try {
        int passed = 0, failed = 0;

        auto show = [&](const char* label, bool ok){
            if (ok) { std::cout << "[PASS] " << label << "\n"; ++passed; }
            else    { std::cout << "[FAIL] " << label << "\n"; ++failed; }
        };

        // 1) Known weekday: Jan 1, 2004 was Thursday
        Date d1{Year{2004}, Month::jan, Day{1}};
        std::cout << d1 << " -> " << day_of_week(d1) << "\n";
        show("Jan 1, 2004 is Thursday", day_of_week(d1) == Day_name::thursday);

        // 2) Leap-day weekday: Feb 29, 2004 was Sunday
        Date d2{Year{2004}, Month::feb, Day{29}};
        std::cout << d2 << " -> " << day_of_week(d2) << "\n";
        show("Feb 29, 2004 is Sunday", day_of_week(d2) == Day_name::sunday);

        // 3) next_weekday across weekend:
        //    - If Friday -> +3 days (to Monday)
        //    - If Saturday -> +2 days (to Monday)
        Date f{Year{2025}, Month::sep, Day{5}}; // 2025-09-05 is Friday
        show("Sep 5, 2025 is Friday", day_of_week(f) == Day_name::friday);
        Date f_next = next_weekday(f);
        std::cout << "next_weekday(" << f << ") -> " << f_next << " " << day_of_week(f_next) << "\n";
        show("Next weekday after Fri Sep 5, 2025 is Mon Sep 8, 2025",
             f_next.get_year()==2025 && f_next.get_month_code()==9 && f_next.get_day()==8 &&
             day_of_week(f_next)==Day_name::monday);

        Date s{Year{2025}, Month::sep, Day{6}}; // Saturday
        show("Sep 6, 2025 is Saturday", day_of_week(s) == Day_name::saturday);
        Date s_next = next_weekday(s);
        std::cout << "next_weekday(" << s << ") -> " << s_next << " " << day_of_week(s_next) << "\n";
        show("Next weekday after Sat Sep 6, 2025 is Mon Sep 8, 2025",
             s_next.get_year()==2025 && s_next.get_month_code()==9 && s_next.get_day()==8 &&
             day_of_week(s_next)==Day_name::monday);

        // 4) week_of_year: by your rules (Week 1 contains Jan 1; weeks start Sunday)
        //    Dec 28, 2004 should be Week 53.
        Date d3{Year{2004}, Month::dec, Day{28}};
        int w3 = week_of_year(d3);
        std::cout << d3 << " week_of_year = " << w3 << "\n";
        show("Dec 28, 2004 is Week 53", w3 == 53);

        // 5) Jan 1 is always in Week 1
        int w1 = week_of_year(d1);
        std::cout << d1 << " week_of_year = " << w1 << "\n";
        show("Jan 1, 2004 is Week 1", w1 == 1);

        // 6) Cross-year sanity: Jan 1, 2005 was Saturday
        Date d4{Year{2005}, Month::jan, Day{1}};
        std::cout << d4 << " -> " << day_of_week(d4) << "\n";
        show("Jan 1, 2005 is Saturday", day_of_week(d4) == Day_name::saturday);
        std::cout << d4 << " week_of_year = " << week_of_year(d4) << "\n";
        show("Jan 1, 2005 is Week 1", week_of_year(d4) == 1);

        std::cout << "\n=== SUMMARY ===\nPassed: " << passed << "\nFailed: " << failed << "\n";
        return failed ? 1 : 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 2;
    }
}

