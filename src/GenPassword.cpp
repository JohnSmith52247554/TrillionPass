/**
 * @file GenPassword.cpp
 * @author username (username52247554@gmail.com)
 * @brief generate password
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <GenPassword.hpp>

#undef min

namespace TP
{
    using RPG = RandomPasswordGenerator;
   
    const std::vector<char> RPG::lower_char = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
        'u', 'v', 'w', 'x', 'y', 'z'};
    const std::vector<char> RPG::upper_char = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z'
    };
    const std::vector<char> RPG::numbers = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };
    const std::vector<char> RPG::special_char = {
        '~', '!', '@', '#', '$', '%', '^', '*', '(', ')',
        '-', '_', '+', '=', '[', ']', '{', '}', '?', '/'
    };

    std::string RPG::generate(unsigned char length, bool upper, bool number, bool special) 
    {
        if (length <= static_cast<unsigned char>(upper) + static_cast<unsigned char>(number) + static_cast<unsigned char>(special))
            throw std::runtime_error("length too small to meet the request");

        std::vector<const std::vector<char> *> char_pool = {&lower_char};
        if (upper)
            char_pool.push_back(&upper_char);
        if (number)
            char_pool.push_back(&numbers);
        if (special)
            char_pool.push_back(&special_char);

        uint32_t pool_size = 0u;
        for (auto pool : char_pool)
            pool_size += pool->size();

        std::vector<uint32_t> rand_dis; // true random distribution

#ifdef _WIN32
        // leave some addition, so we don't need to regenerate
        rand_dis = trueRandom(length + char_pool.size() - 1, pool_size);
#endif
        uint8_t additional = char_pool.size() - 1;

        std::vector<char> password(length + 1);
        password.back() = '\0';

        // record the time of characters of one pool contained in the password
        // used to ensure that characters of special types appear at least once
        std::vector<uint8_t> pool_contain_time(char_pool.size(), 0u);

        for (size_t i = 0u; i < length; i++)
        {
            auto rand = rand_dis[i];
            for (size_t j = 0u; j < char_pool.size(); j++)
            {
                auto pool = char_pool[j];
                if (rand < pool->size())
                {
                    password[i] = pool->at(rand);
                    pool_contain_time[j]++;
                    break;
                }
                else
                {
                    rand -= pool->size();
                }
            }
        }

        // check if characters of special types appear at least once
        for (size_t i = 0u; i < pool_contain_time.size(); i++)
        {
            // insert
            if (pool_contain_time[i] <= 0)
            {
                size_t j = 0u;
                for (; j < pool_contain_time.size(); j++)
                {
                    if (j == i)
                        continue;
                    if (pool_contain_time[j] >= 2)
                    {
                        pool_contain_time[j]--;
                        break;
                    }
                }

                for (auto &c : password)
                {
                    auto found = std::find(char_pool[j]->begin(), char_pool[j]->end(), c);
                    if (found != char_pool[j]->end())
                    {
                        uint32_t max_val = char_pool[i]->size();
                        uint8_t char_size = ceil(static_cast<float>(max_val) / static_cast<float>(UCHAR_MAX));
                        uint32_t merged_max = 0u;
                        for (; char_size > 0; char_size--)
                        {
                            merged_max += 0xff << (i * 8);
                        }
                        uint32_t value = std::min(max_val, static_cast<uint32_t>(static_cast<float>(rand_dis[length + additional - 1]) / static_cast<float>(merged_max) * static_cast<float>(max_val)));
                        c = char_pool[i]->at(value);
                        additional -= 1;
                        break;
                    }
                }
            }
        }

            return std::string(password.begin(), password.end());
    }

#ifdef _WIN32
    const std::vector<uint32_t> RPG::trueRandom(size_t size, uint32_t max_val)
    {
        // the bytes needed to compute one character in the password
        uint8_t char_size = ceil(static_cast<float>(max_val) / static_cast<float>(UCHAR_MAX));

        std::vector<UCHAR> buffer(size * char_size);
        NTSTATUS status = BCryptGenRandom(
            nullptr, // the default algorithm
            buffer.data(),
            static_cast<ULONG>(buffer.size()),
            BCRYPT_USE_SYSTEM_PREFERRED_RNG // automatically choose entropy source
        );

        if (status < 0)
        {
            throw std::runtime_error("BCryptGenRandom failed. Code: 0x" + std::to_string(status));
        }

        std::vector<uint32_t> merged_rand_dis(size, 0); // merge mutiple bytes into one number
        size_t dis_ptr = 0u;
        for (auto &num : merged_rand_dis)
        {
            uint32_t merged_max = 0u;
            for (uint8_t i = 0u; i < char_size; i++)
            {
                num += buffer[dis_ptr] << (i * 8);
                dis_ptr++;
                merged_max += 0xff << (i * 8);
            }
            num = std::min(max_val, static_cast<uint32_t>(static_cast<float>(num) / static_cast<float>(merged_max) * static_cast<float>(max_val)));
        }

        return merged_rand_dis;
    }
#endif
} // namespace TP
