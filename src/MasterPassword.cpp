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
#include <Data.hpp>
#include <GenPassword.hpp>

const std::string USER_CONFIG_PATH = std::string(PROJECT_PATH) + "/data/UserConfig.json";

namespace TP
{
    const bool checkMasterPassword(std::istream &in, std::ostream &out, std::string &master_password)
    {
        TEMP::securetInput(in, out, master_password);

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

   void changeMasterPassword(std::istream &in, std::ostream &out)
    {
        out << "Change Master Password\n==================" << std::endl;
        std::string original_mp;
        if (!login(in, out, original_mp))
            return;
        out << "\nThe MASTER PASSWORD should matches the following conditions:\n";
        out << "\tFirstly, the new master password should be different from the original one\n";
        out << "\tSecondly, the length of the master password shoud be no less than twelve characters.\n";
        out << "\tThirdly, the master password should CONTAIN AND ONLY CONTAIN lower characters, upper characters, numbers and special characters.\n";
        out << "\tFourthly, there should not be a character appears consecutively more than two times.\n" << std::endl;

        std::string new_mp;
        while (true)
        {
            out << "Please enter the NEW master password: ";
            TEMP::securetInput(in, out, new_mp);

            if (new_mp == original_mp)
            {
                out << "The new master password should be different from the original one." << std::endl;
                continue;
            }
            if (new_mp.size() < 12)
            {
                out << "the length of the master password shoud be no less than twelve characters." << std::endl;
                continue;
            }
            if (!TEMP::chararcherCheck(new_mp))
            {
                out << "The master password should CONTAIN AND ONLY CONTAIN lower characters, upper characters, numbers and special characters." << std::endl;
                continue;
            }
            if (!TEMP::consecutiveCheck(new_mp, 2))
            {
                out << "There should not be a character appears consecutively more than two times." << std::endl;
                continue;
            }

            out << "Please enter the NEW master password again: ";
            std::string repeated_mp;
            TEMP::securetInput(in, out, repeated_mp);
            if (repeated_mp != new_mp)
            {
                out << "The input is different." << std::endl;
                continue;
            }

            break;
        }

        out << "Saving the new master password." << std::endl;
        TEMP::setMasterPassword(new_mp);

        out << "Rencrypting password database." << std::endl;
        TP::Data::JsonPData data(std::string(PROJECT_PATH) + "/data/KeyChain.json");
        for (size_t i = 0u; i < data.size(); i++)
        {
            auto keychain = data.find(i);
            auto plaintext = decrypt(keychain.encrypted_password, original_mp);
            keychain.encrypted_password = encrypt(plaintext, new_mp);
            data.overwrite(i, keychain);
        }
        out << "Changing master password complete." << std::endl;
    }
}

namespace TEMP
{
    void setMasterPassword(std::string password)
    {
        auto master_password = TP::hash(password);
        std::ifstream i_file(std::string(PROJECT_PATH) + "/data/UserConfig.json");
        nlohmann::json json;
        i_file >> json;
        json["master_password_hash"] = master_password;
        i_file.close();
        std::ofstream o_file(std::string(PROJECT_PATH) + "/data/UserConfig.json");
        o_file << json.dump(4);
        o_file.close();
    }

    const bool chararcherCheck(const std::string &password)
    {
        std::vector<unsigned char> apper_time(4, 0u);

        for (const auto& c : password)
        {
            if (c >= 'a' && c <= 'z')
                apper_time[0]++;
            else if (c >= 'A' && c <= 'Z')
                apper_time[1]++;
            else if (c >= '0' && c <= '9')
                apper_time[2]++;
            else if (std::find(TP::CharPool::special_char.begin(), TP::CharPool::special_char.end(), c) != TP::CharPool::special_char.end())
                apper_time[3]++;
            else
                return false;
        }

        for (const auto& num : apper_time)
        {
            if (num == '\0')
                return false;
        }
        return true;
    }

    const bool consecutiveCheck(const std::string &password, const unsigned char max_repeat_time)
    {
        char flag = '\0';
        unsigned char time = 0;
        for (auto it = password.begin(); it != password.end(); it++)
        {
            if (*it != flag)
            {
                flag = *it;
                time = 0;
                do{
                    time++;
                    it++;
                } while (*it == flag && it != password.end());
            }
            if (time >= max_repeat_time)
                return false;
        }
        return true;
    }

    void securetInput(std::istream &in, std::ostream &out, std::string &input)
    {
        input.clear();

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
    }
}