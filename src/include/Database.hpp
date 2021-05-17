#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <iostream>
#include <pqxx/pqxx>

class Database
{
public:
    Database() = delete;
    Database(const std::string &name);
    ~Database();

public:
    pqxx::connection &getConnection();

private:
    pqxx::connection _connection;
};

#endif // DATABASE_HPP
