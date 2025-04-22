/**
 * @file Recorder.hpp
 * @author username (username52247554@gmail.com)
 * @brief record the generated password
 * @date 2025-04-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <Data.hpp>

namespace TP
{
    // TODO: support other systems
    /**
     * @brief copy a string into the system clipboard
     *
     * @param text the string to be copied
     */
    void copyToClipboard(const std::string &text);

    /**
     * @brief ask the user to enter the MASTER PASSWORD and check whether the enter string matches the stored master password
     *
     * @param in input stream
     * @param out output stream
     * @return true matches
     * @return false not matches
     */
    const bool checkMasterPassword(std::istream &in, std::ostream &out);

    /**
     * @brief ask the user to login through master password
     *
     * @param in input stream
     * @param out output stream
     * @return true matches
     * @return false not matches
     */
    const bool login(std::istream &in, std::ostream &out);

    /**
     * @brief create a keychain interatively and save it
     * 
     * @param in input stream
     * @param out output stream
     */
    void interactiveCreate(std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData>&& data);
} // namespace TP
 