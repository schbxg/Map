//
// Created by TOM on 2017/8/8.
//

//使用面向对象的思想，调用json接口
//封装json接口

#ifndef OBO_JSON_H
#define OBO_JSON_H

#include "OBOjni.h"
#include <cJSON.h>



class Json {
public:
    Json();
    ~Json();

    //给json插入一对key value(string)，用STL容器string
    void insert(string key, string value);

    //将json Object 转换成字符串
    string print();

    //将json字符串解析成一个json对象
    void parse(string json_str);

    //得到json中的某个key对应的value
    string value(string key);


private:
    //防止json对象进行浅拷贝
    Json(const Json &);
    Json &operator=(const Json &);

    cJSON *_root;


};


#endif //OBO_JSON_H
