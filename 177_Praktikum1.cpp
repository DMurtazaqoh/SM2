#include <iostream>
#include <list> //memasukkan fasilitas list
#include <string>
#include <algorithm>
#include <limits> //nilai-nilai numerik
#include <cmath> 

using namespace std;

struct Tanggal {
    int hari, bulan, tahun;

    void masukkanTanggal() {
        cout << "Masukkan Tgl Lahir : ";
        cin >> hari >> bulan >> tahun;
    }
    
    int hitungDigit() const {
        return (hari % 10 + bulan % 10 + tahun % 10) % 9; // perhitungan digit id dari tanggal lahir
    }
};

struct Supir {
    string Id;
    string Nama;
    Tanggal TglLahir;
    char Kelamin;
    string Alamat;

    void tampilkanSupir() const {
        cout << "ID: " << Id << ", Nama: " << Nama << ", Kelamin: " << Kelamin << ", Alamat: " << Alamat << endl;
        cout << "Tanggal Lahir: " << TglLahir.hari << "-" << TglLahir.bulan << "-" << TglLahir.tahun << endl;
    }
};

// function
void tambahSupir(list<Supir>& daftarSupir);
void editSupir(list<Supir>& daftarSupir);
void hapusSupir(list<Supir>& daftarSupir);
void navigasiSupir(list<Supir>& daftarSupir);

void handleAdmin(list<Supir>& daftarSupir); //fungsi yang menyediakan untuk adm
void handleUser(list<Supir>& daftarSupir); // sebaliknya

string generateId(const Supir& supir, const list<Supir>& daftarSupir);
// first parameter : fungsi ini menggunakan data dari variabel supir untuk menghasilkan id
// second parameter : untuk memeriksa agar tidak ada kesamaan id 
// const : variabel supir dan listnya tidak akan terubah 

struct Interface {
    static void menu() {
        cout << "\t====================\n";
        cout << "\t  Selamat Datang !! \n";
        cout << "\t====================\n";
        cout << "\t1. Masuk Sebagai Admin \n";
        cout << "\t2. Masuk Sebagai User \n";
        cout << "\t0. Keluar \n";
        cout << "\tPilih peran: ";
    }

    static void admin() {
        cout << "\t=== ADMINISTRATOR MENU ===\n";
        cout << "\t1. Mencari Data Supir (navigasi)\n";
        cout << "\t2. Menghapus Data Supir (hapus)\n";
        cout << "\t3. Mengubah Data Supir (edit)\n";
        cout << "\t4. Menambah Data Supir (tambah)\n";
        cout << "\t0. Kembali ke Menu Utama \n";
        cout << "\tMasukkan pilihan: ";
    }

    static void user() {
        cout << "\t=== USER MENU ===\n";
        cout << "\t1. Lihat Daftar Supir (navigasi)\n";
        cout << "\t0. Kembali ke Menu Utama \n";
        cout << "\tMasukkan pilihan: ";
    }
};

int main() {
    list<Supir> daftarSupir; // inisialisasi
    int pilihan;

    do {
        Interface::menu();
        cin >> pilihan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // nampilin menu ke pengguna, setelah diinput variabelnya disimpan

        switch (pilihan) { //pilihan admin atau user
            case 1:
                handleAdmin(daftarSupir);
                break;
            case 2:
                handleUser(daftarSupir);
                break;
        }
    } while (pilihan != 0);

    return 0;
}

// masuk ke menu pilihan admin 
void handleAdmin(list<Supir>& daftarSupir) {
    int perintah;
    do {
        Interface::admin();
        cin >> perintah;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // cin ignore : membersihkan after the inputing

        switch (perintah) {
            case 1:
                navigasiSupir(daftarSupir);
                break;
            case 2:
                hapusSupir(daftarSupir);
                break;
            case 3:
                editSupir(daftarSupir);
                break;
            case 4:
                tambahSupir(daftarSupir);
                break;
        }
    } while (perintah != 0);
}

void handleUser(list<Supir>& daftarSupir) {
    int perintah;
    do {
        Interface::user();
        cin >> perintah;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (perintah == 1) { //jika memilih perintah 1 maka
            navigasiSupir(daftarSupir); // navigasi supir atau program untuk pencarian dipilih
        }
    } while (perintah != 0);
}

void tambahSupir(list<Supir>& daftarSupir) {
    Supir baru;
    cout << "Nama : > ";
    getline(cin, baru.Nama);
    cout << "Tgl Lahir : > ";
    baru.TglLahir.masukkanTanggal();
    cout << "Kelamin (l/p) : > ";
    cin >> baru.Kelamin;
    cin.ignore();
    baru.Kelamin = tolower(baru.Kelamin); // tolower itu untuk merubah karakter menjadi huruf kecil
    // kalau diganti tanpa tolower akan error karena akan menjadi string dan string gabisa menyimpan 2 karakter
    cout << "Alamat : > ";
    getline(cin, baru.Alamat);

    baru.Id = generateId(baru, daftarSupir);
    daftarSupir.push_back(baru);
    //untuk menambahkan data supir baru ke dalam list (push)

    cout << "Data berhasil ditambahkan dengan ID: " << baru.Id << "!\n";
}
//menghasilkan id 
//salah :(
string generateId(const Supir& supir, const list<Supir>& daftarSupir) {
    //untuk mencari posisi dari alphabet
    // +1 agar dimulai dari 1 bukan 0
    //absolut = memastikan positif, harusnya gapake
    int firstLetterPos = abs(supir.Nama.front() - 'A') + 1;
    int lastLetterPos = abs(supir.Nama.back() - 'A') + 1;
    // lewat gender
    int genderDigit = (supir.Kelamin == 'l') ? 1 : 0;
    //dari tanggal lahir
    int birthDigit = supir.TglLahir.hitungDigit();
    string baseId = to_string(abs(firstLetterPos - lastLetterPos)) + to_string(genderDigit) + to_string(birthDigit);
    // menggabungkan semuanya jadi satu or jadi ID

    if (baseId.length() < 4) baseId.insert(0, 2 - baseId.length(), '0'); //baseid = memeriksa tidak ada id yang akan sama //string

    // mencari id yang serupa
    int duplicateCount = 0;
    //read only data
    for (const auto& s : daftarSupir) {
        // jika find = 0 , kemungkinan id nya serupa
        if (s.Id.find(baseId) == 0) {
            duplicateCount++;
        }
    }

    if (duplicateCount > 0) {
        baseId += "-" + to_string(duplicateCount); //agar tidak ada yang duplikat ; ngecek
    }

    return baseId;
}

//bagian pengeditan supir
void editSupir(list<Supir>& daftarSupir) {
    if (daftarSupir.empty()) {
        cout << "Tidak ada data supir untuk diedit.\n";
        return;
    }

    string id;
    cout << "Masukkan ID supir yang ingin diedit: ";
    cin >> id;
    cin.ignore();

    for (auto& supir : daftarSupir) { // auto = agar tidak perlu nulis tipe data secara terperinci
    //jadi masuk ke "supir" dalam daftar supir
        if (supir.Id == id) { //if true
            cout << "Mengedit Supir dengan ID: " << supir.Id << endl;
            cout << "Nama baru: ";
            getline(cin, supir.Nama);
            cout << "Tgl Lahir baru : ";
            supir.TglLahir.masukkanTanggal();
            cout << "Kelamin baru (l/p): ";
            cin >> supir.Kelamin;
            supir.Kelamin = tolower(supir.Kelamin); 
            cin.ignore();
            cout << "Alamat baru: ";
            getline(cin, supir.Alamat);
            cout << "Data berhasil diperbarui.\n";
            return;
        }
    }

    cout << "Supir dengan ID: " << id << " tidak ditemukan.\n";
}
 //bagian penghapusan
void hapusSupir(list<Supir>& daftarSupir) {
    if (daftarSupir.empty()) {
        cout << "Tidak ada data supir untuk dihapus.\n";
        return;
    }

    string id;
    cout << "Masukkan ID supir yang ingin dihapus: ";
    cin >> id;

    auto it = find_if(daftarSupir.begin(), daftarSupir.end(), [&id](const Supir& supir) {
        // auto it find if = cari data yang rumit
        // pencarian data supir dari awal-akhir
        return supir.Id == id;
    });

    if (it != daftarSupir.end()) {
        daftarSupir.erase(it);
        cout << "Data supir dengan ID: " << id << " telah dihapus.\n";
    } else {
        cout << "Supir dengan ID: " << id << " tidak ditemukan.\n";
    }
}

//pencarian
void navigasiSupir(list<Supir>& daftarSupir) {
    if (daftarSupir.empty()) {
        cout << "Belum ada data supir.\n";
        return;
    }

    auto it = daftarSupir.begin();
    int pilihan;

    do {
        cout << "Data Supir saat ini:\n";
        it->tampilkanSupir();

        cout << "1. Supir Selanjutnya\n";
        cout << "2. Supir Sebelumnya\n";
        cout << "0. Kembali\n";
        cout << "Pilihan: ";
        cin >> pilihan;

        if (pilihan == 1) {
            if (next(it) != daftarSupir.end()) {
                ++it;
            } else {
                cout << "Ini adalah data supir terakhir.\n";
            }
        } else if (pilihan == 2) {
            if (it != daftarSupir.begin()) {
                --it;
            } else {
                cout << "Ini adalah data supir pertama.\n";
            }
        }
    } while (pilihan != 0);
}