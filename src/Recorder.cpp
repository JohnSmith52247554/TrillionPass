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

namespace
{
    // TODO: encryt and save it in a file
    const std::string MASTER_PASSWORD = "password";
}

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

    const bool checkMasterPassword(std::istream &in, std::ostream &out)
    {
        std::string input;

#ifdef _WIN32
        char ch;
        while ((ch = _getch()) != '\r') // get character until newline
        {
            if (ch == '\b' && !input.empty())
            { // deal with delete
                input.pop_back();
                out << "\b \b"; // clear console
            }
            else if (ch != '\b')
            {
                input.push_back(ch);
                out << "*";
            }
        }
        out << "\n";
#endif

        if (input == MASTER_PASSWORD)
            return true;
        else
            return false;
    }

    const bool login(std::istream &in, std::ostream &out)
    {
        out << "Please enter MASTER PASSWORD to log in.\nMASTER PASSWORD: ";
        if (!checkMasterPassword(in, out))
        {
            out << "Password doesn't match. Log in failed.\n"
                << std::endl;
            return false;
        }
        else
        {
            out << "Logging in success.\n"
                << std::endl;
            return true;
        }
    }

    void interactiveCreate(std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data)
    {
        out << "Create Keychain\n============================" << std::endl;

        if (!login(in, out))
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
                    out << "The original keychain will vanish FOREVER. To force overwrite, please enter the master password. Enter any thing other to abandon overwriting." << std::endl;
                    out << "MASTER PASSWORD: ";
                    if (checkMasterPassword(in, out))
                    {
                        out << "Keychain \"" << key.name << "\" will be overwritten." << std::endl;
                        data->erase(key.name);
                        break;
                    }
                    else
                    {
                        out << "Abandon overwriting." << std::endl;
                    }
                }

                out << "Please set another name." << std::endl;
                continue;
            }
            else
                break;
        }

        out << "Discription for \"" << key.name << "\": ";
        std::getline(in, key.brief);

        out << "The name of the account: ";
        std::getline(in, key.account_name);

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

        key.encryted_password = raw_password;

        data->add(key);
        data->flush();
        out << "Keychain has been saved." << std::endl;
    }
}