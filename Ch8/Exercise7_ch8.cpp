#include "PPPheaders.h"

//------------------------------------ Utility validation functions ------------------------------------//

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


//------------------------------------------- Supporting Types -------------------------------------------//

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

int main(){
     try {
        // ---------- Construct authors ----------
        Author a1{"Mary-Jane"};
        Author a2{"Mary-Jane", "John O'Neil"};
        vector<string> team = {"Zoe Armitage", "Alice-Beth", "Carlos"};
        Author a3{team};

        // ---------- Titles ----------
        Title t1{"Systems Programming"};
        Title t2{"C++ From Scratch"};
        Title t3{"Kids Corner 101"};

        // ---------- Valid books ----------
        Book b1{"123-456-7-X", t1, a2, Genre::nonfiction,  {2014}};
        Book b2{"123-456-7-X", t2, a1, Genre::nonfiction,  {2020}}; // same ISBN as b1
        Book b3{"9-88-777-5",  t3, a3, Genre::children,    {2023}};

        // ---------- Print (operator<<) ----------
        cout << "---- b1 ----\n" << b1 << "\n\n";
        cout << "---- b3 ----\n" << b3 << "\n\n";

        // ---------- Equality/Inequality (ISBN only) ----------
        cout << "b1 == b2 ? " << (b1 == b2 ? "true" : "false") << "\n"; // expect true
        cout << "b1 != b3 ? " << (b1 != b3 ? "true" : "false") << "\n"; // expect true

        // ---------- Genre helpers ----------
        cout << "b3 genre: " << b3.book_genre()
             << " (code=" << b3.book_genre_code() << ")\n\n";

        // ---------- Check-out / Check-in ----------
        cout << "b1 checked out? " << (b1.is_checkedout() ? "yes" : "no") << "\n";
        b1.check_out();
        cout << "b1 checked out? " << (b1.is_checkedout() ? "yes" : "no") << "\n";
        b1.check_in();
        cout << "b1 checked out? " << (b1.is_checkedout() ? "yes" : "no") << "\n\n";

        // ---------- Negative tests (should throw) ----------
        try {
            Book bad_isbn{"12-34-X", t1, a1, Genre::fiction, {2021}}; // malformed ISBN
            cout << "ERROR: constructed bad_isbn (should not happen)\n";
        } catch (exception& e) {
            cout << "[expected]" << e.what() << "\n";
        }

        try {
            auto bad_g = int_to_genre(99); // invalid genre code
            (void)bad_g;
            cout << "ERROR: int_to_genre(99) did not throw\n";
        } catch (exception& e) {
            cout << "[expected] " << e.what() << "\n";
        }

        try {
            b1.check_in(); // already checked in
            cout << "ERROR: b1.check_in() should have thrown\n";
        } catch (exception& e) {
            cout << "[expected] double check-in: " << e.what() << "\n";
        }

        b1.check_out();
        try {
            b1.check_out(); // already checked out
            cout << "ERROR: b1.check_out() should have thrown\n";
        } catch (exception& e) {
            cout << "[expected] double check-out: " << e.what() << "\n";
        }

        // ---------- Author sorting & exposure ----------
        cout << "\nAuthors in a3 (sorted): ";
        for (const auto& n : a3.book_authors_list()) cout << n << " | ";
        cout << "\n";

    } catch (exception& e) {
        cerr << "UNEXPECTED ERROR: " << e.what() << "\n";
    }
}
