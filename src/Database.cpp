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

void Database::writeTransaction(const std::string &type, const binapi::ws::kline_t &kline)
{
    pqxx::work work(this->_connection);
    work.exec("INSERT INTO transactions VALUES ('" + kline.s + "', '" + kline.i + "', '" + std::to_string(kline.t) + "', '" + std::to_string(kline.T) + "', '" + Tools::Convert::to_string(kline.o, 8) + "', '" + Tools::Convert::to_string(kline.c, 8) + "', '" + Tools::Convert::to_string(kline.l, 8) + "', '" + Tools::Convert::to_string(kline.h, 8) + "', '" + Tools::Convert::to_string(kline.v, 8) + "', '" + Tools::Convert::to_string(kline.q, 8) + "', '" + Tools::Convert::to_string(kline.V, 8) + "', '" + Tools::Convert::to_string(kline.Q, 8) + "', '" + std::to_string(kline.n) + "', '" + type + "', '" + Tools::Convert::to_string(0.0, 8) + "',  '" + Tools::Convert::to_string(0.0, 8) + "')");
    work.commit();
}
