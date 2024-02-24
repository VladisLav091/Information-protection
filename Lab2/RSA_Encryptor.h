//
// Created by rolya on 08.02.2024.
//

#ifndef LAB2_RSA_ENCRYPTOR_H
#define LAB2_RSA_ENCRYPTOR_H


#include <string>
#include <vector>
#include <fstream>

class RSA_Encryptor {
public:
    RSA_Encryptor() = delete;
    RSA_Encryptor(int q, int n);
    int encrypt(const std::string &inputFilePath, const std::string &outputFilePath);
    int decrypt(const std::string &inputFilePath, const std::string &outputFilePath);
    void data();
private:
    const std::string CURRENT_PATH = R"(C:\Users\Vladislav888\CLionProjects\protected_information1\Lab2\)";
    int n, e, d;
    int eilerFunc(int p, int q);
    int checkSimplisity(int end);
};


#endif //LAB2_RSA_ENCRYPTOR_H
