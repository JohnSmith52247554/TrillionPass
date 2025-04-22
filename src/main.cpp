/**
 * @file main.cpp
 * @author username (username52247554@gmail.com)
 * @brief the main fuction
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <pch.hpp>
#include <GenPassword.hpp>

int main(int argv, char *argc)
{
    for (int i = 0; i < 10; i++)
    {
        auto password = TP::RandomPasswordGenerator::generate(20, true, true, true);
        std::cout << password << std::endl;
    }
    return 0;
}