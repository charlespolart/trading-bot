#include "Database.hpp"

Database::Database(const std::string &name) : _connection("dbname=" + name + " user=" + std::getenv("DATABASE_USER") + " password=" + std::getenv("DATABASE_PASSWORD") + " hostaddr=127.0.0.1")
{
}

Database::~Database()
{
}

/* PUBLIC */
pqxx::connection &Database::getConnection()
{
    return (this->_connection);
}
