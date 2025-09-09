#ifndef DB_H
#define DB_H
#include <mysql/mysql.h>
#include <string>
using namespace std;

// 数据库操作类
class MySQL
{ 
public:
    // 1、初始化数据库连接(创建一块内存)
    MySQL();
    // 2、释放数据库连接资源
    ~MySQL();
    // 3、连接数据库
    bool connect();
    // 4、更新操作
    bool update(string sql);
    // 5、查询操作
    MYSQL_RES* query(string sql);
    // 6、获取连接
    MYSQL* getConnection();

private:
    MYSQL *_conn;
};

#endif