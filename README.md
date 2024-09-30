## Introduction
This repo is the Java binding that calls ZFS `libzfs.so` through JNI(Java Native Interface).

## Requirements
- Java 11
- You would need to compile and install ZFS on the host machine first (otherwise gradle is going to complain not being able to find the C linked headers)

## To Compile and Build
- Run `./gradlew build`.
  - This will produce the JAR artifact under `jni-lib/build/libs/jni-lib.jar`
- Run `./gradlew publishToMavenLocal`
  - This will publish the above artifact to the `~/.m2/repo` directory, which is the local cache for Maven dependency management, the build system that Hadoop uses

## Content Structure
```
.
└── jni-lib/
    └── src/
        └── main/
            ├── cpp/
            │   ├── tools.cpp:/
            │   │   ├── C code that link against `libzfs.h`, call ZFS's ioctl adapters.
            │   │   └── Helper functions for nvlist serialization and deserialization
            │   └── CMakeLists.txt/
            │       └── CMake file that link against `libzfs` and `libspl`
            └── java/
                ├── DnodeAttributes.java/
                │   └── Wrapper class used for data transfer at the JNI layer between Java and C
                ├── LibraryLoader.java/
                │   └── No need to change. JNI boiler plate.
                └── Tools.Java/
                    └── Java wrapper for ZFS ioctl calls.
```