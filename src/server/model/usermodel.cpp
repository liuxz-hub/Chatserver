#include "usermodel.hpp"
#include "db.h"
#include <iostream>
using namespace std;

// User 表的增加方法
bool UserModel::insert(User &user)
{
    // 组装sql语句
    char sql[1024] = {0};
    // sprintf(目标缓冲区, 格式字符串, 参数1, 参数2, 参数3);
    sprintf(sql, "insert into user(name, password, state) value('%s', '%s', '%s')", user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str()); 

    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            // 获取插入成功的用户数据生成的 id
            // mysql_insert_id() 用于获取最后插入行的自动增长id
            user.setId(mysql_insert_id(mysql.getConnection())); 
            return true;
        }
    }
    return false;
}

// 通过用户 ID 获取用户信息
User UserModel::query(int id)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from user where id = %d", id); 

    MySQL mysql;
    if(mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr)
        {
            // 获取用户user信息
            MYSQL_ROW row = mysql_fetch_row(res);  // 申请资源
            if(row != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                mysql_free_result(res);  // 不能忘记释放资源
                return user;
            }
            mysql_free_result(res);  // 修复：即使row为nullptr也要释放资源
        }
    }
    return User();
}

// 更新用户的状态信息
bool UserModel::updateState(User user)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d", user.getState().c_str(), user.getId()); 

    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}

// 重置用户的状态信息
void UserModel::resetState()
{
    char sql[1024] = "update user set state = 'offline' where state = 'online'";
    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
    }
}
