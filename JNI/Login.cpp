//
// Created by TOM on 2017/8/5.
//

#include <OBOjni.h>
#include "cJSON.h"
#include "curl/curl.h"
#define RESPONSE_DATA_LEN 4096

//用来接受服务器一个buffer
typedef struct login_response_data
{
    login_response_data()
    {
        memset(data, 0, RESPONSE_DATA_LEN);
        data_len = 0;
    }
    char data[RESPONSE_DATA_LEN];
    int data_len;

}response_data_t;

//处理从服务器返回的数据，将数据拷贝到arg中
size_t deal_response(void *ptr, size_t n, size_t m, void *arg)
{
    int count = m*n;
    response_data_t *response_data = (response_data_t *)arg;
    memcpy(response_data->data, ptr, count);
    response_data->data_len = count;

    return response_data->data_len;
}


extern "C"{

/*
 * Class:     com_example_tom_obo_OBOJNI
 * Method:    login
 * Signature: (Ljava/lang/String;Ljava/lang/String;Z)Z
 */
JNIEXPORT jboolean JNICALL Java_com_example_tom_obo_OBOJNI_login
        (JNIEnv *env, jobject obj, jstring j_username, jstring j_password, jboolean j_isdriver)
{
    const char *username = env->GetStringUTFChars(j_username, 0);
    const char *password = env->GetStringUTFChars(j_password, 0);
    const char *isdriver = j_isdriver == JNI_TRUE ? "yes" : "no";

    __android_log_print(ANDROID_LOG_ERROR,TAG, "JNI-login:username = %s, password = %s, isdirver = %s",
    username, password, isdriver);



    //封装一个数据协议
    /*

       ====给服务端的协议====
     https://ip:port/login [json_data]
    {
        username: "gailun",
        password: "123123",
        driver:   "yes"
    }
     *
     *
     * */

    char *post_str = NULL;
    CURLcode res;
    response_data_t response_data;//专门存放从服务器返回的数据


    //（1）封装一个json字符串
    cJSON *root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "username", username);
    cJSON_AddStringToObject(root, "password", password);
    cJSON_AddStringToObject(root, "driver", isdriver);

    post_str = cJSON_Print(root);

    //释放root对象
    cJSON_Delete(root);
    root = NULL;//释放对象之后把指针置空


    __android_log_print(ANDROID_LOG_ERROR, TAG,"JNI-login: post_str = [%s]\n", post_str);

    //初始化curl句柄
    CURL* curl = curl_easy_init();

    //(2) 想web服务器 发送http请求 其中post数据 json字符串
    //1.设置curl curl
    curl_easy_setopt(curl, CURLOPT_URL, "https://47.93.193.214:7777/login");

    //客户端忽略CA证书认证
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

    //2. 开启post请求开关
    curl_easy_setopt(curl, CURLOPT_POST, true);
    //3. 添加post数据
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_str);
    //4.设定一个处理服务器相应的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, deal_response);
    //5.给回掉函数传递一个形参
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    //6.向服务器发送请求, 等待服务器的响应
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "JNI-login:perform ERROR, rescode=[%d]\n",
                            res);
        return JNI_FALSE;
    }

    //（3） 等待服务器的响应


    /*

      //成功
    {
        result: "ok",
    }
    //失败
    {
        result: "error",
        reason: "why...."
    }

     *
     * */

    //（4）解析服务器返回的json字符串
    root = cJSON_Parse(response_data.data);
    if(root == NULL)
    {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "JNI-login:cJSON_Parse ERROR");

    }


    cJSON *result = cJSON_GetObjectItem(root, "result");
    if(result && strcmp(result->valuestring, "ok") == 0)
    {
        //登陆成功
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "JNI-login:login succ!!!");
        return JNI_TRUE;
    }
    else
    {
        //登录失败
        cJSON* reason = cJSON_GetObjectItem(root, "reason");
        if(reason)
        {
            //已知错误
            __android_log_print(ANDROID_LOG_ERROR, "JNI", "JNI-login:login error, reason = %s!!!", reason->valuestring);

        }
        else
        {
            //未知错误
            __android_log_print(ANDROID_LOG_ERROR, "JNI", "JNI-login:login error, reason = Unknown");
        }

        return JNI_FALSE;

    }


    //如果"result" 字段==OK，登陆成功，error登陆失败
    return JNI_TRUE;


}

}