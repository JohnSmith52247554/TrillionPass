/**
 * @file Data.hpp
 * @author username (username52247554@gmail.com)
 * @brief classed to operate password data file
 * @date 2025-04-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <Structure.hpp>

namespace TP
{
    namespace Data
    {
        /**
         * @brief abstract base class for simple password database
         *
         */
        class PasswordData
        {
        public:
            virtual ~PasswordData() {}

            /**
             * @brief check if a certain keychain exists in the file
             *
             * @param name the name of the key chain
             * @return const int the offset of the keychain, -1 for not exist
             */
            virtual const int exists(std::string name) = 0;

            /**
             * @brief find a certain keychain according to its name
             *
             * @param offset the offset of the keychain
             * @return KeyChain the found keychain
             * @note if unexist, the name of the returned keychain will be empty
             */
            virtual TP::KeyChain find(const int offset) = 0;

            /**
             * @brief find a certain keychain according to its name
             *
             * @param name the name of the keychain
             * @return KeyChain the found keychain
             * @note if unexist, the name of the returned keychain will be empty
             */
            virtual TP::KeyChain find(std::string name) = 0;

            /**
             * @brief overwrite a keychain in the file
             *
             * @param offset the offset of the keychain to be overwritten
             * @param key the keychain will be written
             * @return const bool will be true if success
             */
            virtual const bool overwrite(int offset, TP::KeyChain key) = 0;

            /**
             * @brief add a keychain to the end of the file
             *
             * @param key the keychain to be written
             */
            virtual void add(TP::KeyChain key) = 0;

            /**
             * @brief erase one keychain in the file according to its offset
             *
             * @param offset the offset of the keychain to be erase
             * @return whether the keychain has been erased successfully
             */
            virtual const bool erase(const int offset) = 0;

            /**
             * @brief erase one keychain in the file according to its name
             *
             * @param name the name of the keychain to be erase
             * @return whether the keychain has been erased successfully
             */
            virtual const bool erase(const std::string name) = 0;

            /**
             * @brief flush the buffer
             *
             */
            virtual void flush() = 0;

            /**
             * @brief return the size of data (per keychain)
             * 
             * @return size_t 
             */
            virtual inline size_t size() const = 0;
        };

        /**
         * @brief the class to operate json data
         *
         */
        class JsonPData : public PasswordData
        {
        private:
            std::string file_path;
            nlohmann::json json;

        public:
            JsonPData(const std::string filepath);
            ~JsonPData();

            virtual const int exists(std::string name) override;
            virtual TP::KeyChain find(std::string name) override;
            virtual TP::KeyChain find(const int offset) override;
            virtual const bool overwrite(int offset, TP::KeyChain key) override;
            virtual void add(TP::KeyChain key) override;
            virtual const bool erase(const int offset) override;
            virtual const bool erase(const std::string name) override;
            virtual void flush() override;
            virtual inline size_t size() const override;
        };

        class BinaryPData : public PasswordData
        {
        private:
            std::string file_path;
            std::vector<TP::KeyChain> data;

        public:
            BinaryPData(const std::string filepath);
            ~BinaryPData();

            virtual const int exists(std::string name) override;
            virtual TP::KeyChain find(std::string name) override;
            virtual TP::KeyChain find(const int offset) override;
            virtual const bool overwrite(int offset, TP::KeyChain key) override;
            virtual void add(TP::KeyChain key) override;
            virtual const bool erase(const int offset) override;
            virtual const bool erase(const std::string name) override;
            virtual void flush() override;
            virtual inline size_t size() const override;
        };

        void serializeStr(std::ofstream &file, std::string &str);
        void deserializeStr(std::ifstream &file, std::string &str);
    }
}