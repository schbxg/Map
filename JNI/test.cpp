//
// Created by TOM on 2017/8/5.
//
#include <OBOjni.h>

void test_hello()
{
    __android_log_print(ANDROID_LOG_ERROR, "testjnji", "JNI: hello jni");
    return;
}

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_tom_obo_OBOJNI
 * Method:    test_hello
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_tom_obo_OBOJNI_test_1hello
  (JNIEnv * env, jobject obj)
  {
        test_hello();
  }

/*
 * Class:     com_example_tom_obo_OBOJNI
 * Method:    test_hello1
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_tom_obo_OBOJNI_test_1hello1
        (JNIEnv * env, jobject obj)
{
    __android_log_print(ANDROID_LOG_ERROR, "testjnji", "JNI: hello1 jni!!!");
}

/*
 * Class:     com_example_tom_obo_OBOJNI
 * Method:    test
 * Signature: (II)V
 */
JNIEXPORT jint JNICALL Java_com_example_tom_obo_OBOJNI_test
        (JNIEnv * env, jobject obj, jint j_a, jint j_b)
{
    int a = (int) j_a;
    int b = (int) j_b;
    __android_log_print(ANDROID_LOG_ERROR, "testjnji", "JNI: a = %d, b = %d", a, b);

    int c = 20;
    return c;
}

/*
 * Class:     com_example_tom_obo_OBOJNI
 * Method:    test_jni_api
 * Signature: (Z)Z
 */
JNIEXPORT jboolean JNICALL Java_com_example_tom_obo_OBOJNI_test_1jni_1api
        (JNIEnv *env, jobject obj, jboolean j_bool_a)
{
    bool arg_bool = (j_bool_a == JNI_TRUE) ? true : false;

    __android_log_print(ANDROID_LOG_ERROR, "testjni", "JNI: bool = %s",
                        ((arg_bool == true) ? "true" : "false"));

    return (arg_bool == true) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     com_example_tom_obo_OBOJNI
 * Method:    test_jni_api1
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_tom_obo_OBOJNI_test_1jni_1api1
        (JNIEnv *env, jobject obj, jstring j_str1, jstring j_str2)
{
    //jString --> Char
    const char *c_str1 = NULL;
    const char* c_str2 = NULL;

    //将java中字符串转化成char*类型
    c_str1 = env->GetStringUTFChars(j_str1, 0);

    __android_log_print(ANDROID_LOG_ERROR, "testjni", "JNI:c_str = %s", c_str1);

    //释放java传递过来的string里面的在堆上开辟的字符控件
    env->ReleaseStringUTFChars(j_str1, c_str1);

    //将java中字符串转化成char*类型
    c_str2 = env->GetStringUTFChars(j_str2, 0);

    __android_log_print(ANDROID_LOG_ERROR, "testjni", "JNI:c_str = %s", c_str2);

    //释放java传递过来的string里面的在堆上开辟的字符控件
    env->ReleaseStringUTFChars(j_str2, c_str2);

    //给java返回一个字符串
    jstring ret_j_string = env->NewStringUTF("JNI return string");
    return ret_j_string;
}

/*
 * Class:     com_example_tom_obo_OBOJNI
 * Method:    test_jni_array_api
 * Signature: ([I)V
 */
JNIEXPORT void JNICALL Java_com_example_tom_obo_OBOJNI_test_1jni_1array_1api
        (JNIEnv *env, jobject obj, jintArray j_int_array)
{
    //获取java中j_int_array数组的首地址
    jint *pia = env->GetIntArrayElements(j_int_array, 0);
    //的到数组长度
    jsize array_len = env->GetArrayLength(j_int_array);

    //使用改pia pia[1]
    for(int i = 0; i < array_len; i++)
    {
        __android_log_print(ANDROID_LOG_ERROR, "testjni", "JNI:array[%d]:%d", i, (int)pia[i]);

    }
    //释放j_int_array空间
    env->ReleaseIntArrayElements(j_int_array, pia, 0);

}


#ifdef __cplusplus
}
#endif

