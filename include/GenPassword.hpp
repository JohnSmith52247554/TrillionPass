/**
 * @file GenPassword.hpp
 * @author username (username52247554@gmail.com)
 * @brief generate password
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <pch.hpp>

namespace TP
{
    /**
     * @brief The class to generate random password. Using a class only to maintain visibility.
     *
     */
    class RandomPasswordGenerator
    {
    private:
        static const std::vector<char> lower_char;
        static const std::vector<char> upper_char;
        static const std::vector<char> numbers;
        static const std::vector<char> special_char;

        RandomPasswordGenerator() {}
        ~RandomPasswordGenerator() {}

    public:
        /**
         * @brief the fuction to generate random password
         *
         * @param length the length of the password to be generate
         * @param upper If this parameter is true, there will be at least one upper letter. Otherwise the password will only contain lower letter.
         * @param number If this parameter is true, there will be at least one number. Otherwise the password will not contain number.
         * @param special_char If this parameter is true, there will be at least one special character. Otherwise the password will not contain any spacial character.
         * @note special charater include !@#$%^*()-_=+[]{}?~
         * @return the password
         */
        static std::string generate(unsigned char length, bool upper = false, bool number = false, bool special_char = false);

    private:
#ifdef _WIN32
        /**
         * @brief generate a true random distribution
         *
         * @param size the size of distribution
         * @return the true random distribution
         */
        static const std::vector<uint32_t> trueRandom(size_t size, uint32_t max);
#endif
    };
} // namespace TP