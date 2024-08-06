#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
using namespace std;

struct Layanan {
   void MenuDalamP(){
    cout << "\t =========================\n";
    cout << "\t       1. Wahana          \n";
    cout << "\t       2. Aquatic         \n";
    cout << "\t       3. Complete        \n";
    cout << "\t       4. Keluar          \n";
    cout << "\t =========================\n";
   }

   void Sistem(){
    cout << "\t ================================\n";
    cout << "\t     1. 1 ticket Wahana Aquatic  \n";
    cout << "\t     2. 2 ticket Wahana Aquatic  \n";
    cout << "\t     3. 3 ticket Wahana Aquatic  \n";
    cout << "\t     4. Keluar                   \n";
    cout << "\t ================================\n";
   }
};

struct Hasil {
    int Total1 = 0;
    int Total2 = 0;
    int Total3 = 0;
    int Total4 = 0;
    int Total5 = 0;
    int Total6 = 0;
};

struct Hewan {
    char nama[50];
    char spesies[50];
    char tanggalLahir[20];
    Hewan* next;
};

struct Kandang {
    int id;
    char spesies[50];
    Hewan* hewanList;
    Kandang* next;
};

struct Zona {
    char nama[50];
    Kandang* kandangList;
    Zona* next;
};

struct Pengunjung {
    string nama;
    string wahana;
    char kursi[100];
    Pengunjung* next;
};

struct Petugas {
    int id;
    char nama[50];
    Petugas* next;
};

struct HashTableEntry {
    int key;
    Petugas* value;
    HashTableEntry* next;
};

class Zoo {
private:
    Hasil hasil;
    string menuResult;
    Zona* zonas;
    HashTableEntry** petugasTable;
    Pengunjung* bookingWahana = nullptr;
    Pengunjung* bookingAquatic = nullptr;
    double penghasilanHariIni;
    int tableSize;

    int hashFunction(int key) {
        return key % tableSize;
    }

public:
    void setMenuResult(const string& result){
        menuResult = result;
    }

    string getMenuResult(){
        return menuResult;
    }

    Zoo(int size) {
        tableSize = size;
        zonas = NULL;
        petugasTable = new HashTableEntry*[tableSize];
        for (int i = 0; i < tableSize; i++) {
            petugasTable[i] = NULL;
        }
        bookingWahana = NULL;
        bookingAquatic = NULL;
        penghasilanHariIni = 0.0;
        // Inisialisasi zona
        addZona("neartik");
        addZona("neotropik");
        addZona("australis");
        addZona("oriental");
        addZona("ethiopia");
    }

    ~Zoo() {
        // Free memory
        delete[] petugasTable;
    }

    void addZona(const char* nama) {
        Zona* newZona = new Zona;
        strcpy(newZona->nama, nama);
        newZona->kandangList = NULL;
        newZona->next = zonas;
        zonas = newZona;
    }

    void tambahPetugas(int id, const char* nama) {
        int hashIndex = hashFunction(id);
        HashTableEntry* newEntry = new HashTableEntry;
        newEntry->key = id;
        newEntry->value = new Petugas;
        newEntry->value->id = id;
        strcpy(newEntry->value->nama, nama);
        newEntry->next = petugasTable[hashIndex];
        petugasTable[hashIndex] = newEntry;
    }

    bool validasiPetugas(int id) {
        int hashIndex = hashFunction(id);
        HashTableEntry* entry = petugasTable[hashIndex];
        while (entry != NULL) {
            if (entry->key == id) {
                return true;
            }
            entry = entry->next;
        }
        return false;
    }

    void tambahHewan(const char* zonaNama, Hewan hewan) {
        Zona* zona = zonas;
        while (zona != NULL) {
            if (strcmp(zona->nama, zonaNama) == 0) {
                Kandang* kandang = zona->kandangList;
                while (kandang != NULL) {
                    if (strcmp(kandang->spesies, hewan.spesies) == 0) {
                        Hewan* newHewan = new Hewan(hewan);
                        newHewan->next = kandang->hewanList;
                        kandang->hewanList = newHewan;
                        cout << hewan.nama << " akan ditempatkan pada zona " << zona->nama << " kandang " << kandang->id << endl;
                        return;
                    }
                    kandang = kandang->next;
                }
                Kandang* newKandang = new Kandang;
                newKandang->id = (zona->kandangList == NULL) ? 1 : zona->kandangList->id + 1;
                strcpy(newKandang->spesies, hewan.spesies);
                newKandang->hewanList = new Hewan(hewan);
                newKandang->next = zona->kandangList;
                zona->kandangList = newKandang;
                cout << hewan.nama << " akan ditempatkan pada zona " << zona->nama << " kandang " << newKandang->id << endl;
                return;
            }
            zona = zona->next;
        }
    }

    void periksaPeta(const char* zonaNama) {
        Zona* zona = zonas;
        while (zona != NULL) {
            if (strcmp(zona->nama, zonaNama) == 0) {
                cout << "Zona: " << zona->nama << endl;
                Kandang* kandang = zona->kandangList;
                while (kandang != NULL) {
                    cout << "  Kandang " << kandang->id << " (Spesies: " << kandang->spesies << "):" << endl;
                    Hewan* hewan = kandang->hewanList;
                    while (hewan != NULL) {
                        cout << "    Nama: " << hewan->nama << ", Spesies: " << hewan->spesies << ", Tanggal Lahir: " << hewan->tanggalLahir << endl;
                        hewan = hewan->next;
                        break;
                    }
                    kandang = kandang->next;
                }
                return;
            }
            zona = zona->next;
        }
        cout << "Zona tidak ditemukan." << endl;
    }

     void addPengunjung(Pengunjung*& head, const Pengunjung& newPengunjung) {
        Pengunjung* newNode = new Pengunjung(newPengunjung);
        newNode->next = head;
        head = newNode;
    }

    void bookingPengunjung(Pengunjung& pengunjung, const char* tipe) {
        if (strcmp(tipe, "wahana") == 0) {
            addPengunjung(bookingWahana, pengunjung);
        } else if (strcmp(tipe, "aquatic") == 0) {
            addPengunjung(bookingAquatic, pengunjung);
        }
    }


    void tampilkanDataPengunjung(const char* tipe, const char* wahana) {
        Pengunjung* pengunjung = (strcmp(tipe, "wahana") == 0) ? bookingWahana : bookingAquatic;

        if (pengunjung == nullptr) {
            cout << "Belum ada pengunjung yang masuk untuk " << tipe << " " << wahana << endl;
            return;
        }

        cout << "Booking " << tipe << ": " << wahana << endl;
        bool found = false;
        while (pengunjung != nullptr) {
            if (pengunjung->wahana == wahana) {
                cout << "  Nama: " << pengunjung->nama << ", Kursi: " << pengunjung->kursi << endl;
                found = true;
            }
            pengunjung = pengunjung->next;
        }

        if (!found) {
            cout << "Belum ada pengunjung yang masuk untuk " << tipe << " " << wahana << endl;
        }
    }

    void tampilkanPenghasilanHariIni() {
        int penghasilanHariIni = hasil.Total1 + hasil.Total2 + hasil.Total3 + hasil.Total4 + hasil.Total5 + hasil.Total6;
        cout << "Penghasilan hari ini: Rp. " << penghasilanHariIni << endl;
    }

    void updateEarnings(int amount, int& total) {
        total += amount;
    }

    Hasil& getHasil() {
        return hasil;
    }
};

void menuPengunjung(Zoo &zoo);
void Menu(Zoo &zoo);
void Wahana(Zoo &zoo);
void Aquatic(Zoo &zoo);
void Complete(Zoo &zoo);
void LokasiSaya(Zoo &zoo);
void ticket1(Zoo &zoo);
void ticket2(Zoo &zoo);
void ticket3(Zoo &zoo);

void menuPetugas(Zoo &zoo, int petugasId) {
    if (!zoo.validasiPetugas(petugasId)) {
        cout << "ID Petugas tidak valid!" << endl;
        return;
    }
    int pilihan;
    do {
        cout << "1. Tambah Hewan" << endl;
        cout << "2. Periksa Peta" << endl;
        cout << "3. Data Pengunjung" << endl;
        cout << "4. Tampilkan Penghasilan Hari Ini" << endl;
        cout << "5. Keluar" << endl;
        cout << "Pilih opsi: ";
        cin >> pilihan;

        if (pilihan == 1) {
            Hewan hewan;
            char zonaNama[50];
            cout << "Masukkan Nama Hewan: ";
            cin >> hewan.nama;
            cout << "Masukkan Spesies Hewan: ";
            cin >> hewan.spesies;
            cout << "Masukkan Tanggal Lahir/Masuk (DD-MM-YYYY): ";
            cin >> hewan.tanggalLahir;
            cin.ignore();
            cout << "Pilih Zona (neartik, neotropik, australis, oriental, ethiopia): ";
            cin >> zonaNama;
            zoo.tambahHewan(zonaNama, hewan);
        } else if (pilihan == 2) {
            char zonaNama[50];
            cout << "Pilih Zona untuk diperiksa (neartik, neotropik, australis, oriental, ethiopia): ";
            cin >> zonaNama;
            zoo.periksaPeta(zonaNama);
        } else if (pilihan == 3) {
            int tipe;
            cout << "1. Wahana" << endl;
            cout << "2. Aquatic" << endl;
            cout << "Pilih tipe: ";
            cin >> tipe;
            char wahana[50];
            cout << "Masukkan nama wahana: ";
            cin.ignore();
            cin.getline(wahana, 50);
            if (tipe == 1) {
                zoo.tampilkanDataPengunjung("wahana", wahana);
            } else if (tipe == 2) {
                zoo.tampilkanDataPengunjung("aquatic", wahana);
            } else {
                cout << "Tipe tidak valid." << endl;
            }
        } else if (pilihan == 4) {
            zoo.tampilkanPenghasilanHariIni();
        }
    } while (pilihan != 5);
}

void menuPengunjung(Zoo &zoo) {
    int pilihan;
    do {
        cout << "1. Menu" << endl;
        cout << "2. Lokasi saya" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilih opsi: ";
        cin >> pilihan;

        switch (pilihan)
        {
            case 1:
            Menu(zoo);
            break;

            case 2:
            LokasiSaya(zoo);
            break;

            case 3:
            break;
            default: 
            cout << "Terima Kasih atas kunjungannya!\n";
            cout << "Sampai Jumpa dilain waktu!\n";
        }
    } while (pilihan != 3);
}

void Menu(Zoo& zoo) {
    Hasil& hasil = zoo.getHasil();
    Pengunjung pengunjung;
    string pilih;
    int x = 20000;
    cout << "Masukkan Nama Anda : ";
    cin.ignore();
    getline(cin, pengunjung.nama);
    cout << " Anda akan melakukan pembayaran ticket masuk\n";
    cout << " Ticket seharga Rp.20.000\n";
    cout << " Apakah ingin melanjutkannya? (y/n)\n";
    cin >> pilih;
    if (pilih == "y" || pilih == "Y") {
        hasil.Total6 = x;
        string milih;
        cout << " Apakah anda ingin menikmati wahana yang tersedia ?\n";
        cin >> milih;
        if (milih == "y" || milih == "Y") {
            int pilihan;
            do {
                cout << "Silahkan memilih layanan yang tersedia\n";
                cout << "1. Wahana\n";
                cout << "2. Aquatic\n";
                cout << "3. Complete ( promo habis ) \n";
                cout << "4. Keluar\n";
                cin >> pilihan;
                switch (pilihan) {
                    case 1:
                        Wahana(zoo);
                        break;
                    case 2:
                        Aquatic(zoo);
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                        default:
                        cout << "Pilihan tidak valid\n";
                }
            } while (pilihan != 4 && pilihan != 3);
        } else {
            cout << "Selamat menikmati wisata kebun binatang ketintang\n";
        }
    } else {
        cout << " Pembayaran anda berupa 20.000 untuk ticket masuk\n";
    }
}

void Wahana(Zoo& zoo) {
    Hasil& hasil = zoo.getHasil();
    Pengunjung pengunjung;
    cout << " Hal yang dapat kamu nikamti dengan ticket wahana :\n";
    cout << " 1. Wahana Rolcoaster\n";
    cout << " 2. Wahana Bebek Kayuh\n";
    cout << " 3. Wahana Mobil Keliling\n";
    cout << " Apakah ingin membeli ticket wahana? (y/n)\n";
    cin >> pengunjung.wahana;
    if (pengunjung.wahana == "y" || pengunjung.wahana == "Y") {
        int x = 20000, y = 35000, d = 5000;
        hasil.Total1 = (x + y) - d;
        cout << " Total yang harus di bayarkan: Rp. " << hasil.Total1 << endl;

        // Prompt for visitor's name and store it
        cout << "Masukkan Nama Anda: ";
        cin.ignore();
        getline(cin, pengunjung.nama);

        cout << "Selamat menikmati wisata kebun binatang ketintang\n";
        zoo.bookingPengunjung(pengunjung, "wahana");
    }
    menuPengunjung(zoo);
}

void Aquatic(Zoo& zoo) {
    Layanan layanan;
    Hasil& hasil = zoo.getHasil();
    int pilih;
    cout << " Wahana air yang tersedia :\n";
    cout << " 1. Pertunjukkan Penguin ( Rp. 45.000)\n";
    cout << " 2. Atraksi Lumba-lumba ( Rp. 45.000)\n";
    cout << " 3. Aquarium ( Rp. 55.000)\n";
    do {
        cout << " Sistem pembayaran wahana Aquatic :\n";
        layanan.Sistem();
        cin >> pilih;
        switch (pilih) {
            case 1:
                ticket1(zoo);
                break;
            case 2:
                ticket2(zoo);
                break;
            case 3:
                ticket3(zoo);
                break;
            case 4:
                break;
            default:
                cout << "Pilihan tidak valid\n";
        }
    } while (pilih != 4);
}

void ticket1(Zoo& zoo) {
    Hasil& hasil = zoo.getHasil();
    Pengunjung pengunjung;
    int i = 20000, x = 45000, d = 5000;
    string pilihanWahana;
    
    cout << "Pilih Wahana Air yang dipilih:\n";
    cout << "1. Pertunjukkan Penguin\n";
    cout << "2. Atraksi Lumba-lumba\n";
    cout << "3. Aquarium\n";
    int pilihan;
    cin >> pilihan;
    cin.ignore(); // Clear newline character left in the input buffer

    switch (pilihan) {
        case 1:
            pengunjung.wahana = "Pertunjukkan Penguin";
            cout << "Pilih kursi (1-100): ";
            cin.getline(pengunjung.kursi, 100);
            hasil.Total2 = (x + i) - d;
            break;
        case 2:
            pengunjung.wahana = "Atraksi Lumba-lumba";
            cout << "Pilih kursi (1-100): ";
            cin.getline(pengunjung.kursi, 100);
            hasil.Total2 = (x + i) - d;
            break;
        case 3:
            pengunjung.wahana = "Aquarium";
            hasil.Total2 = (55000 + i) - d;
            break;
        default:
            cout << "Pilihan tidak valid.\n";
            return;
    }

    zoo.bookingPengunjung(pengunjung, "aquatic");
    cout << "Total yang harus dibayarkan: Rp. " << hasil.Total2 << endl;
    menuPengunjung(zoo);
}

void ticket2(Zoo& zoo) {
    Hasil& hasil = zoo.getHasil();
    Pengunjung pengunjung;
    int i = 20000, x = 45000, y = 45000, z = 55000, d = 5000;
    string wahana1, wahana2;

    cout << "Pilih 2 Wahana Air yang dipilih (format: 1 2):\n";
    cout << "1. Pertunjukkan Penguin\n";
    cout << "2. Atraksi Lumba-lumba\n";
    cout << "3. Aquarium\n";
    int pilihan1, pilihan2;
    cin >> pilihan1 >> pilihan2;
    cin.ignore(); // Clear newline character left in the input buffer

    switch (pilihan1) {
        case 1: wahana1 = "Pertunjukkan Penguin"; break;
        case 2: wahana1 = "Atraksi Lumba-lumba"; break;
        case 3: wahana1 = "Aquarium"; break;
        default:
            cout << "Pilihan tidak valid.\n";
            return;
    }

    switch (pilihan2) {
        case 1: wahana2 = "Pertunjukkan Penguin"; break;
        case 2: wahana2 = "Atraksi Lumba-lumba"; break;
        case 3: wahana2 = "Aquarium"; break;
        default:
            cout << "Pilihan tidak valid.\n";
            return;
    }

    pengunjung.wahana = wahana1 + " & " + wahana2;

    if (wahana1 != "Aquarium" && wahana2 != "Aquarium") {
        cout << "Pilih kursi 1 & 2 (1-100): ";
        cin.getline(pengunjung.kursi, 100);
        hasil.Total3 = (x + y + i) - d;
    } else {
        hasil.Total3 = (x + z + i) - d;
    }

    zoo.bookingPengunjung(pengunjung, "aquatic");
    cout << "Total yang harus dibayarkan: Rp. " << hasil.Total3 << endl;
    menuPengunjung(zoo);
}

void ticket3(Zoo& zoo) {
    Hasil& hasil = zoo.getHasil();
    Pengunjung pengunjung;
    int i = 20000, x = 45000, y = 45000, z = 55000, d = 5000;

    pengunjung.wahana = "Pertunjukkan Penguin, Atraksi Lumba-lumba, & Aquarium";
    cout << "Pilih kursi (1-100) untuk Pertunjukkan Penguin dan Atraksi Lumba-lumba: ";
    cin.ignore();
    cin.getline(pengunjung.kursi, 100);

    hasil.Total4 = (i + x + y + z) - d;

    zoo.bookingPengunjung(pengunjung, "aquatic");
    cout << "Total yang harus dibayarkan: Rp. " << hasil.Total4 << endl;
    menuPengunjung(zoo);
}

void LokasiSaya(Zoo &zoo){
    char zonaNama[50];
         cout << "Masukkan nama zona (neartik, neotropik, australis, oriental, ethiopia): ";
         cin >> zonaNama;
        zoo.periksaPeta(zonaNama);
}

int main() {
    Zoo zoo(10);
    // Tambahkan petugas dengan ID yang valid
    zoo.tambahPetugas(180605, "Petugas A");
    zoo.tambahPetugas(260620, "Petugas B");
    zoo.tambahPetugas(230524, "Petugas C");
    zoo.tambahPetugas(170845, "Petugas D");
    zoo.tambahPetugas(031003, "Petugas E");
    zoo.tambahPetugas(140219, "Petugas F");
    zoo.tambahPetugas(300912, "Petugas G");
    zoo.tambahPetugas(280288, "Petugas H");
    zoo.tambahPetugas(190571, "Petugas I");
    zoo.tambahPetugas(291265, "Petugas J");

    int pilihan;
    do {
        cout << "1. Login sebagai Petugas" << endl;
        cout << "2. Lanjutkan sebagai Pengunjung" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilih opsi: ";
        cin >> pilihan;

        if (pilihan == 1) {
            int id;
            cout << "Masukkan ID Petugas: ";
            cin >> id;
            menuPetugas(zoo, id);
        } else if (pilihan == 2) {
            menuPengunjung(zoo);
        }
    } while (pilihan != 3);

    return 0;
}