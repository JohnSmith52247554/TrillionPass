/**
 * @file MasterPassword.hpp
 * @author username (username52247554@gmail.com)
 * @brief manage master password
 * @date 2025-04-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <MasterPassword.hpp>
#include <Encrypter.hpp>

const std::string USER_CONFIG_PATH = std::string(PROJECT_PATH) + "/data/UserConfig.json";

namespace TP
{
    const bool checkMasterPassword(std::istream &in, std::ostream &out, std::string &master_password)
    {
        master_password.clear();

#ifdef _WIN32
        char ch;
        while ((ch = _getch()) != '\r') // get character until newline
        {
            if (ch == '\b' && !master_password.empty())
            { // deal with delete
                master_password.pop_back();
                out << "\b \b"; // clear console
            }
            else if (ch != '\b')
            {
                master_password.push_back(ch);
                out << "*";
            }
        }
        out << "\n";
#endif

        return checkMasterPasswordHash(master_password);
    }

    const bool login(std::istream &in, std::ostream &out, std::string& master_password)
    {
        out << "Please enter the MASTER PASSWORD to log in.\nMASTER PASSWORD: ";
        if (!checkMasterPassword(in, out, master_password))
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

    const bool checkMasterPasswordHash(std::string raw_input)
    {
        std::ifstream file(USER_CONFIG_PATH);
        if (!file.is_open())
            throw std::runtime_error("open user config failed: " + USER_CONFIG_PATH);
        
        nlohmann::json json;
        file >> json;
        std::string master_password_hash = json["master_password_hash"];
        file.close();

        return TP::verifyHashing(raw_input, master_password_hash);
    }
}