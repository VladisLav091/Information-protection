#include <iostream>

#include "RSA_Encryptor.h"


int main() {
    auto test = RSA_Encryptor(11, 13);
    test.data();
    test.encrypt("input.txt", "crypted.bin");
    test.decrypt("crypted.bin", "output.txt");
    return 0;
}
