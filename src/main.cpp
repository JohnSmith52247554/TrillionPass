/**
 * @file main.cpp
 * @author username (username52247554@gmail.com)
 * @brief the main fuction
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <Recorder.hpp>
#include <Data.hpp>
#include <Finder.hpp>
#include <Encrypter.hpp>
#include <MasterPassword.hpp>

// TODO: choose the filepath according to username
const std::string DATA_PATH = "/data/KeyChain.json";
const std::string BINARY_DATA_PATH = "/data/KeyChain.bin";

// structure to store command line parameters
struct CLParameters
{
    std::string command;    // should begin with '-']
    std::vector<std::string> argument;
};

void test(int& argc, char** argv[])
{
    argc = 3;
    *argv = new char* [argc];
    for (int i = 0; i < argc; i++)
    {
        (*argv)[i] = new char[10];
    }
    (*argv)[0] = "pass";
    (*argv)[1] = "-c";
    (*argv)[2] = "-i";
}

int main(int argc, char* argv[])
{
    //TEMP::setMasterPassword("password");
    //test(argc, &argv);

    if (!TP::existsMasterPassword())
        TP::initMasterPassword(std::cin, std::cout);
    TP::checkAndCreatDataBase();

    // parse command
    CLParameters parameters;
    for (int i = 1; i < argc; i++)
    {
        if (i == 1 && argv[i][0] == '-')
        {
            parameters.command = argv[i];
        }
        else
        {
            parameters.argument.push_back(argv[i]);
        }
    }

    if (parameters.command != "")
    {
        // create new keychain
        if (parameters.command == "-c")
        {
            if (parameters.argument.size() > 0 && parameters.argument[0] == "-i") // interactive create
            {
                std::unique_ptr<TP::Data::PasswordData> data = std::make_unique<TP::Data::BinaryPData>(std::string(PROJECT_PATH) + BINARY_DATA_PATH);
                
                TP::interactiveCreate(std::cin, std::cout, std::move(data));
            }
        }
        else if (parameters.command == "-f")    // find
        {
            if (parameters.argument.size() == 0)
                parameters.argument.push_back("");

            std::unique_ptr<TP::Data::PasswordData> data = std::make_unique<TP::Data::BinaryPData>(std::string(PROJECT_PATH) + BINARY_DATA_PATH);

            TP::find(parameters.argument[0], std::cin, std::cout, std::move(data));
        }
        else if (parameters.command == "-v")    // version
        {
            std::cout << "TrillionPass v" << TPASS_VERSION << std::endl;
        }
        else if (parameters.command == "-l")    // list all keychains
        {
            std::unique_ptr<TP::Data::PasswordData> data = std::make_unique<TP::Data::BinaryPData>(std::string(PROJECT_PATH) + BINARY_DATA_PATH);

            TP::listAllKeyChains(std::cin, std::cout, std::move(data));
        }
        else if (parameters.command == "-mp")   // master password
        {
            if (parameters.argument.size() > 0 && parameters.argument[0] == "-c")   // change
            {
                TP::changeMasterPassword(std::cin, std::cout);
            }
        }
        else if (parameters.command == "-d")    // delete
        {
            if (parameters.argument.size() == 0)
                parameters.argument.push_back("");

            std::unique_ptr<TP::Data::PasswordData> data = std::make_unique<TP::Data::BinaryPData>(std::string(PROJECT_PATH) + BINARY_DATA_PATH);

            TP::deleteKeyChain(parameters.argument[0], std::cin, std::cout, std::move(data));
        }
        else
        {
            std::cout << "Command not found." << std::endl;
        }
    }
    else
    {
        std::cout << "Command not found." << std::endl;
    }

    std::cin.get();
    
    return 0;
}