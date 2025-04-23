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
     * @brief create a keychain interatively and save it
     * 
     * @param in input stream
     * @param out output stream
     */
    void interactiveCreate(std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData>&& data);
} // namespace TP
 