cmake_minimum_required(VERSION 3.27)
project(protected_information1)

set(CMAKE_CXX_STANDARD 17)

add_executable(protected_information1 main.cpp
        RSA_Encryptor.cpp)
