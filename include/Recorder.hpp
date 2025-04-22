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

#include <pch.hpp>
#include <Data.hpp>

namespace TP
{
    /**
     * @brief create a keychain interatively and save it
     * 
     * @param in input stream
     * @param out output stream
     */
    void interactiveCreate(std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData>&& data);
} // namespace TP
 