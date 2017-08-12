//
// Created by TOM on 2017/8/8.
//

#ifndef OBO_CURL_H
#define OBO_CURL_H

#include "OBOjni.h"
#include <curl/curl.h>

class Curl {

public:
    Curl(string url, bool ignoreCA);
    ~Curl();

    static size_t deal_response(void *ptr, size_t m, size_t n, void *arg);

    //想远程服务器发出请求
    bool execute(string requestData);

    //提供_responseData的getter方法
    string responseData();

private:
    CURL *_curl;
    string _responseData;//用来存储从服务器返回的数据
};


#endif //OBO_CURL_H
