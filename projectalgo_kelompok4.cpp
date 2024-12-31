#include <iostream>
#include <string>

int MAX_MENU = 10; // Maksimal jumlah menu
int MAX_STOCK = 10; // Maksimal jumlah bahan
int MAX_ORDERS = 100; // Maksimal jumlah pesanan

// Fungsi untuk menampilkan menu
int displayMenu(std::string menu[], float price[], int menuCount) {
    std::cout << "Menu yang tersedia:\n";
    for (int i = 0; i < menuCount; i++) {
        std::cout << i + 1 << ". " << menu[i] << " - Rp " << price[i] << std::endl;
    }
    return 0;
}

// Fungsi untuk menampilkan stok
int displayStock(std::string stock[], int quantity[], int stockCount) {
    std::cout << "Stok bahan saat ini:\n";
    for (int i = 0; i < stockCount; i++) {
        std::cout << stock[i] << ": " << quantity[i] << " unit" << std::endl;
    }
    return 0;
}

// Fungsi untuk menambahkan menu baru
int addMenu(std::string menu[], float price[], int &menuCount) {
    if (menuCount < MAX_MENU) {
        std::string name;
        float priceInput;

        std::cout << "Masukkan nama menu baru: ";
        std::cin.ignore(); // Mengabaikan newline dari input sebelumnya
        std::getline(std::cin, name);
        std::cout << "Masukkan harga menu: ";
        std::cin >> priceInput;

        menu[menuCount] = name;
        price[menuCount] = priceInput;
        menuCount++;
        std::cout << "Menu berhasil ditambahkan!\n";
    } else {
        std::cout << "Menu sudah penuh, nggak bisa tambah menu lagi.\n";
    }
    return 0;
}

// Fungsi untuk menambahkan stok bahan
int addStock(std::string stock[], int quantity[], int &stockCount) {
    std::string ingredient;
    int quantityInput;
    bool found = false;

    std::cout << "Masukkan nama bahan yang mau ditambah: ";
    std::cin.ignore(); // Mengabaikan newline dari input sebelumnya
    std::getline(std::cin, ingredient);
    std::cout << "Masukkan jumlah bahan yang mau ditambah: ";
    std::cin >> quantityInput;

    // Cek apakah bahan sudah ada di stok
    for (int i = 0; i < stockCount; i++) {
        if (stock[i] == ingredient) {
            // Jika bahan ditemukan, tambahkan jumlahnya
            quantity[i] += quantityInput;
            std::cout << "Stok bahan " << ingredient << " berhasil ditambahkan. Sekarang stoknya: " << quantity[i] << " unit.\n";
            found = true;
            break;
        }
    }

    // Jika bahan tidak ditemukan di stok, tambah bahan baru
    if (!found) {
        if (stockCount < MAX_STOCK) {
            stock[stockCount] = ingredient;
            quantity[stockCount] = quantityInput;
            stockCount++;
            std::cout << "Bahan baru berhasil ditambahkan! Stok sekarang: " << quantityInput << " unit.\n";
        } else {
            std::cout << "Stok bahan sudah penuh, nggak bisa tambah bahan lagi.\n";
        }
    }
    return 0;
}

// Fungsi untuk memproses pesanan
int processOrder(std::string menu[], float price[], std::string stock[], int quantity[], std::string orders[], int &orderCount) {
    char moreOrders;

    do {
        displayMenu(menu, price, MAX_MENU);

        int menuChoice, orderQuantity;

        std::cout << "Pilih menu yang mau dipesan (nomor menu): ";
        std::cin >> menuChoice;

        if (menuChoice < 1 || menuChoice > MAX_MENU) {
            std::cout << "Pilihan menu nggak valid, coba lagi ya!\n";
            continue;
        }

        std::cout << "Berapa banyak yang mau dipesan? ";
        std::cin >> orderQuantity;

        // Cek stok bahan yang dibutuhkan menu yang dipilih
        bool isAvailable = true;

        if (menuChoice == 1) { // Nasi Goreng Telur + Ayam
            // Butuh bahan Nasi, Telur, Ayam
            if (orderQuantity > quantity[0] || orderQuantity > quantity[1] || orderQuantity > quantity[2]) {
                isAvailable = false;
            }
        } else if (menuChoice == 2) { // Fanta Susu Es
            // Butuh bahan Fanta, Susu
            if (orderQuantity > quantity[3] || orderQuantity > quantity[4]) {
                isAvailable = false;
            }
        }

        if (!isAvailable) {
            std::cout << "Stok bahan nggak cukup buat pesanan ini.\n";
            continue;
        }

        // Kalau stok cukup, kurangi stoknya
        if (menuChoice == 1) {
            quantity[0] -= orderQuantity; // Nasi
            quantity[1] -= orderQuantity; // Telur
            quantity[2] -= orderQuantity; // Ayam
        } else if (menuChoice == 2) {
            quantity[3] -= orderQuantity; // Fanta
            quantity[4] -= orderQuantity; // Susu
        }

        // Simpan pesanan
        orders[orderCount++] = menu[menuChoice - 1] + " x" + std::to_string(orderQuantity);

        std::cout << "Mau pesan lagi? (y/n): ";
        std::cin >> moreOrders;

    } while (moreOrders == 'y');

    // Hitung total harga pesanan
    float totalCost = 0.0f;

    for (int i = 0; i < orderCount; i++) {
        for (int j = 0; j < MAX_MENU; j++) {
            if (orders[i].find(menu[j]) != std::string::npos) {
                totalCost += orderCount * price[j];
                break;
            }
        }
    }

    // Tampilkan total harga
    std::cout << "Total harga pesanan: Rp " << totalCost << "\n";
    return 0;
}

// Fungsi untuk mencatat jika suatu menu habis
int checkOutOfStock(std::string menu[], int menuCount, std::string stock[], int quantity[], std::string orders[], int orderCount) {
    for (int i = 0; i < menuCount; i++) {
        bool isOutOfStock = false;

        // Cek stok bahan yang dibutuhkan menu yang dipilih
        if (menu[i] == "Nasi Goreng Telur + Ayam") {
            int totalNeeded = 0;
            for (int j = 0; j < orderCount; j++) {
                if (orders[j].find(menu[i]) != std::string::npos) {
                    totalNeeded += std::stoi(orders[j].substr(orders[j].find("x") + 1));
                }
            }
            if (totalNeeded > quantity[0] || totalNeeded > quantity[1] || totalNeeded > quantity[2]) {
                isOutOfStock = true;
            }
        } else if (menu[i] == "Fanta Susu Es") {
            int totalNeeded = 0;
            for (int j = 0; j < orderCount; j++) {
                if (orders[j].find(menu[i]) != std::string::npos) {
                    totalNeeded += std::stoi(orders[j].substr(orders[j].find("x") + 1));
                }
            }
            if (totalNeeded > quantity[3] || totalNeeded > quantity[4]) {
                isOutOfStock = true;
            }
        }

        if (isOutOfStock) {
            std::cout << "Menu \"" << menu[i] << "\" habis ya.\n";
        }
    }
    return 0;
}

int main() {
    std::string menu[MAX_MENU] = {
        "Nasi Goreng Telur + Ayam",
        "Fanta Susu Es",
        "Pisang Bakar Coklat Keju",
        "Indomie Rebus Telur",
        "Es Ovaltine Susu"
    };

    float price[MAX_MENU] = {18000, 11000, 15000, 14000, 11000};
    std::string stock[MAX_STOCK] = {
        "Nasi", "Telur", "Ayam", "Fanta", "Susu", "Es", "Pisang", "Coklat", "Keju", "Indomie"
    };
    int quantity[MAX_STOCK] = {20, 30, 15, 25, 20, 50, 30, 15, 10, 25};

    std::string orders[MAX_ORDERS];
    int orderCount = 0;

    int menuCount = 5; // Jumlah menu yang sudah ada
    int stockCount = 10; // Jumlah bahan yang sudah ada

    int choice;

    do {
        std::cout << "\nManajemen Staff:\n";
        std::cout << "1. Lihat Menu\n";
        std::cout << "2. Tambah Menu\n";
        std::cout << "3. Lihat Stok Bahan\n";
        std::cout << "4. Tambah Stok\n";
        std::cout << "5. Proses Pesanan\n";
        std::cout << "6. Cek Menu Habis\n";
        std::cout << "0. Keluar\n";

        std::cout << "\nPilih opsi: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                displayMenu(menu, price, menuCount);
                break;
            case 2:
                addMenu(menu, price, menuCount);
                break;
            case 3:
                displayStock(stock, quantity, stockCount);
                break;
            case 4:
                addStock(stock, quantity, stockCount);
                break;
            case 5:
                processOrder(menu, price, stock, quantity, orders, orderCount);
                break;
            case 6:
                checkOutOfStock(menu, menuCount, stock, quantity, orders, orderCount);
                break;
            case 0:
                std::cout << "\nKeluar dari program.\n";
                break;
            default:
                std::cout << "\nOpsi nggak valid. Coba lagi deh.\n";
        }
    } while (choice != 0);

    return 0;
}



HASIL OUTPUT 
Manajemen Staff:
1. Lihat Menu
2. Tambah Menu
3. Lihat Stok Bahan
4. Tambah Stok
5. Proses Pesanan
6. Cek Menu Habis
0. Keluar

Pilih opsi: 2
Masukkan nama menu baru: ayam suwir daun jeruk
Masukkan harga menu: 17000
Menu berhasil ditambahkan!

Manajemen Staff:
1. Lihat Menu
2. Tambah Menu
3. Lihat Stok Bahan
4. Tambah Stok
5. Proses Pesanan
6. Cek Menu Habis
0. Keluar

Pilih opsi: 1
Menu yang tersedia:
1. Nasi Goreng Telur + Ayam - Rp 18000
2. Fanta Susu Es - Rp 11000
3. Pisang Bakar Coklat Keju - Rp 15000
4. Indomie Rebus Telur - Rp 14000
5. Es Ovaltine Susu - Rp 11000
6. ayam suwir daun jeruk - Rp 17000

Manajemen Staff:
1. Lihat Menu
2. Tambah Menu
3. Lihat Stok Bahan
4. Tambah Stok
5. Proses Pesanan
6. Cek Menu Habis
0. Keluar

Pilih opsi: 3
Stok bahan saat ini:
Nasi: 20 unit
Telur: 30 unit
Ayam: 15 unit
Fanta: 25 unit
Susu: 20 unit
Es: 50 unit
Pisang: 30 unit
Coklat: 15 unit
Keju: 10 unit
Indomie: 25 unit

Manajemen Staff:
1. Lihat Menu
2. Tambah Menu
3. Lihat Stok Bahan
4. Tambah Stok
5. Proses Pesanan
6. Cek Menu Habis
0. Keluar

Pilih opsi: 0

Keluar dari program.


=== Code Execution Successful ===
