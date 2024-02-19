//
// Created by rafae on 1/21/2024.
//

#ifndef BINARY_DATA_PROCESSING_BINARY_PROCESSOR_H
#define BINARY_DATA_PROCESSING_BINARY_PROCESSOR_H

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cstring>
#include <vector>
#include <algorithm>
#include <math.h>

class Binary_Processor{
    std::fstream& _fstream;
public:
    explicit Binary_Processor(std::fstream& fstream): _fstream(fstream) {
        //if(!fstream.is_open())throw std::runtime_error("File could not be opened successfully");
    }
    template<typename T>
    void serialize(T obj);

    template<typename T>
    T deserialize();

    template<typename T>
    void deserialize(T& obj);
};

enum type{
    INT,
    UNSIGNED_INT,
    SHORT,
    UNSIGNED_SHORT,
    LONG,
    UNSIGNED_LONG,
    LONG_LONG,
    UNSIGNED_LONG_LONG,
    FLOAT,
    DOUBLE,
    LONG_DOUBLE,
    STRING,
    CHAR,
    UNSIGNED_CHAR,
    BOOL,
    INT_ARRAY
};

template<typename T>
T Binary_Processor::deserialize() {
    T obj;
    type Type;
    size_t size;
    _fstream.read(reinterpret_cast<char *>(&Type), 1);
    switch (Type) {
        case 1:
            _fstream.read(reinterpret_cast<char *>(&size), 1);
            _fstream.read(reinterpret_cast<char *>(&obj), size);
            break;
        default:
            throw std::runtime_error("aolo");
    }
    return obj;
}

template <typename T>
size_t calculate_bits(T obj){
    for(size_t i = 1; i <= sizeof(T); ++i){
        if (pow(2,i * 8) > abs(obj)) return i;
    }
    return 0;
}

template <typename T>
size_t calculate_bits_unsigned(T obj){
    for(size_t i = 1; i <= sizeof(T); ++i){
        if (pow(2,i * 8) > obj) return i;
    }
    return 0;
}

template<typename T>
void Binary_Processor::serialize(T obj) {
    throw std::runtime_error("Type not supported");
}

template<>
void Binary_Processor::serialize<int>(int obj) {
    size_t size = calculate_bits(obj);
    type t = type::INT;
    _fstream.write(reinterpret_cast<char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&size), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<unsigned int>(unsigned int obj) {
    size_t size = calculate_bits_unsigned(obj);
    type t = type::UNSIGNED_INT;
    _fstream.write(reinterpret_cast<char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&size), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<short>(short obj) {
    size_t size = calculate_bits(obj);
    type t = type::SHORT;
    _fstream.write(reinterpret_cast<char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&size), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<unsigned short>(unsigned short obj) {
    size_t size = calculate_bits_unsigned(obj);
    type t = type::UNSIGNED_SHORT;
    _fstream.write(reinterpret_cast<char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&size), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<long>(long obj) {
    size_t size = calculate_bits(obj);
    type t = type::LONG;
    _fstream.write(reinterpret_cast<char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&size), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<unsigned long>(unsigned long obj) {
    size_t size = calculate_bits_unsigned(obj);
    type t = type::UNSIGNED_LONG;
    _fstream.write(reinterpret_cast<char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&size), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<long long>(long long obj) {
    size_t size = calculate_bits(obj);
    type t = type::LONG_LONG;
    _fstream.write(reinterpret_cast<char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&size), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<unsigned long long>(unsigned long long obj) {
    size_t size = calculate_bits_unsigned(obj);
    type t = type::UNSIGNED_LONG_LONG;
    _fstream.write(reinterpret_cast<char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&size), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<float>(float obj) {
    size_t size = calculate_bits(obj);
    type t = type::FLOAT;
    _fstream.write(reinterpret_cast<char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<double>(double obj) {
    int size = sizeof (double);
    type t = type::DOUBLE;
    _fstream.write(reinterpret_cast<const char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<long double>(long double obj) {
    int size = sizeof (long double);
    type t = type::LONG_DOUBLE;
    _fstream.write(reinterpret_cast<const char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<std::string>(std::string obj) {
    size_t size = obj.size();
    type t = type::STRING;
    _fstream.write(reinterpret_cast<const char *>(&t), 1);
    _fstream.write(reinterpret_cast<const char *>(&size), sizeof(size));
    char temp[size];
    strcpy(temp, obj.c_str());
    _fstream.write(reinterpret_cast<char *>(temp), size);
}

template<>
void Binary_Processor::serialize<char>(char obj) {
    int size = sizeof (char);
    type t = type::CHAR;
    _fstream.write(reinterpret_cast<const char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<unsigned char>(unsigned char obj) {
    int size = sizeof (unsigned char);
    type t = type::UNSIGNED_CHAR;
    _fstream.write(reinterpret_cast<const char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}

template<>
void Binary_Processor::serialize<bool>(bool obj) {
    int size = sizeof (bool);
    type t = type::BOOL;
    _fstream.write(reinterpret_cast<const char *>(&t), 1);
    _fstream.write(reinterpret_cast<char *>(&obj), size);
}



#endif //BINARY_DATA_PROCESSING_BINARY_PROCESSOR_H
