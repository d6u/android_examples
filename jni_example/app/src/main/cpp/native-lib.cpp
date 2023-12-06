#include <jni.h>
#include <string>

extern "C" JNIEXPORT void JNICALL
Java_com_example_jniexample_MainActivity_callNativeTheOldWay(JNIEnv *env, jobject /* this */) {
}

extern "C" JNIEXPORT void JNICALL
CallNativeTheNewWay(JNIEnv *env, jobject /* this */) {
}

extern "C" JNIEXPORT void JNICALL
CallNativeWithStaticMethod(JNIEnv *env, jclass /* this */) {
}

extern "C" JNIEXPORT jstring JNICALL
GetStringFromJni(JNIEnv *env, jobject /* this */, jstring name) {
    // Use JNI_TRUE or JNI_FALSE for jboolean
    const char *chars = env->GetStringUTFChars(name, JNI_FALSE);

    std::string hello("Hello, ");
    hello += chars;

    // Always pair GetStringUTFChars() with a ReleaseStringUTFChars().
    env->ReleaseStringUTFChars(name, chars);

    return env->NewStringUTF(hello.c_str());
}

// JNIEXPORT is for adding methods to dynamic table of .so file.
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
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
                    "getStringFromJni",
                    "(Ljava/lang/String;)Ljava/lang/String;",
                    reinterpret_cast<void *>(GetStringFromJni),
            },
    };

    int resultCode = env->RegisterNatives(cls, methods, sizeof(methods) / sizeof(JNINativeMethod));

    if (resultCode != JNI_OK) {
        return resultCode;
    }

    return JNI_VERSION_1_6;
}
