#include <iostream>
#include <string>
#include <ctime>
#include <unordered_map>
#include <vector>

class Keadaan {
public:
    Keadaan() : kekenyangan(0), lapar(true), kotor(false), bosan(false), jalanCounter(0), totalJarak(0) {}

    void waktuBerlalu(int waktu) {
        kekenyangan -= waktu;
        updateStatus();
    }

    void updateStatus() {
        if (kekenyangan <= 0) {
            lapar = true;
            std::cout << "Lapar!" << std::endl;
        }
        if (kotor) {
            std::cout << "Segera mandikan!" << std::endl;
        }
        if (bosan) {
            std::cout << "Segera ajak jalan!" << std::endl;
        }
    }

    bool isLapar() const {
        return lapar;
    }

    bool isKotor() const {
        return kotor;
    }

    bool isBosan() const {
        return bosan;
    }

    void makan(int porsi) {
        kekenyangan += porsi;
        lapar = false;
        std::cout << "Makan " << porsi << " porsi. Kekenyangan: " << kekenyangan << std::endl;
    }

    void mandi() {
        kotor = false;
        std::cout << "Dimandikan. Bersih sekarang." << std::endl;
    }

    void ajakJalan(int jarak) {
        bosan = false;
        totalJarak += jarak;
        std::cout << "Diajak jalan. Senangnya! Jarak yang dijelajahi: " << jarak << " meter." << std::endl;
        jalanCounter++;
        if (jalanCounter >= 2) {
            membuatKotor();
            jalanCounter = 0; // Reset the counter after becoming kotor
        }
    }

    void membuatKotor(); // Declaration added

    void melakukanAktivitasLain() {
        jalanCounter = 0; // Reset jalan counter if any other activity is performed
        if (!bosan) {
            std::cout << "Melakukan aktivitas lain." << std::endl;
            bosan = true;
        }
    }

    int getTotalJarak() const {
        return totalJarak;
    }

private:
    int kekenyangan;
    bool lapar;
    bool kotor;
    bool bosan;
    int jalanCounter;
    int totalJarak;
};

void Keadaan::membuatKotor() {
    kotor = true;
    std::cout << "Membuat kotor. Perlu dimandikan!" << std::endl;
}

class Lokasi {
public:
    Lokasi(const std::string& nama) : nama(nama) {}

    const std::string& getNama() const {
        return nama;
    }

private:
    std::string nama;
};

class Binatang : public Lokasi {
public:
    Binatang(const std::string& nama) : Lokasi(nama), keadaan() {}

    void lakukanAksi();

    bool isLapar() const {
        return keadaan.isLapar();
    }

    void tambahJalan(const Lokasi& tujuan, int jarak);

    void jalan();

    int pilihLokasi() const;

    int getTotalJarak() const {
        return keadaan.getTotalJarak();
    }

private:
    Keadaan keadaan;
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adjacencyList;
};

void Binatang::lakukanAksi() {
    // Waktu berlalu, kurangi kekenyangan
    keadaan.waktuBerlalu(1);

    // Cek apakah binatang lapar
    if (keadaan.isLapar()) {
        std::cout << "Segera beri makan " << getNama() << "!" << std::endl;
    }

    // Cek apakah binatang kotor
    if (keadaan.isKotor()) {
        keadaan.membuatKotor();
    }

    // Cek apakah binatang bosan
    if (keadaan.isBosan()) {
        std::cout << "Segera ajak jalan " << getNama() << "!" << std::endl;
        jalan();
    }

    // Menampilkan menu
    std::cout << "Menu: " << std::endl;
    std::cout << "1. Makan" << std::endl;
    std::cout << "2. Jalan" << std::endl;
    std::cout << "3. Mandi" << std::endl;
    std::cout << "4. Melakukan aktivitas lain" << std::endl;
    std::cout << "0. Keluar" << std::endl;

    // Meminta input dari pengguna
    char opsi;
    std::cout << "Pilih aksi (0-4): ";
    std::cin >> opsi;

    // Melakukan aksi berdasarkan input
    switch (opsi) {
        case '1':
            keadaan.makan(2);
            break;
        case '2':
            keadaan.ajakJalan(pilihLokasi());
            break;
        case '3':
            keadaan.mandi();
            break;
        case '4':
            keadaan.melakukanAktivitasLain();
            break;
        default:
            std::cout << "Input tidak valid. Coba lagi." << std::endl;
    }

    // Menunggu sebentar untuk simulasi waktu berlalu
    std::cout << "Waktu: " << std::time(nullptr) << std::endl;
}

void Binatang::tambahJalan(const Lokasi& tujuan, int jarak) {
    adjacencyList[getNama()].emplace_back(tujuan.getNama(), jarak);
    adjacencyList[tujuan.getNama()].emplace_back(getNama(), jarak);
}

void Binatang::jalan() {
    std::cout << "Pilih tujuan jalan:" << std::endl;
    int i = 1;
    for (const auto& pair : adjacencyList.at(getNama())) {
        std::cout << i << ". " << pair.first << " (Jarak: " << pair.second << " meter)" << std::endl;
        i++;
    }

    int pilihan;
    std::cout << "Masukkan nomor tujuan: ";
    std::cin >> pilihan;

    if (pilihan >= 1 && pilihan <= static_cast<int>(adjacencyList.at(getNama()).size())) {
        const auto& tujuan = adjacencyList.at(getNama())[pilihan - 1];
        keadaan.ajakJalan(tujuan.second);
    } else {
        std::cout << "Pilihan tidak valid." << std::endl;
    }
}

int Binatang::pilihLokasi() const {
    std::cout << "Pilih tujuan jalan:" << std::endl;
    int i = 1;
    for (const auto& pair : adjacencyList.at(getNama())) {
        std::cout << i << ". " << pair.first << " (Jarak: " << pair.second << " meter)" << std::endl;
        i++;
    }

    int pilihan;
    std::cout << "Masukkan nomor tujuan: ";
    std::cin >> pilihan;

    if (pilihan >= 1 && pilihan <= static_cast<int>(adjacencyList.at(getNama()).size())) {
        return adjacencyList.at(getNama())[pilihan - 1].second;
    } else {
        std::cout << "Pilihan tidak valid. Kembali ke tempat semula." << std::endl;
        return 0;
    }
}

int main() {
    // Buat objek binatang
    Binatang kucing("Kucing");

    // Tambahkan beberapa lokasi untuk jalan
    Lokasi taman("Taman");
    Lokasi pantai("Pantai");
    Lokasi hutan("Hutan");

    // Tambahkan jalan dari kucing ke taman dengan jarak 200 meter
    kucing.tambahJalan(taman, 200);

    // Tambahkan jalan dari kucing ke pantai dengan jarak 500 meter
    kucing.tambahJalan(pantai, 500);

    // Tambahkan jalan dari kucing ke hutan dengan jarak 300 meter
    kucing.tambahJalan(hutan, 300);

    // Loop utama
    char opsi;
    do {
        kucing.lakukanAksi();

        // Keluar dari loop jika binatang lapar
    } while (!kucing.isLapar());

    std::cout << "Binatang kelaparan. Permainan selesai." << std::endl;

    // Tampilkan total jarak yang telah dijelajahi
    std::cout << "Total Jarak yang Dijelajahi: " << kucing.getTotalJarak() << " meter." << std::endl;

    return 0;
}
