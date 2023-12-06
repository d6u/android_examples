#include "HybridObject.h"

#include <android/log.h>

#include <memory>
#include <unordered_map>

static std::unordered_map<int, std::shared_ptr<HybridObject>> map;
jmethodID jmethodIdGetId;

// static
jint HybridObject::JniRegister(JNIEnv *env) {
    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass cls = env->FindClass("com/example/jniexample/HybridObject");

    if (cls == nullptr) {
        return JNI_ERR;
    }

    static const JNINativeMethod methods[] = {
            {
                    "init",
                    "(I)V",
                    reinterpret_cast<void *>(HybridObject::Init),
            },
            {
                    "sayId",
                    "()V",
                    reinterpret_cast<void *>(HybridObject::SayId),
            },
    };

    int resultCode = env->RegisterNatives(cls, methods, sizeof(methods) / sizeof(JNINativeMethod));

    if (resultCode != JNI_OK) {
        return resultCode;
    }

    jmethodIdGetId = env->GetMethodID(cls, "getId", "()I");

    return JNI_VERSION_1_6;
}

// static
void HybridObject::Init(JNIEnv *env, jobject hybridObject, jint id) {
    map[id] = std::make_shared<HybridObject>(id);
}

// static
void HybridObject::SayId(JNIEnv *env, jobject hybridObject) {
    int id = env->CallIntMethod(hybridObject, jmethodIdGetId);
    map[id]->SayIdInstance();
}

HybridObject::HybridObject(int id) : id_(id) {
}

void HybridObject::SayIdInstance() const {
    __android_log_print(
            ANDROID_LOG_DEBUG, "JniExample", "My ID is %d", id_);
}


