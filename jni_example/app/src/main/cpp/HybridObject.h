#pragma once

#include <jni.h>

class HybridObject {
public:
    static jint JniRegister(JNIEnv *env);

    static void Init(JNIEnv *env, jobject, jint id);

    HybridObject(int id);

private:
    static void SayId(JNIEnv *env, jobject hybridObject);

    int id_;

    void SayIdInstance() const;
};
