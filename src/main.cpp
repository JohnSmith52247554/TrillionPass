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
#include <Recorder.hpp>
#include <Data.hpp>

// TODO: choose the filepath according to username
const std::string DATA_PATH = "/DATA/test.json";

int main(int argc, char *argv[])
{
    std::unique_ptr<TP::Data::PasswordData> data = std::make_unique<TP::Data::JsonPData>(std::string(PROJECT_PATH) + DATA_PATH);
    
    TP::interactiveCreate(std::cin, std::cout, std::move(data));
    return 0;
}