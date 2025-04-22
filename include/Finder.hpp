/**
 * @file Finder.hpp
 * @author username (username52247554@gmail.com)
 * @brief find keychain in the file
 * @date 2025-04-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <pch.hpp>
#include <Structure.hpp>
#include <Data.hpp>
#include <Recorder.hpp>

namespace TP
{
    void find(std::string name, std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data);
}