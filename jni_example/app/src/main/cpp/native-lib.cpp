#include <jni.h>
#include <string>

#include "Example.h"
#include "HybridObject.h"

static jclass jclassMainActivity;
static jmethodID jmethodApplyPrefix;
static jmethodID jmethodApplySuffix;

// Use extern "C" to inhibit name mangling in C++, so C code can get the function name.
// JNICALL contains compiler directives to compile the code. On Android it's probably empty.
extern "C" JNIEXPORT void JNICALL
Java_com_example_jniexample_MainActivity_callNativeTheOldWay(JNIEnv *env, jobject /* this */) {
}

// Function using RegisterNatives() doesn't need extern "C" because it's registered entirely in C++.
JNIEXPORT void JNICALL
CallNativeTheNewWay(JNIEnv *env, jobject /* this */) {
}

JNIEXPORT void JNICALL
CallNativeWithStaticMethod(JNIEnv *env, jclass /* this */) {
}

JNIEXPORT jstring JNICALL
GetStringFromJni(JNIEnv *env, jobject mainActivity /* this */, jstring name) {
    // Use JNI_TRUE or JNI_FALSE for jboolean
    const char *chars = env->GetStringUTFChars(name, JNI_FALSE);
    std::string nameStr(chars);
    // Always pair GetStringUTFChars() with a ReleaseStringUTFChars().
    env->ReleaseStringUTFChars(name, chars);

    // Don't need to call any clean function because JVM will clean it when this function call
    // finish.
    jstring jstringNewName = env->NewStringUTF(nameStr.c_str());

    auto result = (jstring) env->CallObjectMethod(
            mainActivity,
            jmethodApplyPrefix,
            jstringNewName);

    result = (jstring) env->CallStaticObjectMethod(
            // We can either use env->GetObjectClass on the instance
            // or use cached jclassMainActivity above.
            env->GetObjectClass(mainActivity),
            jmethodApplySuffix,
            result);

    return result;
}

// JNIEXPORT is for adding methods to dynamic table of .so file.
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
    JNIEnv *env;

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass cls = env->FindClass("com/example/jniexample/MainActivity");

    if (cls == nullptr) {
        return JNI_ERR;
    }

    static const JNINativeMethod methods[] = {
            {
                    "callNativeTheNewWay",
                    "()V",
                    reinterpret_cast<void *>(CallNativeTheNewWay),
            },
            {
                    "callNativeWithStaticMethod",
                    "()V",
                    reinterpret_cast<void *>(CallNativeWithStaticMethod),
            },
            {
                    "callNativeStaticMethodWithStaticMethod",
                    "()V",
                    reinterpret_cast<void *>(Example::CallNativeStaticMethodWithStaticMethod),
            },
            {
                    "getStringFromJni",
                    "(Ljava/lang/String;)Ljava/lang/String;",
                    reinterpret_cast<void *>(GetStringFromJni),
            },
    };

    int resultCode = env->RegisterNatives(cls, methods, sizeof(methods) / sizeof(JNINativeMethod));

    if (resultCode != JNI_OK) {
        return resultCode;
    }

    // Cached methods, so we can call it later.
    // Also raise any error early if method cannot be found.

    jclassMainActivity = reinterpret_cast<jclass>(env->NewGlobalRef(cls));
    jmethodApplyPrefix = env->GetMethodID(
            jclassMainActivity,
            "applyPrefix",
            "(Ljava/lang/String;)Ljava/lang/String;");
    jmethodApplySuffix = env->GetStaticMethodID(
            jclassMainActivity,
            "applySuffix",
            "(Ljava/lang/String;)Ljava/lang/String;");

    // Advanced example

    if ((resultCode = HybridObject::JniRegister(env)) != JNI_OK) {
        return resultCode;
    }

    return JNI_VERSION_1_6;
}
