#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

// User 表的 ORM 类：将数据库表(字段)映射为C++类
class User
{
public:
    // 构造函数
    User(int id = -1, string name="", string pwd="", string state="offline")
    {
        this->id = id;
        this->name = name;
        this->password = pwd;
        this->state = state;
    }

    // 一些操作方法
    void setId(int id) {this->id = id;}
    void setName(string name) {this->name = name;}
    void setPwd(string pwd) {this->password = pwd;}
    void setState(string state) {this->state = state;}

    int getId() {return this->id;}
    string getName() {return this->name ;}
    string getPwd() {return this->password;}
    string getState() {return this->state;}

private:
    int id;
    string name;
    string password;
    string state;
};


#endif