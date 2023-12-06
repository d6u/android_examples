#pragma once

#include <jni.h>

class Example {
public:
    static void CallNativeStaticMethodWithStaticMethod(JNIEnv *env, jclass);
};
