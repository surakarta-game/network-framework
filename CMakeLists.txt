cmake_minimum_required(VERSION 3.16)

project(network-framework)
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
add_subdirectory(third-party/nlohmann_json)

if(NOT TARGET network-framework)
    SET(SOURCE
        src/exceptions.cpp
        src/client.cpp
        src/server.cpp
    )
    if(WIN32)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
    add_library(network-framework SHARED ${SOURCE})
    target_include_directories(network-framework
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/private-include
    )
    target_link_libraries(network-framework PRIVATE sockpp)
    target_link_libraries(network-framework PRIVATE nlohmann_json)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(network-framework PRIVATE -Wall -Wextra)
    endif()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(network-framework PRIVATE /W4 /w14640)
    endif()
endif()

if(NOT TARGET network-framework-test)
    SET(TEST_SOURCE
        src/test.cpp
    )
    add_executable(network-framework-test ${TEST_SOURCE})
    target_link_libraries(network-framework-test PRIVATE network-framework)
    add_test(NAME network-framework-test COMMAND network-framework-test)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(network-framework-test PRIVATE -Wall -Wextra)
    endif()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(network-framework-test PRIVATE /W4 /w14640)
    endif()
endif()
