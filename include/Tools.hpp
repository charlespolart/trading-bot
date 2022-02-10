#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <chrono>

#include <binapi/double_type.hpp>

#include "Params.h"

namespace Tools
{

    class Date
    {
    public:
        static tm *getLocaltime(size_t timestampMs)
        {
            time_t rawtime = static_cast<time_t>(timestampMs / 1000);
            return (std::gmtime(&rawtime));
        }
        static std::string getDate(size_t timestamp = 0, bool dispMs = true)
        {
            std::string result;
            if (!timestamp)
            {
                timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            }
            tm *tm = getLocaltime(timestamp);
            std::string day = (tm->tm_mday < 10 ? "0" : "") + std::to_string(tm->tm_mday);
            std::string month = (tm->tm_mon + 1 < 10 ? "0" : "") + std::to_string(tm->tm_mon + 1);
            std::string year = std::to_string(tm->tm_year + 1900);
            std::string hour = (tm->tm_hour < 10 ? "0" : "") + std::to_string(tm->tm_hour);
            std::string min = (tm->tm_min < 10 ? "0" : "") + std::to_string(tm->tm_min);
            std::string sec = (tm->tm_sec < 10 ? "0" : "") + std::to_string(tm->tm_sec);
            result = day + "/" + month + "/" + year + " " + hour + ":" + min + ":" + sec;
            if (dispMs)
            {
                std::string ms = std::to_string(timestamp).substr(10, 12);
                result += "." + ms;
            }
            return (result);
        }
        static int diffHour(size_t timestamp1, size_t timestamp2)
        {
            return (std::abs(static_cast<int>(timestamp1 - timestamp2)) / 3600000);
        }
        static int diffSec(size_t timestamp1, size_t timestamp2)
        {
            return (std::abs(static_cast<int>(timestamp1 - timestamp2)) / 1000);
        }
        static int diffMs(size_t timestamp1, size_t timestamp2)
        {
            return (std::abs(static_cast<int>(timestamp1 - timestamp2)));
        }
    };

    class Convert
    {
    public:
        static std::string to_string(binapi::double_type n, int precision)
        {
            return (to_string(static_cast<double>(n), precision));
        }
        static std::string to_string(double n, int precision)
        {
            int trunc = std::pow(10, precision);
            std::stringstream stream;
            stream << std::fixed << std::setprecision(precision) << std::floor(n * trunc) / trunc;
            return (stream.str());
        }
        static int getPrecision(binapi::double_type n)
        {
            return (getPrecision(static_cast<double>(n)));
        }
        static int getPrecision(double n)
        {
            return (std::floor(n) == n ? 0 : n == 1.0 ? 0
                                                      : to_string(n, 8).find("1") - to_string(n, 8).find("."));
        }
    };

    class Log
    {
    public:
        static void writeToFile(const std::string &fileName, const std::string &str)
        {
            std::ofstream file;
            std::string logPath = DEFAULT_LOG_FOLDER_PATH;
            if (std::getenv("LOG_FOLDER_TRADING_BOT"))
                logPath = std::getenv("LOG_FOLDER_TRADING_BOT");
            file.open(logPath + "/" + fileName + ".txt", std::ios::app);
            file << Tools::Date::getDate(0, false) << " " << str + "\n";
            file.close();
        }
    };
}

#endif // TOOLS_HPP
