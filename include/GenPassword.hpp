/**
 * @file GenPassword.hpp
 * @author username (username52247554@gmail.com)
 * @brief generate password
 * @date 2025-04-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

namespace TP::CharPool
{
    const std::vector<char> lower_char = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
        'u', 'v', 'w', 'x', 'y', 'z'};
    const std::vector<char> upper_char = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z'};
    const std::vector<char> numbers = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    const std::vector<char> special_char = {
        '~', '!', '@', '#', '$', '%', '^', '*', '(', ')',
        '-', '_', '+', '=', '[', ']', '{', '}', '?', '/'};
}

namespace TP
{
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
    std::string generate(unsigned char length, bool upper = false, bool number = false, bool special_char = false);
} // namespace TP