//
// Created by rafae on 1/20/2024.
//
/**
 * @file main.cpp
 * @brief The main of this project.
 * @author Rafael Costin Balan
 * @date 2024-01-05
 * @version 1.0
 * @details It has an demonstration on how the
 * @copyright Rafael Public License
 */
#include <fstream>
#include <iostream>
#include "inc/BinaryConverter.hpp"

int main() {
    int ccc = 1;
    std::ofstream nei("bianry_file.bin");
    CES::BinaryConverter::serialize(ccc, nei);
    nei.close();

    std::ifstream noi("bianry_file.bin");
    int test;
    CES::BinaryConverter::deserialize(test, noi);

    std::cout<<test;
    return 0;
}