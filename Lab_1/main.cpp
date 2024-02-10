#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <random>
#include <unordered_map>
#include <sstream>


void generateKeyList(const std::string& filename) {
    std::ofstream keyFile(filename);

    if (!keyFile.is_open()) {
        std::cerr << "Error: unable to open key file" << std::endl;
        return;
    }

    std::vector<int> keys(256);
    for (int i = 0; i < 256; ++i) {
        keys[i] = i;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(keys.begin(), keys.end(), g);

    for (int i = 0; i < 256; ++i) {
        keyFile << keys[i];
        if (i != 255) {
            keyFile << " ";
        }
    }

    keyFile.close();
}

std::vector<unsigned char> readKeyFromFile(const std::string& filename) {
    std::ifstream keyFile(filename);
    std::vector<unsigned char> keys;

    if (!keyFile.is_open()) {
        std::cerr << "Error: unable to open key file" << std::endl;
        return keys;
    }

    std::string line;
    std::getline(keyFile, line);

    std::istringstream iss(line);
    int num;
    while (iss >> num) {
        keys.push_back(static_cast<unsigned char>(num));
    }

    keyFile.close();
    return keys;
}

void processFile(const std::string& inputFile, const std::string& outputFile, const std::vector<unsigned char>& key, bool encrypt) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error: unable to open input or output file" << std::endl;
        return;
    }

    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        if (encrypt) {
            // Шифрование: замена каждого байта на соответствующий байт из ключа
            outFile.write(reinterpret_cast<const char*>(&key[byte]), sizeof(unsigned char));
        } else {
            // Расшифрование: замена каждого байта из ключа на исходный байт
            for (size_t i = 0; i < key.size(); ++i) {
                if (key[i] == byte) {
                    outFile.write(reinterpret_cast<const char*>(&i), sizeof(unsigned char));
                    break;
                }
            }
        }
    }

    inFile.close();
    outFile.close();

    std::cout << (encrypt ? "Encryption" : "Decryption") << " completed." << std::endl;
}

void Task_1_2(){

    std::string desktopPath = "C:/Users/Vladislav888/Desktop/File.doc";
    std::ifstream file(desktopPath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "The file could not be opened." << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize == -1) {
        std::cerr << "Error when getting the file size." << std::endl;
        return;
    }

    std::cout << "File size: " << fileSize << " bytes." << std::endl;
    //Task_2
    std::unordered_map<unsigned char, int> byteFrequency;

    char byte;
    int totalBytes = 0;
    while (file.get(byte)) {
        byteFrequency[static_cast<unsigned char>(byte)]++;
        totalBytes++;
    }

    std::cout << "The relative frequency of occurrence of each byte in the file'" << desktopPath << "':" << std::endl;
    for (const auto& pair : byteFrequency) {
        double relativeFrequency = static_cast<double>(pair.second) / totalBytes;
        std::cout << "Bytes: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(pair.first)
                  << "   Frequency: " << std::dec << pair.second
                  << "   Relative Frequency: " << std::fixed << std::setprecision(4) << relativeFrequency << std::endl;
    }
}
void Task3() {
    std::string inputFile, outputFile, keyFile;
    char mode;
    char regenerateKey;

    std::cout << "Enter mode (e for encrypt, d for decrypt): ";
    std::cin >> mode;

    std::cout << "Enter input file name: ";
    std::cin >> inputFile;

    std::cout << "Enter output file name: ";
    std::cin >> outputFile;

    // Проверка на режим дешифровки
    if (mode == 'd') {
        std::cout << "Processing file..." << std::endl;
        std::vector<unsigned char> key = readKeyFromFile("../key.txt");
        processFile(inputFile, outputFile, key, false);
        std::cout << "Decryption completed." << std::endl;
        return;
    }

    // Проверка на перегенерацию ключа
    std::cout << "Do you want to regenerate the key? (y/n): ";
    std::cin >> regenerateKey;

    if (regenerateKey == 'y') {
        std::cout << "Generating key file..." << std::endl;
        generateKeyList("../key.txt");
    }

    std::cout << "Processing file..." << std::endl;
    std::vector<unsigned char> key = readKeyFromFile("../key.txt");

    if (mode == 'e') {
        processFile(inputFile, outputFile, key, true);
        std::cout << "Encryption completed." << std::endl;
    } else {
        std::cerr << "Invalid mode. Please enter 'e' for encryption or 'd' for decryption." << std::endl;
        return;
    }
}
int main() {
    int choice;

    do {
        std::cout << "Choose an option:\t" << std::endl;
        std::cout << "1) Task 1 and 2" << std::endl;
        std::cout << "2) Task 3" << std::endl;
        std::cout << "3) Exit the program" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                Task_1_2();
                break;
            case 2:
                Task3();
                break;
            case 3:
                std::cout << "Exiting the program." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please enter a valid option." << std::endl;
        }

    } while (choice != 3);

    return 0;
}