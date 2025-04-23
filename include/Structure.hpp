/**
 * @file Structure.hpp
 * @author username (username52247554@gmail.com)
 * @brief define some common-used structure
 * @date 2025-04-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

namespace TP
{
    struct EncryptedData
    {
        std::vector<unsigned char> nonce;
        std::vector<unsigned char> ciphertext;
        std::vector<unsigned char> salt;
    };

    struct EncryptedStr
    {
        std::string nonce;
        std::string ciphertext;
        std::string salt;
    };

    /**
     * @brief the structure to restore the infomation of a password
     *
     */
    struct KeyChain
    {
        std::string name;              // the name of the password ("Github" for example)
        std::string brief;             // a brief discription for the password
        std::string account_name;          // the account_name for the account
        EncryptedStr encrypted_password;   // the password after encrytion
    };
} // namespace TP
