#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include <queue>


//bismillahirrahmanirrahim
//dengan menyebut nama Allah yang maha pengasih lagi maha penyayang

using namespace std;

struct Tanggal {
    int hari, bulan, tahun;
    
    void masukkanTanggal() {
        cout << "Masukkan Tgl Lahir (dd mm yyyy): ";
        cin >> hari >> bulan >> tahun;
    }

    //int ke string = dd-mm-yyyy
    string keString() const { 
        return to_string(hari) + "-" + to_string(bulan) + "-" + to_string(tahun);
    }
    //dari soal,-
    int hitungDigit() const {
        return (hari % 10 + bulan % 10 + tahun % 10) % 9;
    }
};

struct Supir {
    string id;
    string nama;
    Tanggal tglLahir;
    char kelamin; 
    string alamat;

    void tampilkanSupir() const{
        cout << "|-------------------------|\n";
        cout << "|Nama : " << nama << "      |\n";
        cout << "|Id : " << id << "               |\n";
        cout << "|Tgl Lahir: " << tglLahir.keString() << "     |\n";
        cout << "|Kelamin : " << kelamin << "              |\n";
        cout << "|Alamat : " << alamat << "        |\n";
        cout << "|-------------------------|\n";
    }
};

struct DataOrder {
    string idPemesanan;
    string namaPemesan;
    string tujuan;
    string platNomor; 
    string idSupir;

    void tampilkanOrder() const {
    cout << "---Pesanan---" << endl;
    cout << "Id : " << idPemesanan << endl;
    cout << "Nama : " << namaPemesan << endl;
    cout << "Supir : " << idSupir << endl; 
    cout << "Plat nomor : " << platNomor << endl;
    cout << "Tujuan : " << tujuan << endl;
    }
};

struct DataMobil {
    string platMobil;
    string jenisMobil;
    string brandMobil;
};

struct Node {
    DataMobil data; //field
    Node* next; //to te next pointer!!

    Node(DataMobil data) : data(data), next(nullptr){} //inisialisasi sm biar new node = last node
};

class Stack {
    private:
        Node* top; //same; new node = last node, pop the first node,-
    public:
        Stack() : top(nullptr){} // top > null ; waktu buat objek, gada elemen dalam stack

        void push(DataMobil data) { //nambah
            Node* newNode = new Node(data);
            if (top == nullptr) {
                top = newNode;
            } else {
                newNode->next = top;
                top = newNode; //buat ngarahin kalo stacknya ga kosong, biar jd paling atas
            }
            }
        bool pop(DataMobil& data) { //nyalin si data ke DataMObil
            if (top == nullptr) {
                return false;
            } else {
                Node* temp = top;
                data = top->data; //nyalin ke dalam data
                top = top->next; //top diperbarui biar bisa lanjut ke node berikutnya (i think?)
                delete temp;
                return true;
            }
        }
        //memeriksa affakah stack ksong?
        bool isEmpty() const {
            return top == nullptr;
        }
        ~Stack(){
            while (top != nullptr){
                Node* temp = top;
                top = top->next;
                delete temp;
            }
        }
    };

//functions declaration ~_~
void tambahSupir(list<Supir>& daftarSupir);
void editSupir(list<Supir>& daftarSupir);
void hapusSupir(list<Supir>& daftarSupir);
void tampilkanSemuaSupir(const list<Supir>& daftarSupir);
void navigasiSupir(list<Supir>& daftarSupir, queue<DataOrder>& antrianOrder, Stack& gudangMobil);
string generateId(const Supir& supir, const list<Supir>& daftarSupir);
void prosesInputAdmin(list<Supir>& daftarSupir, queue<DataOrder>& antrianOrder, Stack& gudangMobil);
void prosesInputUser(list<Supir>& daftarSupir, queue<DataOrder>& antrianOrder, Stack& gudangMobil);
void buatOrder(queue<DataOrder>& antrianOrder, list<Supir>& daftarSupir, Stack& gudangMobil); //salah mlu dah 凸( •̀_•́ )凸
void prosesAntrianOrder(queue<DataOrder>& antrianOrder);
string generateIdOrder(const DataOrder& order, const queue<DataOrder>& antrianOrder);

struct Interface {
    //static = dikelasin biar rapi
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
        cout << "\t1. Mencari Data Supir \n";
        cout << "\t2. Menghapus Data Supir \n";
        cout << "\t3. Mengubah Data Supir \n";
        cout << "\t4. Menambah Data Supir \n";
        cout << "\t5. Memproses pesanan \n";
        cout << "\t6. Tambah Unit Mobil \n";
        cout << "\t0. Kembali ke Menu Utama \n";
        cout << "\tMasukkan pilihan: ";
    }

    static void user() {
        cout << "\t=== USER MENU ===\n";
        cout << "\t1. Lihat Daftar Supir \n";
        cout << "\t0. Kembali ke Menu Utama \n";
        cout << "\tMasukkan pilihan: ";
    }
};

int main() {
    list<Supir> daftarSupir;
    queue<DataOrder> antrianOrder;
    Stack gudangMobil;

    int pilihan;
    do {
        Interface::menu();
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                prosesInputAdmin(daftarSupir, antrianOrder, gudangMobil);
                break;
            case 2:
                prosesInputUser(daftarSupir, antrianOrder, gudangMobil);
                break;
        }
    } while (pilihan != 0);

    return 0;
}
void tambahUnitMobil(Stack& gudang){
    DataMobil mobilBaru;
    cout << "Masukkan plat nomor mobil : ";
    cin >> mobilBaru.platMobil; //harusnya getline asskdlclhgh
    cout << "Masukkan jenis mobil : ";
    getline(cin, mobilBaru.brandMobil);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Masukkan brandMobil : ";
    getline(cin, mobilBaru.brandMobil);

    gudang.push(mobilBaru);
    cout << "Data mobil berhasil ditambahkan!\n";

    char lagi;
    cout << "ingin menambahkan unit lain? (y/n) > ";
    cin >> lagi;
    if (lagi == 'y' || lagi == 'Y'){
        tambahUnitMobil(gudang);
    }
}

void tambahSupir(list<Supir>& daftarSupir) {
    Supir baru;
    cout << "Nama : > ";
    getline(cin, baru.nama);
    cout << "Tgl Lahir : > ";
    baru.tglLahir.masukkanTanggal();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Kelamin (l/p) : > ";
    cin >> baru.kelamin;
    cin.ignore(1000, '\n'); //bingung(?);ignore biasa gabisa
    baru.kelamin = tolower(baru.kelamin); // tolower itu untuk merubah karakter menjadi huruf kecil
    // kalau diganti tanpa tolower akan error karena akan menjadi string dan string gabisa menyimpan 2 karakter
    cout << "Alamat : > ";
    getline(cin, baru.alamat);

    baru.id = generateId(baru, daftarSupir); //making id 0.0
    daftarSupir.push_back(baru); //masukin id ke list (akhir)

    cout << "Data berhasil ditambahkan dengan ID: " << baru.id << "!\n";

}

void editSupir(list<Supir>& daftarSupir) {
    string id;
    cout << "Masukkan ID Supir yang ingin diedit: ";
    cin >> id;
    cin.ignore(); 

    //nyari ke daftarnya
    auto it = find_if(daftarSupir.begin(), daftarSupir.end(), [&id](const Supir& s) { return s.id == id; });
    // auto it find if = cari data yang rumit
    if (it != daftarSupir.end()) {
        cout << "Data Supir Ditemukan:\n";
        it->tampilkanSupir(); //calling the driver function (atas tdi)
        Supir supirLama = *it; //nyalin 'supir' keeee new variabel, buat next operation 

        int pilihan;
        do {
            cout << "\nBagian yang ingin diedit:\n";
            cout << "1. Nama\n";
            cout << "2. Tanggal Lahir\n";
            cout << "3. Kelamin\n";
            cout << "4. Alamat\n";
            cout << "0. Selesai\n";
            cout << "Masukkan pilihan: ";
            cin >> pilihan;
            cin.ignore(); 

            switch (pilihan) {
                case 1:
                    cout << "Masukkan Nama baru: ";
                    getline(cin, it->nama);
                    break;
                case 2:
                    cout << "Masukkan Tanggal Lahir baru (dd mm yyyy): ";
                    cin >> it->tglLahir.hari >> it->tglLahir.bulan >> it->tglLahir.tahun;
                    cin.ignore(); 
                    break;
               case 3: {
               cout << "Masukkan Kelamin baru (l/p): ";
               char kelaminInput;
               cin >> kelaminInput;
               cin.ignore(); 
    
               kelaminInput = tolower(kelaminInput); 
    
               if (kelaminInput == 'l' || kelaminInput == 'p') {
               it->kelamin = kelaminInput; 
               } else {
               cout << "Input tidak valid. Harap masukkan 'l' untuk laki-laki atau 'p' untuk perempuan.\n";
               }
               break;
               }
                case 4:
                    cout << "Masukkan Alamat baru: ";
                    getline(cin, it->alamat);
                    break;
                case 0:
                    cout << "Selesai mengedit.\n";
                    break;
                default:
                    cout << "Pilihan tidak valid.\n";
            }
        } while (pilihan != 0);

        if (it->nama != supirLama.nama || it->kelamin != supirLama.kelamin || it->tglLahir.hari != supirLama.tglLahir.hari || it->tglLahir.bulan != supirLama.tglLahir.bulan || it->tglLahir.tahun != supirLama.tglLahir.tahun) {
            string idBaru = generateId(*it, daftarSupir);
            cout << "ID lama: " << it->id << " menjadi ID baru: " << idBaru << "\n";
            it->id = idBaru;
        } else {
            cout << "Tidak ada perubahan data yang memerlukan pembuatan ID baru.\n";
        }
    } else {
        cout << "Data Supir dengan ID tersebut tidak ditemukan.\n";
    }
}

//supir kocak 凸( •̀_•́ )凸
void hapusSupir(list<Supir>& daftarSupir) {
     if (daftarSupir.empty()) {
        cout << "Tidak ada data supir untuk dihapus.\n";
        return;
    }

    string id;
    cout << "Masukkan ID supir yang ingin dihapus: ";
    cin >> id;
    cin.ignore();

    auto it = find_if(daftarSupir.begin(), daftarSupir.end(), [&id](const Supir& supir) {
        return supir.id == id; 
    });

    if (it != daftarSupir.end()) {
        char konfirmasi;
        cout << "Anda yakin akan menghapus ID berikut: " << it->id << "? (Y/N): ";
        cin >> konfirmasi;
        if (konfirmasi == 'Y' || konfirmasi == 'y') {
            daftarSupir.erase(it);
            cout << "Data supir dengan ID: " << id << " telah dihapus.\n";
        } else {
            cout << "Penghapusan data dibatalkan.\n";
            return; 
        }
    } else {
        cout << "Supir dengan ID: " << id << " tidak ditemukan.\n";
    }
}

void cariSupirById(const list<Supir>& daftarSupir) {
    if (daftarSupir.empty()) {
        cout << "Daftar supir kosong.\n";
        return;
    }

    string id;
    cout << "Masukkan ID supir yang dicari: ";
    cin >> id;

    auto it = find_if(daftarSupir.begin(), daftarSupir.end(), [id](const Supir& supir) {
        return supir.id == id;
    });

    if (it != daftarSupir.end()) {
        cout << "Supir ditemukan:\n";
        it->tampilkanSupir();
    } else {
        cout << "Tidak ada supir dengan ID tersebut.\n";
    }
}

void tampilkanSemuaSupir(const list<Supir>& daftarSupir) {
    if (daftarSupir.empty()) {
        cout << "Belum ada supir terdaftar." << endl;
        return;
    }
    for (const auto& supir : daftarSupir) {
        supir.tampilkanSupir();
    }
}
//id kesatu
string generateId(const Supir& baru, const list<Supir>& daftarSupir) {
    //digit awal nama
    int firstLetterPos = toupper(baru.nama.front()) - 'A' + 1;
    //terakhir; toupper = konversi ke huruf besar
    int lastLetterPos = toupper(baru.nama.back()) - 'A' + 1;
    int letterDifference = abs(firstLetterPos - lastLetterPos); //biar ga minus
    string firstTwoDigits = (letterDifference < 10) ? "1" + to_string(letterDifference) : to_string(letterDifference);

    string thirdDigit = (baru.kelamin == 'l') ? "1" : "0";

    string fourthDigit = to_string(baru.tglLahir.hitungDigit());

    string baseId = firstTwoDigits + thirdDigit + fourthDigit;

    int duplicateCount = 0;
    for (const auto& s : daftarSupir) {
        if (s.id.find(baseId) == 0) { 
            duplicateCount++;
        }
    }

    string finalId = baseId + (duplicateCount > 0 ? to_string(duplicateCount % 10) : "0");

    if (duplicateCount >= 10) {
        int extraDigits = (duplicateCount / 10) + 1; 
        finalId += to_string(extraDigits);
    }

    return finalId;
}


void prosesInputAdmin(list<Supir>& daftarSupir, queue<DataOrder>& antrianOrder, Stack& gudangMobil) {
    int pilihan;
    do {
        Interface::admin();
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                cariSupirById(daftarSupir);
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
            case 5:
                prosesAntrianOrder(antrianOrder);
                break;
            case 6:
                tambahUnitMobil(gudangMobil);
                break;
        }
    } while (pilihan != 0);
}

void prosesInputUser(list<Supir>& daftarSupir, queue<DataOrder>& antrianOrder, Stack& gudangMobil) {
    int pilihan;
    do {
        Interface::user(); // Tampilkan menu user
        cin >> pilihan;
        cin.ignore(); 

        switch (pilihan) {
            case 1:
               navigasiSupir(daftarSupir, antrianOrder, gudangMobil); 
                break;
            default:
                break;
        }
    } while (pilihan != 0);
}

void navigasiSupir(list<Supir>& daftarSupir, queue<DataOrder>& daftarOrder, Stack& gudangMobil) {
    if (daftarSupir.empty()) {
        cout << "Belum ada data supir.\n";
        return;
    }

    auto it = daftarSupir.begin();
    int pilihan;

    do {
        cout << "Data Supir saat ini:\n";
        it->tampilkanSupir(); //salah

        cout << "1. Supir Selanjutnya\n";
        cout << "2. Supir Sebelumnya\n";
        cout << "3. Buat Order\n";
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
        } else if (pilihan == 3) {
            buatOrder(daftarOrder, daftarSupir, gudangMobil); //gatau diisi apa; error mlu
        }
    }while (pilihan != 0);
}

string generateIdOrder(const DataOrder& order) {
    string id = "";

    //ngambil 2 digit pertama berdasarkan urutan alphabet dari plat 
    if (!order.platNomor.empty()) {
        char firstLetter = toupper(order.platNomor[0]);
        if (firstLetter >= 'A' && firstLetter <= 'Z') {
            int alphabetPosition = firstLetter - 'A' + 1;
            id += (alphabetPosition < 10 ? "0" : "") + to_string(alphabetPosition);
        }
    }

    string supirId = order.idSupir.substr(0, 5);
    while(supirId.length() < 5) supirId += "0";
    id += supirId;

    // Mengambil 2 digit dari 2 huruf trakhir tujuan
    if (order.tujuan.length() >= 2) {
        string tujuan = order.tujuan; // Salin 
        if (!tujuan.empty()) {
            char last = tolower(tujuan.back()) - 'a' + 1; 
            tujuan.pop_back(); // ngapus; kek intinya ngambil last char dari string
            if (!tujuan.empty()) { //gitu intinya
                char early = tolower(tujuan.back()) - 'a' + 1; 
                int digit3 = last + early; 
                id += (to_string(digit3).length() == 1 ? "0" + to_string(digit3) : to_string(digit3));
            }
        }
    }

    // dari nama pelanggan
    int sumOfChars = 0;
    for (char c : order.namaPemesan) {
        if (isalpha(c)) { 
            sumOfChars += tolower(c) - 'a' + 1; 
        }
    }
    id += to_string(sumOfChars % 10);

    // Mengisi digit kosong jika panjang id kurang dari 10
    while (id.length() < 10) id += "0";

    return id;
}

void buatOrder(queue<DataOrder>& antrianOrder, list<Supir>& daftarSupir, Stack& gudangMobil) {
    if (daftarSupir.empty()) {
        cout << "Tidak ada supir tersedia saat ini.\n";
        return;
    }
    if (gudangMobil.isEmpty()) {
        cout << "Unit mobil sedang kosong, harap menunggu....\n";
        return;
    }

    DataOrder orderBaru;
    Supir supirorder;

    // Mengambil data mobil dari paling atas  (mobil paling luar garasi? )
    DataMobil mobilTeratas;
    if (!gudangMobil.pop(mobilTeratas)) {
        cout << "Gagal mengambil mobil dari gudang.\n";
        return;
    }

    orderBaru.platNomor = mobilTeratas.platMobil;

    cout << "Masukkan nama supir: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, supirorder.nama);
    
    cout << "Masukkan Nama Pemesan: ";
    getline(cin, orderBaru.namaPemesan);

    cout << "Masukkan Tujuan: ";
    getline(cin, orderBaru.tujuan);

    auto it = find_if(daftarSupir.begin(), daftarSupir.end(), [&](const Supir& supir) {
        return supir.nama == supirorder.nama;
    });

    if (it != daftarSupir.end()) {
        orderBaru.idSupir = it->id;
        orderBaru.idPemesanan = generateIdOrder(orderBaru); 
        antrianOrder.push(orderBaru);
        cout << "Order berhasil dibuat dan ditambahkan ke antrian dengan ID: " << orderBaru.idPemesanan << endl;
    } else {
        cout << "Supir dengan nama '" << supirorder.nama << "' tidak ditemukan." << endl;
        gudangMobil.push(mobilTeratas); 
    }
}

void prosesAntrianOrder(queue<DataOrder>& antrianOrder) {
    if (antrianOrder.empty()) {
        cout << "Tidak ada order dalam antrian.\n";
        return;
    }

    char choice;

    while (!antrianOrder.empty()) {
        DataOrder order = antrianOrder.front();
        antrianOrder.pop();
        order.tampilkanOrder();

        cout << "Apakah Anda ingin menyetujui pesanan ini? (y/n): ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            cout << "Pesanan telah disetujui\n";
        } else if (choice == 'n' || choice == 'N') {
            cout << "Pesanan tidak disetujui\n";
            cout << "Pesan driver yang lain\n";
            continue; 
        } else {
            cout << "Pilihan tidak valid.\n";
            continue; 
        }
    }
}