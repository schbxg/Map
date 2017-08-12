//
// Created by TOM on 2017/8/8.
//
#include "OBOjni.h"
#include "Json.h"
#include "Curl.h"

extern "C"
{

/*
 * Class:     com_example_tom_obo_OBOJNI
 * Method:    reg
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)Z
 */
JNIEXPORT jboolean JNICALL Java_com_example_tom_obo_OBOJNI_reg
        (JNIEnv *env, jobject obj, jstring j_username, jstring j_passwd, jstring j_email,
         jstring j_phone, jstring j_idCard, jboolean j_isDriver)
{
    //将java中的字符串转化成jni字符串
    const char *username = env->GetStringUTFChars(j_username, NULL);
    const char *password = env->GetStringUTFChars(j_passwd, NULL);
    const char *email = env->GetStringUTFChars(j_email, NULL);
    const char *phone = env->GetStringUTFChars(j_phone, NULL);
    const char *idCard = env->GetStringUTFChars(j_idCard, NULL);
    const char *isDirver = j_isDriver==JNI_TRUE ? "yes" : "no";

    /*

   ==== 给服务端的协议 ====
   https://ip:port/reg [json_data]
   {
       username: "gailun",
       password: "123123",
       driver:   "yes/no",
       tel:      "13331333333",
       email:    "danbing_at@163.cn",
       id_card:  "2104041222121211122"
   }
    *
    */

    //用面向对象的思想封装成为一个json类，使用json对象操作
    // （之前用的是面向过程的思想，直接调用json接口）
    //将用户数据封装成一个json字符串
    Json json;

    json.insert("username", username);
    json.insert("password", password);
    json.insert("driver", isDirver);
    json.insert("tel", phone);
    json.insert("email",email);
    json.insert("id_card",idCard);

    string json_str = json.print();

    __android_log_print(ANDROID_LOG_ERROR,TAG,"JNI-reg: post_str = [%s]\n", json_str.c_str());



    //释放资源
    env->ReleaseStringUTFChars(j_username, username);
    env->ReleaseStringUTFChars(j_passwd, password);
    env->ReleaseStringUTFChars(j_email, email);
    env->ReleaseStringUTFChars(j_phone, phone);
    env->ReleaseStringUTFChars(j_idCard, idCard);

    //向远程服务器发送请求
    Curl curl("https://47.93.193.214:7777/reg", true);

    if(curl.execute(json_str) == false)
    {
        __android_log_print(ANDROID_LOG_ERROR,TAG,"JNI-reg:curl.excute error\n");
        return JNI_FALSE;
    }

    //服务器会回复一个协议

    /*

     ====得到服务器响应数据 ====
     注册成功就默认为登陆状态

       //成功
       {
            result: "ok"
        }
        //失败
        {
            result: "error",
            reason: "why...."
        }
     *
     *
     */
    //从服务器得到的数据 在curl._responseData

    string responserData = curl.responseData();

    Json json_res;
    json_res.parse(responserData);

    string result = json_res.value("result");
    if(result == "ok")
    {
        //注册成功
        __android_log_print(ANDROID_LOG_ERROR,TAG, "JNI-reg:reg succ!!!\n");
        return true;
    }
    else
    {
        //注册失败
        string reason = json_res.value("reason");
        if(reason == "") {
            //未知错误
            __android_log_print(ANDROID_LOG_ERROR,TAG,"JNI-reg:reg fali!! reason unknow\n");

        }
        else {
            //已知错误
            __android_log_print(ANDROID_LOG_ERROR,TAG,"JNI-reg:reg fali!! reason %s\n", reason.c_str());

        }

        return JNI_FALSE;
    }


    return JNI_TRUE;

}



}