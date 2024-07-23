add_library(tools SHARED "")

# Manually specify the include directories and libraries
set(LIBZFS_INCLUDE_DIR /usr/local/include/libzfs)
set(LIBSPL_INCLUDE_DIR /usr/local/include/libspl)

set(LIBZFS_LIBRARY /usr/local/lib/libzfs.so)
set(LIBZFS_CORE_LIBRARY /usr/local/lib/libzfs_core.so)
set(LIBNVPAIR_LIBRARY /usr/local/lib/libnvpair.so)

# Add the include directories to the include path
include_directories(${LIBZFS_INCLUDE_DIR} ${LIBSPL_INCLUDE_DIR})

target_sources(tools PRIVATE tools.cpp)
target_link_libraries(tools ${LIBZFS_LIBRARY} ${LIBZFS_CORE_LIBRARY} ${LIBNVPAIR_LIBRARY})

