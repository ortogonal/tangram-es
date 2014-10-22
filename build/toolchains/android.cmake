if(NOT DEFINED ANDROID_ABI)
    set(ANDROID_ABI "armeabi-v7a")
    message(STATUS "Possible ABI values : armeabi-v7a (default), armeabi, x86")
endif()
message(STATUS "Android ABI : ${ANDROID_ABI}") 

if(NOT DEFINED ANDROID_TARGET)
    set(ANDROID_TARGET "android-19")
endif()
message(STATUS "Android target : ${ANDROID_TARGET}")

include(${CMAKE_SOURCE_DIR}/build/toolchains/android.toolchain.cmake)

add_definitions(-DPLATFORM_ANDROID)

if(NOT MAKE_BUILD_TOOL)
    message(SEND_ERROR "Provide ndk make, located in <NDK_ROOT>/prebuilt/<OS>/bin/, -DMAKE_BUILD_TOOL=<NDK_PREBUILT_BIN>/make")
    return()
else()
    set(CMAKE_BUILD_TOOL ${MAKE_BUILD_TOOL})
    message(STATUS "Will use make prebuilt tool located at : ${CMAKE_BUILD_TOOL}")
endif()

# configurations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic -llog -lz")
set(CXX_FLAGS_DEBUG "${CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -fPIC")
set(LIBCURL_PRECOMPILED_LIB ${CMAKE_SOURCE_DIR}/android/jni/precompiled/${ANDROID_ABI}/libcurl.a)

# include dependency headers
include_directories(${PROJECT_SOURCE_DIR}/android/jni/include)

message(STATUS "Using curl precompiled static library : ${LIBCURL_PRECOMPILED_LIB}")

# adding manually the two jni c++ files
set(ADDITIONNAL_TARGET_DEPENDENT_SRC_FILES 
     ${CMAKE_SOURCE_DIR}/android/jni/jniExports.cpp
     ${CMAKE_SOURCE_DIR}/android/jni/platform_android.cpp)

# load core library
set(INSTALL_CORE_LIBRARY "ON")
set(CORE_LIB_TYPE SHARED)
set(CORE_INSTALLATION_PATH ${CMAKE_SOURCE_DIR}/android/libs/${ANDROID_ABI})
set(CORE_LIB_DEPS GLESv2 ${LIBCURL_PRECOMPILED_LIB})
set(CORE_LIB_NAME tangram) # in order to have libtangram.so

include_directories(${PROJECT_SOURCE_DIR}/core/include/)
add_subdirectory(${PROJECT_SOURCE_DIR}/core)
include_recursive_dirs(${PROJECT_SOURCE_DIR}/core/*.h)

# link and build functions
function(link_libraries)
    # nothing to do
endfunction()

function(build)
    # nothing to do
endfunction()