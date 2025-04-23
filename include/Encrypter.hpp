/**
 * @file Encrypter.hpp
 * @author username (username52247554@gmail.com)
 * @brief functions to encrypt and decrypt passwords, as well as hashing the master password
 * @date 2025-04-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <Structure.hpp>

namespace TP
{
    /**
     * @brief Hashing a string using Argon2id. Used to hash the master password the user hase inputed.
     *
     * @param password  the string (master password) to hash.
     * @return std::vector<unsigned char>
     */
    std::string hash(const std::string &password);

    /**
     * @brief Verify whether the hashing of an input string match the stored hash. Used to verify the master password.
     * 
     * @param password the password user input
     * @param stored_hash the hash of password stored in the database
     */
    const bool verifyHashing(const std::string &password, const std::string &stored_hash);

    /**
     * @brief Encrypt a string using AES with the master password as the key.
     * 
     * @param plaintext the string to be encrypted
     * @param master_password the key
     * @return EncryptedStr a structure consists ciphertext, salt and nonce as string
     */
    EncryptedStr encrypt(const std::string &plaintext, const std::string master_password);

    /**
     * @brief Decrypting the ciphertext using the master password.
     *
     * @param encrypted_str a structure consists ciphertext, salt and nonce as string
     * @param master_password the key
     * @return std::string the plaintext
     */
    std::string decrypt(EncryptedStr &encrypted_str, const std::string master_password);
} // namespace TP

namespace
{
    /**
     * @brief convert a byte vector into a string
     */
    std::string toString(const std::vector<unsigned char> vector);

    /**
     * @brief convert a string into a byte vector
     */
    std::vector<unsigned char> toVector(const std::string str);
    
    TP::EncryptedData encryptVault(const std::vector<unsigned char> &key, const std::vector<unsigned char> &plaintext);

    std::vector<unsigned char> decryptVault(const std::vector<unsigned char> &key, const TP::EncryptedData &encrypted);

    std::vector<unsigned char> encryptSalt();

    std::vector<unsigned char> deriveEncryptionKey(const std::string &master_password, const std::vector<unsigned char> &salt);

    std::string toBase64(const std::vector<unsigned char> binary);
    std::vector<unsigned char> toBinary(std::string base64);
}