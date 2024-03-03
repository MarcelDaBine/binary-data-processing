//
// Created by rafae on 1/20/2024.
//
/**
 * @file main.cpp
 * @brief The main of this project.
 * @author Rafael Costin Balan
 * @date 2024-03-03
 * @version 1.0
 * @details It has an demonstration on how the BinaryConverter class works
 * @copyright CES Public License
 */
#include <fstream>
#include <iostream>
#include "inc/BinaryConverter.hpp"

int main() {
    bool ccc[8] = {1, 1, 0, 0, 1, 0, 1, 1};
    std::ofstream nei("bianry_file.bin");
    CES::BinaryConverter::serialize(ccc, nei);
    nei.close();

    std::ifstream noi("bianry_file.bin");
    bool test[8];
    CES::BinaryConverter::deserialize(test, noi);

    for(const auto &item: test){
        std::cout<<item<<' ';
    }
    return 0;
}