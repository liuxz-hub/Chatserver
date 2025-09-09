#include "friendmodel.hpp"
#include "user.hpp"
#include "db.h"
#include <vector>

using namespace std;

// 添加好友关系
void FriendModel::insert(int userid, int friendid)
{
    // 组装sql语句
    char sql[1024] = {0};
    // sprintf(目标缓冲区, 格式字符串, 参数1, 参数2);
    sprintf(sql, "insert into friend value(%d, %d)", userid, friendid); 

    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
    }
}


// 返回当前用户的好友列表(USER和Friend表的联合查询，借助friendid去USER中找到该用户的name、id和state)
vector<User> FriendModel::query(int userid)
{
    // 组装sql语句
    char sql[1024] = {0};
    // 使用内连接（Inner Join）来查询用户的好友信息
    // 关键：inner join friend b on b.friendid = a.id，将friend表的friendid字段与user表的id字段进行关联
    sprintf(sql, "select a.id,a.name,a.state from user a inner join friend b on b.friendid = a.id where b.userid = %d", userid); 

    vector<User> vec;
    MySQL mysql;
    if(mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr)
        {
            // 把userid用户的宿友离线消息放入vec中返回
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)  // 申请资源
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
        }
        mysql_free_result(res);  // 释放资源
        return vec;
    }
    return vec;
}