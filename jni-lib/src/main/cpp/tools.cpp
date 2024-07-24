#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <libzfs_core.h>
#include <libzfs/sys/nvpair.h>   
#include "jni_Tools.h"

JNIEXPORT jstring JNICALL Java_jni_Tools_bar(JNIEnv *env, jobject thisObject)
{
  std::string res("bar");
  return env->NewStringUTF(res.c_str());
}

int mlec_libzfs_all_failed_chunks(const char *pool, uint64_t objset, nvlist_t **outnvl) {
    if (libzfs_core_init() != 0) {
        fprintf(stderr, "Can't init libzfs\n");
        return 101;
    }

    nvlist_t *innvl;
    if (nvlist_alloc(&innvl, NV_UNIQUE_NAME, 0) != 0) {
        fprintf(stderr, "Failed to allocate nvlist\n");
        return EXIT_FAILURE;
    }

    // Prep the input
    nvlist_add_uint64(innvl, "objset_id", objset);

    int ret = lzc_mlec_get_failed_chunks(pool, innvl, outnvl);
    if (ret) {
        printf("Error while calling get failed chunks %d\n", ret);
        return -1;
    }

    nvlist_free(innvl);
    libzfs_core_fini();
    return 0;
}

int get_from_nvlist_set_field_int64_t(JNIEnv *env, jclass dnodeClass, jobject dnodeObj, nvlist_t *attributes, const char *field_name, const char *nvlist_key) {
      // Get the type, integer
    jfieldID typeField = env->GetFieldID(dnodeClass, field_name, "I");
    if (typeField == NULL) {
        return -1; // Field not found
    }
    int64_t type;
    nvlist_lookup_int64(attributes, nvlist_key, &type);
    // Set the type field with the given type
    env->SetIntField(dnodeObj, typeField, type);

    return type;
}

jobject intToInteger(JNIEnv *env, int value) {
    // Find the Integer class
    jclass integerClass = env->FindClass("java/lang/Integer");
    if (integerClass == NULL) {
        return NULL; // Class not found
    }

    // Get the constructor method ID for Integer(int value)
    jmethodID constructor = env->GetMethodID(integerClass, "<init>", "(I)V");
    if (constructor == NULL) {
        return NULL; // Constructor not found
    }

    // Create a new Integer object
    jobject integerObj = env->NewObject(integerClass, constructor, value);
    if (integerObj == NULL) {
        return NULL; // Object creation failed
    }

    return integerObj;
}

// Utility function to create a new DnodeAttributes object and set its fields
jobject createDnodeAttributes(JNIEnv *env, nvlist_t *attributes) {
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

    /**
     * Populate fields
     */

    // Get the field ID for the path field
    jfieldID pathField = env->GetFieldID(dnodeClass, "path", "Ljava/lang/String;");
    if (pathField == NULL) {
        return NULL; // Field not found
    }
    // Get the path attribute
    char *path;
    nvlist_lookup_string(attributes, "path", &path);
    jstring j_path = env->NewStringUTF(path);
    // Set the path field with the given path
    env->SetObjectField(dnodeObj, pathField, j_path);

    
    get_from_nvlist_set_field_int64_t(env, dnodeClass, dnodeObj, attributes, "objset", "objset");
    get_from_nvlist_set_field_int64_t(env, dnodeClass, dnodeObj, attributes, "object", "object");
    get_from_nvlist_set_field_int64_t(env, dnodeClass, dnodeObj, attributes, "type", "type");
    get_from_nvlist_set_field_int64_t(env, dnodeClass, dnodeObj, attributes, "fsize", "fsize");
    get_from_nvlist_set_field_int64_t(env, dnodeClass, dnodeObj, attributes, "numStripes", "num_stripes");
    get_from_nvlist_set_field_int64_t(env, dnodeClass, dnodeObj, attributes, "numRemainderStripes", "num_remainder_stripes");
    int64_t dcols = get_from_nvlist_set_field_int64_t(env, dnodeClass, dnodeObj, attributes, "dcols", "dcols");
    get_from_nvlist_set_field_int64_t(env, dnodeClass, dnodeObj, attributes, "nparity", "nparity");

    int64_t *child_status;
    // Get the childStatus field, list of integer
    jfieldID childStatusField = env->GetFieldID(dnodeClass, "childStatus", "Ljava/util/List;");
    if (childStatusField == NULL) {
        return NULL; // Field not found
    }
    // Get the childStatus attribute
    unsigned int nelem;
    nvlist_lookup_int64_array(attributes, "child_status", &child_status, &nelem);
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

    for (int i = 0; i < dcols; i++) {
      env->CallBooleanMethod(arrayListObj, arrayListAdd, intToInteger(env, child_status[i]));
    }

    // Set the childStatus field with the given childStatus
    env->SetObjectField(dnodeObj, childStatusField, arrayListObj);

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

    /**
     * MLEC logics here
     */
    const char *cPoolName = env->GetStringUTFChars(poolName, NULL);

    nvlist_t *outnvl;
    if (mlec_libzfs_all_failed_chunks(cPoolName, objsetId, &outnvl)) {
      nvlist_free(outnvl);
      printf("Error calling libzfs failed chunks\n");
      return NULL;
    }
    
    nvpair_t *pair = NULL;
    while ((pair = nvlist_next_nvpair(outnvl, pair)) != NULL) {
        printf("dnode %s\n", nvpair_name(pair));

        // Allocate a new dnode attribute
        nvlist_t *attributes;
        nvlist_alloc(&attributes, NV_UNIQUE_NAME, 0);
        nvpair_value_nvlist(pair, &attributes);
        jobject dnodeObj = createDnodeAttributes(env, attributes);
        if (dnodeObj == NULL) {
          return NULL; // Object creation failed
        }
        nvlist_free(attributes);

        // Add the DnodeAttributes object to the ArrayList
        env->CallBooleanMethod(arrayListObj, arrayListAdd, dnodeObj);
    }

    // Return the ArrayList
    return arrayListObj;
}