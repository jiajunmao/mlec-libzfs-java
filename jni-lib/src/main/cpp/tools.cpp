#include <iostream>
#include "jni_Tools.h"

JNIEXPORT jstring JNICALL Java_jni_Tools_bar(JNIEnv *env, jobject thisObject)
{
  std::string res("bar");
  return env->NewStringUTF(res.c_str());
}

JNIEXPORT jobject JNICALL Java_jni_Tools_getFailedChunks
  (JNIEnv *, jobject, jstring, jint) {

}