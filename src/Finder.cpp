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
#include <MasterPassword.hpp>
#include <Encrypter.hpp>

namespace TP
{
    void find(std::string name, std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data)
    {
        out << "Find Keychain\n============================" << std::endl;
        std::string master_password;
        if (!login(in, out, master_password))
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
            std::string decrypted = TP::decrypt(result.encrypted_password, master_password);
            in.get();
            copyToClipboard(decrypted);
            out << "Password has been copied to the clipboard." << std::endl;
            TP::clean(std::move(decrypted));
        }
        TP::clean(std::move(master_password));
    }

    void deleteKeyChain(std::string name, std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data)
    {
        out << "Delete Keychain\n============================" << std::endl;

        std::string master_password;
        if (!login(in, out, master_password))
            return;
        TP::clean(std::move(master_password));

        if (name == "")
        {
            while (true)
            {
                out << "The name of keychain to be delete: ";
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
            out << "Could not found keychain \"" << name << "\"." << std::endl;
            return;
        }

        out << "Keychain\n    name: " << result.name << "\n    brief: " << result.brief << "\n    account name: " << result.account_name << std::endl;
        out << "The keychain will be deleted forever.\nTo force detele, please enter the master password.\nEnter anything else to abort.\nMASTER PASSWORD: ";
        std::string n_mp;
        if (!checkMasterPassword(in, out, n_mp))
        {
            out << "Aborted." << std::endl;
            TP::clean(std::move(n_mp));
            return;
        }
        TP::clean(std::move(n_mp));

        data->erase(name);
        data->flush();

        out << "Delete keychain succuss." << std::endl;
    }

    void listAllKeyChains(std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data)
    {
        out << "List All Keychains\n============================" << std::endl;
        std::string master_password;
        if (!login(in, out, master_password)) 
            return;
        TP::clean(std::move(master_password));

        for (size_t i = 0; i < data->size(); i++)
        {
            out << i + 1 << ":\n";
            auto key = data->find(i);
            out << "name:  " << key.name << '\n';
            out << "brief: " << key.brief << '\n';
            out << std::endl;
        }
    }
}