#ifndef FETCHPAIRS_HPP
#define FETCHPAIRS_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <rapidjson/document.h>

#include "Params.h"

class FetchPairs
{
public:
    static int fetch(std::vector<std::string> &pairs)
    {
        rapidjson::Document document;

        if (documentParse(document) == EXIT_FAILURE)
            return (EXIT_FAILURE);
        if (getPairs(document, pairs))
            return (EXIT_FAILURE);
        return (EXIT_SUCCESS);
    }

private:
    static int documentParse(rapidjson::Document &document)
    {
        const char *path = std::getenv("PAIR_FILE_TRADING_BOT");
        FILE *fd = nullptr;
        size_t size = 0;
        char *file = nullptr;

        if (!path)
        {
            path = DEFAULT_PAIR_FILE_PATH;
            if (!std::ifstream(path))
            {
                std::cerr << DEFAULT_PAIR_FILE_PATH << " file not found" << std::endl;
                return (EXIT_FAILURE);
            }
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

    static int getPairs(const rapidjson::Document &document, std::vector<std::string> &pairs)
    {
        if (!document.IsObject())
        {
            std::cerr << "JSON invalid: Root must be an object" << std::endl;
            return (EXIT_FAILURE);
        }
        if (!document.HasMember("pair") || !document["pair"].IsArray())
        {
            std::cerr << "JSON invalid: pair not found" << std::endl;
            return (EXIT_FAILURE);
        }
        for (rapidjson::SizeType i = 0; i < document["pair"].Size(); ++i)
        {
            std::string pair;
            bool enable;

            if (!document["pair"][i].HasMember("pair") || !document["pair"][i]["pair"].IsString())
            {
                std::cerr << "JSON invalid: pair not found" << std::endl;
                return (EXIT_FAILURE);
            }
            pair = document["pair"][i]["pair"].GetString();

            if (!document["pair"][i].HasMember("enable") || !document["pair"][i]["enable"].IsBool())
            {
                std::cerr << "JSON invalid: enable not found" << std::endl;
                return (EXIT_FAILURE);
            }
            enable = document["pair"][i]["enable"].GetBool();

            if (enable)
                pairs.push_back(pair);
            else
                std::cout << pair << " : Disabled :(" << std::endl;
        }
        return (EXIT_SUCCESS);
    }
};

#endif // FETCHPAIRS_HPP
