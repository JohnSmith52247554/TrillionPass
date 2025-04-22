/**
 * @file Finder.hpp
 * @author username (username52247554@gmail.com)
 * @brief find keychain in the file
 * @date 2025-04-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <Finder.hpp>

namespace TP
{
    void find(std::string name, std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data)\
    {
        out << "Find Keychain\n============================" << std::endl;
        if (!login(in, out))
            return;
        
        if (name == "")
        {
            while (true)
            {
                out << "The name of keychain to be found: ";
                std::getline(in, name);
                if (name == "")
                {
                    out << "The name should not be empty, please try again." << std::endl;
                    continue;
                }
                if (name.find(' ') != std::string::npos)
                {
                    out << "The name should not contain space, please try again." << std::endl;
                    continue;
                }
                break;
            }
        }

        auto result = data->find(name);

        if (result.name == "")
        {
            out << "Keychain \"" << name << "\" not found." << std::endl;
            return;
        }
        else
        {
            out << "Keychain\n    name: " << result.name << "\n    brief: " << result.brief << "\n    account name: " << result.account_name << std::endl;
            copyToClipboard(result.account_name);
            out << "Account name has been copied to the clipboard. Press any key to continue.";
            in.get();
            copyToClipboard(result.encryted_password);
            out << "Password has been copied to the clipboard." << std::endl;
        }
    }

    void listAllKeyChains(std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data)
    {
        out << "List All Keychains\n============================" << std::endl;
        if (!login(in, out))
            return;

        for (size_t i = 0; i < data->size(); i++)
        {
            out << i + 1 << ":\n";
            auto key = data->find(i);
            out << "name: " << key.name << '\n';
            out << "brief" << key.brief << '\n';
            out << std::endl;
        }
    }
}