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