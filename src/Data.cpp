/**
 * @file Data.cpp
 * @author username (username52247554@gmail.com)
 * @brief classed to operate password data file
 * @date 2025-04-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <Data.hpp>

namespace TP
{
    namespace Data
    {
        JsonPData::JsonPData(const std::string filepath)
            : file_path(filepath)
        {
            std::ifstream file(filepath);
            if (!file.is_open())
                throw std::runtime_error("cannot open keychain data file " + filepath);
            file >> json;
            file.close();
        }

        JsonPData::~JsonPData()
        {
            flush();
        }

        const int JsonPData::exists(std::string name)
        {
            for (size_t i = 0; i < json.size(); i++)
            {
                if (json[i]["name"] == name)
                    return i;
            }
            return -1;
        }

        KeyChain JsonPData::find(std::string name)
        {
            for (size_t i = 0; i < json.size(); i++)
            {
                if (json[i]["name"] == name)
                {
                    EncryptedStr es = {
                        json[i]["encrypted_password"]["nonce"],
                        json[i]["encrypted_password"]["ciphertext"],
                        json[i]["encrypted_password"]["salt"]
                    };
                    KeyChain output = {
                        json[i]["name"],
                        json[i]["brief"],
                        json[i]["account_name"],
                        es
                    };
                    return output;
                }
            }
            KeyChain blank_output;
            return blank_output;
        }

        TP::KeyChain JsonPData::find(const int offset)
        {
            if (offset < 0 || offset >= json.size())
            {
                KeyChain blank_output;
                return blank_output;
            }
            EncryptedStr es = {
                json[offset]["encrypted_password"]["nonce"],
                json[offset]["encrypted_password"]["ciphertext"],
                json[offset]["encrypted_password"]["salt"]};
            KeyChain output = {
                json[offset]["name"],
                json[offset]["brief"], 
                json[offset]["account_name"], 
                es
            };
            return output;
            
        }

        const bool JsonPData::overwrite(int offset, KeyChain key)
        {
            if (offset < 0 || offset >= json.size())
                return false;

            nlohmann::json ep = {
                {"nonce", key.encrypted_password.nonce},
                {"ciphertext", key.encrypted_password.ciphertext},
                {"salt", key.encrypted_password.salt}
            };
            nlohmann::json overwritten_element = {
                {"name", key.name},
                {"brief", key.brief},
                {"account_name", key.account_name},
                {"encrypted_password", ep}
            };
            json[offset] = overwritten_element;

            return true;
        }

        void JsonPData::add(KeyChain key)
        {
            nlohmann::json ep = {
                {"nonce", key.encrypted_password.nonce},
                {"ciphertext", key.encrypted_password.ciphertext},
                {"salt", key.encrypted_password.salt}
            };
            nlohmann::json new_element = { 
                {"name", key.name}, 
                {"brief", key.brief}, 
                {"account_name", key.account_name}, 
                {"encrypted_password", ep}
                };
            json.push_back(new_element);
        }

        const bool JsonPData::erase(const int offset)
        {
            if (offset < 0 || offset >= json.size())
                return false;

            json.erase(json.begin() + offset);
            return true;
        }

        const bool JsonPData::erase(const std::string name)
        {
            for (size_t i = 0; i < json.size(); ++i)
            {
                if (json[i]["name"] == name)
                {
                    json.erase(json.begin() + i);
                    return true;
                }
            }
            return false;
        }

        void JsonPData::flush()
        {
            std::ofstream file(file_path);
            if (!file.is_open())
                throw std::runtime_error("cannot open keychain data file " + file_path);
            file << json.dump(4);
            file.close();
        }

        size_t JsonPData::size() const
        {
            return json.size();
        }

        BinaryPData::BinaryPData(const std::string filepath)
            : file_path(filepath)
        {
            std::ifstream file(file_path);
            if (!file.is_open())
                throw std::runtime_error("cannot open keychain data file " + filepath);
            
            while (true)
            {
                KeyChain key;
                deserializeStr(file, key.name);
                deserializeStr(file, key.brief);
                deserializeStr(file, key.account_name);
                deserializeStr(file, key.encrypted_password.ciphertext);
                deserializeStr(file, key.encrypted_password.nonce);
                deserializeStr(file, key.encrypted_password.salt);
                if (file.eof())
                    break;
                data.push_back(key);
            }
            file.close();
        }

        BinaryPData::~BinaryPData()
        {
            flush();
        }

        const int BinaryPData::exists(std::string name)
        {
            int i = 0;
            for (; i < data.size(); i++)
            {
                if (data[i].name == name)
                    return i;
            }
            return -1;
        }

        TP::KeyChain BinaryPData::find(std::string name)
        {
            for (int i = 0; i < data.size(); i++)
            {
                if (data[i].name == name)
                    return data[i];
            }
            KeyChain blank_keychain;
            return blank_keychain;
        }

        TP::KeyChain BinaryPData::find(const int offset)
        {
            if (offset >= 0 && offset < data.size())
            {
                return data[offset];
            }
            KeyChain blank_keychain;
            return blank_keychain;
        }

        const bool BinaryPData::overwrite(int offset, TP::KeyChain key)
        {
            if (offset < 0 || offset >= data.size())
                return false;
            data[offset] = key;
            return true;
        }

        void BinaryPData::add(TP::KeyChain key)
        {
            data.push_back(key);
        }

        const bool BinaryPData::erase(const int offset)
        {
            if (offset < 0 || offset >= data.size())
                return false;
            data.erase(data.begin() + offset);
            return true;
        }

        const bool BinaryPData::erase(const std::string name)
        {
            for (auto it = data.begin(); it != data.end(); it++)
            {
                if ((*it).name == name)
                {
                    data.erase(it);
                    return true;
                }
            }
            return false;
        }

        void BinaryPData::flush()
        {
            std::ofstream file(file_path);
            if (!file.is_open())
                throw std::runtime_error("cannot open keychain data file " + file_path);

            for (auto& key : data)
            {
                serializeStr(file, key.name);
                serializeStr(file, key.brief);
                serializeStr(file, key.account_name);
                serializeStr(file, key.encrypted_password.ciphertext);
                serializeStr(file, key.encrypted_password.nonce);
                serializeStr(file, key.encrypted_password.salt);
            }
            file.close();
        }

        size_t BinaryPData::size() const
        {
            return data.size();
        }

        void serializeStr(std::ofstream &file, std::string &str)
        {
            size_t size = str.size();
            file.write(reinterpret_cast<char *>(&size), sizeof(size));
            for (size_t i = 0; i < size; ++i)
            {
                unsigned char byte = str[i];
                file << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
            }
        }

        void deserializeStr(std::ifstream &file, std::string &str)
        {
            std::string hex_str;
            size_t size;
            file.read(reinterpret_cast<char *>(&size), sizeof(size));
            hex_str.resize(size * 2);
            file.read(reinterpret_cast<char*>(hex_str.data()), hex_str.size());
            for (size_t i = 0; i < hex_str.size(); i += 2)
            {
                std::string byte = hex_str.substr(i, 2);
                char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
                str.push_back(chr);
            }
        }
    }
}