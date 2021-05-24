#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <iostream>
#include <pqxx/pqxx>

#include <binapi/api.hpp>

#include "Tools.hpp"

class Database
{
public:
    Database() = delete;
    Database(const std::string &name);
    ~Database();

public:
    pqxx::connection &getConnection();
    void writeTransaction(const std::string &type, const binapi::ws::kline_t &kline);

private:
    pqxx::connection _connection;
};

#endif // DATABASE_HPP
