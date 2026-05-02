/********************************************************************************
* @Author : hexne
* @Date   : 2026/05/02 13:21:20
********************************************************************************/
module;
#include <mysql/mysql.h>
export module database;
import std;

class MySQL {
public:
    MySQL(const char* host, const char* user, const char* pass,
          const char* db, unsigned port = 3306)
    {
        conn = mysql_init(nullptr);
        if (!conn)
            throw std::runtime_error("mysql_init failed");

        if (!mysql_real_connect(conn, host, user, pass, db, port, nullptr, 0))
            throw std::runtime_error(mysql_error(conn));
    }

    ~MySQL() {
        if (conn)
            mysql_close(conn);
    }

    MYSQL* get() { return conn; }

private:
    MYSQL* conn{};
};

