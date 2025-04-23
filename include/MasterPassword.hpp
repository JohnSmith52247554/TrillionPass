/**
 * @file MasterPassword.hpp
 * @author username (username52247554@gmail.com)
 * @brief manage master password
 * @date 2025-04-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

namespace TP
{
    /**
     * @brief ask the user to enter the MASTER PASSWORD and check whether the enter string matches the stored master password
     *
     * @param in input stream
     * @param out output stream
     * @return true matches
     * @return false not matches
     */
    const bool checkMasterPassword(std::istream &in, std::ostream &out, std::string &master_password);

    /**
     * @brief ask the user to login through master password
     *
     * @param in input stream
     * @param out output stream
     * @return true matches
     * @return false not matches
     */
    const bool login(std::istream &in, std::ostream &out, std::string& master_password);
    
    /**
     * @brief hashing the raw_input and check whether it is equal to the hash of master password
     * 
     * @param raw_input the raw input string of user
     */
    const bool checkMasterPasswordHash(std::string raw_input);

    void changeMasterPassword(std::istream &in, std::ostream &out);

} // namespace TP

namespace TEMP
{
    void setMasterPassword(std::string password);

    const bool chararcherCheck(const std::string& password);
    const bool consecutiveCheck(const std::string& password, const unsigned char max_repeat_time);

    void securetInput(std::istream&in, std::ostream& out, std::string &input);
}