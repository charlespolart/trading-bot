#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>

#include <binapi/double_type.hpp>

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
        static std::string getDate(size_t timestamp)
        {
            tm *tm = getLocaltime(timestamp);
            std::string day = (tm->tm_mday < 10 ? "0" : "") + std::to_string(tm->tm_mday);
            std::string month = (tm->tm_mon + 1 < 10 ? "0" : "") + std::to_string(tm->tm_mon + 1);
            std::string year = std::to_string(tm->tm_year + 1900);
            std::string hour = (tm->tm_hour < 10 ? "0" : "") + std::to_string(tm->tm_hour);
            std::string min = (tm->tm_min < 10 ? "0" : "") + std::to_string(tm->tm_min);
            std::string sec = (tm->tm_sec < 10 ? "0" : "") + std::to_string(tm->tm_sec);
            std::string ms = std::to_string(timestamp).substr(10, 12);
            return (day + "/" + month + "/" + year + " " + hour + ":" + min + ":" + sec + "." + ms);
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
        static int getPrecision(double n)
        {
            return (std::floor(n) == n ? 0 : n == 1.0 ? 0
                                                      : to_string(n, 8).find("1") - to_string(n, 8).find("."));
        }
    };
}

#endif // TOOLS_HPP
