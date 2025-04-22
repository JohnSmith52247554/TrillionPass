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
            json << file;
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
                    KeyChain output = {
                        json[i]["name"],
                        json[i]["brief"],
                        json[i]["username"],
                        json[i]["encrypted_password"]};
                    return output;
                }
            }
            KeyChain blank_output;
            return blank_output;
        }

        const bool JsonPData::overwrite(int offset, KeyChain key)
        {
            if (offset < 0 || offset >= json.size())
                return false;

            nlohmann::json overwritten_element = {
                {"name", key.name},
                {"brief", key.brief},
                {"username", key.username},
                {"encryted_password", key.encryted_password}};
            json[offset] = overwritten_element;

            return true;
        }

        void JsonPData::add(KeyChain key)
        {
            nlohmann::json new_element = {
                {"name", key.name},
                {"brief", key.brief},
                {"username", key.username},
                {"encrypted_password", key.encryted_password}};
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
    }
}