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
    if (!isalpha(n.front()) || !isalpha(n.back())) return false;

    for (int i = 0; i < n.size(); ++i) {
        unsigned char ch = n[i];

        // Must be alphabetic, '-', '\'', or space
        if (!isalpha(ch) && ch != '-' && ch != '\'' && ch != ' ')
            return false;

        // Reject consecutive non-alphabetic characters
        if (i > 0) {
            unsigned char prev = n[i-1];
            if (!isalpha(prev) && !isalpha(ch)) return false;
        }
    }

    return true;
}

// Validate a book title: must start with a letter or digit
bool is_title(const string& title) {
    if (title.empty()) return false;  // guard against empty titles
    
    unsigned char ch = title.front();
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
            Author(string author1);
            Author(string author1, string author2);
            Author(const vector<string>& authors_list);
            const vector<string>& book_authors_list() const;
            void add_author(const string& author_name);
    private:
            string first_author;
            string second_author;
            vector<string> author;  // all authors stored here
};

// Single-author constructor
Author::Author(string author1): first_author{author1}{
    if(!is_name(author1)) error("Invalid author name: ", author1);
    author.push_back(author1);
}

// Two-author constructor
Author::Author(string author1, string author2): first_author{author1}, second_author{author2}{
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
            Title(string t) : title{t} { if(!is_title(t)) error("Invalid title: ", t);}
            const string& book_title() const {return title;}
            void modify_title(string b_title);
    private:
            string title;
};

// Modify title with validation
void Title::modify_title(string b_title){
    if(!is_title(b_title)) error("Invalid title: ", b_title);
    title = b_title;
}


//------------------------------------------------ Book Class ----------------------------------------------------//
class Book{
    public:
            Book(string bISBN, Title btitle, Author bauthor, Copyright_year bcopyright_date);
            Book(string bISBN, Title btitle, Author bauthor, Copyright_year bcopyright_date, bool bchecked_out);
            
            string book_ISBN() const {return ISBN;}
            string book_title() const {return title.book_title();}
            const vector<string>& book_author() const {return author.book_authors_list();}
            int copyright_year() const {return copyright_date.year;}
            bool is_checkedout() const {return checked_out;}

            void check_in();    // mark as returned
            void check_out();   // mark as borrowed

    private:
            string ISBN = "0-0-0-0";
            Title title;
            Author author;
            Copyright_year copyright_date{2001};
            bool checked_out = false;
};

// Main constructor
Book::Book(string bISBN, Title btitle, Author bauthor, Copyright_year bcopyright_date) :
ISBN{bISBN}, title{btitle}, author{bauthor}, copyright_date{bcopyright_date}
{
    if(!is_ISBN(bISBN)) error("Invalid ISBN. Expected ISBN: n-n-n-x, n a number and x is either a digit or a number");
    if(bcopyright_date.year < 0) error ("Invalid copyright date. Expected positive int");
}

// Constructor with check-out status
Book::Book(string bISBN, Title btitle, Author bauthor, Copyright_year bcopyright_date, bool bchecked_out) :
ISBN{bISBN}, title{btitle}, author{bauthor}, copyright_date{bcopyright_date}, checked_out{bchecked_out}
{
    if(!is_ISBN(bISBN)) error("Invalid ISBN. Expected ISBN: n-n-n-x, n a number and x is either a digit or a number");
    if(bcopyright_date.year < 0) error ("Invalid copyright date. Expected positive int");
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

int main() {
    try {
        cout << "=== Construct a valid Book ===\n";
        Title t1{"1984"};
        Author a1{vector<string>{"George", "Orwell"}};        // names sorted internally
        Copyright_year cy1{1949};
        Book b1{"978-0-452-8", t1, a1, cy1};                  // n-n-n-x (x can be letter or digit)

        cout << "ISBN: " << b1.book_ISBN() << "\n";
        cout << "Title: " << b1.book_title() << "\n";
        cout << "Authors: ";
        for (const auto& s : b1.book_author()) cout << s << " ";
        cout << "\nYear: " << b1.copyright_year() << "\n";
        cout << "Checked out? " << (b1.is_checkedout() ? "yes" : "no") << "\n\n";

        cout << "=== CHECK-OUT / CHECK-IN Functions ===\n";
        cout << "Check out...\n";
        b1.check_out();
        cout << "Checked out? " << (b1.is_checkedout() ? "yes" : "no") << " (expected: yes)\n";

        cout << "Try to check out again (should throw)...\n";
        try {
            b1.check_out();
            cout << "ERROR: double check_out() did not throw\n";
        } catch (exception& e) {
            cout << "Caught expected error: " << e.what() << "\n";
        }

        cout << "Check in...\n";
        b1.check_in();
        cout << "Checked out? " << (b1.is_checkedout() ? "yes" : "no") << " (expected: no)\n";

        cout << "Try to check in again (should throw)...\n";
        try {
            b1.check_in();
            cout << "ERROR: double check_in() did not throw\n";
        } catch (exception& e) {
            cout << "Caught expected error: " << e.what() << "\n";
        }
        cout << "\n";

        cout << "=== MODIFY TITLE & ADD AUTHOR ===\n";
        Title t2{"A Tale of Two Cities"};
        Author a2{"Charles"};
        Book b2{"123-456-789-X", t2, a2, {1859}, /*checked_out=*/false};

        cout << "Original title: " << b2.book_title() << "\n";
        t2.modify_title("A Tale of Two Cities (Revised)");
        // Note: b2 holds its own Title; the change above affects t2, not b2.
        // To test the mutator on the book's title, rebuild book with new Title:
        Book b3{"123-456-789-X", t2, a2, {1859}};
        cout << "Updated title in new book: " << b3.book_title() << "\n";

        cout << "Authors before add: ";
        for (const auto& s : b3.book_author()) cout << s << " ";
        cout << "\n";
        a2.add_author("Dickens");   // add to the author object you pass into Book
        Book b4{"123-456-789-X", t2, a2, {1859}}; // recompose to see the updated list in a Book
        cout << "Authors after add: ";
        for (const auto& s : b4.book_author()) cout << s << " ";
        cout << "\n\n";

        cout << "=== NEGATIVE CASES (should throw) ===\n";

        cout << "Invalid ISBN (wrong hyphen count)...\n";
        try {
            Book bad_isbn1{"123-45-6789X", Title{"Good"}, Author{"Alice"}, {2000}};
            cout << "ERROR: bad_isbn1 constructed but should have failed\n";
        } catch (exception& e) { cout << "Caught expected: " << e.what() << "\n"; }

        cout << "Invalid ISBN (empty group)…\n";
        try {
            Book bad_isbn2{"123--45-6-X", Title{"Good"}, Author{"Alice"}, {2000}};
            cout << "ERROR: bad_isbn2 constructed but should have failed\n";
        } catch (exception& e) { cout << "Caught expected: " << e.what() << "\n"; }

        cout << "Invalid ISBN (last part > 1 char)…\n";
        try {
            Book bad_isbn3{"1-2-3-XY", Title{"Good"}, Author{"Alice"}, {2000}};
            cout << "ERROR: bad_isbn3 constructed but should have failed\n";
        } catch (exception& e) { cout << "Caught expected: " << e.what() << "\n"; }

        cout << "Invalid Title (empty)…\n";
        try {
            Title bad_title{""};
            Book bad{"1-2-3-X", bad_title, Author{"Alice"}, {2000}};
            cout << "ERROR: bad title constructed but should have failed\n";
        } catch (exception& e) { cout << "Caught expected: " << e.what() << "\n"; }

        cout << "Invalid Author name (bad chars)…\n";
        try {
            Author bad_author{"Al!ce"};
            Book bad{"1-2-3-X", Title{"Ok"}, bad_author, {2000}};
            cout << "ERROR: bad author constructed but should have failed\n";
        } catch (exception& e) { cout << "Caught expected: " << e.what() << "\n"; }

        cout << "Invalid copyright year (negative)…\n";
        try {
            Book bad{"1-2-3-X", Title{"Ok"}, Author{"Alice"}, {-10}};
            cout << "ERROR: bad year constructed but should have failed\n";
        } catch (exception& e) { cout << "Caught expected: " << e.what() << "\n"; }

        cout << "\nAll tests completed.\n";
        return 0;

    } catch (exception& e) {
        cerr << "UNCAUGHT: " << e.what() << '\n';
        return 1;
    }
}