#include <jni.h>

extern "C" {
void native_itf_source(JNIEnv *env, jobject *obj, jstring url) {
    char *p = (char *) env->GetStringUTFChars(url, nullptr);
}

//jni auto register
const char *JNI_Class[] = {
        "tv/itf/media/NativeItfMedia"
};
JNINativeMethod JNI_Methods[] = {
        {"native_itf_source", "(Ljava/lang/String;)V", (void *) native_itf_source}
};
#define JNI_LENGTH(n) (sizeof(n) / sizeof(n[0]))
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    jclass clazz = env->FindClass(JNI_Class[0]);
    if (env->RegisterNatives(clazz, JNI_Methods, JNI_LENGTH(JNI_Methods)) != JNI_OK) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}
}