//
// Created by rafae on 1/21/2024.
//

#ifndef BINARY_DATA_PROCESSING_BINARYCONVERTER_HPP
#define BINARY_DATA_PROCESSING_BINARYCONVERTER_HPP

#include <iostream>
#include <typeinfo>
#include "TypeDefinitions.hpp"

namespace CES {
    class BinaryConverter {
        template<typename T>
        static void serialize_element(const T &elem, std::ostream &ostream);

        template<typename T>
        static type find_type(T obj);

        static system_type detect_system_type();

        template<typename T>
        static T switch_bytes(T &obj);

    public:

        BinaryConverter() = delete;

        template<typename T>
        static void serialize(T &obj, std::ostream &ostream);

        template<typename T, std::size_t N>
        static void serialize(T (&arr)[N], std::ostream &ostream);

        template<typename T>
        static void deserialize(T &obj, std::istream &istream);

        template<typename T, std::size_t N>
        static void deserialize(T (&arr)[N], std::istream &istream);
    };

    system_type BinaryConverter::detect_system_type() {
        int n = 1;
        if (*(char *) &n == 1) return LE;
        else return BE;
    }

    template<typename T>
    T BinaryConverter::switch_bytes(T &obj) {
        int total_bits = sizeof(T) * 8;
        int final = 0;
        for (int i = 0; i < total_bits; i += 8) {
            int byte = (obj >> i) & 0xFF;
            final |= (byte << (total_bits - 8 - i));
        }
        obj = final;
        return obj;
    }

    template<typename T>
    void BinaryConverter::deserialize(T &obj, std::istream &istream) {
        char t_char, sts_char;
        istream.get(sts_char);
        istream.get(t_char);

        type t = static_cast<type>(t_char);
        system_type sts = static_cast<system_type>(sts_char), st = detect_system_type();
        switch (t) {
            case INT:
                if (typeid(int) != typeid(T))
                    throw std::invalid_argument("Specified data type does not match serialized data type");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(int));
                break;
            case UNSIGNED_INT:
                if (typeid(unsigned int) != typeid(T))
                    throw std::invalid_argument("Specified data type does not match serialized data type");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(unsigned int));
                break;
            case SHORT:
                if (typeid(short) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(short));
                break;
            case UNSIGNED_SHORT:
                if (typeid(unsigned short) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (UNSIGNED SHORT)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(unsigned short));
                break;
            case LONG:
                if (typeid(long) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (LONG)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(long));
                break;
            case UNSIGNED_LONG:
                if (typeid(unsigned long) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (UNSIGNED LONG)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(unsigned long));
                break;
            case LONG_LONG:
                if (typeid(long long) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (LONG LONG)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(long long));
                break;
            case UNSIGNED_LONG_LONG:
                if (typeid(unsigned long long) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (UNSIGNED LONG LONG)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(unsigned long long));
                break;
            case FLOAT:
                if (typeid(float) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (FLOAT)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(float));
                break;
            case DOUBLE:
                if (typeid(double) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (DOUBLE)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(double));
                break;
            case LONG_DOUBLE:
                if (typeid(long double) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (LONG DOUBLE)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(long double));
                break;
            case STRING:
                if constexpr (std::is_same<T, std::string>::value) {
                    size_t size;
                    istream.read(reinterpret_cast<char *>(&size), sizeof(size_t));

                    char test[size];
                    istream.read(reinterpret_cast<char *>(&test), static_cast<std::streamsize>(size));
                    test[size] = '\0';
                    obj = test;
                } else throw std::invalid_argument("Object type does not match serialized data type (STRING)");
                break;
            case CHAR:
                if (typeid(char) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (CHAR)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(char));
                break;
            case UNSIGNED_CHAR:
                if (typeid(unsigned char) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (UNSIGNED CHAR)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(unsigned char));
                break;
            case BOOL:
                if (typeid(bool) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (bool)");
                istream.read(reinterpret_cast<char *>(&obj), sizeof(bool));
                break;
            default:
                throw std::invalid_argument("Data type not accepted");
                break;
        }
        if (st != sts) {
            if constexpr (!std::is_same<T, std::string>::value && !std::is_same<T, char>::value &&
                          !std::is_same<T, unsigned char>::value && !std::is_same<T, bool>::value) {
                switch_bytes(obj);
            }
        }
    }

    template<typename T, std::size_t N>
    void BinaryConverter::deserialize(T (&arr)[N], std::istream &istream) {
        size_t size;
        char t_char, sts_char;
        istream.get(sts_char);
        istream.get(t_char);

        type t = static_cast<type>(t_char);
        system_type sts = static_cast<system_type>(sts_char),
                st = detect_system_type();

        istream.read(reinterpret_cast<char *>(&size), sizeof(size_t));

        switch (t) {
            case INT_ARRAY:
                if (typeid(int) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (INT ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(int));
                }
                break;
            case UNSIGNED_INT_ARRAY:
                if (typeid(unsigned int) != typeid(T))
                    throw std::invalid_argument(
                            "Object type does not match serialized data type (UNSIGNED INT ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(unsigned int));
                }
                break;
            case SHORT_ARRAY:
                if (typeid(short) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (SHORT ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(short));
                }
                break;
            case UNSIGNED_SHORT_ARRAY:
                if (typeid(unsigned short) != typeid(T))
                    throw std::invalid_argument(
                            "Object type does not match serialized data type (UNSIGNED SHORT ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(unsigned short));
                }
                break;
            case LONG_ARRAY:
                if (typeid(long) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (LONG ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(long));
                }
                break;
            case UNSIGNED_LONG_ARRAY:
                if (typeid(unsigned long) != typeid(T))
                    throw std::invalid_argument(
                            "Object type does not match serialized data type (UNSIGNED LONG ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(unsigned long));
                }
                break;
            case UNSIGNED_LONG_LONG_ARRAY:
                if (typeid(unsigned long long) != typeid(T))
                    throw std::invalid_argument(
                            "Object type does not match serialized data type (UNSIGNED LONG LONG ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(unsigned long long));
                }
                break;
            case LONG_LONG_ARRAY:
                if (typeid(long long) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (LONG LONG ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(long long));
                }
                break;
            case FLOAT_ARRAY:
                if (typeid(float) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (FLOAT ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(float));
                }
                break;
            case DOUBLE_ARRAY:
                if (typeid(double) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (DOUBLE ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(double));
                }
                break;
            case LONG_DOUBLE_ARRAY:
                if (typeid(long double) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (LONG DOUBLE ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(int));
                }
                break;
            case STRING_ARRAY:
                if constexpr (std::is_same<T, std::string>::value) {
                    size_t string_size;
                    for (int i = 0; i < size; ++i) {
                        istream.read(reinterpret_cast<char *>(&string_size), sizeof(size_t));

                        char temp[string_size + 1];
                        istream.read(reinterpret_cast<char *>(&temp), static_cast<std::streamsize>(string_size));
                        temp[string_size] = '\0';

                        arr[i] = std::string(temp);
                    }
                } else
                    throw std::invalid_argument("Object type does not match serialized data type (ARRAY OF STRINGS)");
                break;
            case CHAR_ARRAY:
                if (typeid(char) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (CHAR ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(char));
                }
                break;
            case UNSIGNED_CHAR_ARRAY:
                if (typeid(unsigned char) != typeid(T))
                    throw std::invalid_argument(
                            "Object type does not match serialized data type (UNSIGNED CHAR ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(unsigned char));
                }
                break;
            case BOOL_ARRAY:
                if (typeid(bool) != typeid(T))
                    throw std::invalid_argument("Object type does not match serialized data type (BOOL ARRAY)");
                for (int i = 0; i < size; ++i) {
                    istream.read(reinterpret_cast<char *>(&arr[i]), sizeof(int));
                }
                break;
            default:
                throw std::invalid_argument("Data type not accepted");
        }
        if (st != sts) {
            if constexpr (!std::is_same<T, std::string>::value && !std::is_same<T, char>::value &&
                          !std::is_same<T, unsigned char>::value && !std::is_same<T, bool>::value) {
                for (int i = 0; i < size; ++i) {
                    switch_bytes(arr[i]);
                }
            }
        }
    }

    template<typename T>
    void BinaryConverter::serialize(T &, std::ostream &) {
        throw std::runtime_error("Type not supported");
    }

    template<>
    void BinaryConverter::serialize<int>(int &obj, std::ostream &ostream) {
        type t = type::INT;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(int));
    }

    template<>
    void BinaryConverter::serialize<unsigned int>(unsigned int &obj, std::ostream &ostream) {
        type t = type::UNSIGNED_INT;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(unsigned int));
    }

    template<>
    void BinaryConverter::serialize<short>(short &obj, std::ostream &ostream) {
        type t = type::SHORT;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(short));
    }

    template<>
    void BinaryConverter::serialize<unsigned short>(unsigned short &obj, std::ostream &ostream) {
        type t = type::UNSIGNED_SHORT;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(unsigned short));
    }

    template<>
    void BinaryConverter::serialize<long>(long &obj, std::ostream &ostream) {
        type t = type::LONG;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(long));
    }

    template<>
    void BinaryConverter::serialize<unsigned long>(unsigned long &obj, std::ostream &ostream) {
        type t = type::UNSIGNED_LONG;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(unsigned long));
    }

    template<>
    void BinaryConverter::serialize<long long>(long long &obj, std::ostream &ostream) {
        type t = type::LONG_LONG;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(long long));
    }

    template<>
    void BinaryConverter::serialize<unsigned long long>(unsigned long long &obj, std::ostream &ostream) {
        type t = type::UNSIGNED_LONG_LONG;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(unsigned long long));
    }

    template<>
    void BinaryConverter::serialize<float>(float &obj, std::ostream &ostream) {
        type t = type::FLOAT;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(float));
    }

    template<>
    void BinaryConverter::serialize<double>(double &obj, std::ostream &ostream) {
        type t = type::DOUBLE;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<const char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(double));
    }

    template<>
    void BinaryConverter::serialize<long double>(long double &obj, std::ostream &ostream) {
        type t = type::LONG_DOUBLE;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<const char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(long double));
    }

    template<>
    void BinaryConverter::serialize<std::string>(std::string &obj, std::ostream &ostream) {
        size_t size = obj.size();
        type t = type::STRING;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<const char *>(&t), 1);
        ostream.write(reinterpret_cast<const char *>(&size), sizeof(size));
        ostream.write(const_cast<char *>(obj.c_str()), static_cast<std::streamsize>(size));
    }

    template<>
    void BinaryConverter::serialize<char>(char &obj, std::ostream &ostream) {

        type t = type::CHAR;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<const char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(char));
    }

    template<>
    void BinaryConverter::serialize<unsigned char>(unsigned char &obj, std::ostream &ostream) {
        type t = type::UNSIGNED_CHAR;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<const char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(unsigned char));
    }

    template<>
    void BinaryConverter::serialize<bool>(bool &obj, std::ostream &ostream) {
        type t = type::BOOL;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<const char *>(&t), 1);
        ostream.write(reinterpret_cast<char *>(&obj), sizeof(bool));
    }

    template<typename T>
    type BinaryConverter::find_type(T) {
        if (typeid(T) == typeid(int))return INT_ARRAY;
        else if (typeid(T) == typeid(unsigned int)) return UNSIGNED_INT_ARRAY;
        else if (typeid(T) == typeid(short)) return SHORT_ARRAY;
        else if (typeid(T) == typeid(unsigned short)) return UNSIGNED_SHORT_ARRAY;
        else if (typeid(T) == typeid(long)) return LONG_ARRAY;
        else if (typeid(T) == typeid(unsigned long)) return UNSIGNED_LONG_ARRAY;
        else if (typeid(T) == typeid(long long)) return LONG_LONG_ARRAY;
        else if (typeid(T) == typeid(unsigned long long)) return UNSIGNED_LONG_LONG_ARRAY;
        else if (typeid(T) == typeid(float)) return FLOAT_ARRAY;
        else if (typeid(T) == typeid(double)) return DOUBLE_ARRAY;
        else if (typeid(T) == typeid(long double)) return LONG_DOUBLE_ARRAY;
        else if (typeid(T) == typeid(std::string)) return STRING_ARRAY;
        else if (typeid(T) == typeid(char)) return CHAR_ARRAY;
        else if (typeid(T) == typeid(unsigned char)) return UNSIGNED_CHAR_ARRAY;
        else if (typeid(T) == typeid(bool)) return BOOL_ARRAY;
        else throw std::invalid_argument("Unsupported array type");
    }

    template<typename T, std::size_t N>
    void BinaryConverter::serialize(T (&arr)[N], std::ostream &ostream) {
        type t = find_type(arr[0]);
        size_t size = N;
        system_type st = detect_system_type();

        ostream.write(reinterpret_cast<char *>(&st), 1);
        ostream.write(reinterpret_cast<const char *>(&t), 1);
        ostream.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
        for (const auto &elem: arr) {
            serialize_element(elem, ostream);
        }
    }

    template<typename T>
    void BinaryConverter::serialize_element(const T &elem, std::ostream &ostream) {
        if constexpr (std::is_same<T, std::string>::value) {
            std::size_t elem_size = elem.size();
            ostream.write(reinterpret_cast<const char *>(&elem_size), sizeof(size_t));
            ostream.write(elem.data(), elem_size);
        } else {
            ostream.write(reinterpret_cast<const char *>(&elem), sizeof(T));
        }
    }
}
#endif //BINARY_DATA_PROCESSING_BINARYCONVERTER_HPP
