#include <iostream>
#include "RSA_Encryptor.h"


int sum_mod (
        unsigned  long  long  int x ,
        unsigned  long  long  int y ,
        unsigned  long  long  int m ) {
    if ( m - x > y )
        return  x + y ;
    return  y - ( m - x ) ; }

// ( x * y ) mod m , 0 < x < m , y < m
// ( 5 * y ) mod m = ( y mod m ) + ( ( ( y * 2 ) mod m ) * 2 ) mod m
int mul_mod (
        unsigned  long  long  int x ,
        unsigned  long  long  int y ,
        unsigned  long  long  int m ) {
    if  ( x > y ) {
        unsigned  long  long  int tmp = x ;
        x = y ;
        y = tmp ; }
    // x <= y
    unsigned  long  long  int res = 0 ;
    unsigned  long  long  int iy  = y ;
    while ( x ) {
        if ( x & 1 )
            res = sum_mod ( res , iy , m ) ;
        iy  = sum_mod  ( iy , iy , m ) ;
        x >>= 1 ; }
    return  res ; }

// ( x ^ n ) mod m , x < m , n > 0 , m > 0
// ( x ^ 5 ) mod m == ( ( x mod m ) * ( ( ( ( ( x ^ 2 ) mod m ) ) ^ 2 ) mod m ) ) mod m
int pow_mod (
        unsigned  long  long  int x ,
        unsigned  long  long  int n ,
        unsigned  long  long  int m ) {
    int res = 1 ;
    while ( n ) {
        if ( n & 1 )
            res = mul_mod ( res , x , m ) ;
        x = mul_mod ( x , x , m ) ;
        n >>= 1 ; }
    return res  ; }
int NOD(int a, int b) {
    if (a > b) return NOD(a - b, b);
    else if (a < b) return NOD(a, b - a);
    else return a;
}
int pow(const int &base, const int &degree, int field) {
    int ret = base;
    for (int i = 1; i < degree; ++i) {
        ret = mul_mod(ret, base, field);
    }
    return ret;
}

RSA_Encryptor::RSA_Encryptor(int p, int q) {
    n = p * q;
    int fi = eilerFunc(p, q);
    e = checkSimplisity(fi);
    int k = 1;
    while ((k * fi + 1) % e != 0) {
        ++k;
    }
    d = (k * fi + 1) / e;
}

void RSA_Encryptor::data() {
    std::cout << "n: " << n << " e: " << e << " d: " << d << std::endl;
}

int RSA_Encryptor::eilerFunc(int p, int q) {
    return (p - 1) * (q - 1);
}

int RSA_Encryptor::checkSimplisity(int end) {
    for (int i = 2; i < end / 2; ++i) {
        if (NOD(i, end) == 1) return i;
    }
}

int RSA_Encryptor::encrypt(const std::string &inputFilePath, const std::string &outputFilePath) {
    std::ifstream in(CURRENT_PATH + inputFilePath, std::ios::binary | std::ios::in);
    std::ofstream out(CURRENT_PATH + outputFilePath, std::ios::binary | std::ios::out);
    std::vector<int> buff;
    char byte;
    if (!in) {
        std::cerr << "Error with opening input file" << std::endl;
        return 1;
    }
    if (!out) {
        std::cerr << "Error with opening output file" << std::endl;
        return 1;
    }
    while (in.get(byte)) {
        buff.push_back(static_cast<int>(byte));
    }
    std::vector<int> encrypted;
    for (int m : buff) {
        int c = 1;
        for (int i = 0; i < e; i++) {
            c = (c * m) % n;
        }
        encrypted.push_back(c);
        std::cout << "Read: " << m << " " << static_cast<char>(m) <<
                  " Calculated: " << c << " " << static_cast<char>(c) << std::endl;
        out << static_cast<char>(c);
    }
    return 0;
}

int RSA_Encryptor::decrypt(const std::string &inputFilePath, const std::string &outputFilePath) {
    std::ifstream in(CURRENT_PATH + inputFilePath, std::ios::binary | std::ios::in);
    std::ofstream out(CURRENT_PATH + outputFilePath, std::ios::binary | std::ios::out);
    std::vector<int> buff;
    char byte;
    if (!in) {
        std::cerr << "Error with opening input file" << std::endl;
        return 1;
    }
    if (!out) {
        std::cerr << "Error with opening output file" << std::endl;
        return 1;
    }
    while (in.get(byte)) {
        buff.push_back(byte);
    }
    std::vector<int> decrypted;
    for (int m : buff) {
        int c = 1;
        for (int i = 0; i < d; ++i) {
            c = (c * m) % n;
        }
        decrypted.push_back(c);
        std::cout << "Read: " << m << " " << static_cast<char>(m) <<
                  " Calculated: " << c << " " << static_cast<char>(c) << std::endl;
        out << static_cast<char>(c);
    }
    return 0;
}