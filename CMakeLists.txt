cmake_minimum_required(VERSION 3.16)

project(network-common)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add_compile_options(-fsanitize=address)
# add_compile_options(-g)
# add_link_options(-fsanitize=address)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Do not enable -Wall -Wextra due to the third-party code
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # To disable a false positive warning in libsockpp
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-stringop-truncation")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Do not enable -Wall -Wextra due to the third-party code
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /w14640")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
endif()

add_subdirectory(third-party/sockpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third-party/sockpp/include)

if(NOT TARGET network-common)
    SET(SOURCE
        src/exceptions.cpp
        src/client.cpp
        src/server.cpp
    )
    add_library(network-common SHARED ${SOURCE})
    target_link_libraries(network-common sockpp)
    if(WIN32)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
endif()

if(NOT TARGET network-common-test)
    SET(TEST_SOURCE
        src/test.cpp
    )
    add_executable(network-common-test ${TEST_SOURCE})
    target_link_libraries(network-common-test network-common)
    add_test(NAME network-common-test COMMAND network-common-test)
endif()
