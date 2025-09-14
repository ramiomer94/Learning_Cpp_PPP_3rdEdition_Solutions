#include "PPPheaders.h"

//------------------------------------------- Supporting Types (Date Class) -------------------------------------------//

// Month enum + helpers
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

//------------------------------------ Date validation ------------------------------------//

// Calendar validation (Gregorian, leap-year aware)
bool is_date_valid(Year y, Month m, Day d){
    int year = y.year;
    int month = to_int(m);
    int day = d.day();

    if (year < 0) return false;
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

private:                                                        // defaults to 2001/Jan/1
    Year  y{2001};
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

// print Date as (dd/Mon/yyyy)
ostream& operator<<(ostream& os, const Date& dt) {   
    return os << '(' << dt.get_day() << '/' << dt.get_month() << '/' << dt.get_year() << ')';
}

//------------------------------------ Utility validation functions (Book Class) ------------------------------------//

// Validate an author's name:
// - Non-empty
// - First and last chars must be alphabetic
// - Allowed: alphabetic, '-', '\'', ' '
// - No consecutive non-alphabetic characters
bool is_name(const string& n) {
    if (n.empty()) return false;

    // First and last must be alphabetic
    if (!isalpha(static_cast<unsigned char>(n.front())) || !isalpha(static_cast<unsigned char>(n.back()))) return false;

    for (int i = 0; i < n.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(n[i]);

        // Must be alphabetic, '-', '\'', or space
        if (!isalpha(ch) && ch != '-' && ch != '\'' && ch != ' ')
            return false;

        // Reject consecutive non-alphabetic characters
        if (i > 0) {
            unsigned char prev = static_cast <unsigned char> (n[i-1]);
            if (!isalpha(prev) && !isalpha(ch)) return false;
        }
    }

    return true;
}

// Validate a book title: must start with a letter or digit
bool is_title(const string& title) {
    if (title.empty()) return false;  // guard against empty titles
    
    unsigned char ch = static_cast<unsigned char>(title.front());
    return isalpha(ch) || isdigit(ch);
}

// Compute [start,end] index pairs for each substring split by '-'
vector<pair<int,int>> get_bounds(const string& s){
    vector<pair<int,int>> parts_boundaries;
    int start = 0;
    for(int i = 0; i < s.size(); ++i) {                            
        if(s[i] == '-'){
            parts_boundaries.push_back({start,i - 1}); // mark substring before hyphen
            start = i + 1;                             // move start index past '-'
        } 
    }
    parts_boundaries.push_back({start,s.size()-1});    // add last substring
    return parts_boundaries;
}

// Extract substring manually given [start,end] indices
string find_substr(const string& s, int start, int end){
    string part;
    for(int i = start; i <= end; ++i)
        part += s[i];       // build substring manually
        
    return part;
}

// Check if string is all digits
bool all_digits(const string& s){
    for(unsigned char ch : s)
        if(!isdigit(ch)) return false;
    return true;
}

// Validate ISBN format: n-n-n-x, where n=integer, x=digit or letter
bool is_ISBN(const string& s){

    if(s.empty()) return false;
    if(count(s.begin(),s.end(),'-') != 3) return false;             // must have exactly 3 hyphens
    if(s.front() == '-' || s.back() == '-') return false;           // no leading/trailing '-'

    vector<pair<int,int>> substring_boundaries = get_bounds(s);
    // Split into 4 parts by '-'
    vector<string> parts;
    for(int i = 0; i < 4; ++i){                                     // must have 4 parts
        int start = substring_boundaries[i].first;
        int end = substring_boundaries[i].second;
        if (end < start) return false;                              // consecutive hyphens
        string part = find_substr(s,start,end);
        if(part.empty()) return false;                              // reject empty groups
        parts.push_back(part);
    }

    if(parts.size() != 4) return false;
    if(!all_digits(parts[0]) || !all_digits(parts[1]) || !all_digits(parts[2])) 
        return false;                                               // first 3 must be numbers

    if(parts[3].size() != 1) return false;                          // last must be 1 char
    unsigned char ch = static_cast<unsigned char> (parts[3][0]);
    return isdigit(ch) || isalpha(ch);                              // digit or letter
    
}


//------------------------------------------- Supporting Types (Book Class) -------------------------------------------//

struct Copyright_year{
    int year;
};

// Handles one or more authors; stored alphabetically
class Author{
    public:
            Author(const string& author1);
            Author(const string& author1, const string& author2);
            Author(const vector<string>& authors_list);
            const vector<string>& book_authors_list() const;
            void add_author(const string& author_name);
    private:
            string first_author;
            string second_author;
            vector<string> author;  // all authors stored here
};

// Single-author constructor
Author::Author(const string& author1): first_author{author1}{
    if(!is_name(author1)) error("Invalid author name: ", author1);
    author.push_back(author1);
}

// Two-author constructor
Author::Author(const string& author1, const string& author2): first_author{author1}, second_author{author2}{
    if(!is_name(author1)) error("Invalid author name: ", author1);
    if(!is_name(author2)) error("Invalid author name: ", author2);
    author.push_back(author1);
    author.push_back(author2);
    sort(author.begin(), author.end());    // keep alphabetical
}

// Constructor from full list
Author::Author(const vector<string>& authors_list): author{authors_list}{
    for(const string& author_name : authors_list)
        if(!is_name(author_name)) error("Invalid author name: ", author_name);
    sort(author.begin(), author.end());
}

// Returns alphabetically sorted author list
const vector<string>& Author::book_authors_list() const{
    if(author.empty()) error("No author found");
    return author;
}

// Add another author and re-sort
void Author::add_author(const string& author_name){
    if(!is_name(author_name)) error("Invalid author name: ", author_name);

    if(first_author == "") first_author = author_name;
    else if (second_author == "") second_author = author_name;

    author.push_back(author_name);
    sort(author.begin(), author.end());
}

// Represents book title with validation
class Title{
    public:
            Title(const string& t) : title{t} { if(!is_title(t)) error("Invalid title: ", t);}
            const string& book_title() const {return title;}
            void modify_title(const string& b_title);
    private:
            string title;
};

// Modify title with validation
void Title::modify_title(const string& b_title){
    if(!is_title(b_title)) error("Invalid title: ", b_title);
    title = b_title;
}

// Define book genres as a strongly typed enum
enum class Genre{
    fiction, nonfiction, periodical, biography, children
};

// Convert Genre → int
int genre_to_int(Genre g){
    return static_cast<int>(g);  
}

// Convert int → Genre (validate range)
Genre int_to_genre(int x){
    if (x >= 0 && x <= 4) 
         return Genre{x}; 
    error("Invalid book genre");
    return Genre{-1}; // warning silencer (non reachable)
}

// Check if int corresponds to a valid Genre
bool isgenre(int x){
    return x >= 0 && x <= 4;
}

ostream& operator << (ostream& os, const Genre& g){
    switch(genre_to_int(g)){
        case(0): os << "fiction"; break;
        case(1): os << "nonfiction"; break;
        case(2): os << "periodical"; break;
        case(3): os << "biography"; break;
        case(4): os << "children"; break;
        default: error("invalid genre.");
    }
    return os;
}

//------------------------------------------------ Book Class ----------------------------------------------------//

class Book{
    public:
            Book(const string& bISBN, const Title& btitle, const Author& bauthor, Genre bgenre, Copyright_year bcopyright_date);
            Book(const string& bISBN, const Title& btitle, const Author& bauthor, Genre bgenre, Copyright_year bcopyright_date, bool bchecked_out);
            
            string book_ISBN() const {return ISBN;}
            string book_title() const {return title.book_title();}
            const vector<string>& book_author() const {return author.book_authors_list();}
            const Genre book_genre() const {return genre;}
            int book_genre_code() const {return genre_to_int(genre);}
            int copyright_year() const {return copyright_date.year;}
            bool is_checkedout() const {return checked_out;}

            void check_in();    // mark as returned
            void check_out();   // mark as borrowed

    private:
            string ISBN = "0-0-0-0";
            Title title;
            Author author;
            Genre genre;
            Copyright_year copyright_date{2001};
            bool checked_out = false;
};

// Main constructor
Book::Book(const string& bISBN, const Title& btitle, const Author& bauthor, Genre bgenre, Copyright_year bcopyright_date) :
ISBN{bISBN}, title{btitle}, author{bauthor}, genre{bgenre},  copyright_date{bcopyright_date}
{
    if(!is_ISBN(bISBN)) error("Invalid ISBN. Expected ISBN: n-n-n-x, n a number and x is either a digit or a letter");
    if(bcopyright_date.year < 0) error ("Invalid copyright date. Expected positive int");
    if(!isgenre(genre_to_int(bgenre))) error("Genre not found");
}

// Constructor with check-out status
Book::Book(const string& bISBN, const Title& btitle, const Author& bauthor, Genre bgenre, Copyright_year bcopyright_date, bool bchecked_out) :
ISBN{bISBN}, title{btitle}, author{bauthor}, genre{bgenre}, copyright_date{bcopyright_date}, checked_out{bchecked_out}
{
    if(!is_ISBN(bISBN)) error("Invalid ISBN. Expected ISBN: n-n-n-x, n a number and x is either a digit or a letter");
    if(bcopyright_date.year < 0) error ("Invalid copyright date. Expected positive int");
    if(!isgenre(genre_to_int(bgenre))) error("Genre not found");
}

// Mark book as checked in (if already checked out)
void Book::check_in(){
    if(is_checkedout()) checked_out = false;
    else error("Book already checked in");
}

// Mark book as checked out (if not already checked out)
void Book::check_out(){
    if(!is_checkedout()) checked_out = true;
    else error("Book already checked out");
}

bool operator== (const Book& a, const Book& b){
    return a.book_ISBN() == b.book_ISBN();
}

bool operator!= (const Book& a, const Book& b){
    return !(a == b);
}

ostream& operator<< (ostream& os, const Book& b){
    os << "Title: " << b.book_title() << '\n';
    os << "Author/s: ";
    for(const string& n : b.book_author())
        os << n << ' ';
    os << '\n';
    os << "ISBN: " << b.book_ISBN() << '\n';
    os << "Genre: " << b.book_genre() << '\n';
    os << "Copyright year: " << b.copyright_year() << '\n';
    return os;
}

//------------------------------------ Utility validation functions (Patron Class) ------------------------------------//

// A valid card number must be exactly 9 digits
bool is_cardnumber(const string& n){
    if(n.size()!= 9) return false;
    if(!all_digits(n)) return false;
    return true;
}

//------------------------------------------- Supporting Types (Book Class) -------------------------------------------//

// Represents a library user (first + last name)
class User{
    public:
            User(string fname, string lname);
            const string& user_first_name() const {return first_name;}
            const string& user_last_name() const {return last_name;}
            const string& user_full_name() const {return full_name; }
            void change_first_name(const string& n);
            void change_last_name(const string& n);
    private:
            string first_name;
            string last_name;
            string full_name;
};

User::User(string fname, string lname) : first_name{fname}, last_name{lname}{
    if(!is_name(fname) || !is_name(lname)) error("Invalid user's name");
    full_name = first_name;
    full_name += " ";
    full_name += last_name;
}

// Validate before assigning new first name
void User::change_first_name(const string& n) {
    if(!is_name(n)) error("Invalid first name. Can't change user's firt name");
    first_name = n;
    full_name = first_name;
    full_name += " ";
    full_name += last_name;
}

// Validate before assigning new last name
void User::change_last_name(const string& n) {
    if(!is_name(n)) error("Invalid last name. Can't change user's last name");
    last_name = n;
    full_name = first_name;
    full_name += " ";
    full_name += last_name;
}

//------------------------------------------------ Patron Class ----------------------------------------------------//

// Represents a library patron (user + card number + fees)
class Patron{
    public:
            Patron(const User& usern, string cardn);
            Patron(const User& usern, string cardn, double fee_owed);

            const string& get_cardnumber() const {return card_number;}
            const string& get_user_first_name() const {return user_name.user_first_name();}
            const string& get_user_last_name() const {return user_name.user_last_name();}
            const string& get_user_full_name() const {return user_name.user_full_name();}
            double get_fee_owed() const {return fee;}
            bool is_fee_owed() const { return fee > 0.0;}

            void charge_fee(double amount){fee = amount;} // overwrite fee balance
            
    private:
            User user_name;
            string card_number = "000000000";
            double fee = 0; // default: no fee

};

// valid user + valid 9-digit card, fee defaults to 0
Patron::Patron(const User& usern, string cardn) : user_name{usern}, card_number{cardn} {
    if(!is_cardnumber(cardn)) error("Invalid card number. Expected a 9-digit char array");
}

// same as above, but with explicit fee
Patron::Patron(const User& usern, string cardn, double fee_owed) : user_name{usern}, card_number{cardn}, fee{fee_owed} {
    if(fee < 0) error("Invalid fee charged. Expected 0 or > 0 double.");
    if(!is_cardnumber(cardn)) error("Invalid card number. Expected a 9-digit char array");
}

bool operator== (const Patron& a, const Patron& b){
    return a.get_cardnumber() == b.get_cardnumber();
}

bool operator!= (const Patron& a, const Patron& b){
    return !(a==b);
}

//------------------------------------------------ Library Class ----------------------------------------------------//

class Library{

    public:
            struct Transaction { // Record of a single checkout
                Book book; Patron patron; Date date;
                Transaction(const Book& b, const Patron& p, const Date& d) : book{b}, patron{p}, date{d}{}
            };

            void add_book(const Book& book) { books.push_back(book); } // allows multiple copies of the same book
            const vector<Book>& get_bookList() const { return books; }
    
            void add_patron (const Patron& patron);
            const vector<Patron>& get_patronList() const { return patrons; }

            bool is_book_available(const Book& book) const;             // true if any copy is free
            bool is_patron_registered (const Patron& patron) const;     // check card number match


            void check_out_book(const Book& book, const Patron& patron, const Date& date);
            const vector<Transaction>& get_transactions_History() const { return transactions; }
            vector<string> get_usersOwingFees () const;                 // names of debtors
    private:
            vector<Book> books;
            vector<Patron> patrons;
            vector<Transaction> transactions;

};
 
bool Library::is_book_available(const Book& book) const {

    for(const Book& b : books) 
        if (book == b && !b.is_checkedout()) return true;
    return false;
}

bool Library::is_patron_registered (const Patron& patron) const {

    for (const Patron& p : patrons)
        if(patron == p) return true;
    return false;
}

void Library::add_patron(const Patron& patron){

    for(const Patron& p : patrons)
        if (p == patron) error("Patron already exists in the system.");
    patrons.push_back(patron);
}

void Library::check_out_book(const Book& book, const Patron& patron, const Date& date){

    if(!is_book_available(book)) error("Book not available.");
    if(!is_patron_registered(patron)) error("Customer not registered in the system.");
    if(patron.is_fee_owed()) error("Fees owed! Transaction can't be completed");
    
    
    for (Book& b : books) {
        if (b == book && !b.is_checkedout()) {   // mark one available copy checked out + log transaction
            b.check_out();
            transactions.push_back(Transaction{b, patron, date});
            return;
        }
    }
    error("Internal error: book vanished during checkout");
}

vector<string> Library::get_usersOwingFees () const{
    vector<string> out;
    for(const Patron& patron : patrons){
        if(patron.is_fee_owed())  out.push_back(patron.get_user_full_name());
    }
    return out;
}

//--------------------------------------- Minimal test harness --------------------------------------//

int passed = 0, failed = 0;
void check(const char* label, bool ok) {
    if (ok) { cout << "[PASS] " << label << "\n"; ++passed; }
    else    { cout << "[FAIL] " << label << "\n"; ++failed; }
}
void expect_throw(const char* label, const std::function<void()>& f) {
    try { f(); cout << "[FAIL] " << label << " (no exception)\n"; ++failed; }
    catch (const std::exception&) { cout << "[PASS] " << label << "\n"; ++passed; }
}

//--------------------------------------- Targeted Tests ---------------------------------------------//

int main() {
    // ---- Setup common objects ----
    Title t1{"Effective C++"};
    Author a1{"Scott Meyers"};
    Book b1{"1-2-3-X", t1, a1, Genre::nonfiction, Copyright_year{2005}};

    Title t2{"Clean Code"};
    Author a2{"Robert Martin"};
    Book b2{"9-9-9-9", t2, a2, Genre::nonfiction, Copyright_year{2008}};

    // A second physical copy of the same ISBN to test multi-copy behavior
    Book b1_copy{"1-2-3-X", t1, a1, Genre::nonfiction, Copyright_year{2005}};

    User u1{"Rami", "Omer"};
    Patron p1{u1, "123456789"};

    User u2{"Ana", "Lopez"};
    Patron p2{u2, "987654321", 5.0}; // owes fees

    Library lib;
    lib.add_book(b1);
    lib.add_book(b1_copy);
    lib.add_book(b2);
    lib.add_patron(p1);

    // ---- Date sanity ----
    check("Date: valid", []{
        Date d{ Year{2025}, Month::sep, Day{2} };
        return d.get_year()==2025 && d.get_month_code()==9 && d.get_day()==2;
    }());

    expect_throw("Date: invalid (Feb 30) throws", []{
        Date bad{ Year{2023}, Month::feb, Day{30} };
        (void)bad;
    });

    // ---- Availability before checkout ----
    check("Availability: b1 available before checkout", lib.is_book_available(b1));
    check("Availability: b2 available before checkout", lib.is_book_available(b2));

    // ---- Checkout success path ----
    {
        Date today{ Year{2025}, Month::sep, Day{2} };
        lib.check_out_book(b1, p1, today);        // should consume one copy of b1
        check("After 1st checkout, b1 still available (due to 2nd copy)",
              lib.is_book_available(b1));
    }

    // ---- Second checkout of same ISBN consumes second copy ----
    {
        Date today{ Year{2025}, Month::sep, Day{2} };
        lib.check_out_book(b1, p1, today);        // second copy
        check("After 2nd checkout, b1 no longer available",
              !lib.is_book_available(b1));
    }

    // ---- Error: book not available (both copies out) ----
    expect_throw("Checkout: unavailable book errors", [&]{
        lib.check_out_book(b1, p1, Date{Year{2025}, Month::sep, Day{3}});
    });

    // ---- Error: unregistered patron ----
    expect_throw("Checkout: unregistered patron errors", [&]{
        Patron someoneElse{ User{"Mia","Gray"}, "111222333" }; // not added
        lib.check_out_book(b2, someoneElse, Date{Year{2025}, Month::sep, Day{3}});
    });

    // ---- Error: patron owes fees ----
    expect_throw("Checkout: patron with fees errors", [&]{
        // Not added yet; also test registration requirement first by adding
        lib.add_patron(p2);
        lib.check_out_book(b2, p2, Date{Year{2025}, Month::sep, Day{3}});
    });

    // ---- Duplicate patron guard ----
    expect_throw("Add patron: duplicate card rejected", [&]{
        lib.add_patron(p1); // same card number as existing
    });

    // ---- Users owing fees list ----
    {
        auto debtors = lib.get_usersOwingFees();
        check("Fees list contains Ana Lopez",
              std::find(debtors.begin(), debtors.end(), p2.get_user_full_name()) != debtors.end());
    }

    // ---- Transactions recorded ----
    {
        const auto& tx = lib.get_transactions_History();
        check("Transactions: at least 2 successful checkouts recorded", tx.size() >= 2);
        if (!tx.empty()) {
            // Light sanity on first tx
            check("Transaction[0]: patron name non-empty", !tx[0].patron.get_user_full_name().empty());
        }
    }

    cout << "\n=== SUMMARY ===\nPassed: " << passed << "\nFailed: " << failed << "\n";
    return failed ? 1 : 0;
}

