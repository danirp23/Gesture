# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# enable C++ 11
# set (CMAKE_CXX_STANDARD 11)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             afrecog

             # Sets the library as a shared library.
             SHARED

             # file are automatically included.
             src/main/cpp/FImage.cpp
             src/main/cpp/FImageRect.cpp
             src/main/cpp/FImageContext.cpp
             src/main/cpp/Detector.cpp
             src/main/cpp/HaarDetector.cpp
             src/main/cpp/HoGDetector.cpp
             src/main/cpp/Recognizer.cpp
             src/main/cpp/LBPHRecognizer.cpp
             src/main/cpp/AFContext.cpp
             src/main/cpp/Puntos.cpp
             src/main/jni/jniAdapter.cpp )

#opencv location
include_directories(opencv/include)
link_directories(opencv/libs/${ANDROID_ABI})
link_directories(opencv/3rdparty/libs/${ANDROID_ABI})

#dlib location
include_directories(dlib/include)
link_directories(dlib/libs/${ANDROID_ABI})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

#android graphics
find_library( # Sets the name of the path variable.
              jnigraphics-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              jnigraphics )

find_library( # Sets the name of the path variable.
              android-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              android )

file(GLOB PARTY_LIBS "opencv/3rdparty/libs/${ANDROID_ABI}/*.a")
file(GLOB CV_LIBS  "opencv/libs/${ANDROID_ABI}/*.a")
file(GLOB DLIB_LIBS  "dlib/libs/${ANDROID_ABI}/*.a")

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       afrecog

                       ${CV_LIBS}
                       ${PARTY_LIBS}
                       ${CV_LIBS}
                       ${DLIB_LIBS}
                       ${log-lib}
                       ${android-lib}
                       ${jnigraphics-lib} )

add_custom_command(TARGET afrecog POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy
         ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libafrecog.so
         ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libafrecog.so
         )

add_custom_command(TARGET afrecog POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy
         ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include/FImage.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/FImage.h
         )

add_custom_command(TARGET afrecog POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy
         ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include/FImageContext.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/FImageContext.h
         )

add_custom_command(TARGET afrecog POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy
         ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include/FImageRect.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/FImageRect.h
         )

add_custom_command(TARGET afrecog POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy
         ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include/Detector.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/Detector.h
         )

add_custom_command(TARGET afrecog POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy
         ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include/HaarDetector.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/HaarDetector.h
         )

add_custom_command(TARGET afrecog POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy
         ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include/HoGDetector.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/HoGDetector.h
         )

add_custom_command(TARGET afrecog POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy
         ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include/Recognizer.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/Recognizer.h
         )

add_custom_command(TARGET afrecog POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy
         ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include/Puntos.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/Puntos.h
         )

