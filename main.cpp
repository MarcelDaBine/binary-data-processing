//
// Created by rafae on 1/20/2024.
//
#include <fstream>
#include <iostream>
#include "inc/binary_processor.h"

int main() {
    int ccc = 1;
    std::ofstream nei("bianry_file.bin");
    CES::binary_processor::serialize(ccc, nei);
    nei.close();

    std::ifstream noi("bianry_file.bin");
    int test;
    CES::binary_processor::deserialize(test, noi);

    std::cout<<test;
    return 0;
}