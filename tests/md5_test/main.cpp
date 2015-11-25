#include <iostream>
#include <string>
#include <fstream>
extern "C"{
#include "md5.h"
}

const int MD5_RES_SIZE = 32;
const char file_name[] = "test.txt";
char md5_value[MD5_RES_SIZE + 1];

void eqTest()
{
    std::string empty_str_hash("d41d8cd98f00b204e9800998ecf8427e"),
                md5_str_hash("1bc29b36f623ba82aaf6724fd3b16718");

    std::ofstream test_file(file_name);

    makeHash(file_name, md5_value);
        std::cout << "Hash of empty string is: ";
    if (!empty_str_hash.compare(md5_value))
        std::cout << "correct" << std::endl;
    else
        std::cout << "not correct" << std::endl;

    test_file << "md5";
    test_file.close();

    makeHash(file_name, md5_value);
    std::cout << "Hash of 'md5' string is: ";
    if (!md5_str_hash.compare(md5_value))
        std::cout << "correct" << std::endl;
    else
        std::cout << "not correct" << std::endl;

    remove(file_name);
}

void noFileTest()
{
    std::cout << "If file not exist: ";
    if (makeHash(file_name, md5_value) == -1)
        std::cout << "function works correct" << std::endl;
    else
        std::cout << "function not works correct" << std::endl;
}

int main()
{
    eqTest();
    noFileTest();

    return 0;
}
