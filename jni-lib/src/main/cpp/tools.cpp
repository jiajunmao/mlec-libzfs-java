#include <iostream>
#include "jni_Tools.h"

JNIEXPORT jstring JNICALL Java_jni_Tools_bar(JNIEnv *env, jobject thisObject)
{
  std::string res("bar");
  return env->NewStringUTF(res.c_str());
}

// Utility function to create a new DnodeAttributes object and set its fields
jobject createDnodeAttributes(JNIEnv *env, jstring path) {
    // Find the DnodeAttributes class
    jclass dnodeClass = env->FindClass("jni/DnodeAttributes");
    if (dnodeClass == NULL) {
        return NULL; // Class not found
    }

    // Get the constructor method ID for DnodeAttributes
    jmethodID constructor = env->GetMethodID(dnodeClass, "<init>", "()V");
    if (constructor == NULL) {
        return NULL; // Constructor not found
    }

    // Create a new DnodeAttributes object
    jobject dnodeObj = env->NewObject(dnodeClass, constructor);
    if (dnodeObj == NULL) {
        return NULL; // Object creation failed
    }

    // Get the field ID for the path field
    jfieldID pathField = env->GetFieldID(dnodeClass, "path", "Ljava/lang/String;");
    if (pathField == NULL) {
        return NULL; // Field not found
    }

    // Set the path field with the given path
    env->SetObjectField(dnodeObj, pathField, path);

    return dnodeObj;
}

JNIEXPORT jobject JNICALL Java_jni_Tools_getFailedChunks
  (JNIEnv *env, jobject thisObj, jstring poolName, jint objsetId) {
    // Find the ArrayList class
    jclass arrayListClass = env->FindClass("java/util/ArrayList");
    if (arrayListClass == NULL) {
        return NULL; // Class not found
    }

    // Get the constructor method ID for ArrayList
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    if (arrayListConstructor == NULL) {
        return NULL; // Constructor not found
    }

    // Create a new ArrayList object
    jobject arrayListObj = env->NewObject(arrayListClass, arrayListConstructor);
    if (arrayListObj == NULL) {
        return NULL; // Object creation failed
    }

    // Get the add method ID for ArrayList
    jmethodID arrayListAdd = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    if (arrayListAdd == NULL) {
        return NULL; // Method not found
    }

    // Create a new DnodeAttributes object and set its path field
    jobject dnodeObj = createDnodeAttributes(env, poolName);
    if (dnodeObj == NULL) {
        return NULL; // Object creation failed
    }

    // Add the DnodeAttributes object to the ArrayList
    env->CallBooleanMethod(arrayListObj, arrayListAdd, dnodeObj);

    // Return the ArrayList
    return arrayListObj;
}