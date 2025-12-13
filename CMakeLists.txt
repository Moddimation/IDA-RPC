cmake_minimum_required(VERSION 3.5)
project(ida_discord_rpc CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED ENV{IDASDK})
    message(FATAL_ERROR "The IDASDK environment variable must be set to the path of the IDA SDK.")
endif()

set(IDASDK_PATH $ENV{IDASDK})
set(IDASDK_INCLUDE_DIR "${IDASDK_PATH}/include")
set(IDASDK_LIB "${IDASDK_PATH}/lib/x64_linux_gcc_64/libida.so")

include_directories(${IDASDK_INCLUDE_DIR})

set(DISCORD_SDK_ROOT "${CMAKE_SOURCE_DIR}/lib/discord_game_sdk")

set(DISCORD_SDK_C_INCLUDE_DIR "${DISCORD_SDK_ROOT}/c")
set(DISCORD_SDK_CPP_INCLUDE_DIR "${DISCORD_SDK_ROOT}/cpp")

set(DISCORD_SDK_LIBRARY "discord_game_sdk.soos")

add_library(DiscordGameSDK SHARED IMPORTED)
set_target_properties(DiscordGameSDK PROPERTIES
    IMPORTED_LOCATION "${DISCORD_SDK_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${DISCORD_SDK_C_INCLUDE_DIR}"
)

set(IDA_PLUGIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ida_plugins")

file(GLOB_RECURSE DISCORD_SDK_SOURCES "${DISCORD_SDK_ROOT}/cpp/*.cpp")
file(GLOB_RECURSE DISCORD_RPC_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_library(ida_discord_rpc SHARED
    ${DISCORD_RPC_SOURCES}
    ${DISCORD_SDK_SOURCES}
)

target_include_directories(ida_discord_rpc PRIVATE
    ${DISCORD_SDK_C_INCLUDE_DIR}
    ${DISCORD_SDK_CPP_INCLUDE_DIR}
    src
)

target_compile_definitions(ida_discord_rpc PRIVATE
    __LINUX__
)
target_compile_options(ida_discord_rpc PRIVATE
     -g
)
target_link_options(ida_discord_rpc PRIVATE
     -g
)
target_link_libraries(ida_discord_rpc PRIVATE
    "${IDASDK_LIB}"
    # FIX 3: Link against the new Imported Target name
    DiscordGameSDK
)

set_target_properties(ida_discord_rpc PROPERTIES
    BUILD_WITH_INSTALL_RPATH FALSE
    INSTALL_RPATH ""
    BUILD_RPATH ""
    SKIP_BUILD_RPATH TRUE
    RUNTIME_OUTPUT_DIRECTORY ${IDA_PLUGIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${IDA_PLUGIN_OUTPUT_DIR}
    OUTPUT_NAME "ida_discord_rpc"
    SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}"
)
