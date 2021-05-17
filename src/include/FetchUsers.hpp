#ifndef FETCHUSERS_HPP
#define FETCHUSERS_HPP

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <rapidjson/document.h>

#include "User.hpp"

class FetchUsers
{
public:
    static int fetch(boost::asio::io_context &ioctx, std::vector<User *> &users)
    {
        rapidjson::Document document;

        if (documentParse(document) == EXIT_FAILURE)
            return (EXIT_FAILURE);
        if (createUsers(ioctx, document, users) == EXIT_FAILURE)
            return (EXIT_FAILURE);
        for (size_t i = 0; i < users.size(); ++i)
            std::cout << users[i]->getName() << " : Ready!" << std::endl;
        return (EXIT_SUCCESS);
    }

private:
    static int documentParse(rapidjson::Document &document)
    {
        const char *path = nullptr;
        FILE *fd = nullptr;
        size_t size = 0;
        char *file = nullptr;

        if (!(path = std::ifstream("/etc/trading_bot/person.json") ? "/etc/trading_bot/person.json" : std::getenv("PERSON_FILE_TRADING_BOT")))
        {
            std::cerr << "Person file not found" << std::endl;
            return (EXIT_FAILURE);
        }
        if (!(fd = std::fopen(path, "r")))
        {
            std::cerr << "Failed to open " << path << std::endl;
            return (EXIT_FAILURE);
        }
        std::fseek(fd, 0L, SEEK_END);
        size = std::ftell(fd);
        std::fseek(fd, 0L, SEEK_SET);
        file = new char[size + 1];
        std::fread(file, sizeof(char), size, fd);
        file[size] = '\0';
        document.Parse(file);
        delete[] file;
        std::fclose(fd);
        return (EXIT_SUCCESS);
    }

    static int createUsers(boost::asio::io_context &ioctx, const rapidjson::Document &document, std::vector<User *> &users)
    {
        if (!document.IsObject())
        {
            std::cerr << "JSON invalid: Root must be an object" << std::endl;
            return (EXIT_FAILURE);
        }
        if (!document.HasMember("person") || !document["person"].IsArray())
        {
            std::cerr << "JSON invalid: person not found" << std::endl;
            return (EXIT_FAILURE);
        }
        for (rapidjson::SizeType i = 0; i < document["person"].Size(); ++i)
        {
            std::string name;
            std::string apiKey;
            std::string apiSecret;
            std::string btcAmount;
            bool enable;

            if (!document["person"][i].HasMember("name") || !document["person"][i]["name"].IsString())
            {
                std::cerr << "JSON invalid: name not found" << std::endl;
                return (EXIT_FAILURE);
            }
            name = document["person"][i]["name"].GetString();

            if (!document["person"][i].HasMember("apiKey") || !document["person"][i]["apiKey"].IsString())
            {
                std::cerr << "JSON invalid: apiKey not found" << std::endl;
                return (EXIT_FAILURE);
            }
            apiKey = document["person"][i]["apiKey"].GetString();

            if (!document["person"][i].HasMember("apiSecret") || !document["person"][i]["apiSecret"].IsString())
            {
                std::cerr << "JSON invalid: apiSecret not found" << std::endl;
                return (EXIT_FAILURE);
            }
            apiSecret = document["person"][i]["apiSecret"].GetString();

            if (!document["person"][i].HasMember("btcAmount") || !document["person"][i]["btcAmount"].IsString())
            {
                std::cerr << "JSON invalid: btcAmount not found" << std::endl;
                return (EXIT_FAILURE);
            }
            btcAmount = document["person"][i]["btcAmount"].GetString();

            if (!document["person"][i].HasMember("enable") || !document["person"][i]["enable"].IsBool())
            {
                std::cerr << "JSON invalid: enable not found" << std::endl;
                return (EXIT_FAILURE);
            }
            enable = document["person"][i]["enable"].GetBool();

            if (enable)
                users.emplace_back(new User(ioctx, name, apiKey, apiSecret, btcAmount));
            else
                std::cout << name << " : Disabled :(" << std::endl;
        }
        return (EXIT_SUCCESS);
    }
};

#endif // FETCHUSERS_HPP