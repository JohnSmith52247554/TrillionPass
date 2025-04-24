/**
 * @file Finder.hpp
 * @author username (username52247554@gmail.com)
 * @brief find keychain in the file
 * @date 2025-04-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <Structure.hpp>
#include <Data.hpp>
#include <Recorder.hpp>

namespace TP
{
    /**
     * @brief Find certain keychain in the file.
     * 
     * @param name The name of keychain. If name is blank (""), the fuction will ask the user to enter a valid name.
     * @param in input stream
     * @param out output stream
     * @param data the data to search
     */
    void find(std::string name, std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data);

    void deleteKeyChain(std::string name, std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data);

    /**
     * @brief List the name and brief of all of the keychains in a file.
     * 
     * @param in input stream
     * @param out output stream
     * @param data the data to list
     */
    void listAllKeyChains(std::istream &in, std::ostream &out, std::unique_ptr<TP::Data::PasswordData> &&data);
}