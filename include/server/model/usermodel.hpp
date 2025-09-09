#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.hpp"

// User表的数据操作类：提供方法
class UserModel
{
public:
    // User 表的增加
    bool insert(User &user);
    // 更新用户的状态信息
    bool updateState(User user);
    // 通过用户 ID 获取用户信息
    User query(int id);
    // 重置用户的状态信息
    void resetState();
private:

};
#endif