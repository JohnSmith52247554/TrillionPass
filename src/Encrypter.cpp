
#include <Encrypter.hpp>

namespace TP
{
    std::string hash(const std::string &password)
    {
        if (sodium_init() < 0)
        {
            throw std::runtime_error("Libsodium initialization failed");
        }

        // Argon2id parameters
        constexpr size_t opslimit = crypto_pwhash_argon2id_OPSLIMIT_MODERATE;
        constexpr size_t memlimit = crypto_pwhash_argon2id_MEMLIMIT_MODERATE;

        std::vector<unsigned char> hash(crypto_pwhash_STRBYTES);
        if (crypto_pwhash_str(
                reinterpret_cast<char *>(hash.data()),
                password.c_str(),
                password.length(),
                opslimit,
                memlimit) != 0)
        {
            throw std::runtime_error("hashing password failed");
        }

        return toBase64(hash); // $argon2id$v=19$m=...,t=...,p=...$salt$hash
    }

    const bool verifyHashing(const std::string &password, const std::string &stored_hash)
    {
        auto binary_hash = toBinary(stored_hash);
        return crypto_pwhash_str_verify(
                   reinterpret_cast<const char *>(binary_hash.data()),
                   password.c_str(),
                   password.length()) == 0;
    }

    EncryptedStr encrypt(const std::string &plaintext, const std::string master_password)
    {
        auto salt = encryptSalt();
        auto encryption_key = deriveEncryptionKey(master_password, salt);
        auto en_bin =  encryptVault(encryption_key, toVector(plaintext));
        EncryptedStr output;
        output.nonce = toBase64(en_bin.nonce);
        output.ciphertext = toBase64(en_bin.ciphertext);
        output.salt = toBase64(salt);
        return output;
    }

    std::string decrypt(EncryptedStr &encrypted_str, const std::string master_password)
    {
        EncryptedData encrypted;
        encrypted.nonce = toBinary(encrypted_str.nonce);
        encrypted.ciphertext = toBinary(encrypted_str.ciphertext);
        auto salt = toBinary(encrypted_str.salt);
        auto encryption_key = deriveEncryptionKey(master_password, salt);
        auto decrypted = decryptVault(encryption_key, encrypted);

        return toString(decrypted);
    }

    volatile void clean(std::string &&str)
    {
        for (auto& c : str)
            c = '\0';
        str.clear();
    }
} // namespace TP

namespace
{
    std::string toString(const std::vector<unsigned char> vector)
    {
        std::string str;
        for (const auto &c : vector)
        {
            if (c == 0u)
                break;
            str.push_back(c);
        }
        return str;
    }

    std::vector<unsigned char> toVector(const std::string str)
    {
        std::vector<unsigned char> vec;
        vec.reserve(str.size());
        for (char c : str)
            vec.push_back(static_cast<unsigned char>(c));
        vec.push_back('\000');
        return vec;
    }
    
    TP::EncryptedData encryptVault(const std::vector<unsigned char> &key, const std::vector<unsigned char> &plaintext)
    {
        TP::EncryptedData result;
        result.nonce.resize(crypto_aead_aes256gcm_NPUBBYTES); // 12 bytes
        randombytes_buf(result.nonce.data(), result.nonce.size());

        // length of ciphertext = length of plaintext + length of tag
        result.ciphertext.resize(plaintext.size() + crypto_aead_aes256gcm_ABYTES);
        unsigned long long ciphertext_len;

        crypto_aead_aes256gcm_encrypt(
            result.ciphertext.data(), &ciphertext_len,
            plaintext.data(), plaintext.size(),
            nullptr, 0,
            nullptr,
            result.nonce.data(),
            key.data());

        return result;
    }

    std::vector<unsigned char> decryptVault(const std::vector<unsigned char> &key, const TP::EncryptedData &encrypted)
    {
        std::vector<unsigned char> plaintext(encrypted.ciphertext.size());
        unsigned long long plaintext_len;

        if (crypto_aead_aes256gcm_decrypt(
                plaintext.data(), &plaintext_len,
                nullptr, // no additional data
                encrypted.ciphertext.data(), encrypted.ciphertext.size(),
                nullptr, 0, // no nonce prefix
                encrypted.nonce.data(),
                key.data()) != 0)
        {
            throw std::runtime_error("decrypting failed");
        }

        plaintext.resize(plaintext_len);
        return plaintext;
    }

    std::vector<unsigned char> encryptSalt()
    {
        std::vector<unsigned char> encryption_salt(crypto_pwhash_SALTBYTES);
        randombytes_buf(encryption_salt.data(), encryption_salt.size());
        return encryption_salt;
    }

    std::vector<unsigned char> deriveEncryptionKey(const std::string &master_password, const std::vector<unsigned char> &salt)
    {
        std::vector<unsigned char> key(crypto_aead_aes256gcm_KEYBYTES); // 32 bytes

        if (crypto_pwhash(
                key.data(), key.size(),
                master_password.c_str(), master_password.length(),
                salt.data(),
                crypto_pwhash_argon2id_OPSLIMIT_MODERATE,
                crypto_pwhash_argon2id_MEMLIMIT_MODERATE,
                crypto_pwhash_ALG_ARGON2ID13) != 0)
        {
            throw std::runtime_error("deriving encryption key failed");
        }

        return key;
    }

    std::string toBase64(const std::vector<unsigned char> binary)
    {
        size_t b64_maxlen = sodium_base64_encoded_len(binary.size(), sodium_base64_VARIANT_ORIGINAL);
        std::string base64;
        base64.resize(b64_maxlen);
        if (sodium_bin2base64(base64.data(), b64_maxlen, (const unsigned char *)binary.data(), binary.size(), sodium_base64_VARIANT_ORIGINAL) == NULL)
            throw std::runtime_error("base 64 failed");
        while (base64.back() == '\0')
            base64.pop_back();
        return base64;
    }

    std::vector<unsigned char> toBinary(std::string base64)
    {
        //base64.push_back('\000');
        std::vector<unsigned char> binary;
        binary.resize(base64.size());
        size_t bin_actual_len;
        if (sodium_base642bin(binary.data(), binary.size(), base64.data(), base64.size(), NULL, &bin_actual_len, NULL, sodium_base64_VARIANT_ORIGINAL) != 0)
            throw std::runtime_error("base 64 failed");
        binary.resize(bin_actual_len);
        return binary;
    }
}
