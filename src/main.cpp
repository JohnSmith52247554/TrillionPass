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
const std::string BINARY_DATA_PATH = "/data/KeyChain.bin";

// structure to store command line parameters
struct CLParameters
{
    std::string command;    // should begin with '-'
    std::vector<std::string> argument;
};


int main(int argc, char* argv[])
{
try{
    if (!TP::existsMasterPassword())
        TP::initMasterPassword(std::cin, std::cout);
    TP::checkAndCreatDataBase();

    // parse command
    CLParameters parameters;
    std::vector<CLParameters> commands;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (parameters.command != "")
                commands.push_back(parameters);
            parameters.command = argv[i];
            parameters.argument.clear();
        }
        else
        {
            parameters.argument.push_back(argv[i]);
        }
    }
    if (parameters.command != "")
        commands.push_back(parameters);

    if (commands.size() > 0)
    {
        // create new keychain
        if (commands[0].command == "-c")
        {
            if (commands.size() > 1) // create
            {
                std::unique_ptr<TP::Data::PasswordData> data = std::make_unique<TP::Data::BinaryPData>(std::string(PROJECT_PATH) + BINARY_DATA_PATH);

                if (commands[1].command == "-i") // interactive
                    TP::interactiveCreate(std::cin, std::cout, std::move(data));
                else
                {
                    TP::KeyChain key;
                    for (size_t i = 1u; i < commands.size(); i++)
                    {
                        const auto &command = commands[i];
                        if (command.command == "-name")
                        {
                            if (command.argument.size() > 0)
                                key.name = command.argument[0];
                        }
                        else if (command.command == "-brief")
                        {   
                            for (const auto& str : command.argument)
                            {
                                key.brief += str + ' ';
                            }
                        }
                        else if (command.command == "-account")
                        {
                            for (const auto &str : command.argument)
                            {
                                key.account_name += str + ' ';
                            }
                        }
                    }

                    TP::quickCreate(std::cin, std::cout, key, std::move(data));
                }
            }
        }
        else if (commands[0].command == "-f") // find
        {
            if (commands[0].argument.size() == 0)
                commands[0].argument.push_back("");

            std::unique_ptr<TP::Data::PasswordData> data = std::make_unique<TP::Data::BinaryPData>(std::string(PROJECT_PATH) + BINARY_DATA_PATH);

            TP::find(commands[0].argument[0], std::cin, std::cout, std::move(data));
        }
        else if (commands[0].command == "-v") // version
        {
            std::cout << "TrillionPass v" << TPASS_VERSION << std::endl;
        }
        else if (commands.size() > 1 && commands[1].command == "-l") // list all keychains
        {
            std::unique_ptr<TP::Data::PasswordData> data = std::make_unique<TP::Data::BinaryPData>(std::string(PROJECT_PATH) + BINARY_DATA_PATH);

            TP::listAllKeyChains(std::cin, std::cout, std::move(data));
        }
        else if (commands[0].command == "-mp") // master password
        {
            if (commands.size() > 1 && commands[1].command == "-c") // change
            {
                TP::changeMasterPassword(std::cin, std::cout);
            }
        }
        else if (commands[0].command == "-d") // delete
        {
            if (commands[0].argument.size() == 0)
                commands[0].argument.push_back("");

            std::unique_ptr<TP::Data::PasswordData> data = std::make_unique<TP::Data::BinaryPData>(std::string(PROJECT_PATH) + BINARY_DATA_PATH);

            TP::deleteKeyChain(commands[0].argument[0], std::cin, std::cout, std::move(data));
        }
        else
        {
            std::cout << "Command not found." << std::endl;
        }
    }
    else
    {
        std::cout << "Trillion Pass" << std::endl;
    }
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
    
    return 0;
}