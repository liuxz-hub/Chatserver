#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

// json序列化示例1
string func1()
{
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhang san";
    js["to"] = "li si";
    js["msg"] = "hello, what are you doing now?";

    string sendBuf = js.dump();
    //cout << sendBuf.c_str() << endl;

    return sendBuf;
}

// json序列化示例2
string func2()
{
    json js;
    // 添加数组
    js["id"] = {1, 2, 3, 4, 5};
    // 添加key-value
    js["name"] = "zhang san";
    // 添加对象
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["liu shuo"] = "hello china";
    // 上面等同于下面这句一次性添加数组对象
    js["msg"] = {{"zhang san", "hello world"}, {"liu shuo", "hello china"}};
    //cout << js << endl;

    return js.dump();
}

// json序列化示例代码3
string func3()
{
    json js;
    // 直接序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;

    // 直接序列化一个map容器
    map<int, string> m;
    m.insert({1,"黄山"});
    m.insert({2,"华山"});
    m.insert({3,"泰山"});
    js["path"] = m;

    string sendBuf = js.dump(); //json数据对象 -> 序列化 -> json字符串
    //cout << sendBuf << endl;
    return sendBuf;
}

int main(){
    string recvBuf = func3();
    // 反序列化：json字符串 -> 反序列化 -> 数据对象（看作容器，方便访问）
    json jsonbuf = json::parse(recvBuf);

    cout << jsonbuf["from"] << endl;
    cout << jsonbuf["msg"] << endl;
    cout << jsonbuf["to"] << endl;
    cout << jsonbuf["msg_type"] << endl;

    //cout << jsonbuf["id"] << endl;
    //auto arr = jsonbuf["id"];
    //cout << arr[1] << endl;
    //cout << jsonbuf["msg"] << endl;
    //auto msgj = jsonbuf["msg"];
    //cout << msgj["zhang san"] << endl;
    //cout << msgj["liu shuo"] << endl;

    //vector<int> vec = jsonbuf["list"];
    //for(int &v : vec){
    //    cout << v << " ";
    //}
    //cout << endl;
    //map<int,string> mymap = jsonbuf["path"];
    //for(auto &m : mymap){
    //    cout << m.first << " " << m.second << endl;
    //}

    return 0;
}