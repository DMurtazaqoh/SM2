#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include <chrono>
#include <ctime>

using namespace std;

struct Book {
    string title;
    string Jenis;
    int rackNumber;
    int shelfNumber;
};

// Pinjam
struct Loan {
    string studentName;
    string studentID;
    string bookTitle;
    string dueDate;
};

// Syajarotun
struct TreeNode {
    Book book;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Book b) : book(b), left(nullptr), right(nullptr) {}
};

// Graph rak
struct GraphNode {
    int rackNumber;
    vector<GraphNode*> neighbors;
};

unordered_map<int, vector<Book>> library; 
vector<Loan> loans; 
TreeNode* bookTree = nullptr; 
map<int, GraphNode*> libraryGraph; 
const vector<string> VALID_LIBRARIAN_PASSWORDS = { "180605", "190571", "291265", "231096", "260620" }; // Password khusus petugasnya
const unordered_map<string, int> Jenis_TO_RACK = { 
    {"Non fiksi", 1}, 
    {"Karya ilmiah", 2}, 
    {"Novel", 3}, 
    {"Komik", 4}, 
    {"Majalah", 5} 
}; 

// sorting, mengurutkan nama siswa sesuai alphabet
bool compareByStudentName(const Loan& a, const Loan& b) {
    return a.studentName < b.studentName;
}

void addBookToTree(TreeNode*& node, Book book) {
    if (!node) {
        node = new TreeNode(book);
    } else if (book.title < node->book.title) {
        addBookToTree(node->left, book);
    } else {
        addBookToTree(node->right, book);
    }
}

Book* searchBookInTree(TreeNode* node, const string& title) {
    if (!node) return nullptr;
    if (node->book.title == title) return &node->book;
    if (title < node->book.title) return searchBookInTree(node->left, title);
    return searchBookInTree(node->right, title);
}

bool isShelfNumberOccupied(int rackNumber, int shelfNumber) {
    for (const Book& book : library[rackNumber]) {
        if (book.shelfNumber == shelfNumber) {
            return true;
        }
    }
    return false;
}

// Collision handling is here
void addBook() {
    Book book;
    cout << "Masukkan judul buku: ";
    cin.ignore();
    getline(cin, book.title);

    // Cek apakah buku dengan judul yang sama dah ada
    if (searchBookInTree(bookTree, book.title)) {
        cout << "Buku dengan judul yang sama sudah ada. Mohon masukkan judul yang berbeda.\n";
        return;
    }

    cout << "Masukkan Jenis buku: ";
    getline(cin, book.Jenis);
    
    // Masukkan ke rak
    if (Jenis_TO_RACK.find(book.Jenis) != Jenis_TO_RACK.end()) {
        book.rackNumber = Jenis_TO_RACK.at(book.Jenis);
    } else {
        cout << "Jenis buku tidak dikenal. Buku tidak dapat ditambahkan.\n";
        return;
    }
    
    cout << "Masukkan nomor baris dalam rak: ";
    cin >> book.shelfNumber;

    //cari baris kosong kalau baris yang dituju terisi
    //sebenarnya irl baris bisa diisi lebih dari 1, bingung (?)
    while (isShelfNumberOccupied(book.rackNumber, book.shelfNumber)) {
        cout << "Baris " << book.shelfNumber << " pada rak " << book.rackNumber << " sudah ditempati. ";
        book.shelfNumber++;
        cout << "Mencoba menempatkan buku di baris " << book.shelfNumber << ".\n";
    }

    // Tambahkan buku ke hash table
    library[book.rackNumber].push_back(book);

    // Tambahkan buku ke tree
    addBookToTree(bookTree, book);

    // Tambahkan rak ke graph 
    if (libraryGraph.find(book.rackNumber) == libraryGraph.end()) {
        libraryGraph[book.rackNumber] = new GraphNode{ book.rackNumber };
    }

    cout << "Buku berhasil ditambahkan ke rak " << book.rackNumber << " pada baris " << book.shelfNumber << ".\n";
    system("pause");
    system("cls");
}

void displayLoans() {
    if (loans.empty()) {
        cout << "Belum ada peminjaman.\n";
    } else {
        sort(loans.begin(), loans.end(), compareByStudentName); 

        for (const auto& loan : loans) {
            cout << "Nama: " << loan.studentName << ", NIM: " << loan.studentID 
                 << ", Judul Buku: " << loan.bookTitle << ", Tanggal Kembali: " << loan.dueDate << endl;
        }
    }
    system("pause");
    system("cls");
}

// Menampilkan isi rak berdasarkan nomor rak
void displayRackContents() {
    int rackNumber;
    cout << "Masukkan nomor rak: ";
    cin >> rackNumber;
    
    if (library.find(rackNumber) != library.end()) {
        cout << "Buku pada rak " << rackNumber << ":\n";
        for (const Book& book : library[rackNumber]) {
            cout<< "Baris : " << book.shelfNumber << ", Judul : " << book.title << ", Jenis Buku : " << book.Jenis << endl;
        }
    } else {
        cout << "Rak dengan nomor " << rackNumber << " kosong.\n";
    }
    system("pause");
    system("cls");
}

void librarianMenu() {
    while (true) {
        int choice;
        cout << "1. Tambah Buku\n2. Lihat Peminjaman Hari Ini\n3. Lihat Isi Rak\n4. Keluar\n";
        cout << "Pilih opsi: ";
        cin >> choice;
        system("cls");
        if (choice == 1) {
            addBook();
        } else if (choice == 2) {
            displayLoans();
        } else if (choice == 3) {
            displayRackContents();
        } else {
            break;
        }
    }
}

void searchBook() {
    string title;
    cout << "Masukkan judul buku yang dicari: ";
    cin.ignore();
    getline(cin, title);

    Book* foundBook = searchBookInTree(bookTree, title);
    if (foundBook) {
        cout << "Buku ditemukan pada rak " << foundBook->rackNumber << " baris " << foundBook->shelfNumber << ".\n";
    } else {
        cout << "Buku tidak ditemukan.\n";
    }
    system("pause");
    system("cls");
}

//menyambungkan ke kalender real life
string getDueDate(int days) {
    auto now = chrono::system_clock::now();
    auto due_time = now + chrono::hours(24 * days);
    time_t due_time_t = chrono::system_clock::to_time_t(due_time);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d", localtime(&due_time_t));
    return string(buffer);
}

void removeBookFromLibrary(const string& title) {
    for (auto& it : library) {
        auto& books = it.second;
        auto bookIt = remove_if(books.begin(), books.end(), [&](const Book& book) {
            return book.title == title;
        });
        if (bookIt != books.end()) {
            books.erase(bookIt, books.end());
            break;
        }
    }
}

void borrowBook() {
    Loan loan;
    cout << "Masukkan nama: ";
    cin.ignore();
    getline(cin, loan.studentName);
    cout << "Masukkan NIM: ";
    cin >> loan.studentID;
    cout << "Masukkan judul buku yang akan dipinjam: ";
    cin.ignore();
    getline(cin, loan.bookTitle);

    // Periksa apakah buku sudah dipinjam
    for (const auto& loanedBook : loans) {
        if (loanedBook.bookTitle == loan.bookTitle) {
            cout << "Maaf, buku \"" << loan.bookTitle << "\" sudah dipinjam " << "\n";
            system("pause");
            system("cls");
            return;
        }
    }

    Book* foundBook = searchBookInTree(bookTree, loan.bookTitle);
    if (foundBook) {
        loan.dueDate = getDueDate(3);
        loans.push_back(loan);
        removeBookFromLibrary(loan.bookTitle);
        cout << "Buku berhasil dipinjam. Kembalikan buku pada (" << loan.dueDate << ").\n";
    } else {
        cout << "Buku tidak ditemukan. Peminjaman gagal.\n";
    }

    system("pause");
    system("cls");
}

void studentMenu() {
    while (true) {
        int choice;
        cout << "1. Cari Buku\n2. Peminjaman Buku\n3. Keluar\n";
        cout << "Pilih opsi: ";
        cin >> choice;
        system("cls");
        if (choice == 1) {
            searchBook();
        } else if (choice == 2) {
            borrowBook();
        } else {
            break;
        }
    }
}

bool isValidLibrarianPassword(const string& password) {
    return find(VALID_LIBRARIAN_PASSWORDS.begin(), VALID_LIBRARIAN_PASSWORDS.end(), password) != VALID_LIBRARIAN_PASSWORDS.end();
}

int main() {
    cout << "\n";
    cout << "\t=========================================\n";
    cout << "\t  Selamat Datang di Perpustakaan >< !! \n";
    cout << "\t=========================================\n\n";
    system("pause");
    system("cls");
    while (true) {
        int role;
        cout << "1. Masuk sebagai Petugas Perpustakaan\n2. Masuk sebagai Siswa\n3. Keluar\n";
        cout << "Pilih peran: ";
        cin >> role;

        system("cls");

        if (role == 1) {
            string password;
            cout << "Masukkan password: ";
            cin >> password;
            system("cls");
            if (isValidLibrarianPassword(password)) {
                librarianMenu();
            } else {
                cout << "Password salah.\n";
                system("pause");
                system("cls");
            }
        } else if (role == 2) {
            string studentID;
            cout << "Masukkan NIM: ";
            cin >> studentID;
            system("cls");
            studentMenu();
        } else {
            break;
        }
    }

    return 0;
}