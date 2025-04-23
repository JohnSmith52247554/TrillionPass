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
        DataIterator &JsonDI::operator++()
        {
            ++it;
            return *this;
        }

        DataIterator &JsonDI::operator--()
        {
            --it;
            return *this;
        }

        DataIterator &JsonDI::operator+=(size_t n)
        {
            it += n;
            return *this;
        }

        DataIterator &JsonDI::operator-=(size_t n)
        {
            it -= n;
            return *this;
        }

        KeyChain &JsonDI::operator*() const
        {
            parseJsonElement();
            return *key;
        }

        KeyChain *JsonDI::operator->() const
        {
            parseJsonElement();
            return key.get();
        }

        bool JsonDI::operator==(const JsonDI &other) const
        {
            return it == other.it;
        }

        void JsonDI::parseJsonElement() const
        {
            key = std::make_unique<KeyChain>();
            key->name = (*it)["name"];
            key->brief = (*it)["brief"];
            key->account_name = (*it)["account_name"];
            key->encrypted_password.nonce = (*it)["encryted_password"]["nonce"];
            key->encrypted_password.ciphertext = (*it)["encryted_password"]["ciphertext"];
            key->encrypted_password.salt = (*it)["encryted_password"]["salt"];
        }

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
                json[offset]["encrypted_password"]["salt"]
            };
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
                {"encryted_password", ep}
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
            try{
            std::ofstream file(file_path);
            if (!file.is_open())
                throw std::runtime_error("cannot open keychain data file " + file_path);
            file << json.dump(4);
            file.close();
            }
            catch(std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
        }

        size_t JsonPData::size() const
        {
            return json.size();
        }

        JsonDI JsonPData::begin()
        {
            return JsonDI(json.begin());
        }

        JsonDI JsonPData::end()
        {
            return JsonDI(json.end());
        }
    }
}