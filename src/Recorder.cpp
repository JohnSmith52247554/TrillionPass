/**
 * @file Recorder.cpp
 * @author account_name (username52247554@gmail.com)
 * @brief record the generated password
 * @date 2025-04-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <Recorder.hpp>
#include <GenPassword.hpp>
#include <Structure.hpp>
#include <MasterPassword.hpp>
#include <Encrypter.hpp>

namespace TP
{
    void copyToClipboard(const std::string &text)
    {
#ifdef _WIN32
        // open clipboard
        if (OpenClipboard(NULL))
        {
            // empty clipboard
            EmptyClipboard();

            // the string to be copied
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
            if (hMem)
            {
                char *pMem = static_cast<char *>(GlobalLock(hMem));
                if (pMem)
                {
                    memcpy(pMem, text.c_str(), text.size() + 1);
                    GlobalUnlock(hMem);

                    // set clipboard
                    SetClipboardData(CF_TEXT, hMem);
                }
                else
                {
                    GlobalFree(hMem);
                }
            }

            // close clipboard
            CloseClipboard();
        }
        else
        {
            throw std::runtime_error("unable to open clipboard");
        }
#endif
    }

    void interactiveCreate(std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data)
    {
        out << "Create Keychain\n============================" << std::endl;

        std::string master_password;
        if (!login(in, out, master_password))
            return;

        KeyChain key;

        while (true)
        {
            out << "name: ";
            std::getline(in, key.name);
            if (key.name == "")
            {
                out << "The name should not be empty, please try again." << std::endl;
                continue;
            }
            if (key.name.find(' ') != std::string::npos)
            {
                out << "The name should not contain space, please try again." << std::endl;
                continue;
            }
            auto found = data->find(key.name);
            if (found.name != "")
            {
                out << "The name you have enter already exist." << std::endl;
                out << "   name: " << found.name << "\n   brief: " << found.brief << "\nWould you like to overwrite it? [y/\033[4mn\033[0m]: ";
                std::string input;
                std::getline(in, input);
                if (input == "y" || input == "Y" || input == "yes" || input == "Yes" || input == "YES")
                {
                    out << "The original keychain will be deleted FOREVER.\nTo force overwrite, please enter the master password.\nEnter any thing other to abort." << std::endl;
                    out << "MASTER PASSWORD: ";
                    std::string str;
                    if (checkMasterPassword(in, out, str))
                    {
                        out << "Keychain \"" << key.name << "\" will be overwritten." << std::endl;
                        data->erase(key.name);
                        break;
                    }
                    else
                    {
                        out << "Aborted." << std::endl;
                    }
                    TP::clean(std::move(str));
                }

                out << "Please set another name." << std::endl;
                continue;
            }
            else
                break;
        }

        out << "Discription for \"" << key.name << "\": ";
        std::getline(in, key.brief);


        while (true)
        {
            out << "The name of the account: ";
            std::getline(in, key.account_name);
            if (key.account_name == "")
            {
                out << "Account name can not be empty." << std::endl;
                continue;
            }
            break;
        }

        uint8_t length = 16;
        std::string input;
        out << "Length of the password to be generated [\033[4m16\033[0m]: ";
        std::getline(in, input);
        if (input != "")
            length = std::stoi(input);

        bool upper = true, numbers = true, special = true;
        out << "Contain upper letters [\033[4my\033[0m/n]: ";
        std::getline(in, input);
        if (input == "n" || input == "N" || input == "no" || input == "No" || input == "NO")
            upper = false;
        out << "Contain numbers [\033[4my\033[0m/n]: ";
        std::getline(in, input);
        if (input == "n" || input == "N" || input == "no" || input == "No" || input == "NO")
            numbers = false;
        out << "Contain special characters [\033[4my\033[0m/n]: ";
        std::getline(in, input);
        if (input == "n" || input == "N" || input == "no" || input == "No" || input == "NO")
            special = false;

        std::string raw_password = TP::generate(length, upper, numbers, special);
        out << "Creating keychain success." << std::endl;
        copyToClipboard(raw_password);
        out << "Password has been copied to the clipboard." << std::endl;

        auto encrypted = TP::encrypt(raw_password, master_password);
        TP::clean(std::move(raw_password));
        key.encrypted_password = encrypted;
        TP::clean(std::move(master_password));
        data->add(key);
        data->flush();
        out << "Keychain has been saved." << std::endl;
    }

    void quickCreate(std::istream &in, std::ostream &out, TP::KeyChain &key, std::unique_ptr<TP::Data::PasswordData> &&data)
    {
        out << "Create Keychain\n============================" << std::endl;

        std::string master_password;
        if (!login(in, out, master_password))
            return;

        auto rename = [&]() { out << "name: ";
            std::getline(in, key.name);
        };
        while (true)
        {
            if (key.name == "")
            {
                out << "The name should not be empty, please try again." << std::endl;
                rename;
            }
            else if (key.name.find(' ') != std::string::npos)
            {
                out << "The name should not contain space, please try again." << std::endl;
                rename;
            }
            else
            {
                auto found = data->find(key.name);
                if (found.name != "")
                {
                    out << "The name you have enter already exist." << std::endl;
                    out << "   name: " << found.name << "\n   brief: " << found.brief << "\nWould you like to overwrite it? [y/\033[4mn\033[0m]: ";
                    std::string input;
                    std::getline(in, input);
                    if (input == "y" || input == "Y" || input == "yes" || input == "Yes" || input == "YES")
                    {
                        out << "The original keychain will be deleted FOREVER.\nTo force overwrite, please enter the master password.\nEnter any thing other to abort." << std::endl;
                        out << "MASTER PASSWORD: ";
                        std::string str;
                        if (checkMasterPassword(in, out, str))
                        {
                            out << "Keychain \"" << key.name << "\" will be overwritten." << std::endl;
                            data->erase(key.name);
                            break;
                        }
                        else
                        {
                            out << "Aborted." << std::endl;
                        }

                        TP::clean(std::move(str));
                    }

                    out << "Please set another name." << std::endl;
                    rename;
                }
            }
            break;
        }

        if (key.brief == "")
        {
            out << "brief: ";
            std::getline(in, key.brief);
        }

        if (key.account_name == "")
        {
            while (true)
            {
                out << "account: ";
                std::getline(in, key.account_name);
                if (key.account_name != "")
                    break;
                out << "Account should not be empty" << std::endl;
            }
        }

        std::string raw_password = TP::generate(16, true, true, true);
        out << "Creating keychain success." << std::endl;
        copyToClipboard(raw_password);
        out << "Password has been copied to the clipboard." << std::endl;

        auto encrypted = TP::encrypt(raw_password, master_password);
        TP::clean(std::move(raw_password));
        key.encrypted_password = encrypted;
        TP::clean(std::move(master_password));
        data->add(key);
        data->flush();
        out << "Keychain has been saved." << std::endl;
    }

    void checkAndCreatDataBase()
    {
        if (!std::filesystem::exists(std::string(PROJECT_PATH) + "/data/KeyChain.bin"))
        {
            std::ofstream file(std::string(PROJECT_PATH) + "/data/KeyChain.bin");
            file.close();
        }
    }
}