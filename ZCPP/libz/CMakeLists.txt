cmake_minimum_required(VERSION 3.1.12)
project(zfile VERSION 1.0.0)

set(LIBRARY_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_C_STANDARD 17)

if(ANDROID)
    message("This is an Android platform.")
    add_definitions(-DUNITY_ANDROID)
else()
    message("This is not an Android platform.")
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
endif()

add_library(${LIBRARY_NAME} SHARED zfile.cpp)


include_directories(${CMAKE_SOURCE_DIR}/lz4/lib)


target_link_libraries(${LIBRARY_NAME} log android lz4_static) # unwindstack


add_executable(main zfile.cpp main.cpp)
target_link_libraries(main lz4_static)

add_subdirectory(lz4)
# if(CMAKE_BUILD_TYPE STREQUAL "Release")
#     add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD
#         COMMAND ${CMAKE_STRIP} $<TARGET_FILE:track>
#         COMMENT "Stripping debug symbols from $<TARGET_FILE:track>"
#     )
# endif()


# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     target_compile_options(${LIBRARY_NAME} PUBLIC -fsanitize=address -fno-omit-frame-pointer)
#     target_link_options(${LIBRARY_NAME} PUBLIC -fsanitize=address)
# endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${LIBRARY_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:zfile> ../../../Assets/FastZBin/Plugins/Android/${ANDROID_ABI}/libzfile.so
        COMMENT "Copy $<TARGET_FILE:track>"
    )
endif()